'use strict';

(function() {

      var controller = function (
      $scope,
      $timeout,
      $interval,
      $location,
      $routeParams,
      cacheService,
      mobileService,
      schemeService,
      mouseService,
      mobileSchemeService,
      schemeInfo) {

        var _public = this;

        _public.id = $routeParams.id;
        //_public.breadcrumb = schemeInfo.breadcrumb;
        //_public.breadcrumb.uri = "#/scheme/"+_public.id;
        //_public.breadcrumb.label = "Схема объекта";

        _public.cache = cacheService.cache;
        _public.user = mobileService.user;
        _public.smr = mobileService.smr;
        _public.data = mobileService.data;
        _public.state = mobileService.state;
        _public.location = mobileService.location;
        _public.schemes = mobileService.scheme;
        _public.biogroup = mobileService.biogroup;
        _public.usergroup = mobileService.usergroup;
        _public.HighLightInListId = '';
        _public.UserCount=0;
        _public.ShowCoord=false;

        _public.ShowCoordMethod =function(){
          if(!_public.ShowCoord){
            return _public.ShowCoord = true;
          }
          else{
            return _public.ShowCoord = false;
          }
        };

        _public.UserTable = function(){return mobileService.UserTable();};

        // mouse event handlers
        _public.canvas_coord = {x: 0, y: 0};
        _public.MouseDownState = "";
        _public.MouseUpState = "";
        _public.MouseLeaveState = "";
        _public.MouseOverState = "";

        _public.OnMouseMove = function (event) {
          _public.MouseDownState = "";
          _public.MouseUpState = "";
          _public.MouseLeaveState = "";
          _public.MouseOverState = "";
          var new_cord = mouseService.onMouseMove(event);
          var scroll_changes = _public.Scroll();
          if(0 === scroll_changes.scroll_x) {
            scroll_changes.scroll_x = 0;
          }
          if(0 === scroll_changes.scroll_y) {
            scroll_changes.scroll_y = 0;
          }
          _public.canvas_coord.x = new_cord.x+scroll_changes.scroll_x;
          _public.canvas_coord.y = new_cord.y+scroll_changes.scroll_y;
          return _public.canvas_coord;
        };

        _public.GetCoord = function () {
          return _public.canvas_coord;
        };

        _public.MouseDown = function (event) {
          return _public.MouseDownState = mouseService.MouseDown(event);
        };

        _public.MouseUp = function (event) {
          return _public.MouseUpState = mouseService.MouseUp(event);
        };

        _public.MouseOver = function(event){
          return _public.MouseOverState = mouseService.MouseOver(event);
        };

        _public.MouseLeave = function(event){
          _public.HighLightInListId = '';
          return _public.MouseLeaveState = mouseService.MouseLeave(event);
        };

        _public.Scroll = function(){
          var elem = {scroll_x : 0, scroll_y: 0};
          var canvas_panel = document.getElementById("canvas_panel");
          elem.scroll_x = canvas_panel.scrollLeft;
          elem.scroll_y = canvas_panel.scrollTop;
          return elem;
        };
        ///////////////////////////////////////
        _public.canvas_size = function () {
          var canv_size = {};
          canv_size.width = _public.schemes[_public.id].size.width;
          canv_size.height = _public.schemes[_public.id].size.height;
          schemeService.CanvasInitialization(canv_size.width, canv_size.height, 'canvas', _public.schemes[_public.id].image);
        };
        _public.canvas_size();

        _public.CreateScheme = function(){
          for(var scheme in _public.schemes){
            if(_public.id === scheme){
              _public.schemeData = _public.schemes[scheme];
            }
          }
          if(undefined !== _public.schemeData){
            for(var node in _public.schemeData.nodes){
              if(schemeService.NodeType.transit === _public.schemeData.nodes[node].type){
                _public.schemeData.nodes[node].border_color = schemeService.colorStateTrackt.OperationalActive;
                _public.schemeData.nodes[node].font = '18px Arial';
                _public.schemeData.nodes[node].text_color = 'orange';
              }
              if(schemeService.NodeType.location === _public.schemeData.nodes[node].type){
                _public.schemeData.nodes[node].border_color = schemeService.colorStateTrackt.ConnectingActive;
                _public.schemeData.nodes[node].need_attention = false;
                _public.schemeData.nodes[node].font = '18px Arial';
                _public.schemeData.nodes[node].text_color = 'blue';
                for(var state in _public.state){
                  if(_public.state[state].need_attention){
                    if(node === _public.state[state].location){
                      _public.schemeData.nodes[node].border_color = schemeService.colorStateTrackt.FailureActive;
                      _public.schemeData.nodes[node].need_attention = true;
                    }
                  }
                }
              }
              //_public.schemeData.nodes[node].draw_size = _public.schemeData.nodes[node].size.height/2;
              _public.schemeData.nodes[node].border_weight = 5;

            }
          }
          schemeService.changeFigure(_public.schemeData);
        };
        _public.CreateScheme();

         //Список помещений на данной схеме
        _public.PlacementList = function(scheme){
          return mobileSchemeService.PlacementList(scheme, _public.location);
        };

        // Список пользователей на данной схеме
        _public.UserList = function(scheme){
          _public.UserCount = _public.UserCountInLocation();
          return mobileSchemeService.UserList(scheme, _public.user, _public.UserTable, _public.location);
        };

        _public.UserCountInLocation = function() { mobileSchemeService.UserCountInLocation(_public.user, _public.HighLightInListId) };

        //// Методы для подсветки при наведении на списки (канвас-списки)
        _public.HighLightPlacementList = function(id, scheme){
          if(undefined !== scheme){
            for(var node in scheme.nodes){
              if(id == node){
                scheme.nodes[node].choice_elem = "True";
                schemeService.HighLighter(scheme, node, _public.GetCoord());
              }
            }
          }
        };

        _public.DisLightPlacementList = function(scheme){
          if(undefined !== scheme){
            for(var node in scheme.nodes){
              if(scheme.nodes[node].choice_elem){
                delete scheme.nodes[node].choice_elem;
              }
            }
          }
        };


        // методы маршрутизаторы
        _public.ToTable = function(){
          $location.path('/mobile');
        };

        _public.ToScheme = function(){
          $location.path('/scheme/main');
        };

        _public.SchemeTransit = function(scheme){
          mobileSchemeService.SchemeTransit(scheme, _public.MouseDownState, _public.GetCoord());
        };

        _public.LocationTransit = function(scheme){
          mobileSchemeService.LocationTransit(scheme, _public.MouseDownState, _public.GetCoord());
        };

        _public.HighLighter = function(scheme) {
          schemeService.HighLighter(scheme, _public.GetCoord());
          for(var node in scheme.nodes) {
            if ((_public.GetCoord().x > scheme.nodes[node].position.x) &&
              (_public.GetCoord().y > scheme.nodes[node].position.y) &&
              (_public.GetCoord().x < (scheme.nodes[node].position.x + scheme.nodes[node].size.width)) &&
              (_public.GetCoord().y < (scheme.nodes[node].position.y + scheme.nodes[node].size.height))) {
              _public.HighLightInListId = node;
            }
          }
        };

        //
        _public.LocationsStr = "Помещения";
        _public.UsersStr = "Пользователи";
        _public.flagLocations = true;
        _public.flagUsers = false;

        _public.ChoiceInSelect = function(item){
          if(_public.LocationsStr==item){
            _public.flagLocations = true;
            _public.flagUsers = false;
          }
          if(_public.UsersStr==item){
            _public.flagLocations = false;
            _public.flagUsers = true;
          }
        }

      };

      angular.module('app.scheme').
          controller('SchemeController', [
            '$scope',
            '$timeout',
            '$interval',
            '$location',
            '$routeParams',
            'cacheService',
            'mobileService',
            'schemeService',
            'mouseService',
            'mobileSchemeService',
            'schemeInfo',
            controller
          ]);
})();

