int16_t* readRawMeasurements()
{
    static int16_t raw[3];

    Wire.beginTransmission(AK8963_ADDR);
    Wire.write(0x03);                       // Set pointer to HXL (0x03)
    Wire.endTransmission(false);            // Repeated start

    Wire.requestFrom((uint8_t)AK8963_ADDR, (uint8_t)6);    // Request 6 bytes (0x03–0x08)

    uint32_t t0 = millis();
    while (Wire.available() < 6)
    {
      if (millis() - t0 > 10) return nullptr;   // give up if read fails
    }

    // Read X-axis measurement (Little Endian)
    uint8_t low  = Wire.read();
    uint8_t high = Wire.read();
    raw[0] = (int16_t)((high << 8) | low);

    // Read Y-axis measurement
    low  = Wire.read();
    high = Wire.read();
    raw[1] = (int16_t)((high << 8) | low);

    // Read Z-axis measurement
    low  = Wire.read();
    high = Wire.read();
    raw[2] = (int16_t)((high << 8) | low);

    return raw;
}