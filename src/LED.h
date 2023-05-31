#ifndef LED_H
#define LED_H

#include <iostream>
#include <string>
#include <iomanip>

#include <QMainWindow>
#include <QLabel>

using namespace std;

class LED {
	public:
		LED(QMainWindow*);

		void turnOn(int);
		void turnOff();
	
	private:
		QLabel* lights[3];
};

#endif