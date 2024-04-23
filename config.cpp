#include "config.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "ngui.h"

void promptConfigInfo(std::string* state, std::string* name, std::string* email) {
	std::string first, last;
	std::ofstream outFS;
	doGui(&first, &last, email, state, nullptr, nullptr);

	first.append(" ");
	first.append(last);
	*name = first;

	*state = "NOPROMPT";

	outFS.open(CONFIG_PATH);
	outFS << *name << std::endl;
	outFS << *email << std::endl;
	outFS << *state;

	outFS.close();
}

void readConfigInfo(std::string* state, std::string* name, std::string* email) {
	std::ifstream inFS;
	std::string readLine[3];
	int c = 0;
	inFS.open(CONFIG_PATH);

	for(c = 0; c < 3; c++) {
		std::getline(inFS, readLine[c]);
	}

	*name = readLine[0];
	*email = readLine[1];
	*state = readLine[2];

	inFS.close();
}