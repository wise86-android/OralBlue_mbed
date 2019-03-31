//Copyright (c) 2019, Giovanni Visentini
//SPDX-License-Identifier: Apache-2.0
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//
//You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
//either express or implied.
//
//See the License for the specific language governing permissions and limitations under the License.

#ifndef ORALBLUE_ORALBLUE_H
#define ORALBLUE_ORALBLUE_H

#include <optional>
#include <cstdint>
#include <chrono>

namespace OralBlue {

    enum class Mode : uint8_t {
        OFF,
        DAILY_CLEAN,
        SENSITIVE,
        MASSAGE,
        WHITENING,
        DEEP_CLEAN,
        TONGUE_CLEANING,
        TURBO,
        UNKNOWN
    };

    enum class State {
        UNKNOWN,
        INIT,
        IDLE,
        RUN,
        CHARGE,
        SETUP,
        FLIGHT_MENU,
        FINAL_TEST,
        PCB_TEST,
        SLEEP,
        TRANSPORT,
    };

    enum class Sector{
        SECTOR_1,
        SECTOR_2,
        SECTOR_3,
        SECTOR_4,
        SECTOR_5,
        SECTOR_6,
        LAST_SECTOR,
        NO_SECTOR,
    };

    struct AdvertiseInfo {

        static std::optional<AdvertiseInfo> fromAdvertiseData(const uint8_t *data, int length);
        static std::optional<AdvertiseInfo> fromVendorSpecificData(const uint8_t *data, int length);

        static OralBlue::State stateFromAdvertiseByte(uint8_t rawValue);
        static OralBlue::Mode modeFromAdvertiseByte(uint8_t rawValue);
        static OralBlue::Sector sectorFromAdvertiseByte(uint8_t rawValue);

        constexpr explicit AdvertiseInfo(uint8_t protocolVersion,uint8_t typeId,uint8_t fwVersion,OralBlue::State state,
                                         bool highPressureDetected, bool hasReducedMotorSpeed, bool hasProfessionalTimer,
                                         std::chrono::seconds brushingTime,OralBlue::Mode mode,OralBlue::Sector sector)  noexcept :
                protocolVersion(protocolVersion),
                typeId(typeId),
                fwVersion(fwVersion),
                state(state),
                highPressureDetected(highPressureDetected),
                hasReducedMotorSpeed(hasReducedMotorSpeed),
                hasProfessionalTimer(hasProfessionalTimer),
                brushingTime(brushingTime),
                mode(mode),
                sector(sector){};

        const uint8_t protocolVersion;
        const uint8_t typeId;
        const uint8_t fwVersion;
        const OralBlue::State state;
        const bool highPressureDetected;
        const bool hasReducedMotorSpeed;
        const bool hasProfessionalTimer;
        const std::chrono::seconds brushingTime;
        const OralBlue::Mode mode;
        const OralBlue::Sector sector;
    };

}
#endif //ORALBLUE_ORALBLUE_H