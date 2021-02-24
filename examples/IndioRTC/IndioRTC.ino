#include "IndioRtc.h"
IndioRtc rtc;
now_t now;

char TimeText[20];

void setup()
{
    SerialUSB.begin(9600);
    delay(3000);
    SerialUSB.println("----");

    rtc.begin();

    rtc.setSecond(50);
    rtc.setMinute(57);
    rtc.newMinute(); //Call newMinute() after changing the minute otherwise it will immediately return true in the loop
    rtc.setHour(23);

    rtc.setDOW(1); //The Day of the Week function on the MCP7940 is just a rolling counter that increases at midnight and rolls over after 7

    //setDate(day,month,year) returns true if the date is valid. If the date is not valid, no changes are made.
    rtc.setDate(14, 2, 21);
    //if (!rtc.setDate(30, 2, 21)) SerialUSB.println("30th Feb 2021 is not a valid date");
}

void loop()
{
    delay(50);

    if (rtc.newMinute())
    {
        //getTimeclockTime() returns the time as an int for easier comparisons. 18:30 is returned as 1830
        if (rtc.getTimeclockTime() > 1830) SerialUSB.println("It's Pimms O'Clock!");
        else SerialUSB.println("----");
    }

    if (rtc.newSecond()) //Does the minimum needed to see if the second has changed
    {
        rtc.getTime(&now); //Call to refresh the values in now. Don't miss the &
        sprintf(TimeText, "%02d/%02d/%02d %02d:%02d:%02d", now.day, now.month, now.year, now.hour, now.minute, now.second);
        SerialUSB.print(TimeText);
        SerialUSB.print(" - ");
        switch (now.dow) //Day of the week.
        {
            case 1: SerialUSB.println("Sun"); break;
            case 2: SerialUSB.println("Mon"); break;
            case 3: SerialUSB.println("Tue"); break;
            case 4: SerialUSB.println("Wed"); break;
            case 5: SerialUSB.println("Thu"); break;
            case 6: SerialUSB.println("Fri"); break;
            case 7: SerialUSB.println("Sat"); break;
        }
    }
}
