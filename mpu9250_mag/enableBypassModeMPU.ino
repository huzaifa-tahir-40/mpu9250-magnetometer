// AK8963 magnetometer is connected internally to the MPU9250's auxiliary I²C bus.
// The MPU9250 acts as an I²C master for its internal auxiliary bus. 
// By default, the external I²C interface does not directly expose the AK8963 to the microcontroller.
// By enabling bypass mode, you allow your microcontroller to communicate directly with the AK8963 through the MPU9250.

void enableBypassModeMPU()
{
  // Enable 2nd bit of INT_PIN_CFG register.
  writeRegister(MPU_ADDR, INT_PIN_CFG, 0b00000010);
  delay(10);
}