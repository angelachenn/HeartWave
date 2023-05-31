#include "LED.h"

LED::LED(QMainWindow* mainWindow){
    lights[0] = mainWindow->findChild<QLabel*>("easyLevel");
    lights[1] = mainWindow->findChild<QLabel*>("mediumLevel");
    lights[2] = mainWindow->findChild<QLabel*>("highLevel");
    for (int i = 0; i < 3; ++i){
        lights[i]->setVisible(false);
    }
}

void LED::turnOn(int heartCoherence){
    for (int i = 0; i < 3; ++i){
        if ((i+1) == heartCoherence){
            lights[i]->setVisible(true);
        } else {
            lights[i]->setVisible(false);
        }
    }
}

void LED::turnOff(){
    for (int i = 0; i < 3; ++i){
        lights[i]->setVisible(false);
    }
}