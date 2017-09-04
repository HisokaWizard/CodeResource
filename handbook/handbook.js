'use strict';
(function () {
  angular.module('app.handbook', [
    'ngRoute',
    'app.components.mvvm'
  ]).
    constant(
    'handBookInfo', {
      id: 'handBook',
      breadcrumb: {
        uri: "#/handbook",
        label: "Адресная книга"
      },
      uri: {
        model: '',
        html: 'app/handbook/handbook.html'
      },
      format: 'json'
    }
  ).
  config(function(){
  }).
  run(function(navigationService, handBookInfo){
    
    navigationService.page('app.handbook').
      parameters({
        path : '#/handbook',
        label : 'Адресная книга',
      }).
      route('/handbook', {
        templateUrl: handBookInfo.uri.html
      });
  });
})();