#version 330
 
// the following attributes are supported by this engine
// Mesh::render searches for, and binds them, if they exist
in vec3 a_vertex;
in vec3 a_normal;
in vec2 a_uv;
//in vec4 a_color;

out vec3 v_normal;
out vec2 v_uv;
out vec3 v_vertex_world_pos;

uniform mat4 u_model;
uniform mat4 u_mvp;
uniform mat4 u_normal_matrix;

void main()
{
	v_uv = a_uv;
	v_normal = (u_normal_matrix * vec4(a_normal, 1.0)).xyz;
	v_vertex_world_pos = (u_model * vec4(a_normal, 1.0)).xyz;


	//calculate the position of the vertex using the matrices
	gl_Position = u_mvp * vec4( a_vertex, 1.0 );

}




