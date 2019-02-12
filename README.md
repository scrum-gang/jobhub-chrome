# Chrome Reason React

## How to setup
1. Clone the repo
    ```bash
    git clone https://github.com/scrum-gang/jobhub-chrome.git
    cd jobhub-chrome

2. Install the dependencies and start the Bucklescript watcher to compile `./src/*.re` into `./lib/js/*.js`
    ```bash
    npm install
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
