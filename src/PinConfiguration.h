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

#ifndef ORALBLUE_PINCONFIGURATION_H
#define ORALBLUE_PINCONFIGURATION_H

#include "mbed.h"

namespace Configuration{
    constexpr PinName RED_LED = PB_8;
    constexpr PinName YELLOW_LED = PB_9;
    constexpr PinName GREEN_LED = PA_10;
}

#endif //ORALBLUE_PINCONFIGURATION_H
