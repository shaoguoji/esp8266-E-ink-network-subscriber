var https = require('https')

var host = 'api.github.com'
var repo = 'alibaba/AliOS-Things'
var device_id  = 'D446'

const options = {
  hostname: 'api.github.com',
  path: '/repos/'+ repo,
  headers: {
    'User-Agent': 'shaoguoji'
  }
};

https.get(options, function (response) {
    var body = [];
    var content;
    response.on('data', function (chunk) {
        body.push(chunk);
    });

    response.on('end', function () {
      body = Buffer.concat(body);
      content = JSON.parse(body.toString());
    
      var data = {
        'device_id' : device_id,
        'data_type' : 2,
        'data_content' : {
          'repo_name' : content.full_name.toString(),
          'watch' : content.subscribers_count.toString(),
          'star' : content.stargazers_count.toString(),
          'fork' : content.forks.toString(),
        }
      }

     console.log(JSON.stringify(data).replace(/[\"]+/g, "\\\""));
    });
});
