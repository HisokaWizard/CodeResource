'use strict';
(function() {
  angular.module('app.complan',[
    'ngRoute',
    'oc.lazyLoad',
    'app.components.toarray',
    'app.components.scheme'
  ]).
  constant(
    'complanInfo', {
      id : 'complan',
      breadcrumb : {
        uri : "#/complan",
        label : "Внешняя связь"
      },
      uri : {
        html  : 'app/bud/cp/complan.html',
        html_manual: 'app/bud/cp/complan-manual.html'
      },
      format : 'json',
      strings : {
        none         : '   ',
        unformed     : 'расформирован',
        formed       : 'сформирован',
        form         : 'формируется',
        unform       : 'расформировывается',
        failure      : 'отказ',
        exception    : 'исключение',
        active       : 'активен',
        inactive     : 'неактивен',
        inactiveButton : 'неактивна',
        locked       : 'заблокирован',
        attached     : 'добавлено',
        attaching    : 'добавляется',
        detached     : 'не добавлено',
        detaching    : 'удаляется',
        rejected     : 'отказ',
        connected    : 'доб./под-но',
        disconnected : 'доб./отсоед.',
        uhv : 'УКВ',
        kv : 'КВ',
        'true' : 'да',
        'false' : 'не поддерживается'
      },
      sections : {
        services : {
          name : 'Services',
          sections : {
            information : 'Information',
            equipment : {
              name : 'Equipment',
              sections : {
                list : 'List',
                setting : 'Setting'
              }
            },
            modes : 'Modes'
          }
        },
        scheme : 'Scheme',
        sets :  {
          name : 'Sets',
          sections : {
            deleter : 'Set deleter',
            installer : 'Set installer',
            review : 'Set review'
          }
        }
      },
      debugLevel : {
        alarm : 'alarm',
        info : 'info'
      },
      states : {
        unformed : 'unformed',
        formed : 'formed',
        form : 'form',
        unform : 'unform',
        failure : 'failure',
        exception : 'exception',
        active : 'active',
        inactive : 'inactive',
        locked : 'locked',
        pending : 'pending',
        accepted : 'accepted',
        rejected : 'rejected',
        attached : 'attached',
        attaching : 'attaching',
        detached : 'detached',
        detaching : 'detaching',
        connected : 'connected',
        disconnected : 'disconnected'
      },
      buttonLabel : {
        form : 'Сформировать',
        unform : 'Расформировать',
        inactive : 'Не активна'
      },
      frequencyRange : {
        uhv : 'uhv',
        kv : 'kv'
      },
      square : {
        size : 64,
        borderWidth : 10
      },
      scheme : {
        komdName : 'sk',
        height : 700,
        width : 838,
        cellMax : 25,
        grid : {
          height : 5,
          width : 5,
          cell : {
            height : 124,
            width : 150,
          },
          interval : {
            byHeight : 8,
            byWidth : 25
          }
        },
        icon : {
          size : 64,
          borderWidth : 10
        },
        nodeTypes : {
          antenna : { name : "antenna", schemeLevel : 0},
          radioEquipment : { name : "radio_equipment", schemeLevel : 1},
          ppi : { name : "ppi", schemeLevel : 2},
          gate : { name : "gate", schemeLevel : 3},
          "switch" : { name : "switch", schemeLevel : 4},
          terminal : { name : "terminal", schemeLevel : 5},
          unit : { name : "unit", schemeLevel : 6}
        }
      }
    }
  ).
  config(function(){
  }).
  run(function(navigationService, complanInfo){
    
    navigationService.page('app.complan').
      parameters({
        path : '#/complan',
        label : 'Внешняя связь',
        divId : 'cp',
        moduleId : 'complan',
        /*crumbLabel : function($route, cacheService){
          if ($route.current.params.hasOwnProperty('id')){
            if (cacheService.cache.devices.data.hasOwnProperty($route.current.params.id))
              return cacheService.cache.devices.data[$route.current.params.id].label;
            return($route.current.params.id);
          }
        }*/
      }).
      depend([
        'app/bud/cp/complan-controller.js',
        'app/bud/cp/complan-service.js',
        'app/bud/cp/sets/sets-controller.js',
        'app/bud/cp/sets/filter-controller.js',
        'app/bud/cp/services/services-controller.js',
        'app/bud/cp/services/services-service.js',
        'app/bud/cp/services/endpoints-controller.js',
        'app/bud/cp/services/endpoints-service.js',
        'app/bud/cp/services/filter-controller.js',
        'app/bud/cp/services/filter-service.js',
        'app/bud/cp/services/modes-controller.js',
        'app/bud/cp/services/modes-service.js',
        'app/bud/cp/scheme/scheme-controller.js',
        'app/bud/cp/scheme/scheme-service.js'
      ]).
      route('/complan', {
        templateUrl: complanInfo.uri.html
      });
    
    navigationService.page('app.complan-manual').
      parameters({
        path : '#/complan-manual',
        label : 'Справочник',
        divId : 'cp',
        moduleId : 'complan'
      }).
      depend([
        'app/bud/cp/frequency/frequency-base.js',
        'app/bud/cp/services/services-service.js'
      ]).
      route('/complan-manual', {
        templateUrl: complanInfo.uri.html_manual
      });
      
  });
})();
