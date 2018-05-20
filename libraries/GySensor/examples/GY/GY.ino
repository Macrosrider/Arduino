#include <Wire.h>
#include <GySensor.h>

GySensor sensor = GySensor();

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  sensor.begin();  
  Serial.begin(9600);    
  delay(100);
}

void loop() {
    sensor.get_accel_data();
    sensor.get_gyro_data();
    sensor.get_mag_data();
    sensor.accel_low_pass_filter();
    int roll = sensor.get_roll(FILTERED);
    int pitch = sensor.get_pitch(FILTERED);
    int angle = sensor.get_rotation();
    Serial.print("Roll: ");
    Serial.print(roll);
    Serial.print(" | ");
    Serial.print("Pitch: ");
    Serial.print(pitch);
    Serial.print(" | ");
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" | ");
    Serial.print("Temperature: ");
    sensor.get_temperature();
    Serial.print(sensor.temperature);
    Serial.print(" | ");
    Serial.print("Pressure: ");
    sensor.get_pressure(PA);
    Serial.print(sensor.pressure);
    Serial.print(" | ");
    Serial.print("Altitude: ");
    sensor.get_altitude();
    Serial.println(sensor.altitude);
    delay(200);
}
