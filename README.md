# Heltec ASR650x Series Arduino Develop Environment

English | [简体中文](#简体中文)

This project makes ASR650x (ASR6501, ASR6502) series fully support Arduino®. For [Heltec](https://heltec.org/) [CubeCell](https://heltec.org/proudct_center/lora/cubecell/) series.

Based on the ASR650x series of chips, we have done a lot of migration and development, made it perfectly support **Arduino®**, can run the LoRaWAN protocol stably, and can easily connect lithium batteries and solar panels… This is the CubeCell series.

Cube — Means modular design and easy assembly;

Cell —  Is the product positioning, as a unit of the IoT series, working like a cell.

&nbsp;

## Instructions

The currently supported ASR650x chip product models are:

|   MCU   |                       Relative boards                        |
| :-----: | :----------------------------------------------------------: |
| ASR6501 | [CubeCell-Board](https://heltec.org/project/htcc-ab01/), [CubeCell-Module](https://heltec.org/project/htcc-am01/), [CubeCell-Cupsule](https://heltec.org/project/htcc-ac01/) |

&nbsp;

## Installation Instructions

*For Mac OS® system, we recommend installing Arduino version 1.8.5. Arduino over 1.8.5 version may cause unknown bugs.*

Make sure you have the latest Arduino IDE installed on your computer. If not installed, please refer to this documents:

- [How to Install Git and Arduino](https://docs.heltec.cn/#/en/user_manual/how_to_install_git_and_arduino)
- [Establish Serial Connection](https://docs.heltec.cn/#/en/user_manual/establish_serial_connection)

&nbsp;

- **Via Arduino IDE Boards Manager (Recommend)**
  - [Install ASR650x-Arduino via Aruino Boards Manager](https://docs.heltec.cn/#/en/user_manual/how_to_install_ASR650x_Arduino)
  
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

本项目由Heltec Automation 惠利特自动化科技有限公司公司提供。

* 本项目旨在为ASR650x系列芯片提供Arduino便利的开发环境与例程库，方便用户完成对ASR650x系列芯片的快速部署与开发。


## 说明
目前所支持的ASR650x芯片产品型号有：

|   MCU   |                       Relative boards                        |
| :-----: | :----------------------------------------------------------: |
| ASR6501 | [CubeCell-Board](https://heltec.org/project/htcc-ab01/), [CubeCell-Module](https://heltec.org/project/htcc-am01/), [CubeCell-Cupsule](https://heltec.org/project/htcc-ac01/) |



## 安装指南

首先，确保你的电脑上已经安装了最新的Arduino IDE。如果没有安装，请参考这篇文档：[https://docs.heltec.cn/#/zh_CN/user_manual/how_to_install_git_and_arduino](https://docs.heltec.cn/#/zh_CN/user_manual/how_to_install_git_and_arduino)
我们建议安装Arduino 1.8.5版本，超过1.8.5版本的Arduino可能会导致未知bug。
- **通过Arduino IDE的库管理器安装**
  - [安装基于ASR650x芯片的开发环境](https://docs.heltec.cn/#/en/user_manual/how_to_install_ASR650x_Arduino)
- **通过Git从源码进行安装**
  - [Windows操作系统 -- 安装方法](InstallGuide/windows.md)
  - [MacOS操作系统 -- 安装方法](InstallGuide/mac.md)
  - [Linux操作系统(debian,ubuntu) -- 安装方法](InstallGuide/debian_ubuntu.md)


- 更多安装方法，还可以参考这里：[https://heltec.org/zh/asr650x_install/](xxx)

## 编码规则
可以参考这篇文章来了解Arduino的插件和编码规则：[EspExceptionDecoder](xxx).

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