#include <Wire.h>

#define MPU_ADDR 0b01101000              // 0x68 in Hex. This is I2C address of MPU.
#define PWR_MGMT_1 0x6B                  // To wake-up the MPU
#define INT_PIN_CFG 0b00110111          // 0x37 in Hex. This register of MPU9250 used to enable bypass mode

#define AK8963_ADDR 0b00001100          // 0x0C in Hex. This register is address of magnetometer
#define WIA_ADDR 0b00000000             // This register returns device ID: 0x48.
#define CNTL1_ADDR 0b00001010           // 0x0A in Hex.
#define ST1_ADDR 0b00000010             // 0x02 in Hex.
#define ST2_ADDR 0b00001001             // 0x09 in Hex.
#define ASAX_ADDR 0b00010000            // 0x10 in Hex.

// Modes configured by CNTL1 register
#define POWER_DOWN_MODE 0b00000000
#define ROM_ACCESS_MODE 0b00001111
#define CONTINUOUS_MEASUREMENT_1_LOW_MODE 0b00000010             // 8Hz sampling frequency with 14-bit resolution.
#define CONTINUOUS_MEASUREMENT_1_HIGH_MODE 0b00010010            // 8Hz sampling frequency with 16-bit resolution.
#define CONTINUOUS_MEASUREMENT_2_LOW_MODE 0b00000110             // 100Hz sampling frequency with 14-bit resolution.
#define CONTINUOUS_MEASUREMENT_2_HIGH_MODE 0b00010110            // 100Hz sampling frequency with 16-bit resolution.
uint8_t CHOOSEN_MEASUREMENT_MODE;

// These are the scales depending on choosen measurement mode
float SCALE_14BIT = 4912.0 / 8192.0;                               // For maximum of 4912 micro Tesla, there are 8192 counts at LSB
float SCALE_16BIT = 4912.0 / 32760.0;                              // For maximum of 4912 micro Tesla, there are 32760 counts at LSB

// These registers contain adjustment values
#define ASAX 0x10
#define ASAY 0x11
#define ASAZ 0x12
float AdjX, AdjY, AdjZ;       // They contain sensitivity adjustments

// These contain magnetometer measurements
int16_t rawX, rawY, rawZ;                            // 16-bit signed unadjusted and scaled raw measurements
float magX, magY, magZ;                              // This is scaled and adjusted measurements
float mx, my, mz;                                    // After accounting for orientation of AK8963 in MPU9250 these are true readings in IMU axes.

void setup() {

  Serial.begin(230400);
  delay(1500);
  Serial.println("BOOT");

  Wire.begin();
  Serial.println("wire ok");
  Wire.setClock(400000);
  
  wakeUpMPU();
  Serial.println("woke");

  enableBypassModeMPU();
  Serial.println("bypass set");

  // Detect the magnetometer
  DetectMagnetometer();

  // Obtain measurement adjustments. Adjustments are multiplied later with raw measurements
  uint8_t* Adj = ObtainValueAdjustments();
  AdjX = ((Adj[0] - 128) * 0.5 / 128) + 1; 
  AdjY = ((Adj[1] - 128) * 0.5 / 128) + 1;
  AdjZ = ((Adj[2] - 128) * 0.5 / 128) + 1;

  // Final Mode change: Change mode to continuous measurements
  CHOOSEN_MEASUREMENT_MODE = CONTINUOUS_MEASUREMENT_2_HIGH_MODE;
  changeMode(CHOOSEN_MEASUREMENT_MODE);            // For 100Hz sampling frequency with 16-bit resolution

  Serial.println("!!!MAGNETOMETER INITIALIZED!!!");
}

void loop() {

  if (readRegister(AK8963_ADDR, ST1_ADDR) & 0x01)            // Means if data is ready
  {
    int16_t* raw = readRawMeasurements();
    if(raw == nullptr) return;
    rawX = raw[0]; rawY = raw[1]; rawZ = raw[2];

    // Check magnetic overflow
    uint8_t ST2 = readRegister(AK8963_ADDR, ST2_ADDR);
    if(!(ST2 & 0x08))                                                               // If overflow didn't occur
    {
      setAdjScaledMeasurements();                                   // Will set values for magX, magY, and magZ in required micro-Tesla units

      // Get readings in true axes
      mx = magY;
      my = magX;
      mz = -magZ;

      // Printing
      Serial.print(mx); Serial.print(",");
      Serial.print(my); Serial.print(",");
      Serial.println(mz);
    }
  }
}
