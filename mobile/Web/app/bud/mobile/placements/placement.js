'use strict';
(function() {
    angular.module('app.placement',[
        'ngRoute',
        'oc.lazyLoad'
    ]).
        constant(
        'placementInfo', {
            id : 'placement',
            breadcrumb : {
              uri : "",
              label : "Помещение ТО"
            },
            uri : {
                html  : 'app/bud/mobile/placements/placement.html'
            },
            format : 'json',
            strings : {
            }
        }
    ).config(function(){})
      .run(function(navigationService, placementInfo){
          navigationService.page('app.placement').
            //parameters({
            //    path : '#/placement/:id',
            //    label : "Помещение"
            //}).
            depend([
                'app/bud/mobile/placements/placement-controller.js'
            ]).
            route('/placement/:id', {
                templateUrl : placementInfo.uri.html
            })
      });
})();