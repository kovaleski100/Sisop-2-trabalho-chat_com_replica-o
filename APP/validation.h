#pragma once
#ifndef VALIDATION_H
#define VALIDATION_H

#include <iostream>
#include <regex>
#include <boost/asio.hpp>
#define MAX_PORT 65535
#define MIN_PORT 0

using namespace std;

class Validation
{
private:
    // Usada para username e groupname
    static string nameIsValid(string name);
    static string serverIPAdressIsValid(string serverIPAdress);
    static string portIsValid(int port);

public:
    Validation();
    static string ValidateInputs(string username, string groupName, string serverIPAdress, int port);
};

// Valida todos os inputs, agrega todas as mensagens de erro
string Validation::ValidateInputs(string username, string groupName, string serverIPAdress, int port)
{
    string errorMessage = "";
    string err;

    err = nameIsValid(username);
    if (err != "")
    {
        errorMessage += "username " + username + " is invalid: " + err + "\n";
    }

    err = nameIsValid(groupName);
    if (err != "")
    {
        errorMessage += "groupName " + groupName + " is invalid: " + err + "\n";
    }

    err = serverIPAdressIsValid(serverIPAdress);
    if (err != "")
    {
        errorMessage += "serverIPAdress " + serverIPAdress + " is invalid: " + err + "\n";
    }

    err = portIsValid(port);
    if (err != "")
    {
        errorMessage += "port " + to_string(port) + " is invalid: " + err + "\n";
    }

    return errorMessage;
}

// Usada para username e groupname
string Validation::nameIsValid(string name)
{
    if (name.length() < 4 || name.length() > 20)
    {
        return "precisa ter entre 4 e 20 characteres";
    }
    if (!regex_match(name, regex("[a-zA-Z][a-zA-Z0-9\\.]*")))
    {
        return "precisa ter esse formato: [a-zA-Z][a-zA-Z0-9.]*";
    }
    return "";
}

string Validation::serverIPAdressIsValid(string serverIPAdress)
{
    boost::system::error_code ec;
    boost::asio::ip::address::from_string(serverIPAdress, ec);
    if (ec)
        return "precisa ter esse formato 127.0.0.1";

    return "";
}

string Validation::portIsValid(int port)
{
    if (port > MAX_PORT || port < MIN_PORT)
    {
        return "precisa ser entre " + to_string(MIN_PORT) + " e " + to_string(MAX_PORT);
    }
    return "";
}

#endif