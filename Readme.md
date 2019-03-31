#OralBlue MBed

#What and Way
Because my toothbrush has the bluetooth low energy, since it is a totally useless thing, I need do do something with it.

The toothbrush vibrates each 30seconds to tell you that is time to change your mouth sector.

My idea to create something that tell you your progress, and whitch sector you should clean.

##Setup
I tested this project using an [NUCLEO-F401RE](https://os.mbed.com/platforms/ST-Nucleo-F401RE/) with an [X-NUCLEO-IDB05A1](https://os.mbed.com/components/X-NUCLEO-IDB05A1-Bluetooth-Low-Energy/) BLE expansion board.

I connect 3 colored leds to the pins: PB_8 (Red), PB_9 (Yellow),PA_10 (green).

You can change the pins changing the PinConfiguration.h file, but note that the brightness is controlled using a PWM signal.


##How to compile
This project use some c++17 feature,becouse I started in 2019.
For this reason you have to use a custom build profile, that enable c++17 for GCC Arm.

To do you have to run:

`mbed compile --profile mbed-os/tools/profiles/develop.json --profile enableCpp17.json`

##Note
This project was develop during a [Open Source Saturday](https://www.meetup.com/it-IT/Open-Source-Saturday-Milano/)
