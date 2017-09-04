'use strict';
(function() {

  var service = function(
    $http,
    queueFactory,
    calendarService,
    stateService,
    menuItems)
  {
    var _public = this;

    _public.queue = new queueFactory(updateData);
    _public.events = [];
    _public.queue.reqPath = "eventLog/";
    _public.queue.registerQueue(_public.queue);
    _public.messageCount = 20;
    _public.openAccordeonId = 0;

    var _private = {};
    _private.requestCounter = 1;

    function updateData(path, method, data){
      var _local = _public.queue;
      if(method != 'DELETE') {
         for (var item in data["records"]){
           if (_public.events.length >= _public.messageCount){
             if(_public.openAccordeonId !== _public.events[0]){
               _public.events.splice(0,1)
             }
           }
           data["records"][item].id = (Math.random()*1000000000).toFixed(0);
           _public.events.push(data["records"][item]);
         }
      } else {
         _public.events.length = 0;
      }

      if (!_local.initialized) {
        _local.initialized = true;
      }
    }

    _public.filter = function(data1){
      var req = {
       method: 'PUT',
       url: 'eventLog/' + _public.queue.qId,
       headers: {
         'Q-Path': '/filter',
         'Q-Method': 'PUT',
         'Q-Id': _private.requestCounter++
       },
       data: data1
      };
      $http(req);
    };

    _public.moreMessage = function(){
      _public.messageCount+=10;
      var seacher = {};
      seacher["FilterParams"] = {};
      seacher["MinNumberToFind"]=_public.messageCount;
      _public.filter(seacher);
    };

    _public.getFilter = function(label, accident, fulltextSearchService){
      if(undefined !== accident){
        var match = true;
        if((undefined !== accident)
          && (undefined !== label)
          && ("" !== accident.label)
          && ("" !== label)){
          if(-1 !== fulltextSearchService.fullTextSearch(label.toLowerCase(), accident.label.toLowerCase())){
            match = true;
          }
          else{
            match = false;
          }
        }
        return match;
      }
    };

    _public.getEventsProperty = function(events, devicesData){
      if(0 != Object.size(events)){
        for(var event in events){
          for(var device in devicesData){
            if(undefined != events[event].Device){
              if(events[event].Device.Value === device){
                events[event].label = devicesData[device].label;
                events[event].routeType = devicesData[device].type;
                events[event].routeId = device;
              }
            }
            if(undefined != events[event].Bud){
              events[event].label = events[event].Bud.Value;
            }
          }
        }
      }
    };

    _public.getShortInfo = function(data, bigValue, cache){
      var allowField = {};
      allowField["Description"] = {flag : true,  TypeName : "Описание:"};
      //allowField["Device"] = {flag : true,  TypeName : "Устройство:"};
      //allowField["Host"] = {flag : true,  TypeName : "Программа устройства:"};
      allowField["Message"] = {flag : true,  TypeName : "Сообщение:"};
      allowField["ErrorMessage"] = {flag : true,  TypeName : "Сообщение об ошибке:"};
      allowField["Event"] = {flag : true,  TypeName : "Событие:"};
      allowField["Source"] = {flag : true,  TypeName : "Ресурс:"};
      allowField["Time"] = {flag : true,  TypeName : "Время:"};
      allowField["RegTime"] = {flag : true,  TypeName : "Регистрационное время:"};
      allowField["User"] = {flag : true,  TypeName : "Пользователь:"};
      allowField["From"] = {flag : true,  TypeName : "От кого:"};
      allowField["To"] = {flag : true,  TypeName : "Кому:"};
      allowField["FilePath"] = {flag : true,  TypeName : "Путь к файлу:"};
      allowField["Status"] = {flag : true,  TypeName : "Статус:"};
      //
      allowField["ErrorCode"] = {flag : true,  TypeName : "Код ошибки:"};
      allowField["ErrorDescription"] = {flag : true,  TypeName : "Ошибка описания:"};
      allowField["ErrorEntry"] = {flag : true,  TypeName : "Ошибка кода:"};
      allowField["ErrorLevel"] = {flag : true,  TypeName : "Ошибка уровня:"};
      allowField["ErrorName"] = {flag : true,  TypeName : "Ошибка имени:"};
      //
      allowField["SessionId"] = {flag : true,  TypeName : "Идентификатор сессии:"};
      var ret = {};
      var counter = 0;
      for (var key in data) {
        if(undefined != allowField[key]){
          if(allowField[key].flag){
              //if("Device" === key){
              //  data[key].Value = _public.getLabel(data);
              //}
              //if("Host" === key){
              //  if(undefined !== cache.devices){
              //    if(undefined !== cache.devices.data){
              //      for(var device in cache.devices.data){
              //        if(cache.devices.data[device].label === data["Device"].Value){
              //          if(undefined !== cache.devices.data[device].components){
              //            for(var component in cache.devices.data[device].components){
              //              if(cache.devices.data[device].components[component]['host-id'] === data[key].Value){
              //                data[key].Value = cache.devices.data[device].components[component].label;
              //              }
              //            }
              //          }
              //        }
              //      }
              //    }
              //  }
              //}
            if("Status" === key){
              if(stateService.State.ok.status === data[key].Value){
                data[key].Value = stateService.State.ok.label;
              }
              if(stateService.State.fault.status === data[key].Value){
                data[key].Value = stateService.State.fault.label;
              }
              if(stateService.State.none.status === data[key].Value){
                data[key].Value = stateService.State.none.label;
              }
              if(stateService.State.unknown.status === data[key].Value){
                data[key].Value = stateService.State.unknown.label;
              }
            }
            if("Description" === key){
              data[key].Value = _public.getDescription(data, 1000);
            }
            ret[key]=data[key];
            ret[key].TypeName = allowField[key].TypeName;
            counter++;
          }
        }
      }
      bigValue = false;
      if(3 < counter){
        bigValue=true;
      }else{
        bigValue=false;
      }
      ret.bigValue = bigValue;
      counter = 0;
      return ret;
    };

    _public.applyFilter = function(){
      var seacher = {};
      var filterParams = {};
      if(Date.parse( calendarService.filterDateMin()+"T"+calendarService.filterTimeMin())
        && Date.parse(calendarService.filterDateMax()+"T"+calendarService.filterTimeMax())){
        var time  = {
          'Type'  : 'NAmg::TDateTime',
          'MinValue' : calendarService.filterDateMin()+"T"+calendarService.filterTimeMin(),
          'MaxValue' : calendarService.filterDateMax()+"T"+calendarService.filterTimeMax()
        };
        filterParams["Time"] = time;
      }
      var type = document.getElementsByName("RecordType");
      var checked = "";
      for (var i = 0; i < type.length; i++){
        if  (type[i].checked == true) {
          checked = type[i].value;
        }
      }
      if (checked == "status")
      {
        filterParams["Status"]={};
        filterParams["Status"]["Type"]="NAmg::NLotData::TSatusEnum";
      }
      else if (checked == "register")
      {
        filterParams["RegTime"]={};
        filterParams["RegTime"]["Type"]="NAmg::TDateTime";
      }
      seacher["FilterParams"]=filterParams;
      seacher["MinNumberToFind"]=20;
      _public.messageCount = 20;
      _public.filter(seacher);
    };

    _public.getDescription = function(item, messagelenght){
      if(undefined !== item.Description){
        if(undefined !== item.Description.Value && "" !== item.Description.Value){
          if(10 > item.Description.Value.length){
            return  "Произошло неописанное событие";
          }
          if(messagelenght > item.Description.Value.length){
            return item.Description.Value;
          } else {
            return (item.Description.Value.substring(0, messagelenght)+"...");
          }
        }
      }
      return "Произошло неописанное событие";
    };

    _public.getLabel = function(item){
      if("cp" === item.label){
        return item.label = menuItems[0].parameters.label;
      }
      if("mobile" === item.label){
        return item.label = menuItems[2].parameters.label;
      }
      if("vks" === item.label){
        return item.label = menuItems[7].parameters.label;
      }
      return item.label;
    };

  };

  angular.module('app.eventLog').service('eventLogService', [
    '$http',
    'queueFactory',
    'calendarService',
    'stateService',
    'menuItems',
    service
  ]);
})();