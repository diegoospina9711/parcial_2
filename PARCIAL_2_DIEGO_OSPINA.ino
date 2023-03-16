#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <RTClib.h>
RTC_DS3231 rtc;	

#define echoPin 5 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 18 //attach pin D3 Arduino to pin Trig of HC-SR04

const char* ssid = "Ospina65"; //ssid de la red inalambrica
const char* password = "princesa9711"; //password para conectarse a la red

long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int vel_x;

void setup(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
Serial.begin(115200); //Serial connection
if (! rtc.begin()) {				// si falla la inicializacion del modulo
 Serial.println("Modulo RTC no encontrado !");	// muestra mensaje de error
 while (1);					// bucle infinito que detiene ejecucion del programa
 }  

 rtc.adjust(DateTime(__DATE__, __TIME__));

setup_wifi(); //WiFi connection
delay(1500);

}
void setup_wifi(){
delay(10);
// We start by connecting to a WiFi network
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
  }
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
}

void loop(){

digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor

 

DateTime fecha = rtc.now();
int vel = analogRead(34); 
String variable;
//DynamicJsonDocument doc(1024); //creacion del json
StaticJsonDocument<200> doc;
vel_x=map(vel,0,1024,0,400);
doc["Velocidad"] = vel_x;
doc["distancia"] = distance;
  doc["ID"] = 1234;
Serial.print(fecha.year()," ");
Serial.print(".");
Serial.print(fecha.month());
Serial.print(".");
Serial.print(fecha.day());
Serial.print("t/");
Serial.print(fecha.hour());
Serial.print(":");
Serial.print(fecha.minute());
Serial.print(":");
Serial.println(fecha.second());
//doc["Anno"] = fecha.year();
//doc["Mes"] = fecha.month();
//doc["date"] =fecha.day();
//doc["hora"] = fecha.hour();
//doc["min"] = fecha.minute();
//doc["sec"] = fecha.second();
serializeJson(doc, variable);
Serial.println("dato a enviar: "+ variable);
HTTPClient http; //Declare object of class HTTPClient
WiFiClient client;
//Specify request destination
http.begin(client,"http://things.ubidots.com/api/v1.6/devices/parcial2/?token=BBFF-9j1uyN2IIq4mReCVBmbu04O6R0xUgn");
http.addHeader("Content-Type", "application/json"); //Specify contenttype header
int httpCode = http.POST(variable); //Send the request
String payload = http.getString(); //Get the response payload
Serial.println(httpCode); //Print HTTP return code
Serial.println(payload); //Print request response payload
http.end(); //Close connection
delay(5000); //Send a request every 5 seconds
}
