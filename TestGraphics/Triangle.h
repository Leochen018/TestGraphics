#include "GamesEngineeringBase.h"
#include <algorithm>
#include <iostream>
#include "Math.h"


struct Colour {
	float r, g, b;
	Colour(float _r = 0.0f, float _g = 0.0f, float _b = 0.0f)
		: r(_r), g(_g), b(_b) {
	}

	Colour operator*(float scalar) const {
		return Colour(r * scalar, g * scalar, b * scalar);
	}

	Colour operator+(const Colour& other) const {
		return Colour(r + other.r, g + other.g, b + other.b);
	}
};
class Triangle {
public:
	Vec4 v[3];
	Colour c[3];

	Triangle(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Colour& c0, const Colour& c1, const Colour& c2) {
		v[0] = v0; v[1] = v1; v[2] = v2;
		c[0] = c0; c[1] = c1; c[2] = c2;
	}

	float edgeFunction(const Vec4& v0, const Vec4& v1, const Vec4& p) const {
		return (((p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x)));
	}

	void findBounds(unsigned int width, unsigned int height, Vec4& tr, Vec4& bl) const {
		tr.x = std::min(std::max(std::max(v[0].x, v[1].x), v[2].x), static_cast<float>(width - 1));
		tr.y = std::min(std::max(std::max(v[0].y, v[1].y), v[2].y), static_cast<float>(height - 1));
		bl.x = std::max(std::min(std::min(v[0].x, v[1].x), v[2].x), 0.0f);
		bl.y = std::max(std::min(std::min(v[0].y, v[1].y), v[2].y), 0.0f);
	}

	template <typename T>
	T simpleInterpolateAttribute(const T& a0, const T& a1, const T& a2, float alpha, float beta, float gamma) const {
		return (a0 * alpha) + (a1 * beta) + (a2 * gamma);
	}

	void draw(GamesEngineeringBase::Window& canvas) const {
		Vec4 tr, bl;
		findBounds(canvas.getWidth(), canvas.getHeight(), tr, bl);

		float projArea = edgeFunction(v[0], v[1], v[2]);
		if (projArea == 0.0f) return;
		float area = 1.0f / projArea;

		for (int y = static_cast<int>(bl.y); y <= static_cast<int>(tr.y); y++) {
			for (int x = static_cast<int>(bl.x); x <= static_cast<int>(tr.x); x++) {
				Vec4 p(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f, 0.0f);

				float alpha = edgeFunction(v[1], v[2], p);
				float beta = edgeFunction(v[2], v[0], p);
				float gamma = edgeFunction(v[0], v[1], p);

				if ((alpha >= 0 && beta >= 0 && gamma >= 0) || (alpha <= 0 && beta <= 0 && gamma <= 0)) {
					float normAlpha = alpha * area;
					float normBeta = beta * area;
					float normGamma = gamma * area;

					Colour frag = simpleInterpolateAttribute(c[0], c[1], c[2], normAlpha, normBeta, normGamma);
					unsigned char red = static_cast<unsigned char>(std::clamp(frag.r, 0.0f, 1.0f) * 255);
					unsigned char green = static_cast<unsigned char>(std::clamp(frag.g, 0.0f, 1.0f) * 255);
					unsigned char blue = static_cast<unsigned char>(std::clamp(frag.b, 0.0f, 1.0f) * 255);

					canvas.draw(x, y, red, green, blue);
				}
			}
		}
	}

};