#pragma once

#include <random>

namespace Engine
{

class Random
{
public:
    explicit Random(unsigned int seed) { m_Generator.seed(seed); }
    static Random &GetInstance()
    {
        static Random instance;
        return instance;
    }

    // Getters
    int NextInt(int min = 0, int max = 1)
    {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(m_Generator);
    }
    float NextFloat(float min = 0.0f, float max = 1.0f)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(m_Generator);
    }
    double NextDouble(double min = 0.0, double max = 1.0)
    {
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(m_Generator);
    }

private:
    std::mt19937 m_Generator;

private:
    Random()
    {
        std::random_device rd;
        m_Generator.seed(rd());
    }
    ~Random() = default;
};

} // namespace Engine