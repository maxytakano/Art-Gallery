#include "Bench.h"

Bench::Bench() {
	r = 0;
	g = 1;
	b = 0;
}

Bench::Bench(float r1, float g1, float b1) {
	r = r1;
	g = g1;
	b = b1;
}

void Bench::draw(Matrix4 c) {
		glColor3f(r, g, b);

		Matrix4 result;			// return for glLoadMatrixd

		// Left Bench Leg
		Matrix4 translateLeftLeg;
		Matrix4 scaleLeftLeg;
		translateLeftLeg = translateLeftLeg.translation(-9, 2.5, 0);
		scaleLeftLeg = scaleLeftLeg.scaling(3, 5, 10);
		result = translateLeftLeg.multiply(c);
		result = result.multiply(scaleLeftLeg);

		glLoadMatrixd(result.transpose().getPointer());
		glutSolidCube(1);

		// Right Bench Leg
		Matrix4 translateRightLeg;
		Matrix4 scaleRightLeg;
		translateRightLeg = translateRightLeg.translation(9, 2.5, 0);
		scaleRightLeg = scaleRightLeg.scaling(3, 5, 10);
		result = translateRightLeg.multiply(c);
		result = result.multiply(scaleRightLeg);

		glLoadMatrixd(result.transpose().getPointer());
		glutSolidCube(1);
		
		// Top of the Bench
		Matrix4 translateBenchTop;
		Matrix4 scaleBenchTop;
		translateBenchTop = translateBenchTop.translation(0, 6.5, 0);
		scaleBenchTop = scaleBenchTop.scaling(20, 3, 10);
		result = translateBenchTop.multiply(c);                   // translate, then translate by passed in translate c
		result = result.multiply(scaleBenchTop);					// Then scale after
	
		glLoadMatrixd(result.transpose().getPointer());
		glutSolidCube(1);

}
