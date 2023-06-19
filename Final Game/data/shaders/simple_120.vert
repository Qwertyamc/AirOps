#version 120
attribute vec3 a_vertex;

uniform mat4 u_mvp;

void main()
{
    //calculate the position of the vertex using the matrices
    gl_Position = u_mvp * vec4( a_vertex, 1.0 );
    
}