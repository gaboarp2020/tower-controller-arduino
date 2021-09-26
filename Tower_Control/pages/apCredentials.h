#ifndef AP_Crendentials_Page
#define AP_Crendentials_Page

// HTML ACCESS POINT CREDENTIALS PAGE
const char AP_CRENDENTIALS_PAGE[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <title>CONFIG PAGE</title>
    <meta charset="UTF-8" />
    <style>
      * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
      }
      html,
      body {
        height: 100%;
      }
      body {
        font-size: 24px;
        background: #ededed;
        font-family: Arial, "Helvetica Neue", Helvetica, sans-serif;
      }
      input,
      select,
      textarea {
        resize: vertical;
      }
      main {
        display: flex;
        width: 100vw;
        height: 100vh;
        justify-content: center;
        align-items: center;
      }
      .form-wrapper {
        position: relative;
        min-width: 80%;
        max-width: 80%;
        background: #ffffff;
        border: solid 1px;
        border-color: #f3f3f3 #cccccc #b6b6b6;
        border-radius: 3px;
        box-shadow: 0 1px 3px rgba(0, 0, 0, 0.12), 0 1px 2px rgba(0, 0, 0, 0.24);
      }
      .form-wrapper h1 {
        display: block;
        position: relative;
        left: -1px;
        margin: 40px 0;
        padding: 15px 57px;
        color: #1b6a9e;
        border: solid #1b6a9e;
        border-width: 0 0 0 12px;
        text-transform: uppercase;
      }
      .form-wrapper .form-body {
        padding: 0 60px 40px 60px;
        font-size: 150%;
      }
      .form-wrapper .form-body .fieldset {
        position: relative;
        width: 100%;
        margin-top: 50px;
        font-weight: 400;
      }
      .form-wrapper .form-body .fieldset input {
        width: 100%;
        margin: 15px 0;
        padding: 15px 0;
        border: solid #e8e8e8;
        border-width: 0 0 1px 0;
        font-size: 110%;
        color: #33414e;
        outline: none;
      }
      .form-wrapper .form-body .fieldset input:focus ~ label,
      .form-wrapper .form-body .fieldset input:valid ~ label {
        top: -35px;
        color: #b8cfe4;
        font-size: 120%;
      }
      .form-wrapper .form-body .fieldset label {
        position: absolute;
        top: calc(50% - 30px);
        left: 0;
        cursor: text;
        color: #33414e;
        font-size: 110%;
        font-weight: 300;
        line-height: 60px;
        transition: 0.2s ease;
      }
      .form-wrapper .form-body .fieldset:first-child {
        margin-top: 0px;
      }
      .form-wrapper .form-body input[type="submit"] {
        width: 100%;
        padding: 20px 0;
        background: #fff;
        border: 2px solid #1b6a9e;
        color: #1b6a9e;
        font-size: 120%;
        font-weight: 600;
        line-height: 1;
        text-transform: uppercase;
        cursor: pointer;
      }
      .form-wrapper .form-body input[type="submit"]:active {
        background: #1b6a9e;
        border-color: #1b6a9e;
        color: #fff;
      }
      .form-wrapper .form-body input[type="submit"]:disabled {
        background: #fff;
        border-color: #ccc;
        color: #ccc;
      }
      .message {
        font-size: 70%;
      }
      .success {
        color: #28a745;
      }
      .error {
        color: #dc3545;
      }
    </style>
  </head>
  <body>
    <main>
      <div class="form-wrapper">
        <h1>wifi</h1>
        <div class="form-body">
          <form id="credentials-form" name="credentials-form">
            <div class="fieldset">
              <input id="ssid" name="ssid" type="text" required />
              <label for="ssid">ssid</label>
              <p id="error-ssid" class="message error"></p>
            </div>
            <div class="fieldset">
              <input id="password" name="password" type="password" required />
              <label for="password">password</label>
              <p id="error-password" class="message error"></p>
            </div>
            <div class="fieldset">
              <p id="message"></p>
            </div>
            <div class="fieldset button-set">
              <input id="submit-btn" type="submit" value="save" disabled />
            </div>
          </form>
        </div>
      </div>
    </main>
    <script>
      (() => {
        const init = () => {
          const getQueryParam = (location, name) => {
            name = name.replace(/[\[]/, "\\[").replace(/[\]]/, "\\]");
            const regex = new RegExp("[\\?&]" + name + "=([^&#]*)");
            const results = regex.exec(location.search);

            return results === null
              ? ""
              : decodeURIComponent(results[1].replace(/\+/g, " "));
          };

          const setStateMessage = () => {
            const state = Number(getQueryParam(window.location, "state"));
            const messageHTML = document.querySelector("#message");

            if (state !== 0) {
              // Success
              if (state === 1) {
                messageHTML.classList = "message success";
                messageHTML.innerHTML = "WiFi credentials successfully updated";
              }
              // Error
              else if (state === 2) {
                messageHTML.classList = "message error";
                messageHTML.innerHTML = "Error updating WiFi credentials";
              }
            } else {
              messageHTML.classList = "message";
              messageHTML.innerHTML = "";
            }
          };

          const validation = (validateFn) => {
            // Validation
            const INITIAL_STATE = {
              password: false,
              ssid: false,
            };

            let state = { ...INITIAL_STATE };

            return {
              getState: () => state,
              setState: (name, value) => {
                state = { ...state, [name]: value };

                const isValid = Object.keys(state)
                  .map((key) => state[key])
                  .every((value) => !!value);

                if (isValid) {
                  validateFn(true);
                } else {
                  validateFn(false);
                }
              },
            };
          };

          const setFormValidations = () => {
            // DOM
            const ssid = document.querySelector("#ssid");
            const ssidMessageHTML = document.querySelector("#error-ssid");
            const password = document.querySelector("#password");
            const passwordMessageHTML = document.querySelector("#error-password");
            const submitBtn = document.querySelector("#submit-btn");

            const validator = validation((isValid) => {
              if (isValid) {
                submitBtn.disabled = false;
              } else {
                submitBtn.disabled = true;
              }
            });

            ssid.addEventListener("keyup", (event) => {
              const value = event.target.value;

              if (value.length > 0 && value.length < 31) {
                validator.setState("ssid", true);
                ssidMessageHTML.innerHTML = "";
              } else {
                validator.setState("ssid", false);
                ssidMessageHTML.innerHTML = "Invalid SSID, must have a length shorter than 31 characters";
              }
            });

            password.addEventListener("keyup", (event) => {
              const value = event.target.value;

              if (value.length > 7 && value.length < 64) {
                validator.setState("password", true);
                passwordMessageHTML.innerHTML = "";
              } else {
                validator.setState("password", false);
                passwordMessageHTML.innerHTML = "Invalid Password, must have a length longer than 7 and shorter than 64 characters";
              }
            });
          };

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

          // On Submit Credentials Form
          // DOM
          const credentialsForm = document.querySelector('#credentials-form');

          const submitCredentials = async (event) => {
            event.preventDefault();
            event.stopPropagation();

            await get('save_ap', { ssid: ssid.value, password: password.value })
                .then(response => {
                    //Reseting Inputs Values
                    ssid.value = "";
                    password.value = "";
                })
                .catch(err => {
                    alert(err)
                });
          }

          setStateMessage();
          setFormValidations();

          credentialsForm.addEventListener("submit", submitCredentials, true);
        };

        window.addEventListener("DOMContentLoaded", init);
      })();
    </script>
  </body>
</html>
)=====";

#endif
