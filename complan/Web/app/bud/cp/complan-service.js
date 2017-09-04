'use strict';
(function() {

  var service = function(resourceService,complanInfo)
  {
    var _public = this;
    _public.translate = function(str){
      for(var item in complanInfo.strings){
        if (item == str)
          return complanInfo.strings[item];
      }
      return resourceService.translate(str);
    };
  };
  angular.module('app.complan').service('complanService', [
    'resourceService',
    'complanInfo',
    service
  ]);
})();