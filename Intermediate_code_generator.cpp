#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include "functions.h"
using namespace std;

int tempCount = 1;

// Generate a new temporary variable
string newTemp() {
    return "t" + to_string(tempCount++);
}

// Trim leading and trailing spaces
string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Process expression and generate three-address code
string processExpression(const string &expr, vector<string> &output) {
    // Tokenize by space
    istringstream ss(expr);
    vector<string> tokens;
    string token;
    while (ss >> token) tokens.push_back(token);

    // Handle * and / first
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "*" || tokens[i] == "/") {
            string t = newTemp();
            output.push_back(t + " = " + tokens[i-1] + " " + tokens[i] + " " + tokens[i+1]);
            tokens[i-1] = t;
            tokens.erase(tokens.begin() + i, tokens.begin() + i+2);
            i = 0;
        }
    }

    // Handle + and -
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "+" || tokens[i] == "-") {
            string t = newTemp();
            output.push_back(t + " = " + tokens[i-1] + " " + tokens[i] + " " + tokens[i+1]);
            tokens[i-1] = t;
            tokens.erase(tokens.begin() + i, tokens.begin() + i+2);
            i = 0;
        }
    }

    return tokens[0]; // final result
}

string generateIntermediateCode(const string &tokenizedInput) {
    tempCount = 1; // reset temp variable counter
    vector<string> output;
    istringstream ss(tokenizedInput);
    string stmt;

    // Split statements by ';'
    while (getline(ss, stmt, ';')) {
        stmt = trim(stmt);
        if (stmt.empty()) continue;

        size_t eqPos = stmt.find('=');
        if (eqPos == string::npos) continue;

        string lhs = trim(stmt.substr(0, eqPos));
        string rhs = trim(stmt.substr(eqPos + 1));

        // Generate three-address code for rhs
        string result = processExpression(rhs, output);

        // Add final assignment to original variable
        output.push_back(lhs + " = " + result);
    }

    // Combine output into a single string
    string finalOutput;
    for (size_t i = 0; i < output.size(); i++) {
        finalOutput += output[i];
        if (i != output.size() - 1) finalOutput += "\n";
    }

    return finalOutput;
}

