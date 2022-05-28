#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "logger.h"
#include "particle_sim.h"

#define WWIDTH 800
#define WHEIGHT 600

int main()
{
	INFO("glfwInit\n");
	if(glfwInit() != GLFW_TRUE) {
		ERR("Failed to initialize glfw\n");
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	INFO("glfwCreateWindow\n");
	GLFWwindow* window = glfwCreateWindow(WWIDTH, WHEIGHT, "Particle simulation", NULL, NULL);
	if(window == NULL) {
		ERR("Failed to create glfw window\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	INFO("gladLoadGLLoader\n");
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		ERR("Failed to initialize GLAD\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glViewport(0, 0, WWIDTH, WHEIGHT);

	struct particle_sim_t particle_sim;
	particle_sim_init(&particle_sim, 1000);

	INFO("Main loop\n");
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, 1);
		}

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		particle_sim_draw(&particle_sim);

		glfwSwapBuffers(window);
	}

	particle_sim_destroy(&particle_sim);

	INFO("glfwTerminate\n");
	glfwTerminate();
	return EXIT_SUCCESS;
}
