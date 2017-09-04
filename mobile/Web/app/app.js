'use strict';

(function() {

  angular.module('app', [
    'ngRoute',
    'app.components.request',
    'app.components.eventSystem',
    'app.components.location',
    'app.components.mvvm',
    'app.components.resources',
    'app.components.queue',
    'app.components.breadcrumbs',
    'app.components.cache',
    'app.components.scheme',
    'app.bar',
    'app.components.modules',
    'app.components.navigation',
    'oc.lazyLoad'
  ]).
      constant(
      'appSpace', 'app'
  ).
      config(function($routeProvider, $locationProvider, $httpProvider){

        $httpProvider.defaults.cache = false;
        if (!$httpProvider.defaults.headers.get) {
          $httpProvider.defaults.headers.get = {};
        }
        // disable IE ajax request caching
        $httpProvider.defaults.headers.get['If-Modified-Since'] = '0';

        $routeProvider.otherwise({redirectTo: '/'});

        $locationProvider.html5Mode(false);
      }).
      run(function($rootScope, $ocLazyLoad) {
        //$rootScope.$on('*', function (event, data) {
        //  $rootScope.$broadcast(event, data);
        //});
      });
})();
