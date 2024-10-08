#version 330 core            // Minimal GL version support expected from the GPU

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoord;

uniform mat4 viewMat, projMat, modelMat;
out vec3 fNormal;
out vec3 fPosition;
out vec2 fTexCoord;

void main() {
        gl_Position = projMat * viewMat * modelMat * vec4(vPosition, 1.0); // mandatory to rasterize properly

        fPosition = (modelMat * vec4(vPosition, 1.0)).xyz;
        fNormal = (transpose(inverse(modelMat))*vec4(vNormal, 1.0)).xyz; // will be passed to the next stage
        fTexCoord = vTexCoord;
}
