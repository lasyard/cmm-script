#include <fstream>
#include <iostream>
#include <string>

#include "script.h"

using namespace std;

// 读取文件并执行脚本
int run_file(Script &sc, const std::string &filename)
{
    ifstream file(filename);
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
        return EXIT_SUCCESS;
    }
    cerr << "Cannot open file " << filename << endl;
    return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
    Script sc;
    if (argc > 1) {
        return run_file(sc, string(argv[1]) + ".cmm");
    }
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
            run_file(sc, cmd.substr(4) + ".cmm");
        } else {
            sc.exec(cmd.c_str());
        }
    }
    return 0;
}
