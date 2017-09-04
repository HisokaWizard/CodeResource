'use strict';
(function() {
  var controller = function(
    $rootScope,
    $scope,
    $http,
    cacheService,
    complanService,
    twitterService,
    fulltextSearchService)
  {
    var _public = this,
      _private = {};
    //functions
    $scope.$on('cp::sets::filter',function(name,data){
      _public.label = data;
    });
    _public.getFilter = function(){
      return function(item){
        var match = true;
        if((undefined != item.label)
        && (undefined != _public.label)
        && ("" != item.label)
        && ("" != _public.label)){
          if(-1 != fulltextSearchService.fullTextSearch(_public.label.toLowerCase(), item.label.toLowerCase())){
            match = true;
          }
          else{
            match = false;
          }
        }
        return match;
      };
    };
    _public.getFirstModeId = function(sId){
      for(var name in $scope.cpCtrl.services[sId].modes){
        return name;
      }
    };
    _public.moreThanOneMode = function(sId){
      return Object.size($scope.cpCtrl.services[sId].modes) > 1;
    };
    _public.isEndpointListLong = function(sId){
      return  Object.size($scope.cpCtrl.services[sId].endpoints) > 5;
    };
    _public.isLongServiceList = function(sId){
      return Object.keys($scope.cpCtrl.sets[sId].services).length > 4;
    };
    _public.isLongSetList = function(){
      return Object.keys($scope.cpCtrl.sets).length > 5;
    };
    _public.getSetIdBy$index = function(index){
      return Object.keys($scope.cpCtrl.sets)[index];
    };
    _public.openInstaller = function(setId){
      _public.tmpSetId = setId;
      _public.cSection = $scope.cpCtrl.sections.sets.sections.installer;
    };
    _public.openDeleter = function(setId){
      _public.tmpSetId = setId;
      _public.cSection = cpCtrl.sections.sets.sections.deleter;
    };
    _public.openReview = function(setId){
      if($scope.cpCtrl.cSet == setId){
        $scope.cpCtrl.cSection = $scope.cpCtrl.sections.services.name;
        return;
      }
      _public.cSection = $scope.cpCtrl.sections.sets.sections.review;
      _public.reviewSet = setId;
    };
    _public.apply = function(){
      var body = {};
      body.id = _public.tmpSetId;
      $http.put('bud/cp/sets/state/awaited', body)
        .success(function(body, status){
          //console.log('Status : ' + status);
          _public.twitter.info("Установлен набор связи!");
        })
        .error(function(body, status){
          //console.log('Error Status : ' + status);
          _public.twitter.alarm("Не удалось установить набор связи!");
        });
      _public.tmpSetId = undefined;
      _public.cSection = $scope.cpCtrl.sections.sets.name;
    };
    _public.UpdateWinHeight = function(){
      _public.WinHeight = document.documentElement.clientHeight;
    };
    _public.UpdateWinHeight();
    $scope.$watch(function(){return _public.cSection;},function(){
      if(undefined != $rootScope.servicesCtrlScope){
        $rootScope.servicesCtrlScope.$destroy();
      }
    })
    _public.isLongName = function(name){
      return 50 <= name.length;
    };
    _public.isEncrypted = function(sId){
      return "" != $scope.cpCtrl.services[sId].encrypted_id;
    };
    _public.isLongName = function(name){
      return 50 <= name.length;
    };
    _public.pick = function(accordion, index){
      if(undefined == accordion ||
        undefined == index)
        return;
      if(index != accordion.cTab)
        accordion.cTab = index;
      else
        accordion.cTab = undefined;

    };
    //members
    _public.tmpSetId = undefined;
    _public.reviewSet = undefined;
    _public.cSection = $scope.cpCtrl.sections.sets.name;
    _public.label = undefined;
    _public.twitter = twitterService;
  };

  angular.module('app.complan').controller('setsController', [
    '$rootScope',
    '$scope',
    '$http',
    'cacheService',
    'complanService',
    'twitterService',
    'fulltextSearchService',
    controller
  ]);
})();