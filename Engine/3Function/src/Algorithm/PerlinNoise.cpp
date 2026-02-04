#include "PerlinNoise.hpp"

Engine::PerlinNoise::PerlinNoise()
{
    std::random_device rd;
    Init(rd());
}

Engine::PerlinNoise &Engine::PerlinNoise::GetInstance()
{
    static PerlinNoise instance;
    return instance;
}

double Engine::PerlinNoise::Noise(double x, double y, double z) const
{
    // Unit cube
    int X = (int)std::floor(x) & 255;
    int Y = (int)std::floor(y) & 255;
    int Z = (int)std::floor(z) & 255;

    // Coordinates within the cube
    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    // Smooth curves
    double u = Fade(x);
    double v = Fade(y);
    double w = Fade(z);

    // Hash coordinates of the cube corners
    int A = m_P[X] + Y;
    int AA = m_P[A] + Z;
    int AB = m_P[A + 1] + Z;
    int B = m_P[X + 1] + Y;
    int BA = m_P[B] + Z;
    int BB = m_P[B + 1] + Z;

    // Lerp between gradients
    double res = Lerp(w,
                      Lerp(v, Lerp(u, Grad(m_P[AA], x, y, z), Grad(m_P[BA], x - 1, y, z)),
                           Lerp(u, Grad(m_P[AB], x, y - 1, z), Grad(m_P[BB], x - 1, y - 1, z))),
                      Lerp(v, Lerp(u, Grad(m_P[AA + 1], x, y, z - 1), Grad(m_P[BA + 1], x - 1, y, z - 1)),
                           Lerp(u, Grad(m_P[AB + 1], x, y - 1, z - 1), Grad(m_P[BB + 1], x - 1, y - 1, z - 1))));
    return res;
}

void Engine::PerlinNoise::Init(unsigned int seed)
{
    m_P.resize(256);
    std::iota(m_P.begin(), m_P.end(), 0); // Fill with values 0 to 255
    std::mt19937 gen(seed);
    std::shuffle(m_P.begin(), m_P.end(), gen);     // Shuffle the values
    m_P.insert(m_P.end(), m_P.begin(), m_P.end()); // Duplicate the array
}

double Engine::PerlinNoise::Fade(double t)
{
    return t * t * t * (t * (t * 6 - 15) + 10); // 6t^5 - 15t^4 + 10t^3
}

double Engine::PerlinNoise::Lerp(double t, double a, double b) { return a + t * (b - a); }

double Engine::PerlinNoise::Grad(int hash, double x, double y, double z)
{
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}