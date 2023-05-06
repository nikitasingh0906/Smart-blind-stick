#include <IRremote.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#define echopin 2
#define trigpin 3
#define buzzer 8
const int RECV_PIN = 7;
static const int gpsRXPin = 9, gpsTXPin = 10;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(9,10);
SoftwareSerial gsmSerial(4, 5);
IRrecv irrecv(RECV_PIN);
decode_results results;
const int buzzPin = 8;
int recieve=0;
long duration;
int distance;
unsigned long key_value = 0;
void setup() {
// put your setup code here, to run once:
Serial.begin(9600);
pinMode(3,OUTPUT);
pinMode(2,INPUT);
pinMode(8,OUTPUT);
irrecv.enableIRIn();
//irrecv.blink13(true); 
enableLEDFeedback();
gsmSerial.begin(9600);
gpsSerial.begin(9600);
Serial.println("Initializing...");
delay(1000);
}
void loop() {
// put your main code here, to run repeatedly:
digitalWrite(3,LOW);
delay(2);
digitalWrite(3,HIGH);
delay(10);
digitalWrite(3,LOW);
duration=pulseIn(2,HIGH);
distance=(duration*0.034/2);
if (distance<=100){
digitalWrite(8, HIGH);
delay(1000);
digitalWrite(8, LOW);
digitalWrite(8, HIGH);
delay(1000);
digitalWrite(8, LOW);
digitalWrite(8, HIGH);
delay(1000);
digitalWrite(8, LOW);
}
else if (distance<=50){
digitalWrite(8, HIGH);
delay(500);
digitalWrite(8, LOW);
digitalWrite(8, HIGH);
delay(500);
digitalWrite(8, LOW);
}
else
digitalWrite(8, LOW);
//if (irrecv.decode(&results)){
if (IrReceiver.decode()){
if (results.value == 0XFFFFFFFF)
results.value = key_value;
switch(results.value){
case 0X1FEE01F:
digitalWrite(8,1);
break;
case 0X1FED827:
digitalWrite(8,0);
break;
case 0X1FE00FF:
SendMessage();
break;
}
key_value = results.value;
irrecv.resume();
}
while(gsmSerial.available())
{
Serial.write(gsmSerial.read());//Forward what Software Serial received to Serial Port
}
while (gpsSerial.available() > 0){
gps.encode(gpsSerial.read());
if (gps.location.isUpdated()){
Serial.print("Latitude= ");
Serial.print(gps.location.lat(), 6);
Serial.print(" Longitude= ");
Serial.println(gps.location.lng(), 6);
}
}
}
void SendMessage(){
gsmSerial.println("AT+CMGF=1");
delay(500);
gsmSerial.println("AT+CMGS=\"+918959989599\"\r");
delay(500);
gsmSerial.println("Hi, Kripi NEEDS HELP");
gsmSerial.println();
gsmSerial.print("Latitude= ");
gsmSerial.print(gps.location.lat(), 6);
gsmSerial.print(" Longitude= ");
gsmSerial.println(gps.location.lng(), 6);
delay(500);
gsmSerial.println((char)26);
delay(500);
}
