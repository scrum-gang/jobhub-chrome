# JobHub Google Chrome Extension
:pushpin: Keep track of job applications ad-hoc

[![Build Status](https://travis-ci.org/scrum-gang/jobhub-chrome.svg?branch=master)](https://travis-ci.org/scrum-gang/jobhub-chrome) 
<!--- [![Coverage Status](https://coveralls.io/repos/github/scrum-gang/jobhub-chrome/badge.svg)](https://coveralls.io/github/scrum-gang/jobhub-chrome)--->

| Login Popup  | Job Posting Form |
| ------------- | ------------- |
| ![Login Popup](https://i.imgur.com/GXQQewr.png)  | ![Job Posting Form](https://i.imgur.com/rQyb5oD.png)  |


## How to setup the project
Please check our [contributors guide](.github/CONTRIBUTING.md).

## Why use the JobHub Extension
The Chrome extension is meant to be an ad-hoc access to the JobHub services. When the users are on a job application website, they don't need to stop browsing the post in order to open the JobHub website on a separate tab. Instead, they can start tracking the given application via the popup submit directly on the job posting website.

## Folder Structure
```bash
.
├── .github # contains Code of Conduct & templates for PR/Issues
├── build   # generated by Webpack. Contains the final js/html/css/resources
├── data    # list of companies to scrape
├── lib     # generated by Bucklescript. ReasonML -> Javascript
├── src     # ReasonReact components & global stylesheet
├── .editorconfig       # linter
├── .travis.yml         # CI
├── bsconfig.json       # Bucklescript config
├── jsconfig.json       # VSCode intellisense for Chrome API
├── manifest.json       # Chrome Extension config
├── package.json        # Node config
├── popup.html          # html of the extension
└── webpack.config.js   # Webpack config
```

## Architecture
```bash
           popup.html
               |
             App.re
             {token}
               |
   +-----------+-----------+
   |                       |
Login.re               JobApp.re
                           |
                   ScrapingInputs.re
```

`token` is the state variable which is initially obtained by `Login` during the authentication process and in subsequent  execustions of the extension its validity is confirmed by `App` directly. The variable is passed onto the `JobApp` component to be able to submit job applications.

The system leverages the following modules
- `ScrapingFunctions.re`: functions related to extracting/processing HTML elements
- `Services.re`: functions related to asynchronous actions external to the extension (load files/API calls)
- `SyncStorage.re`: functions related to the Chrome storage management
- `Uilities.re`: general purpose helper functions


## Limitations
* Currently, the only supported `posted date` scraper pattern is `"<num> days ago"`
* Currently, the extension does not have a `deadline date` scraper
* Because of the [bucklescript-chrome](https://github.com/jchavarri/bucklescript-chrome.git#start-extensions) bindings used to build the extension,  
we are locked with outdated versions of core libraries such as `React` and `Bucklescript`
* For the same reason as above, we can't use Chrome's [Content Scripts](https://developer.chrome.com/extensions/content_scripts).  
    Instead we inject a script into the active website to extract DOM information.
