# Efekta THP_LR \ THP

Поблагодарить автора: https://www.donationalerts.com/r/efektalab

Thank the author: http://www.donationalerts.ru/r/EfektaLab

Телеграм чат DIY Devices - https://t.me/diy_devices

Продажа DIY Устройств - https://t.me/diydevmart

##### Option 1 - DIY outdoor/indoor long-range sensor for temperature, humidity and atmospheric pressure

##### Option 2 - DIY outdoor/indoor temperature, humidity and atmospheric pressure sensor, long battery life

### You can make your own pcb here - https://www.pcbway.com/setinvite.aspx?inviteid=550959

## V1_R1

![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/001.png) 


![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/01.jpg)


![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/0005.jpg)


![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/0006.jpg) 


![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/0007.jpg) 


![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/SCHEME/THP_THPLR.png) 


### What is it?
It is a wireless temperature, pressure and humidity sensor, supports working in Zigbee networks. Open source project. Built on CC2530 chip (Zigbee), two modications: Ebyte E18-MS1PA2-PCB radio module with amplifier and Ebyte E18-MS1-PCB. Powered by the most common AAA batteries. The device comes with pre-installed open source software. Supported in Z2M, ZHA, etc.

### What makes it special?
RF module EBYTE E18-MS1PA2-PCB (CC2530) or Ebyte E18-MS1-PCB.

BME280

Long runtime with two AAA battery.

Consumption in data transmission mode ~25mA|50mA, in sleep mode 1.8μA.


### How to flash the device

1. Download the Smart RF Flash Programmer V1 https://www.ti.com/tool/FLASH-PROGRAMMER

2. Open the application select the HEX firmware file

3. Connect the device with wires to CCDebugger, first erase the chip, then flash it.

---

### How to install IAR

https://github.com/ZigDevWiki/zigdevwiki.github.io/blob/main/docs/Begin/IAR_install.md

https://github.com/sigma7i/zigbee-wiki/wiki/zigbee-firmware-install (RU)

### Сonfiguring the firmware before compilation

Open the preinclude.h file, select the desired configuration

//#define OUTDOOR_LONG_RANGE

---

### How to join:
#### If device in FN(factory new) state:

1. Open z2m, make sure that joining is prohibited
2. Insert the battery into the device
3. Click on the icon in z2m - allow joining (you have 180 seconds to add the device)
4. Go to the LOGS tab
5. Press and hold the mode button on the device, the indicator will light up in a second. If the device has found the network and started the login procedure, the indicator will go off after 6-9 seconds, and you can release the button. If the indicator goes off after 15 seconds, it means that the sensor has not detected an open network or has been denied a network connection.

### How to leave:
#### If device in a network:
1. Hold down the mode button for 10 seconds, after 2 seconds the LED will start flashing at a frequency of once per second, after 10 seconds the LED will light up for a long time, the device will send an exit message to the network and erase all network settings in its non-volatile memory.

![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/003.png) 


By default, the device is configured to operate on a timer, data thresholds are not set. The device will send data to the network every time it wakes up. For the version with a standard radio module, the default sleep interval is set to 15 seconds. For the version with a signal amplifier, the interval is 30 seconds. In order to configure the device, you must independently set the minimum report interval other than zero and the data change threshold.
![Efekta THP_LR \ THP](https://raw.githubusercontent.com/smartboxchannel/EFEKTA-TH-THLR-Outdoor-Indoor-sensor-Zigbee/refs/heads/main/IMAGES/0055.png) 


### Troubleshooting

If a device does not connect to your coordinator, please try the following:

1. Power off all routers in your network.
2. Move the device near to your coordinator (about 1 meter).
or if you cannot disable routers (for example, internal switches), you may try the following:
2.1. Disconnect an external antenna from your coordinator.
2.2. Move a device to your coordinator closely (1-3 centimeters).
3. Power on, power on the device.
4. Restart your coordinator (for example, restart Zigbee2MQTT if you use it).

If the device has not fully passed the join

1. If the device is visible in the list of z2m devices, remove it by applying force remove
2. Restart your coordinator (for example, restart Zigbee2MQTT if you use it).
3. Click on the icon in z2m - allow joining (you have 180 seconds to add the device)
4. Go to the LOGS tab
5. Press and hold button (1) for 2-3 seconds, until device start flashing the LED repeatedly
6. Wait, in case of successfull join, device will flash led 5 times, if join failed, device will flash led 2 times



### Other checks

Please, ensure the following:

1. Your power source is OK (a battery has more than 3V). You can temporarily use an external power source for testings (for example, from a debugger).
2. The RF part of your E18 board works. You can upload another firmware to it and try to pair it with your coordinator. Or you may use another coordinator and build a separate Zigbee network for testing.
3. Your coordinator has free slots for direct connections.
4. You permit joining on your coordinator.
5. Your device did not join to other opened Zigbee network. When you press and hold the button, it should flash every 3-4 seconds. It means that the device in the joining state.
