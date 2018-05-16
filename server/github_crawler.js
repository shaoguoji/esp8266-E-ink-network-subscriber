var https = require('https');
var cheerio = require('cheerio');
var mqtt = require('mqtt');

var timer;
var data = '';
var oldData = '';

var opt = {
  port: 1883,
  clientId: 'server/github_crawler'
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

function subGithubData(device_id, repo, interval)
{
  timer = setInterval(function() {
    https.get('https://github.com/'+repo, function (response) {
      var body = [];
      var content;
      response.on('data', function (chunk) {
          body.push(chunk);
      });

      response.on('end', function () {
        var dataCount = [];
        body = Buffer.concat(body);
        var $ = cheerio.load(body, {decodeEntities: false});
        $('.pagehead-actions > li').each(function(index, elem){
             dataCount[index] = $(elem).find($('.social-count')).text().replace(/[\r\n| ]/g,"");
        });
      
        try {
          data = {
            'device_id' : device_id,
            'data_type' : 2,
            'data_content' : {
              'repo_name' : repo,
              'watch' : dataCount[0].replace(/[,]/g,""),
              'star' : dataCount[1].replace(/[,]/g,""),
              'fork' : dataCount[2].replace(/[,]/g,""),
            }
          };
        } catch (e) {
          console.error('Github data Error :', e);
        }

        if (JSON.stringify(data) != JSON.stringify(oldData)) {
          console.log(JSON.stringify(data));
          client.publish('data', JSON.stringify(data), {qos:2});
        }
        oldData = data;
        console.log('[' + new Date().toLocaleTimeString() + ']' + 'check github data...');
      });
  });}, interval*1000);
}

function stopGithubData() {
  clearInterval(timer);
  client.end();
}

exports.subGithubData = subGithubData;
exports.stopGithubData = stopGithubData;
