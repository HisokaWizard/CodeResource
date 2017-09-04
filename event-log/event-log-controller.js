'use strict';
(function() {

  var controller = function(
    $scope,
    cacheService,
    viewModelFactory,
    eventLogInfo,
    eventLogService,
    resourceService,
    locationService,
    fulltextSearchService)
  {
    var _public = this;
    var _private ={};
    _public.breadcrumb = eventLogInfo.breadcrumb;
    _public.WinHeight = document.documentElement.clientHeight;
    _public.bigValue = false;
    _public.fixIndex = 0;
    _public.cache = cacheService.cache;
    _public.translate = function(state){return resourceService.translate(state);};

    $scope.$watch(function(){
      return cacheService.cache.devices;
    }, function(){
      if(undefined != cacheService.cache.devices){
        _public.devices = cacheService.cache.devices;
      }
    });

    $scope.$watch(function(){
      return eventLogService.events;
    }, function(){
      if(undefined != eventLogService.events){
        _public.events = eventLogService.events;
        _public.getEventsProperty();
      }
    }, true);

    _public.getEventsProperty = function(){
      eventLogService.getEventsProperty(_public.events, _public.devices.data);
    };

    _public.alarmsLog = function(){ locationService.showAlarmLog(); };
    _public.graph = function(){locationService.showNetGraph();};
    _public.elements = function () {locationService.showElements();};

    _public.getFilter = function(label){
      return function(accident){
        var match = eventLogService.getFilter(label, accident, fulltextSearchService);
        return match;
      };
    };

    _public.getDescription = function(item, messagelenght){
      return eventLogService.getDescription(item, messagelenght);
    };

    _public.getLabel = function(item){
      return eventLogService.getLabel(item);
    };

    _public.getShortInfo = function(data){
      var info = eventLogService.getShortInfo(data, _public.bigValue, cacheService.cache);
      _public.bigValue = info.bigValue;
      return info;
    };

    _public.applyFilter = function(elemType){
      eventLogService.applyFilter(elemType);
    };

    _public.moreMessage = function(){
      eventLogService.moreMessage();
    };
  };

  angular.module('app.eventLog').controller('eventLogController', [
    '$scope',
    'cacheService',
    'viewModelFactory',
    'eventLogInfo',
    'eventLogService',
    'resourceService',
    'locationService',
    'fulltextSearchService',
    controller
  ]);
})();