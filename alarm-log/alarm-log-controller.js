'use strict';
(function() {

  var controller = function(
    $scope,
    cacheService,
    viewModelFactory,
    alarmLogInfo,
    alarmLogService,
    fulltextSearchService,
    eventLogService,
    locationService)
  {
    var _public = this;
    _public.cache = cacheService.cache;
    _public.breadcrumb = alarmLogInfo.breadcrumb;
    _public.bigValue = false;
    _public.showModal = false;
    _public.filter = {
      label: undefined,
      types: {},
      state: undefined
    };
    _public.translate = function(state){
      return alarmLogService.translate(state);
      };

    $scope.$watch(function(){
        return cacheService.cache;
      },
      function(){
        if(undefined !== cacheService.cache.alarms){
          _public.alarms = cacheService.cache.alarms;
        }
        if(undefined !== cacheService.cache.devices){
          _public.devices = cacheService.cache.devices;
        }
        if(undefined !== _public.alarms && undefined !== _public.devices){
          _public.alarmData = alarmLogService.getAlarmsData(_public.alarms, _public.devices.data);
        }
      },true);

    _public.WinHeight = document.documentElement.clientHeight;

    _public.eventsLog = function(){locationService.showEventLog();};
    _public.graph = function(){locationService.showNetGraph();};
    _public.elements = function () {locationService.showElements();};

    _public.Apply = function(accident){
      alarmLogService.Apply(accident, _public.alarms.devices);
    };

    _public.Delete = function(alarmsObj, accident){
      alarmLogService.Delete(alarmsObj, accident);
      _public.showModal = !_public.showModal;
    };

    _public.deleteClick = function(accident){
      _public.accident = accident;
      _public.showModal = !_public.showModal;
    };

    _public.closedButton = function(){
      _public.showModal = !_public.showModal;
    };

    _public.getFilter = function(label, filter){
      return function(accident){
        var match = alarmLogService.getFilter(label, accident, filter, fulltextSearchService);
        return match;
      };
    };

    _public.getShortInfo = function(data){
      var info = eventLogService.getShortInfo(data, _public.bigValue, cacheService.cache);
      _public.bigValue = info.bigValue;
      return info;
    };

  };

  angular.module('app.alarmLog').controller('alarmLogController', [
    '$scope',
    'cacheService',
    'viewModelFactory',
    'alarmLogInfo',
    'alarmLogService',
    'fulltextSearchService',
    'eventLogService',
    'locationService',
    controller
  ]);
})();