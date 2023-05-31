#ifndef DCS_H
#define DCS_H

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QEventLoop>
#include <QSpinBox>
#include <QCheckBox>

#include "Device.h"
#include "Display.h"
#include "BreathRegulation.h"
#include "LED.h"
#include "Session.h"
#include "DataStorage.h"

using namespace std;

class Device;

class DCS : public QObject {
    Q_OBJECT
    public:
        explicit DCS(Device*, QMainWindow* mainWindow);
		~DCS();

        void initiateSession();
        void graphHeartRate(vector<int>*);
        void endSession();

        Display* getDisplay() const;

    private:
        Device* device;
        Display* display;
        LED* led;
		DataStorage* storage;
        BreathRegulation* breathRegulation;
		bool usingBreath;
        bool interrupted;
        QGraphicsView* graphView;
        int secondsPassed;
        QMainWindow* mainWindow;
        Session* currentSession;

    signals:
        void triggerBreath(bool);
        void triggerBreathLevel(int);

    private slots:
        void updateSession();
        void displayBatteryAlert();
        void disconnectHR();
        void reconnectHR();
        void lowCoherence();
        void medCoherence();
        void highCoherence();
        void kill();

        void setInterval(int);
        void reset();
        void power();
        void up();
        void down();
        void left();
        void right();
        void select();
        void menu();
        void setBreath();
        void deleteSession(int id);

};

#endif
