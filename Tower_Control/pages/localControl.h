#ifndef Local_Control_Page
#define Local_Control_Page

// HTML LOCAL CONTROL PAGE
const char LOCAL_CONTROL_PAGE[] PROGMEM =  R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  </style>
</head>
<body>
  <h1>Tower Control</h1>
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

      // Variables
      const DIRECTION_UP = 1;
      const DIRECTION_DOWN = 2;

      const handleElevationUp = get(`/elevation?direction=${DIRECTION_UP}`);
      const handleElevationDown = get(`/elevation?direction=${DIRECTION_DOWN}`);
      const handleInclinationUp = get(`/elevation?direction=${DIRECTION_UP}`);
      const handleInclinationDown = get(`/elevation?direction=${DIRECTION_DOWN}`);
    });
  </script>
</body>
</html>
)rawliteral";

#endif