# M_IAQ

Here you'll find the files for my homemade Internal Air Quality sensor! It uses a custom PCB designed by me, as well as a SEN55 sensor, and a MHZ19C sensor. You can find the full list of meassured data below. The PCB is designed around the Xiao seeeduino ESP32c3, but something like a supermini will also work, though you'll have to adjust the pinouts in code. Keep in mind that I only accounted for the strapping pins of the Xiao esp32c3.

There are four buttons on top, useful for for example switching what data the OLED shows, or forcing the device to calibrate, or shutting down the sensors. These buttons are connected to boot pins, so they do not have pullup resistors in hardware.
The PCB also has an optional header for an OLED screen, to show data right on the device. I designed the board to be powered over the esp's USB port, trough the 5v pin. There is also the option of soldering on a micro-usb connector on the left as power input. The connector has the data lines shorted so most adaptors will give it up to 5W. If this causes issues, you can break the trace between the solder pads near the connector, and they'll be floating. 

## This sensor collects:
* ( True ) Co2, up to 2000ppm
* VoC
* Relative NoX
* particulate matter:
    * PM10
    * PM4
    * PM2.5
    * PM1

## optional features:
* Seperate micro-USB connector on the side for power ( data lines are not hooked up )
* Headers broken out for OLED screen, MHZ19C analog out, and MHZ19C PWM out. You could also hook up a more accurate temperature sensor to this header.

The SEN55 also has an internal temperature and humidity sensor, but especially the temperature sensor is really not that accurate. You can still use it if you want to. 

I have the SEN55 mounted with some strong double sided tape, and the MHZ19C is mounted with zipties. Feel free to make a more elegant solution for this. The Co2 analog and PWM outputs are also broken out as seperate headers, but are not connected to the esp32 on the pcb. 

The SMD headers on the underside are marked to show what sensor connector goes where, and there is a star near the header that connects to pin 1 of the sensor. For the MHZ19C that's the HD pin, for the SEN55 that's 5v. **Be sure to check how the pins connect, and connect them in the right orientation!**
