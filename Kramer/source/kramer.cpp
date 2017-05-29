/**
 * kramer.cpp
 *
 * Solving SLAE with Kramer method
 *
 * Copyright (C) Pavel Kryukov, Konstantin Margorin 2011
 * for MIPT C++ course
 *
 * Copyright (C) Pavel Kryukov 2012 (remastering)
*/

// C generic
#include <cerrno>
#include <cstring>

// C++ generic
#include <iostream>
#include <vector>

//#define THREADS

#include "matrix.h"
#include "system.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Syntax error\n";
        return -1;
    }

    FILE *fp = std::fopen(argv[1],"r");
    if (!fp)
    {
        std::cout << "Can't open file " << argv[1] << std::endl
            << ".\nError ¹" << errno
            << "(" << strerror(errno) << ")" << std::endl;
        return -1;
    }
    System<float> core(fp);  
    fclose(fp);

    int error = core.getError();
    if (error)
    {
        std::cout << "Error in reading file " << argv[1] << std::endl
            << ".\nError ¹" << error
            << "(" << strerror(error) << ")" << std::endl;
        return -1;
    }

    std::vector<float> solution = core.getSolution();
    if (solution.empty())
    {
        std::cout << "Undetermined system" << std::endl;
        return 0;
    }

    std::vector<float>::const_iterator it;
    for (it = solution.begin(); it != solution.end(); ++it)
        std::cout << "\t"<< *it << std::endl;

    return 0;
}
