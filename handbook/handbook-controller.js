'use strict';

(function(){

  var moduleName = 'app.handbook',
    controllerName =  'handBookCtrl',
    dependencies = [
      'handBookInfo',
      'cacheService',
      'handBookService',
      'fulltextSearchService'
    ],
    controller = function(
    handBookInfo,
    cacheService,
    handBookService,
    fulltextSearchService
  ){
      var _public = this;
      _public.filter = {
        ranks: {}
      };

      _public.breadcrumb = handBookInfo.breadcrumb;
      _public.cache = cacheService.cache;
      _public.users = _public.cache.users;
      _public.WinHeight = document.documentElement.clientHeight;

      _public.getFilter = function(label, filter){
        return function(user){
          var match = handBookService.getFilter(label, filter, user, fulltextSearchService);
          return match;
        };
      };

      _public.userRanks = (function(){
        return handBookService.getUserRanks();
      })();

  };

  dependencies.push(controller);
  angular.module(moduleName).controller(controllerName, dependencies);

})();