#version 400

layout(location=0) in vec4 vp;
layout(location=1) in vec4 np;
out vec3 position;
out vec3 normal;
uniform mat4 mvp;
uniform mat4 viewModel;

void main () {
  
  gl_Position = mvp*vp;
  position = vec3(mvp * vp);
  normal = vec3(viewModel * np);
};
