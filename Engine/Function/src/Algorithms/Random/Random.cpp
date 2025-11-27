#include "Random.hpp"

#include <random>

float Engine::Random::RandomFloat()
{
    static thread_local std::mt19937 generator(std::random_device{}());
    static thread_local std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    return distribution(generator);
}
