#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "functions.h"

using namespace std;

// Simple code generator
string code_generator(const string &tacInput) {
    istringstream ss(tacInput);
    string line;
    string output;

    // We'll use one main register R0 for calculations
    while (getline(ss, line)) {
        if (line.empty()) continue;

        string lhs, rhs;
        size_t eq = line.find('=');
        lhs = line.substr(0, eq);
        rhs = line.substr(eq + 1);

        // Trim spaces
        lhs.erase(0, lhs.find_first_not_of(" \t"));
        lhs.erase(lhs.find_last_not_of(" \t") + 1);
        rhs.erase(0, rhs.find_first_not_of(" \t"));
        rhs.erase(rhs.find_last_not_of(" \t") + 1);

        // Check if RHS is a simple number
        bool isNumber = true;
        for (char c : rhs) if (!isdigit(c) && c != '.') isNumber = false;

        if (isNumber) {
            output += "MOVF #" + rhs + ", " + lhs + "\n";
        } else {
            // Complex expression: parse basic binary operations
            // We'll assume a single operator per line for simplicity
            string op;
            size_t pos = string::npos;
            if ((pos = rhs.find('+')) != string::npos) op = "+";
            else if ((pos = rhs.find('-')) != string::npos) op = "-";
            else if ((pos = rhs.find('*')) != string::npos) op = "*";
            else if ((pos = rhs.find('/')) != string::npos) op = "/";

            if (!op.empty()) {
                string left = rhs.substr(0, pos);
                string right = rhs.substr(pos + 1);
                // Trim spaces
                left.erase(0, left.find_first_not_of(" \t"));
                left.erase(left.find_last_not_of(" \t") + 1);
                right.erase(0, right.find_first_not_of(" \t"));
                right.erase(right.find_last_not_of(" \t") + 1);

                // Load left operand
                output += "MOVF " + left + ", R0\n";

                // Apply operation
                if (op == "+") output += "ADD " + right + ", R0\n";
                if (op == "-") output += "SUB " + right + ", R0\n";
                if (op == "*") output += "MULF " + right + ", R0\n";
                if (op == "/") output += "DIVF " + right + ", R0\n";

                // Store result in lhs
                output += "MOV R0, " + lhs + "\n";
            } else {
                // Single variable assignment
                output += "MOV " + rhs + ", " + lhs + "\n";
            }
        }
    }

    return output;
}

