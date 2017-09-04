'use strict';
(function() {
  var service = function(
    schemeService,
    mouseService,
    servicesService,
    fulltextSearchService
  )
  {
    var _public = this,
      _private = {};

    _public.CreateActiveTract = function (key, scheme, service_data) {
      if(undefined !== service_data[key]){
        var special_object = {};
        if("" === service_data[key].encrypted_id || null === service_data[key].encrypted_id || undefined === service_data[key].encrypted_id){
          special_object.endpoints = {};
          for(var endpoint in service_data[key].endpoints){
            special_object.endpoints[endpoint] = endpoint;
          }
          if("" !== service_data[key]['in'].antenna_id || null !== service_data[key]['in'].antenna_id || undefined !== service_data[key]['in'].antenna_id){
            special_object.in_antenna_id = service_data[key]['in'].antenna_id;
          }
          if("" !== service_data[key]['in'].radio.id || null !== service_data[key]['in'].radio.id || undefined !== service_data[key]['in'].radio.id){
            special_object.in_radio_id = service_data[key]['in'].radio.id;
          }
          if("" !== service_data[key]['out'].antenna_id || null !== service_data[key]['out'].antenna_id || undefined !== service_data[key]['out'].antenna_id){
            special_object.out_antenna_id = service_data[key]['out'].antenna_id;
          }
          if("" !== service_data[key]['out'].radio.id || null !== service_data[key]['out'].radio.id || undefined !== service_data[key]['out'].radio.id){
            special_object.out_radio_id = service_data[key]['out'].radio.id;
          }
        } else {
          special_object.encrypted_id = service_data[key].encrypted_id;
          special_object.endpoints = {};
          for(var endpoint in service_data[key].endpoints){
            special_object.endpoints[endpoint] = endpoint;
          }
          if("" !== service_data[key]['in'].antenna_id || null !== service_data[key]['in'].antenna_id || undefined !== service_data[key]['in'].antenna_id){
            special_object.in_antenna_id = service_data[key]['in'].antenna_id;
          }
          if("" !== service_data[key]['in'].radio.id || null !== service_data[key]['in'].radio.id || undefined !== service_data[key]['in'].radio.id){
            special_object.in_radio_id = service_data[key]['in'].radio.id;
          }
          if("" !== service_data[key]['out'].antenna_id || null !== service_data[key]['out'].antenna_id || undefined !== service_data[key]['out'].antenna_id){
            special_object.out_antenna_id = service_data[key]['out'].antenna_id;
          }
          if("" !== service_data[key]['out'].radio.id || null !== service_data[key]['out'].radio.id || undefined !== service_data[key]['out'].radio.id){
            special_object.out_radio_id = service_data[key]['out'].radio.id;
          }
        }
      }
      return special_object;
    };

    _public.GetFormedTract = function (scheme, service_data, formedObj) {
      if (undefined != scheme) {
        for (var node_return in scheme.nodes) {
          scheme.nodes[node_return].square_color = schemeService.colorStateTrackt.FillingOutTracktActive;
        }
        for (var data in service_data) {
          if ("formed" == servicesService.getServiceState(data)) {
            formedObj.data = data;
            formedObj.formed = "formed";
          }
        }
        if (("empty" != formedObj.data) && ("formed" == formedObj.formed)) {
          _public.ActiveTrackt = [];
          _public.CreateActiveTract(formedObj.data, scheme, service_data, _public.ActiveTrackt);
          for (var node in scheme.nodes) {
            for (var i = 0; i < _public.ActiveTrackt.length; i++) {
              if (node == _public.ActiveTrackt[i]) {
                scheme.nodes[node].square_color = schemeService.colorStateTrackt.FillingInTracktActive;
              }
            }
          }
        }
        schemeService.changeFigure(scheme);
      }
    };

    _public.pick = function (key, scheme, services) {
      _private.pickedServiceId = key;
      if(undefined !== scheme){

        for (var node_return in scheme.nodes) {
          scheme.nodes[node_return].border_color = scheme.nodes[node_return].state.color;
          scheme.nodes[node_return].square_color = schemeService.colorStateTrackt.FillingOutTracktActive;
        }
        _public.formedObj = {data: "empty", formed: "empty"};
        for (var data in services) {
          if ("formed" === servicesService.getServiceState(data)) {
            _public.formedObj.data = data;
            _public.formedObj.formed = "formed";
          }
        }
        if (("empty" !== _public.formedObj.data) && ("formed" === _public.formedObj.formed)) {
          _public.ActiveTrackt = [];
          _public.CreateActiveTract(_public.formedObj.data, scheme, services, _public.ActiveTrackt);
          for (var node_in in scheme.nodes) {
            for (var k = 0; k < _public.ActiveTrackt.length; k++) {
              if (node_in === _public.ActiveTrackt[k]) {
                scheme.nodes[node_in].square_color = schemeService.colorStateTrackt.FillingInTracktActive;
              }
            }
          }
        }

        _public.ActiveTrackt = [];
        var activeTract = _public.CreateActiveTract(key, scheme, services, _public.ActiveTrackt);

        if(undefined !== activeTract.encrypted_id){
          scheme.edges = {};
          for(var endpoint in activeTract.endpoints){
            scheme.edges[endpoint] = {};
            if(undefined !== scheme.nodes[endpoint]){
              scheme.edges[endpoint].weight = scheme.nodes[endpoint].border_weight;
            } else {
              scheme.edges[endpoint].weight = 5;
            }
            scheme.edges[endpoint].form = "line";
            scheme.edges[endpoint].target = endpoint;
            scheme.edges[endpoint].source =  activeTract.encrypted_id;
            scheme.edges[endpoint].color =  schemeService.GetColorEdge(scheme.nodes, scheme.edges[endpoint]);
          }
          ////////////////// in, out - radio
          scheme.edges[activeTract.in_radio_id] = {};
          scheme.edges[activeTract.out_radio_id] = {};
          if(undefined !== scheme.nodes[activeTract.in_radio_id] && undefined !== scheme.nodes[activeTract.out_radio_id]){
            scheme.edges[activeTract.in_radio_id].weight = scheme.nodes[activeTract.in_radio_id].border_weight;
            scheme.edges[activeTract.out_radio_id].weight = scheme.nodes[activeTract.out_radio_id].border_weight;
          } else {
            scheme.edges[activeTract.in_radio_id].weight = 5;
            scheme.edges[activeTract.out_radio_id].weight = 5;
          }
          scheme.edges[activeTract.in_radio_id].form = "line";
          scheme.edges[activeTract.in_radio_id].target = activeTract.encrypted_id;
          scheme.edges[activeTract.in_radio_id].source =  activeTract.in_radio_id;
          scheme.edges[activeTract.in_radio_id].color =  schemeService.GetColorEdge(scheme.nodes, scheme.edges[activeTract.in_radio_id]);
          //
          scheme.edges[activeTract.out_radio_id].form = "line";
          scheme.edges[activeTract.out_radio_id].target = activeTract.encrypted_id;
          scheme.edges[activeTract.out_radio_id].source =  activeTract.out_radio_id;
          scheme.edges[activeTract.out_radio_id].color =  schemeService.GetColorEdge(scheme.nodes, scheme.edges[activeTract.out_radio_id]);
          ////////////////// in, out - antenna
          scheme.edges[activeTract.in_antenna_id] = {};
          scheme.edges[activeTract.out_antenna_id] = {};
          if(undefined !== scheme.nodes[activeTract.in_antenna_id] && undefined !== scheme.nodes[activeTract.out_antenna_id]){
            scheme.edges[activeTract.in_antenna_id].weight = scheme.nodes[activeTract.in_antenna_id].border_weight;
            scheme.edges[activeTract.out_antenna_id].weight = scheme.nodes[activeTract.out_antenna_id].border_weight;
          } else {
            scheme.edges[activeTract.in_antenna_id].weight = 5;
            scheme.edges[activeTract.out_antenna_id].weight = 5;
          }
          scheme.edges[activeTract.in_antenna_id].form = "line";
          scheme.edges[activeTract.in_antenna_id].target = activeTract.in_antenna_id;
          scheme.edges[activeTract.in_antenna_id].source =  activeTract.in_radio_id;
          scheme.edges[activeTract.in_antenna_id].color =  schemeService.GetColorEdge(scheme.nodes, scheme.edges[activeTract.in_antenna_id]);
          //
          scheme.edges[activeTract.out_antenna_id].form = "line";
          scheme.edges[activeTract.out_antenna_id].target = activeTract.out_antenna_id;
          scheme.edges[activeTract.out_antenna_id].source =  activeTract.out_radio_id;
          scheme.edges[activeTract.out_antenna_id].color =  schemeService.GetColorEdge(scheme.nodes, scheme.edges[activeTract.out_antenna_id]);
        } else {
          scheme.edges = {};
          for(var endpoint in activeTract.endpoints){
            scheme.edges[endpoint+"$"+activeTract.in_radio_id] = {};
            scheme.edges[endpoint+"$"+activeTract.out_radio_id] = {};
            if(undefined !== scheme.nodes[endpoint]){
              scheme.edges[endpoint+"$"+activeTract.in_radio_id].weight = scheme.nodes[endpoint].border_weight;
              scheme.edges[endpoint+"$"+activeTract.out_radio_id].weight = scheme.nodes[endpoint].border_weight;
            } else {
              scheme.edges[endpoint+"$"+activeTract.in_radio_id].weight = 5;
              scheme.edges[endpoint+"$"+activeTract.out_radio_id].weight = 5;
            }
            scheme.edges[endpoint+"$"+activeTract.in_radio_id].form = "line";
            scheme.edges[endpoint+"$"+activeTract.in_radio_id].target = endpoint;
            scheme.edges[endpoint+"$"+activeTract.in_radio_id].source =  activeTract.in_radio_id;
            scheme.edges[endpoint+"$"+activeTract.in_radio_id].color =  schemeService.GetColorEdge(scheme.nodes, scheme.edges[endpoint+"$"+activeTract.in_radio_id]);
            //
            scheme.edges[endpoint+"$"+activeTract.out_radio_id].form = "line";
            scheme.edges[endpoint+"$"+activeTract.out_radio_id].target = endpoint;
            scheme.edges[endpoint+"$"+activeTract.out_radio_id].source =  activeTract.out_radio_id;
            scheme.edges[endpoint+"$"+activeTract.out_radio_id].color =  schemeService.GetColorEdge(scheme.nodes, scheme.edges[endpoint+"$"+activeTract.out_radio_id]);
          }
          scheme.edges[activeTract.in_antenna_id] = {};
          scheme.edges[activeTract.out_antenna_id] = {};
          if(undefined !== scheme.nodes[activeTract.in_antenna_id] && undefined !== scheme.nodes[activeTract.out_antenna_id]){
            scheme.edges[activeTract.in_antenna_id].weight = scheme.nodes[activeTract.in_antenna_id].border_weight;
            scheme.edges[activeTract.out_antenna_id].weight = scheme.nodes[activeTract.out_antenna_id].border_weight;
          } else {
            scheme.edges[activeTract.in_antenna_id].weight = 5;
            scheme.edges[activeTract.out_antenna_id].weight = 5;
          }
          scheme.edges[activeTract.in_antenna_id].form = "line";
          scheme.edges[activeTract.in_antenna_id].target = activeTract.in_antenna_id;
          scheme.edges[activeTract.in_antenna_id].source =  activeTract.in_radio_id;
          scheme.edges[activeTract.in_antenna_id].color =  schemeService.GetColorEdge(scheme.nodes, scheme.edges[activeTract.in_antenna_id]);
          //
          scheme.edges[activeTract.out_antenna_id].form = "line";
          scheme.edges[activeTract.out_antenna_id].target = activeTract.out_antenna_id;
          scheme.edges[activeTract.out_antenna_id].source =  activeTract.out_radio_id;
          scheme.edges[activeTract.out_antenna_id].color =  schemeService.GetColorEdge(scheme.nodes, scheme.edges[activeTract.out_antenna_id]);
        }
      }
      schemeService.changeFigure(scheme);
    };

    _public.GetNewAlphaColor = function (ObjColor) {
      if (schemeService.colorStateTrackt.OperationalActive == ObjColor) return schemeService.colorStateTrackt.OperationalPassive;
      if (schemeService.colorStateTrackt.OfflineActive == ObjColor) return schemeService.colorStateTrackt.OfflinePassive;
      if (schemeService.colorStateTrackt.FailureActive == ObjColor) return schemeService.colorStateTrackt.FailurePassive;
      if (schemeService.colorStateTrackt.ConnectingActive == ObjColor) return schemeService.colorStateTrackt.ConnectingPassive;
      if (schemeService.colorStateTrackt.UnknownActive == ObjColor) return schemeService.colorStateTrackt.UnknownPassive;

      if (schemeService.colorStateTrackt.FillingInTracktActive == ObjColor) return schemeService.colorStateTrackt.FillingInTracktPassive;
      if (schemeService.colorStateTrackt.FillingOutTracktActive == ObjColor) return schemeService.colorStateTrackt.FillingOutTracktPassive;
    };

    _public.SchemeTractList = function (scheme, service_data) {
      var Obj = {};
      for (var tract in scheme.nodeowner) {
        for (var data in service_data) {
          if (tract == data) {
            Obj[tract] = {};
            Obj[tract].key = tract;
            Obj[tract].label = service_data[data].label;
          }
        }
      }
      return Obj;
    };

    _public.getFilter = function (service_data, label) {
      return function (item) {
        var match = true;
        if (undefined != service_data) {
          if ((undefined != service_data[item].label)
            && (undefined != label)
            && ("" != service_data[item].label)
            && ("" != label)) {
            if (-1 != fulltextSearchService.fullTextSearch(label.toLowerCase(), service_data[item].label.toLowerCase())) {
              match = true;
            }
            else {
              match = false;
            }
          }
        }
        return match;
      }
    };

    _public.listTract = function(scheme, services, dataAboutDevice, node) {
      for(var tract in scheme.nodeowner){
        for (var data in services.data) {
          if (tract === data) {
            if ("" !== services.data[data].encrypted_id) {
              if (scheme.nodes[node].deviceType === services.data[data].encrypted_id) {
                dataAboutDevice.listTracts[tract] = {};
                dataAboutDevice.listTracts[tract].label = services.data[data].label;
                break;
              }
            }
            if ("" !== services.data[data].switch_id) {
              if (node === services.data[data].switch_id) {
                dataAboutDevice.listTracts[tract] = {};
                dataAboutDevice.listTracts[tract].label = services.data[data].label;
                break;
              }
            }
            for (var endpoint in services.data[data].endpoints) {
              if (node === endpoint) {
                dataAboutDevice.listTracts[tract] = {};
                dataAboutDevice.listTracts[tract].label = services.data[data].label;
                break;
              }
            }
            if ("" !== services.data[data]['in'].antenna_id) {
              if (node === services.data[data]['in'].antenna_id) {
                dataAboutDevice.listTracts[tract] = {};
                dataAboutDevice.listTracts[tract].label = services.data[data].label;
                break;
              }
            }
            if ("" !== services.data[data]['out'].antenna_id) {
              if (services.data[data]['in'].antenna_id !== services.data[data]['out'].antenna_id) {
                if (node === services.data[data]['out'].antenna_id) {
                  dataAboutDevice.listTracts[tract] = {};
                  dataAboutDevice.listTracts[tract].label = services.data[data].label;
                  break;
                }
              }
            }
            if ("" !== services.data[data]['in'].radio.id) {
              if (node === services.data[data]['in'].radio.id) {
                dataAboutDevice.listTracts[tract] = {};
                dataAboutDevice.listTracts[tract].label = services.data[data].label;
                break;
              }
            }
            if ("" !== services.data[data]['out'].radio.id) {
              if (services.data[data]['in'].radio.id !== services.data[data]['out'].radio.id) {
                if (node === services.data[data]['out'].radio.id) {
                  dataAboutDevice.listTracts[tract] = {};
                  dataAboutDevice.listTracts[tract].label = services.data[data].label;
                  break;
                }
              }
            }
          }
        }
      }
      return dataAboutDevice;
    }
  };

  angular.module('app.complan').service('schemeComplanService', [
    'schemeService',
    'mouseService',
    'servicesService',
    'fulltextSearchService',
    service
  ]);
})();