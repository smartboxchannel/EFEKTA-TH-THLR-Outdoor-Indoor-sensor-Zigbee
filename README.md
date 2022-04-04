# Efekta THP_LR \ THP

##### Option 1 - DIY outdoor/indoor long-range sensor for temperature, humidity and atmospheric pressure

##### Option 2 - DIY outdoor/indoor temperature, humidity and atmospheric pressure sensor, long battery life

### Sale: 

#### The cost of the device without the enclosure: 

##### $19.5 (shtc3: temperature, humidity)

##### $27.5 (bme280: temperature, humidity, pressure)

##### +$3 Long range radio module

##### +$8 Enclosure

##### Delivery is $8.5, + $2 for each additional device in the order.

##### Tracknumber for tracking.

##### You can buy a ready-made device by writing to the mail hello@efektalab.com

##### Delivery is carried out worldwide.


#### Video: 

#### Telegram DiyDev - https://t.me/diy_devices

More info at http://efektalab.com/eON_THP

### You can make your own pcb here - https://www.pcbway.com/setinvite.aspx?inviteid=550959



## V1_R1

![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/001.png) 

## V1_R2

![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/002.png) 


![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/01.jpg)


![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/0005.jpg)


![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/0006.jpg) 


![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/0007.jpg) 


![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/SCHEME/THP_THPLR.png) 


### What is it?
It is a wireless temperature, pressure and humidity sensor, supports working in Zigbee networks. (Zigbee2mqtt open source project). Built on CC2530 chip (Zigbee), two modications: Ebyte E18-MS1PA2-PCB radio module with amplifier and Ebyte E18-MS1-PCB. Powered by the most common AAA batteries. The device comes with pre-installed open source software. Supported inZ2M, ZHA

### What makes it special?
RF module EBYTE E18-MS1PA2-PCB (CC2530) or Ebyte E18-MS1-PCB.

BME280 or BMP280

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

//#define BMP

https://github.com/smartboxchannel/EFEKTA-THP-THPLR-Outdoor-Indoor-sensor-Zigbee/blob/9e8450923bca76330c3ec9616b2dffed900a4576/PROJECT%20SOURCE/THP/Source/preinclude.h#L85

---

### How to join:
#### If device in FN(factory new) state:
##### one way
1. Open z2m, make sure that joining is prohibited
2. Insert the battery into the device
3. Click on the icon in z2m - allow joining (you have 180 seconds to add the device)
4. Go to the LOGS tab
5. Press the reset button on the device (the join procedure will begin, еhe device starts flashing the LED repeatedly)
6. Wait, in case of successfull join, device will flash led 5 times, if join failed, device will flash led 2 times

##### another way
1. Open z2m, make sure that joining is prohibited
2. Insert the battery into the device
3. Click on the icon in z2m - allow joining (you have 180 seconds to add the device)
4. Go to the LOGS tab
5. Press and hold button (1) for 2-3 seconds, until device start flashing the LED repeatedly
6. Wait, in case of successfull join, device will flash led 5 times, if join failed, device will flash led 2 times


#### If device in a network:
##### one way 
1. Hold button (1) for 10 seconds, this will reset device to FN(factory new) status 
2. Click on the icon in z2m - allow joining (you have 180 seconds to add the device)
3. Go to the LOGS tab
5. Press and hold button (1) for 2-3 seconds, until device start flashing the LED repeatedly
6. Wait, in case of successfull join, device will flash led 5 times, if join failed, device will flash led 2 times

##### another way
1.Find the device in the list of z2m devices and delete it by applying force remove
2. Click on the icon in z2m - allow joining (you have 180 seconds to add the device)
3. Go to the LOGS tab
4. Press the reset button on the device (the join procedure will begin, еhe device starts flashing the LED repeatedly)
5. Wait, in case of successfull join, device will flash led 5 times, if join failed, device will flash led 2 times

![Efekta THP_LR \ THP](https://github.com/smartboxchannel/Outdoor-long-range-sensor-for-temperature-humidity-and-atmospheric-pressure-Zigbee/blob/main/IMAGES/003.png) 

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
