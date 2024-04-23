using namespace std;
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <direct.h>
#include "registry_poll.h"
#include "config.h"
#include "smtp.h"
#include <chrono>
#include <ctime> 
#include "ngui.h"


/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    MAIN

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */
int main() {
    // +-+-+-+-+ VARIABLES +-+-+-+-+-+
    string windowTitle;
    string formattedTime;
    string cmd;
    string name;
    string email;
    ifstream inFS;
    ofstream outFS;
    int id;
    string promptConfig = "true";
    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    HideConsole();

    inFS.open(CONFIG_PATH);

    if (inFS.is_open()) {
        readConfigInfo(&promptConfig,&name,&email);
        inFS.close();
    } else {
        outFS.open(CONFIG_PATH);
        _mkdir(CONFIG_PATH_LOCATE);
        outFS << "BLANK\r" << "BLANK\r" << "PROMPT\r";
        outFS.close();
        inFS.open(CONFIG_PATH);

        readConfigInfo(&promptConfig, &name, &email);

        inFS.close();
    }
    
    if (promptConfig == "PROMPT") {
        promptConfigInfo(&promptConfig, &name, &email);
    }


    // +-+-+-+-+-+ LOOP +-+-+-+-+-+-+
    while (true) {
        id = 0;

        // wait for game to open
        while (id == 0) {
            id = pollSteamRegistry();
            Sleep(500);
        }

        // Get start time
        auto start = std::chrono::system_clock::now();

        // Get game name
        windowTitle = getSteamTitle(id);

        // wait until game closes
        while (id != 0) {
            id = pollSteamRegistry();
            Sleep(500);
        }

        // Get close time
        auto end = std::chrono::system_clock::now();

        // Close Time - Start Time = Total Time
        std::chrono::duration<double> elapsed_seconds = end - start;
        // Output formatted time
        formattedTime = formatTime(elapsed_seconds.count());

        sendMail(name, email, windowTitle, formattedTime);
        
        promptConfig = "CAUGHT";

        doGui(nullptr, nullptr, nullptr, &promptConfig, &windowTitle, &formattedTime);

        //cmd = "echo You were playing ";
        //cmd.append(windowTitle);
        //cmd.append(" for ");
        //cmd.append(formatTime(elapsed_seconds.count()));
        //cmd.append(" & echo You should have been working! & color 4B & pause");
        //system(cmd.c_str());
        ////closeSteam();
    }

    return 0;
}