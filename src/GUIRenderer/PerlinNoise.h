#ifndef PERLIN_H
#define PERLIN_H

#include <math.h>
#include <stdio.h>
#include <ctime>

static int p[512];

class Perlin{

private:

    static double Fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static double Grad(int hash, double x, double y) {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : (h == 12 || h == 14) ? x : 0;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    Perlin() = delete;

public:

    static void Initialize(){

        srand( time(NULL) );

        for (int i = 0; i < 256; ++i)
            p[i] = i;

        for (int i = 0; i < 256; ++i) {
            int j = rand() & 255;
            std::swap(p[i], p[j]);
        }

        for (int i = 0; i < 256; ++i)
            p[256 + i] = p[i];
    }

    static double Lerp(double t, double a, double b){
        return a + t * (b - a);
    }

    static double Noise(double x, double y) {
        int X = (int)floor(x) & 255;
        int Y = (int)floor(y) & 255;

        x -= floor(x);
        y -= floor(y);

        double u = Fade(x);
        double v = Fade(y);

        int A = p[X] + Y;
        int AA = p[A];
        int AB = p[A + 1];
        int B = p[X + 1] + Y;
        int BA = p[B];
        int BB = p[B + 1];

        return Lerp(v,
        Lerp(u, Grad(p[AA], x, y),
        Grad(p[BA], x - 1, y)),
        Lerp(u, Grad(p[AB], x, y - 1),
        Grad(p[BB], x - 1, y - 1))
        ) * 0.5 + 0.5;
    }

};

#endif
