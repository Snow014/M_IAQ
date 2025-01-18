#include <WiFi.h>
#include <MQTT.h>

WiFiClient net;
MQTTClient client;

const String WIFI_SSID = "Network name";
const String WIFI_PASS = "Network password";

const char IPBroker[] = "MQTT broker IP";
int Port = 1883;

const char mqttClientId[] = "MQTT client ID";
const char mqttUsername[] = "MQTT broker username";
const char mqttPassword[] = "MQTT broker password";

#include <SensirionI2CSen5x.h>
#include <Wire.h>

SensirionI2CSen5x sen5x;

#include <MHZ19.h>

MHZ19 mhz(&Serial1);

unsigned long lastMeasurement = 0;
unsigned long measureInterval = 30000;  //  30 seconds

void setup(){
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, D2, D1);  //  MHZ19C port

  Wire.begin();
  sen5x.begin(Wire);
  if(sen5x.deviceReset() != 0){
    Serial.println("Failed to start SEN55!");
  }
  sen5x.startMeasurement();
}

void loop(){

  connect();
  
  if(millis() - lastMeasurement > measureInterval){
    lastMeasurement = millis();

    //  publish Co2 reading
    MHZ19_RESULT response = mhz.retrieveData();
    if (response == MHZ19_RESULT_OK){
      String co2String = String(mhz.getCO2());

      connect();
      client.publish("CO2", co2String, false, 1);
    }
    else{
      Serial.print(F("Error, code: "));
      Serial.println(response);
    }

    float pm1p0;
    float pm2p5;
    float pm4p0;
    float pm10p0;
    float humidity;
    float temperature;
    float vocIndex;
    float noxIndex;

    bool datardy = false;
    
    // sen5x.startMeasurement();

    // while(!datardy){
    //   sen5x.readDataReady(datardy);
    //   Serial.println("waiting for SEN55..");
    //   delay(100);
    // }

    // delay(2500);

    Serial.println("sending SEN55 data..");

    uint16_t error = sen5x.readMeasuredValues(pm1p0, pm2p5, pm4p0, pm10p0, humidity, temperature, vocIndex, noxIndex);
    Serial.println("VoC: " + String(vocIndex));

    if (error) {
        Serial.print("Error trying to execute readMeasuredValues()");
    } else {
      connect();
      client.publish("PM1P0", String(pm1p0), false, 1);
      delay(50);
      client.publish("PM2P5", String(pm2p5), false, 1);
      delay(50);
      client.publish("PM4P0", String(pm4p0), false, 1);
      delay(50);
      client.publish("PM10P0", String(pm10p0), false, 1);
      delay(50);

      client.publish("VOC", String(vocIndex), false, 1);
      delay(50);
      client.publish("NOX", String(noxIndex), false, 1);
      delay(50);

      client.publish("TEMP_I", String(humidity), false, 1);
      delay(50);
      client.publish("HUM_I", String(temperature), false, 1);
      delay(50);

      //  sen5x.stopMeasurement();
    }
  }
}

void connect(){
  
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting to " + WIFI_SSID + "..");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    int tries = 0;

    while (WiFi.status() != WL_CONNECTED){
      //  hopefully fix the wifi connection loop?
      if(tries > 200){
        Serial.println("Couldn't connect to wifi, restarting ESP..");
        ESP.restart();
      }
      Serial.println(String(tries) + "/200..");
      tries++;
      delay(200); 
    }
    Serial.println("Wifi connected!");
  }
  
  if(!client.connected()){
    Serial.println("Connecting to " + String(IPBroker) + "..");

    client.begin(IPBroker, Port, net);
    while (!client.connect(mqttClientId, mqttUsername, mqttPassword)) {
      Serial.print(".");
      delay(200);
    }

    client.onMessage(messageReceived);
    Serial.println("Connected to" + String(IPBroker) + "!");
  }

  WiFi.setSleep(false);
}

void messageReceived(String &topic, String &payload){
  Serial.print(topic);
  Serial.println(payload);
}