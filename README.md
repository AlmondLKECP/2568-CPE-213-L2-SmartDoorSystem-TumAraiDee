# 🚪[Smart Door System](https://github.com/AlmondLKECP/2568-CPE-213-L2-SmartDoorSystem-TumAraiDee) 🚪
โครงการ Smart Door System เป็นระบบควบคุมประตูอัจฉริยะที่พัฒนาด้วยบอร์ด ESP32 โดยมีการทำงานร่วมกับเซนเซอร์อัลตร้าโซนิค (Ultrasonic Sensor) สำหรับตรวจจับการเข้า–ออก, มอเตอร์เซอร์โว (Servo Motor) สำหรับควบคุมการเปิด–ปิดประตู, ปุ่มกด (Push Button)สำหรับการออกจากด้านใน และจอแสดงผล OLED พร้อมไฟ LED สำหรับแสดงสถานะการทำงานของระบบ<br> <br>
การเข้าใช้งานจากด้านนอกต้องเดินผ่านเซนเซอร์อัลตร้าโซนิคก่อนจึงจะให้ใส่รหัสผ่าน ในการยืนยันด้วยรหัสผ่าน 6 หลัก หากกรอกรหัสถูกต้อง ประตูจะเปิดอัตโนมัติและปิดกลับหลังจากเวลาที่กำหนด แต่ถ้ารหัสผิดเกิน 3 ครั้ง ระบบจะเข้าสู่โหมด Timeout เพื่อป้องกันการบุกรุก ขณะที่การออกจากด้านใน ผู้ใช้สามารถกดปุ่มเพื่อเปิดประตูออกได้ ระบบนี้จึงมีทั้งความสะดวกสบายและความปลอดภัย สามารถนำไปประยุกต์ใช้ได้กับบ้านพักอาศัย หอพัก หรือสำนักงาน

## 💻Software Requirement
* Teraterm
* CP2102 USB to UART Bridge VCP Driver
* Visual Studio Code
* Extension on Visual Studio Code
  * C/C++
  * PlatformIO IDE
    * Adafruit SSD1306 by Adafruit
    * Adafruit GFX Library by Adafruit
    * ESP32Servo by Kevin Harrington

## ✅Hardware Requirement & GPIO Port
|     Pieces     | Name       |      GPIO Port   |    
|:------------:|------------------|:-----------------:|
|    1    |  ESP32 + Shield                                                                                                      |   -                          |
|    1    |  Micro Servo Motor (SG90)                                                                                            |   14                         |
|    1    |  Push Button Switch                                                                                                  |   27                         |
|    1    |  OLED SSD1306 <br> &nbsp;&nbsp;&nbsp;  SCL <br> &nbsp;&nbsp;&nbsp;  SDA                                              |   <br> 22 <br> 21            |
|    1    |  CP-2102 <br> &nbsp;&nbsp;&nbsp; TxD <br> &nbsp;&nbsp;&nbsp; RxD                                                     |   <br> 16 <br> 17            |
|    2    |   Ultrasonic HC-SR04  <br> &nbsp;&nbsp;&nbsp; Sensor 1: Trig1, Echo1 <br> &nbsp;&nbsp;&nbsp; Sensor 2: Trig2, Echo2  |   <br> 32, 33 <br> 25, 26    |
|    2    |   LED (2 colors)                                                                                                     |   18,19                      |
|    2    |  Resistor 1k Ohm                                                                                                     |   -                          |
|    4    |  Resistor 330 Ohm                                                                                                    |   -                          |

## 📊BLock Diagram and Circuit Diagram
Circuit Diagram
![circuit](https://github.com/AlmondLKECP/2568-CPE-213-L2-SmartDoorSystem-TumAraiDee/blob/ServoMotor/img/circuit_image.png)

Block Diagram
![block](https://github.com/AlmondLKECP/2568-CPE-213-L2-SmartDoorSystem-TumAraiDee/blob/ServoMotor/img/block_diagram.jpg)

Actually Circuit
![real](https://github.com/AlmondLKECP/2568-CPE-213-L2-SmartDoorSystem-TumAraiDee/blob/ServoMotor/img/real_circuit.jpg)

## 🎥VDO Demonstration
[![CPE-213 Project Smart Door System](http://img.youtube.com/vi/GOKSnvcHoEI/0.jpg)](https://youtu.be/GOKSnvcHoEI "CPE-213 Project Smart Door System")

## 👨🏻‍💻Team Members
| Name       |      Student ID   |     GitHub ID     |
|------------|------------------|:-----------------:|
| นคบดินท์ เชื้ออิน       |    2311311548   |      [AlmondLKECP](https://github.com/AlmondLKECP)  |
| พรชนก สุริวงษ์        |    2311311688   |      [HXZNS](https://github.com/HXZNS)              |
| นวพล ปิยะธนพร       |    2411312032   |      [PierrotZS](https://github.com/PierrotZS)      |

