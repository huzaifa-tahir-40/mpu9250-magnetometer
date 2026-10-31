void wakeUpMPU()
{
  writeRegister(MPU_ADDR, PWR_MGMT_1, 0x0);
  delay(10);
}