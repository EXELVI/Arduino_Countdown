
#include "RTC.h"

void setup() {
  Serial.begin(9600);
  RTC.begin();

  RTCTime savedTime;
  RTC.getTime(savedTime);

    int year = savedTime.getYear();
    int month = Month2int(savedTime.getMonth());
    int day = savedTime.getDayOfMonth();
    int hour = savedTime.getHour();
    int minute = savedTime.getMinutes();
    int second = savedTime.getSeconds();
    
    Serial.print("Saved time: ");
    Serial.print(year);
    Serial.print("-");
    Serial.print(month);
    Serial.print("-");
    Serial.print(day);
    Serial.print(" ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.println(second);
    
  if (!RTC.isRunning()) {
    if (savedTime.getYear() == 2000) {
      
    } else {  
      RTC.setTime(savedTime);
    }
  }
}

void loop() {

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

  if (secondsLeft > 0) {
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
  } else {
    Serial.println("-");
  }

  delay(1000);
}
