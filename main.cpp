#include <bits/stdc++.h>
#include "functions.h"

using namespace std;

int main()
{
    string output_1 = comment_remover();
    string output_2 = tokenize(output_1);
    analyze(output_2);
    string output_3 = generateIntermediateCode(output_2);
    string final_code = code_generator(output_3);
    cout<<endl<<output_1<<endl<<endl;
    cout<<output_2<<endl<<endl;
    cout<<output_3<<endl<<endl;
    cout<<final_code<<endl<<endl;
}
