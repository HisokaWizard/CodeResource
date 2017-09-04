'use strict';
(function() {

  var moduleName = 'app.components.scheme',
    serviceName =  'schemeObjectService',
    dependencies =
      ['schemeService'],
    service = function (
      schemeService
    )
    {
      var _public = this;

      _public.createDeviceData = function(cache, node_id, node, stateService){
        if(undefined != cache.devices.data){
          for (var device in cache.devices.data){
            if(node_id === device){
              if("" === node.label
                || null === node.label
                || undefined === node.label){
                node.label = cache.devices.data[node_id].label;
              }
              node.description = cache.devices.data[node_id].description;
              node.deviceType = cache.devices.data[node_id].type;
              node.state = stateService.getDeviceState(node_id);
              node.image = cache.devices.types[cache.devices.data[node_id].type]['image-url'];
              node.highlightimage = cache.devices.types[cache.devices.data[node_id].type]['high-light-image-url'];
              node.size = {
                width: cache.devices.types[cache.devices.data[node_id].type].size.width,
                height: cache.devices.types[cache.devices.data[node_id].type].size.height
              };
              node.border_weight = 6;
              node.border_rad = 10;
              node.font = '18px Arial';
              node.text_color = 'black';
              node.border_color = node.state.color;
              node.square_color = schemeService.colorStateTrackt.FillingOutTracktActive;
              break;
            } else {
              node = _public.createMoreData(node);
            }
          }
          //return node;
        }
        return node;
      };

      _public.createMoreData = function(node){
        if(undefined === node.size){
          node.size = {width: 50, height: 50};
        }
        node.font = '15px Arial';
        node.text_color = 'black';
        node.border_weight = 6;
        node.border_rad = 10;
        node.border_color = schemeService.colorStateTrackt.FillingOutTracktActive;
        node.square_color = schemeService.colorStateTrackt.FillingOutTracktActive;
        node.image = "";
        node.highlightimage = "";
        return node;
      };

      _public.createPortData = function(node, node_id, stateService, cache){
        for(var device in cache.devices.data){
          if(undefined !== cache.devices.data[device].ports){
            for(var port in cache.devices.data[device].ports){
              if(port === _public.parserPortComponentId(node_id) && device === _public.parserDeviceId(node_id)){
                node.state = stateService.getPortState(device, port);
                node.size = {width : 10, height : 10};
                node.border_color = node.state.color;
              }
            }
          }
        }
        return node;
      };

      _public.createTransitData = function(node){
        return _public.createMoreData(node);
      };

      _public.createGroupData = function(node){
        return _public.createMoreData(node);
      };

      _public.createComponentData = function(node){
        return _public.createMoreData(node);
      };

      _public.createLocationData = function(node){
        return _public.createMoreData(node);
      };

      _public.parserPortComponentId = function(id){
        var string = id.slice(id.indexOf('$')+1);
        return string;
      };

      _public.parserDeviceId = function(id){
        var string = id.slice(0, id.indexOf('$'));
        return string;
      };

    };

  dependencies.push(service);
  angular.module(moduleName).service(serviceName, dependencies);

})();