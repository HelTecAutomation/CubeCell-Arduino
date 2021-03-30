# Heltec Cubecell Series Arduino Development Environment

English | [简体中文](#简体中文) &nbsp;&nbsp; [![Build Status](https://travis-ci.org/HelTecAutomation/ASR650x-Arduino.png?branch=master)](https://travis-ci.org/HelTecAutomation/ASR650x-Arduino "Build Status")

This project makes ASR650x (ASR6501, ASR6502) and ASR6601 (ASR6601SE, ASR6601CB) series fully Arduino-Compatible. For [Heltec](https://heltec.org/) [CubeCell](https://heltec.org/proudct_center/lora/cubecell/) series.

Based on the ASR6x0x series of SiP, we have done a lot of migration and development, made it perfectly support **Arduino<sup>®</sup>**, can run the LoRaWAN-Node protocol stably, and can easily connect lithium batteries and solar panels… This is the CubeCell series.

Cube — Means modular design and easy assembly;

Cell —  Is the product positioning, as a unit of the IoT series, working like a cell.

CubeCell series LoRaWAN-Node stack is migration form [LoRaMac-node](https://github.com/Lora-net/LoRaMac-node) V4.4.0, based on "LoRaWAN specification 1.0.2" and "LoRaWAN Regional Parameters v1.0.2rB".

&nbsp;

For more details, please refer to **[CubeCell Documents Page](https://heltec-automation-docs.readthedocs.io/en/latest/cubecell/index.html).**

&nbsp;

## Instructions

The currently supported ASR650x chip product models are:

|    MCU    |                       Relative boards                        |
| :-------: | :----------------------------------------------------------: |
|  ASR6501  | [CubeCell-Board](https://heltec.org/project/htcc-ab01/), [CubeCell-Module](https://heltec.org/project/htcc-am01/), [CubeCell-Capsule](https://heltec.org/project/htcc-ac01/) |
|  ASR6502  | [CubeCell-Board Plus](https://heltec.org/project/htcc-ab02/), [CubeCell GPS-6502](https://heltec.org/project/htcc-ab02s/), [CubeCell 1/2AA Node](https://heltec.org/project/htcc-ab02a/), [CubeCell-Module Plus](https://heltec.org/project/htcc-am02/), [CubeCell-Capsule Solar Sensor](https://heltec.org/project/htcc-ac02/) |
| ASR6601SE |                                                              |
| ASR6601CB |                                                              |

## Major update in 3-30-2021

- Convert ASR650x series to a new branch;
- Add ASR6601 series support and makes it as master branch;
- Added and fixed some function to Arduino format, such as:
  - `int analogRead(pin)` -- return adc level 0 ~ 4096;
  - `float analogReadmV(pin)` -- return adc voltage in mV;
    - *The ASR650x series ADC input pin range is 0 ~ 2400 mV*;
    - *The ASR6601 series ADC input pin range is 100 ~ 1100 mV.*
  - `SPI.begin(sck,miso,mosi,ss)` -- SPI begin;
  - `wire.begin(sda, scl, frequency)` -- I2C begin;
  - `Serial.begin(baud, config, rxPin, txPin)` -- UART begin.

&nbsp;

## Installation Instructions

Make sure you have the latest Arduino IDE installed on your computer. If not installed, please refer to this documents:

- [How to Install Git and Arduino](https://heltec-automation-docs.readthedocs.io/en/latest/general/how_to_install_git_and_arduino.html)
- [Establish Serial Connection](https://heltec-automation-docs.readthedocs.io/en/latest/general/establish_serial_connection.html)

&nbsp;

- **Via Arduino IDE Boards Manager (Recommend)**
  - [Install ASR650x-Arduino(CubeCell) via Arduino Boards Manager](https://heltec-automation-docs.readthedocs.io/en/latest/cubecell/quick_start.html)
  
- **Use Git with the development repository**
  + [Instructions for Windows](InstallGuide/windows.md)
  + [Instructions for Mac](InstallGuide/mac.md)
  + [Instructions for Debian/Ubuntu Linux](InstallGuide/debian_ubuntu.md)

&nbsp;

Linux may have serial port can't link problem, please refer to this page:[https://playground.arduino.cc/Linux/All/#Permission](https://playground.arduino.cc/Linux/All/#Permission)

&nbsp;

## Issue/Bug report template
Before reporting an issue, make sure you've searched for similar one that was already created. Also make sure to go through all the issues labelled as [for reference](https://github.com/HelTecAutomation/ASR650x-Arduino/issues).

Also you can talk in our forum: [http://community.heltec.cn/](http://community.heltec.cn/)

&nbsp;

## Contact us
- **Website：[https://heltec.org](https://heltec.org/)**
- **Document Page: [https://docs.heltec.cn](https://docs.heltec.cn)**
- **Forum: [http://community.heltec.cn/](http://community.heltec.cn/)**
- **Twitter: [https://twitter.com/HeltecOrg](https://twitter.com/HeltecOrg)**
- **Face Book: [https://www.facebook.com/heltec.automation.5](https://www.facebook.com/heltec.automation.5)**

&nbsp;
## 简体中文

本项目由成都惠利特自动化科技有限公司开发。

* 本项目旨在为ASR650x、ASR6601系列芯片提供Arduino开发环境与例程库，方便用户完成对ASR650x、ASR6601系列芯片的快速部署与开发。


## 说明
产品型号列表：

|    MCU    |                       Relative boards                        |
| :-------: | :----------------------------------------------------------: |
|  ASR6501  | [CubeCell-Board](https://heltec.org/project/htcc-ab01/), [CubeCell-Module](https://heltec.org/project/htcc-am01/), [CubeCell-Cuapsule](https://heltec.org/project/htcc-ac01/) |
|  ASR6502  | [CubeCell-Board Plus](https://heltec.org/project/htcc-ab02/), [CubeCell GPS-6502](https://heltec.org/project/htcc-ab02s/), [CubeCell 1/2AA Node](https://heltec.org/project/htcc-ab02a/), [CubeCell-Module Plus](https://heltec.org/project/htcc-am02/), [CubeCell-Capsule Solar Sensor](https://heltec.org/project/htcc-ac02/) |
| ASR6601SE |                                                              |
| ASR6601CB |                                                              |

## 安装指南

首先，确保你的电脑上已经安装了最新的Arduino IDE。如果没有安装，请参考这篇文档：[https://docs.heltec.cn/#/zh_CN/user_manual/how_to_install_git_and_arduino](https://docs.heltec.cn/#/zh_CN/user_manual/how_to_install_git_and_arduino)
- **通过Arduino IDE的开发板理器安装**
  - [通过开发板管理器安装ASR650x芯片的Arduino开发环境](https://heltec-automation.readthedocs.io/zh_CN/latest/cubecell/quick_start.html#arduino-board-manager)
- **通过Git从源码进行安装**
  - [Windows操作系统 -- 安装方法](InstallGuide/windows.md)
  - [MacOS操作系统 -- 安装方法](InstallGuide/mac.md)
  - [Linux操作系统(debian,ubuntu) -- 安装方法](InstallGuide/debian_ubuntu.md)

&nbsp;

## 问题讨论 & BUG报告

如果串口不能连接,请参考：[https://playground.arduino.cc/Linux/All/#Permission](https://playground.arduino.cc/Linux/All/#Permission)

在报告BUG之前，请先做详细的测试，如果问题真的存在，您可以通过以下方式报告或者讨论：

- [GitHub问题报告页](https://github.com/HelTecAutomation/ASR650x-Arduino/issues)
- [Heltec论坛 (仅英语交流)](http://community.heltec.cn/)

## 联系我们
- **官网：[https://heltec.org](https://heltec.org/)**
- **Heltec文档页: [https://docs.heltec.cn](https://docs.heltec.cn)**
- **Heltec论坛 (仅英语): [http://community.heltec.cn/](http://community.heltec.cn/)**
- **QQ群: 799093974(中文)**
