#version 330
 
in vec3 a_vertex;
in vec2 a_uv;

out vec2 v_uv;

uniform mat4 u_mvp;

void main()
{
	v_uv = a_uv;

	gl_Position = u_mvp * vec4( a_vertex, 1.0 );

}




