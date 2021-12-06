#version 440

out vec3 frag_colour;
in vec3 normal;
in vec3 position;

uniform int light_count = 3;
uniform vec3 light_position[4]; // can have up to 4 light sources
uniform vec3 light_colour[4];

# define PI 3.14159265358979323846

vec3 lambertian_brdf( vec3 in_direction, vec3 out_direction, vec3 normal )
{
  // YOUR CODE GOES HERE
  // Implement a Lambertian BRDF 
  // (1/630, 1/532, 1/465) * 1/PI     this would be the more physically correct way of scaling the colors.
  return  vec3(0.5f);
}

void main () {

  frag_colour = vec3(0);
  for (int l = 0; l < light_count; ++l )
  {
    // YOUR CODE GOES HERE
    // Implement Equation 1 from the lab instructions: (incoming light colour) * (brdf) * (cosine)
    //float cosine = max(0.0, dot(normalize(light_position[l] - position), normal));

    frag_colour += light_colour[l] * 5 *  lambertian_brdf(light_position[l], position, normal) 
        *max(0.0, dot(normalize(light_position[l] - position), normal));
  }
}
