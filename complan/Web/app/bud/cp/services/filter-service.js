'use strict';
(function() {
  var service = function(
    cacheService,
    servicesService,
    fulltextSearchService)
  {
    var _public = this,
      _private = {};
    //functions
    _public.getFilter = function(filter){
      return function(key){
        var match = true,
          service = cacheService.cache.bud.cp.services.data[key];
        if(undefined == filter)
          return;
        if(undefined != filter.label){
          if(-1 != fulltextSearchService.fullTextSearch(filter.label.toLowerCase(), service.label.toLowerCase()))
            match = true;
          else
            match = false;
          if(!match){
            var devices = _private.getDevices(key);
            for(var i in devices){
              if(-1 != fulltextSearchService.fullTextSearch(filter.label.toLowerCase(), devices[i].toLowerCase())){
                match = true;
                break;
              }
            }
          }
        }
        if(match && undefined != filter.encryption && service.crypted != filter.encryption){
          match = false;
        }
        if(match && undefined != filter.frequencyRange && service.range_id != filter.frequencyRange){
          match = false;
        }
        if(match && undefined != filter.state ){
          var state = servicesService.getServiceState(key);
          if(('formed' == state || 'unformed' == state) && state == filter.state)
            match = true;
          else
            match = false;
        }
        var filterHasEquipmentCriteria = false;
        for(var j in filter.endpoints)
          if(false != filter.endpoints[j]){
            filterHasEquipmentCriteria = true;
            break;
          }
        if(match && filterHasEquipmentCriteria && undefined != filter.endpoints){
          var endpoints = Object.keys(service.endpoints);
          match = false;
          for(var i in filter.endpoints){
            for(var j in endpoints){
              if(filter.endpoints[i] && i == endpoints[j]){
                match = true;
                break;
              }
            }
          }
        }
        return match;
      };
    };
    _private.getDevices = function(sId){
      var service = cacheService.cache.bud.cp.services.data[sId],
        device = cacheService.cache.device,
        list = [];
      for(var e in service.endpoints){
        list.push(device[e].label);
      }
      if(undefined != service.encrypted_id && "" != service.encrypted_id){
        if(undefined!==device[service.encrypted_id]){
          list.push(device[service.encrypted_id].label);
        }
      }
      if(undefined!==device[service['in'].antenna_id]) {
        list.push(device[service['in'].antenna_id].label);
      }
      if(undefined!==device[service['in'].radio.id]) {
        list.push(device[service['in'].radio.id].label);
      }
      if(undefined!==device[service['out'].antenna_id]) {
        list.push(device[service['out'].antenna_id].label);
      }
      if(undefined!==device[service['out'].radio.id]) {
        list.push(device[service['out'].radio.id].label);
      }
      return list;
    };
    //memebers
  };

  angular.module('app.complan').service('filterService', [
    'cacheService',
    'servicesService',
    'fulltextSearchService',
    service]);
})();