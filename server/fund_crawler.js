var cheerio = require('cheerio');
var http = require('http');
var mqtt = require('mqtt');

var timer;
var data = '';
var oldData = '';
var host = 'http://fund.eastmoney.com/'
// var number = '519772'
// var device_id  = 'D446'

var opt = {
  port: 1883,
  clientId: 'server/fund_crawler'
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

function subFundData(device_id, number, interval)
{
  var url = host + number + '.html';

  timer = setInterval(function() {
    http.get(url, function (response) {
      var body = [];

      response.on('data', function (chunk) {
          body.push(chunk);
      });

    response.on('end', function () {
      var titles = [];
      body = Buffer.concat(body);
      var $ = cheerio.load(body, {decodeEntities: false});

      var num = $('.fundDetail-tit .ui-num').text();
      var nowWorth = $('.dataItem02 .dataNums .ui-font-large').text();
      var nowLimit = $('.dataItem02 .dataNums .ui-font-middle').text();
      var nowDatetime = $('.dataItem02 > dt > p').text().replace(/[^0-9\-]+/g,"");
      var forecastWorth = $('.dataItem01 .dataNums .ui-font-large').text();
      var forecastLimit = $('#gz_gszzl').text();
      var forecastDatetime = $('#gz_gztime').text().replace(/[()]+/g,"");;

      try { 
        data = {
          'device_id' : device_id,
          'data_type' : 3,
          'data_content' : {
            'num' : num,
            'info': [
              {'worth': nowWorth, 'limit': nowLimit, 'datetime': nowDatetime}, 
              {'worth': forecastWorth, 'limit': forecastLimit, 'datetime':forecastDatetime}
            ]
          }
        };
      } catch (e) {
        console.error('Fund data Error :', e);
      }

      if (JSON.stringify(data) != JSON.stringify(oldData)) {
        console.log(JSON.stringify(data));
        client.publish('data', JSON.stringify(data), {qos:2});
      }
      oldData = data;
      console.log('[' + new Date().toLocaleTimeString() + ']' + 'check fund data...');
      // console.log(JSON.stringify(data));
      });
    });

  }, interval*1000);
  
}

function stopFundData() {
  clearInterval(timer);
  client.end();
}

exports.subFundData = subFundData;
exports.stopFundData = stopFundData;
