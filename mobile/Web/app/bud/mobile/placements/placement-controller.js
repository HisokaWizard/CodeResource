'use strict';
(function(){

    var controller = function(
        $scope,
        $http,
        $routeParams,
        $location,
        $interval,
        cacheService,
        mobileService,
        placementInfo)
    {
        var _public = this;
        var _private = {};

        _public.id = $routeParams.id;
        //_public.breadcrumb = placementInfo.breadcrumb;
        //_public.breadcrumb.uri = "#/placement/"+_public.id;
        //_public.linkforbreadcrumbs = "#/placement/"+_public.id;
        //_public.placementid = document.getElementById("anything-placement");
        //_public.placementid.href = _public.linkforbreadcrumbs;

        _public.cache = cacheService.cache;
        _public.user = mobileService.user;
        _public.smr = mobileService.smr;
        _public.data = mobileService.data;
        _public.state = mobileService.state;
        _public.location = mobileService.location;
        _public.scheme = mobileService.scheme;
        _public.biogroup = mobileService.biogroup;
        _public.usergroup = mobileService.usergroup;
        _public.Active_Item='';

        _public.translateMobile = function(state){return mobileService.translate(state);};

        _public.ToTable = function(){
            $location.path("/mobile");
        };

        _public.ToScheme = function(){
            $location.path('/scheme/main');
        };

        _public.UserList = function(){
            var count = 0;
            for(var key in _public.user){
                for(var i=0; i<_public.user[key].real_location.length; i++){
                    if(_public.id==_public.user[key].real_location[i]){
                        count++;
                    }
                }
            }
            return count;
        };

        _public.UserCount = _public.UserList();

        _public.UserTable = function(){
            for(var key in _public.user) {
                _public.user[key].endpoint = [];
                for(var key2 in _public.state){
                    if((key == _public.state[key2].user_id) && (_public.id == _public.state[key2].location)){
                        _public.user[key].endpoint.push(key2); // все устройства пользователя
                    }
                }
            }
            return _public.user;
        };

        _public.UTable = function(){
            var utable = [];
            for(var key in _public.UserTable()){
                utable.push(_public.UserTable()[key]);
            }
            return utable;
        };

        _private.SList = function(){
            var scheme_arr = [];
            var obj = _public.scheme;
            for(var key in obj){
                var obj2 = _public.scheme[key].location;
                for(var key2 in obj2){
                    if(_public.id == key2){
                        var elem = {id:"", label: ""};
                        elem.id=key;
                        elem.label = obj[key].label;
                        scheme_arr.push(elem);
                    }
                }
            }
            return scheme_arr;
        };
        _public.SchemeList = _private.SList();

        _public.DoActiveItem = function(item){
            _public.Active_Item = item;
        };

        _public.translate = function(state){return mobileService.translate(state);};
        _public.RealLocation = function(location){return mobileService.UserRealLocation(location);};
        _public.NeedAttention = function(user){return mobileService.NeedAttention(user);};
        _public.SMREnabled = function(user){return mobileService.SMREnabled(user);};
        _public.MultigraphEnabled = function(user){return mobileService.MultigraphEnabled(user);};
        _public.UserHaveNotDevice = function(user){return mobileService.UserHaveNotDevice(user);};
        _public.DeviceMoving = function(item){return mobileService.DeviceMoving(item);};
        _public.ReplaceUserParams = function(){return mobileService.ReplaceUserParams();};
        _public.AllIsWork = function(user){return mobileService.AllIsWork(user);};
        _public.AnyProblemInWork = function(user){return mobileService.AnyProblemInWork(user);};
        _public.SmrProblem = function(user){return mobileService.SmrProblem(user);};

    };

    angular.module('app.placement').controller('PlacementController', [
        '$scope',
        '$http',
        '$routeParams',
        '$location',
        '$interval',
        'cacheService',
        'mobileService',
        'placementInfo',
        controller
    ]);

})();