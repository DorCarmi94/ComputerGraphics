#pragma once
#include "igl/opengl/glfw/Viewer.h"

class Assignment1 : public igl::opengl::glfw::Viewer
{
	
public:
	
	Assignment1();
//	Assignment1(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx);
	void WhenRotate();
	void WhenTranslate();
	void Animate() override;
	void ScaleAllShapes(float amt, int viewportIndx);

	Eigen::Vector3cf FindCubicRoots();

	std::complex<float> NewtonCubicRoot(std::complex<float> num);

	Eigen::Vector3cf FindRootsOfReduceEquation(Eigen::Vector2cf reduceCoeffs);
	
	~Assignment1(void);
	
	Eigen::Vector4cf coeffs;
	int picked_coeff;
	int iterationNum;
};


