#version 330

layout(location = 0) out vec4 out_color; 

in vec2 frag_texcoord;
in vec3 frag_color;
in vec3 frag_position;

uniform sampler2D texture1; 
uniform float Time; 
uniform int useTexture; 
uniform int isWater;
uniform int isMountain;

void main() {
    if (useTexture == 0) {
        vec3 pos = frag_position;

        if (isMountain == 1) {
            if (pos.x >= 12.35 && pos.x <= 13.65 &&
                pos.z >= -18.0 && pos.z <= 9.0 &&
                pos.y >= 0.5 && pos.y <= 3.2)
                {
                    discard;
                }
            }

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
