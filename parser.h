//
// Created by Pranav Konda on 2/25/22.
//

#ifndef FTEX_PARSER_H
#define FTEX_PARSER_H

#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <map>
#include "util.h"

enum envType {
    Def,
    Thm,
    Ex,
    Pf,
    Prob,
    Sol,
};

struct env {
    envType kind;
    explicit env(envType e): kind(e) {}
};

class parser {
private:
    std::string src;
    std::ifstream in;
    std::ofstream out;
    std::vector<env> envStack;
    std::map<std::string, std::string> aliases;

    static std::string getCloseStr(envType);
    std::string parseEnvs(const std::string&);
    static std::string parseMeta(const std::string&);
    std::string parseMath(const std::string&);

public:
    explicit parser(std::string);
    void parse();
};


#endif //UNTITLED_PARSER_H
