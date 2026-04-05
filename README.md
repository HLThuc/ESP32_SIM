# Arduino IDE Setup

### driver for arduino (install CP210x Universal Windows Driver)

* https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads

## install Arduino IDE

* download: https://www.arduino.cc/en/software/

## install esp32 package

* to go `file` -> `Preferences...`

* In the textbox `Additional boards manager URLs`, enter the following text: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`

* to go `Tools` -> `Manage libraries...` -> search `esp32` -> find and install `esp32 by Espressif Systems`

* search `Websockets_Generic` -> install `Websockets_Generic by Markus Sattler, Khoi Hoang`

## Upload code into esp32

* `Tools` -> `Ports` -> select esp32 port
* `Tools` -> `Board` -> `esp32` -> `DOIT ESP32 DEVKIT V1`
* Click `Upload` (->) Button

## ESP32 pinout

* https://mischianti.org/wp-content/uploads/2020/11/ESP32-DOIT-DEV-KIT-v1-pinout-mischianti.png

## sms web test

* https://sms-man.com/vn/free-numbers?id=132926
