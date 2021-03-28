#pragma once

static const char* LineG = R"(
	#version 330 core
	layout(points) in;
	layout(line_strip, max_vertices = 256) out;

	uniform mat4 transform;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	uniform vec3 endPointPos;

	void main()
	{
		gl_Position = projection * view * model * transform * (gl_in[0].gl_Position);
        EmitVertex();
		
        gl_Position = projection * view * model * transform * (gl_in[0].gl_Position + vec4(endPointPos, 1.0));
        EmitVertex();

		EndPrimitive();
	}
)";
