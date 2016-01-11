#include "table.h"

Table::Table() {
}

Table::Table(std::fstream &fin) {
    std::string row;

    while(std::getline(fin, row)) {
        grid_.push_back(std::vector<short>());
        for (size_t i = 0; i < row.size(); i++) {
            if (row[i] == '0')
                grid_.back().push_back(false);
            else if (row[i] == '1')
                grid_.back().push_back(true);
        }
    }
    n_ = grid_.size(); m_ = grid_[0].size();
}

Table::Table(size_t n, size_t m) {
    for (size_t i = 0; i < n; i++) {
        grid_.push_back(std::vector<short>());
        for (size_t j = 0; j < m; j++) {
            if (rand() % 2 == 0)
                grid_.back().push_back(false);
            else
                grid_.back().push_back(true);
        }
    }
    n_ = n; m_ = m;
}

void Table::setCell(size_t row, size_t column, bool value) {
    grid_[row][column] = value;
}


size_t neighbourRow(size_t row, size_t direction) {
    return (row + height() + direction) % height();
}

size_t neighbourColumn(size_t column, size_t direction) {
    return (column + width() + direction) % width();
}

bool Table::neighbourCellValue(size_t row, size_t column,
                               int direction_row, int direction_column) {
    return grid_[neighbourRow][neighbourColumn];
}

bool Table::getNextCellValue(size_t row, size_t column) {
    int alive_neighbours = 0;
    int dx[] = {1, 1, 0, -1, -1, -1, 0, 1};
    int dy[] = {0, -1, -1, -1, 0, 1, 1, 1};
    for (size_t i = 0; i < 8; i++) {
        if (neighbourCellValue(row, column, dx[i], dy[i]))
            alive_neighbours++;
    }

    return (grid_[row][column] == true && alive_neighbours == 2) || (alive_neighbours == 3);
}

void Table::out() {
    for (size_t i = 0; i < grid_.size(); i++) {
        for (size_t j = 0; j < grid_[i].size(); j++) {
            if (grid_[i][j]) {
                std::cout << "*";
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
}

size_t Table::height() {
    return n_;
}

size_t Table::width() {
    return m_;
}

void swap(Table& t, Table& t2) {
    using std::swap;

    t.grid_.swap(t2.grid_);
}
