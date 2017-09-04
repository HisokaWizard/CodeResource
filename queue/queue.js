'use strict';
(function() {
  angular.module('app.components.queue',[]).
    constant(
    'queueInfo', {
      id : 'queue'
    }
  ).
    run(function(){
      ////console.log(' - app.components.queue module loaded - ');
    });
})();
