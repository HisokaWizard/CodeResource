'use strict';
(function() {
  var service = function(
    complanInfo,
    cacheService,
    $rootScope)
  {
    var _public = this,
      _private = {};
    //functions
    _private.checkEndpoints = function(sId, eId){
      return (undefined != _private.services &&
      undefined != _private.services[sId] &&
      undefined != _private.services[sId].endpoints);
    };
    _private.isDetached = function(sId, eId){
      return(!_private.checkEndpoints(sId, eId) ||
      (undefined != _private.services[sId].endpoints[eId].state &&_private.states.detached == _private.services[sId].endpoints[eId].state)
      );
    };
    _private.isException = function(sId, eId){
      return (undefined == _private.services[sId].endpoints[eId] ||
      undefined == _private.services[sId].endpoints[eId].state);
    };
    _private.isAttached = function(sId, eId){
      return _private.states.attached ==  _private.services[sId].endpoints[eId].state;
    };
    _private.isAttaching = function(sId, eId){
      return _private.states.attaching == _private.services[sId].endpoints[eId].state;
    };
    _private.isDetaching = function(sId, eId){
      return _private.states.detaching == _private.services[sId].endpoints[eId].state;
    };
    _private.isConnected = function(sId, eId){
      return _private.services[sId].endpoints[eId].connected;
    };
    _private.isRejected = function(sId, eId){
      return _private.states.rejected == _private.services[sId].endpoints[eId].state;
    };
    _public.getEndpointState = function(sId, eId){
      if(undefined == _private.services)
        return _private.states.exception;
      if(_private.isDetached(sId, eId))
        return _private.states.detached;
      if(_private.isException(sId, eId))
        return _private.states.exception;
      if(_private.isAttached(sId, eId))
        if(_private.isConnected(sId, eId))
          return _private.states.connected;
        else
          return _private.states.disconnected;
      if(_private.isAttaching(sId, eId))
        return _private.states.attaching;
      if(_private.isDetaching(sId, eId))
        return _private.states.detaching;
      if(_private.isRejected(sId, eId))
        return _private.states.rejected;
      return _private.states.exception;
    };
    $rootScope.$watch(
      function(){return cacheService.cache.bud.cp.services.state;},
      function(){
        if(undefined != cacheService.cache.bud.cp.services.state){
          _private.services = cacheService.cache.bud.cp.services.state;
        }
      }
    );
    //members
    _private.states = complanInfo.states;
    _private.services = undefined;
  };

  angular.module('app.complan').service('endpointsService', [
    'complanInfo',
    'cacheService',
    '$rootScope',
    service
  ]);
})();