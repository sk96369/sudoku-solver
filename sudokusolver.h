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
        std::list<int> getPval() const;
        int setTopGuess();

        Cell();
        Cell(const Cell &c);
    };

    std::ostream& operator<<(std::ostream& os, const Cell& c);

    class Grid
    {
        //std::deque<Cell*> grid;
        //std::queue<std::queue<Cell*>> verticals;
        //std::queue<std::queue<Cell*>> horizontals;
        //std::queue<std::queue<Cell*>> boxes;
        Cell* grid[9][9];
        bool legal;
        public:
        Grid(std::list<int> lst);
        void updatePvals(int x, int y, int v);
        std::list<int> getGrid();
        const std::list<int> operator[](int) const;
        // A function that returns a list of integers that point to cells that have no values yet
        std::list<int> missingvalues() const;
        Grid(const Grid &g);
        Cell* getCell(int x, int y) const;
        void legalCheck();
        bool isLegal() const;
        Cell* getCell(int i);
        bool updatelegality();
    };

    Grid solve(Grid g);

}

#endif // SUDOKUSOLVER_H