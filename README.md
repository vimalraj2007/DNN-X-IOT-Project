Smart Delivery Box Condition Monitoring Using IoT

An ESP32-based simulation that monitors a delivery package's temperature and physical movement to identify potentially unsafe handling conditions. The project uses a DHT22 temperature sensor, an MPU6050 accelerometer/gyroscope, and an SSD1306 OLED display.

Project status: Sensor-based prototype/simulation. The current code uses threshold-based rules; a DNN/ML model is not yet implemented.

Features

Reads package-area temperature using the DHT22.

Measures acceleration using the MPU6050 and estimates shock deviation from gravity.

Classifies the parcel condition as SAFE, ABNORMAL, or SENSOR ERROR.

Displays the current condition and sensor readings on an OLED.

Prints detailed readings to the Serial Monitor.

Treats missing/invalid sensor readings as an error instead of incorrectly reporting the parcel as safe.

Hardware and Simulation Components

ESP32 DevKit

DHT22 temperature and humidity sensor

MPU6050 accelerometer and gyroscope

SSD1306 128×64 I²C OLED display

Wokwi online simulator

How It Works

The ESP32 reads the temperature from the DHT22.

The MPU6050 provides acceleration values on the X, Y, and Z axes.

The program calculates acceleration magnitude and estimates shock deviation from standard gravity (9.81 m/s²).

The readings are compared with the configured thresholds.

The parcel status is shown on the OLED and printed to the Serial Monitor.

Classification Rules

Condition

Result

Temperature is at or below 35 °C and shock deviation is at or below 3 m/s², with valid sensor readings

SAFE

Temperature exceeds 35 °C or shock deviation exceeds 3 m/s²

ABNORMAL

The MPU6050 is unavailable or the temperature reading is invalid

SENSOR ERROR

The thresholds are example values for demonstration and should be calibrated for the actual package, sensor placement, and transport conditions.

Pin Connections

Component

ESP32 connection

DHT22 data

GPIO 15

MPU6050 SDA

GPIO 21

MPU6050 SCL

GPIO 22

OLED SDA

GPIO 21

OLED SCL

GPIO 22

Sensor/OLED VCC

3.3 V

Sensor/OLED GND

GND

The MPU6050 and OLED share the ESP32's I²C bus. The OLED uses I²C address 0x3C.

Run the Simulation in Wokwi

Open the project in the Wokwi ESP32 simulator.

Start the simulation.

Open the Serial Monitor to view temperature, acceleration, shock deviation, and parcel status.

Observe the OLED output.

Change the simulated sensor values, where supported, to test normal and abnormal conditions.

You can also import the project files into a new Wokwi project:

sketch.ino — ESP32 program

diagram.json — circuit layout and connections

libraries.txt — required Arduino libraries

Required Libraries

The project uses these Arduino libraries:

Adafruit GFX Library

Adafruit SSD1306

Adafruit MPU6050

Adafruit Unified Sensor

DHT sensor library

Wokwi can install libraries listed in libraries.txt automatically. If building locally, install the libraries through the Arduino IDE Library Manager.

Repository Structure

.
├── sketch.ino       # ESP32 monitoring code
├── diagram.json     # Wokwi circuit and wiring
├── libraries.txt    # Required Arduino libraries
├── wokwi-project.txt
└── README.md

Current Limitations

The current implementation uses fixed threshold rules, not a trained DNN.

Shock deviation is a simple estimate based on acceleration magnitude; it is not a calibrated impact measurement.

The current program does not include GPS tracking, cloud storage, or a web dashboard.

A SAFE result only means the readings are within the configured thresholds; it does not guarantee that the parcel has no damage.

Future Enhancements

Collect labelled delivery-condition data for safe and abnormal journeys.

Train and integrate a DNN or other suitable ML model.

Add journey tracking with GPS.

Store readings in a database and display journey history on a dashboard.

Send notifications when abnormal temperature or shock events are detected.

Calibrate thresholds using real-world transport tests.

Project Objective

To build a simple IoT prototype that monitors temperature and physical shocks during package transportation and provides an immediate indication when handling conditions may be abnormal.

Author

Vimal Raj R.
