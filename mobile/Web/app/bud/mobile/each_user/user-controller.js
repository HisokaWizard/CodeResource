'use strict';
(function(){

    var controller = function(
        $scope,
        $http,
        $routeParams,
        $location,
        $interval,
        cacheService,
        userService,
        userInfo,
        mobileService)
    {
        var _public = this;
        var _private ={};
        _public.id = $routeParams.id;
        //_public.breadcrumb = userInfo.breadcrumb;

        //_public.linkforbreadcrumbs = "#/user/"+_public.id;
        //_public.userid = document.getElementById("anything-user");
        //_public.userid.href = _public.linkforbreadcrumbs;

        _public.cache = cacheService.cache;
        _public.user = mobileService.user;
        _public.smr = mobileService.smr;
        _public.data = mobileService.data;
        _public.state = mobileService.state;
        _public.location = mobileService.location;
        _public.scheme = mobileService.scheme;
        _public.biogroup = mobileService.biogroup;
        _public.usergroup = mobileService.usergroup;
        //_public.breadcrumb.uri = "#/user/"+_public.id;
        //_public.breadcrumb.label = _public.user[_public.id].user_name;
        _public.ToTable = function(){
            $location.path("/mobile");
        };

        _public.ToScheme = function(){
            $location.path('/scheme/main');
        };

        _private.EndPointArr = [];
        _public.EndPoints = function(){
            _private.EndPointArr = [];
            for(var item in _public.state) {
                if(_public.id == _public.state[item].user_id){
                    _private.EndPointArr.push(item);
                }
            }
            return _private.EndPointArr;
        };

        _private.SList = function() {
            var array = [];
            var k = 0;
            var r_array = [];
            _private.EndPointArr = _public.EndPoints();
            for(var key in _public.scheme){
                for(var key2 in _public.scheme[key].nodes){
                    for(var i=0; i<_private.EndPointArr.length; i++){
                        if(key2 == _public.state[_private.EndPointArr[i]].location){
                            _public.scheme[key].id_to_list = key;
                            array.push(_public.scheme[key]);
                        }
                    }
                }
            }
            for(var j = 0; j < array.length; j++){
                var l =0;
                while (l < k && r_array[l].id_to_list !== array[j].id_to_list) l++;
                if (l == k) r_array[k++] = array[j];
            }
            return r_array;
        };

        _public.SchemeList = _private.SList();

        _public.RealLocation = function(location){return mobileService.UserRealLocation(location);};
        _public.ReplaceUserParams = function(){return mobileService.ReplaceUserParams();};
        _public.translateMobile = function(state){return mobileService.translate(state);};

        _public.translate = function(state) {return userService.translate(state); };

    };

    angular.module('app.user').controller('UserController', [
        '$scope',
        '$http',
        '$routeParams',
        '$location',
        '$interval',
        'cacheService',
        'userService',
        'userInfo',
        'mobileService',
        controller
    ]);

})();
