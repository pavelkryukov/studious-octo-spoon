/**
 * system.h
 *
 * SLAE template
 *
 * Copyright (C) Pavel Kryukov, Konstantin Margorin 2011
 * for MIPT C++ course
 *
 * Copyright (C) Pavel Kryukov 2012 (remastering)
*/

#ifndef _SYSTEM_H
#define _SYSTEM_H

// C generic
#include <cstdio>
#include <cerrno>

// C++ generic
#include <vector>

// Pthread generic
#ifdef THREADS
#include <pthread.h>
#endif

#include "matrix.h"

/**
 * SLAE template
 */
template<class T>
class System: public Matrix<T>
{
private:
    bool _correct;  // checks for correctness of input
    int l_errno;
      
    std::vector<T> _column; // right-side numbers
       
    // Struct for calling threads
    struct CallArgs
    {
        const System* const ptr;
        const unsigned num;
        CallArgs(const System* ptr, const unsigned num)
            : ptr(ptr)
            , num(num)
        { }
    };

    /**
     * Counts n-s element of solution
     * @param n number of element
     */
    void _solCounter(unsigned n) const;
       
    // Thread entry point
    static void* _solCounterCall(void*);

    // Solution storage
    mutable std::vector<T> _solution;
    mutable bool _solution_c;
       
public:
    /**
     * System constructor
     * @param fp
     */
    System(FILE* fp);
       
    /**
     * Check errors
     * @return 0 if no errors
     * @return errno
     */
    inline int getError() const { return _correct ? 0 : l_errno; }
       
    /**
     * Returns solution
     * @return solution vector
     * @return zero vector if matrix is wrong
     */
    std::vector<T>& getSolution() const;
};

/*
 * File format:
 *
 * size of matrix in first line
 * other lines is extended system matrix
 */
template<class T>
System<T>::System(FILE* fp)
    : Matrix<T>()
    , _correct(true)
    , _solution_c(false)
{
    _correct = true;
   
    // Scan size
    if (fscanf(fp, "%u", &this->_dim) < 0)
    {
        _correct = false;
        this->l_errno = errno;   
        this->_dim = 0;
    }
      
    T buffer;
    this->resize( this->_dim);
    _column.resize( this->_dim);
    for ( unsigned i = 0; i < this->_dim; i++)
    {

        (*this)[i].resize( this->_dim);
        for (unsigned j = 0; j < this->_dim; j++)
            if (std::fscanf( fp, "%f", &buffer) < 0)
            {
                _correct = false;
                l_errno = errno;
                (*this)[i][j] = 0;
            }
            else {
                (*this)[i][j] = buffer;
            }
       
        // Заполнение столбца свободных членов
        if ( std::fscanf( fp, "%f", &buffer) < 0)
        {
            _correct = false;
            l_errno = errno;
            _column[i] = 0;
        }
        else {
            _column[i] = buffer;
        }
    }
}

template<class T>
void System<T>::_solCounter(unsigned num) const
{
    Matrix<T> swapped(*this);

    swapped.change(num, _column);
   
    T det = swapped.getDet();

    _solution[num] = det / this->_det;
}

template<class T>
void* System<T>::_solCounterCall( void* arg_ptr)
{
    CallArgs* args = reinterpret_cast<CallArgs*>(arg_ptr);
    args->ptr->_solCounter(args->num);
#ifdef THREADS
    pthread_exit(NULL);
#endif
    return NULL;
}      

template<class T>
std::vector<T>& System<T>::getSolution() const {
    if (!_correct)
    {
        _solution_c = true;
        _solution.resize(0);
        return _solution;
    }

    if (_solution_c) 
        return _solution;

    if (!this->_det_c)
        this->getDet();

    if (!this->_det)
    {
        _solution_c = true;
        _solution.resize(0);
        return _solution;
    }

    _solution.resize(this->_dim);
#ifdef THREADS
    std::cout << "THREADS MODE ON" << std::endl;

    std::vector<pthread_t> threads(this->_dim);
    std::vector<CallArgs*> tasks(this->_dim);
    for ( unsigned i = 0; i < this->_dim; i++)
    {
        tasks[i] = new CallArgs(this, i);
        pthread_create(&threads[i], 0, this->_solCounterCall, 
            (void*)(tasks[i]));
    }


    for ( unsigned i = 0; i < this->_dim; i++)
    {
        pthread_join(threads[i], NULL);
        delete tasks[i];
    }
#else
    std::cout << "THREADS MODE OFF" << std::endl;
    for ( unsigned i = 0; i < this->_dim; i++)
        this->_solCounter(i);
#endif

    _solution_c = true;
    return _solution;
}

#endif // _SYSTEM_H