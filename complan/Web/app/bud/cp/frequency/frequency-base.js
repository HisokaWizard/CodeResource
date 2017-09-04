'use strict';
(function(){

    var controller = function(
        $scope,
        $http,
        $routeParams,
        $location,
        $interval,
        cacheService,
        servicesService)
    {
        var _public = this;
        var _private = {};
        _public.cache = cacheService.cache;
        _public.services_data = cacheService.cache.bud.cp.services.data;
        _public.sets_state = cacheService.cache.bud.cp.sets.state;
        _public.sets_data = cacheService.cache.bud.cp.sets.data;
        _public.forbidden_freq_value = cacheService.cache.bud.cp.forbiddenfrequency;
        _public.forbidden_freqrange = cacheService.cache.bud.cp.forbiddenfreqranges;
        _public.breadcrumb = {uri:"#/complan-manual",label:"Справочник"};
        _public.getServiceState = servicesService.getServiceState;

        _public.UpdateWinHeight = function(){
            _public.WinHeight = document.documentElement.clientHeight;
        };
        _public.UpdateWinHeight();

        _public.ForbiddenFreq = false;
        _public.EnableForbiddenFreq = function(){
            _public.FixedFreq = false;
            return _public.ForbiddenFreq = true;
        };

        _public.FixedFreq = true;
        _public.EnableFixedFreq = function(){
            _public.ForbiddenFreq = false;
            return _public.FixedFreq = true;
        };

        _public.ValueFreq = true;
        _public.EnableValueFreq = function(){
            _public.FreqRange = false;
            return _public.ValueFreq = true;
        };

        _public.FreqRange = false;
        _public.EnableFreqRange = function(){
            _public.ValueFreq = false;
            return _public.FreqRange = true;
        };

        _public.ApplyStateKey = function(){
            var keys_current = "";
            if(undefined ==_public.sets_state){
                return keys_current;
            }
            else{
                for(var key in _public.sets_state){
                    if("current"==key){
                        keys_current = _public.sets_state[key];
                    }
                }
                return keys_current;
            }
        };

        _public.ParamsSetsArr = [];
        _public.ServiceData = function() {
            var arr = [];
            var arr_return = [];
            for (var key in _public.sets_data) {
                if (key == _public.ApplyStateKey()) {
                    for (var key2 in _public.sets_data[key].services) {
                        arr.push(key2);
                    }
                }
            }
            _public.FlagToAddFixFreq= true;
            for(var key3 in _public.services_data){
                for(var i=0; i<arr.length; i++){
                    if(arr[i] == key3){
                        var obj = {};
                        obj.frequency = _public.services_data[key3].modes.m1.rx_frequency;
                        obj.state_device = "Прием/Передача";
                        obj.device_id = _public.services_data[key3].modes.m1.build[0].host;
                        obj.device = "";
                        obj.device_type = "";
                        for(var key4 in _public.cache.hosts){
                            if(key4 == obj.device_id){
                                obj.device = _public.cache.hosts[key4].label;
                                obj.device_type = _public.cache.hosts[key4].type;
                            }
                        }
                        if(1 < arr_return.length){
                            for(var j=0; j<arr_return.length; j++){
                                if((arr_return[j].frequency == obj.frequency) &&
                                    (arr_return[j].state_device == obj.state_device) &&
                                    (arr_return[j].device == obj.device)){
                                    _public.FlagToAddFixFreq = false;
                                }
                                else{
                                    _public.FlagToAddFixFreq = true;
                                }
                            }
                        }
                        if(_public.FlagToAddFixFreq){
                            arr_return.push(obj);
                        }
                    }
                }
            }
            _public.ParamsSetsArr = arr_return;
        };
        _public.ServiceData();

        _public.getWebType = function(id){
            if(undefined != _private.types[_public.cache.hosts[id].type]
                && undefined != _private.types[_public.cache.hosts[id].type].defaultProxyPort){
                if(0 != _private.types[_public.cache.hosts[id].type].defaultProxyPort)
                    return 'proxy';
                else
                    return 'nothing';
            }else{
                return 'new';
            }
        };

        _private.types = cacheService.cache.types;
    };

    angular.module('app.complan').controller('CPManualCtrl', [
        '$scope',
        '$http',
        '$routeParams',
        '$location',
        '$interval',
        'cacheService',
        'servicesService',
        controller
    ]);
})();