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

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 240)
const int epd_bitmap_allArray_LEN = 7;
const unsigned char *epd_bitmap_allArray[7] = {
    epd_bitmap_wifi,                      // 20x10px
    epd_bitmap_wifi_1,                    // 20x10px
    epd_bitmap_wifi_2,                    // 20x10px
    epd_bitmap_wifi_off,                  // 20x10px
    epd_bitmap_exclamation_lg,            // 20x10px
    epd_bitmap_exclamation_triangle_fill, // 32x23px
    epd_bitmap_arrow_repeat               // 20x10px
};

const char *ssid = SECRET_SSID;
const char *pass = SECRET_PASS;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int status = WL_IDLE_STATUS;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
RTCTime currentTime;
RTCTime targetDate;
bool updatedWithNTP = false;
bool warning = false;

unsigned long syncIconStart = 0; //Display the sync icon for 1 second

void setup()
{
    Serial.begin(9600);
    RTC.begin();

    targetDate.setYear(2024);
    targetDate.setMonthOfYear(Month::JULY);
    targetDate.setDayOfMonth(28);
    targetDate.setHour(14);
    targetDate.setMinute(07);
    targetDate.setSecond(59);

    RTCTime savedTime;
    RTC.getTime(savedTime);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Countdown to");
    display.setCursor(0, 10);
    int year = targetDate.getYear();
    int month = Month2int(targetDate.getMonth());
    int day = targetDate.getDayOfMonth();
    int hour = targetDate.getHour();
    int minute = targetDate.getMinutes();
    int second = targetDate.getSeconds();
    display.print(year);
    display.print("/");
    display.print(month);
    display.print("/");
    display.print(day);
    display.print(" ");
    display.print(hour);
    display.print(":");
    display.print(minute);
    display.print(":");
    display.print(second);
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
            Serial.println("RTC lost power!");
            warning = true;
            display.clearDisplay();
            display.setTextSize(1);
            display.setCursor(0, 0);
            display.print("RTC lost power!");
            display.drawBitmap(0, 10, epd_bitmap_exclamation_triangle_fill, 32, 23, WHITE);
            display.display();
            delay(2000);
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
                syncIconStart = millis();

                RTC.getTime(currentTime);
                Serial.println("The RTC was just set to: " + String(currentTime));
                warning = false;
            }
            else
            {
                Serial.println("Failed to get time from NTP server, will use the last saved time");
            }
            updatedWithNTP = true;
        }
    }
    display.clearDisplay();
    RTC.getTime(currentTime);
    printStatusBar();
    long secondsLeft = (targetDate.getUnixTime() - currentTime.getUnixTime());
    display.setTextSize(2);
    display.setCursor(0, 10);
    if (secondsLeft > 0)
    {
        long days = secondsLeft / 86400;
        long hours = (secondsLeft % 86400) / 3600;
        long minutes = (secondsLeft % 3600) / 60;
        long secs = secondsLeft % 60;

        if (days > 0)
        {
            display.print(days);
            display.print("d ");
            display.setTextSize(1);
        }
        if (hours > 0)
        {
            display.print(hours);
            display.print("h ");
            display.setTextSize(1);
        }
        if (minutes > 0)
        {
            display.print(minutes);
            display.print("m ");
            display.setTextSize(1);
        }
        display.print(secs);
        display.print("s");
    }
    else
    {
        display.print("End!");
    }

    display.setTextSize(1);
    display.setCursor(0, 25);
    display.print(targetDate.getYear());
    display.print("/");
    display.print(Month2int(targetDate.getMonth()) > 9 ? Month2int(targetDate.getMonth()) : "0" + String(Month2int(targetDate.getMonth())));
    display.print("/");
    display.print(targetDate.getDayOfMonth() > 9 ? targetDate.getDayOfMonth() : "0" + String(targetDate.getDayOfMonth()));
    display.print(" ");
    display.print(targetDate.getHour() > 9 ? targetDate.getHour() : "0" + String(targetDate.getHour()));
    display.print(":");
    display.print(targetDate.getMinutes() > 9 ? targetDate.getMinutes() : "0" + String(targetDate.getMinutes()));
    display.print(":");
    display.print(targetDate.getSeconds() > 9 ? targetDate.getSeconds() : "0" + String(targetDate.getSeconds()));

    display.display();
    delay(1000);
}
bool position1 = false; // there is an icon in position 1
bool itsSyncIcon = false; // there is a sync icon

void printStatusBar()
{

    display.setCursor(0, 0);

    int adjustedHour = (currentTime.getHour() + 2) % 24;
    String hours = String(adjustedHour);
    String minutes = String(currentTime.getMinutes());
    String seconds = String(currentTime.getSeconds());

    display.print(hours.length() == 1 ? "0" + hours : hours);
    display.print(":");
    display.print(minutes.length() == 1 ? "0" + minutes : minutes);
    display.print(":");
    display.print(seconds.length() == 1 ? "0" + seconds : seconds);
    display.setCursor(0, 10);
    display.setTextSize(1);

    

    if (warning)
    {
        //        display.drawBitmap(93, 0, epd_bitmap_allArray[4], 20, 10, WHITE);
        display.drawBitmap(position1 ? 93 : 80, 0, epd_bitmap_allArray[4], 20, 10, WHITE);
        position1 = true;
    }

    if (syncIconStart > 0 && millis() - syncIconStart < 1000)
    {
        display.drawBitmap(position1 ? 93 : 80, 0, epd_bitmap_allArray[6], 20, 10, WHITE);
        itsSyncIcon = true;
        position1 = true;
    } else {
        if (position1 && itsSyncIcon) {
            position1 = false;
        }
    }

    if (status != WL_CONNECTED)
    {
        display.drawBitmap(110, 0, epd_bitmap_allArray[3], 20, 10, WHITE);
    }
    else
    {
        int32_t rssi = WiFi.RSSI();
        if (rssi > -55)
        {
            display.drawBitmap(110, 0, epd_bitmap_allArray[0], 20, 10, WHITE);
        }
        else if (rssi > -70)
        {
            display.drawBitmap(110, 0, epd_bitmap_allArray[1], 20, 10, WHITE);
        }
        else
        {
            display.drawBitmap(110, 0, epd_bitmap_allArray[2], 20, 10, WHITE);
        }
    }
}