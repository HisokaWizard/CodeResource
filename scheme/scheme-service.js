'use strict';
(function () {

  var service = function (
    $timeout,
    squareFactory,
    imageFactory,
    lineFactory,
    mouseService,
    nodeDeviceService,
    nodeTransitService,
    nodeLocationService,
    nodeGroupService,
    nodePortService,
    nodeComponentService,
    stateService,
    $q
  ) {
    var _public = this;
    var _private = {};

    _public.Figures = {};

    _public.CanvasInitialization = function (width, height, canvasId, image) {
      var initcanv = {};
      if (undefined == canvasId) {
        initcanv = document.getElementById('canvas');
      } else {
        initcanv = document.getElementById(canvasId);
      }
      if (null != initcanv) {
        _public.canvtools = initcanv.getContext('2d');
        _public.schemeImage = image;
        _public.Width = width;
        _public.Height = height;
        initcanv.width = width;
        initcanv.height = height;
      }
    };

    _public.changeFigure = function (figure_object) {
      _public.Figures = figure_object;
      _public.CanvasLoad();
    };

    _public.GetColorEdge = function (objNodes, edge) {
      if (objNodes[edge.target].state.status === objNodes[edge.source].state.status) {
        if(stateService.State.none.status === objNodes[edge.target].state.status){
          return stateService.State.none.color;
        }
        if(stateService.State.unknown.status === objNodes[edge.target].state.status){
          return stateService.State.unknown.color;
        }
        if(stateService.State.fault.status === objNodes[edge.target].state.status){
          return stateService.State.fault.color;
        }
        if(stateService.State.ok.status === objNodes[edge.target].state.status){
          return stateService.State.ok.color;
        }
      }
      if (objNodes[edge.target].state.status !== objNodes[edge.source].state.status) {
        if(stateService.State.none.status === objNodes[edge.target].state.status){
          return stateService.State.none.color;
        }
        if(stateService.State.none.status === objNodes[edge.source].state.status){
          return stateService.State.none.color;
        }
        if(stateService.State.unknown.status === objNodes[edge.target].state.status){
          return stateService.State.unknown.color;
        }
        if(stateService.State.unknown.status === objNodes[edge.source].state.status){
          return stateService.State.unknown.color;
        }
        if(stateService.State.fault.status === objNodes[edge.target].state.status){
          return stateService.State.fault.color;
        }
        if(stateService.State.fault.status === objNodes[edge.source].state.status){
          return stateService.State.fault.color;
        }
        if(stateService.State.ok.status === objNodes[edge.target].state.status){
          return stateService.State.ok.color;
        }
        if(stateService.State.ok.status === objNodes[edge.source].state.status){
          return stateService.State.ok.color;
        }
      }
    };

    _public.colorStateTrackt = {
      OperationalActive: "rgb(0, 128, 0)",   // green
      OperationalPassive: "rgb(113, 148, 113)",   // green alpha

      UnknownActive: "rgb(118, 118, 118)",  //grey
      UnknownPassive: "rgb(148, 147, 147)",  //grey alpha

      ConnectingActive: "rgb(255, 136, 0)", // orange
      ConnectingPassive: "rgb(165, 144, 105)", // orange alpha

      OfflineActive: "rgb(255, 255, 0)",  //yellow
      OfflinePassive: "rgb(174, 174, 125)",  //yellow alpha

      FailureActive: "rgb(255, 37, 32)",   //red
      FailurePassive: "rgb(218, 117, 99)",  //red alpha

      FillingInTracktActive: "rgb(111, 150, 111)", // green filling
      FillingInTracktPassive: "rgb(137, 158, 137)", // green filling

      FillingOutTracktActive: "rgb(210, 218, 227)", // grey filling
      FillingOutTracktPassive: "rgb(174, 181, 190)", // grey filling

      BackGroundComplanScheme: "rgba(100, 100, 100, 0.2)",
      HighLighter: "rgb(24, 248, 255)" // much light blue
    };

    _public.NodeType = {
      device: "device",
      component: "component",
      port: "port",
      transit: "transit",
      group: "group",
      location: "location",
      none: ""
    };

    _public.CanvasLoad = function () {
      _public.path_to_substrate = _public.schemeImage;
      _public.promise = undefined;
      if (undefined != _public.path_to_substrate && '' != _public.path_to_substrate) {
        _public.promise = imageFactory.draw_substrate(
          _public.canvtools,
          0,
          0,
          _public.Width,
          _public.Height,
          _public.path_to_substrate
        );
      }
      if (undefined == _public.promise) {
        var delay = $q.defer();
        delay.resolve();
        _public.promise = delay.promise;
      }
      _public.promise.then(function () {
        // Рисуем все связи
        for (var edge in _public.Figures.edges) {
          if ("line" === _public.Figures.edges[edge].form) {
            lineFactory.EdgeLine(
              _public.canvtools,
              _public.Figures.edges[edge].weight,
              _public.Figures.edges[edge].color,
              _public.Figures.nodes[_public.Figures.edges[edge].target].position.x,
              _public.Figures.nodes[_public.Figures.edges[edge].target].position.y,
              _public.Figures.nodes[_public.Figures.edges[edge].source].position.x,
              _public.Figures.nodes[_public.Figures.edges[edge].source].position.y
            );
          }
          if ("arc" === _public.Figures.edges[edge].form) {

          }
        }
        // Рисуем все узлы (devices, transit, room, group, component, port)
        for (var node in _public.Figures.nodes) {
          if (_public.NodeType.device === _public.Figures.nodes[node].type) {
            nodeDeviceService.viewNodeDevices(_public.Figures.nodes[node], node, _public.canvtools);
          }
          if (_public.NodeType.component === _public.Figures.nodes[node].type) {
            nodeComponentService.viewNodeComponent(_public.Figures.nodes[node], node, _public.canvtools);
          }
          if (_public.NodeType.port === _public.Figures.nodes[node].type) {
            nodePortService.viewNodePort(_public.Figures.nodes[node], node, _public.canvtools);
          }
          if (_public.NodeType.transit === _public.Figures.nodes[node].type) {
            nodeTransitService.viewNodeTransit(_public.Figures.nodes[node], node, _public.canvtools);
          }
          if (_public.NodeType.group === _public.Figures.nodes[node].type) {
            nodeGroupService.viewNodeGroup(_public.Figures.nodes[node], node, _public.canvtools);
          }
          if (_public.NodeType.location === _public.Figures.nodes[node].type) {
            nodeLocationService.viewNodeLocation(_public.Figures.nodes[node], node, _public.canvtools);
          }
          if (_public.NodeType.none === _public.Figures.nodes[node].type) {

          }
        }
      });
    };

    _public.DrawPartOfScheme = function(partOfScheme, node){
      if (_public.NodeType.device === partOfScheme.type) {
        nodeDeviceService.viewNodeUpdateDevices(partOfScheme, node, _public.canvtools);
      }
      if (_public.NodeType.component === partOfScheme.type) {
        nodeComponentService.viewNodeComponent(partOfScheme, node, _public.canvtools);
      }
      if (_public.NodeType.port === partOfScheme.type) {
        nodePortService.viewNodePortUpdate(partOfScheme, _public.canvtools);
      }
      if (_public.NodeType.transit === partOfScheme.type) {
        nodeTransitService.viewNodeTransit(partOfScheme, node, _public.canvtools);
      }
      if (_public.NodeType.group === partOfScheme.type) {
        nodeGroupService.viewNodeGroupUpdate(partOfScheme, node, _public.canvtools);
      }
      if (_public.NodeType.location === partOfScheme.type) {
        nodeLocationService.viewNodeLocation(partOfScheme, node, _public.canvtools);
      }
      if (_public.NodeType.none === partOfScheme.type) {

      }
    };

    _public.HighLighter = function (figureObj, coord) {
      if (undefined !== figureObj) {
        for (var node in figureObj.nodes) {
          if (_public.NodeType.group !== figureObj.nodes[node].type) {
            if ((coord.x > figureObj.nodes[node].position.x) &&
              (coord.y > figureObj.nodes[node].position.y) &&
              (coord.x < (figureObj.nodes[node].position.x + figureObj.nodes[node].size.width)) &&
              (coord.y < (figureObj.nodes[node].position.y + figureObj.nodes[node].size.height))) {
              _public.canvtools.beginPath();
              if (_public.NodeType.transit === _public.Figures.nodes[node].type) {
                nodeTransitService.viewNodeTransitHighLight(_public.Figures.nodes[node], _public.colorStateTrackt.HighLighter, _public.canvtools);
              }
              if (_public.NodeType.device === _public.Figures.nodes[node].type) {
                nodeDeviceService.viewNodeDevicesHighLight(_public.Figures.nodes[node],  _public.canvtools);
              }
              if (_public.NodeType.location === _public.Figures.nodes[node].type) {
                nodeLocationService.viewNodeLocationHighLight(_public.Figures.nodes[node], _public.colorStateTrackt.HighLighter, node, _public.canvtools);
              }
              if (_public.NodeType.group === _public.Figures.nodes[node].type) {
                nodeGroupService.viewNodeGroupHighLight(_public.canvtools);
              }
              if (_public.NodeType.port === _public.Figures.nodes[node].type) {
                nodePortService.viewNodePortHighLight(_public.Figures.nodes[node], _public.colorStateTrackt.HighLighter, _public.canvtools);
              }
              if (_public.NodeType.component === _public.Figures.nodes[node].type) {
                nodePortService.viewNodeComponentHighLight(_public.Figures.nodes[node], _public.colorStateTrackt.HighLighter, _public.canvtools);
              }
              _public.canvtools.stroke();
            }
            else {
              _public.canvtools.beginPath();
              if (_public.NodeType.transit === _public.Figures.nodes[node].type) {
                nodeTransitService.viewNodeTransitDisHighLight(_public.Figures.nodes[node], _public.canvtools);
              }
              if (_public.NodeType.device === _public.Figures.nodes[node].type) {
                nodeDeviceService.viewNodeDevicesDisHighLight(_public.Figures.nodes[node], _public.canvtools);
              }
              if (_public.NodeType.location === _public.Figures.nodes[node].type) {
                if(!_public.Figures.nodes[node].choice_elem){
                  nodeLocationService.viewNodeLocationDisHighLight(_public.Figures.nodes[node], node, _public.canvtools);
                }
                else{
                  nodeLocationService.viewNodeLocationDisHighLight(_public.Figures.nodes[node], node, _public.canvtools);
                }
              }
              if (_public.NodeType.group === _public.Figures.nodes[node].type) {
                nodeGroupService.viewNodeGroupDisHighLight(_public.canvtools);
              }
              if (_public.NodeType.port === _public.Figures.nodes[node].type) {
                nodePortService.viewNodePortDisHighLight(_public.Figures.nodes[node], _public.canvtools);
              }
              if (_public.NodeType.component === _public.Figures.nodes[node].type) {
                nodePortService.viewNodeComponentDisHighLight(_public.Figures.nodes[node], _public.canvtools);
              }
              _public.canvtools.stroke();

            }
          }
        }
      }
    };

  };

  angular.module('app.components.scheme').service('schemeService', [
    '$timeout',
    'squareFactory',
    'imageFactory',
    'lineFactory',
    'mouseService',
    'nodeDeviceService',
    'nodeTransitService',
    'nodeLocationService',
    'nodeGroupService',
    'nodePortService',
    'nodeComponentService',
    'stateService',
    '$q',
    service
  ]);
})();
