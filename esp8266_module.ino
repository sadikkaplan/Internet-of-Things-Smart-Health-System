#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//Def
#define myPeriodic 15 //in secondi | Thingspeak pubblica in 15sec
#define ONE_WIRE_BUS 2 // DS18B20 SONDA N° 1 su arduino pin2 corrisponde a D4 su nodeMCU
#define ONE_WIRE_BUS2 14 // DS18B20 SONDA N° 2 su arduino pin2 corrisponde a D5 su nodeMCU

OneWire oneWire(ONE_WIRE_BUS);
OneWire oneWire2(ONE_WIRE_BUS2);
DallasTemperature DS18B20(&oneWire);

float prevTemp = 0;
float prevTemp2 = 0;
const char* server = "api.thingspeak.com";
String apiKey ="xxxxxxxxxxxxxxxx"; // apikey thingspeak.com
const char* MY_SSID = "xxxxxxxxxxxx"; // Nome SSID Wifi
const char* MY_PWD = "xxxxxxxxxxxxx"; // Password Wifi
int sent = 0;
void setup() {
Serial.begin(115200);
connectWifi();
}

void loop() {
float temp;
//char buffer[10];
DS18B20.requestTemperatures();
temp = DS18B20.getTempCByIndex(0);
//String tempC = dtostrf(temp, 4, 1, buffer);//gestito in sendTemp()
Serial.print(String(sent)+" Temperatura Sonda Alta Puffer: ");
Serial.println(temp);

float temp2;
//char buffer[10];
DS18B20.requestTemperatures();
temp2= DS18B20.getTempCByIndex(1);
//String tempC = dtostrf(temp, 4, 1, buffer);//gestito in sendTemp()
Serial.print(String(sent)+" Temperatura Sonda Bassa Puffer: ");
Serial.println(temp2);

//if (temp != prevTemp)
//{
//sendTeperatureTS(temp);
//prevTemp = temp;
//}

sendTeperatureTS(temp);
sendTeperatureTS(temp2);
int count = myPeriodic;
while(count--)
delay(1000);

}

void connectWifi()
{
Serial.print("Connecting to "+*MY_SSID);
WiFi.begin(MY_SSID, MY_PWD);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}

Serial.println("");
Serial.println("Connesso");
Serial.println("");
}//fine connessione

void sendTeperatureTS(float temp)
void sendTeperatureTS(float temp2)
{
WiFiClient client;

if (client.connect(server, 80)) { // usa ip 184.106.153.149 oppure api.thingspeak.com
Serial.println("WiFi Client connected ");

String postStr = apiKey;
postStr += "&field1=";
postStr += String(temp);
postStr += "

";

String postStr = apiKey;
postStr += "&field2=";
postStr += String(temp2);
postStr += "

";

client.print("POST /update HTTP/1.1
");
client.print("Host: api.thingspeak.com
");
client.print("Connection: close
");
client.print("X-THINGSPEAKAPIKEY: " + apiKey + "
");
client.print("Content-Type: application/x-www-form-urlencoded
");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("

");
client.print(postStr);
delay(1000);

}//fine if
sent++;
client.stop();
}//fine invio
