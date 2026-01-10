#version 330

layout(location = 0) out vec4 out_color; 

in vec2 frag_texcoord;
in vec3 frag_color;

uniform sampler2D texture1; 
uniform float Time; 
uniform int useTexture; 
uniform int isWater;

void main() {
    if (useTexture == 0) {
        out_color = vec4(frag_color, 1.0);
        return;
    }

    vec2 uv = frag_texcoord;

    if (isWater == 0) {
        out_color = texture(texture1, uv);
        return;
    }

    uv.x += sin(uv.x * 10.0 + Time) * 0.05;
    uv.y += cos(uv.y * 10.0 + Time) * 0.05;
    out_color = texture(texture1, uv);
}
