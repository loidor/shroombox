/*
Shroombox code

The Shroombox is an automated mushroom fruiting chamber.

You need an AM2320 sensor, a two channel 5v relay,
a 5v fan and a 5v humidifier (I suggest something that runs on usb).

Connect the AM2320:
  Pin 1 (VDD) - 5V
  Pin 2 (SDA) - A4
  Pin 3 (GND) - GND
  Pin 4 (SCL) - A5

Connect the relay:
  Pin 1 (VCC) - 5V
  Pin 3 (GND) - GND
  Pin 3 (IN1) - D2
  Pin 4 (IN2) - D3

Connect the fan to relay 1, humidifier to relay 2.
*/

#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>

// Create a new instance of the Adafruit_AM2320 class:
Adafruit_AM2320 AM2320 = Adafruit_AM2320();

#define FANPIN 2
#define HUMIDPIN 3

//Setup humidifier & fan states
int humidifierState = 0;

//Setup fan timer
int fanTimer = 0;

void setup() {
  //Debug
  Serial.begin(9600);
  //Setup sensor
  AM2320.begin();
  
  //Setup relays
  pinMode(FANPIN, OUTPUT);
  pinMode(HUMIDPIN, OUTPUT);

}

void loop() {
  Serial.println("Now let's wait...");
  //Wait a minute. 
  delay(60000);

  //Increase fan timer
  fanTimer++;

  //Read humidity
  float h = AM2320.readHumidity();

  // Check if read failed and exit early (to try again):
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //Debug
  Serial.print("Humidity: ");
  Serial.print(h);

  //If humidity < 60, start humidifier. If > 80, stop humidifier.
  if (h < 60 && humidifierState == 0) {
    //Debug
    Serial.print("Humidity is less than 60%, starting humidifier.");
    digitalWrite(HUMIDPIN, HIGH);
    humidifierState = 1;
  }
  else if (h > 80 && humidifierState == 0) {
    //Debug
    Serial.print("Humidity is more than 80&, stopping humidifier.");
    digitalWrite(HUMIDPIN, LOW);
    humidifierState = 0;
  }

  //If the fan hasn't run for one hour, start it for five minutes
  if (fanTimer > 60) {
    //Debug
    Serial.print("Starting fan");
    digitalWrite(FANPIN, HIGH);
    delay(60000*5);
    fanTimer = 0;
    //Debug
    Serial.print("Stopping fan");
    digitalWrite(FANPIN, LOW);
  }

}
