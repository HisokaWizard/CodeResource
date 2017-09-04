'use strict';
(function() {

var factory = function(){
    var SquareObj = {};

    SquareObj["fillRoundedRect"] = function(canvtools, x, y, width, height, rad, border_color, color, border_weight){
        canvtools.beginPath();
        canvtools.moveTo(x+rad, y);
        canvtools.lineTo(x+width-rad, y);
        canvtools.quadraticCurveTo(x+width, y, x+width, y+rad);
        canvtools.lineTo(x+width, y+height-rad);
        canvtools.quadraticCurveTo(x+width, y+height, x+width-rad, y+height);
        canvtools.lineTo(x+rad, y+height);
        canvtools.quadraticCurveTo(x, y+height, x, y+height-rad);
        canvtools.lineTo(x, y+rad);
        canvtools.quadraticCurveTo(x, y, x+rad, y);
        canvtools.fill();

        canvtools.fillStyle = color;
        canvtools.fill();
        canvtools.lineWidth = border_weight;
        canvtools.strokeStyle = border_color;
        canvtools.stroke();
    };

    SquareObj["strokeRoundedRect"] = function(canvtools, x, y, width, height, rad, border_color, border_weight){
        canvtools.beginPath();
        canvtools.moveTo(x+rad, y);
        canvtools.lineTo(x+width-rad, y);
        canvtools.quadraticCurveTo(x+width, y, x+width, y+rad);
        canvtools.lineTo(x+width, y+height-rad);
        canvtools.quadraticCurveTo(x+width, y+height, x+width-rad, y+height);
        canvtools.lineTo(x+rad, y+height);
        canvtools.quadraticCurveTo(x, y+height, x, y+height-rad);
        canvtools.lineTo(x, y+rad);
        canvtools.quadraticCurveTo(x, y, x+rad, y);
        canvtools.lineWidth = border_weight;
        canvtools.strokeStyle = border_color;
        canvtools.stroke();
    };

    SquareObj["strokeRect"] = function(canvtools, x, y, width, height, border_color, border_weight){
        canvtools.beginPath();
        canvtools.strokeStyle = border_color;
        canvtools.lineWidth = border_weight;
        canvtools.strokeRect(x, y, width, height);
        canvtools.stroke();
    };

    SquareObj["fillRect"] = function(canvtools, x, y, width, height, border_color){
        canvtools.beginPath();
        canvtools.fillStyle = border_color;
        canvtools.fillRect(x, y, width, height);
        canvtools.fill();
    };

    SquareObj["LabelUnderObject"] = function(canvtools, label, id, width, height, border_weight, x, y, font, text_color){
        var _x = x + width/2, _y = y + height + border_weight*2 + 13;
        canvtools.beginPath();
        canvtools.font = font;
        canvtools.fillStyle = text_color;
        canvtools.strokeStyle = 'white';
        canvtools.textAlign = 'center';
        if(false == window.isOldBrowser){
            canvtools.fillText(label, _x,_y);
        }
        if(window.isOldBrowser){
            canvtools.fillText(id, _x,_y);
        }
    };

    SquareObj["LabelInObject"] = function(canvtools, label, id, width, height, border_weight, x, y, font, text_color){
        canvtools.beginPath();
        canvtools.font = font;
        canvtools.textAlign = 'center';
        canvtools.fillStyle = text_color;
        canvtools.strokeStyle = 'white';
        var _x = x + width/2, _y = y + height/2;
        if(!window.isOldBrowser){
            canvtools.fillText(label, _x, _y);
        }
        if(window.isOldBrowser){
            canvtools.fillText(id, _x, _y);
        }
    };

    return SquareObj;
};

angular.module('app.components.scheme').factory('squareFactory',[
    factory
]);
})();