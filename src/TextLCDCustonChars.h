//
// Created by wise on 4/27/19.
//

#ifndef TESTBLE2_TEXTLCDCUSTONCHAR_H
#define TESTBLE2_TEXTLCDCUSTONCHAR_H

#include "TextLCD.h"

class TextLCDCustomChars : public TextLCD {
public:
    TextLCDCustomChars(PinName rs, PinName e, PinName d4, PinName d5, PinName d6, PinName d7, LCDType type = LCD16x2):
        TextLCD(rs,e,d4,d5,d6,d7,type){
        writeCommand(0x40); //set custom char 0
        for(int i= 0; i<7;i++){
            writeData(0x1F);
        }
    }


};
#endif //TESTBLE2_TEXTLCDCUSTONCHAR_H
