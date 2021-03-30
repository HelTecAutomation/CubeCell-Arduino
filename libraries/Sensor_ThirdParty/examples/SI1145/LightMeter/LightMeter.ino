/***************************************************
  This is a reading lux demo for the Si1145 UV/IR/Visible Light Sensor

  Designed specifically to work with the Si1145 sensor in the
  adafruit shop
  ----> https://www.adafruit.com/products/1777

  These sensors use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Oleg Moiseenko for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

  Adapted to CubeCell HTCC-AB01 by Mike Cochrane
  December 28, 2019

  Hardware Connections:

   CubeCell Pin  SI1145 Board  Function

   Vext          VIN              Power
   GND           GND              Ground
   SDA           SDA              I2C Data
   SCL           SCL              I2C Clock

 ****************************************************/

#include <Wire.h>
#include "Adafruit_SI1145.h"

#define SF(x) (String(x))

Adafruit_SI1145 si = Adafruit_SI1145();

uint8_t Reset()
{
    si.getLastError();
    uint8_t err = si.reset();

    return err;
}

bool SensorInit()
{
    if (!si.begin(false))
    {
        Serial.println(SF("Si1145 sensor init error. last error:") + String(si.getLastError()));
        return false;
    };

    si.getLastError();
    uint8_t siID = si.readPartId();
    SI114X_CAL_S *siCalibParam = si.getCalibrationParameters();
    si.setMeassureChannels(SI1145_PARAM_CHLIST_ENUV | SI1145_PARAM_CHLIST_ENALSIR | SI1145_PARAM_CHLIST_ENALSVIS |
                           SI1145_PARAM_CHLIST_ENPS2 | SI1145_PARAM_CHLIST_ENPS3);
    uint8_t err = si.getLastError();

    if (!err && siID != 0x00 && siID != 0xFF)
    {
        // sensor online
        String TextIDs = SF("Si1145: partID=0x") + String(siID, HEX) +   // ID: 0x45 Si1145, 0x46 Si1146, 0x47 Si1147
                         SF(" revID=0x") + String(si.readRevId(), HEX) + // 0x00
                         SF(" seqID=0x") + String(si.readSeqId(), HEX);  // 0x08

        Serial.println(TextIDs);

        if (siCalibParam)
        {
            Serial.println(SF("Calibration parameters:") +
                           SF(" adcrange=") + String(FX20_TO_FLT(siCalibParam->adcrange_ratio)) +
                           SF(" vispd=") + String(FX20_TO_FLT(siCalibParam->vispd_correction)) +
                           SF(" irpd=") + String(FX20_TO_FLT(siCalibParam->irpd_correction)) +
                           SF(" irsize=") + String(FX20_TO_FLT(siCalibParam->irsize_ratio)) +
                           SF(" ledi=") + String(FX20_TO_FLT(siCalibParam->ledi_ratio)));
            if (siCalibParam->ucoef_p)
                Serial.println(
                    SF("ucoef_p[0]=") + String(siCalibParam->ucoef_p[0], 16) +
                    SF(" [1]=") + String(siCalibParam->ucoef_p[1], 16) +
                    SF(" [2]=") + String(siCalibParam->ucoef_p[2], 16) +
                    SF(" [3]=") + String(siCalibParam->ucoef_p[3], 16));
        }
        else
        {
            Serial.println(SF("Calibration parameters is empty! Used default calibration values."));
        }

        return true;
    }
    else
    {
        Serial.println(SF("Si1145 sensor offline. error:") + String(err) + SF(" id:0x") + String(siID, HEX));
        return false;
    }
}

void setup()
{
    // Vext ON
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);
    delay(10);

    // Initialize Serial port
    Serial.begin(115200);
    Serial.println("Adafruit SI1145 lux reading demo.");

    Wire.begin();
    if (!SensorInit()) {
        Serial.println("Sensor init ERROR!");
    } else {
        Serial.println("OK!");
    }
}

bool ExecMeasurementCycle(uint16_t *gainVis, uint16_t *gainIR, double *uv)
{
    *gainVis = 0x8000;
    *gainIR = 0x8000;
    *uv = 0;
    si.getLastError();

    si.setVisibleGain(*gainVis);
    si.setIRGain(*gainIR);
    uint8_t merr = si.takeForcedMeasurement();
    if (merr || si.getLastError())
    {
        Serial.println(SF("SI1145 error get data for gain calculation. err:") + String(merr, HEX));
        return false;
    }
    else
    {
        int gvis = si.readVisible() - si.getADCOffset();
        int gir = si.readIR() - si.getADCOffset();
        *gainVis = si.calcOptimalGainFromSignal(gvis);
        *gainIR = si.calcOptimalGainFromSignal(gir);
        Serial.println(SF("vis=") + String(gvis) + SF(" ir=") + String(gir) +
                       SF(" gain Vis=0x") + String(*gainVis, HEX) + SF(" Ir=0x") + String(*gainIR, HEX));
    }

    // from datasheet:
    // To enable UV reading, set the EN_UV bit in CHLIST, and configure UCOEF [0:3] to the default values of 0x7B,
    // 0x6B, 0x01, and 0x00. Also, set the VIS_RANGE and IR_RANGE bits
    *uv = si.readUV() / 100.; // the index is multiplied by 100

    uint8_t err;
    si.getLastError();

    // cycle while not error
    bool repeat = true;
    while (repeat)
    {
        repeat = false;
        si.setVisibleGain(*gainVis);
        si.setIRGain(*gainIR);
        uint8_t meserr = si.takeForcedMeasurement();
        if (meserr)
            Serial.println(SF("SI1145 take measurement error:0x") + String(meserr, HEX));

        err = si.getLastError();
        if (err)
        {
            Serial.println(SF("SI1145 error get data for gain calculation. err:") + String(err, HEX));
            return false;
        }

        switch (meserr)
        {
        case 0x80: // Invalid command
        case 0x88: // PS1 overflow
        case 0x89: // PS2 overflow
        case 0x8A: // PS3 overflow
            break;
        case 0x8C: // VIS overflow
            *gainVis = si.decGain(*gainVis);
            if (*gainVis == 0xFFFF)
            {
                *gainVis = 0x8000;
            }
            else
            {
                repeat = true;
                Serial.println(SF("SI1145 Vis measurement repeat. new gain:0x") + String(*gainVis));
            }
            break;
        case 0x8D: // IR overflow
            *gainIR = si.decGain(*gainIR);
            if (*gainIR == 0xFFFF)
            {
                *gainIR = 0x8000;
            }
            else
            {
                repeat = true;
                Serial.println(SF("SI1145 IR measurement repeat. new gain:0x") + String(*gainIR));
            }
            break;
        case 0x8E: // AUX overflow
            break;
        default:
            break;
        }
    }

    return true;
}

void loop()
{
    Serial.println("===================");

    uint16_t gainVis = 0x8000;
    uint16_t gainIR = 0x8000;
    double uv = 0; // calc UV in ExecMeasurementCycle() according to the datasheet

    // if we can do measurement (dont have i2c error)
    if (ExecMeasurementCycle(&gainVis, &gainIR, &uv))
    {
        double visible = si.readVisible();
        double ir = si.readIR();
        double ref = si.readPS2();
        double temp = si.readPS3();
        uint8_t err = si.getLastError();

        if (!err)
        {
            Serial.println(SF("Got from ADC: Visible=") + String(visible) + SF(" IR=") + String(ir) + SF(" UVindx=") + String(uv) +
                           SF(" Temp=") + String(temp) + SF(" ref=") + String(ref));
            ir = ir - si.getADCOffset();
            if (ir < 0)
                ir = 0;

            visible = visible - si.getADCOffset();
            if (visible < 0)
                visible = 0;

            float lux = (5.41f * visible) / si.calcGain(gainVis) + (-0.08f * ir) / si.calcGain(gainIR);
            if (lux < 0)
                lux = 0.0;
            float luxir = ir / (si.calcGain(gainIR) * 2.44f);
            if (lux < 0)
                lux = 0.0;

            Serial.println(SF("Real: Visible=") + String(visible / si.calcGain(gainVis)) + SF(" IR=") + String(ir / si.calcGain(gainIR)) +
                           SF(" lux=") + String(lux) + SF(" luxir=") + String(luxir));
        }
        else
        {
            Serial.println("Si1145 I2C error: " + String(err));
        }
    }
    else
    {
        Serial.println("Si1145 I2C error.");
    }

    delay(2000);
}