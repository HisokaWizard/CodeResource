'use strict';
(function() {

    var service = function(

    ) {

        var _public = this;

        _public.result = {
            x: 0,
            y: 0
        };

        _public.MouseEventValue = {
            up : "mouse up called",
            down : "mouse down called",
            leave: "mouse leave called",
            over : "mouse over called"
        };

        _public.onMouseMove = function (event) {
            if (!event) {
                event = window.event;
                return _public.result;
            }
            if (event.pageX || event.pageY) {
                _public.result.x = event.pageX;
                _public.result.y = event.pageY;
            }
            else if (event.clientX || event.clientY) {
                _public.result.x = event.clientX + document.body.scrollLeft +
                    document.documentElement.scrollLeft;
                _public.result.y = event.clientY + document.body.scrollTop +
                    document.documentElement.scrollTop;
            }
            if (event.target)
            {
                var offEl = event.target;
                var offX = 0;
                var offY = 0;
                if (typeof(offEl.offsetParent) != "undefined") {
                    while (offEl){
                        offX += offEl.offsetLeft;
                        offY += offEl.offsetTop;

                        offEl = offEl.offsetParent;
                    }
                }
                else{
                    offX = offEl.x;
                    offY = offEl.y;
                }
                _public.result.x -= offX;
                _public.result.y -= offY;
            }
            return _public.result;
        };

        _public.MouseDown = function(event){
            if(event){
                return _public.MouseEventValue.down;
            }
        };

        _public.MouseUp = function(event){
            if(event){
                return _public.MouseEventValue.up;
            }
        };

        _public.MouseLeave = function(event){
            if(event){
                return _public.MouseEventValue.leave;
            }
        };

        _public.MouseOver = function(event){
            if(event){
                return _public.MouseEventValue.over;
            }
        };

    };

    angular.module('app.components.scheme').service('mouseService', [
        service
    ]);
})();