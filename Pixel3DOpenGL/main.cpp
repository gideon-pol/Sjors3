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
#include "mesh.h"
#include "renderer.h"
#include "OBJloader.h"
#include "meshrenderer.h"

std::vector<Vertex> quadMesh = {
	Vertex {glm::vec3(-1, 0,-1), glm::vec3(0, 1, 0), glm::vec2(0)}, // bottom left
	Vertex {glm::vec3( 1, 0,-1), glm::vec3(0, 1, 0), glm::vec2(1, 0)}, // bottom right
	Vertex {glm::vec3(-1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(0, 1)}, // top left

	Vertex {glm::vec3( 1, 0,-1), glm::vec3(0, 1, 0), glm::vec2(1, 0)},
	Vertex {glm::vec3( 1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(1)}, // top right
	Vertex {glm::vec3(-1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(0, 1)}
};

const int ShadowWidth = 4096;
const int ShadowHeight = 4096;

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

	Shader shader = Shader("diffuse.shader");
	Shader quad = Shader("quad.shader");
	Shader depthShader = Shader("depth.shader");
	Shader pixel = Shader("pixel.vert", "pixel.frag");

	Texture wknd = Texture("weeknd.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture testtex = Texture("testtex.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture normalMap = Texture("weeknd.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	
	Renderer::Init();
	Renderer:: activeCamera = &cam;

	Mesh mesh = loadObj("highdefsphere.obj");
	Object obj = Object();
	obj.position = glm::vec3(0, 0, 0);
	MeshRenderer* mrenderer = obj.AddComponent<MeshRenderer>();
	mrenderer->SetMesh(mesh);
	mrenderer->shader = shader;

	Mesh mesh2 = loadObj("smallsphere.obj");
	Object obj2 = Object();
	obj2.position = glm::vec3(-2, 2, 0);
	MeshRenderer* mrenderer2 = obj2.AddComponent<MeshRenderer>();
	mrenderer2->SetMesh(mesh2);
	mrenderer2->shader = shader;
	mrenderer2->castShadows = false;
	
	Mesh mesh1 = loadObj("teapot.obj");
	Object obj1 = Object();
	obj1.position = glm::vec3(2, 2, 0);
	MeshRenderer* mrenderer1 = obj1.AddComponent<MeshRenderer>();
	mrenderer1->SetMesh(mesh1);
	mrenderer1->shader = shader;

	Mesh mesh3 = loadObj("solidplane.obj");
	Object obj3 = Object();
	obj3.position = glm::vec3(0, -1, 0);
	MeshRenderer* mrenderer3 = obj3.AddComponent<MeshRenderer>();
	mrenderer3->SetMesh(mesh3);
	mrenderer3->shader = shader;

	glm::vec3 lightPos = glm::vec3(0, 0.5, 0);

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

		lightPos = glm::vec3(cos((double)now / 10000000) * 3, 2, sin((double)now / 10000000) * 3);

		obj2.position = lightPos;

		glm::mat4 lightProjection, lightView, lightSpaceMat;

		float near = 0.01f, far = 20.0f;
		lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near, far);
		lightView = glm::lookAt(lightPos, glm::vec3(0), glm::vec3(0, 1, 0));
		//lightView = glm::translate(lightView, cam.position);
		lightSpaceMat = lightProjection * lightView;
		depthShader.Activate();
		depthShader.SetMat4Parameter("lightSpaceMat", glm::value_ptr(lightSpaceMat));
		
		glViewport(0, 0, ShadowWidth, ShadowHeight);
		FBO depthBuffer = FBO();
		depthBuffer.GenenerateTexture(ShadowWidth, ShadowWidth, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_CLAMP_TO_BORDER, 1);

		Renderer::Clear();
		glEnable(GL_DEPTH_TEST);
		Renderer::DrawSceneShadowMap(depthShader);

		depthBuffer.Unbind();
		depthBuffer.Delete();
		
		glViewport(0, 0, Renderer::resolution.x, Renderer::resolution.y);
		
		shader.Activate();

		shader.SetVec3Parameter("lightPos", lightPos);
		shader.SetFloatParameter("lightIntensity", 1);
		shader.SetVec3Parameter("lightColor", glm::vec3(1,1,1));
		shader.SetFloatParameter("ambientIntensity", 0.2);
		shader.SetVec3Parameter("ambientColor", glm::vec3(1, 1, 1));
		shader.SetVec3Parameter("objectColor", glm::vec3(1));
		shader.SetVec3Parameter("viewPos", cam.position);
		shader.SetMat4Parameter("lightSpaceMat", glm::value_ptr(lightSpaceMat));

		shader.SetIntParameter("diffuseMap", 0);
		shader.SetIntParameter("normalMap", 1);
		shader.SetIntParameter("shadowMap", 2);
		
		FBO pixelBuffer = FBO();
		pixelBuffer.GenenerateTexture(Renderer::resolution.x, Renderer::resolution.y, GL_COLOR_ATTACHMENT0, GL_RGBA, GL_UNSIGNED_BYTE, GL_REPEAT, 1);
		
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1000, 1000);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		//pixelBuffer.Unbind();

		glActiveTexture(GL_TEXTURE0);
		wknd.Bind();
		glActiveTexture(GL_TEXTURE1);
		normalMap.Bind();
		glActiveTexture(GL_TEXTURE2);
		depthBuffer.BindTexture(0);
		
		Renderer::Clear();
		Renderer::DrawScene();

		pixelBuffer.Unbind();
		pixelBuffer.Delete();
		GLuint rbos = { rbo };
		glDeleteRenderbuffers(1, &rbos);
		
		glActiveTexture(GL_TEXTURE0);
		pixelBuffer.BindTexture(0);

		Renderer::Clear();
		glDisable(GL_DEPTH_TEST);
		Renderer::DrawQuad(pixel);

		depthBuffer.DeleteTexture(-1);
		pixelBuffer.DeleteTexture(-1);

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