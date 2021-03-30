Installation instructions for Mac OS
=====================================

- Install latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/Main/Software)
- Open Terminal and execute the following command (copy->paste and hit enter one by one):

  ```bash
  mkdir -p ~/Documents/Arduino/hardware/CubeCell
  cd ~/Documents/Arduino/hardware/CubeCell
  git clone https://github.com/HelTecAutomation/ASR650x-Arduino.git CubeCell
  cd CubeCell/tools
  python get.py
  ```

- Restart Arduino IDE

