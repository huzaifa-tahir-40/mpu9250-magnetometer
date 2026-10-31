void DetectMagnetometer()
{
    if (readRegister(AK8963_ADDR, WIA_ADDR) == 0x48)
    {
        Serial.println("!!!MAGNETOMETER DETECTED!!!");
    }
    else
    {
        Serial.println("!!!MAGNETOMETER NOT DETECTED!!!");
        while (1);
    }
}