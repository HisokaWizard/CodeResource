"use strict";

(function(){

  var service = function(
    $location,
    mouseService,
    schemeService,
    nodeLocationService)
  {
    var _public = this;

    _public.SchemeTransit = function(scheme, mousedown, coord) {
      if(undefined !== scheme){
        for (var node in scheme.nodes) {
          if (schemeService.NodeType.transit === scheme.nodes[node].type) {
            if (mouseService.MouseEventValue.down === mousedown) {
              if ((coord.x > scheme.nodes[node].position.x) &&
                (coord.y > scheme.nodes[node].position.y) &&
                (coord.x < (scheme.nodes[node].position.x + scheme.nodes[node].size.width)) &&
                (coord.y < (scheme.nodes[node].position.y + scheme.nodes[node].size.height))) {
                $location.path('/scheme/' + scheme.nodes[node].transitId);
                break;
              }
            }
          }
        }
      }
    };

    _public.LocationTransit = function(scheme, mousedown, coord){
      if(undefined !== scheme){
        for(var node in scheme.nodes){
          if(schemeService.NodeType.location === scheme.nodes[node].type) {
            if (mouseService.MouseEventValue.down === mousedown) {
              if ((coord.x > scheme.nodes[node].position.x) &&
                (coord.y > scheme.nodes[node].position.y) &&
                (coord.x < (scheme.nodes[node].position.x + scheme.nodes[node].size.width)) &&
                (coord.y < (scheme.nodes[node].position.y + scheme.nodes[node].size.height))) {
                $location.path('/placement/' + node);
                break;
              }
            }
          }
        }
      }
    };

    _public.PlacementList = function(scheme, locations){
      var array = [];
      if(undefined !== scheme) {
        for (var location in locations) {
          for (var node in scheme.nodes) {
            if (location === node) {
              locations[location].id_to_list = location;
              array.push(locations[location]);
            }
          }
        }
      }
      return array;
    };

    _public.UserList = function(scheme, user, usertable, locations){
      if(undefined !== scheme) {
        usertable();
        var array = [];
        var i = 0;
        var k = 0;
        var locate = _public.PlacementList(scheme, locations);
        for (i = 0; i < locate.length; i++) {
          for (var _user in user) {
            for (k = 0; k < user[_user].real_location.length; k++) {
              if (locate[i].id_to_list == user[_user].real_location[k] && 1 == user[_user].real_location.length) {
                array.push(user[_user]);
              }
            }
          }
        }
        var counter = 0;
        for (var __user in user) {
          for (i = 0; i < locate.length; i++) {
            if (1 == counter) {
              break;
            }
            for (k = 0; k < user[__user].real_location.length; k++) {
              if (locate[i].id_to_list == user[__user].real_location[k] && 1 < user[__user].real_location.length) {
                array.push(user[__user]);
                counter++;
              }
            }
          }
        }
        locate = [];
      }
      return array;
    };

    _public.UserCountInLocation = function(user, HighLightInListId){
      _public.UserCount = 0;  //вычисляем количество пользователей в помещении
      for (var _user in user) {
        for (var j = 0; j < user[_user].real_location; j++) {
          if (HighLightInListId == user[_user].real_location[j] && '' != user[_user].real_location[j]) {
            _public.UserCount++;
          }
        }
      }
      return _public.UserCount;
    };

  };

  angular.module('app.scheme').service('mobileSchemeService', [
    '$location',
    'mouseService',
    'schemeService',
    'nodeLocationService',
    service
  ]);

})();