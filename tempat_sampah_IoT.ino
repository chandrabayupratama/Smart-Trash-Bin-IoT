/*
This example shows how to connect to Cayenne using an ESP32 and send/receive sample data.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. If you have not already installed the ESP32 Board Package install it using the instructions here: https://github.com/espressif/arduino-esp32/blob/master/README.md#installation-instructions.
2. Select your ESP32 board from the Tools menu.
3. Set the Cayenne authentication info to match the authentication info from the Dashboard.
4. Set the network name and password.
5. Compile and upload the sketch.
6. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#define CAYENNE_DEBUG
#include <CayenneMQTTESP32.h>
#include <ESP32Servo.h>
const int triggerPin_1 = 27;
const int echoPin_1 = 26;
unsigned int duration_1;
unsigned int jarak1;
const int triggerPin_2 = 25;
const int echoPin_2 = 33;
const int ledMerah = 23;
unsigned int duration_2;
unsigned int jarak2;

// WiFi network info.
char ssid[] = "";
char wifiPassword[] = "";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char* username = "";
char* password = "";
char* clientID = "";

float x0, x1;
int volPersen;
Servo myservo1;

void setup() {
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  pinMode(triggerPin_1, OUTPUT);
  pinMode(echoPin_1, INPUT);
  pinMode(triggerPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  pinMode (ledMerah, OUTPUT);
  digitalWrite(ledMerah, LOW);
  myservo1.attach(4);
  myservo1.write(0);
    
}
void readSensor_1() {
  digitalWrite(triggerPin_1, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin_1, LOW);
  duration_1 = pulseIn(echoPin_1, HIGH);
  jarak1 = (duration_1 / 2) / 28.5;
  Serial.print("Sensor 1 = ");
  Serial.print(jarak1);
  Serial.println(" cm");
}
void readSensor_2() {
  digitalWrite(triggerPin_2, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin_2, LOW);
  duration_2 = pulseIn(echoPin_2, HIGH);
  jarak2 = (duration_2 / 2) / 28.5;
  Serial.print("Sensor 2 = ");
  Serial.print(jarak2);
  Serial.println(" cm");
  if (jarak2 > 25 ){
    jarak2 = 25;
  }
  else if (jarak2 <= 2 ) {
    jarak2 = 0;
  }
  else {
    jarak2 = jarak2;
  }
  
  
  Serial.println("VolPersen = ");
  Serial.println(volPersen);
}

void bukaTutup(){
 if(jarak1 <= 10 && jarak1 > 1){
    myservo1.write(180);
    delay(2000);
  }
  else{
    myservo1.write(0);
    x0 = 25 - jarak2;
    volPersen = x0 * 100/25;
    Cayenne.virtualWrite(1, volPersen);
  }
}


void loop() {
  Cayenne.loop();
  readSensor_1();
  readSensor_2();
  bukaTutup();
  if (volPersen > 90 ) {
    digitalWrite(ledMerah, HIGH);
  }
  else{
    digitalWrite(ledMerah, LOW);
  }
}
