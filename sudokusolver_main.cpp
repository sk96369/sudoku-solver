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
    bool solved = false; // Flag for checking whether the puzzle has been declared solved or not

    for(int i = 0;i<81;i++)
    {
        std::cin >> value;
        l.push_back(value);
    }
    std::cout << std::endl;

    std::ofstream out("out.txt");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    SS::Grid grid = SS::Grid(l);

    /* If the input puzzle is simple, the puzzle might already be solved at this point. */
    


    // As long as the puzzle has not been declared solved, make guesses until it is
    while(!solved)
    {
        
        //The puzzle is solved as long as no zeroes are found, so we set solved = true and flip it back to false if a zero is encountered
        solved = true;
        int* out = grid.checkPuzzle();
        if(out[0] != -1)
        {
            int x = out[0]; int y = out[1];
            int v = grid[x][y]->p;
            grid[x][y]->value(v);
            grid->updatePvals(x, y, v);
        }
    }
    

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

    std::cout << "Input puzzle:\n";
    for(;ptr != l.end();ptr++)
    {
        std::cout << *ptr;
        help++;
        if(help%9==0)
        {
            std::cout << std::endl;
        }
    }
    ptr = l.begin();
    std::cout << "Finished puzzle:\n";
    for(int i = 0;i<9;i++)
    {
        for(int j = 0;j<9;j++)
        {
            std::cout << gridList.front();
            if(gridList.front() != *ptr)
            {
                onelinelist.push_back(gridList.front());
            }
            ptr++;
            gridList.pop_front();
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << "The missing numbers in one list, front left to right and top to bottom:\n";
    for(ptr = onelinelist.begin();ptr != onelinelist.end();ptr++)
    {
        std::cout << *ptr;
    }
}