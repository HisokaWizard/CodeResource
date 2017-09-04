'use strict';
(function() {
  var controller = function(
    $scope,
    $rootScope,
    cacheService,
    complanInfo,
    complanService,
    twitterService)
  {
    var _public = this,
      _private = {};
    //functions
    $rootScope.$on('cp::services::filter',function(event, data){
      $scope.$broadcast('cp::services::filter',data);
    });
    $rootScope.$on('cp::sets::filter',function(event, data){
      $scope.$broadcast('cp::sets::filter',data);
    });
    $rootScope.$on('cp::scheme::invalidConfig',function(event, data){
      $scope.$broadcast('cp::scheme::invalidConfig',data);
    });
    $scope.$watch(function(){
      return cacheService.cache.bud.cp.sets.state;
    },function(){
      if(undefined != cacheService.cache.bud.cp.sets.state &&
      '' != cacheService.cache.bud.cp.sets.state.current){
        _public.cSet = cacheService.cache.bud.cp.sets.state.current;
      }
    },true);
    var listener = $scope.$watch(function(){
      return cacheService.cache.graphs;
    },function(){
      if(undefined != cacheService.cache.graphs){
        _private.hasGraphs = true;
        listener();
      }
    });
    _public.open = function(){
      if(_private.hasGraphs)
        _public.cSection = _public.sections.scheme;
    };
    _public.UpdateWinHeight = function(){
      _public.WinHeight = document.documentElement.clientHeight;
    };
    _public.translate = function(word){return complanService.translate(word);};
    _public.openCurrentSet = function(){
      if(undefined !=  cacheService.cache.bud.cp.sets.state)
        _public.cSection = complanInfo.sections.services.name;
    };
    //members
    _private.hasGraphs = false;
    _public.cSection = complanInfo.sections.sets.name;
    _public.kv = cacheService.cache.bud.cp.ranges.kv;
    _public.uhv = cacheService.cache.bud.cp.ranges.uhv;
    _public.classes = cacheService.cache.bud.cp.classes;
    _public.services = cacheService.cache.bud.cp.services.data;
    _public.sets = cacheService.cache.bud.cp.sets.data;
    _public.cache = cacheService.cache;
    _public.hosts = cacheService.cache.hosts;
    _public.devices = cacheService.cache.devices.data;
    _public.cpInfo = complanInfo;
    _public.sections = complanInfo.sections;
    _public.cSet = undefined;
    _public.informationDesk = $rootScope;
    _public.debug = complanInfo.debugLevel;
    _public.getKeys = Object.keys;
    _public.states = complanInfo.states;
    _public.breadcrumb = complanInfo.breadcrumb;
    _public.twitter = twitterService;
    $scope.cpCtrl = this;
  };

  angular.module('app.complan').controller('complanController', [
    '$scope',
    '$rootScope',
    'cacheService',
    'complanInfo',
    'complanService',
    'twitterService',
    controller
  ]);
})();