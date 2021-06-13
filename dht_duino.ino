/* dht_duino

Write time, humidity, and temperature to Serial

COMPONENTS

 * DHT Digital Humidity & Temperature Sensor (DHT library)

   How to use the DHT-22 sensor with Arduino Uno
   Temperature and humidity sensor
   More info: http://www.ardumotive.com/how-to-use-dht-22-sensor-en.html
   Dev: Michalis Vasilakis // Date: 1/7/2015 // www.ardumotive.com

 * DS3231 real-time-clock (RtcDS3231 library)
      CONNECTIONS:
      DS3231 SDA --> SDA
      DS3231 SCL --> SCL
      DS3231 VCC --> 3.3v or 5v
      DS3231 GND --> GND
 */

//Libraries
#include <DHT.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>

//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define LOOP_DELAY_MS 20
#define UPDATE_INTERVAL_MS 2000
#define countof(a) (sizeof(a) / sizeof(a[0]))

// Create
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
RtcDS3231<TwoWire> Rtc(Wire);


//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
unsigned long reference_time;
RtcDateTime compiled;
RtcDateTime rtc_now;
RtcTemperature rtc_temp;

void setup()
{
    Serial.begin(115200);
    Serial.println("======================================");

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.print(" ");
    Serial.println(__TIME__);

    //--------RTC SETUP ------------
    Rtc.Begin();
    compiled = RtcDateTime(__DATE__, __TIME__);
    check_RTC_setup();
    check_RTC_update();

    rtc_now = Rtc.GetDateTime();

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);

    //--------DHT22 SETUP ------------
    dht.begin();
    reference_time = 0;
}

void loop()
{
    char iso8601_datestring[20];

    check_RTC_loop();

    if (millis()-reference_time > UPDATE_INTERVAL_MS) {
      reference_time += UPDATE_INTERVAL_MS;
      //Read data and store it to variables hum and temp
      hum = dht.readHumidity();         // DHT22 relative humidity, %
      temp= dht.readTemperature();      // DHT22 temperature, C
      rtc_now = Rtc.GetDateTime();      // DS3231 time
      rtc_temp = Rtc.GetTemperature();  // DS3231 temperature

      snprintf_P(
        iso8601_datestring,
        countof(iso8601_datestring),
        PSTR("%04u-%02u-%02u %02u:%02u:%02u"),
        rtc_now.Year(),
        rtc_now.Month(),
        rtc_now.Day(),
        rtc_now.Hour(),
        rtc_now.Minute(),
        rtc_now.Second()
      );

      //Print temp and humidity values to serial monitor
      Serial.print(iso8601_datestring);
      Serial.print("  RH=");
      Serial.print(hum);
      Serial.print(" % T=");
      Serial.print(temp);
      Serial.print(" C");
      Serial.print(" % RTC_T=");
      Serial.print(rtc_temp.AsFloatDegC());
      Serial.println(" C");
    }
    delay(LOOP_DELAY_MS);
}

void check_RTC_setup() {
    if (!Rtc.IsDateTimeValid()) {
        if (Rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            Serial.println("RTC lost confidence in the DateTime!");

            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            Rtc.SetDateTime(compiled);
        }
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }
}

void check_RTC_update() {
    if (rtc_now < compiled)
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (rtc_now > compiled)
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (rtc_now == compiled)
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
}

void check_RTC_loop() {
    if (!Rtc.IsDateTimeValid())
    {
        if (Rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) the battery on the device is low or even missing and the power line was disconnected
            Serial.println("RTC lost confidence in the DateTime!");
        }
    }
}
