#include <iostream>

using namespace std;

void build_matrix(vector<vector<double> >* matrix, uint64_t matrix_size) {
    for (uint64_t i = 0; i < matrix_size; i++) {
        vector<double> row;

        for (uint64_t j = 0; j < matrix_size; j++) {
            row.push_back(i == j ? (static_cast<double>(i + 1) / static_cast<double>(matrix_size)) : 0);
        }

        (*matrix).push_back(row);
    }
}

void print_matrix(vector<vector<double> >* matrix) {
    uint64_t size = (*matrix).size();

    for (uint64_t i = 0; i < size; i++) {
        for (uint64_t j = 0; j < (*matrix)[i].size(); j++) {
            printf("%.3f ", (*matrix)[i][j]);
        }

        cout << "\n" << endl;
    }
}
