'use strict';
(function() {

  var moduleName = 'app.components.scheme',
    serviceName =  'nodeComponentService',
    dependencies =
      [
        'squareFactory'
      ],
    service = function (
      squareFactory
      )
    {
      var _public = this;

      _public.viewNodeComponent = function(NodeDevice, id, canvtools){
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

      _public.viewNodeComponentHighLight = function(NodeDevice, HighLighter, canvtools){
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

      _public.viewNodeComponentDisHighLight = function(NodeDevice, canvtools){
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