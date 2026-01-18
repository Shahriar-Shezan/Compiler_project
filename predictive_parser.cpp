#include <bits/stdc++.h>
using namespace std;

map<string, vector<vector<string>>> grammar;
map<string, set<string>> FIRST, FOLLOW;
map<string, map<string, vector<string>>> table;

set<string> nonterminals;
set<string> terminals;

bool isTerminal(const string &s)
{
    return nonterminals.find(s) == nonterminals.end() && s != "ε";
}

/* ---------------- FIRST ---------------- */

set<string> computeFIRST(string X)
{
    if (!FIRST[X].empty())
        return FIRST[X];

    for (auto &prod : grammar[X])
    {
        bool allEps = true;

        for (auto &symbol : prod)
        {
            if (symbol == "ε")
            {
                FIRST[X].insert("ε");
                break;
            }

            if (isTerminal(symbol))
            {
                FIRST[X].insert(symbol);
                allEps = false;
                break;
            }

            auto sub = computeFIRST(symbol);
            bool hasEps = false;

            for (auto &t : sub)
            {
                if (t == "ε") hasEps = true;
                else FIRST[X].insert(t);
            }

            if (!hasEps)
            {
                allEps = false;
                break;
            }
        }

        if (allEps)
            FIRST[X].insert("ε");
    }

    return FIRST[X];
}

/* ---------------- FOLLOW ---------------- */

void computeFOLLOW(const string &start)
{
    FOLLOW[start].insert("$");

    bool changed = true;

    while (changed)
    {
        changed = false;

        for (auto &g : grammar)
        {
            string A = g.first;

            for (auto &prod : g.second)
            {
                for (int i = 0; i < prod.size(); i++)
                {
                    string B = prod[i];

                    if (isTerminal(B)) continue;

                    if (i + 1 < prod.size())
                    {
                        string next = prod[i + 1];

                        if (isTerminal(next))
                        {
                            changed |= FOLLOW[B].insert(next).second;
                        }
                        else
                        {
                            for (auto &x : FIRST[next])
                                if (x != "ε")
                                    changed |= FOLLOW[B].insert(x).second;

                            if (FIRST[next].count("ε"))
                                for (auto &x : FOLLOW[A])
                                    changed |= FOLLOW[B].insert(x).second;
                        }
                    }
                    else
                    {
                        for (auto &x : FOLLOW[A])
                            changed |= FOLLOW[B].insert(x).second;
                    }
                }
            }
        }
    }
}

/* ---------------- TABLE ---------------- */

void buildParsingTable()
{
    for (auto &g : grammar)
    {
        string A = g.first;

        for (auto &prod : g.second)
        {
            bool epsAll = true;

            for (auto &X : prod)
            {
                if (X == "ε")
                {
                    for (auto &b : FOLLOW[A])
                        table[A][b] = prod;
                    epsAll = false;
                    break;
                }

                if (isTerminal(X))
                {
                    table[A][X] = prod;
                    epsAll = false;
                    break;
                }

                for (auto &t : FIRST[X])
                    if (t != "ε")
                        table[A][t] = prod;

                if (!FIRST[X].count("ε"))
                {
                    epsAll = false;
                    break;
                }
            }

            if (epsAll)
            {
                for (auto &b : FOLLOW[A])
                    table[A][b] = {"ε"};
            }
        }
    }
}

/* ---------------- PARSER ---------------- */

bool parseString(vector<string> input, const string &start)
{
    input.push_back("$");

    stack<string> st;
    st.push("$");
    st.push(start);

    int i = 0;

    while (!st.empty())
    {
        string X = st.top();
        string a = input[i];

        if (X == a)
        {
            st.pop();
            i++;
        }
        else if (isTerminal(X))
        {
            return false;
        }
        else
        {
            if (table[X].count(a) == 0)
                return false;

            auto prod = table[X][a];
            st.pop();

            if (!(prod.size() == 1 && prod[0] == "ε"))
                for (int j = prod.size() - 1; j >= 0; j--)
                    st.push(prod[j]);
        }
    }

    return i == input.size();
}

/* ---------------- MAIN ---------------- */


string predictive_parser()
{
    grammar.clear();
    FIRST.clear();
    FOLLOW.clear();
    table.clear();
    nonterminals.clear();
    terminals.clear();
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cin.ignore();

    cout << "Enter grammar (space separated):\n";

    for (int i = 0; i < n; i++)
    {
        string line;
        getline(cin, line);

        stringstream ss(line);
        string lhs, arrow;
        ss >> lhs >> arrow;

        nonterminals.insert(lhs);

        vector<string> symbols;
        string sym;

        while (ss >> sym)
        {
            if (sym == "|")
            {
                grammar[lhs].push_back(symbols);
                symbols.clear();
            }
            else
            {
                symbols.push_back(sym);
            }
        }

        grammar[lhs].push_back(symbols);
    }

    string start;
    cout << "Enter start symbol: ";
    cin >> start;

    for (auto &nt : nonterminals)
        computeFIRST(nt);

    computeFOLLOW(start);
    buildParsingTable();

    cout << "\nLL(1) Parsing Table:\n";
    for (auto &r : table)
        for (auto &c : r.second)
            cout << "M[" << r.first << "," << c.first << "] = "
                 << r.first << " -> ";

    cout << "\n\nEnter input string (space separated): ";
    cin.ignore();
    string inputLine;
    getline(cin, inputLine);

    stringstream ss(inputLine);
    vector<string> input;
    string tok;

    while (ss >> tok)
        input.push_back(tok);

    if (parseString(input, start))
        return "\n✅ STRING ACCEPTED\n";
    else
        return  "\n❌ STRING REJECTED\n";
}
