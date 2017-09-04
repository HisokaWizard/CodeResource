'use strict';
(function() {

  var moduleName = 'app.components.scheme',
    serviceName =  'nodePortService',
    dependencies =
      ['squareFactory', 'circleFactory'],
    service = function (
      squareFactory, circleFactory)
    {
      var _public = this;

      _public.viewNodePort = function(NodeDevice, id, canvtools){
        squareFactory.LabelUnderObject(
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
        squareFactory.fillRect(
          canvtools,
          NodeDevice.position.x,
          NodeDevice.position.y,
          NodeDevice.size.width,
          NodeDevice.size.height,
          NodeDevice.square_color
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

      _public.viewNodePortUpdate = function(NodeDevice, canvtools){
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

      _public.viewNodePortHighLight = function(NodeDevice, HighLighter, canvtools){
        squareFactory.fillRect(
          canvtools,
          NodeDevice.position.x,
          NodeDevice.position.y,
          NodeDevice.size.width,
          NodeDevice.size.height,
          HighLighter
        );
      };

      _public.viewNodePortDisHighLight = function(NodeDevice, canvtools){
        squareFactory.fillRect(
          canvtools,
          NodeDevice.position.x,
          NodeDevice.position.y,
          NodeDevice.size.width,
          NodeDevice.size.height,
          NodeDevice.square_color
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

  dependencies.push(service);
  angular.module(moduleName).service(serviceName, dependencies);

})();