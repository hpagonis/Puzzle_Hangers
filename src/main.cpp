#include <Arduino.h>
#include <Bounce2.h>

#define HANGER_NUM 5
#define DEBOUNCE_INTERVAL 100
#define SIG_PIN 13

struct  Hanger
{
  Bounce button;
  uint8_t pin;
};

Hanger hanger[HANGER_NUM] = {
  {Bounce(), 3},
  {Bounce(), 4},
  {Bounce(), 5},
  {Bounce(), 7},
  {Bounce(), 8},
};

uint8_t valid_state = 0B00001010;

void setup() {
  pinMode(SIG_PIN, OUTPUT);
  digitalWrite(SIG_PIN, LOW);
  for (int i = 0; i < HANGER_NUM; i++) {
    hanger[i].button.attach(hanger[i].pin, INPUT);
    hanger[i].button.interval(DEBOUNCE_INTERVAL);
    //hanger[i].button.setPressedState(LOW);
  }

  Serial.begin(115200);
  Serial.println("Puzzle Hangers initialized...");
}

void loop() {
  uint8_t state = 0;
  for (int i = 0; i < HANGER_NUM; i++) {
    hanger[i].button.update();
    state |= hanger[i].button.read() << i;
  }
  if (state == valid_state) {
    digitalWrite(SIG_PIN, HIGH);
    Serial.println("Relay open");
    delay(5000);
    digitalWrite(SIG_PIN, LOW);
    Serial.println("Relay closed");
  }
  delay(50);
}