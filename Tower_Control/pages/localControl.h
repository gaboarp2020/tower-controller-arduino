#ifndef Local_Control_Page
#define Local_Control_Page

// HTML LOCAL CONTROL PAGE
const char LOCAL_CONTROL_PAGE[] PROGMEM =  R"rawliteral(
<!DOCTYPE html>
<html lang="es">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Local Control</title>
    <style>
      * {
        padding: 0;
        margin: 0;
        box-sizing: border-box;
      }
      body {
        font-family: Arial, "Helvetica Neue", Helvetica, sans-serif;
      }
      .title {
        text-align: center;
      }
      main {
        display: flex;
        min-width: 100vw;
        min-height: 100vh;
        justify-content: center;
        align-items: center;
      }
      .row {
        display: flex;
      }
      .column {
        display: flex;
        flex-direction: column;
      }
      .w-100 {
        width: 100%;
      }
      .jc-center {
        justify-content: center;
      }
      .control-panel {
        max-width: 620px;
        margin: auto;
        padding: 0px 60px;
      }
      .control-buttons {
        padding: 20px 0;
        display: grid;
        grid-gap: 40px;
      }
      .arrow-button {
        display: block;
        font-size: 32px;
      }
      #elevation-up-button .arrow-button {
        transform: rotate(-90deg);
      }
      #elevation-down-button .arrow-button {
        transform: rotate(90deg) translateY(-3px);
      }
      #inclination-up-button .arrow-button {
        transform: rotate(225deg) scaleX(-1);
      }
      #inclination-down-button .arrow-button {
        transform: rotate(45deg);
      }
      #elevation-up-button {
        grid-column-start: 2;
        grid-column-end: 3;
        grid-row-start: 1;
        grid-row-end: 2;
      }
      #elevation-down-button {
        grid-column-start: 2;
        grid-column-end: 3;
        grid-row-start: 3;
        grid-row-end: 4;
      }
      #inclination-up-button {
        grid-column-start: 1;
        grid-column-end: 2;
        grid-row-start: 2;
        grid-row-end: 3;
      }
      #inclination-down-button {
        grid-column-start: 3;
        grid-column-end: 4;
        grid-row-start: 2;
        grid-row-end: 3;
      }
      #stop-button {
        grid-column-start: 2;
        grid-column-end: 3;
        grid-row-start: 2;
        grid-row-end: 3;
      }
      /* Elements styling */
      .box-wrapper {
        padding: 3rem;
        position: relative;
        min-width: 80%;
        max-width: 80%;
        background: #ffffff;
        border: solid 1px;
        border-top: none;
        border-color: #f3f3f3 #cccccc #b6b6b6;
        border-radius: 3px;
        box-shadow: 0 1px 3px rgba(0, 0, 0, 0.12), 0 1px 2px rgba(0, 0, 0, 0.24);
      }
      .control-buttons > button {
        border: none;
        height: 80px;
        width: 80px;
        border-radius: 50%;
        font-size: 1.5rem;
        font-weight: bold;
        border: 3px solid #585a5a;
      }
      .control-buttons > button {
        outline: none;
      }
      button,
      input[type="button"] {
        cursor: pointer;
      }
      button:disabled,
      button[disabled] {
        border-color: #a9a9a9bd !important;
        color: #a9a9a9bd !important;
        cursor: not-allowed;
      }
      .light-gray {
        color: #a9a9a9bd;
      }
      p,
      span,
      .tab-button,
      image {
        color: #585a5a;
      }
      button {
        background-color: #fff;
        color: #585a5a;
      }
      @media screen and (max-width: 992px) {
        main.row {
          flex-direction: column;
        }
        .control-panel {
          padding: 0;
        }
        .box-wrapper {
          min-width: 100%;
          min-height: 100vh;
          border: none;
        }
      }
    </style>
  </head>
  <body>
    <main>
      <div class="box-wrapper">
        <header class="column">
          <!-- <h1 class="title">Tower Control</h1> -->
          <h1 class="title" data-chronometer>00:00:00</h1>
        </header>
        <div class="row">
          <div class="column w-100">
            <div class="control-panel w-100">
              <div class="control-buttons jc-center">
                <button id="elevation-up-button">
                  <span class="arrow-button">&#10141;</span>
                </button>
                <button id="elevation-down-button">
                  <span class="arrow-button">&#10141;</span>
                </button>
                <button id="inclination-up-button">
                  <span class="arrow-button">&#9735;</span>
                </button>
                <button id="inclination-down-button">
                  <span class="arrow-button">&#9735;</span>
                </button>
                <button id="stop-button">⏸</button>
              </div>
            </div>
          </div>
        </div>
        <h1 id="action-message" class="title"></h1>
      </div>
    </main>
    <script>
      document.addEventListener("DOMContentLoaded", function () {
        // GET Method
        const BASE_URL = "/";

        const getHeaders = {
          Accept: "text/plain",
        };

        const isText = (contentType) =>
          contentType.indexOf("text/plain") >= 0 ||
          contentType.indexOf("text/html") >= 0;

        const isJson = (contentType) =>
          contentType.indexOf("application/json") >= 0;

        const formatResponse = (response) => {
          if (!response.ok) {
            throw response;
          }

          const contentType = response.headers.get("content-type");

          if (isText(contentType)) {
            return response.text();
          }

          if (!contentType || isJson(contentType)) {
            return response.json();
          }

          return response;
        };

        const catchError = (response) => {
          if (response && response.status >= 400) {
            throw response;
          }

          const handleError = () => {};

          return handleError();
        };

        const request = async (path, args = {}) => {
          args.headers = args.headers || {};

          return fetch(`${BASE_URL}${path}`, args)
            .then(formatResponse)
            .catch(catchError);
        };

        const queryStringify = (query) => {
          let urlparams = Object.keys(query).reduce(
            (url, key) => url + `${key}=${query[key]}&`,
            "?"
          );

          return urlparams.substring(0, urlparams.length - 1);
        };

        const get = (url, query) => {
          const params = query ? queryStringify(query) : "";

          return request(`${url}${params}`, {
            headers: getHeaders,
          });
        };

        // Constants
        const DIRECTION_UP = 1;
        const DIRECTION_DOWN = 2;

        // State
        const state = {
          activeAction: false,
        };

        // DOM Elements
        const actionButtons = {
          elevationUp: document.querySelector("#elevation-up-button"),
          elevationDown: document.querySelector("#elevation-down-button"),
          inclinationUp: document.querySelector("#inclination-up-button"),
          inclinationDown: document.querySelector("#inclination-down-button"),
        };

        const stopButton = document.querySelector("#stop-button");
        stopButton.disabled = true;

        const actionMessage = document.querySelector('#action-message');

        // Chronometer
        const chronometer = {
          hours: `00`,
          minutes: `00`,
          seconds: `00`,
          display: document.querySelector(`[data-chronometer]`),
          timer: null,
        };

        const chronometerInitWithFormat = () => {
          chronometer.seconds++;
          
          if (chronometer.seconds < 10) chronometer.seconds = `0` + chronometer.seconds;
          
          if (chronometer.seconds > 59) {
            chronometer.seconds = `00`;
            chronometer.minutes++;
            
            if (chronometer.minutes < 10) chronometer.minutes = `0` + chronometer.minutes;
          }
          
          if (chronometer.minutes > 59) {
            chronometer.minutes = `00`;
            chronometer.hours++;
            
            if (chronometer.hours < 10) chronometer.hours = `0` + chronometer.hours;
          }
          
          chronometer.display.textContent = `${chronometer.hours}:${chronometer.minutes}:${chronometer.seconds}`;
        };

        const chronometerClear = () => {
          chronometer.display.textContent = `00:00:00`;

          chronometer.hours = `00`;
          chronometer.minutes = `00`;
          chronometer.seconds = `00`;
        };

        // Helpers
        const addActiveClassTo = (element) => {
          if (element) {
            element.classList.add("active");
          }
        };

        const disableUnactiveActionButtons = () => {
          Object.keys(actionButtons).forEach((button) => {
            if (!actionButtons[button].classList.contains("active")) {
              actionButtons[button].disabled = true;
              actionButtons[button].firstElementChild.classList.add(
                "light-gray"
              );
            }
          });
        };

        const displayActionMessage = (actionButton) => {
          switch (actionButton.id) {
            case 'elevation-up-button': 
              actionMessage.textContent = 'Elevando torre...';
              break;
            case 'elevation-down-button': 
              actionMessage.textContent = 'Bajando torre...';
              break;
            case 'inclination-up-button': 
              actionMessage.textContent = 'Inclinando hacia arriba...';
              break;
            case 'inclination-down-button': 
              actionMessage.textContent = 'Inclinando hacia abajo...';
              break;
          }
        };

        const clearActionMessage = () => {
          actionMessage.textContent = 'Actuador en reposo';
        }

        const stopAction = () => {
          Object.keys(actionButtons).forEach((button) => {
            if (!actionButtons[button].classList.contains("active")) {
              actionButtons[button].disabled = false;
              actionButtons[button].firstElementChild.classList.remove(
                "light-gray"
              );
              stopButton.disabled = true;
            } else {
              actionButtons[button].classList.remove("active");
            }
          });
        };

        const handleActions = async (uri, params = null, actionButton) => {
          if (state.activeAction) {
            return;
          }
          
          try {
            await get(uri, params);
            
            state.activeAction = true;
            displayActionMessage(actionButton)
            addActiveClassTo(actionButton);
            disableUnactiveActionButtons();
            stopButton.disabled = false;
            chronometerClear();
            chronometer.timer = setInterval(chronometerInitWithFormat, 1000);
          } catch (error) {
            console.log(error);
          }
        };

        // HTTP Handlers
        const handleElevationUp = () => {
          handleActions(
            "elevation",
            { direction: DIRECTION_UP },
            actionButtons.elevationUp
          );
        };

        const handleElevationDown = () => {
          handleActions(
            "elevation",
            { direction: DIRECTION_DOWN },
            actionButtons.elevationDown
          );
        };

        const handleInclinationUp = () => {
          handleActions(
            "inclination",
            { direction: DIRECTION_UP },
            actionButtons.inclinationUp
          );
        };

        const handleInclinationDown = () => {
          handleActions(
            "inclination",
            { direction: DIRECTION_DOWN },
            actionButtons.inclinationDown
          );
        };

        const handleStop = async () => {
            try {
                await get("stop");

                state.activeAction = false;
                clearActionMessage();
                stopAction();
                clearInterval(chronometer.timer);
            } catch (error) {
                console.log(error);
            }
        };

        // DOM Events
        actionButtons.elevationUp.addEventListener("click", handleElevationUp);
        actionButtons.elevationDown.addEventListener(
          "click",
          handleElevationDown
        );

        actionButtons.inclinationUp.addEventListener(
          "click",
          handleInclinationUp
        );
        actionButtons.inclinationDown.addEventListener(
          "click",
          handleInclinationDown
        );

        stopButton.addEventListener("click", handleStop);

        clearActionMessage();
      });
    </script>
  </body>
</html>
)rawliteral";

#endif