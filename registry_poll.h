#ifndef REGISTRY_POLL_
#define REGISTRY_POLL_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <regex>
#include <unordered_map>
#include <curl/curl.h>
#include <curl/easy.h>

size_t curl_to_string(char* ptr, size_t size, size_t nmemb, void* data);

std::string curlGetHtmlSource(std::string& link);

std::string stringRegex(const std::string& in_string, const std::string& regx);

std::string entityParser(std::string text);

std::string getTitle(std::string& link);

std::string getSteamTitle(int val);

DWORD pollSteamRegistry();

void closeSteam();

void HideConsole();

std::string formatTime(int arg);

#endif