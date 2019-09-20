Installation instructions for Debian / Ubuntu OS
=================================================

- Install latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/Main/Software)
- Open Terminal and execute the following command (copy->paste and hit enter):

  ```bash
  sudo usermod -a -G dialout $USER && \
  sudo apt-get install git && \
  wget https://bootstrap.pypa.io/get-pip.py && \
  sudo python get-pip.py && \
  sudo pip install pyserial && \
  mkdir -p ~/Arduino/hardware/CubeCell && \
  cd ~/Arduino/hardware/CubeCell && \
  git clone https://github.com/HelTecAutomation/ASR650x-Arduino.git && \
  cd ASR650x-Arduino && \
  git submodule update --init --recursive && \
  cd tools && \
  python get.py 
  mv gcc-arm-none-eabi-8-2019-q3-update gcc-arm-none-eabi
  sudo chown root:root gcc-arm-none-eabi/bin/arm-none-eabi-g++
  sudo chmod u+s gcc-arm-none-eabi/bin/arm-none-eabi-g++
  ```
- Restart Arduino IDE



- If you have Arduino.app installed to /Applications/, modify the installation as follows, beginning at `mkdir -p ~/Arduino...`:

```bash
  cd /Applications/Arduino_*/Contents/java/
  mkdir -p hardware/CubeCell && \
  cd hardware/CubeCell && \
  git clone https://github.com/HelTecAutomation/ASR650x-Arduino.git && \
  cd ASR650x-Arduino && \
  git submodule update --init --recursive && \
  cd tools && \
  python get.py
  mv gcc-arm-none-eabi-8-2019-q3-update gcc-arm-none-eabi
  sudo chown root:root gcc-arm-none-eabi/bin/arm-none-eabi-g++
  sudo chmod u+s gcc-arm-none-eabi/bin/arm-none-eabi-g++
```
