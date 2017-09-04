'use strict';
(function() {
  var service = function(
    complanInfo,
    cacheService)
  {
    var _public = this,
      _private = {};
    //functions
    _public.checkState = function(sId){
      return (undefined != _public.services &&
      '' != _public.services &&
      !Object.isEmpty(_public.services) &&
      undefined != _public.services[sId] &&
      !Object.isEmpty(_public.services[sId]));
    };
    _public.checkMode = function(sId){
      return (undefined != _public.services[sId].mode &&
      !Object.isEmpty(_public.services[sId].mode));
    };
    _public.checkCurrent = function(sId){
      return (undefined != _public.services[sId].mode.current &&
      !Object.isEmpty(_public.services[sId].mode.current));
    };
    _private.checkAwaited = function(sId){
      return (undefined != _public.services[sId].mode.awaited ||
      !Object.isEmpty(_public.services[sId].mode.awaited));
    };
    _private.isAwaitedEmpty = function(sId){
      return ("" == _public.services[sId].mode.awaited);
    };
    _private.isCurrentAwaitedEmpty = function(sId){
      return ("" == _public.services[sId].mode.awaited &&
      "" == _public.services[sId].mode.current);
    };
    _private.isModeStateEmpty = function(sId){
      return(undefined == _public.services[sId].mode.state ||
        null == _public.services[sId].mode.state ||
        Object.isEmpty(_public.services[sId].mode.state)
      );
    };
    _private.checkStateBuild = function(sId){
      return (undefined != _public.services[sId].mode.state.build &&
      !Object.isEmpty(_public.services[sId].mode.state.build));
    };
    _private.checkBuildDevices = function(sId){
      for(var i in _public.services[sId].mode.state.build)
        if(_private.states.accepted !=  _public.services[sId].mode.state.build[i])
          return false;
      return true;
    };
    _private.isBuildPending = function(sId){
      for(var i in _public.services[sId].mode.state.build)
        if(_private.states.pending == _public.services[sId].mode.state.build[i])
          return true;
      return false;
    };
    _private.isDestroyPending = function(sId){
      for(var i in _public.services[sId].mode.state.destroy)
        if(_private.states.pending == _public.services[sId].mode.state.destroy[i])
          return false;
      return true;
    };
    _private.isDestroyUndefined = function(sId){
      return (undefined == _public.services[sId].mode ||
      undefined == _public.services[sId].mode.state ||
      undefined == _public.services[sId].mode.state.destroy);
    };
    _private.isCurrentAwaitedEquality = function(sId){
      return (_public.services[sId].mode.awaited == _public.services[sId].mode.current
      && "" != _public.services[sId].mode.awaited);
    };
    _private.isDevicesRejected = function(sId){
      for(var i in _public.services[sId].mode.state.build)
        if(_private.states.rejected == _public.services[sId].mode.state.build[i])
          return true;
      return false;
    };
    _public.getServiceState = function(sId){
      if(!_public.checkState(sId))
        return _private.states.unformed;

      if(!_private.isModeStateEmpty(sId)
        &&
        !(_public.checkCurrent(sId) &&
        _private.checkAwaited(sId) &&
        _private.checkStateBuild(sId)))
        return _private.states.unformed;

      if(_private.isCurrentAwaitedEmpty(sId) &&
        (_private.isModeStateEmpty(sId) ||
        _private.checkBuildDevices(sId)))
        return _private.states.unformed;

      if(_private.isCurrentAwaitedEquality(sId))
        return _private.states.formed;

      if(_private.isBuildPending(sId))
        return _private.states.form;

      if(_private.isDevicesRejected(sId))
        return _private.states.failure;

      if(_private.isDestroyPending(sId) &&
        _private.isAwaitedEmpty(sId))
        return _private.states.unform;

      return _private.states.exception;
    };
    //memebers
    _private.states = complanInfo.states;
    _public.services = cacheService.cache.bud.cp.services.state;
  };

  angular.module('app.complan').service('servicesService', [
    'complanInfo',
    'cacheService',
    service
  ]);
})();