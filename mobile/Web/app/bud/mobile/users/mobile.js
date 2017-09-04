'use strict';
(function() {
  angular.module('app.mobile',[
      'ngRoute',
      'oc.lazyLoad'
  ]).
  constant(
    'mobileInfo', {
      id : 'mobile',
      breadcrumb : {
        uri : "#/mobile",
        label : "Контроль экипажа"
      },
      uri : {
        html  : 'app/bud/mobile/users/mobile.html'
      },
      format : 'json',
      strings : {
        'MobileTitleName' : 'Контроль местоположения и физического состояния пользователей',
        'table' : 'Т',
        'scheme' : 'С',
        'user' : "Пользователь",
        'position': "Место",
        'pulse' : "Пульс",
        'active' : "Активность",
        'room' : "Помещение №",
        'movement_yes' : "Движется",
        'movement_no' : "Неподвижен",
        'acceleration' : "Ускорение:",
        'max_speed' : "Максимальная скорость:",
        'min_speed' : "Минимальная скорость:",
        'time' : "Время:",
        'temperature' : "Температура:",
        'pressure' : "Давление:",
        'blood_oxygen_saturation' : "Насыщенность крови кислородом:",
        'additionally' : "Дополнительно",
        'search_for_name' : "Поиск пользователя по имени:"
      }
    }
  ).
  config(function(){
  }).
  run(function(navigationService, mobileInfo){
    
    navigationService.page('app.mobile').
      parameters({
        parentId  : 'intcom',
        path      : '#/mobile',
        label     : 'Контроль экипажа',
        divId     : 'mobile',
        moduleId  : 'mobile'
      }).
      depend([
        'app/bud/mobile/users/mobile-controller.js',
        'app/bud/mobile/users/mobile-service.js',
        'app/bud/mobile/scheme/scheme.js',
        'app/bud/mobile/each_user/user.js',
        'app/bud/mobile/placements/placement.js'
      ]).
      route('/mobile', {
        templateUrl: mobileInfo.uri.html
      });
      
  });
})();