#include <iostream>
#define MAX_PORT 65535
#define MIN_PORT 0

using namespace std;

bool valid_firts_letter(char letter);
bool valid_letter(char letter);
bool validation(string user);
bool validation_port(int port);

bool validation(string user)
{
    int user_tam = user.length();

    if(user_tam < 4)
    {
        return false;
    }
    else
    {
        if(valid_firts_letter(user[0]))
        {
            for(int i= 1; i< user_tam; i++)
            {
                if(valid_letter(user[i]))
                {
                    continue;
                }
                else
                {
                    return false;
                }
                
            }
        }
        else
        {
            return false;
        }
    }
}

bool valid_firts_letter(char letter)
{
    if(letter >= 'A' && letter<='Z')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool valid_letter(char letter)
{
    if((letter>='A' && letter <='Z') || 
       (letter>='a' && letter <='z') || 
       (letter>='0' && letter <='9') || 
       letter == '.')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool validation_port(int port)
{
    if(port >= MIN_PORT && port <= MAX_PORT)
    {
        return true;
    }
    else
    {
        return false;
    }
}