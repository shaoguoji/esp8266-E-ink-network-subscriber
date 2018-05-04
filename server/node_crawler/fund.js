var cheerio = require('cheerio');
var http = require('http');
var iconv = require('iconv-lite');

var host = 'http://fund.eastmoney.com/'
var number = '519772'
var device_id  = 'D446'

var url = host + number + '.html';

http.get(url, function (response) {
    var body = [];

    response.on('data', function (chunk) {
        body.push(chunk);
    });

response.on('end', function () {
  var titles = [];
  body = Buffer.concat(body);
  //var html = iconv.decode(body, 'gb2312');
  var $ = cheerio.load(body, {decodeEntities: false});

  // var name = $('.fundDetail-tit').text();
  var num = $('.fundDetail-tit .ui-num').text();
  var nowWorth = $('.dataItem02 .dataNums .ui-font-large').text();
  var nowLimit = $('.dataItem02 .dataNums .ui-font-middle').text();
  var nowDatetime = $('.dataItem02 > dt > p').text().replace(/[^0-9\-]+/g,"");
  var forecastWorth = $('.dataItem01 .dataNums .ui-font-large').text();
  var forecastLimit = $('#gz_gszzl').text();
  var forecastDatetime = $('#gz_gztime').text().replace(/[()]+/g,"");;

  var data = {
    'device_id' : device_id,
    'data_type' : 3,
    'data_content' : {
      'num' : num,
      'info': [
        {'worth': nowWorth, 'limit': nowLimit, 'datetime': nowDatetime}, 
        {'worth': forecastWorth, 'limit': forecastLimit, 'datetime':forecastDatetime}
      ]
    }
  }

console.log(JSON.stringify(data).replace(/[\"]+/g, "\\\""));
});
});