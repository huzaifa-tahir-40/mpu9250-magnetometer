void writeRegister(uint8_t ADDR, uint8_t REG, uint8_t VALUE)
{
  Wire.beginTransmission(ADDR);
  Wire.write(REG);
  Wire.write(VALUE);
  Wire.endTransmission();
}