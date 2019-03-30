//
// Created by wise on 3/30/19.
//

#ifndef TESTBLE2_LEDMANAGER_H
#define TESTBLE2_LEDMANAGER_H

#include <cstdint>
#include <array>

namespace LedManager{

    template <typename LedImplementation>
    class Led {
    public:
        void setDimming(uint8_t dimmingValue){
            static_cast<LedImplementation*>(this)->setDimmingImpl(dimmingValue);
        }
    };

    template <class LED_T,std::size_t N>
    struct ProgressManager{

        explicit ProgressManager(std::array<LED_T,N> &leds):mLeds(leds){}

        void setProgress(uint8_t progress){
            progress = std::min(uint8_t{100},progress);
            auto remainingProgress = progress*N;
            for(size_t i= 0; i<N ; i++){
                if(remainingProgress>100){
                    mLeds[i].setDimming(100);
                    remainingProgress-=100;
                }else{
                    mLeds[i].setDimming(remainingProgress);
                    remainingProgress = 0;
                }
            }
        }

        ~ProgressManager(){
            for(auto &led : mLeds){
                led.setDimming(0);
            }
        }


    private:
        std::array<LED_T,N>&  mLeds;
    };

}

#endif //TESTBLE2_LEDMANAGER_H
