'use strict';
(function () {

  var moduleName = 'app.netGraph',
    controllerName =  'netGraphCtrl',
    dependencies =
      [
        '$scope',
        '$location',
        '$routeParams',
        'cacheService',
        'schemeService',
        'mouseService',
        'locationService',
        'netGraphInfo',
        'netGraphService',
        'stateService',
        'schemeObjectService'
      ],
    controller = function (
      $scope,
      $location,
      $routeParams,
      cacheService,
      schemeService,
      mouseService,
      locationService,
      netGraphInfo,
      netGraphService,
      stateService,
      schemeObjectService
    )
    {
      var _public = this;
      var _private = {};
      _public.schemes = {};
      _public.cache = cacheService.cache;
      _public.id = $routeParams.id;
      _public.breadcrumb = netGraphInfo.breadcrumb;
      _public.schemeLabel = undefined;
      $scope.$watch(function(){
        return _public.id;
      },function(){
        for(var id in _public.cache.schemes){
          if(id == _public.id){
            _public.schemeLabel = _public.cache.schemes[id].label;
            return;
          }
        }
      });
      // mouse event handlers
      _public.canvas_coord = {x: 0, y: 0};
      _public.MouseDownState = "";
      _public.MouseUpState = "";
      _public.MouseOverState = "";
      _public.MouseLeaveState = "";
      _public.OnMouseMove = function (event) {
        var new_cord = mouseService.onMouseMove(event);
        _public.MouseDownState = "";
        _public.MouseUpState = "";
        _public.MouseOverState = "";
        _public.MouseLeaveState = "";
        _public.canvas_coord.x = new_cord.x;
        _public.canvas_coord.y = new_cord.y;
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
        return _public.MouseLeaveState = mouseService.MouseLeave(event);
      };
      ///////////////////////////////////////
      _public.viewCoordFlag = false;
      _public.ViewCoord = function () {
        if (!_public.viewCoordFlag) {
          _public.viewCoordFlag = true;
        } else {
          _public.viewCoordFlag = false;
        }
      };
      var destructorTypes = $scope.$watch(
        function () {
          return cacheService.cache.types;
        },
        function () {
          if (undefined != cacheService.cache.types) {
            _private.types = cacheService.cache.types;
            destructorTypes();
          }
        });

      var destructorSchemes = $scope.$watch(
        function () {
          return cacheService.cache.schemes;
        },
        function () {
          if (undefined != cacheService.cache.schemes) {
            _public.cache.schemes = cacheService.cache.schemes;
            for(var scheme in _public.cache.schemes){
              if(undefined !== _public.cache.schemes[scheme]){
                if(undefined === _public.id || "" === _public.id){
                  _public.id = scheme;
                  $location.path("/net-graph/"+_public.id);
                  break;
                }
              }
            }
            _public.canvas_size();
            destructorSchemes();
          }
        });

      $scope.$watch(
        function () {
          return cacheService.cache;
        },
        function () {
          if (undefined != cacheService.cache.hosts) {
            _public.hosts = cacheService.cache.hosts;
            if (undefined != _public.cache.schemes) {
              var change = _public.WatchHostsChange();
              if ("" == change) {
                return;
              }
              if ("create" == change) {
                _public.CreateGraphs();
              }
              if ("update" == change) {
                _public.UpdateGraphs();
              }
              if ("add" == change) {
                _public.AddGraphs();
              }
              if ("delete" == change) {
                _public.DeleteGraphs();
              }
            }
          }
        },
        true);

      _public.elements = function () {
        locationService.showElements();
      };

      _public.eventsLog = function(){
        locationService.showEventLog();
      };

      _public.alarmsLog = function(){
        locationService.showAlarmLog();
      };

      _public.WatchHostsChange = function () {
        return netGraphService.WatchHostsChange(_public.cache.schemes, _public.schemes, _public.hosts, _public.id);
      };

      _public.canvas_size = function () {
        var canv_size = {};
        canv_size.width = _public.cache.schemes[_public.id].size.width;
        canv_size.height = _public.cache.schemes[_public.id].size.height;
        schemeService.CanvasInitialization(canv_size.width, canv_size.height, 'canvas', _public.cache.schemes[_public.id].image);
      };

      _public.CreateGraphs = function () {
        _public.schemes["nodes"] = _public.cache.schemes[_public.id].nodes;
        _public.schemes["edges"] = _public.cache.schemes[_public.id].edges;
        for (var node in _public.schemes.nodes) {
          if(schemeService.NodeType.device === _public.schemes.nodes[node].type){
            _public.schemes.nodes[node] = schemeObjectService.createDeviceData(cacheService.cache, node, _public.schemes.nodes[node], stateService);
          }
          if(schemeService.NodeType.port === _public.schemes.nodes[node].type){
            _public.schemes.nodes[node] = schemeObjectService.createPortData(_public.schemes.nodes[node], node, stateService, cacheService.cache);
          }
          if(schemeService.NodeType.component === _public.schemes.nodes[node].type){
            _public.schemes.nodes[node] = schemeObjectService.createComponentData(_public.schemes.nodes[node]);
          }
          if(schemeService.NodeType.transit === _public.schemes.nodes[node].type){
            _public.schemes.nodes[node] = schemeObjectService.createTransitData(_public.schemes.nodes[node]);
          }
          if(schemeService.NodeType.group === _public.schemes.nodes[node].type){
            _public.schemes.nodes[node] = schemeObjectService.createGroupData(_public.schemes.nodes[node]);
          }
          if(schemeService.NodeType.location === _public.schemes.nodes[node].type){
            _public.schemes.nodes[node] = schemeObjectService.createLocationData(_public.schemes.nodes[node]);
          }
        }
        schemeService.changeFigure(_public.schemes);
      };

      _public.AddGraphs = function () {
      };

      _public.DeleteGraphs = function () {
      };

      _public.UpdateGraphs = function () {
        for (var node in _public.schemes.nodes) {
          if(schemeService.NodeType.device === _public.schemes.nodes[node].type){
            _public.schemes.nodes[node].state = stateService.getDeviceState(node);
            _public.schemes.nodes[node].border_color = _public.schemes.nodes[node].state.color;
            schemeService.DrawPartOfScheme(_public.schemes.nodes[node], node);
          }
          if(schemeService.NodeType.transit === _public.schemes.nodes[node].type){
            _public.schemes.nodes[node].border_color = schemeService.colorStateTrackt.FillingOutTracktActive;
            schemeService.DrawPartOfScheme(_public.schemes.nodes[node], node);
          }
          if(schemeService.NodeType.port === _public.schemes.nodes[node].type){
            _public.schemes.nodes[node].border_color = schemeService.colorStateTrackt.FillingOutTracktActive;
            schemeService.DrawPartOfScheme(_public.schemes.nodes[node], node);
          }
        }
      };

      _public.GoToNextScheme = function(){
        netGraphService.GoToNextScheme(_public.schemes, _public.GetCoord(), _public.MouseDownState);
      };

      _public.GoToDevice = function(){
        netGraphService.GoToDevice(_public.schemes, _public.GetCoord(), _public.MouseDownState);
      };

      _public.HighLighter = function(scheme) { schemeService.HighLighter(scheme, _public.GetCoord()); };

    };

  dependencies.push(controller);
  angular.module(moduleName).controller(controllerName, dependencies);
})();