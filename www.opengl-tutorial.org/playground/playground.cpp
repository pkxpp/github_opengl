#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include <common/shader.hpp>

int main( void )
{
	// my first window
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);		//4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//open a window and create its OpenGL context
	GLFWwindow *window;
	window = glfwCreateWindow( 1024, 768, "Tutorial 02", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they ar not 3.3 compatible. Try 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	//Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

	//
	glm::mat4 myMatrix;
	glm::vec4 myVector;
	glm::vec4 transformedVector = myMatrix * myVector;

	//An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f, 
		1.0f, -1.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 
	};

	//This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give oour vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// Projection matrix:45бу filed of view; 4:3 ratio; display range: 0.1 -100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 6.0f, 100.0f);
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
			glm::vec3(4, 3, 3),		// Camera is at (4, 3, 3) of World Space
			glm::vec3(0, 0, 0),		// and looks at the origin
			glm::vec3(0, -1, 0)		// Head is up(set to 0, -1, 0 to look upside-down)
		);
	// Model matrix
	glm::mat4 Model = glm::mat4(1.0f);
	//Model = glm::translate(Model, glm::vec3(0, 0, 1));
	// Our ModelViewProjection: multiplication of our 3 matrix
	glm::mat4 MVP = Projection * View * Model;
	//Get a handle for our "MVP" uniform
	// Only at initialation time
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	do{
		//Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		//use our shader
		glUseProgram(programID);

		// Send Our transformation to the currently bound shader
		// in the "MVP" uniform
		// for each model you render, since the MVP will be diffrent(at the least MVP matrix)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		//Draw nothing, see you in tutorial 2!
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,					// attribute 0.
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0			// array bufffer offset
		);

		//Draw the triangle!
		glDrawArrays(GL_TRIANGLES, 0, 3);	//Starting from vertex 0; 
		glDisableVertexAttribArray(0);

		//Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

