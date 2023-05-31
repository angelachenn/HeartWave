#include "Display.h"

Display::Display(QMainWindow* mainWindow):
    menuFrame(mainWindow->findChild<QFrame*>("menuFrame")),
    offFrame(mainWindow->findChild<QFrame*>("offFrame")),
    sessionFrame(mainWindow->findChild<QFrame*>("sessionFrame")),
    historyFrame(mainWindow->findChild<QFrame*>("historyFrame")),
    settingsFrame(mainWindow->findChild<QFrame*>("settingsFrame")),
    individualHistoryFrame(mainWindow->findChild<QFrame*>("individualHistoryFrame")),
    current(offFrame),
    settingsButton(menuFrame->findChild<QPushButton*>("settingsButton")),
    historyButton(menuFrame->findChild<QPushButton*>("historyButton")),
    batteryAlert(mainWindow->findChild<QGroupBox*>("batteryAlert")),
    disconnectAlert(mainWindow->findChild<QGroupBox*>("connectAlert"))
{
    QVBoxLayout* layout = new QVBoxLayout(historyFrame);
    historyFrame->setLayout(layout);
    layout->addWidget(historyFrame->findChild<QLabel*>("label_5"));
}

Display::~Display(){
    delete historyFrame->layout();
}

void Display::menuDisplay(){
    emit toggleFrame(current);
    current = menuFrame;
    emit toggleFrame(current);
}

//updates values of length, achievement and coherence labels at the top of sessionFrame during a live session
void Display::updateDisplay(Session* session, int secs){
    QLabel* length = sessionFrame->findChild<QLabel*>("lengthValue");
    length->setText(QString::number(secs)+".00");

    if (secs%5 == 0){
        QLabel* achievement = sessionFrame->findChild<QLabel*>("achievementValue");
        achievement->setText(QString::number(session->getAchievementScore()));

        QLabel* coherence = sessionFrame->findChild<QLabel*>("coherenceValue");
        coherence->setText(QString::number(session->getCoherenceScores()->back()));
    }

    if (secs == 1){
        QLabel* achievement = sessionFrame->findChild<QLabel*>("achievementValue");
        achievement->setText(QString::number(0));

        QLabel* coherence = sessionFrame->findChild<QLabel*>("coherenceValue");
        coherence->setText(QString::number(0));
    }
}

// function to change Display based on the string passed in
// the UI will change the display tab and emit the necessary signals in order to 
// showcase the correct UI
void Display::changeDisplay(string displayType){
    if (current == offFrame && displayType == "P") {
        menuDisplay();
        emit toggleBattery(true);
    } else if (current != offFrame && displayType == "M") {
            menuDisplay();
    } else {
        emit toggleFrame(current);
        if (current != sessionFrame) {
            shownFrames.append(current);
        }

        if (displayType == "SESH") {
            current = sessionFrame;
        } else if (displayType == "P") {
            current = offFrame;
            emit toggleBreath(false);
            emit toggleBattery(false);
            shownFrames.clear();
            batteryAlert->setVisible(false);
        } else if (displayType == "H") {
            current = historyFrame;
        } else if (displayType == "SETT") {
            current = settingsFrame;
        } else if (displayType == "IHF") {
            current = individualHistoryFrame;
        }
        emit toggleFrame(current);
    }
}

//shows all our sessions in the history log
void Display::showcaseSessions(vector<Session*>* sessions){
    if (historyFrame->layout()) { //just resets the inner layout every time we open
        QLayoutItem* child;
        while ((child = historyFrame->layout()->takeAt(1)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
    QWidget* scrollWidget = new QWidget(historyFrame);
    QVBoxLayout* layout = new QVBoxLayout(scrollWidget);
    scrollWidget->setLayout(layout);
    QSignalMapper* viewMapper = new QSignalMapper(this);
    QSignalMapper* deleteMapper = new QSignalMapper(this);
    //creates a button to view and delete each session
    for (int i = 0; i < static_cast<int>(sessions->size()); ++i){
        QHBoxLayout* hbox = new QHBoxLayout();

        QPushButton* p = new QPushButton("Session: " + sessions->at(i)->getCreatedAt().toString("yyyy-MM-dd hh:mm:ss"));
        QPushButton* d = new QPushButton("X");

        viewMapper->setMapping(p, sessions->at(i));
        deleteMapper->setMapping(d, sessions->at(i));

        hbox->addWidget(p);
        hbox->addWidget(d);

        layout->addLayout(hbox);

        connect(p, SIGNAL(released()), viewMapper, SLOT(map()));
        connect(d, SIGNAL(released()), deleteMapper, SLOT(map()));
        
    }
    connect(viewMapper, SIGNAL(mapped(QObject*)), this, SLOT(viewSession(QObject*)));
    connect(deleteMapper, SIGNAL(mapped(QObject*)), this, SLOT(deleteSession(QObject*)));

    QScrollArea* scrollArea = new QScrollArea(historyFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);
    historyFrame->layout()->addWidget(scrollArea);
}

// function to showcase session summary data and call 
// changeDisplay in order to change the screen to IHF
void Display::showcaseSession(Session* session){
    int t = session->getTotalTime();
    QString low = QString::number((session->getLowTime() * 100)/t) + "%";
    QString med = QString::number((session->getMedTime() * 100)/t) + "%";
    QString high = QString::number((session->getHighTime() * 100)/t) + "%";

    individualHistoryFrame->findChild<QLabel*>("coherenceLow")->setText(low);
    individualHistoryFrame->findChild<QLabel*>("coherenceMedium")->setText(med);
    individualHistoryFrame->findChild<QLabel*>("coherenceHigh")->setText(high);

    individualHistoryFrame->findChild<QLabel*>("averageCoherence")->setText(QString::number(session->getAverageCoherence()));
    individualHistoryFrame->findChild<QLabel*>("lengthSession")->setText(QString::number(session->getSecondsPassed())+" sec");
    individualHistoryFrame->findChild<QLabel*>("achievementScore")->setText(QString::number(session->getAchievementScore()));
    changeDisplay("IHF");
}

QFrame* Display::getCurrentFrame() const {
    return current;
}

QFrame* Display::getSessionFrame() const {
    return sessionFrame;
}

QFrame* Display::getSettingsFrame() const {
    return settingsFrame;
}

QPushButton* Display::getSettingsButton() const {
    return settingsButton;
}

QPushButton* Display::getHistoryButton() const {
    return historyButton;
}

QGroupBox* Display::getBatteryAlert() const {
    return batteryAlert;
}

QGroupBox* Display::getDisconnectAlert() const {
    return disconnectAlert;
}

//implementation of the back button with the help of a QList that acts as a stack
void Display::back(){
    if (!shownFrames.isEmpty()) {
        emit toggleFrame(current);
        current = shownFrames.last();
        shownFrames.removeLast();
        emit toggleFrame(current);
    }
}

void Display::viewSession(QObject* obj){
    Session* session = qobject_cast<Session*>(obj);
    if (session) {
        showcaseSession(session);
    }
}

void Display::deleteSession(QObject* obj){
    Session* session = qobject_cast<Session*>(obj);
    if (session) {
        emit sessionDelete(session->getId());
    }
}