#include "sudokusolver.h"
#include <iostream>
#include <list>
#include <iterator>
#include <tuple>
#include <ostream>

namespace SS
{

    Cell::Cell() : val(0)
    {
        for(int i = 1;i<10;i++)
        {
            pval.push_back(i);
        }
    }

    Grid::Grid(std::list<int> lst)
    {
        for(int i = 0;i<9;i++)
        {
            for(int j = 0;j<9;j++)
            {
                grid[i][j] = new Cell();
            }
        }
        for(int i = 0;i<9;i++)
        {
            for(int j = 0;j<9;j++)
            {
                if(lst.front() != 0)
                {
                    Cell* cell = grid[i][j];
                    int v = lst.front();
                    cell->value(v);
                    grid[i][j] = cell;
                    updatePvals(i, j, v);
                }
                lst.pop_front();
                
            }
        }
        std::cout << std::endl;
    }

    // Checks if there is only one possible value for the cell and changes the value for the cell if it is the case
    // Returns the value that was left on the list, or 0 if there are still multiple left
    int Cell::checkList()
    {
        int v = 0;
        if(pval.size() < 2)
        {
            value(pval.back());
            pval.clear();
            v = val;
        }
        return v;
    }

    int Cell::value() const
    {
        return val;
    }

    // update the pvals of each cell affected by a value assignment
    void Grid::updatePvals(int x, int y, int v)
    {
        std::cout << "HORVER REMOVAL\n";                                                                                          //TESTOUTPUT
        // Update the lists of possible values for vertical and horizontal neighbors
        for(int k = 0;k<9;k++)
        {
            int newval;
            std::cout << "[" << x << "][" << k << "]: ";                                                                                   //TESTOUTPUT
            newval = grid[x][k]->removePval(v);
            if(newval != 0)
            {
                std::cout << "NEW VALUE :" << newval << "!";                                                                                   //TESTOUTPUT
                updatePvals(x, k, newval);
            }
            std::cout << "[" << k << "][" << y << "]: ";                                                                                   //TESTOUTPUT
            newval = grid[k][y]->removePval(v);
            if(newval != 0)
            {
                std::cout << "NEW VALUE :" << newval << "!";                                                                                   //TESTOUTPUT
                updatePvals(k, y, newval);
            }
        }

        // Update the lists of possible values for the cells in the same box
        // Cells that belong to the same box get the same (int)values from the division operations of location.x/3*3 and location.y/3*3
        std::cout << "BOX REMOVAL: [" << x << "][" << y << "]\n";                                                                             //TESTOUTPUT
        for(int k = 0;k<3;k++)
        {
            for(int l = 0;l<3;l++)
            {
                if((x/3) + (k%3) != x%3 && (y/3) + (l%3) != y%3)
                {
                    int newval;
                    std::cout << "[" << ((x/3)*3)+(k%3) << "][" << ((y/3)*3) + (l%3) << "]: ";                                                                                   //TESTOUTPUT
                    newval = grid[((x/3)*3) + (k%3)][((y/3)*3) + (l%3)]->removePval(v);
                    if(newval != 0)
                    {
                        std::cout << "NEW VALUE :" << newval << "!";                                                                                   //TESTOUTPUT
                        updatePvals(((x/3)*3) + (k%3), ((y/3)*3) + (l%3), newval);
                    }
                }
            }
        }
    }

    void Cell::value(int v)
    {
        val=v;
        pval.clear();
    }


    std::list<int> Grid::getGrid()
    {
        std::list<int> l;
        for(int i = 0;i<9;i++)
        {
            for(int j = 0;j<9;j++)
            {
                l.push_back(grid[i][j]->value());
                //std::cout << *grid[i][j] << " / ";
                std::list<int> pv;                                                                                      //TESTOUTPUT
                std::cout << "Pval at pos: [" << i << "][" << j << "]: ";
                grid[i][j]->getPval(pv);
                for(std::list<int>::iterator ptr = pv.begin();ptr != pv.end();ptr++)
                {
                    std::cout << *ptr;
                }                                                                                                       //TESTOUTPUT
                std::cout << std::endl;
            }
        }
        /*std::list<int>::iterator ptr;
        for(ptr = l.begin();ptr != l.end();ptr++)
        {
            std::cout << "-" << *ptr;
        }
        std::cout << std::endl;*/
        return l;
    }

    // Goes through the list of possible values for the cell and removes matches to v
    int Cell::removePval(int v)
    {
        if(!pval.empty())
        {
            std::list<int>::iterator ptr = pval.begin();
            while(ptr != pval.end())
            {
                if(*ptr == v)
                {
                    std::cout << v << std::endl;                                                        //TESTOUTPUT
                    // Set the pointer to the next 
                    pval.erase(ptr++);
                    // If a possible value was erased, check if there is only one left in the list
                    int left = checkList();
                    if(left != 0)
                    {
                        std::cout << "PLACED: " << left << std::endl;                                   //TESTOUTPUT
                    }
                    return left;
                }
                ptr++;
            }
        }
        std::cout << "None" << std::endl;                                                               //TESTOUTPUT
        return 0;
    }

    // Function for checking whether the puzzle is complete or not. Returns the coordinates of the first cell with the value of 0 encountered, or {-1} if no zeroes were encountered
    int* Grid::checkPuzzle() const
    {
        //The puzzle is solved as long as no zeroes are found, so we set solved = true and flip it back to false if a zero is encountered
        solved = true;
        for(int i = 0;solved == true && i<9;i++)
        {
            for(int j = 0;solved == true && j<9;j++)
            {
                if(grid[i][j]->value() == 0)
                    return {i, j};
            }
        }
        return {-1};
    }

    std::ostream& operator<<(std::ostream& os, const Cell& c)
    {
        os << c.value();
        return os;
    }

    void Cell::getPval(std::list<int>& out)
    {
        std::list<int>::iterator ptr;
        for(ptr = pval.begin();ptr != pval.end();ptr++)
        {
            out.push_back(*ptr);
        }
    }

    const std::list<int> Grid::operator[](int i) const
    {
        std::list<int> l;
        grid[i/9][i%9]->getPval(l);
        return l;
    }

}