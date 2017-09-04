'use strict';
(function() {
  var controller = function($scope)
  {
    var _public = this,
        _private = {};
    //functions
    $scope.$watch(function(){
      return _public.label;
    },function(){
      $scope.$emit('cp::sets::filter', _public.label);
    });
    _public.reset = function(){
      _public.label = "";
    };
    //members
    _public.label = undefined;
  };
  angular.module('app.complan').controller('setsFilterController', [
    '$scope',
    controller
  ]);
})();