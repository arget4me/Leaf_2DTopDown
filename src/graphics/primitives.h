#pragma once


namespace LEAF 
{
struct Vec2
{
    union
    {
        struct{
            float a;
            float b;
        };
        struct{
            float x;
            float y;
        };
    };

    Vec2(float a, float b) : a(a), b(b) {}
    Vec2() : a(0.0f), b(0.0f) {}


    Vec2& operator=(Vec2& other);
    bool operator==(Vec2& other);

    Vec2& operator+(Vec2& other);
    Vec2& operator-(Vec2& other);
    Vec2& operator/(Vec2& other);
    Vec2& operator*(Vec2& other);
    Vec2& operator+(float value);
    Vec2& operator-(float value);
    Vec2& operator/(float value);
    Vec2& operator*(float value);
};

struct Vec3
{
    union
    {
        struct{
            float a;
            float b;
            float c;
        };
        struct{
            float x;
            float y;
            float z;
        };
        Vec2 ab;
        Vec2 xy;
        Vec2 vec2;
    };

    Vec3(float a, float b, float c) : a(a), b(b), c(c) {}
    Vec3() : a(0.0f), b(0.0f), c(0.0f) {}

    Vec3& operator=(Vec3& other);
    Vec3& operator=(Vec2& other);
    bool operator==(Vec3& other);

    Vec3& operator+(Vec3& other);
    Vec3& operator-(Vec3& other);
    Vec3& operator/(Vec3& other);
    Vec3& operator*(Vec3& other);
    Vec3& operator+(float value);
    Vec3& operator-(float value);
    Vec3& operator/(float value);
    Vec3& operator*(float value);
};

struct Vec4
{
    union
    {
        struct{
            float a;
            float b;
            float c;
            float d;
        };
        struct{
            float x;
            float y;
            float z;
            float w;
        };
        Vec2 ab;
        Vec2 xy;
        Vec2 vec2;
        Vec3 abc;
        Vec3 xyz;
        Vec3 vec3;
    };

    Vec4(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {}
    Vec4() : a(0.0f), b(0.0f), c(0.0f), d(0.0f) {}

    Vec4& operator=(Vec4& other);
    Vec4& operator=(Vec3& other);
    Vec4& operator=(Vec2& other);

    bool operator==(Vec4& other);
    bool operator==(Vec3& other);
    bool operator==(Vec2& other);

    Vec4& operator+(Vec4& other);
    Vec4& operator-(Vec4& other);
    Vec4& operator/(Vec4& other);
    Vec4& operator*(Vec4& other);
    Vec4& operator+(float value);
    Vec4& operator-(float value);
    Vec4& operator/(float value);
    Vec4& operator*(float value);
};

struct Quad
{
    Vec4 position[4];
    Vec2 uvs[4];
    unsigned int indices[6];

    Quad();
};

}