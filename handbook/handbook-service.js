'use strict';

(function(){

  var moduleName = 'app.handbook',
    serviceName =  'handBookService',
    dependencies = [
    ],
    service = function(
    ){
      var _public = this;

      _public.getFilter = function(label, filter, user, fulltextSearchService){
        if(undefined !== user){
          var match = true,
              ranks = filter.ranks;
          if((undefined !== user.user_name)
            && (undefined !== label)
            && ("" !== user.user_name)
            && ("" !== label)){
            if(-1 !== fulltextSearchService.fullTextSearch(label.toLowerCase(), user.user_name.toLowerCase())){
              match = true;
            }
            else{
              match = false;
            }
          }
          var filterHasRankCriteria = false;
          for (var rank in ranks){
            if (false !== ranks[rank]) {
              filterHasRankCriteria = true;
              break;
            }
          }
          if (match && filterHasRankCriteria && undefined !== ranks) {
            match = false;
            for (var rank2 in ranks) {
              if (ranks[rank2] && user.rank.toLowerCase() === rank2.toLowerCase()) {
                match = true;
                break;
              }
            }
          }
          return match;
        }
      };

      _public.getUserRanks = function(){
        var ranks = [];
        ranks[0] = "Контр-адмирал";
        ranks[1] = "Капитан 1-го ранга";
        ranks[2] = "Капитан 2-го ранга";
        ranks[3] = "Капитан 3-го ранга";
        ranks[4] = "Мичман";
        return ranks;
      };

    };

  dependencies.push(service);
  angular.module(moduleName).service(serviceName, dependencies);

})();
