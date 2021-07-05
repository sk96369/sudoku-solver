#include "sudokusolver.h"
#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <iterator>

int main()
{
    std::ifstream in("in.txt");
    std::streambuf *cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(in.rdbuf());

    std::list<int> l;
    int value;

    for(int i = 0;i<81;i++)
    {
        std::cin >> value;
        l.push_back(value);
    }

    std::ofstream logtxt("log.txt");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(logtxt.rdbuf());

    SS::Grid grid = SS::Grid(l);

    /* If the input puzzle is simple, the puzzle might already be solved at this point. */

    // Call the solver function to complete the puzzle.
    grid = solve(grid);

    for(int i = 0;i<81;i++)
    {
        std::list<int>::iterator ptr;
        std::list<int> pvals = grid[i];
        for(ptr = pvals.begin();ptr != pvals.end();ptr++)
        {
            //std::cout << i << ": " << *ptr << std::endl;                                      //TESTOUTPUT
        }
    }
    std::list<int> gridList = grid.getGrid();
    std::list<int>::iterator ptr = l.begin();
    std::list<int> onelinelist;
    int help = 0;

    std::ofstream out("out.txt");

    out << "Input puzzle:\n";
    for(;ptr != l.end();ptr++)
    {
        out << *ptr;
        help++;
        if(help%9==0)
        {
            out << std::endl;
        }
    }

    // List for printing out the missing numbers formatted into a grid
    std::list<int> missingparts;

    if(grid.isLegal())
    {
        ptr = l.begin();
        out << "\nFinished puzzle:\n";
        for(int i = 0;i<9;i++)
        {
            for(int j = 0;j<9;j++)
            {
                out << gridList.front();
                if(gridList.front() != *ptr)
                {
                    int front = gridList.front();
                    // Adding 48 to the value, so it converts into the same number when cast into char
                    missingparts.push_back(front+48);
                    onelinelist.push_back(front);
                }
                else
                {
                    // push 32 to represent empty spaces, because it converts to ' ' when cast into char
                    missingparts.push_back(32);
                }
                ptr++;
                gridList.pop_front();
            }
            out << std::endl;
        }
        out << std::endl << "\n\nThe missing numbers in one list, front left to right and top to bottom:\n";
        for(ptr = onelinelist.begin();ptr != onelinelist.end();ptr++)
        {
            out << *ptr;
        }

        std::list<int>::iterator charptr = missingparts.begin();
        out << "\n\nThe missing numbers arranged into the shape of the input puzzle:\n";
        for(int i = 0;i<9;i++)
        {
            for(int j = 0;j<9;j++)
            {
                out << (char)*charptr;
                charptr++;
            }
            out << std::endl;
        }

    }
    else
    {
        out << "The puzzle is impossible to solve :(" << std::endl;
    }
    out.close();
    return 0;
}