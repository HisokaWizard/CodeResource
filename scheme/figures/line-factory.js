'use strict';
(function() {

    var factory = function(){
        var LineObj = this;

        LineObj["EdgeLine"] = function(canvtools, weight, color, x1, y1,  x2, y2){
            canvtools.beginPath();
            canvtools.strokeStyle = color;
            canvtools.moveTo(x1+25, y1+25);
            canvtools.lineTo(x2+25, y2+25);
            canvtools.lineWidth = weight;
            canvtools.stroke();
        };

        LineObj["EdgeArc"] = function(canvtools, weight, color){

        };

        return LineObj;
    };

    angular.module('app.components.scheme').factory('lineFactory',[
        factory
    ]);
})();