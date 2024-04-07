// to run: g++ index.cpp src/lib/fileloader.cpp -lGL -lglfw -lGLEW -o index && ./index

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <random>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "src/lib/shader.hpp"
#include "src/lib/fileloader.hpp"

GLFWwindow* window;
/** Rotation angle. */
float angle = 0.0f;
/** Rotation increment. */
float angle_inc = 0.5f;

void update(void) {
	angle = ((angle+angle_inc) < 360.0f) ? angle+angle_inc : 360.0-angle+angle_inc;
}


std::vector<float> gerarVetorAleatorio(const std::vector<float>& original) {
    std::vector<float> resultado;
    resultado.reserve(original.size()); // Otimização para alocar espaço de antemão

    // Motor de números aleatórios
    std::random_device rd;  // Obter um número aleatório do hardware
    std::mt19937 gen(rd()); // Alimentar o gerador de números aleatórios
    std::uniform_real_distribution<> dis(0.0, 1.0); // Distribuição entre 0 e 1

    for (size_t i = 0; i < original.size(); i++) {
        // Gerar um novo glm::vec3 com componentes aleatórias entre 0 e 1
        resultado.push_back(dis(gen));
    }

    return resultado;
}

int loadOpenGL() {
  // Initialize GLFW
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

  glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow( 1024, 768, "Trabalho 1", NULL, NULL);
	if( window == NULL ) {
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
  	glfwMakeContextCurrent(window);

  // Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

 	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

  return 1;
}

int main () {

  int loadOpenGLStatus = loadOpenGL();
  if (loadOpenGLStatus != 1) {
    std::cout << "Erro ao inicilizar o OpenGl" << std::endl;
    exit(-1);
  }

	char filename[] = "bunny.obj";
  FileLoader fileloader(filename);
  std::vector<float> vec;
  fileloader.getShapes(vec);
  std::vector<float> colors = gerarVetorAleatorio(vec);


  GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

  GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  // glm::mat4 Projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 0.1f, 100.0f);

	glm::mat4 View       = glm::lookAt(
								glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 MVP = Projection * View * Model;

  GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, vec.size() * sizeof(float), vec.data(), GL_STATIC_DRAW);
	// glBufferData(GL_ARRAY_BUFFER, vec.capacity() * sizeof(float) + sizeof(vec), vec.data(), GL_STATIC_DRAW);

  GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);








  




  do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_TRUE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

    glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_TRUE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

    update();
    glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f,0.0f,1.0f));
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(5, 5, 5)); // Escala uniforme

    glm::mat4 M = Rz *S ;
		M = MVP * M;



    unsigned int loc = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(M));




	  glDrawArrays(GL_TRIANGLES, 0, vec.size());

		glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
	glDeleteProgram(programID);
	glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &colorbuffer);


	// Close OpenGL window and terminate GLFW
	glfwTerminate();



  std::cout << "hello world!" << vec.size() << std::endl;
  return 0;
}