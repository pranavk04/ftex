#include "parser.h"

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;

    std::string file = argv[1];

    parser p(file);

    p.parse();

    return 0;
}