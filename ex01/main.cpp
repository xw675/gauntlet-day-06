#include "Grid.hpp"
#include <iostream>
#include <string>

// O(1). Prints a cell as "(x,y)".
static void printCell(Cell c) {
    std::cout << '(' << c.x << ',' << c.y << ')';
}

// O(W*H + Q): one pass to load and print the map, then O(1) per queried cell.
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "usage: ./grid <map> <x> <y> [<x> <y> ...]\n";
        return 1;
    }

    const Grid grid = Grid::load(argv[1]);
    if (grid.rows().empty()) {
        std::cout << "cannot read map: " << argv[1] << '\n';
        return 1;
    }

    for (const std::string& row : grid.rows()) {
        std::cout << row << '\n';
    }

    std::cout << "start S = ";
    printCell(grid.start());
    std::cout << '\n';

    std::cout << "goal  G = ";
    printCell(grid.goal());
    std::cout << '\n';

    for (int i = 2; i + 1 < argc; i += 2) {
        const Cell q{ std::stoi(argv[i]), std::stoi(argv[i + 1]) };
        std::cout << "neighbours(" << q.x << ',' << q.y << "):";
        for (Cell n : grid.neighbours(q)) {
            std::cout << ' ';
            printCell(n);
        }
        std::cout << '\n';
    }

    return 0;
}
