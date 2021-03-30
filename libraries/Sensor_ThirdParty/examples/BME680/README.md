# BME680 library<br>[![Build Status](https://travis-ci.org/SV-Zanshin/BME680.svg?branch=master)](https://travis-ci.org/SV-Zanshin/BME680) [![DOI](https://zenodo.org/badge/139349456.svg)](https://zenodo.org/badge/latestdoi/139349456) [![arduino-library-badge](https://www.ardu-badge.com/badge/BME680.svg?)](https://www.ardu-badge.com/BME680)
<img src="https://github.com/SV-Zanshin/BME680/blob/master/Images/adafruit-bme680.jpg" width="175" align="left"/><img src="https://github.com/SV-Zanshin/BME680/blob/master/Images/BlueDotBME680.jpg" width="175" align="right"/> *Arduino* library for using the [Bosch BME680](https://www.bosch-sensortec.com/bst/products/all_products/bme680) sensor which senses temperature, humidity and pressure. Since the package is quite small and difficult to breadboard with, I found the [BlueDot BME680 Breakout](https://www.bluedot.space/sensor-boards/bme680/) or one can use the [Adafruit BME680 Breakout](https://www.adafruit.com/product/3660)  for development.

A complete description of the BME680 sensor can be found in the [BME680 Datasheet](https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME680-DS001-00.pdf) while the [Adafruit BME680 tutorial](https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas) describes additional board information.

## Sensing
The BME680 allows the user to individually adjust the sampling rates for each sensor component (temperature, humidity and pressure) as well as to set the inter-measurement delay time and the [IIR](https://en.wikipedia.org/wiki/Infinite_impulse_response) filter sampling coeffient. Combining these settings allows very fine-grained control of measurement times and measurement accuracy.

### Temperature sensing
The BME280 measures temperatures between -40°C and 85°C with a full accuracy range between 0°C and +65°C. The accuracy is ±0.5°c with a full resolution of 0.01°C. Since the temperature sensor in the small package is tied to the PCB board on which it is soldered and also has some self-heating as well the temperature reading will usually be above ambient to some degree.

### Humidity sensing
Non-condensing humidity between 0% and 100% is measured with an accuracy of ±3% at a maximum resolution of 0.008%.

### Pressure sensing
The pressure sensor works in temperatures between -40°C and +85°C although the zone of full accuracy only goes from 0°C to +65°C. Pressure is measured between 300hPa and 1100hPa with an accuracy of ±1.0 hPa and a resolution of 0.2Pa

### Environment gas sensing
The gas sensor works by heating a small surface internally and measuring the resistance of the gas layer. This indicates the amount of volatile components in the air and can be used as an indicator of air quality.

A detailed library description and further details are available at the [GitHub BME680 Wiki](https://github.com/SV-Zanshin/BME680/wiki)

![Zanshin Logo](https://www.sv-zanshin.com/r/images/site/gif/zanshinkanjitiny.gif) <img src="https://www.sv-zanshin.com/r/images/site/gif/zanshintext.gif" width="75"/>
