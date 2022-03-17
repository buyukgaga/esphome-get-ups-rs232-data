# esphome-UPS-Makelsan


I tried to develop it with ESPHome and Wemos D1 Mini esp8266 to receive and process data from the RS232 port of my Makelsan 1kVA UPS device.

The devices I use are Wemos D1 Mini ESP8266 and RS232 to TTL. You can find the web links below.

The communication ports of the devices are RX to RX and TX to TX.

Battery capacity and used watt calculation may need to be reviewed, especially for sensors created for devices below or above 1kVA.

The nice thing about the development is that I will not be disturbed by the device giving a warning signal during a power outage. With the automation installed on Home Assistant, I can automatically turn off the device warning sound when there is a power cut.

Related document for receiving data and sending commands from RS232 port.
https://www.falconups.com/SG%20PROTOCOL%20FALCON.pdf


![image](https://user-images.githubusercontent.com/82785819/158834470-7b54794a-73e4-481c-924c-823dd68380c9.png)

https://www.f1depo.com/urun/wemos-d1-mini-nodemcu-esp8266


![image](https://user-images.githubusercontent.com/82785819/158834589-b614445a-b160-43f8-a639-d6d99f7cadd5.png)

https://www.robitshop.com/urun/ttl-cevirici-kart-rs232


![image](https://user-images.githubusercontent.com/82785819/158834674-decd22b1-d0d6-4fc4-85a9-14d950cfdcee.png)

https://www.makelsan.com.tr/ups/urundetay/powerpack-se-serisi-1-2-3-kva/index-81.html


Home Assistant Screenshot
![image](https://user-images.githubusercontent.com/82785819/158834835-91da6223-0fa0-407b-b049-0f928ad0c43b.png)
