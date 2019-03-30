//
// Created by wise on 3/17/19.
//

#include "OralBlue.h"

constexpr int PROTOCOL_VERSION_INDEX = 2;
constexpr int TYPE_ID_INDEX = 3;
constexpr int FW_VERSION_INDEX = 4;
constexpr int STATUS_INDEX = 5;
constexpr int HIGH_PRESSURE_INDEX = 6;
constexpr int MOTOR_SPEED_INDEX = 6;
constexpr int HAS_PROFESSIONAL_INDEX = 6;
constexpr int BRUSHING_MINUTE_INDEX = 7;
constexpr int BRUSHING_SECONDS_INDEX = 8;
constexpr int BRUSHING_MODE_INDEX = 9;
constexpr int SECTOR_INDEX = 10;

constexpr static inline bool hasCorrectSize(int length){
    return length==11 || length ==13;
}
constexpr static inline bool hasValidPrefix(const uint8_t* data){
    return data[0]==0xDC && data[1] == 0x00;
}

constexpr static inline bool highPressureBitSelected(const uint8_t flag){
    return (flag & uint8_t{0x80})!=0;
}
constexpr static inline bool isMotorSpeedReduced(const uint8_t flag){
    return (flag & uint8_t{0x40})!=0;
}
constexpr static inline bool hasProfessionalTimerBitSelected(const uint8_t flag){
    return (flag & uint8_t{0x01})!=0;
}


OralBlue::State OralBlue::AdvertiseInfo::stateFromAdvertiseByte(uint8_t rawValue){
    switch (rawValue){
        case 0x01:
            return OralBlue::State::INIT;
        case 0x02:
            return OralBlue::State::IDLE;
        case 0x03:
            return OralBlue::State::RUN;
        case 0x04:
            return OralBlue::State::CHARGE;
        case 0x05:
            return OralBlue::State::SETUP;
        case 0x06:
            return OralBlue::State::FLIGHT_MENU;
        case 0x71:
            return OralBlue::State::FINAL_TEST;
        case 0x72:
            return OralBlue::State::PCB_TEST;
        case 0x73:
            return OralBlue::State::SLEEP;
        case 0x74:
            return OralBlue::State::TRANSPORT;
        default:
            return OralBlue::State::UNKNOWN;
    }
}

OralBlue::Mode OralBlue::AdvertiseInfo::modeFromAdvertiseByte(uint8_t rawValue) {
    switch (rawValue){
        case 0x00:
            return OralBlue::Mode::OFF;
        case 0x01:
            return OralBlue::Mode::DAILY_CLEAN;
        case 0x02:
            return OralBlue::Mode::SENSITIVE;
        case 0x03:
            return OralBlue::Mode::MASSAGE;
        case 0x04:
            return OralBlue::Mode::WHITENING;
        case 0x05:
            return OralBlue::Mode::DEEP_CLEAN;
        case 0x06:
            return OralBlue::Mode::TONGUE_CLEANING;
        case 0x07:
            return OralBlue::Mode::TURBO;
        default:
            return OralBlue::Mode::UNKNOWN;
    }
}

OralBlue::Sector OralBlue::AdvertiseInfo::sectorFromAdvertiseByte(uint8_t rawValue) {
    switch (rawValue & uint8_t(0x07)){
        case 0x01:
            return OralBlue::Sector::SECTOR_1;
        case 0x02:
            return OralBlue::Sector::SECTOR_2;
        case 0x03:
            return OralBlue::Sector::SECTOR_3;
        case 0x04:
            return OralBlue::Sector::SECTOR_4;
        case 0x05:
            return OralBlue::Sector::SECTOR_5;
        case 0x06:
            return OralBlue::Sector::SECTOR_6;
        case 0x07:
            return OralBlue::Sector::LAST_SECTOR;
        default:
            return OralBlue::Sector::NO_SECTOR;
    }
}

std::optional<OralBlue::AdvertiseInfo> OralBlue::AdvertiseInfo::fromAdvertiseData(const uint8_t *data, int length) {
    const uint8_t *currentFieldPtr = data;
    const uint8_t *endPtr = data+length;
    do{
        if(currentFieldPtr[1] == 0xFF){
            return fromVendorSpecificData(currentFieldPtr+2,currentFieldPtr[0]-1);
        }
        currentFieldPtr+=currentFieldPtr[0]+1;
    }while(currentFieldPtr<endPtr);
    return {};
}

std::optional<OralBlue::AdvertiseInfo> OralBlue::AdvertiseInfo::fromVendorSpecificData(const uint8_t *data, int length) {
    if(hasCorrectSize(length) && hasValidPrefix(data)){
        return std::make_optional<AdvertiseInfo>(
                data[PROTOCOL_VERSION_INDEX],
                data[TYPE_ID_INDEX],
                data[FW_VERSION_INDEX],
                stateFromAdvertiseByte(data[STATUS_INDEX]),
                highPressureBitSelected(data[HIGH_PRESSURE_INDEX]),
                isMotorSpeedReduced(data[MOTOR_SPEED_INDEX]),
                hasProfessionalTimerBitSelected(data[HAS_PROFESSIONAL_INDEX]),
                std::chrono::minutes(data[BRUSHING_MINUTE_INDEX])+
                std::chrono::seconds(data[BRUSHING_SECONDS_INDEX]),
                modeFromAdvertiseByte(data[BRUSHING_MODE_INDEX]),
                sectorFromAdvertiseByte(data[SECTOR_INDEX])
        );
    }else {
        return {};
    }
}
