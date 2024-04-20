#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>
#include <cassert>

class Vector3{
public:

    float x;
    float y;
    float z;
    float w;

    Vector3(const float & x, const float & y, const float & z){
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1.0f;
    }

    Vector3(const float & x, const float & y){
        this->x = x;
        this->y = y;
        this->z = 0.0f;
        this->w = 1.0f;
    }

    Vector3(){
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
        this->w = 0.0f;
    }

    float Magnitude() const{
        return sqrtf(x*x + y*y + z*z);
    }

    float MagnitudeSqr() const{
        return x*x + y*y + z*z;
    }

    Vector3 Normalize(){
        float magnitude = this->Magnitude();
        assert(magnitude!=0.0f && "Normalization require non zero length vector");
        return Vector3(x/magnitude, y/magnitude, z/magnitude);
    }

    static float Dot(const Vector3 & a, const Vector3 & b){
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static float Distance(const Vector3 & a, const Vector3 & b){
        return (a-b).Magnitude();
    }

    static Vector3 Cross(const Vector3 & a, const Vector3 & b){

        float x = a.y * b.z - b.y * a.z;
        float y = a.x * b.z - b.x * a.z;
        float z = a.x * b.y - b.x * a.y;

        return Vector3(x, y, z);
    }

    static Vector3 Max(const Vector3 & a, const Vector3 & b){
        return Vector3( fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z) );
    }

    static Vector3 Min(const Vector3 & a, const Vector3 & b){
        return Vector3( fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z) );
    }

    static Vector3 Lerp(const Vector3 & a, const Vector3 & b, float t){
        t = fmax(0.0f, fmin(t, 1.0f));
        return a + (b - a) * t;
    }

    static Vector3 Scale(const Vector3 & a, const Vector3 & b){
        return  Vector3(a.x*b.x, a.y*b.y, a.z*b.z);
    }

    static Vector3 Reflect(const Vector3 & incoming, const Vector3 & normal){
        float dotProduct = Vector3::Dot(incoming, normal);
        return incoming - normal * 2.0f * dotProduct;
    }

    static float Angle(const Vector3 & a, const Vector3 & b){

        float aMagn = a.Magnitude();
        float bMagn = b.Magnitude();

        float product = aMagn * bMagn;

        assert(product != 0.0f && "Determining an angle require non zero length vectors");

        float dotProduct = Vector3::Dot(a, b);

        float cosine = dotProduct/product;

        return acos(cosine);
    }

    Vector3 operator-(const Vector3 & v) const{
        return Vector3(x-v.x, y-v.y, z-v.z);
    }

    Vector3 operator+(const Vector3 & v) const{
        return Vector3(x+v.x, y+v.y, z+v.z);
    }

    Vector3 operator*(const float & scalar) const{
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator/(const float & scalar) const{
        assert(scalar != 0.0f && "Cannot divide by zero");
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    bool operator==(const Vector3 & v) const{
        return (x==v.x && y==v.y && z==v.z && w == v.w);
    }

    bool operator!=(const Vector3 & v) const{
        return (x!=v.x || y!=v.y || z!=v.z || w != v.w);
    }

};

#endif
