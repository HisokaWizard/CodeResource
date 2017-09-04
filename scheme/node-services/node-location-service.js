'use strict';
(function() {

  var service = function(
    squareFactory
  )
  {
    var _public = this;

    _public.viewNodeLocation = function(NodeDevice, id, canvtools){
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

    _public.viewNodeLocationHighLight = function(NodeDevice, HighLighter, id, canvtools){
      squareFactory.fillRect(
        canvtools,
        NodeDevice.position.x,
        NodeDevice.position.y,
        NodeDevice.size.width,
        NodeDevice.size.height,
        HighLighter
      );
      squareFactory.LabelInObject(
        canvtools,
        NodeDevice.label,
        id,
        NodeDevice.size.width,
        NodeDevice.size.height+6,
        NodeDevice.border_weight,
        NodeDevice.position.x,
        NodeDevice.position.y,
        NodeDevice.font,
        NodeDevice.text_color
      );
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

    _public.viewNodeLocationDisHighLight = function(NodeDevice, id, canvtools){
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
        NodeDevice.size.height+6,
        NodeDevice.border_weight,
        NodeDevice.position.x,
        NodeDevice.position.y,
        NodeDevice.font,
        NodeDevice.text_color
      );
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

  angular.module('app.components.scheme').service('nodeLocationService', [
    'squareFactory',
    service
  ]);
})();