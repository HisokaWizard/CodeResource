'use strict';
(function () {

  var moduleName = 'app.complan',
    controllerName =  'schemeController',
    dependencies =
      [
        '$scope',
        '$location',
        'cacheService',
        'schemeService',
        'servicesService',
        'mouseService',
        'fulltextSearchService',
        'schemeComplanService',
        'stateService',
        'schemeObjectService'
      ],
    controller = function (
      $scope,
      $location,
      cacheService,
      schemeService,
      servicesService,
      mouseService,
      fulltextSearchService,
      schemeComplanService,
      stateService,
      schemeObjectService
    )
    {
      var _public = this;
      _public.ActiveTrackt = [];

      // mouse event handlers
      _public.canvas_coord = {x: 0, y: 0};
      _public.MouseDownState = "";
      _public.MouseUpState = "";

      _public.OnMouseMove = function (event) {
        var new_cord = mouseService.onMouseMove(event);
        _public.MouseDownState = "";
        _public.MouseUpState = "";
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
      //////////////////////////////////////

      var destructorGraphs = $scope.$watch(
        function () {
          return cacheService.cache.schemes;
        },
        function () {
          if (undefined != cacheService.cache.schemes) {
            _public.cache.schemes = cacheService.cache.schemes;
            destructorGraphs();
          }
        });


      var destructorSchemes = $scope.$watch(
        function () {
          return cacheService.cache.bud.cp.schemes;
        },
        function () {
          if (undefined != cacheService.cache.bud.cp.schemes) {
            _public.schemes = cacheService.cache.bud.cp.schemes;
            if ((undefined != _public.schemes) && (undefined != _public.setsData) && (undefined != _public.setsState)) {
              _public.CreateScheme();
              destructorSchemes();
            }
          }
        },
        true);

      var destructorServices = $scope.$watch(
        function () {
          return cacheService.cache.bud.cp.services;
        },
        function () {
          if (undefined != cacheService.cache.bud.cp.services) {
            _public.services = cacheService.cache.bud.cp.services;
            _public.formedObj = {data: "empty", formed: "empty"};
            schemeComplanService.GetFormedTract(_public.useScheme, _public.services.data, _public.formedObj);
            destructorServices();
          }
        },
        true);

      _public.getFilter = function () {
        if(undefined !== _public.label && undefined !== _public.services.data) {
          return schemeComplanService.getFilter(_public.services.data, _public.label);
        }
      };

      _public.isLongList = function (list) {
        return 8 < Object.size(list);
      };

      _public.CreateScheme = function () {
        for (var data in _public.setsData) {
          if (data === _public.setsState.current) {
            _public.scheme_id = _public.setsData[data].scheme_id;
          }
        }
        for (var scheme in _public.schemes) {
          if (_public.scheme_id === scheme) {
            _public.useScheme = _public.schemes[scheme];
            for(var setdata in _public.setsData){
              if(setdata === _public.setsState.current){
                _public.useScheme.nodeowner = {};
                _public.useScheme.nodeowner = _public.setsData[setdata].services;
              }
            }
          }
        }
        for (var scheme in _public.schemes) {
          for (var s_node in _public.schemes[scheme].nodes) {
            for (var node in _public.useScheme.nodes) {
              if (node === s_node) {
                if (schemeService.NodeType.device === _public.useScheme.nodes[node].type) {
                  _public.useScheme.nodes[node] = schemeObjectService.createDeviceData(
                    cacheService.cache,
                    node,
                    _public.useScheme.nodes[node]
                    , stateService);
                }
                if(schemeService.NodeType.port === _public.useScheme.nodes[node].type){
                  _public.useScheme.nodes[node] = schemeObjectService.createPortData(_public.useScheme.nodes[node], node, stateService, cacheService.cache);
                }
                if(schemeService.NodeType.component === _public.useScheme.nodes[node].type){
                  _public.schemes.nodes[node] = schemeObjectService.createComponentData(_public.schemes.nodes[node]);
                }
                if(schemeService.NodeType.transit === _public.useScheme.nodes[node].type){
                  _public.useScheme.nodes[node] = schemeObjectService.createTransitData(_public.useScheme.nodes[node]);
                }
                if(schemeService.NodeType.group === _public.useScheme.nodes[node].type){
                  _public.schemes.nodes[node] = schemeObjectService.createGroupData(_public.schemes.nodes[node]);
                }
                if(schemeService.NodeType.location === _public.useScheme.nodes[node].type){
                  _public.schemes.nodes[node] = schemeObjectService.createLocationData(_public.schemes.nodes[node]);
                }
              }
            }
          }
        }
        for (var edge in _public.useScheme.edges) {
          _public.useScheme.edges[edge].weight = 4;
          _public.useScheme.edges[edge].color = schemeService.colorStateTrackt.BackGroundComplanScheme;
          _public.useScheme.edges[edge].form = "line";
        }
        _public.canvas_size();
        schemeService.changeFigure(_public.useScheme);
      };

      _public.canvas_size = function () {
        _public.WinHeight = document.documentElement.clientHeight;
        var canv_size = {};
        canv_size.width = _public.useScheme.size.width;
        canv_size.height = _public.useScheme.size.height;
        schemeService.CanvasInitialization(canv_size.width, canv_size.height, 'canvas', _public.useScheme.image);
      };

      _public.GetNewAlphaColor = function(ObjColor){
        schemeComplanService.GetNewAlphaColor(ObjColor);
      };

      _public.SchemeTractList = function () {
        if(undefined !== _public.useScheme && undefined !== _public.services.data) {
          schemeComplanService.SchemeTractList(_public.useScheme, _public.services.data);
        }
      };

      _public.pick = function (key) {
        _public.isPick = {key : key, pick : true};
        if(undefined !== _public.useScheme && undefined !== _public.services.data) {
          schemeComplanService.pick(key, _public.useScheme, _public.services.data);
        }
      };

      _public.isPicked = function (key) {
        if(undefined !== _public.isPick){
          if(key === _public.isPick.key && _public.isPick.pick) {
            return true;
          }
        } else {
          return false;
        }
      };

      _public.GoToDevice = function(){
        if(undefined != _public.useScheme) {
          for (var node in _public.useScheme.nodes) {
            var coord_obj = _public.GetCoord();
            if ((coord_obj.x > _public.useScheme.nodes[node].position.x) &&
              (coord_obj.y > _public.useScheme.nodes[node].position.y) &&
              (coord_obj.x < (_public.useScheme.nodes[node].position.x + _public.useScheme.nodes[node].size.width)) &&
              (coord_obj.y < (_public.useScheme.nodes[node].position.y + _public.useScheme.nodes[node].size.height))) {
              if (mouseService.MouseEventValue.down == _public.MouseDownState) {
                _public.dataAboutDevice = {};
                _public.dataAboutDevice.elemName = _public.useScheme.nodes[node].label;
                _public.dataAboutDevice.linkToSettings = "/devices/" + _public.useScheme.nodes[node].deviceType + "/" + node;
                _public.dataAboutDevice.listTracts = {};
                _public.dataAboutDevice = schemeComplanService.listTract(_public.useScheme, _public.services, _public.dataAboutDevice, node);
                _public.MouseDownState = null;
                _public.modalShow = true;
                break;
              }
            }
          }
        }
      };

      _public.closedButton = function () {
        _public.modalShow = false;
      };

      _public.goToSettings = function (path) {
        $location.path(path);
      };

      _public.HighLighter = function() {
        if(undefined !== _public.useScheme){
          schemeService.HighLighter(_public.useScheme, _public.GetCoord());
        }
      };

      // members
      _public.cache = cacheService.cache;
      _public.hosts = cacheService.cache.hosts;
      _public.setsData = cacheService.cache.bud.cp.sets.data;
      _public.setsState = cacheService.cache.bud.cp.sets.state;
      _public.services = cacheService.cache.bud.cp.services;
      _public.schemeTractList = {};
      _public.label = undefined;
      _public.dataAboutDevice = {};
      _public.modalShow = false ;
    };

  dependencies.push(controller);
  angular.module(moduleName).controller(controllerName, dependencies);

})();