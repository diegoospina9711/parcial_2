#include <ArduinoJson.h>
//#include <HTTPClient.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <RTClib.h>
RTC_DS3231 rtc;	
#define echoPin 5 // attach pin Echo of HC-SR04
#define trigPin 18 //attach pin  Trig of HC-SR04
#define Led 2
#define mqttUser ""
#define mqttPass ""
#define mqttPort 1883

long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int vel_x;

const char* ssid ="Ospina65"; //ssid de la red inalambrica
const char* password ="princesa9711"; //password para conectarse a la red

char mqttBroker[] = "broker.mqtt-dashboard.com"; //ip del servidor
char mqttClientId[] = "device1"; //cualquier nombre
char inTopic[] = "topico21";

void callback(char* topic, byte* payload, unsigned int length) {
Serial.print("Message arrived [");
Serial.print(topic);
Serial.print("] ");
for (int i=0;i<length;i++) {
Serial.print((char)payload[i]);
}
String json=String((char *)payload);
Serial.println();
StaticJsonDocument<300> doc;
DeserializationError error = deserializeJson(doc, json);
if (error) { return; }
int estado = doc["estado"];

Serial.println(estado);
if (estado == 1) {
digitalWrite(Led, HIGH);
}
else {
digitalWrite(Led, LOW);
}

}
WiFiClient BClient;
PubSubClient client(BClient);

void reconnect() {
// Loop until we're reconnected
while (!client.connected()) {
Serial.print("Attempting MQTT connection...");
// Attempt to connect
if (client.connect(mqttClientId, mqttUser, mqttPass)) {
Serial.println("connected");
client.subscribe("parcial_3");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");
// Wait 5 seconds before retrying
delay(5000);
    }
  }
}

void setup(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(Led, OUTPUT);
  Serial.begin(115200); //Serial connection
  setup_wifi(); //WiFi connection
  client.setServer( mqttBroker, mqttPort );
  client.setCallback( callback );
  Serial.println("Setup done");

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

  WiFi.begin(ssid,password);
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

  

if (!client.connected()) {
reconnect();
}
client.loop();

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
  vel_x=map(vel,0,4096,0,2000);
  String variable;

StaticJsonDocument<200> doc;
  doc["id_nodo"] = "ESP32";
  doc["Velocidad"] = vel_x; 
  doc["distancia"] = distance;
  

  Serial.print(fecha.year());
  Serial.print(".");
  Serial.print(fecha.month());
  Serial.print(".");
  Serial.print(fecha.day());
  Serial.println("/t");
  Serial.print(fecha.hour());
  Serial.print(":");
  Serial.print(fecha.minute());
  Serial.print(":");
  Serial.println(fecha.second());

serializeJson(doc, variable);
int lon = variable.length()+1;
Serial.println(variable);
char datojson[lon];
variable.toCharArray(datojson, lon);
client.publish(inTopic,datojson);
delay (5000);
}
