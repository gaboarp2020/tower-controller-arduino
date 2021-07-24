#define DEBUG

// Elevetion Actions
#define ELEVATION_RELAY_UP   14 // Elevation UP -------- GPIO14
#define ELEVATION_RELAY_DOWN 12 // Elevation DOWN ------ GPIO12

// Inclination Actions
#define INCLINATION_RELAY_UP   13 // Inclination UP ------ GPIO13
#define INCLINATION_RELAY_DOWN 15 // Inclination DOWN ---- GPIO15

#define SCREEN_ORIENTATION 1
#define SCREEN_FORCE_REFRESH_INTERVAL 1000
#define SCREEN_REFRESH_INTERVAL 50 // ~ 1000 / 20
#define SCREEN_INPUT_BLINK_INTERVAL 500
#define SCREEN_INACTIVITY_REDIRECT_INTERVAL 120000

#define ADDRESS 0x3C
#define SDA 16
#define SCL 17

#define PANEL_REBOOT_ATTEMPTS 5

#define SERVER_PORT 80
#define WEB_SOCKET_PATH "/ws"

#define SERIAL_PORT 115200

#define RELAY_NO false