#version 430 core

layout (location = 0) in vec2 Positioin;

out vec4 Color;

subroutine vec4 FUNC_color();

subroutine (FUNC_color) vec4 Red()
{
	return vec4(1.0, 0.0, 0.0, 1.0);
}

subroutine (FUNC_color) vec4 Blue()
{
	return vec4(0.0, 0.0, 1.0, 1.0);
} 

subroutine uniform FUNC_color selected_func_color;

void main()
{
	gl_Position = vec4(Positioin, 0.0, 1.0);
	Color = selected_func_color();
}