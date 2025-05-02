
const int firstLed = 1;
const int secondLed = 2;
const int thirdLed = 3;
const int fourthLed = 4;
const int fifthLed = 5;
const int sixthLed = 6;

const long redLightTime = 5000;
const long greenLightTime = 3000;
const long yellowLightTime = 2000;

unsigned long currentTime;
unsigned long firstElapsedTime;
unsigned long secondElapsedTime;
unsigned long firstTime = 0;
unsigned long secondTime = 0;

int firstState = 0;
int secondState = 0;

void setup() {

  // Pin assignment.

  pinMode(firstLed, OUTPUT);
  pinMode(secondLed, OUTPUT);
  pinMode(thirdLed, OUTPUT);
  pinMode(fourthLed, OUTPUT);
  pinMode(fifthLed, OUTPUT);
  pinMode(sixthLed, OUTPUT);

}

// Defining helper functions.

void firstTrafficLight(unsigned long currentTime, unsigned long firstElapsedTime, int &firstState) {

  if (firstState == 0) { // Red traffic light.
  
    digitalWrite(secondLed, HIGH);
  
    if (firstElapsedTime >= greenLightTime) {

      digitalWrite(secondLed, LOW);

      firstState = 1;
      firstTime = currentTime;
    }
  
  }

  else if (firstState == 1) { // Green traffic light.

    digitalWrite(thirdLed, HIGH);

    if (firstElapsedTime >= yellowLightTime) {

      digitalWrite(thirdLed, LOW);

      firstState = 2;
      firstTime = currentTime;
    }
  } 
  
  else if (firstState == 2) { // Yellow traffic light.

    digitalWrite(firstLed, HIGH);

    if (firstElapsedTime >= redLightTime) {
      
      digitalWrite(firstLed, LOW);

      firstState = 0;
      firstTime = currentTime;
    }
  }

}

void secondTrafficLight(unsigned long currentTime, unsigned long secondElapsedTime, int &secondState) {

  if (secondState == 0) { // Red traffic light.
  
    digitalWrite(fourthLed, HIGH);
  
    if (secondElapsedTime >= redLightTime) {

      digitalWrite(fourthLed, LOW);

      secondState = 1;
      secondTime = currentTime;
    }
  
  }

  else if (secondState == 1) { // Green traffic light.

    digitalWrite(fifthLed, HIGH);

    if (secondElapsedTime >= greenLightTime) {

      digitalWrite(fifthLed, LOW);

      secondState = 2;
      secondTime = currentTime;
    }
  } 
  
  else if (secondState == 2) { // Yellow traffic light.

    digitalWrite(sixthLed, HIGH);

    if (secondElapsedTime >= yellowLightTime) {
      
      digitalWrite(sixthLed, LOW);

      secondState = 0;
      secondTime = currentTime;
    }
  }

}

void loop() {
  
  currentTime = millis();
  firstElapsedTime = currentTime - firstTime;
  secondElapsedTime = currentTime - secondTime;

  firstTrafficLight(currentTime,firstElapsedTime,firstState);
  secondTrafficLight(currentTime,secondElapsedTime,secondState);

}