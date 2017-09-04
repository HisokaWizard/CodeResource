'use strict';
(function() {
  var service = function(
    complanInfo,
    cacheService,
    servicesService)
  {
    var _public = this,
      _private = {};
    //functions
    _private.isLocked = function(sId){
      return (servicesService.checkState(sId)&&servicesService.checkMode(sId)&&
      undefined != _private.services[sId]["locked-modes"] &&
      true == _private.services[sId]["locked-modes"]);
    };
    _private.isActive = function(sId, mId){
      return (_private.states.formed  == servicesService.getServiceState(sId))&&
        (_private.services[sId].mode.current == mId ||
        _private.services[sId].mode.awaited == mId);
    };
    _private.isInactive = function(sId, mId){
      if(!(servicesService.checkState(sId)&&servicesService.checkMode(sId)&&servicesService.checkCurrent(sId)))
        return true;
      if(_private.services[sId].mode.current != mId)
        switch(servicesService.getServiceState(sId)){
          case  _private.states.unformed:
          case  _private.states.form:
          case  _private.states.unform:
          case  _private.states.formed:
            return true;
            break;
          default:
        }
      return false;
    };
    _private.isFailure = function(sId){
      return  _private.states.failure == servicesService.getServiceState(sId);
    };
    _public.getModeState = function(sId, mId){
      if(_private.isLocked(sId))
        return  _private.states.locked;
      if(_private.isActive(sId, mId))
        return  _private.states.active;
      if(_private.isInactive(sId, mId))
        return  _private.states.inactive;
      if(_private.isFailure(sId))
        return  _private.states.failure;
      return  _private.states.exception;
    };
    //members
    _private.states = complanInfo.states;
    _private.services = cacheService.cache.bud.cp.services.state;
  };

  angular.module('app.complan').service('modesService', [
    'complanInfo',
    'cacheService',
    'servicesService',
    service
  ]);
})();