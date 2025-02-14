#include "Arduino.h"
#include "robotka.h"

/////////////////////////////////////
static const uint8_t Bbutton1 = 34;
static const uint8_t Bbutton2 = 35;
//const float ticksToMm = 1.5873; // prepocet z tiku v enkoderech na mm
const double ticksToMm = 1.57480314951;
const float roztec = 132.855; // roztec kol v mm
double ML= 0.991489999;// rozdíl v motorech
double MP= 0.99999999999;// rozdíl v motorech
void encodery() {
    Serial.printf("L: %f, R: %f\n", rkMotorsGetPositionLeft(), rkMotorsGetPositionRight());
}
void forward(float mm, int speed)
{
  //rkMotorsDrive(mm * ticksToMm * ML, mm * ticksToMm * MP, speed);
  bool leftDone = false;
  bool rightDone = false;

  rkMotorsDriveRightAsync(mm * ticksToMm * -1, speed * MP, [&]() {
    printf("Pravy motor dojel!\n");
    rightDone = true;
  });

  rkMotorsDriveLeftAsync(mm * ticksToMm * -1, speed * ML, [&]() {
    printf("Levy motor dojel!\n");
    leftDone = true;
  });

  // Wait for both motors to finish
  while (!leftDone && !rightDone) {
    delay(10); // Small delay to prevent busy waiting
  }
}
void forward_ak(int mm, int speed)
{
  int a=0;
  float o= (1/50) * mm;
  //float position =  mm * ticksToMm;
    for(int i = 5 ; (i< speed); i= (i +5)){
      //rkMotorsDrive((1/50) * position, (1/50) * position, i);
      
      forward(o, i);
      std::cout << "enconderyyyy: %f\n" << rkMotorsGetPositionRight() << std::endl;
      printf("zrychlovani\n");
      encodery();
      a++;
    }

    int b= ((50-2*a)/50)* mm;
    //rkMotorsDrive((b/50)* position, (b/50) * position, speed);
    forward(b , speed);

    for(int i =  speed; (i> 5); i= (i -5)){
      //rkMotorsDrive((1/50) * position, (1/50) * position, i);
      forward(o, i);
      std::cout << "enconderyyyy: %f\n" << rkMotorsGetPositionRight() << std::endl;
      printf("spomalovani\n");
       encodery();
    }
    rkMotorsSetPower(0, 0); 
}
void radius_r(int degrees, int polomer, int speed)
{
    bool rightDone = false;
    bool leftDone = false;

    int sR = speed * ((polomer) / (polomer + roztec));
    int sL = speed ;

    rkMotorsDriveRightAsync((ticksToMm * polomer * -PI * degrees) / 180, sR, [&]() {
        printf("Pravy motor dojel!\n");
        rightDone = true;
    });

    rkMotorsDriveLeftAsync((ticksToMm * (polomer + roztec) * -PI * degrees) / 180, sL, [&]() {
        printf("Levy motor dojel!\n");
        leftDone = true;
    });

    // Wait for both motors to finish
    while (!rightDone || !leftDone) {
        delay(10); // Small delay to prevent busy waiting
    }
}
void radius_l(int degrees, int polomer, int speed)
{
    bool rightDone = false;
    bool leftDone = false;

    int sR = speed;
    int sL = speed * ((polomer) / (polomer + roztec));

    rkMotorsDriveRightAsync((ticksToMm * (polomer + roztec) * -PI * degrees) / 180 * MP, sR, [&]() {
        printf("Pravy motor dojel!\n");
        rightDone = true;
    });

    rkMotorsDriveLeftAsync((ticksToMm * polomer * -PI * degrees) / 180 * ML, sL, [&]() {
        printf("Levy motor dojel!\n");
        leftDone = true;
    });

    // Wait for both motors to finish
    while (!rightDone || !leftDone) {
        delay(10); // Small delay to prevent busy waiting
    }
}
void turn_on_spot(int degrees)
{
  printf("L: %f, R: %f\n", (-PI * roztec * (degrees / 360.0) * ticksToMm), (PI * roztec * (degrees / 360.0) * ticksToMm));
  rkMotorsDrive(-PI * roztec * (degrees / 360.0) * ticksToMm , PI * roztec * (degrees / 360.0) * ticksToMm , 30);
}
void back_buttons(int speed)
{
    rkMotorsSetPower((speed* ML), (speed * MP));
    while (true)
    {
      if ((digitalRead(Bbutton1) == LOW)&&(digitalRead(Bbutton2) == LOW)){
        break;
      }
      delay(100);
    }
    delay(150);
    Serial.println("Obě Tlačítka STISKNUTY!");
    rkMotorsSetPower(0, 0);
}
void srovnani(){
  delay(10);
  back_buttons(30);
  delay(10);
  forward(89,30);
  delay(10);
  turn_on_spot(-90);
  delay(10);
  back_buttons(30);
  delay(10);
  forward(89,30);
  delay(10);

}
