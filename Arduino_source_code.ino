#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>
Servo myservo;

const char* ssid = "andrey";
const char* password = "0526928109";
WebServer server(80);  // Create server at port 80

int speed = 100;
int servo = 90;// Speed variable
int DistanceConst = 20;

// These constants are for the ledc functions
const int freq = 1000;
const int resolution = 8;
const int channel[4] = {4, 5, 2, 3};
const int pin[4] = {18, 19, 15, 2}; // GPIO4 is now used for channel[1] and GPIO15 for channel[3]


const int trigPin = 23; // HC-SR04 sensor
const int echoPin = 22; // HC-SR04 sensor
const int servoPin = 12; //servo pwm
long duration;
int distance;


const int buzzerPin = 5; // Buzzer connected to pin 5
const int buzzerFreq = 1000;





unsigned long lastDistanceMeasureTime = 0;  // Last time distance was measured
unsigned long lastPrintTime = 0;  // Last time distance was printed

bool isMovingBackward = false;  // Flag to track if the robot is moving backward
int PreviousDistance = 0;
void setupMotorControl() {
  
  for (int i = 0; i < 4; i++) {
    ledcSetup(channel[i], freq, resolution);
    ledcAttachPin(pin[i], channel[i]);
  }
}










void handleForward() {
  if(distance > DistanceConst) {
  int pwm_speed = map(speed, 0, 100, 0, 255); // Map speed from 0-100 to 0-255
  ledcWrite(channel[0], 0);
  ledcWrite(channel[1], pwm_speed);
  ledcWrite(channel[2], 0);
  ledcWrite(channel[3], pwm_speed);
  myservo.write(servo);
  }
  Serial.println("forward");
  server.send(200, "text/plain", "forward");
}

void handleBackward() {
  int pwm_speed = map(speed, 0, 100, 0, 255);
  ledcWrite(channel[0], pwm_speed);
  ledcWrite(channel[1], 0);
  ledcWrite(channel[2], pwm_speed);
  ledcWrite(channel[3], 0);
  myservo.write(servo);
  Serial.println("backward");
  server.send(200, "text/plain", "backward");
  isMovingBackward = true;
}

void handleLeft() {
   if(distance > DistanceConst) {
  int pwm_speed = map(speed, 0, 100, 0, 255);
  ledcWrite(channel[0], pwm_speed);
  ledcWrite(channel[1], 0);
  ledcWrite(channel[2], 0);
  ledcWrite(channel[3], pwm_speed);
  myservo.write(servo);
   }
  Serial.println("left");
  server.send(200, "text/plain", "left");
}

void handleRight() {
   if(distance > DistanceConst) {
  int pwm_speed = map(speed, 0, 100, 0, 255);
  ledcWrite(channel[0], 0);
  ledcWrite(channel[1], pwm_speed);
  ledcWrite(channel[2], pwm_speed);
  ledcWrite(channel[3], 0);
  myservo.write(servo);
   }
  Serial.println("right");
  server.send(200, "text/plain", "right");
}

void handleStop() {
  ledcWrite(channel[0], 0);
  ledcWrite(channel[1], 0);
  ledcWrite(channel[2], 0);
  ledcWrite(channel[3], 0);
  Serial.println("stop");
  server.send(200, "text/plain", "stop");
  isMovingBackward = false;
   myservo.write(servo);
}









void handleCameraLeft() {
  
    if(servo >= 0) {
    servo = servo-20;
      myservo.write(servo);
  }
    Serial.println("servo left = ");
    Serial.println(servo);
    server.send(200, "text/plain", "servo left=" + String(servo)); // convert speed to a String
  
}


void handleCameraRight() {
  
    if(servo <= 180) {
    servo = servo+20;
      myservo.write(servo);
  }
    Serial.println("servo Right = ");
    Serial.println(servo);
    server.send(200, "text/plain", "servo left=" + String(servo)); // convert speed to a String
  
}



















void slower() {
  if(speed > 0) {
    speed = speed-10;
  }
   
  server.send(200, "text/plain", "speed=" + String(speed)); // convert speed to a String
  Serial.print("slower speed= ");
  Serial.println(speed);
}



void faster() {
  if(speed < 100) {
    speed = speed + 10;
    server.send(200, "text/plain", "speed=" + String(speed)); // convert speed to a String
  }
  server.send(200, "text/plain", "speed=" + String(speed)); // convert speed to a String
  Serial.print("Faster speed=");
  Serial.println(speed);
}








void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(18,OUTPUT);
  pinMode(19,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(2,OUTPUT);
  digitalWrite(18,LOW);
  digitalWrite(19,LOW);
  digitalWrite(15,LOW);
  digitalWrite(2,LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  setupMotorControl();

  pinMode(trigPin, OUTPUT); // HC-SR04 sensor
  pinMode(echoPin, INPUT);  // HC-SR04 sensor
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT); // HC-SR04 sensor
  pinMode(echoPin, INPUT);  // HC-SR04 sensor
  pinMode(buzzerPin, OUTPUT);
  myservo.attach(26);
  handleStop();



  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/stop", handleStop);
  server.on("/faster", faster);
  server.on("/slower", slower);
 // server.on("/BuzzerOn", handleBuzzerOn);
  server.on("/CameraLeft", handleCameraLeft);
  server.on("/CameraRight", handleCameraRight);
  server.begin();
  
  
  
  
}

void loop() {
  server.handleClient();
 
  
  // Distance measurement
  if (millis() - lastDistanceMeasureTime >= 100) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    if (distance < DistanceConst && !isMovingBackward) {
      if (PreviousDistance >20){
      handleStop();
      }
      
      
      
    }
    PreviousDistance = distance;
    
    

    lastDistanceMeasureTime = millis();
  }

  // Print distance every second
  if (millis() - lastPrintTime >= 1000) {
    Serial.print("Distance: ");
    Serial.println(distance);
    server.send(200, "text/plain", "distance=" + String(distance)); 
    lastPrintTime = millis();
  }
}
