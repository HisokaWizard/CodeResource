'use strict';
(function() {
//
  var service = function(
    $http,
    queueFactory)
  {
    var _public = this;

    _public.queue = new queueFactory(updateData);
    _public.cache = {};
    _public.queue.reqPath = "_notify/";
    _public.queue.registerQueue(_public.queue);

    function updateData(path, method, data){
      var _local = _public.queue;
      var tmpNode = _public.cache;
      var tmpParent = [];
      var pathArray = path.split('/');

      for (var i = 1; i < pathArray.length; i++) {
        if ('' == pathArray[i]) {
          break;
        }
        if (null == tmpNode[pathArray[i]]) {
          tmpNode[pathArray[i]] = {};
        }
        tmpParent = tmpNode;
        tmpNode = tmpNode[pathArray[i]];
      }

      if ("DELETE" == method) {
        delete tmpParent[pathArray[pathArray.length - 1]];
      } else if ("PUT" == method) {
        if ("string" == typeof(data)) //костылик, нужен для обновления проперти, когда в path прописан путь к листу json-дерева
        {
          var start = data.indexOf('"');
          var stop = data.indexOf('"', start + 1);
          if (start < stop){
            tmpParent[pathArray[pathArray.length - 1]] = data.slice(start + 1, stop);
          }
          else{
            tmpParent[pathArray[pathArray.length - 1]] = data;
          }
        }
        else{
          Object.assign(tmpNode, data);
        }
      }

      if (!_local.initialized) {
        _local.initialized = true;
      }
    }

  };

  angular.module('app.components.cache').service('cacheService', [
    '$http',
    'queueFactory',
    service]);
})();
