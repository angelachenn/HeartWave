#include "DataStorage.h"

DataStorage::DataStorage(){}

DataStorage::~DataStorage(){
    int n = sessions.size();

    for (int i = 0; i < n; ++i){
        delete sessions[i];
    }
}

// adding a session to the end of the vector
void DataStorage::addSession(Session* s){
    sessions.push_back(s);
}

// removing session from session vector based on session id
void DataStorage::removeSession(int id){
    for (vector<Session*>::iterator itr = sessions.begin(); itr != sessions.end(); ++itr){
        if ((*itr)->getId() == id){
            sessions.erase(itr);
            return;
        }
    }
}

// getting session based on session id
void DataStorage::getSession(int id, Session** s) const {
    *s = NULL;
    for (vector<Session*>::const_iterator itr = sessions.begin(); itr != sessions.end(); ++itr){
        if ((*itr)->getId() == id){
            *s = *itr;
            return;
        }
    }
}

// returns vector of all sessions
vector<Session*>* DataStorage::getSessions() {
    return &sessions;
}