
// Including libraries.

#include <esp_camera.h>
#include <WiFi.h>

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

const char *ssid = "yourWifiName";
const char *password = "yourWifiPassword";

const int firstInfrared = 12;
const int secondInfrared = 3;
const int firstMotorA = 13;
const int secondMotorA = 15;
const int firstMotorB = 14;
const int secondMotorB = 2;

int firstInfraredValue = 0;
int secondInfraredValue = 0;

extern int led_duty;

// Defining helper functions.

void startCameraServer();
void setupLedFlash(int pin);

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // Configuring the camera.

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (config.pixel_format == PIXFORMAT_JPEG) {

    if (psramFound()) {

      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;

    } 
    
    else {

      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
      
    }
  } 

  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {

    Serial.printf("Camera init failed with error 0x%x", err);
    return;

  }

  sensor_t *s = esp_camera_sensor_get();

  if (config.pixel_format == PIXFORMAT_JPEG) {

    s->set_framesize(s, FRAMESIZE_QVGA);

  }

  // Configuring Wifi.

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  // Pin assignment.

  pinMode(firstInfrared, INPUT);
  pinMode(secondInfrared, INPUT);
  pinMode(firstMotorA, OUTPUT);
  pinMode(secondMotorA, OUTPUT);
  pinMode(firstMotorB, OUTPUT);
  pinMode(secondMotorB, OUTPUT);

  startCameraServer();

}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    Serial.print("Your ip address: ");
    Serial.println(WiFi.localIP());

    // Gearmotor logic.

    if (led_duty == 1) { // Red traffic light.

      analogWrite(firstMotorA, LOW);
      analogWrite(secondMotorA, LOW);
      analogWrite(firstMotorB, LOW);
      analogWrite(secondMotorB, LOW);

    }

    else if (led_duty == 0) {

      firstInfraredValue = digitalRead(firstInfrared);
      secondInfraredValue = digitalRead(secondInfrared);

      if (firstInfraredValue == 0 && secondInfraredValue == 0) { // Moves forward.

        analogWrite(firstMotorA, 170);
        analogWrite(secondMotorA, LOW);
        analogWrite(firstMotorB, 170);
        analogWrite(secondMotorB, LOW);

      }

      else if (firstInfraredValue == 1 && secondInfraredValue == 1) { // Stop.

        analogWrite(firstMotorA, LOW);
        analogWrite(secondMotorA, LOW);
        analogWrite(firstMotorB, LOW);
        analogWrite(secondMotorB, LOW);

      }

      else if (firstInfraredValue == 1 && secondInfraredValue == 0) { // Move to the left.

        analogWrite(firstMotorA, LOW);
        analogWrite(secondMotorA, LOW);
        analogWrite(firstMotorB, 170);
        analogWrite(secondMotorB, LOW);

      }

      else if (firstInfraredValue == 0 && secondInfraredValue == 1) { // Move to the right.
        
        analogWrite(firstMotorA, 170);
        analogWrite(secondMotorA, LOW);
        analogWrite(firstMotorB, LOW);
        analogWrite(secondMotorB, LOW);

      }

    }

  }

  else {

    Serial.println("Wifi connection lost or not established.");

  }

}
