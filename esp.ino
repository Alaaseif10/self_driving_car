#include <ESP8266WiFi.h>  
#include <string.h>               
#include <FirebaseArduino.h>      
#define FIREBASE_HOST "cartask-2ee8b-default-rtdb.firebaseio.com"      
#define FIREBASE_AUTH "e8PcgQxuXxO99KEQp1u32chsusbHqClbO5kdviyO"  


         
#define WIFI_SSID "DESKTOP-N6LQPOV 6238"                                  
#define WIFI_PASSWORD "d39M=084"
         
//define sound velocity in cm/uS
#define SOUND_VELOCITY 
#define CM_TO_INCH 
const int trigPin = 12;
const int echoPin = 14;
int n;  
long duration;
float distanceCm;
void setup() 
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(115200);           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println();
  Serial.print("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());                               //prints local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                 // connect to the firebase
  pinMode(D1,OUTPUT);  
  pinMode(D2,OUTPUT); 
 
}
 
void loop() 
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
//  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance
//  distanceCm = duration * SOUND_VELOCITY/2;
  // Convert to inches
  // Prints the distance on the Serial Monitor
//  Serial.print("Distance (cm): ");
//  Serial.println(distanceCm); 
//  if (distanceCm <=20 ){
//    digitalWrite(D1,0); 
//    digitalWrite(D2,1);
//    digitalWrite(D4,1);
//    Serial.print("stop");
//    delay(2000);
//      
//  }
  delay(1000);
  n=Firebase.getInt("status/status");
  if (Firebase.failed()) 
    {
 
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error()); 
  }
  Serial.print(n); 
  if (n==1) {  
      digitalWrite(D1,1); 
      digitalWrite(D2,1); 
      Serial.println("F"); 
  }  
   else if (n==2){  
   digitalWrite(D1,0); 
   digitalWrite(D2,0); 
   Serial.println("L");
 }
    else if ((n==3 ) || (n == 0)){  
   digitalWrite(D1,0); 
   digitalWrite(D2,0);
   Serial.println("S"); 
 } 
 else if (n==4){  
   digitalWrite(D1,1); 
   digitalWrite(D2,0);
   Serial.println("R"); 
 } 
  delay(100);
}
