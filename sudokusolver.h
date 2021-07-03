#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H
#include <list>
#include <ostream>

namespace SS
{
    // Class representing a cell
    class Cell
    {
        int val = 0;
        std::list<int> pval;

        public:
        int value() const;
        void value(int v);
        int checkList();
        // Removes the value from the list of possible values for the cell. Returns 0 if value was not set, returns the value if the value was changed.
        int removePval(int v);
        void getPval(std::list<int>& out);


        Cell();
    };

    std::ostream& operator<<(std::ostream& os, const Cell& c);

    class Grid
    {
        //std::deque<Cell*> grid;
        //std::queue<std::queue<Cell*>> verticals;
        //std::queue<std::queue<Cell*>> horizontals;
        //std::queue<std::queue<Cell*>> boxes;
        Cell* grid[9][9];
        public:
        Grid(std::list<int> lst);
        void updatePvals(int x, int y, int v);
        std::list<int> getGrid();
        const std::list<int> operator[](int) const;
        int* checkPuzzle() const;
    };

}

#endif // SUDOKUSOLVER_H