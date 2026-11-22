#include "GamesEngineeringBase.h"
#include <algorithm>
#include <iostream>
#include "Triangle.h"
#include "Math.h"



int main() {
	GamesEngineeringBase::Window canvas;
	canvas.create(800, 600, "Triangle Rasterization");

	Triangle tri(
		Vec4(400.0f, 100.0f, 0.0f),
		Vec4(150.0f, 500.0f, 0.0f),
		Vec4(650.0f, 500.0f, 0.0f),
		Colour(1.0f, 0.0f, 0.0f),
		Colour(0.0f, 1.0f, 0.0f),
		Colour(0.0f, 0.0f, 1.0f)

	);
	float aspect = (float)canvas.getWidth() / (float)canvas.getHeight();
	Matrix m = Matrix::perspectiveProjMatrix(45, aspect, 0.01, 100000);

	Vec4 screen[3];

	for (int i = 0; i < 3; i++)
	{
		Vec4 clip = m.Mul(tri.v[i]);
		clip.divideByW();
		screen[i] = Vec4::toScreen(clip, (float)canvas.getWidth(), (float)zanvas.getHeight());
	}
	Triangle tri2(screen[0], screen[1], screen[2], Colour(1, 0, 0), Colour(0, 1, 0), Colour(0, 0, 1));

	while (true) {
		canvas.checkInput();
		if (canvas.keyPressed(VK_ESCAPE)) {
			break;
		}

		canvas.clear();

		tri2.draw(canvas);

		canvas.present();
	}

	return 0;
}
