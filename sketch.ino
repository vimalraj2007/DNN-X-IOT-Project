
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <math.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// Sensor configuration
#define DHT_PIN 15
#define DHT_TYPE DHT22

// Example thresholds for demonstration
#define TEMP_LIMIT 35.0
#define SHOCK_LIMIT 3.0

// Create sensor objects
Adafruit_SSD1306 display(
  SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET
);

Adafruit_MPU6050 mpu;
DHT dht(DHT_PIN, DHT_TYPE);

bool mpuReady = false;
bool oledReady = false;

void setup() {
  Serial.begin(115200);

  // Initialize I2C: SDA = GPIO 21, SCL = GPIO 22
  Wire.begin(21, 22);

  // Initialize temperature sensor
  dht.begin();

  // Initialize OLED
  oledReady = display.begin(
    SSD1306_SWITCHCAPVCC, OLED_ADDRESS
  );

  if (oledReady) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("SMART DELIVERY BOX");
    display.println("Initializing...");
    display.display();
  } else {
    Serial.println("OLED not detected!");
  }

  // Initialize MPU6050
  mpuReady = mpu.begin();

  if (mpuReady) {
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("MPU6050 initialized");
  } else {
    Serial.println("MPU6050 not detected!");
  }

  Serial.println("Delivery monitoring started");
}

void loop() {
  // Read temperature
  float temperature = dht.readTemperature();
  bool tempValid = !isnan(temperature);

  // Read acceleration
  float acceleration = 0.0;
  float shock = 0.0;

  if (mpuReady) {
    sensors_event_t accel, gyro, mpuTemp;
    mpu.getEvent(&accel, &gyro, &mpuTemp);

    float ax = accel.acceleration.x;
    float ay = accel.acceleration.y;
    float az = accel.acceleration.z;

    // Acceleration magnitude in m/s^2
    acceleration = sqrt(
      ax * ax + ay * ay + az * az
    );

    // Approximate deviation from gravity
    shock = fabs(acceleration - 9.81);
  }

  // Classify monitored conditions
  bool shockDetected =
    mpuReady && shock > SHOCK_LIMIT;

  bool tempAbnormal =
    tempValid && temperature > TEMP_LIMIT;

  bool abnormal = shockDetected || tempAbnormal;

  // A sensor failure must not be reported as SAFE
  const char* status;

  if (!mpuReady || !tempValid) {
    status = "SENSOR ERROR";
  } else if (abnormal) {
    status = "ABNORMAL";
  } else {
    status = "SAFE";
  }

  // Serial Monitor output
  Serial.println("\n----------------------");
  Serial.println("SMART DELIVERY BOX");

  if (tempValid) {
    Serial.print("Temperature: ");
    Serial.print(temperature, 1);
    Serial.println(" C");
  } else {
    Serial.println("Temperature: SENSOR ERROR");
  }

  if (mpuReady) {
    Serial.print("Acceleration: ");
    Serial.print(acceleration, 2);
    Serial.println(" m/s^2");

    Serial.print("Shock deviation: ");
    Serial.print(shock, 2);
    Serial.println(" m/s^2");
  } else {
    Serial.println("Acceleration: SENSOR ERROR");
  }

  Serial.print("Parcel status: ");
  Serial.println(status);

  // OLED display
  if (oledReady) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    if (!mpuReady || !tempValid) {
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println("SENSOR");
      display.println("ERROR!");

      display.setTextSize(1);
      display.println("Check wiring");
    }
    else if (abnormal) {
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println("DANGER!");

      display.setTextSize(1);
      display.setCursor(0, 27);

      if (shockDetected) {
        display.println("STRONG IMPACT!");
      }

      if (tempAbnormal) {
        display.println("HIGH TEMP!");
      }

      display.println("Parcel: ABNORMAL");
    }
    else {
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println("SAFE");

      display.setTextSize(1);
      display.setCursor(0, 27);

      display.print("Temp: ");
      display.print(temperature, 1);
      display.println(" C");

      display.print("Shock: ");
      display.print(shock, 1);
      display.println(" m/s2");

      display.println("Condition: Normal");
    }

    display.display();
  }

  delay(1000);
}
