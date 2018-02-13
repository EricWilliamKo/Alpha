# Platformio_rugby

## Target ID list
   * 328p_left  (U19 on RB, uart thru mega_wheel U9)
   * 328p_right (U16 on RB, uart thru mega_wheel U9)
   * 328p_imu   (U11 on RB, uart thru mega_wheel U9)   
   * 328p_pan   (U25 on RB, uart thru mega_head U12)
   * 328p_tilt  (U22 on RB, uart thru mega_head U12)
   * 328p_avoid (U03 on RB, uart thru ftdi U4)
   * 328p_dock(U01 on RB, uart thru ftdi U2) 
   * mega_head  (U12 on RB, uart thru ftdi U13)
   * mega_wheel (U09 on RB, uart thru ftdi U10)

## Connector Pins 

Left (M1) right (M2)|Pins| Pan (M3) |Pins| (S1) |Pins| (S2) |Pins| Tilt (M4) |Pins| (S3) |Pins| (S4) |Pins|       
:------------------:|:--:| :-------:|:--:|:----:|:--:|:----:|:--:|:---------:|:--:|:----:|:--:|:----:|:--:|
 OUT_A              | 6  |  OUT_A   | 6  |  --  | -- |  --  | -- |  OUT_A    | 6  |  --  | -- |  --  | -- |
 OUT_B              | 5  |  OUT_B   | 5  |  --  | -- |  --  | -- |  OUT_B    | 5  |  --  | -- |  --  | -- |
 GND                | 4  |  GND     | 4  |  --  | -- |  --  | -- |  GND      | 4  |  --  | -- |  --  | -- |
 5V                 | 3  |  5V      | 3  |  5V  | 3  |  5V  | 3  |  5V       | 3  |  5V  | 3  |  5V  | 3  |
 HALL_A             | 2  |  HALL_A  | 2  |  LA  | 2  |  LB  | 2  |  HALL_A   | 2  |  LA  | 2  |  LB  | 2  |
 HALL_B             | 1  |  HALL_B  | 1  |  GND | 1  |  GND | 1  |  HALL_B   | 1  |  GND | 1  |  GND | 1  |

328p_dock (J1)|Pins| 328p_avoid (J3) |Pins| (J4) |Pins| (J5) |Pins| (J6) |Pins|        
:------------:|:--:|:---------------:|:--:|:----:|:--:|:----:|:--:|:----:|:--:|
  --          | -- |        --       | -- |  --  | -- | --   | -- | 5V   | 7  |
  --          | -- |        --       | -- |  --  | -- | 5V   | 6  | GND  | 6  |
  --          | -- |        --       | -- |  --  | -- | Gnd  | 5  | D10  | 5  |
  5V          | 4  |        5V       | 4  |  5V  | 4  | D6   | 4  | D11  | 4  |
  Gnd         | 3  |        Gnd      | 3  |  Gnd | 3  | D7   | 3  | D12  | 3  |
  D5          | 2  |        D2       | 2  |  D4  | 2  | D8   | 2  | D13  | 2  |
  D6          | 1  |        D3       | 1  |  D5  | 1  | D9   | 1  | RESET| 1  | 
  
328p_program (J8) | Pins | 328p_IMU (J9) | Pins | mega_head (J10) | Pins | (J11) | Pins |      
:----------------:|:----:|:-------------:|:----:|:---------------:|:----:|:-----:|:----:|
  GND             |  6   |        --     |  --  |  --             |  --  | --    |  --  |
  RESET           |  5   |        5V     |  5   |  5V             |  5   | SCL   |  5   |
  MOSI            |  4   |        --     |  4   |  Gnd            |  4   | SDA   |  4   |
  SCK             |  3   |        Gnd    |  3   |  SA0            |  3   | --    |  3   |
  5V              |  2   |        SCL    |  2   |  SA1            |  2   | GND   |  2   | 
  MISO            |  1   |        SDA    |  1   |  SD2            |  1   | GND   |  1   |

## Upload bootloader for programmer with Arduino Uno
    $ cd bootloader
    $ sudo pio run -t uploadboot

## Checking programmer fuse
    $ avrdude -P /dev/ttyUSB5 -b 57600 -c arduino -p m328p -v

## Setting fuse for all target devices
    $ sudo python fuse-set.py

## Rugby_board sanity test 
    $ sudo python sanity-blink.py 
    - LED should blink every 500ms
    $ sudo python sanity-blink-fast.py 
    - LED should blink every 100ms
    - we should see all LEDs blinking, if not, check if target fuse set correctly
    $ sudo python sanity-uart.py

## Checking target fuse 
    $ sudo platformio run -d ArduinoISP/328p_dock --target upload
    - replace 328p_dock when checking other targets
    $ avrdude -P /dev/ttyUSB5 -b 19200 -c avrisp -p m328p -v
    - fuse for 328p: efuse=0x05 hfuse=0xDA lfuse0xFF
    $ avrdude -P /dev/ttyUSB5 -b 19200 -c avrisp -p m2560 -v
    - fuse for 2560: efuse=0xFD hfuse=0xD8 lfuse0xFF

## Upload code for individual target (1st arg is the target, 2nd arg is the code to upload)
    $ sudo python upload.py 328p_dock rugby-blink/rugby-328-blink 
    $ sudo python upload.py mega_wheel mcu_control/base_control/mega_base_pyserial
    $ sudo python upload.py mega_head mcu_control/rugby_head/rugby_head_ctrl
    $ sudo python upload.py 328p_pan mcu_control/rugby_head/rugby_head_joint_pan
    $ sudo python upload.py 328p_left mcu_control/base_control/Rugby_wheel
    $ sudo python upload.py 328p_dock mcu_control/autodocking
    $ sudo python upload.py 328p_imu mcu_control/base_control/GY-85-yaw

## Reference 
  http://docs.platformio.org/en/latest/installation.html
  
  http://docs.platformio.org/en/latest/platforms/atmelavr.html#custom-fuses

  http://heliosoph.mit-links.info/arduinoisp-reading-writing-fuses-atmega328p/
  
  http://www.martyncurrey.com/arduino-atmega-328p-fuse-settings/
  
  http://www.codingwithcody.com/2011/06/25/arduino-default-fuse-settings/
