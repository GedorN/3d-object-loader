#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "src/lib/shader.hpp"
#include "src/lib/fileloader.hpp"
#include "src/lib/datadefinition.hpp"
#include "src/render_model.hpp"
#include "src/view.hpp"
#include "src/utils.hpp"


VisualizationMode currentVisualizationMode = FILLED;
ManipulationMode currentManipulationMode = TRANSLATING;
EntityManipuled currentEntityToBeManipuled = MESH;

GLFWwindow* window;

glm::mat4 T;
glm::mat4 Rz;
glm::mat4 Rx;
glm::mat4 Ry;
glm::mat4 M;
glm::mat4 S;
glm::mat4 ViewStore;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_Q && action == GLFW_PRESS ) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	} else if (key == GLFW_KEY_V && action == GLFW_PRESS ) {
		if (currentVisualizationMode == FILLED) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			currentVisualizationMode = WIREFRAME;
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			currentVisualizationMode = FILLED;
		}
	} else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		currentManipulationMode = TRANSLATING;
	} else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		currentManipulationMode = ROTATING;
	} else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		currentManipulationMode = SCALING;
	} else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		currentEntityToBeManipuled = MESH;
		currentManipulationMode = TRANSLATING;
	} else if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		currentEntityToBeManipuled = OBSERVER;
		currentManipulationMode = TRANSLATING;
	}
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
		glfwSetKeyCallback(window, key_callback);

  // Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glClearColor(0.f, 0.f, 0.f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  return 1;
}

int main (int argc, char* argv[]) {

	if (argc <= 1) {
		std::cout << "Parâmetro obj não recebido. Digite -h ou --help para mais informações " << std::endl;
		exit(-1);
	}
	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		std::cout << "Uso: " << argv[0] << std::endl;
		std::cout << "Modos de visualização:" << std::endl;
		std::cout << "  v: Alterna a vizualização entre face (polígonos preenchidos) e wireframe"<< std::endl;
		std::cout << "Modos de manipualção: " << std::endl;
		std::cout << "  m: o programa entra em modo de manipulação de malha" << std::endl;
		std::cout << "  o: o programa entra em modo de manipulação do obersavador" << std::endl;
		std::cout << "Manipulação de malha: " << std::endl;
		std::cout << "  t: o programa entra em modo de translação do objeto" << std::endl;
		std::cout << "    -seta para cima: deslocamento positivo em y" << std::endl;
		std::cout << "    -seta para baixo: deslocamento negativo em y" << std::endl;
		std::cout << "    -seta para a direita: deslocamento positivo em x" << std::endl;
		std::cout << "    -seta para a esquerda: deslocamento negativo em x" << std::endl;
		std::cout << "    -a: deslocamento positivo em z" << std::endl;
		std::cout << "    -d: deslocamento negativo em z" << std::endl;
		std::cout << "  r: o programa entra em modo de rotação do objeto" << std::endl;
		std::cout << "    -seta para cima: rotação positivo em y" << std::endl;
		std::cout << "    -seta para baixo: rotação negativo em y" << std::endl;
		std::cout << "    -seta para a direita: rotação positivo em x" << std::endl;
		std::cout << "    -seta para a esquerda: rotação negativo em x" << std::endl;
		std::cout << "    -a: rotação positivo em z" << std::endl;
		std::cout << "    -d: rotação negativo em z" << std::endl;
		std::cout << "  e: o programa entra em modo de rotação do objeto" << std::endl;
		std::cout << "    -seta para cima: fator de escala maior que 1 em y" << std::endl;
		std::cout << "    -sseta para baixo: fator de escala menor que 1 e maior que 0 em y" << std::endl;
		std::cout << "    -seta para a direita: fator de escala maior que 1 em x" << std::endl;
		std::cout << "    -seta para a esquerda: fator de escala menor que 1 e maior que 0 em x" << std::endl;
		std::cout << "    -a: fator de escala maior que 1 em z" << std::endl;
		std::cout << "    -d: fator de escala menor que 1 e maior que 0 em z" << std::endl;
		std::cout << "Manipulação de observador: " << std::endl;
		std::cout << "  t: o programa entra em modo de translação do observador" << std::endl;
		std::cout << "    -seta para cima: deslocamento positivo em y" << std::endl;
		std::cout << "    -seta para baixo: deslocamento negativo em y" << std::endl;
		std::cout << "    -seta para a direita: deslocamento positivo em x" << std::endl;
		std::cout << "    -seta para a esquerda: deslocamento negativo em x" << std::endl;
		std::cout << "    -a: deslocamento positivo em z" << std::endl;
		std::cout << "    -d: deslocamento negativo em z" << std::endl;
		std::cout << "  r: o programa entra em modo de rotação do observador" << std::endl;
		std::cout << "    -seta para cima: rotação positiva em y" << std::endl;
		std::cout << "    -seta para baixo: rotação negativa em y" << std::endl;
		std::cout << "    -seta para a direita: rotação positiva em x" << std::endl;
		std::cout << "    -seta para a esquerda: rotação negativa em x" << std::endl;
		std::cout << "    -a: rotação positiva em z" << std::endl;
		std::cout << "    -d: rotação negativa em z" << std::endl;

		exit(0);
	}

  int loadOpenGLStatus = loadOpenGL();
  if (loadOpenGLStatus != 1) {
    std::cout << "Erro ao inicilizar o OpenGl" << std::endl;
    exit(-1);
  }

	RenderModel renderModel(argv[1]);
  std::vector<float> vec;
	renderModel.getShape(vec);

	std::cout << "Número de triângulos carregados: " << vec.size() / 9 << std::endl;
  std::vector<float> colors = color_generator(vec);

	float objHeight = get_object_height(vec);
	float objectWidth = get_object_width(vec);

	std::cout << "Proporções: " << objHeight <<  " X " << objectWidth << std::endl;

	View viewObj(objHeight, objectWidth);

  GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

  GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	ViewStore = viewObj.getView();
  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 MVP = Projection * ViewStore * Model;

  GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, vec.size() * sizeof(float), vec.data(), GL_STATIC_DRAW);

  GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

	PlanCoords3d initial_tranlation_coords = renderModel.getTranlationCoords();
	PlanCoords3d initial_rotation_angles = renderModel.getRotationCoords();
	PlanCoords3d scale_factor = renderModel.getScaleCoords();
	T = glm::translate(glm::mat4(1.0f), glm::vec3(initial_tranlation_coords.x, initial_tranlation_coords.y, initial_tranlation_coords.z));
	Rz = glm::rotate(glm::mat4(1.0f), glm::radians(initial_rotation_angles.z), glm::vec3(0.0f,0.0f,1.0f));
	Rx = glm::rotate(glm::mat4(1.0f), glm::radians(initial_rotation_angles.x), glm::vec3(1.0f,0.0f,0.0f));
	Ry = glm::rotate(glm::mat4(1.0f), glm::radians(initial_rotation_angles.y), glm::vec3(0.0f,1.0f,0.0f));
	S = glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor.x, scale_factor.y, scale_factor.z));
  do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_TRUE,
			0,
			(void*)0
		);

    glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,
			4,
			GL_FLOAT,
			GL_TRUE,
			0,
			(void*)0
		);

		if (currentEntityToBeManipuled == MESH) {
			if (currentManipulationMode == TRANSLATING) {
				renderModel.handleTranslationKeyboardInput(window);
				PlanCoords3d tranlation_coords = renderModel.getTranlationCoords();
				T = glm::translate(glm::mat4(1.0f), glm::vec3(tranlation_coords.x, tranlation_coords.y, tranlation_coords.z));
			} else if (currentManipulationMode == ROTATING) {
				renderModel.handleRotationKeyboardInput(window);
				PlanCoords3d rotation_angles = renderModel.getRotationCoords();
				Rz = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angles.z), glm::vec3(0.0f,0.0f,1.0f));
				Rx = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angles.x), glm::vec3(1.0f,0.0f,0.0f));
				Ry = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angles.y), glm::vec3(0.0f,1.0f,0.0f));
			} else if (currentManipulationMode == SCALING) {
				renderModel.handleScaleKeyboardInput(window);
				PlanCoords3d scale_factor = renderModel.getScaleCoords();
    		S = glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor.x, scale_factor.y, scale_factor.z));
			}
		} else if (currentEntityToBeManipuled == OBSERVER) {
			if (currentManipulationMode == TRANSLATING) {
				viewObj.handleTranslationKeyboardInput(window);
				ViewStore = viewObj.getView();
			} if (currentManipulationMode == ROTATING) {
				viewObj.handleRotationKeyboardInput(window);
				ViewStore = viewObj.getView();

			}
		}

    glm::mat4 M = S * T * Rz * Rx * Ry;
		M = Projection * ViewStore * M;

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

  return 0;
}