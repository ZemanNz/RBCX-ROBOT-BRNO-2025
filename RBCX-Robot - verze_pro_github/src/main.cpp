#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "robotka.h"
#include "motor_commands.h"
#include "barvy.h"
void sprint(int squers){
    forward(300 * squers ,80);
}
void slalom(std::string side){
    if(side == "Right"){
    forward(300,60);
    turn_on_spot(90);
    forward(150,60);
    radius_l(180, 90, 60);
    radius_r(184, 90, 60);
    forward(150,60);
    }
    else if(side == "Left"){
    forward(300,60);
    turn_on_spot(-90);
    forward(150,60);
    radius_r(180, 90, 60);
    radius_l(184, 90, 60);
    forward(150,60);
    }
}
void kulicky(){
    turn_on_spot(90);
    forward(46,30);
    turn_on_spot(-90);
    back_buttons(30);
    forward(150,30);
    rkServosSetPosition(1, 30);
    delay(200);
    rkServosSetPosition(1, 90);
    forward(300,50);
    rkServosSetPosition(1, 30);
    delay(200);
    rkServosSetPosition(1, 90);
    forward(300,50);
    rkServosSetPosition(1, 30);
    delay(200);
    rkServosSetPosition(1, 90);
    forward(-70,30);
    turn_on_spot(-90);
    forward(340,60);
}
void medved(std::string side){
    if(side == "Right"){
        rkServosSetPosition(4, 88);
        delay(10);
        forward(450,70);
        delay(10);
        radius_r(90, 75, 70);
        delay(10);
        forward(450,70);
        delay(10);
        back_buttons(45);
        delay(10);
        rkServosSetPosition(4, -41);
        delay(10);
        forward(89,30);
        delay(10);
        turn_on_spot(90);
        delay(10);
        back_buttons(30);
        delay(10);
        forward(89,30);
        delay(10);
        forward(300,70);
        delay(10);
        turn_on_spot(-90);
        delay(10);
        forward(300,70);
    }
    else if(side == "Left"){
        rkServosSetPosition(4, 88);
        delay(10);
        forward(450,70);
        delay(10);
        //rkServosSetPosition(4, 40);// sem kdyztak dodat privreni klepet, ne, tonefunguje
        radius_l(90, 75, 70);
        delay(10);
        forward(450,70);
        delay(10);
        back_buttons(45);
        delay(10);
        rkServosSetPosition(4, -41);
        delay(10);
        forward(89,30);
        delay(10);
        turn_on_spot(-90);
        delay(10);
        back_buttons(30);
        delay(10);
        forward(89,30);
        delay(10);
        forward(300,70);
        delay(10);
        turn_on_spot(90);
        delay(10);
        forward(300,70);
    }

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
    rkBuzzerSet(true);
    delay(200);
    rkBuzzerSet(false);
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
    OFF,
    BUTTON1,
    BUTTON2
};

RobotButton getPressed() {
    if (rkButtonUp()) return UP;
    if (rkButtonOff()) return OFF;
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
            case OFF:
                rkLedRed(true);
                delay(3000);

                rkLedRed(false);
                break;

            case UP:
                rkLedBlue(true);
                delay(3000);
                bludiste();
                rkLedBlue(false);
                break;

            case DOWN:
                rkLedYellow(true);
                delay(3000);
                medved("Right");
                rkLedYellow(false);
                break;
                
            case RIGHT:
                rkLedGreen(true);
                delay(3000);
                kulicky();
                rkLedGreen(false);
                break;
                
            case LEFT:
                rkLedYellow(true);
                delay(3000);
                slalom("Right");
                rkLedYellow(false);
                break;
                
            case ON:
                rkLedRed(true);
                delay(3000);
                slalom("Left");
                rkLedRed(false);
                break;
                
            case BUTTON1:
                rkLedGreen(true);
                delay(3000);
                sprint(4);
                rkLedGreen(false);
                break;
                
            case BUTTON2:
                rkLedRed(true);
                delay(3000);
                
                rkLedRed(false);
                break;
        }
        delay(50);
    }
}

void loop() {
}

