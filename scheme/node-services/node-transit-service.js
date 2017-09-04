'use strict';
(function() {

  var service = function(
    squareFactory
  )
  {
    var _public = this;

    _public.viewNodeTransit = function(NodeDevice, id, canvtools){
      squareFactory.strokeRect(
        canvtools,
        NodeDevice.position.x,
        NodeDevice.position.y,
        NodeDevice.size.width,
        NodeDevice.size.height,
        NodeDevice.border_color,
        NodeDevice.border_weight
      );
      squareFactory.fillRect(
        canvtools,
        NodeDevice.position.x,
        NodeDevice.position.y,
        NodeDevice.size.width,
        NodeDevice.size.height,
        NodeDevice.square_color
      );
      squareFactory.LabelInObject(
        canvtools,
        NodeDevice.label,
        id,
        NodeDevice.size.width,
        NodeDevice.size.height,
        NodeDevice.border_weight,
        NodeDevice.position.x,
        NodeDevice.position.y,
        NodeDevice.font,
        NodeDevice.text_color
      );
    };

    _public.viewNodeTransitHighLight = function(NodeDevice, HighLighter, canvtools){
      squareFactory.strokeRect(
        canvtools,
        NodeDevice.position.x,
        NodeDevice.position.y,
        NodeDevice.size.width,
        NodeDevice.size.height,
        HighLighter,
        NodeDevice.border_weight
      );
    };

    _public.viewNodeTransitDisHighLight = function(NodeDevice, canvtools){
      squareFactory.strokeRect(
        canvtools,
        NodeDevice.position.x,
        NodeDevice.position.y,
        NodeDevice.size.width,
        NodeDevice.size.height,
        NodeDevice.border_color,
        NodeDevice.border_weight
      );
    };
  };

  angular.module('app.components.scheme').service('nodeTransitService', [
    'squareFactory',
    service
  ]);
})();