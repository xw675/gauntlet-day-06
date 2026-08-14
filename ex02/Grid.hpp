#pragma once
#include <string>
#include <vector>

struct Cell { int x, y; };

class Grid {
public:
    static Grid load(const std::string& path);
    bool inBounds(Cell c) const;
    bool isOpen(Cell c) const;                   // in bounds and not '#'
    std::vector<Cell> neighbours(Cell c) const;  // walkable 4-neighbours
    Cell start() const;   // the 'S'
    Cell goal()  const;   // the 'G'

    const std::vector<std::string>& rows() const;

private:
    char at(Cell c) const;

    std::vector<std::string> m_rows;
    Cell m_start{-1, -1};    // (-1,-1) means the marker was not in the file
    Cell m_goal {-1, -1};
};
