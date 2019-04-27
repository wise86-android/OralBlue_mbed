//
// Created by wise on 4/27/19.
//
#include <map>
#include "LCDManagerTextLCD.h"

static constexpr auto SECTOR_1_NAME = "SX-DOWN";
static constexpr auto SECTOR_2_NAME = "SX-UP";
static constexpr auto SECTOR_3_NAME = "DX-DOWN";
static constexpr auto SECTOR_4_NAME = "DX-UP";

enum class ProgressChar: unsigned char{
    PROGRESS_0=0,
    PROGRESS_2=1,
    PROGRESS_4=2,
    PROGRESS_6=3,
    PROGRESS_8=4,
    PROGRESS_10=5,
};

static const std::map<ProgressChar,std::array<char,8>> customProgressChar = {
        //generated from: https://www.quinapalus.com/hd44780udg.html
        {ProgressChar::PROGRESS_0, {0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
        {ProgressChar::PROGRESS_2, {0x10,0x10,0x10,0x10,0x10,0x10,0x10}},
        {ProgressChar::PROGRESS_4, {0x18,0x18,0x18,0x18,0x18,0x18,0x18}},
        {ProgressChar::PROGRESS_6, {0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c}},
        {ProgressChar::PROGRESS_8, {0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e}},
        {ProgressChar::PROGRESS_10,{0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f}}
};

LCDManagerTextLCD::LCDManagerTextLCD(TextLCD& lcd):mLcd(lcd){
    //for (auto const& [charId, charBitMap] : customProgressChar){
    //    mLcd.setUDC(charId,charBitMap);
    //}
}

void LCDManagerTextLCD::setProgress(const OralBlue::Sector sector,const uint8_t percentage){
    printZoneName(sector);
    printZoneProgress(sector);
    printPercentage(percentage);
    printProgressBar(percentage);
}

void LCDManagerTextLCD::printZoneName(const OralBlue::Sector sector){
    mLcd.locate(0,0);
    switch (sector){
        case OralBlue::Sector::SECTOR_1:
            mLcd.puts(SECTOR_1_NAME);
            break;
        case OralBlue::Sector::SECTOR_2:
            mLcd.puts(SECTOR_2_NAME);
            break;
        case OralBlue::Sector::SECTOR_3:
            mLcd.puts(SECTOR_3_NAME);
            break;
        case OralBlue::Sector::SECTOR_4:
            mLcd.puts(SECTOR_4_NAME);
            break;
        default:
            break;
    }
}

void LCDManagerTextLCD::printZoneProgress(const OralBlue::Sector sector){
    mLcd.locate(13,0);
    switch (sector){
        case OralBlue::Sector::SECTOR_1:
            mLcd.puts("1/4");
            break;
        case OralBlue::Sector::SECTOR_2:
            mLcd.puts("2/4");
            break;
        case OralBlue::Sector::SECTOR_3:
            mLcd.puts("3/4");
            break;
        case OralBlue::Sector::SECTOR_4:
            mLcd.puts("4/4");
            break;
        default:
            break;
    }
}

void LCDManagerTextLCD::printPercentage(const uint8_t percentage){
    mLcd.locate(13,1);
    mLcd.printf("%02d%%",percentage);
}

void LCDManagerTextLCD::printProgressBar(const uint8_t percentage){
    mLcd.locate(0,1);
    for(auto i = 0 ; i<10; i++){
        if(percentage>=i*10) {
            mLcd.putc(0);
        }else{
            mLcd.putc(' ');
        }
    }
}

