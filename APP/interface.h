#pragma once
#ifndef INTERFACE_H
#define INTERFACE_H
#include <iostream>
#include <string>
#include <curses.h>
#include <ncurses.h>
#include <boost/uuid/uuid.hpp>

class GMClient;

using namespace std;

class Interface
{
private:
    GMClient *gmc;
    WINDOW *mainWin, *inputWin, *chatWin, *chatWinBox, *inputWinBox;
    string user;

    void displayText();
    void createWindows();
    void createChatWindow();
    void createInputWindow();
    bool getInput();

public:
    Interface(GMClient *gmc_, string user_);

    void DisplayText(string text);

};



#endif