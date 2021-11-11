#include <fstream>
#include <iostream>
#include <string>

#include "script.h"

using namespace std;

int main(void)
{
    Script sc;
    string cmd;
    while (true) {
        cout << ": ";
        getline(cin, cmd);
        if (cmd == "exit") {
            break;
        } else if (cmd == "reset") {
            sc.reset();
        } else if (cmd == "ids") {
            sc.printIds();
        } else if (cmd == "funs") {
            sc.printFuns();
        } else if (cmd.substr(0, 4) == "run ") {
            ifstream file(cmd.substr(4) + ".cmm");
            if (file.is_open()) {
                file.seekg(0, ios::end);
                int len = file.tellg();
                file.seekg(0, ios::beg);
                char *buf = new char[len + 1];
                file.read(buf, len);
                buf[file.gcount()] = '\0';
                file.close();
                sc.exec(buf);
                delete[] buf;
            } else {
                cerr << "Cannot open file " << cmd.substr(4) + ".cmm" << endl;
            }
        } else {
            sc.exec(cmd.c_str());
        }
    }
    return 0;
}
