#include "IndioRtc.h"

//Public
void IndioRtc::begin()
{
    Wire.begin();
    Wire.beginTransmission(INDIO_RTC_ID);
    Wire.write(0);
    Wire.endTransmission();
    Wire.requestFrom(INDIO_RTC_ID, 4);
    for (byte b = 0; b < 4; b++) bcd[b] = Wire.read();
    //Set bit 3 of register 3 to 1 to enable External Battery Backup Supply (VBAT)
    bitWrite(bcd[3], 3, 1);
    writeSingle(3, bcd[3]);
    //Set bit 7 of register 0 to 1 to enable the Oscillator
    bitWrite(bcd[0], 7, 1);
    writeSingle(0, bcd[0]);
    //Set old values to stop newSecond and newMinute returning true at bootup
    newMinute();
    newSecond();
}


void IndioRtc::getTime (now_t * n)
{
    Wire.beginTransmission(INDIO_RTC_ID);
    Wire.write(0);
    Wire.endTransmission();
    Wire.requestFrom(INDIO_RTC_ID, 7);
    for (byte b = 0; b < 7; b++) bcd[b] = Wire.read();
    n->second = BcdToDec(0);
    n->minute = BcdToDec(1);
    n->hour   = BcdToDec(2);
    n->dow    = BcdToDec(3);
    n->day    = BcdToDec(4);
    n->month  = BcdToDec(5);
    n->year   = BcdToDec(6);
}


int IndioRtc::getTimeclockTime()
{
    //Timeclock time is a single int that is easier for quick comparisons. 17:35 is represented as 1735
    Wire.beginTransmission(INDIO_RTC_ID);
    Wire.write(1);
    Wire.endTransmission();
    Wire.requestFrom(INDIO_RTC_ID, 2);
    bcd[1] = Wire.read();
    bcd[2] = Wire.read();
    return (100 * BcdToDec(2)) + BcdToDec(1); //(100 * Hours) + Minutes
}


bool IndioRtc::newSecond()
{
    bcd[0] = readSingle(0);
    if (oldSecondBcd != bcd[0])
    {
        oldSecondBcd = bcd[0];
        return true;
    }
    return false;
}


bool IndioRtc::newMinute()
{
    bcd[1] = readSingle(1);
    if (oldMinuteBcd != bcd[1])
    {
        oldMinuteBcd = bcd[1];
        return true;
    }
    return false;
}


void IndioRtc::setSecond(byte s)
{
    if (s >= 0 && s < 60)
    {
        bcd[0] = DecToBcd (s);
        bitWrite(bcd[0], 7, 1); //Set the 7th bit to 1 to enable the Oscillator
        writeSingle(0, bcd[0]);
    }
}


void IndioRtc::setMinute(byte m)
{
    if (m >= 0 && m < 60) writeSingle(1, DecToBcd (m));
}


void IndioRtc::setHour(byte h)
{
    if (h >= 0 && h < 24) writeSingle(2, DecToBcd (h));
}


void IndioRtc::setDOW(byte d)
{
    if (d > 0 && d < 8)
    {
        bcd[3] = DecToBcd (d);
        bitWrite(bcd[3], 3, 1); //Set the 3rd bit to 1 to enable External Battery Backup Supply (VBAT)
        writeSingle(3, bcd[3]);
    }
}


bool IndioRtc::setDate(byte d, byte m, byte y)
{
    if ((y >= 0 && y < 100) && (m > 0 && m < 13) && (d > 0 && d < 32))
    {
        switch (m)
        {
            case 2: if ((((y + 2000) % 4 == 0) && d > 29) || d > 28) return false;
            case 4: if (d > 30) return false;
            case 6: if (d > 30) return false;
            case 9: if (d > 30) return false;
            case 11: if (d > 30) return false;
        }
        if (d > 31) return false;
        writeSingle(4, DecToBcd (1)); //Temporarily set the date to 1 to avoid unexpedted outcomes as the date is being set
        writeSingle(6, DecToBcd (y));
        writeSingle(5, DecToBcd (m));
        writeSingle(4, DecToBcd (d));
        return true;
    }
}


void IndioRtc::getMac (byte * m)
{
    Wire.beginTransmission(INDIO_RTC_EEPROM);
    Wire.write(0xf0);
    Wire.endTransmission();
    Wire.requestFrom(INDIO_RTC_EEPROM, 8);
    m[0] = Wire.read();
    m[1] = Wire.read();
    m[2] = Wire.read();
    Wire.read();
    Wire.read();
    m[3] = Wire.read();
    m[4] = Wire.read();
    m[5] = Wire.read();
}


//Private
byte IndioRtc::readSingle(byte addr)
{
    Wire.beginTransmission(INDIO_RTC_ID);
    Wire.write(addr);
    Wire.endTransmission();
    Wire.requestFrom(INDIO_RTC_ID, 1);
    return Wire.read();
}


void IndioRtc::writeSingle(byte addr, byte value)
{
    Wire.beginTransmission(INDIO_RTC_ID);
    Wire.write(addr);
    Wire.write(value);
    Wire.endTransmission();
}


byte IndioRtc::DecToBcd (byte b)
{
    return ((b / 10) << 4) + (b % 10);
}


byte IndioRtc::BcdToDec(byte b)
{
    return (((bcd[b] & maskHigh[b]) >> 4) * 10) + (bcd[b] & maskLow[b]);
}
