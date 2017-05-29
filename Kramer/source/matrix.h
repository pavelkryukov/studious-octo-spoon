/**
 * system.h
 *
 * Matrix template
 *
 * Copyright (C) Pavel Kryukov, Konstantin Margorin 2011
 * for MIPT C++ course
 *
 * Copyright (C) Pavel Kryukov 2012 (remastering)
*/

#ifndef _MATRIX_H
#define _MATRIX_H

// C++ generic
#include <vector>
#include <iostream>

/**
 * Matrix template
 */
template<class T>
class Matrix : protected std::vector<std::vector<T> >
{  
protected:
    unsigned _dim; // matrix dimension

    mutable T _det; // determinant storages
    mutable bool _det_c;   
public:
    /**
     * Zero constructor
     */
    Matrix<T>();

    /**
     * Copy constructor
     * @param source source
     */
    explicit Matrix<T>(const Matrix<T> & source);

    /**
     * Changes column to mentioned
     * @param num number of column
     * @param vector vector
     */
    void change(unsigned num, const std::vector<T> & vector);

    /**
     * Gauss transform
     */
    void gauss();

    /**
     * Diagonal mulitplication result
     * @return result
     */
    T diag() const;

    /**
     * Print operator
     */
    template<class T1>
    friend std::ostream & operator<< (std::ostream & out, const Matrix<T1> & item);
       
    /**
     * Returns determinant of matrix
     */
    T getDet() const;
       
    /**
     * Returns dimension of matrix
     */
    inline unsigned getDim() const { return _dim; }
};

template<class T>
Matrix<T>::Matrix()
    : std::vector<std::vector<T> >()
    , _dim(0)
    , _det_c(false)
{ }

template<class T>
Matrix<T>::Matrix(const Matrix<T> & source)
    : std::vector<std::vector<T> >(source)
    , _dim(source._dim)
    , _det_c(false)
{ }

template<class T>
void Matrix<T>::change( unsigned whereNum, const std::vector<T> & what)
{
    for ( unsigned i = 0; i < _dim; i++)
        (*this)[i][whereNum] = what[i];
   
    // Changing matrix cause change of its det
    _det_c = false;
}

/*
 * Gauss transform of Matix
 * O(n^3)
 */
template<class T>
void Matrix<T>::gauss()
{
    T buf;
    unsigned l;
   
    // For every line...
    for ( unsigned i = 1; i < _dim; i++)
        for ( unsigned k = 0; k < i; k++)
        {
        // ... substract linear combination of rows with coeffs
            if (!(*this)[k][k])
            {
            // If there's zero already, swap columns with change of sign
                l = k + 1;
                while ((l < _dim) && (!(*this)[k][l]))
                    l++;

                if (l == _dim)
                {
                // If there's no such column, this matrix can be transformed
                // to zero matrix. Det is 0, so we can just put zero on diag.
                    (*this)[0][0] = 0;
                    return;
                }

                // Swap with sign change
                for ( unsigned j = 0; j < _dim; j++)
                {
                    buf = (*this)[j][k];
                    (*this)[j][k] = -(*this)[j][l];
                    (*this)[j][l] = buf;
                }
            }
           
            // Substraction
            buf = (*this)[i][k] / (*this)[k][k];
            for ( unsigned j = 0; j < _dim; j++)
                (*this)[i][j] = (*this)[i][j] - (*this)[k][j] * buf;
        }
}

template<class T>
T Matrix<T>::diag() const
{
    T result = 1;
    for ( unsigned i = 0; i < _dim; i++)
        result *= (*this)[i][i];

    return result;
}

template<class T> T
Matrix<T>::getDet() const
{ 
    if (_det_c)
        return _det;
   
    Matrix<T> Gauss(*this);
   
    Gauss.gauss();
   
    _det = Gauss.diag();
    _det_c = true;

    return _det;
}  

template<class T>
std::ostream & operator<< (std::ostream & out, const Matrix<T> & item)
{
    for ( unsigned i = 0; i < item._dim; i++)
    {
        for ( unsigned j = 0; j < item._dim; j++)
            out << item[i][j] << " ";
        out << std::endl;
    }
    return out;
}

#endif  // _MATRIX_H
