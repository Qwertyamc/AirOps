#version 330

in vec3 v_normal;
in vec2 v_uv;
in vec3 v_vertex_world_pos;

out vec4 fragColor;

uniform vec3 u_cam_pos;
uniform vec3 u_light_dir;

uniform sampler2D u_texture;


void main(void)
{
	vec3 N = normalize(v_normal);
	vec3 L = normalize(vec3(1, 1, 1));
	vec3 R = normalize(reflect(-L, N));
	vec3 E = normalize(u_cam_pos - v_vertex_world_pos);

	float NdotL = max(dot(N, L), 0.0);
	float RdotE = max(dot(R, E), 0.0);

	vec3 texture_color = texture(u_texture, v_uv).xyz;

	vec3 ambient_color = texture_color * 0.2;
	vec3 diffuse_color = texture_color * NdotL;
	vec3 specular_color = vec3(1,1,1) * pow(RdotE, 32);

	vec3 final_color = ambient_color +diffuse_color;

	fragColor =  vec4(final_color, 1.0);
}
