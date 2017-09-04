'use strict';
(function() {

    var factory = function($q){
        var ImageObj = this;

        ImageObj["draw_substrate"] = function(canvtools, x, y, width, height, icon){
            var image = new Image(),
                delay = $q.defer();
            image.onload = function () {
                canvtools.drawImage(image, x, y, width, height);
                delay.resolve();
            };
            image.src = icon;
            return delay.promise;
        };

        ImageObj["draw_image"] = function(canvtools, x, y, width, height, icon){
            var image = new Image();
            image.onload = function () {
                canvtools.drawImage(image, x, y, width, height);
            };
            image.src = icon;
        };

        return ImageObj;
    };

    angular.module('app.components.scheme').factory('imageFactory',[
        '$q',
        factory
    ]);
})();