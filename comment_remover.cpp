#include "functions.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string comment_remover()
{
    ifstream fin("input.txt");

    if (!fin.is_open())
    {
        cout << "Error opening file!" << endl;
        return "Error Opening File!";
    }

    char c;
    string output;   // ✅ stores entire result

    enum State
    {
        NORMAL,
        IN_STRING,
        IN_CHAR,
        IN_SL_COMMENT,
        IN_ML_COMMENT
    };

    State state = NORMAL;

    while (fin.get(c))
    {
        if (state == NORMAL)
        {
            if (c == '/')
            {
                if (fin.peek() == '/')
                {
                    state = IN_SL_COMMENT;
                    fin.get(c); // consume second '/'
                    continue;
                }
                else if (fin.peek() == '*')
                {
                    state = IN_ML_COMMENT;
                    fin.get(c); // consume '*'
                    continue;
                }
                else
                {
                    output += c;
                }
            }
            else if (c == '"')
            {
                state = IN_STRING;
                output += c;
            }
            else if (c == '\'')
            {
                state = IN_CHAR;
                output += c;
            }
            else
            {
                output += c;
            }
        }
        else if (state == IN_STRING)
        {
            output += c;

            if (c == '\\')
            {
                if (fin.get(c))
                    output += c;
            }
            else if (c == '"')
            {
                state = NORMAL;
            }
        }
        else if (state == IN_CHAR)
        {
            output += c;

            if (c == '\\')
            {
                if (fin.get(c))
                    output += c;
            }
            else if (c == '\'')
            {
                state = NORMAL;
            }
        }
        else if (state == IN_SL_COMMENT)
        {
            if (c == '\n')
            {
                output += '\n';
                state = NORMAL;
            }
        }
        else if (state == IN_ML_COMMENT)
        {
            if (c == '*' && fin.peek() == '/')
            {
                fin.get(c); // consume '/'
                state = NORMAL;
            }
        }
    }

    fin.close();



    return output;
}

