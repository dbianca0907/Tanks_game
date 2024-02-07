#version 330

// Input
in vec3 frag_normal;
in vec2 tex_coord;

// Uniform properties
uniform sampler2D texture_1;
uniform float hp; 

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    out_color = texture(texture_1, tex_coord);
       //innegreste culoarea tancului
    if (hp > 0) {
        out_color += vec4((hp - 1) * 25 * 0.01f, 0.0, 0.0, 1.0);
    }

    // Verifică dacă transparența pixelului este mai mică decât 0.9 și dacă nivelul de sănătate este sub un anumit prag
    if (out_color.a < 0.9)
    {
        discard; // Închide culoarea tancului 

    }
}
