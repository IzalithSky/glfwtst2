#version 330 core

in vec3 vPos;
in vec3 vCol;
in vec2 vTxt;

out vec3 color;
out vec2 txt;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    color = vCol;
    txt = vTxt;
}
