#include "Grid.hpp"
#include <fstream>

// O(W*H) time, O(W*H) aux space (the stored rows).
Grid Grid::load(const std::string& path) {
    Grid g;
    std::ifstream in(path);
    if (!in) { return g; }   // no rows; the caller reports the failure 

    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') { line.pop_back(); }
        if (line.empty()) { continue; }

        const int y = static_cast<int>(g.m_rows.size());
        for (size_t x = 0; x < line.size(); ++x) {
            if (line[x] == 'S') { g.m_start = Cell{ static_cast<int>(x), y }; }
            if (line[x] == 'G') { g.m_goal  = Cell{ static_cast<int>(x), y }; }
        }
        g.m_rows.push_back(line);
    }
    return g;
}

// O(1). Row first, then that row's own length.
bool Grid::inBounds(Cell c) const {
    if (c.y < 0 || c.y >= static_cast<int>(m_rows.size())) { return false; }
    return c.x >= 0 && c.x < static_cast<int>(m_rows[static_cast<size_t>(c.y)].size());
}

// O(1). (x, y) -> [y][x]. No other method indexes m_rows.
// precondition: inBounds(c), callers must check first.
char Grid::at(Cell c) const {
    return m_rows[static_cast<size_t>(c.y)][static_cast<size_t>(c.x)];
}

// O(1). inBounds first: && short-circuits, so at() is never called off-map.
bool Grid::isOpen(Cell c) const {
    return inBounds(c) && at(c) != '#';
}

// O(1): four fixed offsets, so at most four cells are ever built.
// Scan order is right, left, down, up.
std::vector<Cell> Grid::neighbours(Cell c) const {
    std::vector<Cell> out;
    const int dx[] = { 1, -1, 0,  0 };
    const int dy[] = { 0,  0, 1, -1 };
    for (int i = 0; i < 4; ++i) {
        const Cell n{ c.x + dx[i], c.y + dy[i] };
        if (isOpen(n)) { out.push_back(n); }
    }
    return out;
}

// O(1) each: found once during load, cached.
Cell Grid::start() const { return m_start; }
Cell Grid::goal()  const { return m_goal;  }

const std::vector<std::string>& Grid::rows() const { return m_rows; }
