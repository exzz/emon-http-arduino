# emon-http-arduino

# emonlib HTTP JSON server
Super simple JSON HTTP server to read energy monitor status.

Please see [open energy monitor projet](http://openenergymonitor.org/emon/) and [emonlib](https://github.com/openenergymonitor/EmonLib)

This code has been tested with [this open energy monitor shield](http://openenergymonitor.org/emon/emontxshield/smt) and [Arduino yun](https://www.arduino.cc/en/Main/ArduinoBoardYun?from=Products.ArduinoYUN).

# Quickstart

- Configure your Arduino yun board with NTP enabled
- Clone this repo
- Setup your project with [platform.io](http://platform.io)
```shell
platformio init --board=yun
platformio lib install 116
```
- Customize your settings
```c++
// Energy monitor setup
emon1.current(1, 60.606);
emon1.voltage(0, 250, 1.7);
```
- Build code
```shell
platformio run
```
- Upload firmware to Arduino yun
```shell
scp .pioenvs/yun/firmware.hex arduino.local:/tmp/
ssh arduino.local '/usr/bin/merge-sketch-with-bootloader.lua /tmp/firmware.hex && /usr/bin/kill-bridge && /usr/bin/run-avrdude /tmp/firmware.hex'
```

# Usage

```shell
curl -i 'http://arduino.local:5555'
```

```json
HTTP/1.1 200 OK
Content-type: text/json

{ "SiteName": "Home", "ModuleName": "mainPower", "Irms": 0.95, "Vrms": 239.87, "RealPower": 126.21, "ApparentPower": 228.76, "PowerFactor": 0.55, "Time": 1442234001, "ExecTime": 1860788 }
```
