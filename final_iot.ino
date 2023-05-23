
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <RTClib.h>
RTC_DS3231 rtc;	
#define Led 2
#define echoPin 5 // attach pin Echo of HC-SR04
#define trigPin 18 //attach pin  Trig of HC-SR04
double vel_x;
long duration; // variable for the duration of sound wave travel
double distance; // variable for the distance measurement
const char* ssid = "Ospina65";
const char* password = "princesa9711";
StaticJsonDocument<300> doc;
int nodo =1;



void setup() {

pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
Serial.begin(115200); //Serial connection
setup_wifi(); //WiFi connection
delay(1500);

if (! rtc.begin()) {				// si falla la inicializacion del modulo
  Serial.println("Modulo RTC no encontrado !");	// muestra mensaje de error
  while (1);					// bucle infinito que detiene ejecucion del programa
  }  

  rtc.adjust(DateTime(__DATE__, __TIME__));
    delay(1500);
}
void setup_wifi() {
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
void loop() {
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

int vel = analogRead(34); 
  vel_x=map(vel,0,4096,0,2000);
  DateTime fecha = rtc.now();

String variable;

 //StaticJsonDocument<200> doc;
DynamicJsonDocument doc(1024);
  
  doc["idnodo"] = nodo;
  doc["distancia"] = distance;
  doc["velocidad"] = vel_x; 
  doc["time"] = 22052023;

Serial.print(fecha.hour());
  Serial.print(":");
  Serial.print(fecha.minute());
  Serial.print(":");
  Serial.println(fecha.second());

serializeJson(doc, variable);
//Serial.println("dato a enviar: "+ variable);
int lon = variable.length()+1;
Serial.println(variable);
char datojson[lon];
HTTPClient http;
WiFiClient client;
http.begin(client, "http://192.168.1.60:3000/final_diego");
http.addHeader("Content-Type", "application/json");
int httpCode = http.POST(variable); //Send the request
String payload = http.getString(); //Get the response
Serial.println(httpCode); //Print HTTP return code

Serial.println(payload); //Print request response payload
http.end(); //Close connection
delay(5000); //Send a request every 30 seconds
}
