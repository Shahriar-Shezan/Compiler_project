#include <iostream>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>
#include "functions.h"

using namespace std;

#define MAX 50

/* Symbol table */
char symtab[MAX][20];
int symCount = 0;

/* Check keyword */
int isKeyword(char *w)
{
    const char *kw[] = {"int", "float", "return", "printf", "main", "include"};
    for (int i = 0; i < 6; i++)
        if (strcmp(w, kw[i]) == 0)
            return 1;
    return 0;
}

/* Get ID number */
int getId(char *w)
{
    for (int i = 0; i < symCount; i++)
        if (strcmp(symtab[i], w) == 0)
            return i + 1;

    strcpy(symtab[symCount], w);
    return ++symCount;
}

/* Operator check */
int isOp(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=');
}

/* ============================= */
/*        TOKENIZER FUNCTION     */
/* ============================= */

string tokenize(const string &input)
{
    symCount = 0;   // reset symbol table
    vector<string> tokens; // store tokens
    size_t i = 0;

    while (i < input.length())
    {
        char c = input[i];

        // Skip whitespace
        if (isspace(c)) { ++i; continue; }

        // Skip preprocessor and braces
        if (c == '#' || c == '{' || c == '}') { ++i; continue; }

        // Identifier
        if (isalpha(c))
        {
            string id;
            while (i < input.length() && isalnum(input[i]))
                id += input[i++];

            char buf[50]; strcpy(buf, id.c_str());
            if (isKeyword(buf))
            {
                // Skip everything until semicolon
                while (i < input.length() && input[i] != ';') i++;
                if (i < input.length() && input[i] == ';') i++;
                continue;
            }

            int idnum = getId(&id[0]);
            tokens.push_back("id" + to_string(idnum));
        }
        // Number (int or float)
        else if (isdigit(c))
        {
            string num;
            while (i < input.length() && (isdigit(input[i]) || input[i]=='.'))
                num += input[i++];
            tokens.push_back(num);
        }
        // Operator
        else if (isOp(c))
        {
            tokens.push_back(string(1, c));
            ++i;
        }
        // Semicolon or comma
        else if (c == ';' || c == ',')
        {
            tokens.push_back(";");
            ++i;
        }
        else
        {
            // Skip unknown characters
            ++i;
        }
    }

    // Join tokens into single string separated by spaces
    string output;
    for (size_t j = 0; j < tokens.size(); ++j)
    {
        output += tokens[j];
        if (j + 1 < tokens.size()) output += " ";
    }

    return output;
}
