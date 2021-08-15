#include <errno.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <string.h>

int main(int argc, char * argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << ": libname\n";
        return -1;
    }
    std::regex reg(std::string("^([a-f0-9]+)\\-[0-9a-f]+\\s+.+\\s+(\\d+)\\s+.+\\s+\\d+\\s+") + std::string(argv[1]) + std::string("\\s*$"));

    /* open /proc/self/maps */
    std::string ln;
    std::ifstream m_file("/proc/self/maps");
    if (!m_file) {
        std::cerr << "Unable to open /proc/self/maps " << strerror(errno) << "\n";
        return -1;
    }
    while (getline(m_file, ln)) {
        std::smatch sm;
        if (std::regex_match (ln,sm, reg)) {
            if (std::stoi(sm[2]) == 0) {
                std::cout << sm[1];
                return 0;
            }
        }
    }
    return -1;
}
