#include "Grid.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <cmath>

struct CellLess {
    bool operator()(const Cell& a, const Cell& b) const {
        return a.y != b.y ? a.y < b.y : a.x < b.x;
    }
};

static bool same(Cell a, Cell b) { return a.x == b.x && a.y == b.y; }

static int h(Cell a, Cell b) { return std::abs(a.x - b.x) + std::abs(a.y - b.y); }

struct Node { Cell cell; int f; };                        // f = g + h
struct ByF { bool operator()(const Node& a, const Node& b) const { return a.f > b.f; } };

using ScoreMap = std::map<Cell, int,  CellLess>;
using FromMap  = std::map<Cell, Cell, CellLess>;
static const int INF = 1000000000;

static int gOf(const ScoreMap& g, Cell c) {
    const ScoreMap::const_iterator it = g.find(c);
    return it == g.end() ? INF : it->second;
}

// O(P) time and space, P = path length. Walks cameFrom backwards from the goal,
// then flips it.
static std::vector<Cell> reconstruct(const FromMap& cameFrom, Cell start, Cell goal) {
    std::vector<Cell> rev;
    Cell cur = goal;
    while (!same(cur, start)) {
        rev.push_back(cur);
        const FromMap::const_iterator it = cameFrom.find(cur);
        if (it == cameFrom.end()) { return {}; }   // chain broken; treat as no path
        cur = it->second;
    }
    rev.push_back(start);

    std::vector<Cell> path;
    path.reserve(rev.size());
    for (size_t i = rev.size(); i > 0; --i) { path.push_back(rev[i - 1]); }
    return path;   // start .. goal
}

// A* with the Manhattan heuristic.
// O(E log V) time, O(V) aux space; V = open cells, E = 4V on a 4-directional grid.
static std::vector<Cell> astar(const Grid& grid) {
    const Cell start = grid.start();
    const Cell goal  = grid.goal();
    if (!grid.isOpen(start) || !grid.isOpen(goal)) { return {}; }

    ScoreMap g;                                               // best cost so far per cell
    FromMap  cameFrom;
    std::priority_queue<Node, std::vector<Node>, ByF> open;   // min-heap on f = g + h

    // every cell's g starts at "infinity" — here, an absent map entry (see gOf)
    g[start] = 0;                                    // paid nothing yet; forget this and the search is dead
    open.push({start, h(start, goal)});
    while (!open.empty()) {
        Node cur = open.top(); open.pop();            // cheapest-looking cell in the frontier
        if (same(cur.cell, goal)) {
            return reconstruct(cameFrom, start, goal);   // popped = its cost is settled
        }
        for (Cell nb : grid.neighbours(cur.cell)) {
            int tentativeG = gOf(g, cur.cell) + 1;     // cost to reach nb via cur (uniform step cost 1)
            if (tentativeG < gOf(g, nb)) {              // strictly cheaper than any known route to nb
                g[nb] = tentativeG;                     // record the better route
                cameFrom[nb] = cur.cell;                // and who found it, for reconstruction
                open.push({nb, tentativeG + h(nb, goal)});   // f = paid so far + optimistic remainder
            }
        }
    }
    return {};   // frontier drained without reaching the goal: no path exists
}

// O(E log V) overall, dominated by the search.
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "usage: ./astar <map>\n";
        return 1;
    }

    const Grid grid = Grid::load(argv[1]);
    if (grid.rows().empty()) {
        std::cout << "cannot read map: " << argv[1] << '\n';
        return 1;
    }

    const std::vector<Cell> path = astar(grid);
    if (path.empty()) {
        std::cout << "no path\n";
        return 0;
    }

    std::vector<std::string> out = grid.rows();
    for (size_t i = 0; i < path.size(); ++i) {
        char& ch = out[static_cast<size_t>(path[i].y)][static_cast<size_t>(path[i].x)];
        if (ch != 'S' && ch != 'G') { ch = '*'; }
    }

    for (const std::string& row : out) { std::cout << row << '\n'; }
    std::cout << "path length: " << path.size() - 1 << '\n';

    return 0;
}
