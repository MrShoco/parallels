#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cstdlib>

class Table {
    public:

    friend void swap(Table& t, Table& t2);

    Table();
    Table(std::fstream &fin);
    Table(size_t n, size_t m);

    void setCell(size_t row, size_t column, bool value);
    bool getNextCellValue(size_t row, size_t column);
    size_t neighbourRow(size_t row, size_t direction);
    size_t neighbourColumn(size_t column, size_t direction);
    bool neighbourCellValue(size_t row, size_t column, 
                            int direction_row, int direction_column);
    vector<short>* getRow(size_t row_number) {
        return &grid_[row_number];
    }
    void setRow(size_t row_number, vector<short>& row) {
        grid_[row_number] = row;
    }

    size_t width();
    size_t height();
    void out();

    private:
    size_t n_, m_;
    std::vector<std::vector<short>> grid_;
};

#endif
