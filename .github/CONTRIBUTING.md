# How to contribute to the Jobhub Chrome extension

Thanks for your interest on contributing to Jobhub!  
Here are a few general guidelines on contributing and reporting bugs that we ask you to take a look first.  
Notice that all of your interactions in the project are expected to follow our [Code of Conduct](CODE_OF_CONDUCT.md).

## Reporting Issues

If your issue wasn't already reported or fixed in our [issue board](https://github.com/scrum-gang/jobhub-chrome/issues), you can submit it using [this template](ISSUE_TEMPLATE.md).


**If you find a security bug, do not report it through GitHub.  
Please send an
e-mail to [alexander.bratyshkin@mail.mcgill.ca](mailto:alexander.bratyshkin@mail.mcgill.ca) or [camilo.e.garcia@mail.mcgill.ca](mailto:camilo.e.garcia@mail.mcgill.ca)
instead.**

## Sending Pull Requests

If your enhancement or bug fix wasn't already proposed or fixed in our [issue board](https://github.com/scrum-gang/jobhub-chrome/issues), you can submit it using [this template](PULL_REQUEST_TEMPLATE.md).

The basic things every PR must have are the following:
- an issue atttached to it so that the team can discuss before a solution is implemented
- a set of tests that validate the bug is fixed or the enhancement is functional

## How to setup
1. Clone the repo and install the dependencies
    ```bash
    git clone https://github.com/scrum-gang/jobhub-chrome.git
    cd jobhub-chrome
    npm install
    ```

2. Start the Bucklescript watcher to compile ReasonML `./src/*.re` into Javascript `./lib/js/*.js`
    ```bash
    npm start
    ```

3. Start the Webpack watcher to bundle `./lib/js/*.js` into `./build/index.js`
    ```bash
    npm run webpack-dev
    ```

4. Start the Jest watcher to continously test your changes
    ```bash
    npm test
    ```
