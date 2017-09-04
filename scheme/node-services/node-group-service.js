'use strict';
(function() {

  var moduleName = 'app.components.scheme',
    serviceName =  'nodeGroupService',
    dependencies =
      [
        'squareFactory',
        'imageFactory'
      ],
    service = function(
      squareFactory,
      imageFactory
    )
    {
      var _public = this;

      _public.viewNodeGroup = function(NodeDevice, id, canvtools){
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
        //squareFactory.strokeRoundedRect(
        //  canvtools,
        //  NodeDevice.position.x,
        //  NodeDevice.position.y,
        //  NodeDevice.size.width,
        //  NodeDevice.size.height,
        //  NodeDevice.border_rad,
        //  NodeDevice.border_color,
        //  NodeDevice.border_weight
        //);
        squareFactory.fillRect(
          canvtools,
          NodeDevice.position.x,
          NodeDevice.position.y+NodeDevice.size.height+30,
          NodeDevice.size.width,
          20,
          NodeDevice.border_color
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

      _public.viewNodeGroupUpdate = function(NodeDevice, canvtools){
        //squareFactory.strokeRoundedRect(
        //  canvtools,
        //  NodeDevice.position.x,
        //  NodeDevice.position.y,
        //  NodeDevice.size.width,
        //  NodeDevice.size.height,
        //  NodeDevice.border_rad,
        //  NodeDevice.border_color,
        //  NodeDevice.border_weight
        //);
        squareFactory.fillRect(
          canvtools,
          NodeDevice.position.x,
          NodeDevice.position.y+NodeDevice.size.height+30,
          NodeDevice.size.width,
          20,
          NodeDevice.border_color
        );
      };

      _public.viewNodeGroupHighLight = function(NodeDevice, canvtools){
        imageFactory.draw_image(
          canvtools,
          NodeDevice.position.x,
          NodeDevice.position.y,
          NodeDevice.size.width,
          NodeDevice.size.height,
          NodeDevice.highlightimage
        );
      };

      _public.viewNodeGroupDisHighLight = function(NodeDevice, canvtools){
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