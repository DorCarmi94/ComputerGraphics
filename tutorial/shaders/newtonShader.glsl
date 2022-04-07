#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;


uniform sampler2D sampler1;
uniform float x1;
uniform float y1;
uniform float x2;
uniform float y2;
uniform float x3;
uniform float y3;
uniform int iterNum;
uniform vec4 coeffs;


out vec4 Color;

void main()
{
	//red = (x1, y1)
	//green = (x2, y2)
	//blue = (x3, y3)
	float a = coeffs[0];
	float b = coeffs[1];	
	float c = coeffs[2];
	float d = coeffs[3];
	float root_x1 = x1;
	float root_y1 = y1;
	float root_x2 = x2;
	float root_y2 = y2;
	float root_x3 = x3;
	float root_y3 = y3;

	for(int i = 0; i < iterNum; i++){
		root_x1 = a*root_x1 - ((a*(pow(root_x1, 3) - 3*root_x1*root_y1) + b*(pow(root_x1, 2) - root_y1) + c*root_x1 + d) / (3*a*(pow(root_x1, 2)-root_y1) + 2*b*root_x1 + c));
		root_x2 = a*root_x2 - ((a*(pow(root_x2, 3) - 3*root_x2*root_y2) + b*(pow(root_x2, 2) - root_y2) + c*root_x2 + d) / (3*a*(pow(root_x2, 2)-root_y2) + 2*b*root_x2 + c));
		root_x3 = a*root_x3 - ((a*(pow(root_x3, 3) - 3*root_x3*root_y3) + b*(pow(root_x3, 2) - root_y3) + c*root_x3 + d) / (3*a*(pow(root_x3, 2)-root_y3) + 2*b*root_x3 + c));
		//x2 = a*x2 - ((a*(pow(x2, 3) - 3*x2*y2) + b*(pow(x2, 2) - y2) + c*x2 + d) / (3*a*(pow(x2, 2)-y2) + 2*b*x2 + c));

		root_y1 = a*root_y1 - ((a*(pow(root_y1, 3) + 3*pow(root_x1, 2)*root_y1) + b*2*root_x1*root_y1 + c*root_y1) / (3*a*2*root_x1*root_y1 + 2*b*root_y1 + c));
		root_y2 = a*root_y2 - ((a*(pow(root_y2, 3) + 3*pow(root_x2, 2)*root_y2) + b*2*root_x2*root_y2 + c*root_y2) / (3*a*2*root_x2*root_y2 + 2*b*root_y2 + c));
		root_y3 = a*root_y3 - ((a*(pow(root_y3, 3) + 3*pow(root_x3, 2)*root_y3) + b*2*root_x3*root_y3 + c*root_y3) / (3*a*2*root_x3*root_y3 + 2*b*root_y3 + c));
		//y2 = a*y2 - ((a*(pow(y2, 3) + 3*pow(x2, 2)*y2) + b*2*x2*y2 + c*y2) / (3*a*2*x2*y2 + 2*b*y2 + c));
	}
	float min_red = sqrt(pow(position0.x - root_x1, 2) + (pow(position0.y - root_y1, 2)));
	float min_green = sqrt(pow(position0.x - root_x2, 2) + (pow(position0.y - root_y2, 2)));
	float min_blue = sqrt(pow(position0.x - root_x3, 2) + (pow(position0.y - root_y3, 2)));

	float min = min_green;
	Color = vec4(0, 1, 0, 1.0);
	if(min > min_red){
		min = min_red;
		Color = vec4(1, 0, 0, 1.0);
	}
	if(min > min_blue){
		min = min_blue;
		Color = vec4(0, 0, 1, 1.0);
	}
}
