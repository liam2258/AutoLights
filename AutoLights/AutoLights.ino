/*
  Project: AutoLights
  Author: Liam Dyer
  Created: January 10, 2024
  Last Modified: January 10, 2024
  Description: This Arduino sketch controls an LED strip based on motion detection. When motion is detected,
               it triggers a predefined LED animation sequence. After a period of inactivity, the LEDs fade out.
*/

#include <FastLED.h>
const int MOTION_PIN = 2;
const int LED_PIN = 7;
const int NUM_LEDS = 120;

CRGB leds[NUM_LEDS];
bool motionDetected = false;
unsigned long motionStartTime = 0;
const unsigned long motionDuration = 60000; // 1 minute in milliseconds
uint8_t brightnessLevel = 100; // Initial brightness

/*
  Function: setup
  Description: Initializes the LED strip, sets initial brightness, and starts serial communication.
*/
void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS); // Initialize the LED lights
  FastLED.setBrightness(brightnessLevel); // Set initial brightness
  Serial.begin(9600);
  pinMode(MOTION_PIN, INPUT);
}

/*
  Function: lightAnimation
  Description: This function runs the light animation, it continuously loops through the lights
               Turning them all blue then green then purple
*/
void lightAnimation() {
  for (int i = 0; i < 120; i++) {
    leds[i] = CRGB(0, 0, 255);
    FastLED.show();
    delay(15);
  }

  for (int i = 0; i < 120; i++) {
    leds[i] = CRGB(34, 139, 34);
    FastLED.show();
    delay(15);
  }

  for (int i = 0; i < 120; i++) {
    leds[i] = CRGB(75, 0, 130);
    FastLED.show();
    delay(15);
  }
}


/*
  Function: loop
  Description: Handles motion detection, animation, and LED control based on motion state and time elapsed.
*/
void loop() {
  int motionValue = digitalRead(MOTION_PIN);
  
  if (motionValue && !motionDetected) {
    motionDetected = true;
    motionStartTime = millis();
    Serial.println("Motion detected");
  }

  if (motionDetected) {
    unsigned long elapsedTime = millis() - motionStartTime;

    if (elapsedTime >= motionDuration) {
      // If 1 minute has passed
      if (!digitalRead(MOTION_PIN)) {
        // If no motion is detected after 1 minute, turn off LEDs
        motionDetected = false;

        // Fade out LEDs after 1 minute
        for (int fadeValue = brightnessLevel; fadeValue > 0; fadeValue -= 5) {
          FastLED.setBrightness(fadeValue); // Set brightness level
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i].fadeToBlackBy(5); // Fade out LEDs
          }
          FastLED.show();
          delay(50);
        }
        // Turn off LEDs completely
        FastLED.clear();
        FastLED.show();
        Serial.println("Turning off LEDs");
      } else {
        // If motion is still detected after 1 minute, reset the timer and restart animation
        motionStartTime = millis();
        lightAnimation();
      }
    } else {
      // Run the animation while motion is detected
      lightAnimation();
    }
  }
}

