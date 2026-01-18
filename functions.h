#pragma once
#include<string>

std::string comment_remover();
std::string tokenize(const std::string &input);
void analyze(const std::string &tokenizedInput);
void left_recursion_remover();
std::string predictive_parser();
std::string generateIntermediateCode(const std::string &tokenizedInput);
std::string code_generator(const std::string &tacInput);
