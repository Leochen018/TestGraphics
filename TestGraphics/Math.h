#pragma once
#include <cmath>
#include <algorithm>
#include <string>

#define Square(x) ((x) * (x))

template <typename T>
static T lerp(const T a, const T b, const float time) {
    return a * (1.0f - time) + (b * time);
}

class Vec3 {
public:
    union {
        float v[3];
        struct { float x, y, z; };
    };

    Vec3() : x(0), y(0), z(0) {}

    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float LengthSquare() const {
        return (x * x) + (y * y) + (z * z);
    }

    float Length() const {
        return sqrtf(LengthSquare());
    }

    Vec3 Normalize() const {
        float length = Length();
        if (length == 0.0f) return Vec3(0.0f, 0.0f, 0.0f);

        return Vec3(x / length, y / length, z / length);
    }

    float Normalize_GetLength() {
        float length = Length();

        if (length == 0.0f) return 0.0f;

        x /= length;
        y /= length;
        z /= length;

        return length;
    }

    float Dot(const Vec3& pVec) const {
        return (x * pVec.x) + (y * pVec.y) + (z * pVec.z);
    }

    Vec3 Cross(const Vec3& pVec) const {
        return Vec3(
            (y * pVec.z) - (z * pVec.y),
            (z * pVec.x) - (x * pVec.z),
            (x * pVec.y) - (y * pVec.x)
        );
    }

    float Min() const {
        return std::min({ x, y, z });
    }

    float Max() const {
        return std::max({ x, y, z });
    }

    std::string ToString() const {
        return "{" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "}";
    }

    // Overload Operators
    Vec3 operator+(const Vec3& pVec) const {
        return Vec3(x + pVec.x, y + pVec.y, z + pVec.z);
    }

    Vec3& operator+=(const Vec3& pVec) {
        x += pVec.x;
        y += pVec.y;
        z += pVec.z;

        return *this;
    }

    Vec3 operator-(const Vec3& pVec) const {
        return Vec3(x - pVec.x, y - pVec.y, z - pVec.z);
    }

    Vec3& operator-=(const Vec3& pVec) {
        x -= pVec.x;
        y -= pVec.y;
        z -= pVec.z;

        return *this;
    }

    Vec3 operator*(const Vec3& pVec) const {
        return Vec3(x * pVec.x, y * pVec.y, z * pVec.z);
    }

    Vec3 operator*(const float value) const {
        return Vec3(x * value, y * value, z * value);
    }

    Vec3& operator*=(const Vec3& pVec) {
        x *= pVec.x;
        y *= pVec.y;
        z *= pVec.z;

        return *this;
    }

    Vec3& operator*=(const float value) {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    Vec3 operator/(const Vec3& pVec) const {
        return Vec3(x / pVec.x, y / pVec.y, z / pVec.z);
    }

    Vec3 operator/(const float value) const {
        return Vec3(x / value, y / value, z / value);
    }

    Vec3& operator/=(const Vec3& pVec) {
        x /= pVec.x;
        y /= pVec.y;
        z /= pVec.z;

        return *this;
    }

    Vec3& operator/=(const float value) {
        x /= value;
        y /= value;
        z /= value;

        return *this;
    }
};

inline float Dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 Cross(const Vec3& a, const Vec3& b) {
    return Vec3(
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)
    );
}

inline Vec3 Min(const Vec3& a, const Vec3& b) {
    return Vec3(
        std::min(a.x, b.x),
        std::min(a.y, b.y),
        std::min(a.z, b.z)
    );
}

inline Vec3 Max(const Vec3& a, const Vec3& b) {
    return Vec3(
        std::max(a.x, b.x),
        std::max(a.y, b.y),
        std::max(a.z, b.z)
    );
}




class Vec4 {
public:
    union {
        float v[4];
        struct { float x, y, z, w; };
    };

   
    Vec4(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 1.0f)

        : x(_x), y(_y), z(_z), w(_w) {

    }

    void divideByW()
    {
        if (w == 0) return;

        float inW = 1 / w;
        x = x * inW;
        y = y * inW;
        z = z * inW;
        w = inW;

    }

    static Vec4 toScreen(Vec4 vertex, float width, float height)
    {
        float screenX = ((vertex.x + 1) / 2) * width;
        float screenY = (1 - (vertex.y + 1) / 2) * height; //flip y
        return Vec4(screenX, screenY, vertex.z, vertex.w);
    }
};

class Matrix {
private:
    float Det2(float data[4]) const {
        return (data[0] * data[3]) - (data[1] * data[2]);
    }

    float Det3(float data[9]) const {
        float matrix2A[4] = {
            data[4], data[5],
            data[7], data[8]
        };
        float matrix2B[4] = {
            data[3], data[5],
            data[6], data[8]
        };
        float matrix2C[4] = {
            data[3], data[4],
            data[6], data[7]
        };

        return (data[0] * Det2(matrix2A)) - (data[1] * Det2(matrix2B)) + (data[2] * Det2(matrix2C));
    }

    Matrix Mul(const Matrix& matrix) const {
        Matrix ret;
        ret.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
        ret.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
        ret.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
        ret.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];
        ret.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
        ret.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
        ret.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
        ret.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];
        ret.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
        ret.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
        ret.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
        ret.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];
        ret.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
        ret.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
        ret.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
        ret.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
        return ret;
    }

public:
    union {
        float a[4][4];
        float m[16];
    };
    //constructor
    Matrix(const float values[16]) {
        m[0] = values[0]; m[1] = values[1]; m[2] = values[2]; m[3] = values[3];
        m[4] = values[4]; m[5] = values[5]; m[6] = values[6]; m[7] = values[7];
        m[8] = values[8]; m[9] = values[9]; m[10] = values[10]; m[11] = values[11];
        m[12] = values[12]; m[13] = values[13]; m[14] = values[14]; m[15] = values[15];
    }

    Matrix() {
        m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
        m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
        m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
    }

    Matrix(float num)
    {
        for (int i = 0; i < 16; i++)
        {
            m[i] = num;
        }
    }
    // Martix Transformations
    static Matrix Translate(float x, float y, float z) {
        Matrix translate;

        translate.a[0][3] = x;
        translate.a[1][3] = y;
        translate.a[2][3] = z;

        return translate;
    }

    static Matrix RotateX(float angle) {
        float cos = cosf(angle);
        float sin = sinf(angle);

        Matrix rotateX;

        rotateX.a[1][1] = cos;
        rotateX.a[1][2] = -sin;
        rotateX.a[2][1] = sin;
        rotateX.a[2][2] = cos;

        return rotateX;
    }

    static Matrix RotateY(float angle) {
        float cos = cosf(angle);
        float sin = sinf(angle);

        Matrix rotateY;

        rotateY.a[0][0] = cos;
        rotateY.a[0][2] = sin;
        rotateY.a[2][0] = -sin;
        rotateY.a[2][2] = cos;

        return rotateY;
    }

    static Matrix RotateZ(float angle) {
        float cos = cosf(angle);
        float sin = sinf(angle);

        Matrix rotateZ;

        rotateZ.a[0][0] = cos;
        rotateZ.a[0][1] = -sin;
        rotateZ.a[1][0] = sin;
        rotateZ.a[1][1] = cos;

        return rotateZ;
    }

    static Matrix Scale(float value) {
        Matrix scale;

        scale.a[0][0] = value;
        scale.a[1][1] = value;
        scale.a[2][2] = value;

        return scale;
    }

    static Matrix Scale(float x, float y, float z) {
        Matrix scale;

        scale.a[0][0] = x;
        scale.a[1][1] = y;
        scale.a[2][2] = z;

        return scale;
    }

    // Martix Operators
    static Matrix CreateBasis(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d) {
        Matrix basis;
        basis.a[0][0] = a.x; basis.a[0][1] = b.x; basis.a[0][2] = c.x; basis.a[0][3] = d.x;
        basis.a[1][0] = a.y; basis.a[1][1] = b.y; basis.a[1][2] = c.y; basis.a[1][3] = d.y;
        basis.a[2][0] = a.z; basis.a[2][1] = b.z; basis.a[2][2] = c.z; basis.a[2][3] = d.z;
        return basis;
    }

    Matrix Transpose() const {
        float values[16] = {
            a[0][0], a[1][0], a[2][0], a[3][0],
            a[0][1], a[1][1], a[2][1],  a[3][1],
            a[0][2], a[1][2], a[2][2], a[3][2],
            a[0][3], a[1][3], a[2][3], a[3][3]
        };

        return Matrix(values);
    }

    float Det() const {
        float matrix3A[9] = {
            m[5], m[6], m[7],
            m[9], m[10], m[11],
            m[13], m[14], m[15]
        };

        float matrix3B[9] = {
            m[4], m[6], m[7],
            m[8], m[10], m[11],
            m[12], m[14], m[15]
        };

        float matrix3C[9] = {
            m[4], m[5], m[7],
            m[8], m[9], m[11],
            m[12], m[13], m[15]
        };

        float matrix3D[9] = {
            m[4], m[5], m[6],
            m[8], m[9], m[10],
            m[12], m[13], m[14]
        };

        return (m[0] * Det3(matrix3A)) - (m[1] * Det3(matrix3B)) + (m[2] * Det3(matrix3C)) - (m[3] * Det3(matrix3D));
    }

    Matrix invert() const {
        Matrix inv;
        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

        float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        if (det == 0) {
            // Handle this case
            return Matrix();
        }

        float inverseDet = 1.0f / det;
        for (int i = 0; i < 16; i++)
            inv[i] = inv[i] * inverseDet;

        return inv;
    }

    Vec4 Mul(const Vec4& vector) const {
        return Vec4(
            m[0] * vector.x + m[1] * vector.y + m[2] * vector.z + m[3] * vector.w,
            m[4] * vector.x + m[5] * vector.y + m[6] * vector.z + m[7] * vector.w,
            m[8] * vector.x + m[9] * vector.y + m[10] * vector.z + m[11] * vector.w,
            m[12] * vector.x + m[13] * vector.y + m[14] * vector.z + m[15] * vector.w
        );
    }

    Vec3 MulPoint(const Vec3& vector) const {
        return Vec3(
            m[0] * vector.x + m[1] * vector.y + m[2] * vector.z + m[3],
            m[4] * vector.x + m[5] * vector.y + m[6] * vector.z + m[7],
            m[8] * vector.x + m[9] * vector.y + m[10] * vector.z + m[11]
        );
    }

    Vec3 MulVec(const Vec3& vector) const {
        return Vec3(
            m[0] * vector.x + m[1] * vector.y + m[2] * vector.z,
            m[4] * vector.x + m[5] * vector.y + m[6] * vector.z,
            m[8] * vector.x + m[9] * vector.y + m[10] * vector.z
        );
    }

    // Operator Overload
    float& operator[](int index) {
        return m[index];
    }

    const float& operator[](int index) const {
        return m[index];
    }

    Matrix operator*(const Matrix& matrix) const {
        return Mul(matrix);
    }

    Matrix& operator*=(const Matrix& matrix) {
        *this = Mul(matrix);
        return *this;
    }

    static Matrix perspectiveProjMatrix(float fovDegrees, float aspect, float farPlane, float nearPlane)
    {
        
        float fovRadians = fovDegrees * M_PI / 180;
        float t = tan(fovRadians / 2);
        Matrix m(0.0f);
        m[0] = 1 / (aspect * t);
        m[5] = 1 / t;
        m[10] = farPlane / (farPlane - nearPlane);
        m[11] = (-farPlane * nearPlane) / (farPlane - nearPlane);
        m[14] = 1;
        return m;

    }
};

class ShadingFrame {
public:
    Vec3 u, v, n;

    ShadingFrame(const Vec3& normal) : u(0, 0, 0), v(0, 0, 0), n(normal.Normalize()) {
        Vec3 colinearAxis = (n.x != 1.0f || n.y != 0.0f || n.z != 0.0f) ? Vec3(1, 0, 0) : Vec3(0, 1, 0);
        u = Cross(n, colinearAxis).Normalize();
        v = Cross(n, u).Normalize();
    }

    /* Matrix Visualization:

    |u.x, v.x, n.x|   | x |
    |u.y, v.y, n.y| * | y |
    |u.z, v.z, n.z|   | z |
    */
    Vec3 ForwardTransform(const Vec3& vector) const {
        return (u * vector.x) + (v * vector.y) + (n * vector.z);
    }

    /* Matrix Visualization:

    |u.x, u.y, u.z|   | x |
    |v.x, v.y, v.z| * | y |
    |n.x, n.y, n.z|   | z |
    */
    Vec3 InverseTransform(const Vec3& vector) const {
        return Vec3(
            Dot(u, vector),
            Dot(v, vector),
            Dot(n, vector)
        );
    }
};

class SphereCoord {
public:
    union {
        float v[3];
        struct { float theta, phi, r; };
    };

    bool isCamera; // true if Y Up, false is Z Up

    SphereCoord(const Vec3& vector, float r_ = 1.0f, bool isCamera_ = false) : r(r_), isCamera(isCamera_) {
        if (r <= 0.0f || vector.LengthSquare() == 0.0f) {
            theta = 0.0f;
            phi = 0.0f;
            return;
        }

        Vec3 normalizedVector = vector.Normalize();
        theta = acosf((isCamera) ? normalizedVector.y : normalizedVector.z);
        phi = atan2f((isCamera) ? normalizedVector.z : normalizedVector.y, normalizedVector.x);
    }

    Vec3 GetCartesian() const {
        float x = r * sinf(theta) * cos(phi);
        float y = (isCamera) ? (r * cosf(theta)) : (r * sinf(theta) * sin(phi));
        float z = (isCamera) ? (r * sinf(theta) * sin(phi)) : (r * cosf(theta));

        return Vec3(x, y, z);
    }
};

class Quaternion {
public:
    union {
        float q[4];
        struct { float a, b, c, d; }; // Q = d + ai + bj + ck
    };

    Quaternion() : a(0.0f), b(0.0f), c(0.0f), d(1.0f) {}
    Quaternion(float a_, float b_, float c_, float d_) : a(a_), b(b_), c(c_), d(d_) {}
    Quaternion(const Vec3& vector, float angle) {
        if (vector.LengthSquare() == 0.0f) {
            *this = Quaternion();
            return;
        }

        float s = sinf(angle / 2.0f);
        Vec3 normalizedVector = vector.Normalize();

        a = normalizedVector.x * s;
        b = normalizedVector.y * s;
        c = normalizedVector.z * s;
        d = cosf(angle / 2.0f);
    }

    float MagnitudeSquared() const {
        return (a * a) + (b * b) + (c * c) + (d * d);
    }

    float Magnitude() const {
        return sqrtf(MagnitudeSquared());
    }

    Quaternion Normalize() const {
        float magnitude = Magnitude();

        if (magnitude == 0.0f) return Quaternion();

        return Quaternion(
            a / magnitude,
            b / magnitude,
            c / magnitude,
            d / magnitude
        );
    }

    // Q* = d - ai - bj - dk
    Quaternion Conjugate() const {
        return Quaternion(-a, -b, -c, d);
    }

    // Q^-1 = Q* / |Q|^2
    Quaternion Inverse() const {
        float magnitudeSquared = MagnitudeSquared();

        if (magnitudeSquared == 0.0f) return Quaternion();

        return Quaternion(
            -a / magnitudeSquared,
            -b / magnitudeSquared,
            -c / magnitudeSquared,
            d / magnitudeSquared
        );
    }

    Matrix ToMatrix() const {
        float aa = a * a, ab = a * b, ac = a * c;
        float bb = b * b, bc = b * c, cc = c * c;
        float da = d * a, db = d * b, dc = d * c;

        Matrix m;
        m[0] = 1 - 2 * (bb + cc); m[1] = 2 * (ab - dc); m[2] = 2 * (ac + db); m[3] = 0;
        m[4] = 2 * (ab + dc); m[5] = 1 - 2 * (aa + cc); m[6] = 2 * (bc - da); m[7] = 0;
        m[8] = 2 * (ac - db); m[9] = 2 * (bc + da); m[10] = 1 - 2 * (aa + bb); m[11] = 0;
        m[12] = m[13] = m[14] = 0; m[15] = 1;

        return m;
    }

    float Dot(const Quaternion& quaternion) const {
        return d * quaternion.d + a * quaternion.a + b * quaternion.b + c * quaternion.c;
    }

    // Operation Overload
    Quaternion operator+(const Quaternion& quaternion) const {
        return Quaternion(
            a + quaternion.a,
            b + quaternion.b,
            c + quaternion.c,
            d + quaternion.d
        );
    }

    Quaternion& operator+=(const Quaternion& quaternion) {
        a += quaternion.a;
        b += quaternion.b;
        c += quaternion.c;
        d += quaternion.d;
        return *this;
    }

    Quaternion operator-(const Quaternion& quaternion) const {
        return Quaternion(
            a - quaternion.a,
            b - quaternion.b,
            c - quaternion.c,
            d - quaternion.d
        );
    }

    Quaternion& operator-=(const Quaternion& quaternion) {
        a -= quaternion.a;
        b -= quaternion.b;
        c -= quaternion.c;
        d -= quaternion.d;
        return *this;
    }

    Quaternion operator*(const Quaternion& quaternion) const {
        return Quaternion(
            a * quaternion.d + d * quaternion.a + b * quaternion.c - c * quaternion.b,
            b * quaternion.d + d * quaternion.b + c * quaternion.a - a * quaternion.c,
            c * quaternion.d + d * quaternion.c + a * quaternion.b - b * quaternion.a,
            d * quaternion.d - a * quaternion.a - b * quaternion.b - c * quaternion.c
        );
    }

    Quaternion& operator*=(const Quaternion& quaternion) {
        *this = *this * quaternion;
        return *this;
    }

    Quaternion operator*(float value) const {
        return Quaternion(
            a * value,
            b * value,
            c * value,
            d * value
        );
    }

    Quaternion& operator*=(float value) {
        a *= value;
        b *= value;
        c *= value;
        d *= value;

        return *this;
    }

};

inline float Dot(const Quaternion& a, const Quaternion& b) {
    return a.d * b.d + a.a * b.a + a.b * b.b + a.c * b.c;
}

inline Quaternion Slerp(Quaternion from, Quaternion to, float time) {
    float dotProduct = Dot(from, to);
    if (dotProduct < 0.0f) {
        to = to * -1.0f;
        dotProduct = -dotProduct;
    }

    float angle = acosf(dotProduct);
    float sin = sinf(angle);

    return from * (sinf(angle * (1 - time)) / sin) + to * (sinf(angle * time) / sin);
}

class Color {
public:
    union {
        float c[4];
        struct { float r, g, b, a; };
    };

    // Operator Overload
    Color(float r_, float g_, float b_, float a_ = 1.0f) : r(r_), g(g_), b(b_), a(a_) {
        r = std::clamp(r, 0.0f, 1.0f);
        g = std::clamp(g, 0.0f, 1.0f);
        b = std::clamp(b, 0.0f, 1.0f);
        a = std::clamp(a, 0.0f, 1.0f);
    }

    Color(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_ = 255) : r(r_ / 255.0f), g(g_ / 255.0f), b(b_ / 255.0f), a(a_ / 255.0f) {}

    Color operator+ (const Color& color) const {
        return Color(
            std::clamp(r + color.r, 0.0f, 1.0f),
            std::clamp(g + color.g, 0.0f, 1.0f),
            std::clamp(b + color.b, 0.0f, 1.0f),
            std::clamp(a + color.a, 0.0f, 1.0f)
        );
    }

    Color& operator+= (const Color& color) {
        r = std::clamp(r + color.r, 0.0f, 1.0f);
        g = std::clamp(g + color.g, 0.0f, 1.0f);
        b = std::clamp(b + color.b, 0.0f, 1.0f);
        a = std::clamp(a + color.a, 0.0f, 1.0f);

        return *this;
    }

    Color operator* (const Color& color) const {
        return Color(
            std::clamp(r * color.r, 0.0f, 1.0f),
            std::clamp(g * color.g, 0.0f, 1.0f),
            std::clamp(b * color.b, 0.0f, 1.0f),
            std::clamp(a * color.a, 0.0f, 1.0f)
        );
    }

    Color& operator*= (const Color& color) {
        r = std::clamp(r * color.r, 0.0f, 1.0f);
        g = std::clamp(g * color.g, 0.0f, 1.0f);
        b = std::clamp(b * color.b, 0.0f, 1.0f);
        a = std::clamp(a * color.a, 0.0f, 1.0f);

        return *this;
    }

    Color operator* (const float a_) const {
        return Color(
            std::clamp(r * a_, 0.0f, 1.0f),
            std::clamp(g * a_, 0.0f, 1.0f),
            std::clamp(b * a_, 0.0f, 1.0f),
            a
        );
    }

    Color& operator*= (const float a_) {
        r = std::clamp(r * a_, 0.0f, 1.0f);
        g = std::clamp(g * a_, 0.0f, 1.0f);
        b = std::clamp(b * a_, 0.0f, 1.0f);

        return *this;
    }

    Color operator/ (const float a_) const {
        // Check for zero-division
        if (a_ == 0.0f)
            return Color(0, 0, 0, a);

        return Color(
            std::clamp(r / a_, 0.0f, 1.0f),
            std::clamp(g / a_, 0.0f, 1.0f),
            std::clamp(b / a_, 0.0f, 1.0f),
            a
        );
    }

    Color& operator/= (const float a_) {
        // Check for zero-division
        if (a_ == 0.0f) {
            r = g = b = 0.0f;
            return *this;
        }


        r = std::clamp(r / a_, 0.0f, 1.0f);
        g = std::clamp(g / a_, 0.0f, 1.0f);
        b = std::clamp(b / a_, 0.0f, 1.0f);

        return *this;
    }
};