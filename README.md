# IndioRtc
## _Real Time Clock Library for the Industruino_

Library to access the MCP7940 clock on the Industruino D21G.
The library assumes that there is a CR1220 backup battery for the RTC.

## Features
- `setDate(d,m,y)` changes the values in the RTC if the date is valid and returns false if not valid
- `getTimeclockTime()` returns the time as a single **int** for easy comparisons
- `newSecond()` does the minimum necessary to see if the second has changed. Useful for updating a clock display.

## Basic code
```sh
#include <IndioRtc.h>
IndioRtc rtc;
now_t now;

void setup()
{
    SerialUSB.begin(9600);
    rtc.begin();
}

void loop()
{
    if (rtc.newSecond())
    {
        rtc.getTime(&now);
        SerialUSB.println(now.second);
    }
}
```

## Functions
    begin()
    setSecond(byte)
    setMinute(byte)
    setHour(byte)
    setDOW(byte)
    bool = setDate(byte,byte,byte) //(day, month, 2 digit year)
    getTime(&now_t)
    int = getTimeclockTime()
    bool = newSecond()
    bool = newMinute()

## Data Type
    now_t
    {
        byte second
        byte minute
        byte hour
        byte dow
        byte day
        byte month
        byte year
    }

## Notes

### setDate()
`setDate(28,2,21);` Sets the date in the RTC and returns true as 28th February 2021 is a valid date.

`setDate(29,2,21);` Returns false as 2021 is not a leap year. The RTC is unchanged.

### newSecond() and newMinute()
Each function does the minimum necessary to determine if the second or minute has changed.
Returns true if the second or minute has changed **since the function was last called.**

Use with care, each function should only be called in one place in your code to avoid unexpected results.

### getTimeclockTime()
Returns an **int** that represents the time for easier comparisons.
13:15 is returned as 1315
```
int t = rtc.getTimeclockTime();
if (t > 1130 && t < 1345) SerialUSB.print("Getting hungry");
```

### setDOW()
The Day of the Week function on the MCP7940 is just a counter that increases at midnight and rolls over after 7
It has no relationship to the date.

## Contact
https://github.com/NeilSWells

## License

MIT
