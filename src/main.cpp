#include <Arduino.h>
#include "DevAndonMin.h"

void setup() {
  Serial.begin(9600);
  delay(2000);
  pinMode(LED_BUILTIN, OUTPUT);
  buzzer.configure();
  led.configure();
  connection.connect();

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    task.readData(&task);
    int mode = led.getLEDMode();
    led.light(mode);
  } else {
    digitalWrite(BUILTIN_LED, LOW);
  }
}

void Connection::connect() {
  delay(500);
  Serial.print("Connecting to Network... ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  unsigned long startAttempTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttempTime < WIFI_TIMEOUT_MS) {
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status() != WL_CONNECTED) { 
    Serial.println();
    Serial.println("Failed");
  }
  else { 
    Serial.println();
    Serial.print("Connected With IP :  ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address       :  ");
    Serial.println(WiFi.macAddress());
  }
}

void Task::getRestAPI() {
  delay(500);
  Serial.print("Getting Data From API... ");
  WiFiClient client;
  HTTPClient http;
  http.begin(client, API_URL); 
  http.setAuthorization(API_USER,API_PASSWD);
  httpCode = http.GET();
  payload = http.getString();
  Serial.print("Response Code :");
  Serial.println(httpCode);
  if (httpCode == 200 ) {
    setAPIStat(true);
    Serial.print("API OKE \n\r");
  } else {
    setAPIStat(false);
    Serial.print("API FAIL \n\r");
  }
}

void Task::readData(void *params) { 
  Task *config_obj = (Task*) params;
  task.getRestAPI();
  config_obj->restAPIStat();
  if (config_obj->restAPIStat()) {
    digitalWrite(BUILTIN_LED, HIGH);
    DynamicJsonDocument doc (1024);
    deserializeJson(doc, payload);
    JsonObject summary = doc["summary"];
    defect = summary["defect_val"];
    repaired = summary["repaired_val"];
    pending = defect-repaired;
    Serial.print("Defect : ");
    Serial.println(defect);
    Serial.print("Repaired : ");
    Serial.println(repaired);
    Serial.print("Pending : ");
    Serial.println(pending);
    if (pending<=2) {
      led.setLightMode(20);
    } else if (pending>2 && pending<=4) {
      led.setLightMode(21);
    } else {
      led.setLightMode(22);
    }
    digitalWrite(BUILTIN_LED, LOW);
  } else {
    Serial.println("Rest API Error");
  }
}

void Buzzer::configure() {
  Serial.print("Configuring Buzzer.... ");
  pinMode(GPIO_BUZZER,OUTPUT);
  Serial.print("Done\n\r");
}

void LEDIndicator::configure() {
  Serial.print("Configuring LED Indicator.... ");
  pinMode(GPIO_LED_GREEN,OUTPUT);
  pinMode(GPIO_LED_YELLOW,OUTPUT);
  pinMode(GPIO_LED_RED,OUTPUT);
  Serial.print("Done\n\r");
}

void LEDIndicator::light(int8_t mode) {
  switch (mode)
  {
  case LED_GREEN_MODE :
    digitalWrite(GPIO_LED_GREEN,LOW);
    digitalWrite(GPIO_LED_YELLOW,HIGH);
    digitalWrite(GPIO_LED_RED,HIGH);
    digitalWrite(GPIO_BUZZER,HIGH);
    Serial.println("GREEN");
    break;

  case LED_YELLOW_MODE : 
    digitalWrite(GPIO_LED_GREEN,HIGH);
    digitalWrite(GPIO_LED_YELLOW,LOW);
    digitalWrite(GPIO_LED_RED,HIGH);
    digitalWrite(GPIO_BUZZER,HIGH);
    break;

  case LED_RED_MODE : 
    digitalWrite(GPIO_LED_GREEN,HIGH);
    digitalWrite(GPIO_LED_YELLOW,HIGH);
    digitalWrite(GPIO_LED_RED,LOW);
    Serial.println("RED");
    digitalWrite(GPIO_BUZZER,LOW);
    break;
  
  default :
      LED_GREEN_MODE;
    break;
  }
}