#pragma once

static const char* s_sphereGeometryPtr = R"(
	#version 330 core
	#extension GL_EXT_geometry_shader4: enable
	layout(triangles) in;
	layout(triangle_strip, max_vertices = 256) out;

	uniform mat4 transform;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	uniform int meshDetailLevel;
	uniform float radius;

	const float PI = 3.1415926;
	vec3 V0, V01, V02;

	void ProduceVertex( float s, float t )
	{
		vec3 v = V0 + s*V01 + t*V02;
		v = normalize(v);
		gl_Position = projection * view * model * transform * vec4( (radius*v), 1.0 );
		EmitVertex( );
	}

	void main()
	{
		V01 = ( gl_PositionIn[1] - gl_PositionIn[0] ).xyz;
		V02 = ( gl_PositionIn[2] - gl_PositionIn[0] ).xyz;
		V0 = gl_PositionIn[0].xyz;
		int numLayers = 1;
		for(int i = 0; i < meshDetailLevel; i++)
		{
			numLayers *= 2;
		}
		float numLayersReciprocal = 1.0 / float( numLayers );
		float t_top = 1.0;

		for( int it = 0; it < numLayers; it++ )
		{
			float t_bot = t_top - numLayersReciprocal;
			float smax_top = 1.0 - t_top;
			float smax_bot = 1.0 - t_bot;
			int nums = it + 1;
			float ds_top = smax_top / float( nums - 1 );
			float ds_bot = smax_bot / float( nums );
			float s_top = 0.0;
			float s_bot = 0.0;
			for( int is = 0; is < nums; is++ )
			{
				ProduceVertex( s_bot, t_bot );
				ProduceVertex( s_top, t_top );
				s_top += ds_top;
				s_bot += ds_bot;
			}
			ProduceVertex( s_bot, t_bot );
			EndPrimitive( );
			t_top = t_bot;
			t_bot -= numLayersReciprocal;
		}
	}
)";
