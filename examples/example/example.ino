#include <Simple5641AS.h>

const uint8_t segmentPins[] = { 8, 9, 10, 11, 12, 13, A0, A1 };
const uint8_t digitSelectionPins[] = { 4, 5, 6, 7 };
Simple5641AS component( segmentPins, digitSelectionPins );

void setup() {
  Serial.begin(9600);
  Serial.println("Program started!");
}

const char letters[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g' };

void loop() {
  delay(300);

  Serial.println("Display Numbers:");
  for (int i = 0; i < 10; i++) {
    component.shortDisplayNumber(i);
    delay(500);
  }

  Serial.println("Display Letters:");
  for (int i = 0; i < sizeof(letters); i++) {
    component.shortDisplayLetter(letters[i]);
    delay(500);
  }

  Serial.println("Display Numbers:");
  for (int i = -500; i < 501; i++) {
    component.Display(7, 5, 5, i);
  }

  Serial.println("Animating zeros:");
  component.zeroAnimation();

  Serial.println("Display anything!");
  component.Display(5, "b2a6");

  // reset
  delay(5000);
}