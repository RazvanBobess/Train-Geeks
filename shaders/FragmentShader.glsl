#version 330

layout(location = 0) out vec4 out_color; 

in vec2 frag_texcoord;
in vec3 frag_color;
in vec3 frag_position;
in vec3 frag_normal;

uniform sampler2D texture1; 
uniform float Time; 
uniform int useTexture; 
uniform int isWater;
uniform int isMountain;

uniform float snowHeight;
uniform float snowFade;
uniform float snowSlopeLimit;

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

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
            if (pos.y >= 6.0) {
                out_color = vec4(1.0, 1.0, 1.0, 1.0);

                return;
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
