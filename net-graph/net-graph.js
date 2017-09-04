'use strict';
(function() {
  angular.module('app.netGraph',[
    'ngRoute',
    'app.components.mvvm'
  ]).
  constant(
    'netGraphInfo', {
        id : 'netGraph',
        breadcrumb : {
          uri : "#/",
          label : "Мнемосхема"
        },
        uri : {
            model : '',
            html  : 'app/net-graph/net-graph.html'
        },
        format : 'json'
    }
  ).
  config(function(){
  }).
  run(function(navigationService, netGraphInfo){
    navigationService.page('app.netGraph').
      parameters({
        path : '#/',
        label : '',
        hidden : true
      }).
      route('/net-graph/:id', {
        templateUrl: netGraphInfo.uri.html
      }, function($route, cacheService){
        if ($route.current.params.hasOwnProperty('id')){
          if (cacheService.cache.schemes.hasOwnProperty($route.current.params.id))
            return cacheService.cache.schemes[$route.current.params.id].label;
          return($route.current.params.id);
        }
      }).
      route('/', {
        templateUrl: netGraphInfo.uri.html
      });
  });
})();
