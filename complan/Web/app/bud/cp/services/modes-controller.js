'use strict';
(function() {
  var controller = function(
    $scope,
    $http,
    complanInfo,
    servicesService,
    twitterService,
    cacheService,
    modesService)
  {
    var _public = this,
      _private = {};
    //functions
    _public.switchTo = function(sKey,mKey){
      $http.post('bud/cp/services/state/' + sKey + '/mode/' + mKey)
        .success(function(body, status){
          _public.twitter.info("Активирован режим!");
        })
        .error(function(body, status){
          _public.twitter.alarm("Не удалось активировать режим!");
        });
    };
    _public.getModeState = modesService.getModeState;
    //members
    _private.states = complanInfo.states;
    _private.services = cacheService.cache.bud.cp.services.state;
  };

  angular.module('app.complan').controller('modesController', [
    '$scope',
    '$http',
    'complanInfo',
    'servicesService',
    'twitterService',
    'cacheService',
    'modesService',
    controller
  ]);
})();