#version 330

in vec2 v_uv;

out vec4 fragColor;

uniform sampler2D u_texture;


void main(void)
{
	fragColor =  vec4(texture(u_texture, v_uv).xyz, 1.0);
}
