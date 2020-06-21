
//LED
#include <Adafruit_NeoPixel.h>
#define PIN 4
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

//MPU650
#include "Wire.h"
#include <MPU6050_light.h>
#include "model.h"

#define NUM_SAMPLES 30
#define NUM_AXES 3
// sometimes you may get "spikes" in the readings
// set a sensible value to truncate too large values
#define TRUNCATE_AT 35
//#define TRUNCATE_AT_g 360
float features[NUM_SAMPLES * NUM_AXES];
#define ACCEL_THRESHOLD 2
#define INTERVAL 30

MPU6050 mpu(Wire);


void setup() {
  //MPU6050 init
  Serial.begin(9600);
  Wire.begin();
  mpu.begin();
  Serial.println(F("Calculating gyro offset, do not move MPU6050"));
  delay(1000);
  mpu.calcGyroOffsets();
  Serial.println("Done!\n");

  //LED STRIP init
  strip.begin();
  strip.setBrightness(30);
  strip.show(); // Initialise all pixels to 'off'
  delay(30);
  rainbowStart(50);
  colorChange(strip.Color(0, 0, 0));
}


void loop() {
  mpu.update();
  float ax, ay, az;
  //float gx, gy, gz;

  ax = constrain(mpu.getAccX(), -TRUNCATE_AT, TRUNCATE_AT);
  ay = constrain(mpu.getAccY(), -TRUNCATE_AT, TRUNCATE_AT);
  az = constrain(mpu.getAccZ(), -TRUNCATE_AT, TRUNCATE_AT);

  /*
  gx = constrain(mpu.getGyroX(), -TRUNCATE_AT_g, TRUNCATE_AT_g);
  gy = constrain(mpu.getGyroY(), -TRUNCATE_AT_g, TRUNCATE_AT_g);
  gz = constrain(mpu.getGyroZ(), -TRUNCATE_AT_g, TRUNCATE_AT_g);
*/
  
 /*   Serial.print(ax);
    Serial.print('\t');
    Serial.print(ay);
    Serial.print('\t');
    Serial.print(az);
    Serial.print('\t');
    Serial.print(gx);
    Serial.print('\t');
    Serial.print(ay);
    Serial.print('\t');
    Serial.println(gz);
  */
  if (!motionDetected(ax, ay, az)) {
    delay(10);
    return;
  }

  recordMPU();

/*
 * Use classify to detect gestures which have been classified in the model.h file
 */
classify();

/*
 * Use printFeatures to collect data. It will print n amount of samples - use serial monitor to copy 
 * and paste the date recorded into a csv. file
 */
printFeatures();


delay(2000);
}


// Detect gesture and change color
void classify() {
  Serial.print("Detected gesture: ");
  Serial.println(classIdxToName(predict(features)));

  if (classIdxToName(predict(features)) == "nein") {
    colorChange(strip.Color(255, 0, 0)); // Red
  } else if (classIdxToName(predict(features)) == "nicken") {
    colorChange(strip.Color(0, 255, 0)); // Green
  }
}

// Detect coordinates of Gyroscope and Accelerometer
void recordMPU() {
  float ax, ay, az;
  //float gx, gy, gz;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    mpu.update();

    ax = constrain(mpu.getAccX(), -TRUNCATE_AT, TRUNCATE_AT);
    ay = constrain(mpu.getAccY(), -TRUNCATE_AT, TRUNCATE_AT);
    az = constrain(mpu.getAccZ(), -TRUNCATE_AT, TRUNCATE_AT);
   /* gx = constrain(mpu.getGyroX(), -TRUNCATE_AT_g, TRUNCATE_AT_g);
    gy = constrain(mpu.getGyroY(), -TRUNCATE_AT_g, TRUNCATE_AT_g);
    gz = constrain(mpu.getGyroZ(), -TRUNCATE_AT_g, TRUNCATE_AT_g); */


    features[i * NUM_AXES + 0] = ax;
    features[i * NUM_AXES + 1] = ay;
    features[i * NUM_AXES + 2] = az;
   /* features[i * NUM_AXES + 3] = gx;
    features[i * NUM_AXES + 4] = gy;
    features[i * NUM_AXES + 5] = gz;*/

    delay(INTERVAL);
  }
}

// Display recorded data
void printFeatures() {
  const uint16_t numFeatures = sizeof(features) / sizeof(double);

  for (int i = 0; i < numFeatures; i++) {
    Serial.print(features[i]);
    Serial.print(i == numFeatures - 1 ? '\n' : ',');
  }
}

// Detect motion
bool motionDetected(float ax, float ay, float az) {
  return (abs(ax) + abs(ay) + abs(az)) > ACCEL_THRESHOLD;
}

// Fill all dots with a color
void colorChange(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

// From the colorWipe boot sequence, start the rainbow
void rainbowStart(uint8_t wait) {
  uint16_t i, j;
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + 256 * 5 - 1) & 255));
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
