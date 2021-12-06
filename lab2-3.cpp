// This code only shows how to load an object. Input this into your
// program to also draw it.

#include <iostream>
#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include "readfile.hpp"

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

float g_rotation[2];
int width = 800, height = 600;
glm::mat4 model = glm::mat4(1.0f);

void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::string errorLog;
		errorLog.resize(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		std::cout << "shader compilation failed:" << std::endl;
		std::cout << errorLog << std::endl;
		return;
	}
	else
		std::cout << "shader compilation success." << std::endl;

	return;
}


static void error_callback(int error, const char* description)
{
	std::cerr << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//-------------------------------------------------------------------------//
	// COPY YOUR CODE FROM 1.6 HERE
	// Update rotation angle here, for example
	//-------------------------------------------------------------------------//
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
	{
		//implement reloading of the shaders on the fly
		std::string vertex_shader_str = readFile("../lab2-1_vs.glsl");
		std::string fragment_shader_str = readFile("../lab2-3_fs.glsl");

		const char* vertex_shader_src = vertex_shader_str.c_str();
		const char* fragment_shader_src = fragment_shader_str.c_str();

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_shader_src, NULL);
		glCompileShader(vs);
		checkShaderCompileError(vs);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_shader_src, NULL);
		glCompileShader(fs);
		checkShaderCompileError(fs);

		GLuint shader_program = glCreateProgram();
		glAttachShader(shader_program, fs);
		glAttachShader(shader_program, vs);
		glLinkProgram(shader_program);
		glDeleteShader(vs);
		glDeleteShader(fs);
		glUseProgram(shader_program);
	}

	

	if ((key == GLFW_KEY_RIGHT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		g_rotation[0] += 10.0f;
		
	}
	if ((key == GLFW_KEY_LEFT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		g_rotation[0] -= 10.0f;
	
	}
	if ((key == GLFW_KEY_UP) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		g_rotation[1] += 10.0f;
		
	}
	if ((key == GLFW_KEY_DOWN) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		g_rotation[1] -= 10.0f;
	
	}
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main( int argc, char** argv )
{

  std::string inputfile = "../../common/data/bunny.obj";
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string err;
  int flags = 0; // see load_flags_t enum for more information.

  // Load the object
  bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), 0, flags);

  // Check for errors
  if (!err.empty()) { // `err` may contain warning message.
    std::cerr << err << std::endl;
  }

  if (!ret) {
    exit(1);
  }
  else
  {
    std::cout << "Loaded " << inputfile
              << " ("
              << shapes.size() << " shapes"
              << ")"
              << "\n";
    if (shapes.size() > 0)
    {
      std::cout << shapes[0].mesh.positions.size() << " points "
                << "(" << shapes[0].mesh.positions.size()*sizeof(float) << " bytes), "
                << shapes[0].mesh.indices.size() << " indices "
                << "(" << shapes[0].mesh.indices.size()*sizeof(unsigned int) << " bytes), "
                << shapes[0].mesh.normals.size() << " normals.\n"
        ;
    }
  }

  // start GL context and O/S window using the GLFW helper library

  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
	  exit(EXIT_FAILURE);


  int w_height = 600;
  int w_width = 800;

  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
	  exit(EXIT_FAILURE);

  GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Hello Icosahedron", NULL, NULL);
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!window) {
	  glfwTerminate();
	  exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  // Populate your VBO with shapes[0].mesh.positions and shapes[0].mesh.normals
  // ...
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.positions.size() * sizeof(float), &(shapes[0].mesh.positions[0]), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
 
  unsigned int VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.normals.size() * sizeof(float), &(shapes[0].mesh.normals[0]), GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // Populate your EBO with shapes[0].mesh.indices
  // ...
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size() * sizeof(unsigned int), &(shapes[0].mesh.indices[0]), GL_STATIC_DRAW);
 
  std::string vertex_shader_str = readFile("../lab2-1_vs.glsl");
  std::string fragment_shader_str = readFile("../lab2-3_fs.glsl");

  const char* vertex_shader_src = vertex_shader_str.c_str();
  const char* fragment_shader_src = fragment_shader_str.c_str();

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader_src, NULL);
  glCompileShader(vs);
  //checkShaderCompileError(vs);

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader_src, NULL);
  glCompileShader(fs);
  //checkShaderCompileError(fs);

  GLuint shader_program = glCreateProgram();
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  // attach and link vertex and fragment shaders into a shader program
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  glAttachShader(shader_program, fs);
  glAttachShader(shader_program, vs);
  glLinkProgram(shader_program);
  glDeleteShader(vs);
  glDeleteShader(fs);
  glUseProgram(shader_program);

  glUseProgram(shader_program);

  const float n = 1.0f;
  const float f = 100.0f;

  glm::vec3 light_position[3] = {
		{0.0f, 0.0f, -1.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f}
  };
  glm::vec3 light_colour[3] = {
	  {0.0f, 1.0f, 0.0f},
	  {1.0f, 0.0f, 0.0f},
	  {0.0f, 0.0f, 1.0f}
  };

  glUniform3fv(glGetUniformLocation(shader_program, "light_position"), 3, glm::value_ptr(*light_position));
  glUniform3fv(glGetUniformLocation(shader_program, "light_colour"), 3, glm::value_ptr(*light_colour));

  while (!glfwWindowShouldClose(window))
  {

	  glfwGetFramebufferSize(window, &w_width, &w_height);

	  glm::mat4 view = glm::mat4(1.0f);
	  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

	  glm::mat4 projection;
	  projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 100.0f);

	  model = glm::mat4(1.0f);
	  model = glm::rotate(model, glm::radians(g_rotation[0]), glm::vec3(0.0f, 1.0f, 0));
	  model = glm::rotate(model, glm::radians(g_rotation[1]), glm::vec3(1.0f, 0.0f, 0));
	  model = glm::translate(model, glm::vec3(0, -0.5f, 0.0f));
	  model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

	  glm::mat4 mvp = projection * view * model;
	  glUniformMatrix4fv(glGetUniformLocation(shader_program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	  glm::mat4 viewModel = view * model;
	  glUniformMatrix4fv(glGetUniformLocation(shader_program, "viewModel"), 1, GL_FALSE, glm::value_ptr(viewModel));

	  // update other events like input handling 
	  glfwPollEvents();

	  // clear the drawing surface
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  //-----------------------------------------------------------------------//
	  // Issue an appropriate glDraw*() command.
	  //-----------------------------------------------------------------------//
	
	  glDrawElements(GL_TRIANGLES, 208353, GL_UNSIGNED_INT, 0);

	  glfwSwapBuffers(window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  exit(EXIT_SUCCESS);

  return 0;
}



