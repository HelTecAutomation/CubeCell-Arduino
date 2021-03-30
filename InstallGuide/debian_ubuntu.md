Installation instructions for Debian / Ubuntu OS
=================================================

- Install latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/Main/Software)

- Open Terminal and execute the following command (copy->paste and hit enter):

  ```bash
  sudo usermod -a -G dialout $USER && \
  sudo apt-get install git && \
  mkdir -p ~/Arduino/hardware/CubeCell && \
  cd ~/Arduino/hardware/CubeCell && \
  git clone https://github.com/HelTecAutomation/ASR650x-Arduino.git CubeCell && \
  cd CubeCell/tools && \
  python get.py 
  ```
  
- Restart Arduino IDE

- If program can't be upload, and show message:


 ```bash
 PermissionError: [Errno 13] Permission denied: '/dev/ttyUSB0'
 ```

- the page http://playground.arduino.cc/Linux/All#Permission may be helpful.

