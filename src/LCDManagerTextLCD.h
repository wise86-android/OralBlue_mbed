//
// Created by wise on 4/27/19.
//

#ifndef TESTBLE2_LCDMANAGERTEXTLCD_H
#define TESTBLE2_LCDMANAGERTEXTLCD_H

#include <string>
#include "TextLCD.h"
#include "LCDManager.h"

class LCDManagerTextLCD : LCDManager{
public:

    explicit LCDManagerTextLCD(TextLCD& lcd);
    void setProgress(OralBlue::Sector sector,uint8_t percentage) override;

private:

    void printZoneName(const OralBlue::Sector sector);
    void printZoneProgress(const OralBlue::Sector sector);
    void printPercentage(const uint8_t percentage);
    void printProgressBar(const uint8_t percentage);

    TextLCD &mLcd;
};

#endif //TESTBLE2_LCDMANAGERTEXTLCD_H
