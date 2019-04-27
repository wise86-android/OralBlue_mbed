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
        printPercentage(percentage);
        printProgressBar(percentage);
    }


private:
    void printPercentage(uint8_t percentage){
        mLcd.locate(13,1);
        mLcd.printf("%02d%%",percentage);
    }

    void printProgressBar(uint8_t percentage){
        auto nSquare = percentage/10;
        mLcd.locate(0,1);
        for(auto i = 0 ; i<10; i++){
            if(percentage>=i*10) {
                mLcd.printf("%c", 219);
            }else{
                mLcd.printf(" ");
            }
        }
    }

    TextLCD &mLcd;

};

#endif //TESTBLE2_LCDMANAGERTEXTLCD_H
