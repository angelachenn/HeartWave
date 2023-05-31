#include "Device.h"

Device::Device(QMainWindow* mainWindow): 
    dcs(new DCS(this, mainWindow)),
    battery(new Battery(mainWindow->findChild<QProgressBar*>("battery"))){}

Device::~Device(){
    delete battery;
    delete dcs;
}

//creates a sin graph to imitate a heart rate based on the given coherence level
void Device::generatePulseData(int coherence){
    srand(time(NULL));
    const int num_points = 60; // Number of points to generate - option to add more if we have a setting for longer time
    const int amplitude = 25; // Amplitude of sine wave
    const double frequency = 0.8; // Frequency of sine wave
    double noise = 0.02;
    if (coherence == 1) {noise = 1.2;}
    else if (coherence == 2) {noise = 0.5;}
    pulseData.clear();

    for (int i = 0; i < num_points; i++) {
         double x = (double) (i + 1);
         double y = amplitude * sin(frequency * x) + noise * amplitude * ((double)rand() / RAND_MAX - 0.5); // Calculate y coordinate with added noise
         y = y + 75; // keeping values in the range of 100 - 50
         pulseData.push_back((int) y); // Convert y-coordinate to integer and store in array
     }
}

vector<int>* Device::readPulse(){
    return &pulseData;
}

DCS* Device::getDCS() const {
    return dcs;
}

Battery* Device::getBattery() const {
    return battery;
}