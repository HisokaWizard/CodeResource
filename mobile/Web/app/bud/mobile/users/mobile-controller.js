'use strict';
(function(){

  var controller = function(
      $scope,
      $http,
      $location,
      $interval,
      $filter,
      orderByFilter,
      cacheService,
      mobileService,
      mobileInfo)
  {
      var _public = this;
      _public.cache = cacheService.cache;
      _public.user = mobileService.user;
      _public.smr = mobileService.smr;
      _public.data = mobileService.data;
      _public.state = mobileService.state;
      _public.location = mobileService.location;
      _public.scheme = mobileService.scheme;
      _public.biogroup = mobileService.biogroup;
      _public.usergroup = mobileService.usergroup;
      _public.accordion_background = {};
      _public.Active_Item='';
      _public.breadcrumb = mobileInfo.breadcrumb;


      _public.translate = function(state){return mobileService.translate(state);};
      _public.UserTable = function(){return mobileService.UserTable();};
      _public.NeedAttention = function(user){return mobileService.UserTable(user);};
      _public.SMREnabled = function(user){return mobileService.SMREnabled(user);};
      _public.MultigraphEnabled = function(user){return mobileService.MultigraphEnabled(user);};
      _public.UserHaveNotDevice = function(user){return mobileService.UserHaveNotDevice(user);};
      _public.DeviceMoving = function(item){return mobileService.DeviceMoving(item);};
      _public.RealLocation = function(location){return mobileService.UserRealLocation(location);};
      _public.ReplaceUserParams = function(){return mobileService.ReplaceUserParams();};
      _public.AllIsWork = function(user){return mobileService.AllIsWork(user);};
      _public.AnyProblemInWork = function(user){return mobileService.AnyProblemInWork(user);};
      _public.SmrProblem = function(user){return mobileService.SmrProblem(user);};

      _public.UserName = function(){
          var array = [];
          for(var key in _public.user) {
              _public.user[key].id_to_list = key;
              array.push(_public.user[key]);
          }
          return array;
      };

      _public.UTable = function(){
          var utable = [];
          for(var key in _public.UserTable()){
              utable.push(_public.UserTable()[key]);
          }
          return utable;
      };

      _public.ToScheme = function(){
          $location.path("/scheme/main");
      };

      _public.ToTable = function(){
          $location.path("/mobile");
      };

      _public.DoActiveItem = function(item){
          _public.Active_Item = item;
      };

  };

  angular.module('app.mobile').controller('MobileController', [
    '$scope',
    '$http',
    '$location',
    '$interval',
    '$filter',
    'orderByFilter',
    'cacheService',
    'mobileService',
    'mobileInfo',
    controller
  ]);

})();
