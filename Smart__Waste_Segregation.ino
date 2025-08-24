#include <Servo.h>

// ------------------ PIN DEFINITIONS ------------------
// Ultrasonic sensors
#define ULTRA_CENTER_TRIG 2
#define ULTRA_CENTER_ECHO 3
#define ULTRA_LEFT_TRIG 4
#define ULTRA_LEFT_ECHO 5
#define ULTRA_RIGHT_TRIG 6
#define ULTRA_RIGHT_ECHO 7

// Motors for buggy (example using L298N)
#define MOTOR_LEFT_FWD 8
#define MOTOR_LEFT_BWD 9
#define MOTOR_RIGHT_FWD 10
#define MOTOR_RIGHT_BWD 11

// Robotic arm servo
#define ARM_SERVO_PIN 12
#define GRIP_SERVO_PIN 13

// Smart bin IR sensors
#define IR_BIN_TOP  A0
#define IR_BIN_BOTTOM A1

// Segregation servo
#define SORT_SERVO_PIN 14

// ------------------ GLOBAL OBJECTS ------------------
Servo armServo;
Servo gripServo;
Servo sortServo;

// ------------------ FUNCTION DECLARATIONS ------------------
long readUltrasonicDistance(int trigPin, int echoPin);
void moveForward();
void stopRobot();
void alignToTrash();
void pickTrash();
void placeTrash();
void sortTrash(String type);

// ------------------ SETUP ------------------
void setup() {
  Serial.begin(115200);

  // Setup ultrasonic pins
  pinMode(ULTRA_CENTER_TRIG, OUTPUT);
  pinMode(ULTRA_CENTER_ECHO, INPUT);
  pinMode(ULTRA_LEFT_TRIG, OUTPUT);
  pinMode(ULTRA_LEFT_ECHO, INPUT);
  pinMode(ULTRA_RIGHT_TRIG, OUTPUT);
  pinMode(ULTRA_RIGHT_ECHO, INPUT);

  // Setup motors
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_BWD, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_BWD, OUTPUT);

  // Setup IR sensors
  pinMode(IR_BIN_TOP, INPUT);
  pinMode(IR_BIN_BOTTOM, INPUT);

  // Setup servos
  armServo.attach(ARM_SERVO_PIN);
  gripServo.attach(GRIP_SERVO_PIN);
  sortServo.attach(SORT_SERVO_PIN);

  // Initialize arm
  armServo.write(90);   // Arm upright
  gripServo.write(0);   // Grip open
  sortServo.write(90);  // Center
}

// ------------------ MAIN LOOP ------------------
void loop() {
  // Read distances
  long distanceCenter = readUltrasonicDistance(ULTRA_CENTER_TRIG, ULTRA_CENTER_ECHO);
  long distanceLeft = readUltrasonicDistance(ULTRA_LEFT_TRIG, ULTRA_LEFT_ECHO);
  long distanceRight = readUltrasonicDistance(ULTRA_RIGHT_TRIG, ULTRA_RIGHT_ECHO);

  // Check if trash detected within 20cm
  if(distanceCenter < 20 || distanceLeft < 20 || distanceRight < 20) {
    stopRobot();
    alignToTrash();
    pickTrash();
    placeTrash();

    // Wait for AI classification signal from ESP32
    if(Serial.available()) {
      String trashType = Serial.readStringUntil('\n');
      sortTrash(trashType);
    }
  } else {
    moveForward();
  }
}

// ------------------ FUNCTIONS ------------------

// Read distance from ultrasonic sensor
long readUltrasonicDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Distance in cm
}

// Move buggy forward
void moveForward() {
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

// Stop buggy
void stopRobot() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

// Align robot to face trash using side ultrasonics
void alignToTrash() {
  long left = readUltrasonicDistance(ULTRA_LEFT_TRIG, ULTRA_LEFT_ECHO);
  long right = readUltrasonicDistance(ULTRA_RIGHT_TRIG, ULTRA_RIGHT_ECHO);

  while(abs(left - right) > 5) {
    if(left < right) { // rotate right
      digitalWrite(MOTOR_LEFT_FWD, HIGH);
      digitalWrite(MOTOR_RIGHT_FWD, LOW);
    } else { // rotate left
      digitalWrite(MOTOR_LEFT_FWD, LOW);
      digitalWrite(MOTOR_RIGHT_FWD, HIGH);
    }
    delay(100);
    stopRobot();
    left = readUltrasonicDistance(ULTRA_LEFT_TRIG, ULTRA_LEFT_ECHO);
    right = readUltrasonicDistance(ULTRA_RIGHT_TRIG, ULTRA_RIGHT_ECHO);
  }
}

// Pick trash using robotic arm
void pickTrash() {
  armServo.write(0);    // Lower arm
  delay(500);
  gripServo.write(90);  // Close grip
  delay(500);
  armServo.write(90);   // Lift arm
}

// Place trash in smart bin
void placeTrash() {
  armServo.write(0);    // Lower arm
  delay(500);

  // Wait for bin detection
  while(digitalRead(IR_BIN_TOP) == LOW || digitalRead(IR_BIN_BOTTOM) == LOW) {
    delay(100);
  }

  gripServo.write(0);   // Release trash
  delay(500);
  armServo.write(90);   // Lift arm
}

// Sort trash based on type from ESP32 AI
void sortTrash(String type) {
  if(type == "plastic") {
    sortServo.write(0);   // Right
  } else if(type == "metal") {
    sortServo.write(180); // Left
  } else {
    sortServo.write(90);  // Center/others
  }
  delay(500);
  sortServo.write(90); // Return to center
}
