#include <iostream>
#include <thread>
#include <vector>
#include <string>

using namespace std;

int parse_args(int argc, char* argv[], vector<size_t*>& args) {
    if (static_cast<size_t>(argc) != args.size() + 1) {
        cout << "Warning: " << args.size() << " arguments are required! Given: " << argc - 1 << endl;
        return -1;
    }

    for (size_t i = 0; i < args.size(); ++i) {
        *args[i] = strtoull(argv[i + 1], nullptr, 10);
    }

    return 0;
}