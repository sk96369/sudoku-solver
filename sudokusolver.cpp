#include "sudokusolver.h"
#include <iostream>
#include <list>
#include <iterator>
#include <tuple>
#include <ostream>

namespace SS
{
    // Define the static member as 0
    int Grid::count = 0;

    Cell::Cell() : val(0)
    {
        for(int i = 1;i<10;i++)
        {
            pval.push_back(i);
        }
    }

    Cell::Cell(const Cell &c) : val(c.value())
    {
        pval=c.getPval();
    }

    std::list<int> Cell::getPval() const
    {
        std::list<int> newlist;
        for(std::list<int>::const_iterator ptr = pval.begin();ptr != pval.end();ptr++)
        {
            newlist.push_back(*ptr);
        }
        return newlist;
    }

    int Grid::getNr() const
    {
        return gridNr;
    }

    Grid::Grid(std::list<int> lst) : legal(true), gridNr(count)
    {
        count++;
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

    Cell* Grid::getCell(int x, int y) const
    {
        return grid[x][y];
    }

    Grid::Grid(const Grid &g) : legal(g.isLegal()), gridNr(count)
    {
        count++;
        for(int i = 0;i<9;i++)
        {
            for(int j = 0;j<9;j++)
            {
                grid[i][j] = new Cell(*g.getCell(i, j));
            }
        }
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

    void Cell::removeTopPval()
    {
        pval.pop_front();
    }

    // update the pvals of each cell affected by a value assignment
    void Grid::updatePvals(int x, int y, int v)
    {
        //std::cout << "HORVER REMOVAL\n";                                                                                          //TESTOUTPUT
        // Update the lists of possible values for vertical and horizontal neighbors
        for(int k = 0;k<9;k++)
        {
            int newval;
            //std::cout << "[" << x << "][" << k << "]: ";                                                                                   //TESTOUTPUT
            newval = grid[x][k]->removePval(v);
            if(newval != 0)
            {
                //std::cout << "NEW VALUE :" << newval << "!";                                                                                   //TESTOUTPUT
                updatePvals(x, k, newval);
            }
            //std::cout << "[" << k << "][" << y << "]: ";                                                                                   //TESTOUTPUT
            newval = grid[k][y]->removePval(v);
            if(newval != 0)
            {
                //std::cout << "NEW VALUE :" << newval << "!";                                                                                   //TESTOUTPUT
                updatePvals(k, y, newval);
            }
        }

        // Update the lists of possible values for the cells in the same box
        // Cells that belong to the same box get the same (int)values from the division operations of location.x/3*3 and location.y/3*3
        //std::cout << "BOX REMOVAL: [" << x << "][" << y << "]\n";                                                                             //TESTOUTPUT
        for(int k = 0;k<3;k++)
        {
            for(int l = 0;l<3;l++)
            {
                if((x/3) + (k%3) != x%3 && (y/3) + (l%3) != y%3)
                {
                    int newval;
                    //std::cout << "[" << ((x/3)*3)+(k%3) << "][" << ((y/3)*3) + (l%3) << "]: ";                                         //TESTOUTPUT
                    newval = grid[((x/3)*3) + (k%3)][((y/3)*3) + (l%3)]->removePval(v);
                    if(newval != 0)
                    {
                        //std::cout << "NEW VALUE :" << newval << "!";                                                                                   //TESTOUTPUT
                        updatePvals(((x/3)*3) + (k%3), ((y/3)*3) + (l%3), newval);
                    }
                }
            }
        }
    }

    bool Grid::isLegal() const
    {
        return legal;
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
                grid[i][j]->getPval(pv);
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

    // Goes through the list of possible values for the cell and removes matches to v. Returns 0 if no possible values were removed
    int Cell::removePval(int v)
    {
        if(!pval.empty())
        {
            std::list<int>::iterator ptr = pval.begin();
            while(ptr != pval.end())
            {
                if(*ptr == v)
                {
                    //std::cout << v << std::endl;                                                        //TESTOUTPUT
                    // Set the pointer to the next 
                    pval.erase(ptr++);
                    // If a possible value was erased, check if there is only one left in the list
                    int left = checkList();
                    return left;
                }
                ptr++;
            }
        }
        //std::cout << "None" << std::endl;                                                               //TESTOUTPUT
        return 0;
    }

    int Grid::missingvalues() const
    {
        for(int i = 0;i<81;i++)
        {
            if(grid[i/9][i%9]->value() == 0)
                return i;
        }
        return -1;
    }

    void Grid::legalCheck()
    {
        legal=updatelegality();
        if(!legal)
        {
            std::cout << "The grid is illegal!" << std::endl;
        }
        else
            std::cout << "The grid is legal!" << std::endl;
    }

    // Function for testing if the completed grid is legal. True for legal, false for illegal
    bool Grid::updatelegality()
    {
        std::cout << "The filled grid:\n";                                                                                 //TESTOUTPUT
        for(int i = 0;i<9;i++)
        {
            for(int j = 0;j<9;j++)
            {
                std::cout << grid[i][j]->value();
            }
            std::cout << std::endl;
        }

        // Horizontal sums
        int sum = 0;
        std::cout << "Horizontal sum:";
        for(int i = 0;i<9;i++)
        {
            sum = 0;
            std::cout << std::endl << "Sum for row " << i << std::endl;
            for(int j = 0;j<9;j++)
            {
                sum += grid[i][j]->value();
                std::cout << grid[i][j]->value();
            }
            std::cout << "The horizontal sum: " << sum << std::endl;
            if(sum != 45)
            {
                std::cout << "Incorrect sum!" << std::endl;
                return false;
            }
                
        }
        

        // Vertical sums
        for(int i = 0;i<9;i++)
        {
            sum = 0;
            std::cout << std::endl << "Sum for collumn " << i << std::endl;
            for(int j = 0;j<9;j++)
            {
                sum += grid[j][i]->value();
                std::cout << grid[j][i]->value();
            }
            if(sum != 45)
                return false;
                std::cout << "The vertical sum: " << sum << std::endl;
        }

        // Box sums
        for(int i = 0;i < 9;i++)
        {
            sum = 0;
            std::cout << std::endl << "Sum for box " << i << std::endl;
            for(int j = 0;j < 9;j++)
            {
                sum += grid[((i/3)*3)+(j/3)][((i%3)*3)+(j%3)]->value();
                std::cout << grid[((i/3)*3)+(j/3)][((i%3)*3)+(j%3)]->value();
            }
            if(sum != 45)
                return false;
                std::cout << "The box sum: " << sum << std::endl;
        }
        
        // Check for duplicates horizontally
        for(int i = 0;i<9;i++)
        {
            int duplcheck[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            for(int j = 0;j<9;j++)
            {
                int v = grid[i][j]->value();
                if(duplcheck[v] == 0)
                    duplcheck[v] = v;
                else
                {
                    return false;
                }
            }
        }

        // Check for duplicates vertically
        for(int i = 0;i<9;i++)
        {
            int duplcheck[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            for(int j = 0;j<9;j++)
            {
                int v = grid[j][i]->value();
                if(duplcheck[v] == 0)
                    duplcheck[v] = v;
                else
                    return false;
            }
        }
        // Check for duplicates by boxes
        for(int i = 0;i<9;i++)
        {
            int duplcheck[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            for(int j = 0;j<9;j++)
            {
                int v = grid[(i/3)*3 + (j/3)][(i%3)*3 + (j%3)]->value();
                if(duplcheck[v] == 0)
                    duplcheck[v] = v;
                else
                    return false;
            }
        }
        return true;

    }

    Cell* Grid::getCell(int i)
    {
        return grid[i/9][i%9];
    }

    // A function that makes guesses on the missing values until the puzzle is complete. Returns the completed puzzle grid (or NULL if the puzzle is impossible)
    // state. As long as the puzzle is not completed, creates a copy of the current grid, in which guesses on the missing values are made. If the puzzle doesn't complete itself 
    // upon taking a guess, the function will call itself recursively until the puzzle is complete 
    Grid solve(Grid &g)
    {
        //List of indexes to the cells with missing values
        int firstcell = g.missingvalues();
        std::cout << "_______________\nSolving..." << std::endl;
        if(firstcell != -1)
        {
            // Get the pointer to the first unfilled cell of the given grid
            Cell* c = g.getCell(firstcell);
            // Go through all the possible values for the cell
            while(c->hasPval())
            {
                std::list<int> cpvals;
                // Create a copy of the given grid
                Grid copy = g;
                Cell* c_copy = copy.getCell(firstcell);
                c_copy->getPval(cpvals);

                std::cout << "Possible values for Cell #" << firstcell << std::endl;
                for(std::list<int>::iterator cpvalsptr = cpvals.begin();cpvalsptr != cpvals.end();cpvalsptr++)
                {
                    std::cout << *cpvalsptr << " ";
                }
                std::cout << std::endl;

                std::cout << "Created a new copy of the grid, grid #: " << copy.getNr() << std::endl;
                std::cout << "Taking a guess on cell #: " << firstcell << "\n";
                int newval = c_copy->setTopGuess();
                copy.updatePvals(firstcell/9, firstcell%9, newval);              
                copy = solve(copy);
                if(copy.isLegal())
                {
                    std::cout << "The solution is legal! Grid nr: " << copy.getNr() << " legality: " << copy.isLegal() << std::endl;
                    return copy;
                }
                // If the the call of the solve function returns an illegal solution, remove the guessed value from the given grid, because that value is now known to
                // not be a possible value
                c->removeTopPval();
            }
            // If all of the possible values end up producing an illegal solution, declare the grid illegal and return it
            std::cout << "The cell has no more possible values, marking this copy of the grid illegal.\n";
            g.markIllegal();
            return g;
        }
        std::cout << "The puzzle is filled! Checking legality..." << std::endl;
        g.legalCheck();
        return g;
    }

    void Grid::markIllegal()
    {
        legal = false;
    }

    // Sets the cell's value to be the one on the top of the list of possible values.
    int Cell::setTopGuess()
    {
        int top = pval.front();
        std::cout << "Guessing the value: " << top << std::endl;
        val = top;
        pval.clear();
        return top;
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

    // Returns true if the cell's list of possible values is not empty
    bool Cell::hasPval() const
    {
        return !pval.empty();
    }

}