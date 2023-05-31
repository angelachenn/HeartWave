#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

#include "Battery.h"
#include "DCS.h"
#include <QMainWindow>
#include <cmath>

using namespace std;

class DCS;

class Device {
	public:
		Device(QMainWindow* mainWindow);
		~Device();

    	void generatePulseData(int);
		vector<int>* readPulse();

		// getters
		DCS* getDCS() const;
        Battery* getBattery() const;
	
	private:
        DCS* dcs;
    	Battery* battery;
		vector<int> pulseData;
};

#endif
