#ifndef BATTERY_H
#define BATTERY_H

#include <iostream>
#include <string>
#include <iomanip>

#include <QProgressBar>
#include <QTimer>

using namespace std;

class Battery : public QWidget {
    Q_OBJECT
	public:
        Battery(QProgressBar* widget);
		~Battery();

		void decBatteryLevel();

		int getBatteryLevel() const;
	
    signals:
        void batteryAlert();
        void kill();

    private slots:
        void updateBatteryLevel();
        void toggleVisible(bool on);
        void killBattery();
        void recharge();

	private:
        int batteryLevel;
        QProgressBar* progressBar;
        QTimer* timer;
};

#endif
