
#include<Wire.h>
const int MPU = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
double compAngleX, compAngleY,/*gzangle,*/compAnglez, timer;
double accXangle , accYangle, acczangle , gyroXrate , gyroYrate, gyroZrate;
double gyroXAngle, gyroYAngle, gyroZAngle;
// float rgyro,w;
int ap = 0.955;

void getraw(const int add)
{
  Wire.beginTransmission(add);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  // Serial.println("HERE");
  Wire.requestFrom(add, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)



  accXangle = (atan2(AcY, AcZ) * RAD_TO_DEG);
  accYangle = (atan2(AcX, AcZ) * RAD_TO_DEG);
  acczangle = (atan2(AcX, AcY) * RAD_TO_DEG); /* my attempt to calculate yaw but not correct*/
  gyroXrate = GyX / 16.5;
  gyroYrate = GyY / 16.5;
  gyroZrate = GyZ / 16.5;
  timer = millis();
  //angular position
  gyroXAngle += gyroXrate * (millis() - timer) / 1000;
  gyroYAngle += gyroYrate * (millis() - timer) / 1000;
  gyroZAngle += gyroZrate * (millis() - timer) / 1000; /* my attempt to calculate yaw but not correct*/
  //---------------------------\\
  //COMPLIMENTRY FILTER STARTED\\
  //---------------------------\\

  compAngleX = ap * (compAngleX + gyroXAngle) + (1 - ap) * accXangle;
  compAngleY = ap * (compAngleY + gyroYAngle) + (1 - ap) * accYangle;
  compAnglez = ap * (compAnglez + gyroZAngle) + (1 - ap) * acczangle; /*yaw but not correct*/
  
  Serial.print("compAngleX: ");
  Serial.println(compAngleX);
  Serial.print("compAngleY: ");
  Serial.println(compAngleY);
  Serial.print("compAnglez: ");
  Serial.println(compAnglez);
  
}

void wakeupmpu()
{
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);  // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}
void setup()
{

  Serial.begin(115200);
  pinMode(A0, OUTPUT);
  pinMode(A2, OUTPUT);

}
int loopnumber;
void loop()
{
  wakeupmpu();
  delay(20);
  digitalWrite(A0, loopnumber % 2);
  digitalWrite(A2, loopnumber / 2);
  Serial.print("#############################");
  Serial.print(loopnumber);
  Serial.println("#################################");
  getraw(MPU);
  loopnumber++;
  if (loopnumber == 4)
  {
    loopnumber = 0;
  }
  delay(100);
}
