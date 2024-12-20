#pragma once

#include "Custommath.hpp"
#include "Vector2D.hpp"
#include "Vector3D.hpp"

class Perlin
{
private:
    static inline const int perm[]
    {
        151,160,137,91,90,15,
       131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
       190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
       88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
       77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
       102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
       135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
       5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
       223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
       129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
       251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
       49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
       138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
       151
    };

public:
    static float Noise(float x)
    {
        auto X = FloorToInt(x) & 0xff;
        x -= Floor(x);
        auto u = Fade(x);
        return Lerp(u, Grad(perm[X], x), Grad(perm[X + 1], x - 1)) * 2;
    }

    static float Noise(float x, float y)
    {
        auto X = FloorToInt(x) & 0xff;
        auto Y = FloorToInt(y) & 0xff;
        x -= Floor(x);
        y -= Floor(y);
        auto u = Fade(x);
        auto v = Fade(y);
        auto A = (perm[X] + Y) & 0xff;
        auto B = (perm[X + 1] + Y) & 0xff;
        return Lerp(v, Lerp(u, Grad(perm[A], x, y), Grad(perm[B], x - 1, y)),
            Lerp(u, Grad(perm[A + 1], x, y - 1), Grad(perm[B + 1], x - 1, y - 1)));
    }

    static float Noise(const Vector2D& coord)
    {
        return Noise(coord.x, coord.y);
    }

    static float Noise(float x, float y, float z)
    {
        auto X = FloorToInt(x) & 0xff;
        auto Y = FloorToInt(y) & 0xff;
        auto Z = FloorToInt(z) & 0xff;
        x -= Floor(x);
        y -= Floor(y);
        z -= Floor(z);
        auto u = Fade(x);
        auto v = Fade(y);
        auto w = Fade(z);
        auto A = (perm[X] + Y) & 0xff;
        auto B = (perm[X + 1] + Y) & 0xff;
        auto AA = (perm[A] + Z) & 0xff;
        auto BA = (perm[B] + Z) & 0xff;
        auto AB = (perm[A + 1] + Z) & 0xff;
        auto BB = (perm[B + 1] + Z) & 0xff;
        return Lerp(w, Lerp(v, Lerp(u, Grad(perm[AA], x, y, z), Grad(perm[BA], x - 1, y, z)),
            Lerp(u, Grad(perm[AB], x, y - 1, z), Grad(perm[BB], x - 1, y - 1, z))),
            Lerp(v, Lerp(u, Grad(perm[AA + 1], x, y, z - 1), Grad(perm[BA + 1], x - 1, y, z - 1)),
                Lerp(u, Grad(perm[AB + 1], x, y - 1, z - 1), Grad(perm[BB + 1], x - 1, y - 1, z - 1))));
    }

    static float Noise(const Vector3D& coord)
    {
        return Noise(coord.x, coord.y, coord.z);
    }

    static float Fbm(float x, int octave)
    {
        auto f = 0.0f;
        auto w = 0.5f;
        for (auto i = 0; i < octave; i++) {
            f += w * Noise(x);
            x *= 2.0f;
            w *= 0.5f;
        }
        return f;
    }

    static float Fbm(Vector2D coord, int octave)
    {
        auto f = 0.0f;
        auto w = 0.5f;
        for (auto i = 0; i < octave; i++) {
            f += w * Noise(coord);
            coord *= 2.0f;
            w *= 0.5f;
        }
        return f;
    }

    static float Fbm(float x, float y, int octave)
    {
        return Fbm(Vector2D(x, y), octave);
    }

    static float Fbm(Vector3D coord, int octave)
    {
        auto f = 0.0f;
        auto w = 0.5f;
        for (auto i = 0; i < octave; i++) {
            f += w * Noise(coord);
            coord *= 2.0f;
            w *= 0.5f;
        }
        return f;
    }

    static float Fbm(float x, float y, float z, int octave)
    {
        return Fbm(Vector3D(x, y, z), octave);
    }

private:
    static float Fade(float t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static float Lerp(float t, float a, float b)
    {
        return a + t * (b - a);
    }

    static float Grad(int hash, float x)
    {
        return (hash & 1) == 0 ? x : -x;
    }

    static float Grad(int hash, float x, float y)
    {
        return ((hash & 1) == 0 ? x : -x) + ((hash & 2) == 0 ? y : -y);
    }

    static float Grad(int hash, float x, float y, float z)
    {
        auto h = hash & 15;
        auto u = h < 8 ? x : y;
        auto v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
};