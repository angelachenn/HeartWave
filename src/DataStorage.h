#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

#include "Session.h"

using namespace std;

class DataStorage {
	public:
		DataStorage();
		~DataStorage();

		void addSession(Session*);
		void removeSession(int);

		void getSession(int, Session**) const;
		vector<Session*>* getSessions();
	
	private:
		vector<Session*> sessions;
};

#endif