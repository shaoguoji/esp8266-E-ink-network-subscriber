var github = require("./github_crawler");
var fund = require("./fund_crawler");
var weather = require("./weather_crawler");

github.subGithubData('D446', 'torvalds/linux', 5);
fund.subFundData('D446', '519772', 5);
weather.subWeatherData('D446', '广州', 5);

// setTimeout(github.stopGithubData, 60000);
