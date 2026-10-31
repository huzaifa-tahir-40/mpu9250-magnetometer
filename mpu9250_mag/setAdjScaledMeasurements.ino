void setAdjScaledMeasurements()
{
  float s = (CHOOSEN_MEASUREMENT_MODE & 0x10) ? SCALE_16BIT : SCALE_14BIT;

  // Obtain adjusted and scaled values of magnetometer
  magX = rawX * AdjX;
  magY = rawY * AdjY;
  magZ = rawZ * AdjZ;

  // Multiply by scale to get units in Micro-Tesla instead of Micro-Tesla-per-LSB
  magX *= s;
  magY *= s;
  magZ *= s;
}
