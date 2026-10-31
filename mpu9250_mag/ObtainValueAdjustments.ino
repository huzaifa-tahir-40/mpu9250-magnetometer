uint8_t* ObtainValueAdjustments()
{
  static uint8_t Adj[3];

  // Enter Power-down mode of AK8963
  changeMode(POWER_DOWN_MODE);

  // Enter Fuse ROM access mode
  changeMode(ROM_ACCESS_MODE);

  // Obtain measurement adjustments. Adjustments are multiplied later with raw measurements
  Adj[0] = readRegister(AK8963_ADDR, ASAX);
  Adj[1] = readRegister(AK8963_ADDR, ASAY);
  Adj[2] = readRegister(AK8963_ADDR, ASAZ);

  // Return to Power-down mode of AK8963
  changeMode(POWER_DOWN_MODE);

  return Adj;
}