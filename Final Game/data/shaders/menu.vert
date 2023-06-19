#version 330

in vec3 a_vertex;
in vec2 a_uv;

out vec2 v_uv;

void main()
{
    v_uv = a_uv;

    //calculate the position of the vertex using the matrices
    gl_Position = vec4( a_vertex, 1.0 );
    
}