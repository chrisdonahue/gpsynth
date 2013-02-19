#include <random>

class GPRandom {
    GPRandom(bool lowerbetter, unsigned seed);

    bool lowerIsBetter;
    unsigned seed;
    std::mt19937_64 engine;
    std::uniform_real_distribution<double> uni_real;

    void normalizeDistribution(std::vector<double>* weights);
    int sampleFromDistribution(std::vector<double>* weights);
    double random();
}
