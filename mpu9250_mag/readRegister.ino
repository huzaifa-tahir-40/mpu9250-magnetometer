uint8_t readRegister(uint8_t ADDR, uint8_t REG)
{
  Wire.beginTransmission(ADDR);
  Wire.write(REG);                // Set pointer to register to read
  Wire.endTransmission(false);    // Send register address without releasing I2C bus  

  Wire.requestFrom(ADDR, (uint8_t)1);       // request 1 byte from address

  if(Wire.available())
  {
    return Wire.read();                     // Read and return received byte
  }

  return 0;                                 // return 0 if fails
}