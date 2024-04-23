#ifndef SMTP_
#define SMTP_

#include <stdio.h>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <string.h>
#include <curl/curl.h>

#define FROM    "wsugamenarc@gmail.com"

static size_t payload_source(void* ptr, size_t size, size_t nmemb, void* userp);
int sendMail(std::string name, std::string email, std::string game, std::string formattedTime);

#endif