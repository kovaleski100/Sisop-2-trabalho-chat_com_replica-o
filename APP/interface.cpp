#include <thread>
#include "gmClient.h"
#include "interface.h"

Interface::Interface(GMClient *gmc_, string user_)
{
    gmc = gmc_;
    user = user_;
};
Interface::~Interface(){
    wmove(mainWin,0,0);
    endwin();
}
void Interface::Start()
{
    gmc->setInterface(this);
    createWindows();
    gmc->Start();
    thread t(&GMClient::SendMessage, gmc, "<entrou no grupo>");
    t.detach();
    while (getInput())
    {
        continue;
    }
}

void Interface::createWindows()
{
    mainWin = initscr();
    // Capture special keys
    keypad(mainWin, TRUE);

    // Two windows, one for the chat and one for the input
    createChatWindow();
    createInputWindow();
}

void Interface::createChatWindow()
{

    chatWinBox = subwin(mainWin, (LINES * 0.8), COLS, 0, 0);
    box(chatWinBox, 0, 0);
    wrefresh(chatWinBox);
    chatWin = subwin(chatWinBox, (LINES * 0.8 - 2), COLS - 2, 1, 1);
    scrollok(chatWin, TRUE);
}

void Interface::createInputWindow()
{
    // Create input box and window
    inputWinBox = subwin(mainWin, (LINES * 0.2) - 1, COLS, (LINES * 0.8) + 1, 0);
    box(inputWinBox, 0, 0);
    inputWin = subwin(inputWinBox, (LINES * 0.2) - 3, COLS - 2, (LINES * 0.8) + 2, 1);
}

bool Interface::getInput()
{
    // Draw the "user1: " waiting the input
    wmove(inputWin, 0, 0);
    wprintw(inputWin, (user + ": ").c_str());
    wrefresh(inputWin);
    // Get the input
    char str[256];
    wgetstr(inputWin, str);
    // Clear the input window to remove what the just sended
    wclear(inputWin);
    string text = str;
    wrefresh(inputWin);
    if (text == "exit")
    {
        return false;
    }
    if (!text.empty())
    {
        thread t(&GMClient::SendMessage, gmc, text);
        t.detach();
    }

    return true;
}

// Called by the GMClient to show the new messages
void Interface::DisplayText(string text)
{
    wprintw(chatWin, text.c_str());
    wprintw(chatWin, "\n");
    wrefresh(chatWin);
    // Refresh the input window so the cursor go to there
    wrefresh(inputWin);
}
