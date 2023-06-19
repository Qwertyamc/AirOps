#version 330

in vec2 v_uv;

uniform sampler2D u_texture;

out vec4 fragColor;

void main(void)
{
    vec4 texture_color = texture(u_texture, v_uv);

    fragColor =  vec4(texture_color);
}