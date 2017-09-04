'use strict';
(function() {
  var controller = function($scope)
  {
    var _public = this,
      _private = {};
    //functions

    $scope.$watch(function(){
      return _public.filter;
    },function(){
      $scope.$emit('cp::services::filter', _public.filter);
    },true);
    _public.reset = function(){
      _public.filter = {
        label : undefined,
        endpoints : {},
        encryption : undefined,
        frequencyRange : undefined,
        state : undefined
      };
    };
    //members
    _public.filter = {
      label : undefined,
      endpoints : {},
      encryption : undefined,
      frequencyRange : undefined,
      state : undefined
    };
  };

  angular.module('app.complan').controller('servicesFilterController', [
    '$scope',
    controller
  ]);
})();