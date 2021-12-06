#version 440

out vec3 frag_colour;
in vec3 normal;
in vec3 position;

uniform int light_count = 3;
uniform vec3 light_position[4]; // can have up to 4 light sources
uniform vec3 light_colour[4];

# define PI 3.14159265358979323846

vec3 blinn_phong_brdf( vec3 in_direction, vec3 out_direction, vec3 normal )
{
  float kl = 0.9;
  float kg = 0.1;
  int s = 1;
  vec3 pl = vec3(0.5f);
  vec3 pg = vec3(0.5f);
  vec3 h = normalize(in_direction + out_direction);
  return  kl * pl * (1/PI) + kg  * pg *  (8 + s)/(8*PI) * pow(dot(normal, h), s); // * max(0.0, dot(normalize(in_direction - out_direction), normal));
}

void main () {

  frag_colour = vec3(0);
  for (int l = 0; l < light_count; ++l )
  {
    // YOUR CODE GOES HERE
    // Implement Equation 1 from the lab instructions: (incoming light colour) * (brdf) * (cosine)
    //float cosine = max(0.0, dot(normalize(light_position[l] - position), normal));

    frag_colour +=  light_colour[l] * 3 * blinn_phong_brdf(light_position[l], position, normalize(normal)) 
        *max(0.0, dot(normalize(light_position[l] - position), normalize(normal)));
  }
}
