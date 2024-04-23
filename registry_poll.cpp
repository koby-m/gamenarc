#include "registry_poll.h"
#include <string>

size_t curl_to_string(char* ptr, size_t size, size_t nmemb, void* data)
{
    std::string* str = (std::string*)data;
    int c;

    for (c = 0; c < size * nmemb; c++)
    {
        (*str) += ptr[c];
    }

    return size * nmemb;
}

std::string curlGetHtmlSource(std::string& link)
{
    CURL* curl;
    CURLcode res;
    std::string html_txt;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, &link[0]);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_to_string);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html_txt);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            throw std::runtime_error("Can't get html source");
        }

        curl_easy_cleanup(curl);
    }

    return html_txt;
}

//Pars regex from text
std::string stringRegex(const std::string& in_string, const std::string& regx)
{
    const std::regex pattern{ regx };

    for (
        std::sregex_iterator it(in_string.begin(), in_string.end(), pattern);
        it != std::sregex_iterator{};
        ++it
        )
    {
        return (*it)[1];
    }
    return {};
}

//Replace html entities
std::string entityParser(std::string text) {
    std::unordered_map<std::string, std::string> convert({
        {"&quot;", "\""},
        {"&apos;", "'"},
        {"&amp;", "&"},
        {"&gt;", ">"},
        {"&lt;", "<"},
        {"&frasl;", "/"}
        });
    std::string res = "";
    for (int i = 0; i < text.size(); ++i)
    {
        bool flag = false;
        for (auto it = begin(convert); it != end(convert); ++it)
        {
            std::string key = it->first;
            std::string value = it->second;
            if (i + key.size() - 1 < text.size())
            {
                if (text.substr(i, key.size()) == key)
                {
                    res += value;
                    i += static_cast<int>(key.size() - 1);
                    flag = true;
                    break;
                }
            }
        }
        if (!flag) {
            res += text[i];
        }
    }
    return res;
}

std::string getTitle(std::string& link)
{
    std::string title = curlGetHtmlSource(link);
    title = stringRegex(title, R"(<title>([^<]*)<)");
    title = entityParser(title);
    return title;
}

DWORD pollSteamRegistry() {
    DWORD val;
    DWORD dataSize = sizeof(val);

    if (ERROR_SUCCESS == RegGetValueA(HKEY_CURRENT_USER, "Software\\Valve\\Steam", "RunningAppID", RRF_RT_DWORD, nullptr, &val, &dataSize)) {

    } else {
        std::cout << "No Steam registry directory found.\n";
        val = 0;
    }

    return val;
}

std::string getSteamTitle(int val) {
    std::string title;
    std::string scrapeUrl;
    int c; // iterator

    // small delay of 5s
    Sleep(5000);


    // Set URL from sampled Steam RunningAppID
    scrapeUrl = "https://store.steampowered.com/app/";
    scrapeUrl.append(std::to_string(val));
    scrapeUrl.append("/");

    // Scrape!
    title = getTitle(scrapeUrl);

    // take off "on Steam" from returned string
    for (c = 0; c < 9; c++) {
        title.pop_back();
    }

    return title;
}

void closeSteam() {
    // send kill command to Command Prompt
    system("\"\\Program Files (x86)\\Steam\\steam.exe\" -shutdown");
}


void HideConsole()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
}

std::string formatTime(int arg) {
    std::vector<int> timeData;
    std::string formattedTime;

    timeData.push_back(arg);
    timeData.push_back(timeData.at(0) / 60);
    timeData.at(0) %= 60;
    timeData.push_back(timeData.at(1) / 60);
    timeData.at(1) %= 60;
    timeData.push_back(timeData.at(2) / 24);
    timeData.at(2) %= 24;

    if (timeData.at(0) != 0) {
        formattedTime.append(std::to_string(timeData.at(0)));
        formattedTime.append(" second");
        if (timeData.at(0) != 1) {
            formattedTime.append("s");
        }
    }
    if (timeData.at(1) != 0) {
        formattedTime.append(", ");
        formattedTime.append(std::to_string(timeData.at(1)));
        formattedTime.append(" minute");
        if (timeData.at(1) != 1) {
            formattedTime.append("s");
        }
    }
    if (timeData.at(2) != 0) {
        formattedTime.append(", ");
        formattedTime.append(std::to_string(timeData.at(2)));
        formattedTime.append(" hour");
        if (timeData.at(2) != 1) {
            formattedTime.append("s");
        }
    }
    if (timeData.at(3) != 0) {
        formattedTime.append(", ");
        formattedTime.append(std::to_string(timeData.at(3)));
        formattedTime.append(" day");
        if (timeData.at(3) != 1) {
            formattedTime.append("s");
        }
    }

    return formattedTime;
}