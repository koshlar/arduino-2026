#define RED_PIN 7
#define GREEN_PIN 8
#define BLUE_PIN 9

unsigned long previousMillisRed = 0;
unsigned long previousMillisGreen = 0;
unsigned long previousMillisYellow = 0;

enum LightState { RED, GREEN, YELLOW, WAIT_RED, WAIT_GREEN, WAIT_YELLOW, IDLE };

class TrafficLight {
 protected:
  int redPin;
  int yellowPin;
  int greenPin;
  String currentLight;
  unsigned long lastChangeTime;
  LightState state;

 public:
  TrafficLight(int red, int yellow, int green) {
    redPin = red;
    yellowPin = yellow;
    greenPin = green;
    pinMode(redPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    currentLight = "off";
    lastChangeTime = 0;
    state = IDLE;
  }

  virtual void changeLight(String newLight) {
    currentLight = newLight;
    updateLEDs();
  }

  String getCurrentLight() { return currentLight; }

 protected:
  void updateLEDs() {
    digitalWrite(redPin, currentLight == "red" ? HIGH : LOW);
    digitalWrite(yellowPin,
                 yellowPin != -1 && currentLight == "yellow" ? HIGH : LOW);
    digitalWrite(greenPin, currentLight == "green" ? HIGH : LOW);
  }

  void randomColor() {
    int red = random(0, 256);
    int green = random(0, 256);
    int blue = random(0, 256);
    setColor(red, green, blue);
  }

  void setColor(int red, int green, int blue) {
    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);
  }
};

class CarTrafficLight : public TrafficLight {
 public:
  CarTrafficLight(int red, int yellow, int green)
      : TrafficLight(red, yellow, green) {}

  void changeLightCycle() {
    unsigned long currentMillis = millis();

    switch (state) {
      case IDLE:
        changeLight("red");
        randomColor();
        lastChangeTime = currentMillis;
        state = WAIT_RED;
        break;

      case WAIT_RED:
        if (currentMillis - lastChangeTime >= 3000) {
          changeLight("green");
          randomColor();
          lastChangeTime = currentMillis;
          state = WAIT_GREEN;
        }
        break;

      case WAIT_GREEN:
        if (currentMillis - lastChangeTime >= 3000) {
          changeLight("yellow");
          randomColor();
          lastChangeTime = currentMillis;
          state = WAIT_YELLOW;
        }
        break;

      case WAIT_YELLOW:
        if (currentMillis - lastChangeTime >= 1000) {
          changeLight("red");
          randomColor();
          lastChangeTime = currentMillis;
          state = WAIT_RED;
        }
        break;
    }
  }
};

class PedestrianTrafficLight : public TrafficLight {
 public:
  PedestrianTrafficLight(int red, int green) : TrafficLight(red, -1, green) {}

  void changeLightCycle() {
    unsigned long currentMillis = millis();

    switch (state) {
      case WAIT_RED:
        if (currentMillis - lastChangeTime >= 3000) {
          changeLight("green");
          randomColor();
          lastChangeTime = currentMillis;
          state = WAIT_GREEN;
        }
        break;

      case IDLE:
        changeLight("red");
        randomColor();
        lastChangeTime = currentMillis;
        state = WAIT_RED;
        break;

      case WAIT_GREEN:
        if (currentMillis - lastChangeTime >= 4000) {
          changeLight("red");
          randomColor();
          lastChangeTime = currentMillis;
          state = WAIT_RED;
        }
        break;
    }
  }
};

const int CAR_RED_PIN = 2;
const int CAR_YELLOW_PIN = 3;
const int CAR_GREEN_PIN = 4;
const int PED_RED_PIN = 5;
const int PED_GREEN_PIN = 6;

CarTrafficLight carLight(CAR_GREEN_PIN, CAR_YELLOW_PIN, CAR_RED_PIN);
PedestrianTrafficLight pedLight(PED_RED_PIN, PED_GREEN_PIN);

void setup() { setColor(0, 0, 0); }

void setColor(int red, int green, int blue) {
  // Объявление светофоров
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void loop() {
  carLight.changeLightCycle();
  pedLight.changeLightCycle();
}