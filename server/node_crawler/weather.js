var https = require('https')
var pinyin = require("node-pinyin")

var host = 'https://www.sojson.com/open/api/weather/json.shtml?city='
var city = '广州'
var device_id  = 'D446'

var url = host + city

https.get(encodeURI(url), function (response) {
    var body = [];
    var content;
    response.on('data', function (chunk) {
        body.push(chunk);
    });

response.on('end', function () {
  body = Buffer.concat(body);
  content = JSON.parse(body.toString());

  var cityPinyin = pinyin(city, {style: 'normal'}).toString().replace(/[^a-zA-Z]+/g,"").replace(/( |^)[a-z]/g, (L) => L.toUpperCase());     
  var todayTemp =  content.data.forecast[0].high.replace(/[^0-9.]+/g,"") + '-' 
                + content.data.forecast[0].low.replace(/[^0-9.]+/g,"") + 'C';
  var todayPm25 = 'PM2.5: ' + content.data.pm25.toString();
  var tomorrowTemp = content.data.forecast[1].high.replace(/[^0-9.]+/g,"") + '-' 
                + content.data.forecast[1].low.replace(/[^0-9.]+/g,"") + 'C';
  var tomorrowAqi = 'aqi: ' + content.data.forecast[1].aqi.toString();

  var data = {
    'device_id' : device_id,
    'data_type' : 1,
    'data_content' : {
      'city' : cityPinyin,
      'weather': [
        {'date': content.date.toString(), 'temp': todayTemp, 'air': todayPm25}, 
        {'date': 'tomorrow', 'temp': tomorrowTemp, 'air': tomorrowAqi}
      ]
    }
  }

  console.log(JSON.stringify(data).replace(/[\"]+/g, "\\\""));
});
});
