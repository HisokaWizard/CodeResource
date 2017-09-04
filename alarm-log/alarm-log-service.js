'use strict';
(function() {

  var service = function(
    $rootScope,
    $http,
    alarmLogInfo,
    resourceService,
    cacheService)
  {
    var _public = this;
    _public.getUnacknowledgedCount = 0;
    _public.translate = function(state){return resourceService.translate(state);};

    $rootScope.$watch(function(){
      return cacheService.cache;
    }, function(){
      if(undefined !== cacheService.cache.alarms) {
        if (undefined !== cacheService.cache.alarms.devices) {
          _public.getUnacknowledgedCount = _public.getCount();
        }
      }
    }, true);

    _public.Delete = function(accident){
      var longKey = accident.$key;
      var indexBaks = longKey.indexOf('$');
      var deviceID = longKey.slice(0, indexBaks);
      var elementID = longKey.slice(indexBaks+1);
      var deletePathComponent = "/alarms/devices/"+deviceID+"/components/"+elementID;
      var deletePathPort = "/alarms/devices/"+deviceID+"/port/"+elementID;
      var deletePathBud = "/alarms/buds/"+longKey;
      $http['delete'](deletePathComponent)
        .success(function(data, status, headers, config){
          //console.log("DELETE SUCCESS", data, status, headers, config);
        })
        .error(function(data, status, headers, config){
          //console.log("DELETE ERROR", data, status, headers, config);
        });
      $http['delete'](deletePathPort)
        .success(function(data, status, headers, config){
          //console.log("DELETE SUCCESS", data, status, headers, config);
        })
        .error(function(data, status, headers, config){
          //console.log("DELETE ERROR", data, status, headers, config);
        });
      $http['delete'](deletePathBud)
        .success(function(data, status, headers, config){
          //console.log("DELETE SUCCESS", data, status, headers, config);
        })
        .error(function(data, status, headers, config){
          //console.log("DELETE ERROR", data, status, headers, config);
        });
    };

    _public.Apply= function(accident, devices){
      accident.confirmed.value = true;
      if(undefined !== devices[accident.routeId].components){
        devices[accident.routeId].components[accident.elemId].confirmed.value = true;
      }
      if(undefined !== devices[accident.routeId].ports){
        devices[accident.routeId].ports[accident.elemId].confirmed.value = true;
      }
    };

    _public.getAlarmsData = function(alarms, devicesData){
      _public.alarmsData = {};
      if(undefined !== alarms.buds){
        for(var bud in alarms.buds){
          _public.alarmsData[bud] = alarms.buds[bud];
          _public.alarmsData[bud].label = _public.alarmsData[bud].Bud.Value;
        }
      }
      if(undefined !== alarms.devices){
        for(var device in alarms.devices){
          for(var devicedata in devicesData){
            if(device === devicedata){
              if(undefined !== alarms.devices[device].components){
                for(var component in alarms.devices[device].components){
                  _public.alarmsData[device+'$'+component] = alarms.devices[device].components[component];
                  _public.alarmsData[device+'$'+component].label = devicesData[devicedata].label;
                  _public.alarmsData[device+'$'+component].routeType = devicesData[devicedata].type;
                  _public.alarmsData[device+'$'+component].routeId = devicedata;
                  _public.alarmsData[device+'$'+component].elemType = "Компонент";
                  _public.alarmsData[device+'$'+component].filterType = "component";
                  _public.alarmsData[device+'$'+component].elemId = component;
                  for(var deviceobj in devicesData){
                    if(deviceobj === device){
                      for(var componentobj in devicesData[deviceobj].components){
                        if(componentobj === component){
                          _public.alarmsData[device+'$'+component].elemLabel = devicesData[deviceobj].components[componentobj].label;
                        }
                      }
                    }
                  }
                }
              }
              if(undefined !== alarms.devices[device].ports){
                for(var port in alarms.devices[device].ports){
                  _public.alarmsData[device+'$'+port] = alarms.devices[device].ports[port];
                  _public.alarmsData[device+'$'+port].label = devicesData[devicedata].label;
                  _public.alarmsData[device+'$'+port].routeType = devicesData[devicedata].type;
                  _public.alarmsData[device+'$'+port].routeId = devicedata;
                  _public.alarmsData[device+'$'+port].elemType = "Порт";
                  _public.alarmsData[device+'$'+port].filterType = "port";
                  _public.alarmsData[device+'$'+port].elemId = port;
                  for(var deviceobj in devicesData){
                    if(deviceobj === device){
                      for(var portobj in devicesData[deviceobj].ports){
                        if(portobj === port){
                          _public.alarmsData[device+'$'+port].elemLabel = devicesData[deviceobj].ports[portobj].label;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      return _public.alarmsData;
    };

    _public.getCount = function(){
      var count = 0;
      if(undefined === cacheService.cache
        || undefined === cacheService.cache.alarms)
          return count;
      if(undefined !== cacheService.cache.alarms.devices){
        var devices = cacheService.cache.alarms.devices;
        for(var device in devices){
          if(undefined !== devices[device].components)
            for(var component in devices[device].components){
              if(!devices[device].components[component].confirmed.value){
                count++;
              }
              if(devices[device].components[component].confirmed.value){
                count;
              }
            }

          if(undefined !== devices[device].ports){
            for(var port in devices[device].ports){
              if(!devices[device].ports[port].confirmed.value){
                count++;
              }
              if(devices[device].ports[port].confirmed.value){
                count;
              }
            }
          }
        }
      }
      if(undefined !== cacheService.cache.alarms.buds){
        var buds = cacheService.cache.alarms.buds;
        for(var bud in buds){
          count++;
        }
      }
      return count;
    };

    _public.getFilter = function(label, accident, filter, fulltextSearchService){
      if(undefined !== accident){
        var match = true,
          types = filter.types;
        if((undefined !== accident)
          && (undefined !== label)
          && ("" !== accident.label)
          && ("" !== label)){
          if(-1 !== fulltextSearchService.fullTextSearch(label.toLowerCase(), accident.label.toLowerCase())){
            match = true;
          }
          else{
            match = false;
          }
        }
        var filterHasTypeCriteria = false;
        for (var type1 in types){
          if (false != types[type1]) {
            filterHasTypeCriteria = true;
            break;
          }
        }
        if (match && filterHasTypeCriteria && undefined != types) {
          match = false;
          for (var type2 in types) {
            if (types[type2] && accident.filterType == type2) {
              match = true;
              break;
            }
          }
        }
        return match;
      }
    };

    _public.getAlarmsProperty = function(alarms, devicesData){
      if(0 != Object.size(alarms)){
        for(var alarm in alarms){
          for(var device in devicesData){
            if(undefined != alarms[alarm].Device){
              if(alarms[alarm].Device.Value === device){
                alarms[alarm].label = devicesData[device].label;
                alarms[alarm].routeType = devicesData[device].type;
                alarms[alarm].routeId = device;
              }
            }
            if(undefined != alarms[alarm].Bud){
              alarms[alarm].label = alarms[alarm].Bud.Value;
            }
          }
        }
      }
    };

  };

  angular.module('app.alarmLog').service('alarmLogService', [
    '$rootScope',
    '$http',
    'alarmLogInfo',
    'resourceService',
    'cacheService',
    service
  ]);
})();