#include <Simple5641AS.h>

const uint8_t segmentPins[] = { 8, 9, 10, 11, 12, 13, A0, A1 };
const uint8_t digitSelectionPins[] = { 4, 5, 6, 7 };
Simple5641AS component( segmentPins, digitSelectionPins );

void setup() {
  // Setup
}

// Main loop
void loop() {
  component.Display(1, "2bg4");
}