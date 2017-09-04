'use strict';
(function () {

  var factory = function ($http) {

    function Queue(updateData) {
      var _local = this;
      _local.updateData = updateData;
      _local.qId = '';
      _local.initialized = false;
      _local.registering = false;
      return _local;
    }

    Queue.prototype.registerQueue = function(aliveObj){
      var _public = aliveObj;

      if (_public.registering){
        return;
      }

      _public.registering = true;

      $http.post(_public.reqPath+'_register', '')
        .success(function (data, status, headers) {
          _public.qId = headers('Q-Name');
          _public.registering = false;
          if (null === _public.qId) {
            _public.qId = '';
            setTimeout(function(){
              _public.registerQueue(_public);
            }, 3000);
          } else {
            _public.pollQueue(_public);
          }
        })
        .error(function () {
          _public.registering = false;
          setTimeout(function(){
            _public.registerQueue(_public);
          }, 3000);
        });
    };

    Queue.prototype.pollQueue = function(aliveObj){
      var _public = aliveObj;

      $http.get(_public.reqPath + _public.qId)
        .success(function (data, status, headers) {
          var queuePath = headers("Q-Path");
          var queueMethod = headers("Q-Method");
          if (null === queuePath || null === queueMethod) {
            setTimeout(function(){
              _public.pollQueue(_public);
            }, 2000);
          } else {
            _public.updateData(queuePath, queueMethod, data);
            _public.pollQueue(_public);
          }
        })
        .error(function (data, status) {
          _public.qId = '';
          _public.registerQueue(_public);
        });
    };

    return Queue;
  };

  angular.module('app.components.queue').factory('queueFactory', [
    '$http',
    factory]);
})();