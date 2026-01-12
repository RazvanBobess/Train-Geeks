#version 330 

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 frag_texcoord;
out vec3 frag_color;
out vec3 frag_position;
out vec3 frag_normal;

void main() {
    vec3 world_position = (Model * vec4(v_position, 1)).xyz;
    frag_position = world_position;

    mat3 normalMatrix = transpose(inverse(mat3(Model)));
    frag_normal = normalize(normalMatrix * v_normal);

    frag_texcoord = v_texture_coord;
    frag_color = v_color;
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}