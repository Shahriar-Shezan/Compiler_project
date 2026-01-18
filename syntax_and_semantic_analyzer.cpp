#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include "functions.h"

using namespace std;

// Symbol table for semantic analysis
map<string, string> semanticSymtab;

// Helper to check if string is a number (int or float)
bool isNumber(const string &s)
{
    bool dotSeen = false;
    if (s.empty()) return false;

    for (char c : s)
    {
        if (c == '.')
        {
            if (dotSeen) return false; // more than one dot → invalid
            dotSeen = true;
        }
        else if (!isdigit(c))
            return false;
    }
    return true;
}

void analyze(const string &tokenizedInput)
{
    istringstream ss(tokenizedInput);
    string token;
    int lineNum = 1;

    string lhs;           // left-hand side of assignment
    bool expectLHS = true; // are we expecting a new variable on LHS?
    bool inExpression = false;

    while (ss >> token)
    {
        if (token.substr(0, 2) == "id") // identifier
        {
            if (expectLHS)
            {
                lhs = token;
                // Declare LHS variable if not declared yet
                if (semanticSymtab.find(lhs) == semanticSymtab.end())
                    semanticSymtab[lhs] = "int"; // default type
                expectLHS = false;
                inExpression = false; // '=' not yet seen
            }
            else
            {
                // Semantic check: RHS identifier must exist
                if (semanticSymtab.find(token) == semanticSymtab.end())
                    cout << "Semantic Error: Undeclared variable "
                         << token << " at line " << lineNum << endl;
                inExpression = true;
            }
        }
        else if (isNumber(token))
        {
            inExpression = true; // number is valid in expression
        }
        else if (token == "=")
        {
            if (!expectLHS) inExpression = true;
            else
                cout << "Syntax Error: Unexpected '=' at line " << lineNum << endl;
        }
        else if (token == "+" || token == "-" || token == "*" || token == "/")
        {
            if (!inExpression)
                cout << "Syntax Error: Unexpected operator " << token
                     << " at line " << lineNum << endl;
        }
        else if (token == ";")
        {
            // End of statement, reset for next line
            expectLHS = true;
            inExpression = false;
            lineNum++;
        }
        else
        {
            cout << "Syntax Error: Unknown token '" << token
                 << "' at line " << lineNum << endl;
        }
    }

    // Print semantic table at the end
    cout << "\nSemantic Analysis Complete.\n";
    cout << "Symbol Table:\n";
    for (auto &p : semanticSymtab)
        cout << p.first << " : " << p.second << endl;
}
