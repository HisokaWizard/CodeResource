'use strict';
(function() {

    var service = function(
        $rootScope,
        mobileInfo,
        cacheService,
        resourceService)
    {
        var _public = this;
        _public.cache = cacheService.cache;
        _public.user = _public.cache.users;
        _public.smr = _public.cache.bud.mobile.smr;
        _public.data = _public.cache.bud.mobile.endpoints.data;
        _public.state = _public.cache.bud.mobile.endpoints.state;
        _public.location = _public.cache.locations;
        _public.scheme = _public.cache.bud.mobile.scheme;
        _public.biogroup = _public.cache.bud.mobile.biogroup;
        _public.usergroup = _public.cache.usergroups;

        _public.translate = function(str){
            for(var item in mobileInfo.strings){
                if (item == str)
                    return mobileInfo.strings[item];
            }
            return resourceService.translate(str);
        };

        _public.UserRealLocation = function(location) {
            for (var key in _public.location) {
                if (location == key) {
                    return _public.location[key].label;
                }
            }
        };

        _public.ReplaceUserParams = function(){
          for(var key in _public.state){
            if(500 < _public.state[key].pulse ||
                0 > _public.state[key].pulse ||
                0 == _public.state[key].pulse ||
                '255' == _public.state[key].pulse){
                _public.state[key].pulse = '-';
            }
            if(500 < _public.state[key].bp ||
                0 > _public.state[key].bp ||
                0 == _public.state[key].bp ||
                '255' == _public.state[key].bp){
                _public.state[key].bp = '-';
            }
            if(500 < _public.state[key].blood ||
                0 > _public.state[key].blood ||
                0 == _public.state[key].blood ||
                '255' == _public.state[key].blood){
                _public.state[key].blood = '-';
            }
            if(500 < _public.state[key].temperature ||
                0 > _public.state[key].temperature ||
                0 == _public.state[key].temperature ||
                '255' == _public.state[key].temperature){
               _public.state[key].temperature = '-';
            }
          }
        };

        _public.UserTable = function(){
            for(var key in _public.user) {
                _public.user[key].endpoint = [];
                _public.user[key].real_location = [];
                for(var key2 in _public.state){
                    if(key == _public.state[key2].user_id){
                        _public.user[key].endpoint.push(key2); // все устройства пользователя
                        _public.user[key].real_location.push(_public.state[key2].location); // все устройства пользователя
                    }
                }
            }
            return _public.user;
        };

        _public.NeedAttention = function(user) {
            var counter = 0;
            for(var i=0; i<user.endpoint.length; i++){
                if(!_public.state[user.endpoint[i]].need_attention
                    && _public.state[user.endpoint[i]].sm_enabled){
                    counter++;
                }
            }
            if(counter == user.endpoint.length)
                return true;
            else
                return false;
        };

        _public.SMREnabled = function(user){
            var counter = 0;
            for(var i=0; i<user.endpoint.length; i++){
                if(_public.state[user.endpoint[i]].smr_enabled){
                    counter++;
                }
            }
            if(counter == user.endpoint.length)
                return true;
            else
                return false;
        };

        _public.MultigraphEnabled = function(user){
            var counter = 0;
            for(var i=0; i<user.endpoint.length; i++){
                if(_public.state[user.endpoint[i]].multigraph_enabled){
                    counter++;
                }
            }
            if(counter == user.endpoint.length)
                return true;
            else
                return false;
        };

        _public.UserHaveNotDevice = function(user){
            if(0==user.endpoint.length){
                return true;
            }
            else
                return false;
        };

        _public.DeviceMoving = function(item){
            if(0 < +_public.state[item].accel){
                return true;
            }
            else
                return false;
        };

        _public.AllIsWork = function(user){
            if(_public.NeedAttention(user) && _public.SMREnabled(user) && _public.MultigraphEnabled(user) && !_public.UserHaveNotDevice(user)){
                return true;
            }
            else
                return false;
        };

        _public.AnyProblemInWork = function(user){
            if((!_public.NeedAttention(user) && !_public.MultigraphEnabled(user) && _public.SMREnabled(user) && !_public.UserHaveNotDevice(user)) ||
                (!_public.NeedAttention(user) && _public.MultigraphEnabled(user) && _public.SMREnabled(user) && !_public.UserHaveNotDevice(user)) ||
                (_public.NeedAttention(user) && !_public.MultigraphEnabled(user) && _public.SMREnabled(user) && !_public.UserHaveNotDevice(user))){
                return true;
            }
            else
                return false;
        };

        _public.SmrProblem = function(user){
            if((!_public.SMREnabled(user) && !_public.MultigraphEnabled(user) && !_public.UserHaveNotDevice(user)) ||
                (!_public.SMREnabled(user) && _public.MultigraphEnabled(user) && !_public.UserHaveNotDevice(user))){
                return true;
            }
            else
                return false;
        };

    };


    angular.module('app.mobile').service('mobileService', [
        '$rootScope',
        'mobileInfo',
        'cacheService',
        'resourceService',
        service
    ]);
})();