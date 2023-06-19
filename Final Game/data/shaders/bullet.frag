#version 330

in vec3 v_normal;
in vec2 v_uv;
in vec3 v_vertex_world_pos;

out vec4 fragColor;

void main(void)
{
	fragColor =  vec4(1.0, 1.0, 0.0, 1.0);
}
