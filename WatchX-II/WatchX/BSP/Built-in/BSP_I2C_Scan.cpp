#include "BSP/BSP.h"

#if (WX_USE_BUILT_IN_I2C != 0)

#include "Wire.h"

void I2C_Scan(bool startScan)
{
    Wire.begin();
    
    if(!startScan)
        return;
    
    uint8_t error, address;
    int nDevices;
    
    Serial.println("I2C device scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++ )
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println(" !");

            nDevices++;
        }
        else if (error == 4)
        {
            Serial.print("Unknow error at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }

    Serial.printf("%d I2C devices was found\r\n", nDevices);
}

#endif
