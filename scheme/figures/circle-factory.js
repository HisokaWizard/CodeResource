'use strict';
(function() {


  var moduleName = 'app.components.scheme',
    factoryName =  'circleFactory',

    factory = function (){
      var CircleObj = {};

      CircleObj["fillCircle"] = function(canvastools, x, y, radius, color){
        canvastools.beginPath();
        canvastools.fillStyle = color;
        canvastools.arc(x, y, radius, 0, Math.PI*2, false);
        canvastools.closePath();
        canvastools.fill();
      };

      CircleObj["strokeCircle"] = function(canvastools, x, y, radius, color){
        canvastools.beginPath();
        canvastools.strokeStyle = color;
        canvastools.lineWidth = 2;
        canvastools.arc(x, y, radius, 0, Math.PI*2, false);
        canvastools.closePath();
        canvastools.stroke();
      };

      return CircleObj;
    };

  angular.module(moduleName).factory(factoryName,[factory]);
})();