'use strict';
(function() {
  var controller = function(
    $rootScope,
    $scope,
    $http,
    servicesService,
    filterService,
    twitterService,
    complanInfo,
    modesService,
    endpointsService,
    cacheService)
  {
    var _public = this,
      _private = {};
    //functions
    _public.hasScroll = function(sId){
      var r = Object.keys($scope.cpCtrl.sets[sId].services).length > 4;
      return r;
    };
    _public.getServiceState = servicesService.getServiceState;
    _public.getModeState = modesService.getModeState;
    _public.getCurrentModeId = function(sId){
      if(complanInfo.states.formed != servicesService.getServiceState(sId)) {
        for(var mId in $scope.cpCtrl.services[sId].modes) {
          return mId;
        }
      }else{
        for(var mId in $scope.cpCtrl.services[sId].modes){
          if(complanInfo.states.active == modesService.getModeState(sId,mId)){
            return mId;
          }
        }
      }
    };
    _public.isEncrypted = function(sId){
      return "" != $scope.cpCtrl.services[sId].encrypted_id;
    };
    _public.moreThanOneMode = function(sId){
      return Object.size($scope.cpCtrl.services[sId].modes) > 1;
    };
    _public.isEndpointListLong = function(sId){
      return  Object.size($scope.cpCtrl.services[sId].endpoints) > 5;
    };
    _public.isLongServiceList = function(sId){
      return Object.keys($scope.cpCtrl.sets[sId].services).length > 3;
    };
    _public.isLongSetList = function(){
      return Object.keys($scope.cpCtrl.sets).length > 5;
    };
    _public.getFilter = function(){
      return filterService.getFilter(_public.filter);
    };
    $scope.$on('cp::services::filter',function(event, data){
      _public.filter = data;
    });
    _public.buttonLabel = function(sId){
      if(servicesService.getServiceState(sId) == $scope.cpCtrl.states.unformed)
        return complanInfo.buttonLabel.form;//заменить картинками
      if(servicesService.getServiceState(sId) == $scope.cpCtrl.states.formed)
        return complanInfo.buttonLabel.unform;//заменить картинками
      return complanInfo.buttonLabel.inactive;
    };
    _public.power = function(sId){
      switch(servicesService.getServiceState(sId)){
        case 'formed' : {
          _private.destroy(sId);
          break;
        }
        case 'unformed' : {
          _private.build(sId);
          break;
        }
        default :{
          _public.twitter.alarm("Конфликтное состояние тракта!");
        }
      }
    };
    _private.build = function(id){
      $http.post('bud/cp/services/state/' + id)
        .success(function(body, status){
           _public.twitter.info("Сформирован  тракт!");
        })
        .error(function(body, status){
          _public.twitter.alarm("Не удалось сформировать тракт!");
        });
    };
    _private.destroy = function(id){
      $http['delete']('bud/cp/services/state/' + id)
        .success(function(body, status){
          _public.twitter.info("Расформирован тракт!");
        })
        .error(function(body, status){
          _public.twitter.alarm("Не удалось расформировать тракт!");
        });
    };
    _public.updateWinHeight = function(){
      _public.WinHeight = document.documentElement.clientHeight;
    };
    _public.openModes = function(){
      _public.mainTabs[0] = false;
      _public.mainTabs[1] = true;
    };
    _public.openInfo = function(){
      _public.mainTabs[0] = true;
      _public.mainTabs[1] = false;
    };
    _public.openEditor = function(sId){
      _public.isEndpointEditor = true;
      _public.getNewEndpointList(sId);
    };
    _public.getEndpointState = endpointsService.getEndpointState;
    _public.getNewEndpointList = function(sId){
      if(undefined == $scope.cpCtrl.devices)
        return [];
      _public.newEndpointList = _public.getEndpointList(sId);
    };
    _public.getEndpointList = function(sId){
      if(undefined == $scope.cpCtrl.devices)
        return [];
      var list = Object.keys($scope.cpCtrl.services[sId].endpoints);
      if(undefined != cacheService.cache.bud.cp.services.state){
        var servicesState = cacheService.cache.bud.cp.services.state;
        if(undefined == servicesState[sId])
          return [];
        if(null != servicesState[sId].endpoints && undefined != servicesState[sId].endpoints){
          for(var i in servicesState[sId].endpoints){
            var index = list.indexOf(i);
            if(-1 ==  index && complanInfo.states.detached != servicesState[sId].endpoints[i].state){
              list.push(i);
            }else{
              if(-1 != index && complanInfo.states.detached == servicesState[sId].endpoints[i].state)
                list.splice(index,1);
            }
          }
        }
      }
      return list;
    };
    _public.removeEndpoint = function(sId, eId){
      var index = _public.newEndpointList.indexOf(eId);
      if(-1 != index){
        _public.newEndpointList.splice(index,1);
        var endpointList = _public.getEndpointList(sId);
        index = endpointList.indexOf(eId);
        if(-1 != index){
          $http['delete']('/bud/cp/services/state/' + sId + '/endpoints/' + eId)
            .success(function(body, status){
              _public.twitter.info("Удалено оконечное устройство!");
              //console.log('Status : ' + status);
            })
            .error(function(body, status){
              _public.twitter.alarm("Не удалось удалить оконечное устройство!");
              //console.log('Error Status : ' + status);
            });
        }
      }
    };
    _public.addEndpoint = function(){
      if(undefined == _public.newEndpoint)
        return;
      if(-1 != _public.newEndpointList.indexOf(_public.newEndpoint)){
        _public.twitter.alarm("Данное устройство уже добавлено!");
        return;
      }
      _public.newEndpointList.push(_public.newEndpoint);
      _public.newEndpoint = undefined;
    };
    _private.isUndefined = function(sId,eId){
      var isUndefined, servicesState = cacheService.cache.bud.cp.services.state;;
      if(undefined == $scope.cpCtrl.services[sId].endpoints[_public.newEndpointList[eId]] &&
        undefined == servicesState[sId].endpoints[eId]){
        isUndefined = true;
      }else{
        if(complanInfo.states.detached == servicesState[sId].endpoints[eId].state){
          isUndefined = true;
        }else{
          isUndefined = false;
        }
      }
      return isUndefined;
    };
    _public.saveNewEndpointList = function(serviceId){
      for(var i in _public.newEndpointList){
        if(_private.isUndefined(serviceId,_public.newEndpointList[i])){
          $http.post('/bud/cp/services/state/' + serviceId + '/endpoints/' + _public.newEndpointList[i])
            .success(function(body, status){
              _public.twitter.info("Добавлено оконечное устройство!");
              //console.log('Status : ' + status);
            })
            .error(function(body, status){
              _public.twitter.alarm("Не удалось добавить оконечное устройство!");
              //console.log('Error Status : ' + status);
            });
        }
      }
      _public.newEndpointList = [];
      _public.isEndpointEditor = false;
    };
    _public.cleanNewEndpointList = function(){
      _public.newEndpointList = [];
      _public.isEndpointEditor = false;
    };
    _public.openReceiverEditor = function(){
      _public.isReceiverEditor = true;
      _public.isTransmitterEditor = false;
    };
    _public.openTransmitterEditor = function(){
      _public.isTransmitterEditor = true;
      _public.isReceiverEditor = false;
    };
    _public.saveReceiver = function(sId){
      if(undefined == _public.receiveAntenna || undefined == _public.receiver)
        return;
      var uri = '/bud/cp/services/state/' + sId + '/in',
        body = cacheService.cache.bud.cp.services.state[sId];
      body['in'].antenna_id = _public.receiveAntenna;
      body['in'].radio.id = _public.receiver;
      $http.put(uri,body)
        .success(function(body, status){
          _public.twitter.info("Изменены настройки приема!");
        })
        .error(function(body, status){
          _public.twitter.alarm("Не удалось изменить настройки приема!");
        });
      _public.receiveAntenna = undefined;
      _public.receiver = undefined;
      _public.isReceiverEditor = false;
    };
    _public.saveTransmitter = function(sId){
      if( undefined == _public.transmitterAntenna || undefined == _public.transmitter)
       return;
      var uri = '/bud/cp/services/state/' + sId + '/out',
        body = cacheService.cache.bud.cp.services.state[sId];
      body['out'].antenna_id = _public.transmitterAntenna;
      body['out'].radio.id = _public.transmitter;
      $http.put(uri,body)
        .success(function(body, status){
          _public.twitter.info("Изменены настройки передачи!");
        })
        .error(function(body, status){
          _public.twitter.alarm("Не удалось изменить настройки передачи!");
        });
      _public.transmitterAntenna = undefined;
      _public.transmitter = undefined;
      _public.isTransmitterEditor = false;
    };
    _public.cancelReceiver = function(){
      _public.receiveAntenna = undefined;
      _public.receiver = undefined;
      _public.isReceiverEditor = false;
    };
    _public.cancelTransmitter = function(){
      _public.transmitterAntenna = undefined;
      _public.transmitter = undefined;
      _public.isTransmitterEditor = false;
    };
    _public['switch'] = function(sId,mId){
      $http.post('/bud/cp/services/state/' + sId + '/mode/' + mId)
        .success(function(body, status){
          _public.twitter.info("Активирован режим!");
        })
        .error(function(body, status){
          _public.twitter.alarm("Не удалось активировать режим!");
        });
    };
    _public.isLongName = function(name){
      return 50 <= name.length;
    };
    _public.pick = function(accordion, index){
      if(undefined == accordion ||
        undefined == index)
        return;
      if(index != accordion.cTab)
        accordion.cTab = index;
      else
        accordion.cTab = undefined;
    };
    _public.saveEncryptor = function(sId){
      if(undefined == _public.encryptor)
        return;
      var uri = '/bud/cp/services/state/' + sId + '/encrypted_id',
        body = cacheService.cache.bud.cp.services.state[sId];
      body.encrypted_id = _public.encryptor;
      $http.put(uri,body)
        .success(function(body, status){
        _public.twitter.info("Выбран новый шифратор!");
      })
        .error(function(body, status){
          _public.twitter.alarm("Не удалось сменить шифратор!");
        });
      _public.encryptor = undefined;
      _public.isEncryptorEditor = false;
    };
    _public.cancelEncryptor = function(){
      _public.encryptor = undefined;
      _public.isEncryptorEditor = false;
    };
    _public.isEncryptor = function(device){
      if('t614' == device.type)
        return true;
      else
        return false;
    };
    _public.getEncryptor = function(sId){
      if(undefined == cacheService.cache.bud.cp.services.state ||
        "" == cacheService.cache.bud.cp.services.state[sId].encrypted_id)
          return $scope.cpCtrl.services[sId].encrypted_id;
        else
          return cacheService.cache.bud.cp.services.state[sId].encrypted_id;
    };
    _public.getReceiver = function(sId){
      if(undefined == cacheService.cache.bud.cp.services.state ||
        "" == cacheService.cache.bud.cp.services.state[sId]['in'].radio.id)
        return $scope.cpCtrl.services[sId]['in'].radio.id;
      else
        return cacheService.cache.bud.cp.services.state[sId]['in'].radio.id;
    };
    _public.getReceivingAntenna = function(sId){
      if(undefined == cacheService.cache.bud.cp.services.state ||
        "" == cacheService.cache.bud.cp.services.state[sId]['in'].antenna_id)
        return $scope.cpCtrl.services[sId]['in'].antenna_id;
      else
        return cacheService.cache.bud.cp.services.state[sId]['in'].antenna_id;
    };
    _public.getTransmitter = function(sId){
      if(undefined == cacheService.cache.bud.cp.services.state ||
        "" == cacheService.cache.bud.cp.services.state[sId].out.radio.id)
        return $scope.cpCtrl.services[sId].out.radio.id;
      else
        return cacheService.cache.bud.cp.services.state[sId].out.radio.id;
    };
    _public.getTransmittingAntenna = function(sId){
      if(undefined == cacheService.cache.bud.cp.services.state ||
        "" == cacheService.cache.bud.cp.services.state[sId].out.antenna_id)
        return $scope.cpCtrl.services[sId].out.antenna_id;
      else
        return cacheService.cache.bud.cp.services.state[sId].out.antenna_id;
    };
    //members
    _public.filter = {
      label : undefined,
      endpoints : {},
      encryption : undefined,
      frequencyRange : undefined,
      state : undefined
    };
    _public.transmitter = undefined;
    _public.transmitterAntenna = undefined;
    _public.receiver = undefined;
    _public.receiveAntenna = undefined;
    _public.sKey = undefined;
    _public.mainTabs = [true,false];
    _public.newEndpoint = undefined;
    _public.newEndpointList = [];
    _public.isEncryptorEditor = false;
    _public.isEndpointEditor = false;
    _public.isReceiverEditor = false;
    _public.isTransmitterEditor = false;
    _public.encryptor = undefined;
    $rootScope.servicesCtrlScope = $scope
    _public.twitter = twitterService;
    _public.extraDeviceTypes = cacheService.cache.bud.cp.types;
  };
  
  angular.module('app.complan').controller('servicesController', [
    '$rootScope',
    '$scope',
    '$http',
    'servicesService',
    'filterService',
    'twitterService',
    'complanInfo',
    'modesService',
    'endpointsService',
    'cacheService',
    controller
  ]);
})();