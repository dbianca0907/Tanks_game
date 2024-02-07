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
uniform float hp;


// Output
// TODO(student): Output values to fragment shader
out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;

vec3 calculateVertexDeformation(vec3 vertex, float damage) {

    // Aplică deformarea folosind funcțiile sinus și cosinus
    vec3 deformed_position = vertex + 0.1 * vec3(
        sin(vertex.x * (90.0) * damage),
        sin(vertex.y * (20.0) * damage),
        cos(vertex.z * (-60.0) * damage)
    );

    return deformed_position;
}


void main()
{
    // Send output to fragment shader
    frag_normal = v_normal;
    frag_color = v_color;
    tex_coord = v_texture_coord;

     // Compute world position
    //vec4 worldPos = Model * vec4(v_position, 1.0f);

    // Calculate vertex deformation
    vec3 deformed_position = calculateVertexDeformation(v_position, hp);
    //vec3 deformed_position = 0;

    //vec3 damagedPosition = worldPos.xyz + deformed_position;

    vec4 worldPos = Model * vec4(deformed_position, 1.0f);

    gl_Position = Projection * View * vec4(worldPos.xyz, 1.0f);
}
