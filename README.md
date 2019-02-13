# JobHub Google Chrome Extension
Keep track of job applications ad-hoc

[![Build Status](https://travis-ci.org/scrum-gang/jobhub-chrome.svg?branch=master)](https://travis-ci.org/scrum-gang/jobhub-chrome) 
[![Coverage Status](https://coveralls.io/repos/github/scrum-gang/jobhub-chrome/badge.svg)](https://coveralls.io/github/scrum-gang/jobhub-chrome)

## How to setup the project & contribute
Please check our [contributors guide](.github/CONTRIBUTING.md).

## Why use the JobHub Extension
TODO

## Architecture
TODO

## Limitations
* Currently, the only supported `posted date` scraper pattern is `"<num> days ago"`
* Currently, the extension does not have a `deadline date` scraper
* Because of the [bucklescript-chrome](https://github.com/jchavarri/bucklescript-chrome.git#start-extensions) bindings used to build the extension,  
we are locked with outdated versions of core libraries such as `React` and `Bucklescript`
* For the same reason as above, we can't use Chrome's [Content Scripts](https://developer.chrome.com/extensions/content_scripts).  
    Instead we inject a script into the active website to extract DOM information.
