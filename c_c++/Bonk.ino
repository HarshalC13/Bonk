#include <WiFi.h> //Connect to WiFi Network
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h> //Used in support of TFT Display
#include <string.h>  //used for some string handling and processing.
#include <mpu6050_esp32.h>
#include<math.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

const uint8_t LOOP_PERIOD = 10; //milliseconds
uint32_t primary_timer = 0;
uint32_t record_timer = 0;
uint32_t loop_timer = 0;
float x, y, z;
int fall_data[6000];

const uint8_t INPUT_PIN1 = 26; //pin connected to button
const uint8_t INPUT_PIN2 = 27; //pin connected to button

uint8_t state;  //system state for step counting

#define IDLE 0  //change if you'd like
#define READING 1  //change if you'd like


MPU6050 imu; //imu object called, appropriately, imu

float old_acc_mag;
float highest_acc = 0;

void setup() {
  Serial.begin(115200); //begin serial comms
  delay(100); //wait a bit (100 ms)
  Wire.begin();
  delay(50); //pause to make sure comms get set up
  if (imu.setupIMU(1)) {
    //Serial.println("IMU Connected!");
  } else {
    //Serial.println("IMU Not Connected :/");
    //Serial.println("Restarting");
    ESP.restart(); // restart the ESP (proper way)
  }
  
  pinMode(INPUT_PIN1, INPUT_PULLUP); //set input pin as an input!
  state = IDLE;
}

void loop() {
  uint8_t button1 = digitalRead(INPUT_PIN1);
  measure_fall(button1);
  highest_acc = 0; // RESET
  while (millis() - primary_timer < LOOP_PERIOD); //wait for primary timer to increment
  primary_timer = millis();
}

void measure_fall(int button_press) {
  
  switch(state)
  {
  case IDLE:
      if (button_press == 0)
      {
        state =   READING;
      }
      break;
      
  case READING:
    int i = 0;
    record_timer = millis();
    while(millis() - record_timer < 5000)
    {
      imu.readAccelData(imu.accelCount);
      x = imu.accelCount[0] * imu.aRes;
      y = imu.accelCount[1] * imu.aRes;
      z = imu.accelCount[2] * imu.aRes;
      float acc_mag = sqrt(x * x + y * y + z * z);
      float avg_acc_mag = 1.0 / 2.0 * (acc_mag + old_acc_mag);
      char output[40];
      //sprintf(output,"%4.2f,%4.2f,%4.2f, %4.2f",x,y,z, avg_acc_mag); //render numbers with %4.2 float formatting
      Serial.println(avg_acc_mag); //print to serial for plotting
      if (avg_acc_mag > highest_acc)
      {
        highest_acc = avg_acc_mag;
      }
      //fall_data[i] = avg_acc_mag;
      old_acc_mag = avg_acc_mag;
      i++;
      while (millis() - loop_timer < LOOP_PERIOD); //wait for primary timer to increment
      loop_timer = millis();
    }
    state = IDLE;
    break;
  }

}
