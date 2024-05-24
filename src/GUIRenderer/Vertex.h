#ifndef VERTEX_H
#define VERTEX_H

struct position{
    float x;
    float y;
    float z;
};

struct float2{
    float u;
    float v;
};

struct vColor{
    float r;
    float g;
    float b;
    float a;
};

struct vertex{
    position pos;
    vColor color;
    float2 uvs;
};

#endif
