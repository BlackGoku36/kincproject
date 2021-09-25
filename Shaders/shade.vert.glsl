#version 450

in vec3 pos;
in vec3 nor;

out vec3 fragPos;
out vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform mat4 mvp;

void main() {
    fragPos = vec3(M * vec4(pos, 1.0));
    normal = nor;
    gl_Position = P * V * M * vec4(pos, 1.0);
}
