'use strict';
(function() {

    var service = function(
        $rootScope,
        userInfo,
        resourceService)
    {
        var _public = this;

        _public.translate = function(str){
            for(var item in userInfo.strings){
                if (item == str)
                    return userInfo.strings[item];
            }
            return resourceService.translate(str);
        };
    };

    angular.module('app.user').service('userService', [
        '$rootScope',
        'userInfo',
        'resourceService',
        service
    ]);
})();