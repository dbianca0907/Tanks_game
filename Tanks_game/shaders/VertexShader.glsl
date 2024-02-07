#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float translateX;
uniform float translateZ;


// Output
// TODO(student): Output values to fragment shader
out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;


void main()
{
    // Send output to fragment shader
    frag_normal = v_normal;
    frag_color = v_color;
    tex_coord = v_texture_coord;
    
    // Compute gl_Position
    vec3 relativeVec = vec3(translateX, 0.5f, translateZ) -  vec3( Model * vec4(v_position, 1.0f));
    vec3 world_pos =  vec3 (Model * vec4(v_position, 1));
    float newPosY =  world_pos.y -  length(relativeVec) * length(relativeVec) * 0.03;
    vec3 newVec = vec3(world_pos.x, newPosY, world_pos.z);


    gl_Position = Projection * View * vec4(world_pos, 1.0f);
}
