#include "BSP.h"
#include "MAX3010x/src/MAX30105.h"
#include "MAX3010x/src/heartRate.h"

typedef struct{
    int16_t BeatsPerMinute;
    int16_t BeatAvg;
}DataPack_TypeDef;

static DataPack_TypeDef DataPack;

ComSlave_MakeNode(ParticleSensor, DataPack);

static MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

void ParticleSensor_Init()
{
    ComSlave_InitNode(ParticleSensor, 0x03);
    
    Serial.println("Initializing...");

    // Initialize sensor
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
    {
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }
    Serial.println("Place your index finger on the sensor with steady pressure.");

    particleSensor.setup(); //Configure sensor with default settings
    particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
    particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

void ParticleSensor_Update()
{
    long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true)
    {
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20)
        {
            rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
            rateSpot %= RATE_SIZE; //Wrap variable

            //Take average of readings
            beatAvg = 0;
            for (byte x = 0 ; x < RATE_SIZE ; x++)
                beatAvg += rates[x];
            beatAvg /= RATE_SIZE;
        }
    }
    
    DataPack.BeatsPerMinute = beatsPerMinute * 10;
    DataPack.BeatAvg = beatAvg * 10;
    ComSlave_ReqExecuteUserCallbackNode(&ParticleSensor_Node);

//    Serial.print("IR=");
//    Serial.print(irValue);
//    Serial.print(", BPM=");
//    Serial.print(beatsPerMinute);
//    Serial.print(", Avg BPM=");
//    Serial.print(beatAvg);

//    if (irValue < 50000)
//        Serial.print(" No finger?");

//    Serial.println();
}
