'use strict';
(function() {

  var moduleName = 'app.components.scheme',
    serviceName =  'nodeDeviceService',
    dependencies =
      ['squareFactory', 'imageFactory'],
    service = function (
      squareFactory, imageFactory)
    {
      var _public = this;

      _public.viewNodeDevices = function(NodeDevice, id, canvtools){
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
        squareFactory.strokeRoundedRect(
          canvtools,
          NodeDevice.position.x,
          NodeDevice.position.y,
          NodeDevice.size.width,
          NodeDevice.size.height,
          NodeDevice.border_rad,
          NodeDevice.border_color,
          NodeDevice.border_weight
        );
        imageFactory.draw_image(
          canvtools,
          NodeDevice.position.x,
          NodeDevice.position.y,
          NodeDevice.size.width,
          NodeDevice.size.height,
          NodeDevice.image
        );
      };

      _public.viewNodeUpdateDevices = function(NodeDevice, id, canvtools){
        squareFactory.strokeRoundedRect(
          canvtools,
          NodeDevice.position.x,
          NodeDevice.position.y,
          NodeDevice.size.width,
          NodeDevice.size.height,
          NodeDevice.border_rad,
          NodeDevice.border_color,
          NodeDevice.border_weight
        );
      };

      _public.viewNodeDevicesHighLight = function(NodeDevice, canvtools){
        imageFactory.draw_image(
          canvtools,
          NodeDevice.position.x,
          NodeDevice.position.y,
          NodeDevice.size.width,
          NodeDevice.size.height,
          NodeDevice.highlightimage
        );
      };

      _public.viewNodeDevicesDisHighLight = function(NodeDevice, canvtools){
        imageFactory.draw_image(
          canvtools,
          NodeDevice.position.x,
          NodeDevice.position.y,
          NodeDevice.size.width,
          NodeDevice.size.height,
          NodeDevice.image
        );
      };
    };

  dependencies.push(service);
  angular.module(moduleName).service(serviceName, dependencies);
})();