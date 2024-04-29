// Group Watts Of IQ
// Written by Vicente (Software Lead)
// Members: Nathan, Mariam, Manuel, Yaimel

// Define pin for distance sensor and laser sensor
//const int laserPin = 2;

// Define motor driver pins
int in1 = 4;
int in2 = 5;
int in3 = 9;
int in4 = 10;

// Define ultrasonic sensor pins
int triggerPin1 = 13; // Ultrasonic sensor trigger pin
int echoPin1 = 12;    // Ultrasonic sensor echo pin

int triggerPin2 = 3;
int echoPin2 = 2;

// Thresholds
const int forwardThreshold = 35;
const int backwardThreshold = 25;
const int stopThreshold = 100;

//Resistance
const int resistance = 20;

int distances1[3] = {0}; // Initialize last three distances
int distances2[3] = {0};
int index1 = 0;
int index2 = 0;

int averageDistance1 = 0;
int averageDistance2 = 0;

// Function declarations
int readDistance(int triggerPin, int echoPin);
void driveForward();
void driveBackward();
void stopMotors();
void turnLeft();
void turnRight();
void averageDistances();
void calculateResistance();

void setup() {
  // Initialize motor control pins as outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(triggerPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  Serial.begin(9600);
}

void loop() {
  averageDistances();
  calculateResistance();

  int lastAverageDistance1 = averageDistance1;
  int lastAverageDistance2 = averageDistance2;

  int resistanceUpper = lastAverageDistance1 + resistance;
  int resistanceLower = lastAverageDistance1 - resistance;

  if (lastAverageDistance1 > stopThreshold && lastAverageDistance2 > stopThreshold){
    Serial.println("Too Far Away!");
    stopMotors();
  } else {
    if (lastAverageDistance2 < resistanceLower) {
      turnRight();
    } else if (lastAverageDistance2 > resistanceUpper) {
      turnLeft();
    } else {
      if (lastAverageDistance1 > forwardThreshold && lastAverageDistance2 > forwardThreshold) {
        driveForward();
      } else if (lastAverageDistance1 < backwardThreshold && lastAverageDistance2 < backwardThreshold) {
        driveBackward();
      } else {
        stopMotors();
      }
    }
  }
}


int readDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.0344 / 2;
  return distance;
}

void driveForward() {
  Serial.println("Driving Forward");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void driveBackward() {
  Serial.println("Driving Backwards");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stopMotors() {
  Serial.println("Stopping Motors");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void turnLeft() {
  Serial.println("Turning Left");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); // Stop right wheels
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); // Only left wheels move
}

void turnRight() {
  Serial.println("Turning Right");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW); // Only right wheels move
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); // Stop left wheels
}

void averageDistances() {
  int currentDistance1 = readDistance(triggerPin1, echoPin1);
  int currentDistance2 = readDistance(triggerPin2, echoPin2);

  distances1[index1] = currentDistance1;
  distances2[index2] = currentDistance2;
  index1 = (index1 + 1) % 3;
  index2 = (index2 + 1) % 3;

  int sum1 = 0, sum2 = 0;
  for (int i = 0; i < 3; i++) {
    sum1 += distances1[i];
    sum2 += distances2[i];
  }
  averageDistance1 = sum1 / 3;
  averageDistance2 = sum2 / 3;

  Serial.print("Sensor 1 Distance: ");
  Serial.println(averageDistance1);

  Serial.print("Sensor 2 Distance: ");
  Serial.println(averageDistance2);
}

void calculateResistance() {
    // Adjusted to calculate resistance correctly as per your requirements
    int resistancePlus1 = averageDistance1 + resistance;
    int resistanceMinus1 = averageDistance1 - resistance;

    // Output the resistance calculations for sensor 1
    Serial.print("Sensor 1 - Resistance for Last Average Distance + ");
    Serial.print(resistance);
    Serial.print(": ");
    Serial.println(resistancePlus1);
    Serial.print("Sensor 1 - Resistance for Last Average Distance - ");
    Serial.print(resistance);
    Serial.print(": ");
    Serial.println(resistanceMinus1);
}
