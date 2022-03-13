#pragma once

#include <cmath>
#include <ostream>

class v4f {
private:

public:
    float x, y, z, w;

    v4f() {}

    v4f(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    };
    ~v4f() {}

    v4f operator + (v4f that) {
        return { this->x + that.x, this->y + that.y, this->z + that.z, this->w + that.w };
    }

    v4f operator - (v4f that) {
        return { this->x - that.x, this->y - that.y, this->z - that.z, this->w - that.w };
    }

    v4f operator / (float scalar) {
        return { this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar };
    }

    v4f operator * (float scalar) {
        return { this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar };
    }

    friend std::ostream& operator << (std::ostream& os, v4f const& vec) { 
        return os << "[ " << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << " ]";
    }

    float norm() {
        return sqrt(this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w);
    }

    v4f normalize() {
        return (v4f){x, y, z, w} / this->norm();
    }
};

class v3f {
private:

public:
    float x, y, z;

    v3f() {}

    v3f(v4f v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    v3f(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    };
    ~v3f() {}

    v3f operator + (v3f that) {
        return { this->x + that.x, this->y + that.y, this->z + that.z };
    }

    v3f operator - (v3f that) {
        return { this->x - that.x, this->y - that.y, this->z - that.z };
    }

    v3f operator / (float scalar) {
        return { this->x / scalar, this->y / scalar, this->z / scalar };
    }

    v3f operator * (float scalar) {
        return { this->x * scalar, this->y * scalar, this->z * scalar };
    }

    bool operator == (v3f that) {
        return (this->x == that.x && this->y == that.y && this->z == that.z);
    }

    bool operator != (v3f that) {
        return !(this->x == that.x && this->y == that.y && this->z == that.z);
    }

    friend std::ostream& operator << (std::ostream& os, v3f const& vec) { 
        return os << "[ " << vec.x << ", " << vec.y << ", " << vec.z << " ]";
    }

    v3f cross(v3f that) {
        v3f crossed = {
            this->y*that.z - this->z*that.y,
            this->z*that.x - this->x*that.z,
            this->x*that.y - this->y*that.x,
        };
        return crossed.normalize();
    }

    float dot(v3f that) {
        return this->x*that.x + this->y*that.y + this->z*that.z;
    }

    float norm() {
        return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
    }

    v3f normalize() {
        return (v3f){x, y, z} / this->norm();
    }
};
