#include <cstdio>
#include <ctime>

#include <bitset>

template<typename T>
class PrimeGenerator
{
    std::bitset<static_cast<T>(-1)>* bitset;
public:
    PrimeGenerator()
    {
        bitset = new std::bitset<static_cast<T>(-1)>;
    }

    void Generate()
    {
        T max = (static_cast<T>(-2)) / 2;
        for (T i = 1; i < max; ++i)
            if (!(*bitset)[i])
            {
                T val = (i << 1) + 1;
                for (T j = i + val; j > val; j += val)
                    (*bitset)[j] = 1;
            }
    }
    
    void Print() const
    {
        FILE* fp = fopen("primes", "w");
        for (T i = 1; i != static_cast<T>(-1); ++i)
            if (!(*bitset)[i])
                fprintf(fp, "%llu\n", ((unsigned long long)i << 1) + 1);
        fclose(fp);
    }
    
    ~PrimeGenerator()
    {
        delete bitset;
    }
};

int main()
{
    PrimeGenerator<unsigned long> gen;
    time_t start = time(NULL);
    gen.Generate();
    printf("GENERATED in %d seconds\n", time(NULL) - start);
    //gen.Print();
    return 0;
}
