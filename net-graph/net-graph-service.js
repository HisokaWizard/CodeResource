'use strict';
(function() {

  var service = function(
    $location,
    schemeService,
    mouseService
  )
  {
    var _public = this;

    _public.WatchHostsChange = function (cache_scheme, scheme, hosts, id) {
      var hostsLength = Object.size(hosts);
      var schemesNodesLength = 0;
      var schemesEdgesLength = 0;
      var cacheSchemesNodeLength = Object.size(cache_scheme[id].nodes);
      var cacheSchemesEdgesLength = Object.size(cache_scheme[id].edges);
      if ((undefined != scheme.nodes) || (undefined != scheme.edges)) {
        schemesNodesLength = Object.size(scheme.nodes);
        schemesEdgesLength = Object.size(scheme.edges);
      }
      if ((0 < hostsLength) && (undefined == scheme.nodes) && (undefined == scheme.edges)) {
        return "create";
      }
      if ((undefined != scheme.nodes) || (undefined != scheme.edges)) {
        if ((0 < hostsLength)
          && (cacheSchemesNodeLength === schemesNodesLength)
          && (cacheSchemesEdgesLength === schemesEdgesLength)) {
          return "update";
        }
        if ((0 < hostsLength)
          && (cacheSchemesNodeLength > schemesNodesLength)
          && (cacheSchemesEdgesLength > schemesEdgesLength)) {
          return "add";
        }
        if ((0 < hostsLength)
          && (cacheSchemesNodeLength < schemesNodesLength)
          && (cacheSchemesEdgesLength < schemesEdgesLength)) {
          return "delete";
        }
      }
    };

    _public.GoToNextScheme = function(scheme, coord, mouseDown){
      if (0 != Object.size(scheme.nodes)) {
        for (var node in scheme.nodes) {
          if(schemeService.NodeType.transit === scheme.nodes[node].type){
            if(undefined !== scheme.nodes[node].position && undefined!== scheme.nodes[node].size){
              if ((coord.x > scheme.nodes[node].position.x) &&
                (coord.y > scheme.nodes[node].position.y) &&
                (coord.x < (scheme.nodes[node].position.x + scheme.nodes[node].size.width)) &&
                (coord.y < (scheme.nodes[node].position.y + scheme.nodes[node].size.height))) {
                if (mouseService.MouseEventValue.down == mouseDown) {
                  $location.path("/net-graph/"+scheme.nodes[node].transitId);
                }
                break;
              }
            }
          }
        }
      }
    };

    _public.GoToDevice = function (scheme, coord, mouseDown) {
      if (0 != Object.size(scheme.nodes)) {
        for (var node in scheme.nodes) {
          if(undefined !== scheme.nodes[node].position && undefined!== scheme.nodes[node].size) {
            if ((coord.x > scheme.nodes[node].position.x) &&
              (coord.y > scheme.nodes[node].position.y) &&
              (coord.x < (scheme.nodes[node].position.x + scheme.nodes[node].size.width)) &&
              (coord.y < (scheme.nodes[node].position.y + scheme.nodes[node].size.height))) {
              if (mouseService.MouseEventValue.down == mouseDown) {
                $location.path("/devices/" + scheme.nodes[node].deviceType + "/" + node);
                break;
              }
            }
          }
        }
      }
    };

  };

  angular.module('app.netGraph').service('netGraphService', [
    '$location',
    'schemeService',
    'mouseService',
    service
  ]);
})();