'use strict';
(function () {

  var service = function (cacheService) {
    var _public = this, _private ={};

    _public.cache = cacheService.cache;

    // структурка описывает статус объектов, в него входит 3 поля, цвета, пока что простейшие, потом буду это усложнять
    _public.State = {
      ok: {status: "ok", label: "Работоспособно", color: "green"},
      none: {status: "none", label: "Недоступно", color: "yellow"},
      fault: {status: "fault", label: "Неработоспособно", color: "red"},
      unknown: {status: "", label: "Не управляется", color: "grey"}
    };
    _public.isEmptyObject = function (obj) {
      if (undefined == obj) {
        return true;
      }
      for(var key in obj) {
        if(obj.hasOwnProperty(key))
          return false;
      }
      return true;
    };
    _private.isFault = function(deviceId){
      if(undefined != _public.cache.alarms &&
         undefined != _public.cache.alarms.devices &&
         undefined != _public.cache.alarms.devices[deviceId]){
        var device = _public.cache.alarms.devices[deviceId];
        if(!_public.isEmptyObject(device.components) || !_public.isEmptyObject(device.ports))
          return true;
      }
      return false;
    };
    _private.isNone = function(deviceId){
      if(undefined == _public.cache.devices.state ||
         undefined == _public.cache.devices.state[deviceId])
        return true;
      else
        return false;
    };
    _private.isOk = function(deviceId){
      return _public.cache.devices.state[deviceId].managed.value;
    };
    _private.isUnknown = function(deviceId){
      return !_public.cache.devices.state[deviceId].managed.value;
    };
    _public.getDeviceState = function (deviceId) {
      var state = undefined;
      if(_private.isFault(deviceId))
        state = _public.State.fault;
      if(undefined == state && _private.isNone(deviceId))
        state = _public.State.none;
      if(undefined == state && _private.isOk(deviceId))
        state = _public.State.ok;
      if(undefined == state && _private.isUnknown(deviceId))
        state = _public.State.unknown;
      return state;
    };

    _public.getComponentState = function (deviceID, componentID) {
      if(undefined !== _public.cache.alarms) {
        if ((undefined !== _public.cache.alarms.devices)) {
          if (undefined !== _public.cache.alarms.devices[deviceID]) {
            if (undefined !== _public.cache.alarms.devices[deviceID].components[componentID]) {
              return _public.State.fault;
            }
          }
        }
      }
      return _public.State.ok;
    };

    _public.getPortState = function (deviceID, portID) {
      if (undefined === _public.cache.alarms) {
        return _public.State.none;
      }
      if(undefined !== _public.cache.alarms.devices[deviceID]){
        if (undefined !== _public.cache.alarms.devices[deviceID].ports){
          if (undefined !== _public.cache.alarms.devices[deviceID].ports[portID]) {
            return _public.State.fault;
          } else {
            return _public.State.ok;
          }
        }
      }
      // не понятно что за идем дальше
      return _public.State.ok;
    };

    _public.getGroupState = function (groupID) {
      return _public.State.ok;
    };

    _public.getNetWorkState = function (networkID) {
      return _public.State.ok;
    };

    _public.getEdgeState = function (edgeID) {
      return _public.State.ok;
    };
  };

  angular.module('app.components.State').service('stateService', [
    'cacheService',
    service]);
})();
