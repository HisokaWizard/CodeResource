'use strict';
(function() {
  var controller = function(
    $http,
    cacheService,
    complanInfo,
    twitterService,
    endpointsService)
  {
    var _public = this,
      _private = {};
    //functions
    _public.add = function(serviceId, endpointId){
      $http.post('/bud/cp/services/state/' + serviceId + '/endpoints/' + endpointId)
        .success(function(body, status){
          _public.twitter.info("Добавлено оконечное устройство!",_private.success);
          //console.log('Status : ' + status);
        })
        .error(function(body, status){
          _public.twitter.alarm("Не удалось добавить оконечное устройство!",_private.danger);
          //console.log('Error Status : ' + status);
        });
    };
    _public.remove = function(serviceId, endpointId){
      $http['delete']('/bud/cp/services/state/' + serviceId + '/endpoints/' + endpointId)
        .success(function(body, status){
          _public.twitter.info("Удалено оконечное устройство!",complanInfo.debugLevel.info);
          //console.log('Status : ' + status);
        })
        .error(function(body, status){
          _public.twitter.alarm("Не удалось удалить оконечное устройство!",complanInfo.debugLevel.alarm);
          //console.log('Error Status : ' + status);
        });
    };
    _public.getEndpointState = endpointsService.getEndpointState;
    //members
    _public.newEndpoint = undefined;
    _public.obj = {1:{label:'name1'},2:{label:'name2'}};
  };

  angular.module('app.complan').controller('endpointsController', [
    '$http',
    'cacheService',
    'complanInfo',
    'twitterService',
    'endpointsService',
    controller
  ]);
})();