// to run: g++ index.cpp src/lib/fileloader.cpp src/render_model.cpp src/view.cpp -lGL -lglfw -lGLEW -o index && ./index

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
#include "src/lib/datadefinition.hpp"
#include "src/render_model.hpp"
#include "src/view.hpp"


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
		// Fecha a janela se 'Q' for pressionado
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	} else if (key == GLFW_KEY_V && action == GLFW_PRESS ) {
		// Show wireframe if "v" is pressed
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

std::vector<float> gerarVetorAleatorio(const std::vector<float>& original) {
    std::vector<float> resultado;
    resultado.reserve(original.size()); // Otimização para alocar espaço de antemão

    // Motor de números aleatórios
    std::random_device rd;  // Obter um número aleatório do hardware
    std::mt19937 gen(rd()); // Alimentar o gerador de números aleatórios
    std::uniform_real_distribution<> dis(0.0, 1.0); // Distribuição entre 0 e 1

    for (size_t i = 0; i < original.size(); i++) {
        // Gerar um novo glm::vec3 com componentes aleatórias entre 0 e 1
        resultado.push_back(0.5);
        // resultado.push_back(dis(gen));
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
		glfwSetKeyCallback(window, key_callback);

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
	View viewObj;
  int loadOpenGLStatus = loadOpenGL();
  if (loadOpenGLStatus != 1) {
    std::cout << "Erro ao inicilizar o OpenGl" << std::endl;
    exit(-1);
  }

	char filename[] = "bunny.obj";
	RenderModel renderModel(filename);
  // FileLoader fileloader(filename);
  std::vector<float> vec;
	renderModel.getShape(vec);
  // fileloader.getShapes(vec);
  std::vector<float> colors = gerarVetorAleatorio(vec);


  GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

  GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  // glm::mat4 Projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 0.1f, 100.0f);
	ViewStore = viewObj.getView();
	// glm::mat4 View       = glm::lookAt(
	// 							glm::vec3(0,0,3), // Camera is at (4,3,-3), in World Space
	// 							glm::vec3(0,0,0), // and looks at the origin
	// 							glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	// 					   );
  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 MVP = Projection * ViewStore * Model;

  GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, vec.size() * sizeof(float), vec.data(), GL_STATIC_DRAW);
	// glBufferData(GL_ARRAY_BUFFER, vec.capacity() * sizeof(float) + sizeof(vec), vec.data(), GL_STATIC_DRAW);

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
		// Model m(void);
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
    		S = glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor.x, scale_factor.y, scale_factor.z)); // Escala uniforme
			// std::cout <<"(" << scale_factor.x << ", " << scale_factor.y << ", " << scale_factor.z << ")" << std::endl;
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
		// std::cout <<"(" << rotation_angles.x << ", " << rotation_angles.y << ", " << rotation_angles.z << ")" << std::endl;

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



  std::cout << "hello world!" << vec.size() << std::endl;
  return 0;
}