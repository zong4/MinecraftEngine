#pragma once

#include "Random.hpp"

namespace Engine
{

class PerlinNoise
{
public:
    explicit PerlinNoise(unsigned int seed) { Init(seed); }
    static PerlinNoise &GetInstance();

    // Getters
    double Noise(double x, double y) const { return Noise(x, y, 0.0); }
    double Noise(double x, double y, double z) const;

private:
    std::vector<int> m_P; // Permutation vector

private:
    PerlinNoise();
    ~PerlinNoise() = default;

private:
    void Init(unsigned int seed);
    static double Fade(double t);
    static double Lerp(double t, double a, double b);
    static double Grad(int hash, double x, double y, double z);
};

} // namespace Engine