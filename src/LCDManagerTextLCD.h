//
// Created by wise on 4/27/19.
//

#ifndef TESTBLE2_LCDMANAGERTEXTLCD_H
#define TESTBLE2_LCDMANAGERTEXTLCD_H

#include <string>
#include "LCDManager.h"

class LCDManagerTextLCD : LCDManager{
public:

    explicit LCDManagerTextLCD(TextLCD& lcd):mLcd(lcd){}

    void setProgress(OralBlue::Sector sector,uint8_t percentage) override {
        mLcd.locate(13,1);
        mLcd.printf("%02d%%",percentage);
    }
private:

    TextLCD &mLcd;

};

#endif //TESTBLE2_LCDMANAGERTEXTLCD_H
