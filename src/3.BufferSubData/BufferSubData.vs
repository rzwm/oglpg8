﻿#version 430 core

layout (location = 0) in vec2 Positon;
layout (location = 1) in vec4 Color;

out vec4 VertexColor;

void main()
{
	gl_Position = vec4(Positon, 0.0, 1.0);
	VertexColor = Color;
}