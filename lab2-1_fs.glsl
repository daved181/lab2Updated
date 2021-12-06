#version 440

out vec3 frag_colour;
in vec3 normal;
in vec3 position;

void main () {
  
  frag_colour.r = (normal.x+1)*0.5;
  frag_colour.g = (normal.y+1)*0.5;
  frag_colour.b = (normal.z+1)*0.5;
  
};
