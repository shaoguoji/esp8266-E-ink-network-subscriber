var https = require('https')
var pinyin = require("node-pinyin")
var mqtt = require('mqtt')

var timer;
var data = '';
var oldData = '';
var host = 'https://www.sojson.com/open/api/weather/json.shtml?city='
// var city = '广州'
// var device_id  = 'D446'

var opt = {
  port: 1883,
  clientId: 'server/weather_crawler'
};

var client = mqtt.connect('mqtt://btdoor.cn', opt);

client.on('connect', function () {
  client.subscribe('data');
});

client.on('message', function (topic, message) {
  if (topic == 'data') {
    var msg = JSON.parse(message.toString());

    if ((msg.data_type == 2) && (msg.data_content.star == '---')) {
      client.publish('data', JSON.stringify(data), {qos:2});
    }
  }
  // console.log(message.toString());
});

function subWeatherData(device_id, city, interval)
{
  var url = host + city

  timer = setInterval(function() {
    https.get(encodeURI(url), function (response) {
          var body = [];
          var content;
          response.on('data', function (chunk) {
              body.push(chunk);
          });

        response.on('end', function () {
        body = Buffer.concat(body);
        content = JSON.parse(body.toString());

        try {
          var cityPinyin = pinyin(city, {style: 'normal'}).toString().replace(/[^a-zA-Z]+/g,"").replace(/( |^)[a-z]/g, (L) => L.toUpperCase());     
          var todayTemp =  content.data.forecast[0].low.replace(/[^0-9.]+/g,"") + '-' 
                        + content.data.forecast[0].high.replace(/[^0-9.]+/g,"") + 'C';
          var todayPm25 = 'PM2.5: ' + content.data.pm25.toString();
          var tomorrowTemp = content.data.forecast[1].low.replace(/[^0-9.]+/g,"") + '-' 
                        + content.data.forecast[1].high.replace(/[^0-9.]+/g,"") + 'C';
          var tomorrowAqi = 'aqi: ' + content.data.forecast[1].aqi.toString();

          data = {
            'device_id' : device_id,
            'data_type' : 1,
            'data_content' : {
              'city' : cityPinyin,
              'weather': [
                {'date': content.date.toString(), 'temp': todayTemp, 'air': todayPm25}, 
                {'date': 'tomorrow', 'temp': tomorrowTemp, 'air': tomorrowAqi}
              ]
            }
          };
        } catch (e) {
          console.error('Weather data Error :', e);
        }
        
        if (JSON.stringify(data) != JSON.stringify(oldData)) {
          console.log(JSON.stringify(data));
          client.publish('data', JSON.stringify(data), {qos:2});
        }
        oldData = data;
        console.log('[' + new Date().toLocaleTimeString() + ']' + 'check weather data...');
        // console.log(JSON.stringify(data));
      });
      });

  }, interval*1000);

}

function stopWeatherData() {
  clearInterval(timer);
  client.end();
}

exports.subWeatherData = subWeatherData;
exports.stopWeatherData = stopWeatherData;
