#include "Assignment1.h"
#include <iostream>


static void printMat(const Eigen::Matrix4d& mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat(j,i)<<" ";
		std::cout<<std::endl;
	}
}

Assignment1::Assignment1()
{
}

//Assignment1::Assignment1(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}

Eigen::Vector3cf Assignment1::FindCubicRoots()
{
	Eigen::Vector2cf reduceCoeffs = Eigen::Vector2cf::Zero();
	Eigen::Vector3cf roots;
	std::complex<float> bOver3a = (coeffs[1] / coeffs[0]) / 3.0f;
	reduceCoeffs[0] = coeffs[2] / coeffs[0] - 3.0f * bOver3a * bOver3a;
	reduceCoeffs[1] = coeffs[2] / coeffs[0] * bOver3a - coeffs[3] / coeffs[0] - 2.0f * bOver3a * bOver3a * bOver3a;
	// std::cout<<"reduced\n"<<reduceCoeffs<<std::endl;
	if (reduceCoeffs.norm() > 0.000001)
	{
		roots = FindRootsOfReduceEquation(reduceCoeffs);
		roots[0] -= bOver3a;
		roots[1] -= bOver3a;
		roots[2] -= bOver3a;
	}
	else
	{
		roots[0] = -1.0f * bOver3a;
		roots[1] = std::complex<float>(std::cosf(3.14159f / 3.0f), std::sinf(3.14159f / 3.0f)) * bOver3a;
		roots[2] = std::complex<float>(std::cosf(2.0f * 3.14159f / 3.0f), std::sinf(2 * 3.14159f / 3.0f)) * bOver3a;
	}

	return roots;
}

std::complex<float> Assignment1::NewtonCubicRoot(std::complex<float> num)
{
	std::complex<float> root = num;
	const int iter = 7;
	for (int k = 0; k < iter; k++)
	{
		root = (2.0f * root * root * root + num) / root / root / 3.0f;
	}
	return root;
}

Eigen::Vector3cf Assignment1::FindRootsOfReduceEquation(Eigen::Vector2cf reduceCoeffs)
{
	Eigen::Vector3cf roots = Eigen::Vector3cf::Zero();
	std::complex<float> sqroot = std::sqrt(reduceCoeffs[0] * reduceCoeffs[0] * reduceCoeffs[0] / 27.0f + reduceCoeffs[1] * reduceCoeffs[1] / 4.0f);
	std::complex<float> p = NewtonCubicRoot(reduceCoeffs[1] / 2.0f + sqroot);
	std::complex<float> n = NewtonCubicRoot(reduceCoeffs[1] / 2.0f - sqroot);
	roots[0] = p + n;
	roots[1] = p * std::complex<float>(std::cosf(2.0f * 3.14159f / 3.0f), std::sinf(2 * 3.14159f / 3.0f)) - n * std::complex<float>(std::cosf(1.0f * 3.14159f / 3.0f), std::sinf(1 * 3.14159f / 3.0f));
	roots[2] = -p * std::complex<float>(std::cosf(1.0f * 3.14159f / 3.0f), std::sinf(1 * 3.14159f / 3.0f)) + n * std::complex<float>(std::cosf(2.0f * 3.14159f / 3.0f), std::sinf(2 * 3.14159f / 3.0f));
	return roots;
}

void Assignment1::Init()
{		
	unsigned int texIDs[3] = { 0 , 1, 2};
	unsigned int slots[3] = { 0 , 1, 2 };
	coeffs = {3, -5, 7, 80};
	picked_coeff = 1;
	iterationNum = 1;
	
	AddShader("../../shaders/pickingShader");
	AddShader("../../shaders/cubemapShader");
	AddShader("../../shaders/basicShader");
	AddShader("../../shaders/pickingShader");
	AddShader("../../shaders/newtonShader");
	
	AddTexture("../../textures/plane.png",2);
	AddTexture("../../textures/cubemaps/Daylight Box_", 3);
	AddTexture("../../textures/grass.bmp", 2);

	AddMaterial(texIDs,slots, 1);
	AddMaterial(texIDs+1, slots+1, 1);
	AddMaterial(texIDs + 2, slots + 2, 1);
	
	AddShape(Cube, -2, TRIANGLES);
	AddShapeFromFile("../../data/plane.obj", -1, TRIANGLES);

	SetShapeShader(1, 4);
	SetShapeMaterial(1, 0);
	SetShapeMaterial(0, 1);

	pickedShape = 0;
	float s = 60;
	ShapeTransformation(scaleAll, s,0);
	pickedShape = -1;
	SetShapeStatic(0);

//	ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Assignment1::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((shapeIndx+1) & 0x000000FF) >>  0;
	int g = ((shapeIndx+1) & 0x0000FF00) >>  8;
	int b = ((shapeIndx+1) & 0x00FF0000) >> 16;


		s->Bind();
	s->SetUniformMat4f("Proj", Proj);
	s->SetUniformMat4f("View", View);
	s->SetUniformMat4f("Model", Model);
	if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
	{
//		materials[shapes[pickedShape]->GetMaterial()]->Bind(textures);
		BindMaterial(s, data_list[shapeIndx]->GetMaterial());
	}
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor2", r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else {
		Eigen::Vector3cf temp = FindCubicRoots();
		s->SetUniform1f("x1", temp[0].real());
		s->SetUniform1f("y1", temp[0].imag());
		s->SetUniform1f("x2", temp[1].real());
		s->SetUniform1f("y2", temp[1].imag());
		s->SetUniform1f("x3", temp[2].real());
		s->SetUniform1f("y3", temp[2].imag());
	
		//s->SetUniform4f("coeffs", temp[0], temp[1], temp[2], 1);
		//s->SetUniform4f("coeffs", coeffs[0], coeffs[1], coeffs[2], coeffs[3]);
		//s->SetUniform4f("lightColor", 1, 0, 0, 0.5f);
	}
	//textures[0]->Bind(0);

	
	

	//s->SetUniform1i("sampler2", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(1));
	//s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
//	if(shaderIndx == 0)
//		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
//	else 
//		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}


void Assignment1::WhenRotate()
{
}

void Assignment1::WhenTranslate()
{
}

void Assignment1::Animate() {
    if(isActive)
	{
		
	}
}

void Assignment1::ScaleAllShapes(float amt,int viewportIndx)
{
	for (int i = 1; i < data_list.size(); i++)
	{
		if (data_list[i]->Is2Render(viewportIndx))
		{
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
		}
	}
}

Assignment1::~Assignment1(void)
{
}



