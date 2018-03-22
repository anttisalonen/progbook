#include <iostream>
#include <fstream>
#include <iterator>
#include <cstdio>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <cassert>

class Cell {
    public:
        Cell();
        bool only_one() const;
        int get_one() const;
        bool valid() const;
        bool eliminate(int i);
        std::vector<int> get() const;
        void set_value(int i);
        bool has(int i) const;
        int count() const;

    private:
        std::vector<int> values;
};

Cell::Cell() {
    values = {1, 2, 3, 4, 5, 6, 7, 8, 9};
}

bool Cell::only_one() const {
    return values.size() == 1;
}

int Cell::get_one() const {
    if(!only_one()) {
        throw std::runtime_error("get_one() called but multiple values exist");
    }
    return *values.begin();
}

bool Cell::valid() const {
    return values.size() != 0;
}

int Cell::count() const {
    return values.size();
}

bool Cell::eliminate(int i) {
    values.erase(std::remove(values.begin(), values.end(), i), values.end());
    return valid();
}

std::vector<int> Cell::get() const {
    return values;
}

void Cell::set_value(int i) {
    values.clear();
    values.push_back(i);
}

bool Cell::has(int i) const {
    return std::find(values.begin(), values.end(), i) != values.end();
}

class Puzzle {
    public:
        Puzzle(const std::vector<int> vec);
        bool solved() const;
        bool propagate(int i);
        bool valid() const;
        bool set_cell(int i, int value);
        Puzzle search(bool& found) const;
        void print() const;

    private:
        static std::vector<int> get_peers(int i);
        static std::array<std::array<int, 9>, 3> get_units(int self);
        static int get_index(int x, int y);
        bool check_units(int index);
        std::array<Cell, 81> cells;
};

Puzzle::Puzzle(const std::vector<int> vec) {
    int i = 0;
    for(const auto& v : vec) {
        if(v >= 1 && v <= 9) {
            bool ret = set_cell(i, v);
            if(!ret) {
                throw std::runtime_error("Invalid board");
            }
            i++;
        } else {
            i++;
        }
        if(i > 81) {
            throw std::runtime_error("Too many values");
        }
    }
    if(i != 81) {
        throw std::runtime_error("Not enough values");
    }
}

bool Puzzle::solved() const {
    for(const auto& p : cells) {
        if(!p.only_one()) {
            return false;
        }
    }
    return true;
}

bool Puzzle::set_cell(int i, int value) {
    cells.at(i).set_value(value);
    return propagate(i);
}

Puzzle Puzzle::search(bool& found) const {
    std::vector<int> open;
    if(solved()) {
        found = true;
        return *this;
    }

    for(int i = 0; i < 81; i++) {
        if(!cells.at(i).valid()) {
            found = false;
            return *this;
        }
        if(!cells.at(i).only_one()) {
            open.push_back(i);
        }
    }

    std::sort(open.begin(), open.end(),
            [&](int i1, int i2) {
            return cells.at(i1).count() <
            cells.at(i2).count();
            });

    auto o = open.at(0);
    auto values = cells.at(o).get();
    for(auto v : values) {
        Puzzle alt = *this;
        bool ret = alt.set_cell(o, v);
        if(ret) {
            bool alt_found;
            alt = alt.search(alt_found);
            if(alt_found) {
                found = true;
                return alt;
            }
        }
    }
    found = false;
    return *this;
}

void Puzzle::print() const {
    bool s = solved();
    for(int i = 0; i < 81; i++) {
        if(s) {
            printf("%d", cells.at(i).get_one());
        } else {
            for(int v = 1; v <= 9; v++) {
                if(cells.at(i).has(v)) {
                    printf("%d", v);
                } else {
                    printf(" ");
                }
            }
            printf(" ");
        }
        if(i % 9 == 8) {
            printf("\n");
            if(i % 27 == 26) {
                printf("---\n");
            }
        } else if(i % 3 == 2) {
            printf("|");
        }
    }
    printf("\n");
}

bool Puzzle::propagate(int i) {
    const auto& p = cells.at(i);

    auto one = p.get_one();
    auto peers = Puzzle::get_peers(i);
    for(const auto& peer : peers) {
        if(cells.at(peer).has(one)) {
            bool still_valid = cells.at(peer).eliminate(one);
            if(!still_valid)
                return false;
            bool has_one = cells.at(peer).only_one();
            if(has_one) {
                bool ret = propagate(peer);
                if(!ret)
                    return false;
            }
            bool ret = check_units(peer);
            if(!ret)
                return false;
        }
    }
    return true;
}

bool Puzzle::check_units(int index) {
    auto units = get_units(index);
    for(const auto& unit : units) {
        for(int i = 1; i <= 9; i++) {
            int found = -1;
            for(auto peer : unit) {
                if(cells[peer].has(i)) {
                    if(found != -1) {
                        found = -1;
                        break;
                    }
                    found = peer;
                }
            }
            if(found != -1) {
                bool ret = set_cell(found, i);
                if(!ret)
                    return false;
            }
        }
    }
    return true;
}

std::array<std::array<int, 9>, 3> Puzzle::get_units(int self) {
    std::array<std::array<int, 9>, 3> ret;

    int col = self % 9;
    for(int i = 0; i < 9; i++) {
        int ind = Puzzle::get_index(col, i);
        ret[0][i] = ind;
    }

    int row = self / 9;
    for(int i = 0; i < 9; i++) {
        int ind = Puzzle::get_index(i, row);
        ret[1][i] = ind;
    }

    int xc = row / 3;
    int yc = col / 3;
    for(int j = 0; j < 3; j++) {
        for(int i = 0; i < 3; i++) {
            int ind = Puzzle::get_index(i + yc * 3, j + xc * 3);
            ret[2][i * 3 + j] = ind;
        }
    }

    return ret;
}

bool Puzzle::valid() const {
    for(const auto& p : cells) {
        if(!p.valid())
            return false;
    }
    return true;
}

std::vector<int> Puzzle::get_peers(int self) {
    std::vector<int> ret;
    auto val = get_units(self);
    for(auto& v : val) {
        for(auto& i : v) {
            ret.push_back(i);
        }
    }

    ret.erase(std::remove(ret.begin(), ret.end(), self), ret.end());

    return ret;
}

int Puzzle::get_index(int x, int y) {
    assert(x < 9);
    assert(y < 9);
    return y * 9 + x;
}

int main(int argc, char** argv)
{
    std::ifstream ifs(argv[1]);
    std::string contents((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));

    std::vector<Puzzle> puzzles;

    int buf_location = 0;
    std::vector<int> my_buf;
    for(auto c : contents) {
        if(c >= '1' && c <= '9') {
            my_buf.push_back(c - '0');
            buf_location++;
        } else if(c == '.' || c == '0') {
            my_buf.push_back(0);
            buf_location++;
        }
        if(buf_location == 81) {
            puzzles.push_back(Puzzle(my_buf));
            my_buf.clear();
            buf_location = 0;
        }
    }

    for(auto& p : puzzles) {
        if(!p.valid()) {
            std::cerr << "Not valid\n";
        } else {
            bool found;
            p = p.search(found);
            std::cout << "Solved: " << found << "\n";
        }
        p.print();
    }

    return 0;
}
