#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "robotka.h"
#include "motor_commands.h"
#include "barvy.h"
void sprint(int squers){
    forward(300 * squers ,80);
}
void slalom(){
    forward(350,60);
    turn_on_spot(90);
    forward(150,60);
    radius_l(180, 100, 60);
    radius_r(180, 150, 60);
    forward(150,60);
}
void kulicky(){
    turn_on_spot(90);
    forward(45,30);
    turn_on_spot(-90);
    bool rightDone, leftDone;
    rkMotorsDriveRightAsync(660 * ticksToMm * -1, 30 * MP, [&]() {
        printf("Pravy motor dojel!\n");
        rightDone = true;
    });
    rkMotorsDriveLeftAsync(660 * ticksToMm * -1, 30 * ML, [&]() {
        printf("Levy motor dojel!\n");
        leftDone = true;
    });
    while (!rightDone || !leftDone) {
        rkServosSetPosition(1, 10);
        delay(200);
        rkServosSetPosition(1, 90);
        delay(150);
    }
    forward(-70,30);
    delay(100);
    turn_on_spot(-90);
    forward(340,60);
}
void medved(){  
    rkServosSetPosition(4, 88);
    forward(450,70);
    radius_r(90, 75, 70);
    forward(450,70);
    delay(10);
    back_buttons(45);
    delay(10);
    rkServosSetPosition(4, -41);
    delay(10);
    forward(85,40);
    delay(10);
    turn_on_spot(90);
    delay(10);
    back_buttons(30);
    delay(10);
    forward(85,40);
    delay(10);
    forward(300,70);
    turn_on_spot(-90);
    forward(300,70);
}
void bludiste(){
    forward(300,70);
    for(int i=0; i< 5; i++){
        if(rkUltraMeasure(1) > 250){// v pravo je volno
            turn_on_spot(90);
            delay(10);
            forward(300,70);
            delay(10);
        }
        else if(rkUltraMeasure(3) > 250){// ve predu je volno
            forward(300,70);
            delay(10);
        }
        else{
            turn_on_spot(-90);
            srovnani();
        }
        delay(100);
    }
    while(!cervena()){
        if(rkUltraMeasure(1) > 250){// v pravo je volno
            turn_on_spot(90);
            forward(300,50);
        }
        else if(rkUltraMeasure(3) > 250){// ve predu je volno
            forward(300,70);
        }
        else{
            turn_on_spot(-90);
            if(rkUltraMeasure(3) > 250){
                srovnani(); 
                turn_on_spot(90); 
            }
            else{
                srovnani();
            }
        }
        delay(100);
    }
}
////////////////////////////////////////////
void trap() {
    Serial.println("trap\n");
    while (1)
        ;
}
void test_batery(){
    printf("batery percent: %u\n", rkBatteryPercent());
    printf("batery percent: %u\n", rkBatteryVoltageMv()/1000);
}
void test_servos(){
    rkServosSetPosition(1, 90);//dole
    delay(1000);
    rkServosSetPosition(4, -41);// vevnitr
    delay(1000);
    rkServosSetPosition(1, 30);
    delay(1000);
    rkServosSetPosition(4, 88);
    delay(1000);
    rkServosSetPosition(1, 90);
    delay(1000);
    rkServosSetPosition(4, -41);
    delay(1000); 
}
void test_motory(){
    forward(1000,50);
    encodery();
    delay(1000);
    //back_buttons();
    encodery();
    delay(1000);
    turn_on_spot(90);
    encodery();
    delay(1000);
    turn_on_spot(-90);
    encodery();
    delay(1000);
    radius_r(90, 300, 100);
    encodery();
    delay(1000);
    radius_l(90, 300, 100);
    encodery();
    delay(1000);
}
void test_ultrazvuk(){
    while(true){
        printf("ultrazvuk 1: %u\n", rkUltraMeasure(1));
        printf("ultrazvuk 3: %u\n", rkUltraMeasure(3));
        delay(500);
    }
}
void configurating(){
    Serial.begin(115200);
    rkConfig cfg;
    rkSetup(cfg);
    static const uint8_t TCS_SDA_pin = 21;
    static const uint8_t TCS_SCL_pin = 22;
    static const uint8_t Bbutton1 = 34;
    static const uint8_t Bbutton2 = 35;
    pinMode(Bbutton1, GPIO_MODE_INPUT);
    pinMode(Bbutton2, GPIO_MODE_INPUT);
    Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
    pinMode(TCS_SDA_pin, PULLUP);
    pinMode(TCS_SCL_pin, PULLUP);
    Wire1.begin(TCS_SDA_pin, TCS_SCL_pin, 100000);
    if (!tcs.begin(TCS34725_ADDRESS, &Wire1)) {
        printf("Can not connect to the RGB sensor");
        trap();
    }
    printf("Starting main loop\n");
    //start tlacitko pro kalibraci klepet

    rkLedBlue(false);
    rkLedGreen(false);
    rkLedYellow(false);
    rkLedRed(false);
    //setup_dalkove();// pokud chceš použít změn hesla a jména wifi
    test_batery();
}

enum RobotButton {
    NONE,
    UP,
    DOWN,
    RIGHT,
    LEFT,
    ON,
    BUTTON1,
    BUTTON2
};

RobotButton getPressed() {
    if (rkButtonUp()) return UP;
    if (rkButtonDown()) return DOWN;
    if (rkButtonRight()) return RIGHT;
    if (rkButtonLeft()) return LEFT;
    if (rkButtonOn()) return ON;
    if (digitalRead(Bbutton1) == LOW) return BUTTON1;
    if (digitalRead(Bbutton2) == LOW) return BUTTON2;
    return NONE;
}

void setup() {
    configurating();
    
    while(true) {
        switch(getPressed()) {
            case UP:
                rkLedBlue(true);
                delay(3000);
                radius_r(180, 300, 60);
                rkLedBlue(false);
                break;
                
            case DOWN:
                rkLedYellow(true);
                delay(10000);
                medved();
                delay(10000);
                bludiste();
                delay(10000);
                kulicky();
                delay(10000);
                slalom();
                delay(10000);
                sprint(4);
                rkLedYellow(false);
                break;
                
            case RIGHT:
                rkLedGreen(true);
                delay(5000);
                bludiste();
                rkBuzzerSet(true);
                delay(200);
                rkBuzzerSet(false);
                rkLedGreen(false);
                break;
                
            case LEFT:
                rkLedYellow(true);
                delay(5000);
                medved();
                rkLedYellow(false);
                break;
                
            case ON:
                rkLedRed(true);
                delay(5000);
                kulicky();
                rkLedRed(false);
                break;
                
            case BUTTON1:
                rkLedGreen(true);
                delay(3000);
                slalom();
                rkLedGreen(false);
                break;
                
            case BUTTON2:
                rkLedRed(true);
                delay(3000);
                sprint(4);
                rkLedRed(false);
                break;
        }
        delay(50);
    }
}

void loop() {
}

