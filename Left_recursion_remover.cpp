#include <iostream>
#include <vector>
#include <sstream>
#include "functions.h"
using namespace std;
void left_recursion_remover()
{
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cin.ignore();
    vector<string> lhs(n), rhs(n);
    cout << "Enter productions (e.g. E->E+T|T):" << endl;
    for (int i = 0; i < n; i++)
    {
        getline(cin, lhs[i]);
        size_t pos = lhs[i].find("->");
        rhs[i] = lhs[i].substr(pos + 2);
        lhs[i] = lhs[i].substr(0, pos);
    }
    cout << "\nAfter removing left recursion:\n";
    for (int i = 0; i < n; i++)
    {
        string A = lhs[i];
        string right = rhs[i];
        vector<string> alpha, beta;
        stringstream ss(right);
        string token;
        while (getline(ss, token, '|'))
        {
            if (token[0] == A[0])
                alpha.push_back(token.substr(1));
            else
                beta.push_back(token);
        }

        if (alpha.empty())
        {
            cout << A << " -> " << right << endl;
        }
        else
        {
            string Aprime = A + "'";
            cout << A << " -> ";
            for (size_t j = 0; j < beta.size(); j++)
            {
                cout << beta[j] << Aprime;
                if (j != beta.size() - 1) cout << " | ";
            }
            cout << endl;

            cout << Aprime << " -> ";
            for (size_t j = 0; j < alpha.size(); j++)
            {
                cout << alpha[j] << Aprime << " | ";
            }
            cout << "ε" << endl;
        }
    }
}
