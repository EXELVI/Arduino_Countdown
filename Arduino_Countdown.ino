#include <NTPClient.h>
#include "WiFiS3.h"
#include <WiFiUdp.h>
#include "WiFiSSLClient.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTC.h"

#include "arduino_secrets.h"
#include "icons.h"

const char *ssid = SECRET_SSID;
const char *pass = SECRET_PASS;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int status = WL_IDLE_STATUS;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
RTCTime currentTime;

bool updatedWithNTP = false;

void setup()
{
    Serial.begin(9600);
    RTC.begin();

    RTCTime savedTime;
    RTC.getTime(savedTime);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Countdown to");
    display.setCursor(0, 10);
    display.print("2024-07-27 23:59:59"); //This date is an example 
    display.display();
    delay(2000);

    Serial.print("Saved time: ");
    Serial.print(savedTime.getYear());
    Serial.print("-");
    Serial.print(Month2int(savedTime.getMonth()));
    Serial.print("-");
    Serial.print(savedTime.getDayOfMonth());
    Serial.print(" ");
    Serial.print(savedTime.getHour());
    Serial.print(":");
    Serial.print(savedTime.getMinutes());
    Serial.print(":");
    Serial.println(savedTime.getSeconds());

    if (!RTC.isRunning())
    {
        if (savedTime.getYear() == 2000)
        {
            Serial.println("RTC lost power, setting the time to 2024-07-27 12:00:00");
            RTCTime timeToSet = RTCTime(2024, Month::JULY, 27, 12, 0, 0, DayOfWeek::WEDNESDAY, SaveLight::SAVING_TIME_INACTIVE);
            RTC.setTime(timeToSet);
        }
        else
        {
            RTC.setTime(savedTime);
        }
    }
}

void loop()
{
    if (!updatedWithNTP)
    {
        if (WiFi.status() == WL_NO_MODULE)
        {
            Serial.println("Communication with WiFi module failed!");
            updatedWithNTP = true; // we will use the last saved time
        }

        String fv = WiFi.firmwareVersion();
        if (fv < WIFI_FIRMWARE_LATEST_VERSION)
        {
            Serial.println("Please upgrade the firmware");
        }

        if (status != WL_CONNECTED)
        {
            Serial.print("Attempting to connect to SSID: ");
            Serial.println(ssid);
            status = WiFi.begin(ssid, pass);
        }
        else
        {
            Serial.println("Connected to WiFi network");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());

            timeClient.begin();

            timeClient.update();

            auto unixTime = timeClient.getEpochTime();
            Serial.print("Unix time = ");
            Serial.println(unixTime);
            if (unixTime > 0)
            {
                RTCTime timeToSet = RTCTime(unixTime);
                RTC.setTime(timeToSet);

                RTC.getTime(currentTime);
                Serial.println("The RTC was just set to: " + String(currentTime));
            }
            else
            {
                Serial.println("Failed to get time from NTP server, will use the last saved time");
            }
            updatedWithNTP = true;
        }
    }

    RTCTime now;
    RTC.getTime(now);
    RTCTime targetDate;
    targetDate.setYear(2024);
    targetDate.setMonthOfYear(Month::JULY);
    targetDate.setDayOfMonth(27);
    targetDate.setHour(23);
    targetDate.setMinute(59);
    targetDate.setSecond(59);

    long secondsLeft = (targetDate.getUnixTime() - now.getUnixTime());

    if (secondsLeft > 0)
    {
        long days = secondsLeft / 86400;
        secondsLeft %= 86400;
        long hours = secondsLeft / 3600;
        secondsLeft %= 3600;
        long minutes = secondsLeft / 60;
        long seconds = secondsLeft % 60;

        Serial.print(days);
        Serial.print(" days, ");
        Serial.print(hours);
        Serial.print(" hours, ");
        Serial.print(minutes);
        Serial.print(" minutes, ");
        Serial.print(seconds);
        Serial.println(" seconds");
    }
    else
    {
        Serial.println("-");
    }

    delay(1000);
}
