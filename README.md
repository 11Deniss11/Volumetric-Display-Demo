# DIY Volumetric Display

This is a platformio project for an ESP32-S3 to make a Volumetric Display.

![cubeImage](https://github.com/user-attachments/assets/a238da17-0f1f-4190-975c-de3b39407505)


## Display Requirements:

1. 16x16 WS2812B LED matrix which is wired in a snake pattern (most common on online stores)
2. External driving setup able to spin the LED matrix close to or above 500 rpm
3. A way to create a pulse at the start of each new revolution (magnet with reed switch recommended)
4. Constant power to the processor and uninterrupted connection to the LED matrix

## 3D Print Files and Components for My Design:

3D print files available for free on my [Patreon](https://www.patreon.com/posts/volumetric-3d-157857025?utm_medium=clipboard_copy&utm_source=copyLink&utm_campaign=postshare_creator&utm_content=join_link)

### Components Used:
- 16x16 WS2812B LED matrix
- ESP32 S3 Wroom 1 N16R8V
- Reed switch (requires magnet)
- **2x** 6-channel slip rings
- 25 watt, 5 volt power supply
- 1000kv brushless motor
- 40 amp electronic speed controller
- 3 cell lipo battery (or a 12 volt power supply capable of 10 amps or more but this is untested)
- Servo tester to create the PWM signal for the electronic speed controller (controls the speed)
