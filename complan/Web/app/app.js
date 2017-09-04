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
    'app.components.scheme',
    'app.components.toarray',
    'app.components.fulltextSearch',
    'app.components.cache',
    'app.components.twitter',
    'app.bar',
    'app.alarmLog',
    'app.components.modules',
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
  run(function($rootScope) {
  });
})();