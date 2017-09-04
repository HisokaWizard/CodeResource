'use strict';
(function() {
    angular.module('app.user',[
        'ngRoute',
        'oc.lazyLoad'
    ]).
        constant(
        'userInfo', {
            id : 'user',
            breadcrumb : {
              uri : "",
              label : ""
            },
            uri : {
                html  : 'app/bud/mobile/each_user/user.html'
            },
            format : 'json',
            strings : {
                'profile' : "Профиль пользователя",
                'main_work_place' : "Основное место работы:",
                'info_locate' : "Сведения о расположении/перемещении:",
                'in_work_place' : "Находится на рабочем месте",
                'main_scheme' : "Показать на общей схеме"
            }
        }
    ).config(function(){})
      .run(function(navigationService, userInfo){
          navigationService.page('app.user').
            //parameters({
            //    path : '#/user/:id',
            //    label : "Пользователь"
            //}).
            depend([
                'app/bud/mobile/each_user/user-controller.js',
                'app/bud/mobile/each_user/user-service.js'
            ]).
            route('/user/:id', {
                templateUrl : userInfo.uri.html
            })
      });
})();