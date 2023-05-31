#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <Session.h>

#include <QObject>
#include <QMainWindow>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <QGroupBox>
#include <QDateTime>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSignalMapper>

using namespace std;

class Display : public QObject {
    Q_OBJECT
	public:
        explicit Display(QMainWindow* mainWindow);
		~Display();

		void menuDisplay();
		void updateDisplay(Session*, int);
		void changeDisplay(string);

        void showcaseSessions(vector<Session*>*);
        void showcaseSession(Session*);

        // Getters for QFrames and QPushButtons
        QFrame* getCurrentFrame() const;
        QFrame* getSessionFrame() const;
        QFrame* getSettingsFrame() const;
        QPushButton* getSettingsButton() const;
        QPushButton* getHistoryButton() const;
        QGroupBox* getBatteryAlert() const;
        QGroupBox* getDisconnectAlert() const;

    signals:
        void toggleFrame(QFrame* frame);
        void toggleButtonHighlight(QPushButton* button);
        void toggleBattery(bool on);
        void toggleBreath(bool on);
        void sessionDelete(int);

    private slots:
        void back();
        void viewSession(QObject*);
        void deleteSession(QObject*);

    private:
        QFrame* menuFrame;
        QFrame* offFrame;
        QFrame* sessionFrame;
        QFrame* historyFrame;
        QFrame* settingsFrame;
        QFrame* individualHistoryFrame;
        QFrame* current;

        QPushButton* settingsButton;
        QPushButton* historyButton;

        QGroupBox* batteryAlert;
        QGroupBox* disconnectAlert;

        QList<QFrame*> shownFrames;
};

#endif
