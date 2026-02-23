# Air humidity (RH) measurement

## Description

An IoT device comprising a Raspberry Pi Pico, three sensors (including the HIH4020 air humidity sensor), additional passive electronic components and the ESP8266. 

## Test Software

SW V2.XX -> below an example web browser output (WD means "Watchdog"). 

Port forwarding within the Local Area Network is not necessary; if you wanted to access this particular device from the internet, the router would have to forward port 78 and you would need to know the router's public IP address.  

Since no SSL/TLS is or will be implemented (i.e. certificates), hence the connection is only HTTP, I would advise against making such an IoT device accessible to the wider world.

![Screenshot_20260221_082506_Chrome](https://github.com/user-attachments/assets/9e813d88-88c5-4820-94d9-8a30f8fa005e)

You can hardcode the WiFi credentials in your own build, but it is also possible to modify them in the uploaded .uf2-file via Picotool. Copy the file to a respective directory, and use the following shell commands:

*$ picotool config HIH4020-humidityMeasurement.uf2 -s SSID "WiFi-123"*

*$ picotool config HIH4020-humidityMeasurement.uf2 -s PASS "001122334455"*

The downside with this method is that these data is now plaintext in the .uf2-file and readable in a HEX-Editor.

![plaintext](https://github.com/user-attachments/assets/3e5d2040-7ced-4214-88fd-daaddf9500a5)

With all that being said, you'll probably see that this code example is only meant and good enough for experimental use.

## Hardware

The ad-hoc arrangement.

Wiring ESP8266 - Pico:

* Pin 1 - GND

* Pin 2 (TXO) - GP5 (UART1 RX)

* Pin 4 - 3.3V

* Pin 7 (RXI) - GP4 (UART1 TX)

* Pin 8 - 3.3V

Other pins n.c.

![Konfiguration--HIH4020-103AT2-SFH300--edited](https://github.com/user-attachments/assets/4849d053-7d6d-408c-93bc-e882a25d550d)
