#ifndef INCLUDE_DEFANDONMIN_H_
#define INCLUDE_DEFANDONMIN_H_

#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

#define GPIO_LED_GREEN      21
#define GPIO_LED_YELLOW     23
#define GPIO_LED_RED        22
#define GPIO_BUZZER         19

//#define WIFI_SSID           "USGRENT71"
//#define WIFI_PASSWD         "1234567890"
#define WIFI_SSID           "USG"
#define WIFI_PASSWD         "pPx1xN!NH1Sq$68"
#define WIFI_TIMEOUT_MS     20000

#define LED_GREEN_MODE      20
#define LED_YELLOW_MODE     21
#define LED_RED_MODE        22

#define API_URL             "http://10.61.0.21:7993/getSummaryCounterLine?id_dev=C3QC1001"
#define API_USER            "ACTIFY"
#define API_PASSWD          "12345678"

class Connection {
    public:
        void connect();
};
Connection connection;

class Task {
    public:
        String payload;
        int httpCode;
        int defect;
        int pending;
        int repaired;
        void getRestAPI();
        void readData(void *params);
        void setAPIStat(int8_t Status) {
            status = Status;
        }
        int8_t restAPIStat() const {
		    return status;
        }
        int8_t status;  
    private:
      	

};
Task task;

class Buzzer {
    public:
        void configure();
};
Buzzer buzzer;

class LEDIndicator {
    public:
        void configure();
        void light(int8_t mode);
        void setLightMode(int8_t Mode) {
            mode = Mode;
        }
        int8_t getLEDMode() const {
		    return mode;
        }

    private:
      	int8_t mode;
};

LEDIndicator led;

#endif /*INCLUDE_DEFANDONMIN_H_*/