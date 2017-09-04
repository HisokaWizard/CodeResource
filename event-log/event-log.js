'use strict';
(function(){
  angular.module('app.eventLog',[
    'ngRoute',
    'app.components.mvvm',
    'app.components.filter-calendar'
  ]).
  constant(
    'eventLogInfo', {
      id : 'eventLog',
      breadcrumb : {
        uri : "#/event-log",
        label : "События"
      },
      uri : {
        model : 'events',
        html  : 'app/event-log/event-log.html'
      },
      format : 'json'
    }
  ).
  config(function(){
  }).
  run(function(navigationService, eventLogInfo){
    navigationService.page('app.event.log').
      parameters({
        path : '#/event-log',
        label : 'События'
      }).
      route('/event-log', {
        templateUrl: eventLogInfo.uri.html
      });
  });
})();
