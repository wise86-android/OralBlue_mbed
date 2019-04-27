//
// Created by wise on 4/27/19.
//

#ifndef TESTBLE2_LCDMANAGER_H
#define TESTBLE2_LCDMANAGER_H

#include "OralBlue.h"

class LCDManager{
public:
    virtual void setProgress(OralBlue::Sector sector,uint8_t progress) = 0;
};

#endif //TESTBLE2_LCDMANAGER_H
