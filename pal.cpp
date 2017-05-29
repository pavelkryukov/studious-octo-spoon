#include <iostream>

template<unsigned N>
bool isPalindrom(unsigned n)
{
    if (!(n % N))
        return false;
    unsigned reverse = 0;
    unsigned temp = n;
 
    while( temp )
    {
       reverse *= N;
       reverse += temp % N;
       temp /= N;
    }
 
    return n == reverse;
}

int main()
{
    for (unsigned i = 0; i != -1; ++i)
        if (isPalindrom<6>(i) && isPalindrom<2>(i))
            std::cout << i << std::endl;
     
    return 0;
}
