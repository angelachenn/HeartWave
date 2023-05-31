#include "DCS.h"

DCS::DCS(Device* d, QMainWindow* mainWindow):
    device(d),
    display(new Display(mainWindow)),
    led(new LED(mainWindow)),
    storage(new DataStorage()),
    breathRegulation(new BreathRegulation(display->getSessionFrame()->findChild<QProgressBar*>("breathPacer"))),
    usingBreath(false),
    interrupted(false),
    graphView(mainWindow->findChild<QGraphicsView*>("graphView")),
    secondsPassed(0),
    mainWindow(mainWindow),
    currentSession(NULL) 
{
    connect(this, SIGNAL(triggerBreath(bool)), this->breathRegulation, SLOT(toggleVisibleBreath()));
    connect(this, SIGNAL(triggerBreathLevel(int)), this->breathRegulation, SLOT(changeBreathLevel(int)));
    connect(this->display, SIGNAL(sessionDelete(int)), this, SLOT(deleteSession(int)));
}

DCS::~DCS(){
    delete breathRegulation;
    delete storage;
    delete led;
    delete display;
}

// initiates a new session to be run by generating pseudo-random data
void DCS::initiateSession() {
    interrupted = false;
    display->changeDisplay("SESH");
    //set using breath
    breathRegulation->setBreathRegulation(usingBreath);
    emit triggerBreath(true);
    //create new session
    currentSession = new Session(rand()%3 + 1);
    led->turnOn(currentSession->getChallengeLevel());
    //generate pulse data for the session based on the challenge level
    device->generatePulseData(currentSession->getChallengeLevel());
    vector<int>* heartRate = device->readPulse();
    graphHeartRate(heartRate);
}

//uses generated heart rates to graph and calculate coherence scores
void DCS::graphHeartRate(vector<int>* heartRate){
    QGraphicsScene *scene = new QGraphicsScene();
    graphView->setScene(scene);
    vector<QGraphicsEllipseItem*> dataPoints;

    //determines how incoherent our graph is (low noise = coherent)
    double noise = 0.02;
    if (currentSession->getChallengeLevel() == 1) {noise = 1.2;}
    else if (currentSession->getChallengeLevel() == 2) {noise = 0.5;}

    for (int i = 0; i < static_cast<int>(heartRate->size()); ++i) {
        if (interrupted){ //if something has stopped the session from running
            heartRate =  new vector<int>(heartRate->begin(), heartRate->begin() + (i - 1));
            break;
        }

        if (dataPoints.size() % 5 == 0){ //every 5 seconds calculate the coherence score
            int coherence = 16 - (16 * noise) + rand() % 5 - 2;
            while (coherence > 16) { coherence -= rand() % 3 + 1; }
            while (coherence < 0) { coherence += rand() % 3 + 1; }

            currentSession->increaseAchievement(coherence);
            currentSession->addCoherenceScore(coherence);
        }

        QGraphicsEllipseItem *point = new QGraphicsEllipseItem(0, 0, 5, 5);

        // Set the position and color of the point
        point->setPos(0, heartRate->at(i));
        point->setBrush(QBrush(Qt::red));

        // Add the point to the scene
        scene->addItem(point);
        dataPoints.push_back(point);

        //move all our points to the left by 10 so it looks like the graph is moving centered arounf the middle of the screen
        for (int j = 0; j <= i; ++j) {
            QGraphicsEllipseItem *p = dataPoints.at(j);
            dataPoints.at(j)->setPos(p->pos().x() - 10, p->pos().y());
            if (p->pos().x() < -230) {
                p->hide();
            }
        }
        //timer
        QEventLoop loop;
        updateSession();
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();
    }
}

//called whenever a session is ended
void DCS::endSession() {
    interrupted = true;
    led->turnOff();
    display->changeDisplay("IHF");
    currentSession->setSecondsPassed(secondsPassed);
    storage->addSession(currentSession);
    display->showcaseSession(currentSession);

    secondsPassed = 0;
}

Display* DCS::getDisplay() const {
    return display;
}

void DCS::updateSession() {
    display->updateDisplay(currentSession, ++secondsPassed);
}

void DCS::displayBatteryAlert() {
    display->getBatteryAlert()->setVisible(true);
}

void DCS::disconnectHR() {
    display->getDisconnectAlert()->setVisible(true);
    if (display->getCurrentFrame()->objectName() == "sessionFrame") {
        endSession();
    }
}

void DCS::reconnectHR() {
    display->getDisconnectAlert()->setVisible(false);
}

//simulation to run a low coherence session
void DCS::lowCoherence() {
    interrupted = false;
    display->changeDisplay("SESH");

    breathRegulation->setBreathRegulation(usingBreath);
    emit triggerBreath(true);

    currentSession = new Session(1);
    led->turnOn(currentSession->getChallengeLevel());
    secondsPassed = 0;
    device->generatePulseData(currentSession->getChallengeLevel());
    vector<int>* heartRate = device->readPulse();
    graphHeartRate(heartRate);
}

//simulation to run a medium coherence session
void DCS::medCoherence() {
    interrupted = false;
    display->changeDisplay("SESH");

    breathRegulation->setBreathRegulation(usingBreath);
    emit triggerBreath(true);

    currentSession = new Session(2);
    led->turnOn(currentSession->getChallengeLevel());
    secondsPassed = 0;
    device->generatePulseData(currentSession->getChallengeLevel());
    vector<int>* heartRate = device->readPulse();
    graphHeartRate(heartRate);
}

//simulation to run a high coherence session
void DCS::highCoherence() {
    interrupted = false;
    display->changeDisplay("SESH");

    breathRegulation->setBreathRegulation(usingBreath);
    emit triggerBreath(true);

    currentSession = new Session(3);
    led->turnOn(currentSession->getChallengeLevel());
    secondsPassed = 0;
    device->generatePulseData(currentSession->getChallengeLevel());
    vector<int>* heartRate = device->readPulse();
    graphHeartRate(heartRate);
}

//when the battery dies save the current session and turn the device off 
void DCS::kill() {
    if (display->getCurrentFrame()->objectName() == "sessionFrame") {
        endSession();
    }
    power();
}

void DCS::setInterval(int interval){
   emit triggerBreathLevel(interval);
}

//resets the device by deleting all stored sessions and reseting settings to default
void DCS::reset() {
    delete storage;
    storage = new DataStorage();
    usingBreath = true;
    display->changeDisplay("M");
    emit triggerBreathLevel(10);
    QFrame* settingsFrame = display->getSettingsFrame();
    Qt::CheckState state = settingsFrame->findChild<QCheckBox*>("checkBoxSetBreath")->checkState();

    if (state == Qt::Checked) {
        usingBreath = true;
    } else {
        usingBreath = false;
    }

    settingsFrame->findChild<QCheckBox*>("checkBoxSetBreath")->setChecked(false);
    settingsFrame->findChild<QSpinBox*>("breathLevel")->setValue(10);
}

//turning the device on and off
void DCS::power(){
    interrupted = true;
    led->turnOff();
    display->changeDisplay("P");
}

void DCS::up(){
    if (display->getCurrentFrame()->objectName() == "menuFrame") {
        if (display->getHistoryButton()->isDown()) {
            emit display->toggleButtonHighlight(display->getHistoryButton());
        }
        if (display->getSettingsButton()->isDown()) {
            emit display->toggleButtonHighlight(display->getSettingsButton());
        }
    }
}

void DCS::down(){
    if (display->getCurrentFrame()->objectName() == "menuFrame") {
        if (!display->getHistoryButton()->isDown() && !display->getSettingsButton()->isDown()) {
            emit display->toggleButtonHighlight(display->getHistoryButton());
        }
    }
}

void DCS::left(){
    if (display->getCurrentFrame()->objectName() == "menuFrame") {
        if (display->getSettingsButton()->isDown()) {
            emit display->toggleButtonHighlight(display->getSettingsButton());
            emit display->toggleButtonHighlight(display->getHistoryButton());
        }
    }
}
void DCS::right(){
    if (display->getCurrentFrame()->objectName() == "menuFrame") {
        if (display->getHistoryButton()->isDown()) {
            emit display->toggleButtonHighlight(display->getSettingsButton());
            emit display->toggleButtonHighlight(display->getHistoryButton());
        }
    }
}

void DCS::select(){
    if (display->getBatteryAlert()->isVisible()) {
        display->getBatteryAlert()->setVisible(false);
        return;
    }
    if (display->getCurrentFrame()->objectName() == "menuFrame") {
        if (display->getHistoryButton()->isDown()) {
            display->showcaseSessions(storage->getSessions());
            display->changeDisplay("H");
        } else if (display->getSettingsButton()->isDown()) {
            display->changeDisplay("SETT");
        } else {
            initiateSession();
        }
    }
    else if (display->getCurrentFrame()->objectName() == "sessionFrame") {
            endSession();
    }
}

void DCS::menu(){
    display->changeDisplay("M");
}

void DCS::setBreath(){
    if (usingBreath == false) {
        usingBreath = true;
    } else {
        usingBreath = false;
    }
}

void DCS::deleteSession(int id){
    storage->removeSession(id);
    display->showcaseSessions(storage->getSessions());
}