#pragma once

static const char* OrbitG = R"(
	#version 330 core
	layout(points) in;
	layout(line_strip, max_vertices = 256) out;

	uniform mat4 transform;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	uniform int sidesCount;
	uniform float radius;

	const float PI = 3.1415926;

	void main()
	{
		for(int i = 0; i <= sidesCount; i++)
		{
			// Angle between each side in radians
            float ang = PI * 2.0 / sidesCount * i;

            vec4 offset = vec4(cos(ang) * radius, 0.0, -sin(ang) * radius, 0.0);
            gl_Position = projection * view * model * transform * (gl_in[0].gl_Position + offset);

            EmitVertex();
		}
		EndPrimitive();
	}
)";
