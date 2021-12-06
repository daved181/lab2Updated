
#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr

float g_rotation[2];
int width = 800, height = 600;
glm::mat4 model = glm::mat4(1.0f);
glm::vec4 normals[24];

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
		std::string fragment_shader_str = readFile("../lab2-1_fs.glsl");

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
		model = glm::rotate(glm::mat4(1.0f), glm::radians(5.0f), glm::vec3(0, 1.0f, 0)) * model;
	}
	if ((key == GLFW_KEY_LEFT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		model = glm::rotate(glm::mat4(1.0f), glm::radians(-5.0f), glm::vec3(0, 1.0f, 0)) * model;
	}
	if ((key == GLFW_KEY_UP) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		model = glm::rotate(glm::mat4(1.0f), glm::radians(-5.0f), glm::vec3(1.0f, 0, 0)) * model;
	}
	if ((key == GLFW_KEY_DOWN) && ((action == GLFW_PRESS) || action == GLFW_REPEAT)) {
		model = glm::rotate(glm::mat4(1.0f), glm::radians(5.0f), glm::vec3(1.0f, 0, 0)) * model;
	}
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

glm::vec4 crossVec4(glm::vec4 _v1, glm::vec4 _v2) {
	glm::vec3 vec1 = glm::vec3(_v1[0], _v1[1], _v1[2]);
	glm::vec3 vec2 = glm::vec3(_v2[0], _v2[1], _v2[2]);
	glm::vec3 res = cross(vec1, vec2);
	return glm::vec4(res[0], res[1], res[2], 0);
}

glm::vec4 subtractV4(glm::vec4 _v1, glm::vec4 _v2) {
	glm::vec4 res;
	for (int c = 0; c < 3; c++) {
			res[c] = _v1[c] - _v2[c];
		}
	return res;
}

int main(int argc, char const* argv[])
{
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

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// Set up geometry, VBO, EBO, VAO
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	GLfloat cube_vertices[] = {
		// A cube has 8 vertices, but now we have three copies of each one:
		-0.5, -0.5, -0.5, 1, //0 0
		-0.5, -0.5, -0.5, 1, //0 1
		 -0.5, -0.5, -0.5, 1, //0 2
		 //
		-0.5, -0.5,  0.5, 1, //1 3
		-0.5, -0.5,  0.5, 1, //1 4
		-0.5, -0.5,  0.5, 1, //1 5
		  //
		-0.5,  0.5, -0.5, 1, //2 6
		-0.5,  0.5, -0.5, 1, //2 7
		-0.5,  0.5, -0.5, 1, //2 8
		  //
		-0.5,  0.5,  0.5, 1, //3 9
		-0.5,  0.5,  0.5, 1, //3 10
		-0.5,  0.5,  0.5, 1, //3 11
		  //
		0.5, -0.5, -0.5, 1, //4 12
		0.5, -0.5, -0.5, 1, //4 13
		0.5, -0.5, -0.5, 1, //4 14
			  //
		0.5, -0.5,  0.5, 1, //5 15
		0.5, -0.5,  0.5, 1, //5 16
		0.5, -0.5,  0.5, 1, //5 17
		  //
		0.5,  0.5, -0.5, 1, //6 18
		0.5,  0.5, -0.5, 1, //6 19
		0.5,  0.5, -0.5, 1, //6 20
		  //
		0.5,  0.5,  0.5, 1, //7 21
		0.5,  0.5,  0.5, 1, //7 22
		0.5,  0.5,  0.5, 1, //7 23
	};

	GLushort cube_elements[] = {
		// ... and 12 triangular faces, 
		// defined by the following vertex indices:	
		0, 9, 6, // 0 3 2
		0, 3, 9, // 0 1 3
		//
		1, 7, 18, // 0 2 6
		1, 18, 12, // 0 6 4
		//
		13, 19, 15, // 4 6 5
		15, 19, 21, // 5 6 7
		//
		16, 22, 10, // 5 7 3
		16, 10, 4, // 5 3 1
		//
		8, 11, 23, // 2 3 7
		8, 23, 20, // 2 7 6
		//
		2, 14, 5, // 0 4 1
		5, 14, 17 // 1 4 5
	};

	for (int crosses = 0; crosses < 36; crosses += 3) {
		glm::vec4 _vec1 = glm::vec4(cube_vertices[cube_elements[crosses] * 4], cube_vertices[cube_elements[crosses] * 4 + 1], cube_vertices[cube_elements[crosses] * 4 + 2], 0);
		glm::vec4 _vec2 = glm::vec4(cube_vertices[cube_elements[crosses + 1] * 4], cube_vertices[cube_elements[crosses + 1] * 4 + 1], cube_vertices[cube_elements[crosses + 1] * 4 + 2], 0);
		glm::vec4 _vec3 = glm::vec4(cube_vertices[cube_elements[crosses + 2] * 4], cube_vertices[cube_elements[crosses + 2] * 4 + 1], cube_vertices[cube_elements[crosses + 2] * 4 + 2], 0);

		glm::vec4 cross = crossVec4(_vec2 - _vec1, _vec3 - _vec1);

		normals[cube_elements[crosses]] = cross;
		normals[cube_elements[crosses + 1]] = cross;
		normals[cube_elements[crosses + 2]] = cross;

		//printf("%f %f %f %f \n %f %f %f %f \n ---------\n", cube_vertices[cube_elements[crosses] * 4], cube_vertices[cube_elements[crosses] * 4 + 1], cube_vertices[cube_elements[crosses] * 4 + 2], 0,
			//cube_vertices[cube_elements[crosses + 1] * 4], cube_vertices[cube_elements[crosses + 1] * 4 + 1], cube_vertices[cube_elements[crosses + 1] * 4 + 2], 0);
	}
	for (int c = 0; c < 24; c++) {
		printf("%f %f %f %f\n", normals[c].x, normals[c].y, normals[c].z, normals[c].w);
	}

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	

	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// load and compile shaders  "../lab1-7_vs.glsl" and "../lab1-7_fs.glsl"
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	std::string vertex_shader_str = readFile("../lab2-1_vs.glsl");
	std::string fragment_shader_str = readFile("../lab2-1_fs.glsl");

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

	while (!glfwWindowShouldClose(window))
	{

		glfwGetFramebufferSize(window, &w_width, &w_height);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 100.0f);

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
		int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


