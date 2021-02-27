// IndioRtc - MCP7940 Real Time Clock on the Industruino
// Copyright Neil Wells 2021  https://github.com/NeilSWells
// MIT License

#ifndef INDIO_RTC_h
#define INDIO_RTC_h

#include <Arduino.h>
#include <Wire.h>

#define INDIO_RTC_ID 0x6F
#define INDIO_RTC_EEPROM 0x57

struct now_t
{
    byte second;
    byte minute;
    byte hour;
    byte dow;
    byte day;
    byte month;
    byte year;
};

class IndioRtc
{
    public:
        void begin();
        void getTime (now_t * n);
        int  getTimeclockTime();
        bool newSecond();
        bool newMinute();
        void setSecond(byte s);
        void setMinute(byte m);
        void setHour(byte h);
        void setDOW(byte d);
        bool setDate(byte d, byte m, byte y);
        void getMac(byte * m);

    private:
        byte readSingle(byte addr);
        void writeSingle(byte addr, byte value);
        byte DecToBcd (byte b);
        byte BcdToDec (byte b);
        byte oldSecondBcd;
        byte oldMinuteBcd;
        //Binary coded decimals
        //Mask values shown on page 19 https://ww1.microchip.com/downloads/en/DeviceDoc/20005010H.pdf
        byte bcd[7];           // Second     Minute     Hour       Day of wk  Day        Month      Year
        const byte maskHigh[7] = {B01110000, B01110000, B00110000, B00000000, B00110000, B00010000, B11110000};
        const byte maskLow[7]  = {B00001111, B00001111, B00001111, B00000111, B00001111, B00001111, B00001111};
};

#endif
