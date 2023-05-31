#include "Battery.h"

Battery::Battery(QProgressBar* widget): 
    batteryLevel(100),
    progressBar(widget),
    timer(new QTimer(this)) 
{
    progressBar->setVisible(false);
    progressBar->setValue(batteryLevel);
    progressBar->setRange(0, 100);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateBatteryLevel()));
}

Battery::~Battery(){
    delete timer;
}

//decreases battery level by 1% and is called every 1 second
//if battery level is 10, emit signal that triggers the alert for low battery
//if battery level is 0, emit signal to power off the device
void Battery::decBatteryLevel() {
    --batteryLevel;
    if (batteryLevel == 10){
        emit batteryAlert();
    } else if (batteryLevel == 0) {
        emit kill();
    }
}

int Battery::getBatteryLevel() const {
    return batteryLevel;
}

//updates the visual change of the qprogressbar based on battery level
void Battery::updateBatteryLevel(){
    decBatteryLevel();
    progressBar->setValue(batteryLevel);
}

//battery should only be on and actively decreasing when the device is powered on
void Battery::toggleVisible(bool on) {
    if (on) {
        progressBar->setVisible(true);
        timer->start(1000);
    } else {
        progressBar->setVisible(false);
        timer->stop();
    }
}

//slots for test buttons-- in this one, battery level is set to 11 in order to showcase our alert extension
//battery will still decrease and hit 0 eventually, which will power off the device
void Battery::killBattery() {
    batteryLevel = 11;
    decBatteryLevel();
}

void Battery::recharge() {
    batteryLevel = 100;
}