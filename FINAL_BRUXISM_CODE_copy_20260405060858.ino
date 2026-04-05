#include <SPI.h>
#include "SdFat.h"

// ----------- SD CONFIGURATION -----------
// Use built-in SD card slot on Teensy 4.1 (uses SDIO)
#define SD_FAT_TYPE 3  // Use SdFs for best performance

#if SD_FAT_TYPE == 3
  SdFs sd;
  FsFile sdcard_file;
#else
  #error This setup requires SD_FAT_TYPE 3 (SdFs)
#endif

#define SD_CONFIG SdioConfig(FIFO_SDIO)  // Use SDIO (FAST)

// ----------- PINS AND CONSTANTS -----------
const int sensorPin = A0;
const int ledPin = LED_BUILTIN;

// ----------- SAMPLING SETTINGS -----------
const unsigned long SAMPLE_INTERVAL_US = 5000;  // 1 ms = 1000 Hz
unsigned long lastSampleTime = 0;

const int FLUSH_INTERVAL = 100;  // Flush every 100 samples
int sampleCount = 0;

// ----------- STATE FLAGS -----------
bool sdReady = false;
bool fileReady = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // LED solid during init
}

// ----------- MAIN LOOP -----------
void loop() {
  // Initialize SD card
  if (!sdReady) {
    if (sd.begin(SD_CONFIG)) {
      sdReady = true;
    } else {
      return;  // Try again on next loop
    }
  }

  // Create unique log file
  if (!fileReady) {
    char filename[32];
    for (int i = 0; i < 1000; i++) {
      snprintf(filename, sizeof(filename), "/brux_log_%03d.csv", i);
      if (!sd.exists(filename)) {
        if (sdcard_file.open(filename, O_CREAT | O_WRITE)) {
          sdcard_file.println("Timestamp(us),RawValue");
          fileReady = true;
          digitalWrite(ledPin, LOW);  // Blink LED after start
        }
        break;
      }
    }
    if (!fileReady) return;
  }

  // Blinking LED every 100ms for visual confirmation
  static unsigned long lastBlinkTime = 0;
  static bool ledState = false;
  unsigned long nowMillis = millis();
  if (nowMillis - lastBlinkTime >= 100) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    lastBlinkTime = nowMillis;
  }

  // Sampling Logic
  unsigned long nowMicros = micros();
  if (nowMicros - lastSampleTime >= SAMPLE_INTERVAL_US) {
    lastSampleTime += SAMPLE_INTERVAL_US;  // Avoid drift

    int val = analogRead(sensorPin);

    sdcard_file.print(lastSampleTime);  // Evenly spaced timestamps
    sdcard_file.print(',');
    sdcard_file.println(val);

    // Flush every FLUSH_INTERVAL samples
    sampleCount++;
    if (sampleCount >= FLUSH_INTERVAL) {
      sdcard_file.flush();
      sampleCount = 0;
    }
  }
}

