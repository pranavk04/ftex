//
// Created by Pranav Konda on 2/25/22.
//

#include "parser.h"

#include <utility>
#include <regex>

parser::parser(std::string filename) {
    src = std::move(filename);
    in = std::ifstream(src + ".ftex");
    out = std::ofstream(src + ".tex");

    std::ifstream conf("maps.cfg");
    std::string line;

    while(std::getline(conf, line)) {
        auto idx = line.find(' ');

        std::string head = line.substr(0, idx);
        std::string tail = line.substr(idx + 1, line.length() - idx - 1);

        aliases.emplace(std::make_pair(head, tail));
    }
}

std::string parser::getCloseStr(envType e) {
    std::string res;

    switch(e) {
        case envType::Def:
            res = "\\end{definition}";
            break;
        case envType::Thm:
            res = "\\end{theorem}";
            break;
        case envType::Ex:
            res = "\\end{example}";
            break;
        case envType::Pf:
            res = "\\end{proof}";
            break;
        case envType::Prob:
            res = "\\end{problem}";
            break;
        case envType::Sol:
            res = "\\end{soln}";
            break;
        default:
            std::cerr << "Not sure how this happened. Check the source code." << std::endl;
    }

    return res;
}

std::string parser::parseEnvs(const std::string& data) {
    std::string res;

    if (data == "!thm") {
        res = "\\begin{theorem}{}{}";
        envStack.emplace_back(env(envType::Thm));
    }
    else if (data == "!ex") {
        res = "\\begin{example}{}{}";
        envStack.emplace_back(env(envType::Ex));
    }
    else if (data == "!pf") {
        res = "\\begin{proof}";
        envStack.emplace_back(env(envType::Pf));
    }
    else if (data == "!prob") {
        res = "\\begin{problem}";
        envStack.emplace_back(env(envType::Prob));
    }
    else if (data == "!sol") {
        res = "\\begin{sol}";
        envStack.emplace_back(env(envType::Sol));
    }
    else if (data == "!end") {
        if (envStack.empty())
            throw std::invalid_argument("You have too many !ends or forgot to close an environment. Please check your code!");
        else {
            res = getCloseStr(envStack.back().kind);
            envStack.pop_back();
        }
    }
    else {
        throw std::invalid_argument(data + " is not a valid environment.");
    }

    return res;
}

std::string parser::parseMeta(const std::string& data) {
    std::string res;

    auto idx = data.find(' '); // add support for unnamed sections later
    if (idx < 0)
        throw std::invalid_argument("You need a title for your section!");

    std::string head = data.substr(0, idx);
    std::string tail = data.substr(idx + 1, data.length() - idx - 1);

    if (head == "@section")
        res = "\\section{" + tail + "}";
    else if (head == "@subsection")
        res = "\\subsection{" + tail + "}";

    std::cout << res << std::endl;

    return res;
}

void parser::parse() {
    // change this to read from a ftex.conf file instead
    std::string cls = "\\documentclass{pranav2} \n";
    std::string title;
    std::string startOptions = "\\begin{document} \n \\maketitle \n";

    std::getline(in, title);
    title = "\\title{" + title + "} \n";

    out << cls << title << startOptions;

    std::string data;
    while(std::getline(in, data)) {
        if (data.empty())
            out << std::endl;
        else if (data[0] == '!')
            out << parseEnvs(data) << std::endl;
        else if (data[0] == '@')
            out << parseMeta(data) << std::endl;
        else{
            data = parseMath(data);
            out << data << std::endl;
        }
    }
    out << "\\end{document}";

    std::cout << "File written to " + src + ".tex.";
}

std::string parser::parseMath(const std::string &data) {
   std::string res = data;

   std::cout << aliases.size() << std::endl;
   for (const auto& p : aliases)
       util::replace_all(res, p.first, p.second); // by the reasonable(?) logic that people won't try to use math commands without $$.

   // only check math strings for parenthesis and bracket replacement.
   std::regex mathMode(R"(\\\[(.+?)\\\])");
//   std::regex inlineMath(R"((?<![\\])\$(.+?)\$)");

   std::smatch exprs;

   std::regex_search(res, exprs, mathMode);

   for (auto e : exprs) {
       std::string insert = e.str();

       util::replace_all(insert, "(", "\\left (");
       util::replace_all(insert, ")", "\\right )");

       util::replace_all(insert, "\\{", "\\left \\{");
       util::replace_all(insert, "\\}", "\\right \\}");

       util::replace_all(res, e.str(), insert);
   }

   return res;
}
