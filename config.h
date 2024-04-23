#ifndef GAMENARC_CONFIG_
#define GAMENARC_CONFIG_
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#define CONFIG_PATH "C:\\Program Files (x86)\\WSUCougHacks\\config.cfg"
#define CONFIG_PATH_LOCATE "C:\\Program Files (x86)\\WSUCougHacks\\"

void promptConfigInfo(std::string* state, std::string* name, std::string* email);

void readConfigInfo(std::string* state, std::string* name, std::string* email);

#endif