#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <stb/stb_image.h>
#include <OpenFBX/ofbx.h>
#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "FBO.h"
#include "texture.h"
#include "camera.h"
#include "renderer.h"
#include "OBJloader.h"
#include "meshrenderer.h"

GLfloat vertices[] =
{ //     COORDINATES     /      TexCoord  //
	-1.0f, -1.0f,  0.0f,     0.0f, 0.0f, 0,
	 1.0f, -1.0f,  0.0f,     1.0f, 0.0f, 0,
	 1.0f,  1.0f,  0.0f,     1.0f, 1.0f, 0,
	 -1.0f, 1.0f,  0.0f,     0.0f, 1.0f, 0,

	 -1.0f, -1.0f,  0.0f,     0.0f, 0.0f, 0,
	 1.0f, -1.0f,  0.0f,      1.0f, 0.0f, 0,
	 1.0f,  -1.0f,  2.0f,     1.0f, 1.0f, 0,
	 -1.0f, -1.0f,  2.0f,     0.0f, 1.0f, 0,
};

GLfloat plane[] =
{
	1, 0, 1,	0, 1, 0,	1, 0,
	-1, 0, -1,	 0, 1, 0,	0, 0,
	-1, 0, 1,	0, 1, 0,	0, 0,
	1, 0, 1,	0, 1, 0,	1, 0,
	1, 0, -1,	0, 1, 0,	1, 0,
	-1, 0, -1,	0, 1, 0,	0, 0,

};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	4,5,6,
	4,6,7
};

const int ShadowWidth = 1000;
const int ShadowHeight = 1000;

float baseCamSpeed = 0.1f;
float camSpeed = 0.1f;

Camera cam = Camera(70, Renderer::resolution.x, Renderer::resolution.y);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	baseCamSpeed += yoffset/50000;
	std::cout << "Mouse scroll: " << yoffset << std::endl;
}


int main() {
	cam.position = glm::vec3(0, 0, -1);
	long long start = _Query_perf_counter();

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(Renderer::resolution.x, Renderer::resolution.y, "HIJ ZEI OEH", NULL, NULL);

	if (window == NULL) {
		std::cout << "Couldn't create window\n" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, 1000, 1000);

	//Shader def = Shader("default.vert", "default.frag");
	Shader shader = Shader("diffuse.shader");
	Shader quad = Shader("quad.shader");
	std::cout << "Shader error: " << glGetError() << std::endl;
	Shader depthShader = Shader("depth.shader");
	//Shader pixel = Shader("pixel.vert", "pixel.frag");

	Texture wknd = Texture("weeknd.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	
	Renderer::Init();
	Renderer::activeCamera = &cam;

	Mesh mesh = loadObj("teapot.obj");
	Object obj = Object();
	MeshRenderer* mrenderer = obj.AddComponent<MeshRenderer>();
	mrenderer->SetMesh(mesh);
	mrenderer->shader = shader;

	Mesh mesh2 = loadObj("cone.obj");
	Object obj2 = Object();
	obj2.position = glm::vec3(2, 2, 0);
	MeshRenderer* mrenderer2 = obj2.AddComponent<MeshRenderer>();
	mrenderer2->SetMesh(mesh2);
	mrenderer2->shader = shader;

	glm::vec3 lightPos = glm::vec3(1, 1, 1);

	glEnable(GL_DEPTH_TEST);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_L)) {
			cam.rotation.z += 0.5f;
		}
		if (glfwGetKey(window, GLFW_KEY_J)) {
			cam.rotation.z -= 0.5f;
		}
		if (glfwGetKey(window, GLFW_KEY_K)) {
			cam.rotation.x += 0.5f;
		}
		if (glfwGetKey(window, GLFW_KEY_I)) {
			cam.rotation.x -= 0.5f;
		}

		glm::vec3 camDir = cam.GetForward();

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
			camSpeed = baseCamSpeed * 10;
		}
		else {
			camSpeed = baseCamSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_W)) {
			cam.position += cam.GetForward() * camSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			cam.position -= cam.GetForward() * camSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			cam.position += cam.GetLeft() * camSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			cam.position -= cam.GetLeft() * camSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_R)) {
			cam.position -= cam.GetUp() * camSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_F)) {
			cam.position += cam.GetUp() * camSpeed;
		}
		
		shader.Activate();

		long long now = _Query_perf_counter();

		shader.SetFloatParameter("time", (double)now / 10000000);
		shader.SetFloatParameter("deltaTime", (double)(now - start) / 10000000);

		if (std::fmod((double)now / 10000000, 2) <= 1.0) {
			//mrenderer->enabled = false;
		}
		else {
			mrenderer->enabled = true;
		}

		//obj.position.x = sin((double)now / 10000000);
		//obj.rotation.y = (double)now / 200000;

		shader.SetVec3Parameter("lightPos", glm::vec3(0));
		shader.SetFloatParameter("lightIntensity", 2);
		shader.SetVec3Parameter("lightColor", glm::vec3(0.6, 1, 0));
		shader.SetFloatParameter("ambientIntensity", 0.1);
		shader.SetVec3Parameter("ambientColor", glm::vec3(0, 1, 0));
		shader.SetVec3Parameter("objectColor", glm::vec3(1,1,1));
		shader.SetVec3Parameter("viewPos", cam.position);
		
		glViewport(0, 0, ShadowWidth, ShadowHeight);
		FBO depthBuffer = FBO();
		//depthBuffer.Unbind();
		depthBuffer.GenenerateTexture(ShadowWidth, ShadowHeight, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, 2);
		//depthBuffer.GenenerateTexture(ShadowWidth, ShadowHeight, GL_COLOR_ATTACHMENT0, GL_RGBA, 1);

		Renderer::Clear();
		Renderer::DrawSceneShadowMap(lightPos);
		//cam.CalculateVPMatrix();
		//mrenderer2->Draw();

		depthBuffer.Unbind();
		depthBuffer.Delete();

		glViewport(0, 0, Renderer::resolution.x, Renderer::resolution.y);

		depthBuffer.BindTexture(0);

		//wknd.Bind();
		Renderer::Clear();
		Renderer::DrawQuad(quad);
		//Renderer::DrawScene();

		depthBuffer.DeleteTexture(-1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
}


/*
		FBO frameBuffer = FBO();
		frameBuffer.GenenerateTexture(ScreenWidth, ScreenHeight, GL_COLOR_ATTACHMENT0, 2);
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		FBO pingPong[2];
		for (unsigned int i = 0; i < 2; i++)
		{
			pingPong[i] = FBO();
			pingPong[i].GenenerateTexture(ScreenWidth, ScreenHeight, GL_COLOR_ATTACHMENT0, 1);
		}

		bool horizontal = true, first_iteration = true;
		int amount = 5;
		bloom.Activate();
		for (unsigned int i = 0; i < amount; i++)
		{
			pingPong[horizontal].Bind();
			bloom.SetIntParameter("horizontal", horizontal);
			bloom.SetFloatParameter("lod", 1);
			first_iteration ? frameBuffer.BindTexture(1) : pingPong[!horizontal].BindTexture(0);
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
			glGenerateMipmap(GL_TEXTURE_2D);
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}

		frameBuffer.Unbind();
		frameBuffer.Delete();
		for (unsigned int i = 0; i < 2; i++)
		{
			pingPong[i].Delete();
		}


		bloomblend.Activate();

		glActiveTexture(GL_TEXTURE0);
		frameBuffer.BindTexture(0);

		glActiveTexture(GL_TEXTURE1);
		pingPong[1].BindTexture(0);

		bloomblend.SetIntParameter("scene", 0);
		bloomblend.SetIntParameter("bloomBlur", 1);
		bloomblend.SetFloatParameter("exposure", 1);
		*/

		/*
				frameBuffer.DeleteTexture(-1);

				for (unsigned int i = 0; i < 2; i++)
				{
					pingPong[i].DeleteTexture(-1);
				}
				*/