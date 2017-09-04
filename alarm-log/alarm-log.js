'use strict';
(function() {
  angular.module('app.alarmLog',[
    'ngRoute',
    'app.components.mvvm',
    'app.components.filter-calendar'
  ]).
  constant(
    'alarmLogInfo', {
      id : 'alarmLog',
      breadcrumb : {
        uri : "#/alarm-log",
        label : "Аварии"
      },
      uri : {
        model : 'alarms',
        html  : 'app/alarm-log/alarm-log.html'
      },
      format : 'json'
    }
  ).
  config(function(){
  }).
  run(function(navigationService, alarmLogInfo){
    navigationService.page('app.alarm.log').
      parameters({
        path : '#/alarm-log',
        label : 'Аварии',
        onUpdateState : function(alarmLogService){
          this.state.notificationCount = alarmLogService.getUnacknowledgedCount;
          if (0 < this.state.notificationCount)
            this.state.isFlashing = true;
          if (0 < alarmLogService.getCount())
            this.state.isAlerted = true;
        }
      }).
      route('/alarm-log', { templateUrl: alarmLogInfo.uri.html });
  });
})();
