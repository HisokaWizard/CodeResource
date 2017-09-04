'use strict';
(function() {

    var service = function(){
        var _public = this;

        // На выходе индекс 1-го символа подстроки.
        // Если искомой подстроки нет в строке, то -1.
        // Например,
        // fullTextSearch('ips', 'Lorem ipsum') = 6,
        // fullTextSearch('dolor', 'Lorem ipsum') = "-1".
        _public.fullTextSearch = function(substr, fullstr){
            var sub_length = substr.length;
            var delta = fullstr.length - sub_length + 1;
            for(var i=0; i<delta; i++){
                var j = 0;
                while((j < sub_length) && (substr.charAt(j) === fullstr.charAt(i+j))) {
                    j++;
                }
                if(j === sub_length){
                    return i;
                }
            }
            return -1;
        }
    };

    angular.module('app.components.fulltextSearch').service('fulltextSearchService', [
        service]);
})();
