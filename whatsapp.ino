

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = D6, TXPin = D5;
static const uint32_t GPSBaud = 9600;
int button = 5; // push button is connected
// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
double longitude,latitude;

// Define your WIFI settings.
const char *ssid = "Shahzaib";
const char *password = "shah12345";

// Create data holders to send data packets.

String fingerprint, WebHook, HEDRON, id, token, message, from, to, trespass;


void setup() {

// Wait until connected.
  delay(1000);
  pinMode(button, INPUT); // declare push button as input
  pinMode(2,OUTPUT);
   pinMode(16,OUTPUT);
  digitalWrite(16,HIGH);
  digitalWrite(2,HIGH);
  Serial.begin(9600);
  ss.begin(GPSBaud);
  // It is just for assuring connection is alive.
  WiFi.mode(WIFI_OFF);
  delay(1000);
  // This mode allows NodeMCU to connect any WiFi directly.
  WiFi.mode(WIFI_STA);        
  // Connect NodeMCU to your WiFi.
  WiFi.begin(ssid, password);
  
  Serial.print("\n\n");
  Serial.print("Try to connect to WiFi. Please wait! ");
  Serial.print("\n\n");
  // Halt the code until connected to WiFi.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }
 
  // If connection is successful, turn control led(if used) on and write WiFi SSID to serial monitor along with assigned IPAddress.
  Serial.print("\n\n");
  Serial.print("-------------------------------------");
  Serial.print("\n\n");
  Serial.print("Connection is successful!");
  Serial.print("\n\n");
  Serial.print("Connected WiFi SSID : ");
  Serial.print(ssid);
  Serial.print("\n\n");
  Serial.println("Connected IPAddress : ");
  Serial.println(WiFi.localIP());
  Serial.print("\n\n"); 
   digitalWrite(2,LOW);
  delay(2000);
}

void loop() {



    while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      latitude=(gps.location.lat());
      longitude=(gps.location.lng());
//       Serial.print("Latitude= "); 
//      Serial.print(latitude, 6);
//      Serial.print("Longitude= "); 
//      Serial.println(longitude, 6);
    
    }
  }
  // Enter the fingerprint.
  fingerprint = "d4 07 ff 2f ba 5a 90 f2 81 b4 89 2f a0 ac d1 13 87 07 6f c2";

  // Enter your hedron provided by TheAmplituhedron, Twilio Account Information, and phone numbers.
  HEDRON = "ed779d864db6337cb7079f857457914ae27f7b72";
  id= "AC6c2735643e426991db1a1ae6ef334dde";
  token = "2577f2e7beb0bad5bda1b1af70a9cf2c";
  from = "14155238886"; // Enter numbers.
  to = "923363613894";

  // Create message,use %0A to start a new line.
  message = "The+Car+is+in+trouble.%0A+The+Current+location+is+https://www.google.com/maps?q="+String(latitude,6)+","+String(longitude,6);

  // Create the webhook path.
  WebHook = "https://www.theamplituhedron.com/dashboard/WhatsApp-Web-Hook/" + HEDRON + ".php?id=" + id + "&token=" + token + "&body=" + message + "&from=" + from + "&to=" + to;

  int  temp = digitalRead(button);
     if (temp == LOW) {
        
        digitalWrite(16,HIGH);
        
       }
     else {
      digitalWrite(16,LOW);
        // Create HTTP object to make a request.
         HTTPClient http;
           // Send a request. 
     http.begin(WebHook, fingerprint);

       int httpCode = http.GET();           
 String payload = http.getString();    
 
 if(httpCode == 200)
  { 
   Serial.println("\n Message Sent Successfully..."); 
    Serial.println(message);
  }
  
  else
  { 
    Serial.println("\n Not Working! Error Code: " + httpCode); 
    }
 
  // End HTTP Get Request.
  http.end(); 

  // Send data packets every quarter minute to the webhook.
        delay(10000);
       }



  

 
}
