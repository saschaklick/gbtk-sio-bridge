# gbtk-sio-bridge

Multiboard Arduino implementation for a Game Boy link port to USB serial communication bridge.

Flashing the provided firmware on a compatible microcontroller and attaching four wires of a Game Link cable allows to communicate with software running on a Game Boy and a computer.

The relevant signals the Game Link port consist of:

* `CLK` The **Clock** signal, which pulses every time a bit is send.
* `SO` **Signal out** which the receiving Game Boy reads in on a clock pulse.
* `SI` **Signal in** which the receiving Game Boy changes on each pulse to send data back.

Also required is the `GND` ground wire to ensure that the signals are getting through cleanly - not the shield ground, but the `GND` pin itself.

On every clock pulse one of the eight bits of the byte to be transfered - already prepared before the sending process was initiated - is transmitted, while the receiving Game Boy sends eight bits back. The latter also had to be prepared in advance by the receiver.

On the computer's side, the adapter acts as a serial port, using the microprocessor dev board's build in USB-serial adapter.

For each byte send to the serial port on the computer's side, one byte is send to the Game Boy and simultanously received byte is send back via the serial port.

## Requirements

* Visual Studio Code
* PlatformIO extension

## Microcontroller compatibilty

The firmware is written using the Arduino framework with PlatformIO to allow easy portability.

At the moment the following microprocessors are supported and tested:

* Espressif ESP32-C3 (using a NodeMCUv2 dev board)
* Espressif ESP8266 (using a NodeMCU dev board)
* Raspberry Pi Pico (using a official 2020 dev board)

Porting the firmware to further boards and controllers should be possible with minor adjustments.

## Microcontroller configuration

Each device has a seperate `[env]` entry in the *platformio.ini*. For each board a different *pin_config.h* file is included which specifies board-specific settings.

### Signal pins

The pins are all simple general purpose IO pins. The Game Boy, being a ~5V machine could overtax and possibly damage the 3.3V pin peripherals on non-5V-tolerant boards and controllers, but most controllers and dev boards are at least 5V-tolerant or the load on the Game link port is not high enough to damage any of the tested boards.

The pins are specified via preprocessor `#define` and are called `PIN_CLK`, `PIN_SO`, `PIN_SI` for the signal and additionally `PIN_STAT` which is mapped to the dev boards build-in LED and makes it act as an activity indicator, blinking when data is send and received.

### Baudrate

Another more or less relevant setting is the `SERIAL_BAUDRATE` which should match the baudrate specified for the serial port on the computer side. The ESP8266 for example requires the baudrate to be set to a slower and more standart value then the newer boards.

### Signal timings

While the baudrate has no effect on the actual speed of the clock signal on the Link cable, the two timing values `WAIT_BIT_US` and `WAIT_BYTE_US` do, as the former determines the length of a single bit pulse and the latter the pause between the sending of two bytes.

The bit pulse length might need to be increased if the board and cable are not able to create a clean pulse signal the Game Boy can recognize, while the wait between bytes is nescessary to give the Game Boy enough time to process the received byte and create a response byte for the next transfer. They can be dialed in for most efficient speed to reliability ratio, but very little is gained as the main bottleneck is the aforementioned speed with which the Game Boy is able to prepare the next byte.