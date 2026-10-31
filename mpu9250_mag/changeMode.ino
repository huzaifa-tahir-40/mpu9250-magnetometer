// This function will access the CNTL1 register.
// MODE[3:0]: Operation mode setting
// "0000": Power-down mode
// "0001": Single measurement mode
// "0010": Continuous measurement mode 1 
// "0110": Continuous measurement mode 2 
// "0100": External trigger measurement mode 
// "1000": Self-test mode
// "1111": Fuse ROM access mode
// Other code settings are prohibited

// =============================================================================
// The AK8963 must pass through power-down mode when changing operating modes.
// =============================================================================

void changeMode(uint8_t MODE)
{
  writeRegister(AK8963_ADDR, CNTL1_ADDR, MODE);
  delay(10);
}