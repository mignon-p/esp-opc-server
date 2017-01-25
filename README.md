This is an [Arduino][1] sketch which implements an
[Open Pixel Control][2] server for controlling [NeoPixels][3] from an
[ESP8266][5], such as the [Adafruit Feather HUZZAH][4].

Here is an example circuit:

<img src="breadboard.png" width="800" height="350" alt="Breadboard with ESP8266 Feather and level shifter" />

Bill of materials:

* [Adafruit Feather HUZZAH ESP8266][4]
* [74AHCT125 - Quad Level-Shifter][6]
* [470 ohm resistor][7]
* [Big Freaking Capacitor - 4700uF][8]
* [NeoPixel stick][9]

You will need to edit the sketch to set the number of pixels you have,
and your WiFi SSID and password.

Once the sketch is running, you'll need to figure out which IP address
it has been assigned.  Generally, your router's web interface will
have a way to look at DHCP leases, and you can find it from there.

Once you know the IP address, you can connect with any Open Pixel
Control client.  The [openpixelcontrol repository][10] has some
[clients in Python][11], and I have [a client in Haskell][12].

[1]: https://www.arduino.cc/
[2]: http://openpixelcontrol.org/
[3]: https://www.adafruit.com/category/168
[4]: https://www.adafruit.com/products/2821
[5]: https://github.com/esp8266/Arduino
[6]: https://www.adafruit.com/products/1787
[7]: https://www.adafruit.com/products/2781
[8]: https://www.adafruit.com/products/1589
[9]: https://www.adafruit.com/products/1426
[10]: https://github.com/zestyping/openpixelcontrol/
[11]: https://github.com/zestyping/openpixelcontrol/tree/master/python
[12]: https://github.com/ppelleti/hs-opc-client
