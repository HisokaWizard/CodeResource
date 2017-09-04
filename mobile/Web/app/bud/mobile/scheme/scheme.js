'use strict';
(function() {
    angular.module('app.scheme',[
        'ngRoute',
        'oc.lazyLoad'
    ]).
        constant(
        'schemeInfo', {
            id : 'scheme',
            breadcrumb : {
              uri : "",
              label : ""
            },
            uri : {
                html  : 'app/bud/mobile/scheme/scheme.html'
            },
            format : 'json',
            strings : {
            }
        }
    )
      .config(function(){})
      .run(function(navigationService, schemeInfo){
          navigationService.page('app.scheme').
            //parameters({
            //    path : '#/scheme/:id',
            //    label : "Схема объектов"
            //}).
            depend([
                'app/bud/mobile/scheme/scheme-controller.js',
                'app/bud/mobile/scheme/scheme-service.js'
            ]).
            route('/scheme/:id', {
                templateUrl : schemeInfo.uri.html
            })
      });

})();
