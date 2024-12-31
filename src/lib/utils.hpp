#include <iostream>
#include <thread>
#include <vector>
#include <string>

using namespace std;

void parse_args(int argc, char* argv[], vector<uint64_t*>& args) {
    if (argc != args.size() + 1) {
        cout << "Error: " << args.size() << " arguments are required! Given: " << argc - 1 << endl;
        exit(-1);
    }

    for (size_t i = 0; i < args.size(); ++i) {
        *args[i] = strtoull(argv[i + 1], nullptr, 10);
    }
}