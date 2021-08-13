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
#include "Core.h"
#include "rendering.h"
#include "OBJloader.h"
#include "meshrenderer.h"
#include "windowmanager.h"
#include "input.h"

const int ShadowWidth = 4096;
const int ShadowHeight = 4096;

float baseCamSpeed = 0.1f;
float camSpeed = 0.1f;

Camera cam = Camera(70, Renderer::resolution.x, Renderer::resolution.y);

int main() {
	cam.position = glm::vec3(0, 2, -1);
	long long start = _Query_perf_counter();

	glfwInit();

	if (!WindowManager::CreateWindow("Window", Renderer::resolution)) {
		std::cout << "Couldn't create window\n" << std::endl;
		glfwTerminate();
		return -1;
	}

	gladLoadGL();

	glViewport(0, 0, 1000, 1000);

	Ref<Shader> shader(new Shader("diffuse.shader"));
	Ref<Shader> quad(new Shader("quad.shader"));
	Ref<Shader> depthShader(new Shader("depth.shader"));
	Ref<Shader> pixel(new Shader("pixel.vert", "pixel.frag"));

	Ref<Texture> wknd(new Texture("weeknd.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Texture testtex = Texture("testtex.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Ref<Texture> housetexture(new Texture("housetexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> housetreetexture(new Texture("housetreetexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> housegroundtexture(new Texture("housegroundtexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));

	Renderer::Init();
	Renderer::activeCamera = &cam;

	Input::Init();

	std::cout << std::endl << "Starting first object init" << std::endl;
	Object obj = Object();
	obj.position = glm::vec3(0, 0, 0);
	Ref<MeshRenderer> mrenderer = obj.AddComponent<MeshRenderer>();
	Ref<Mesh> mesh = loadObj("house.obj");
	mrenderer->SetMesh(mesh);
	Ref<Material> mat(new Material());
	mat->SetShader(shader);
	mat->SetVec3("objectColor", glm::vec3(1));
	mat->SetFloat("specularIntensity", 1);
	mat->SetTexture("diffuseMap", housetexture);
	Ref<Material> mathousetree(new Material(mat));
	mathousetree->SetTexture("diffuseMap", housetreetexture);
	mathousetree->SetFloat("specularIntensity", 0);
	Ref<Material> mathouseground(new Material(mat));
	mathouseground->SetFloat("specularIntensity", 0.25);
	mathouseground->SetTexture("diffuseMap", housegroundtexture);
	mrenderer->materials[0] = mat;
	mrenderer->materials[1] = mathousetree;
	mrenderer->materials[2] = mat;
	mrenderer->materials[3] = mathouseground;
	std::cout << "Finished first object init" << std::endl;

	std::cout << std::endl << "Starting second object init" << std::endl;
	Ref<Mesh> smallSphere = loadObj("smallsphere.obj");
	Object objDir = Object();
	objDir.position = glm::vec3(-2, 2, 0);
	Ref<MeshRenderer> mrenderer2 = objDir.AddComponent<MeshRenderer>();
	mrenderer2->SetMesh(smallSphere);
	mrenderer2->castShadows = false;
	Ref<Material> mat2(new Material());
	mat2->SetShader(shader);
	mat2->SetVec3("objectColor", glm::vec3(1));
	mat2->SetTexture("diffuseMap", wknd);
	mrenderer2->materials[0] = mat2;
	std::cout << "Finished second object init" << std::endl;

	std::cout << std::endl << "Starting third object init" << std::endl;
	Object objPoint = Object();
	Ref<MeshRenderer> mrendererPoint = objPoint.AddComponent<MeshRenderer>();
	mrendererPoint->SetMesh(smallSphere);
	mrendererPoint->castShadows = false;
	Ref<Material> matPoint(new Material(mat2));
	matPoint->SetVec3("objectColor", glm::vec3(1));
	mrendererPoint->materials[0] = matPoint;
	std::cout << "Finished third object init" << std::endl;
	
	std::cout << std::endl << "Starting fourth object init" << std::endl;
	Ref<Mesh> mesh1 = loadObj("teapot.obj");
	Object obj1 = Object();
	obj1.position = glm::vec3(0, 5, 0);
	Ref<MeshRenderer> mrenderer1 = obj1.AddComponent<MeshRenderer>();
	Ref<Material> matw(new Material(mat));
	matw->SetVec3("objectColor", glm::vec3(1));
	mrenderer1->SetMesh(mesh1);
	mrenderer1->materials[0] = matw;
	std::cout << "Finished fourth object init" << std::endl;

	Object lightDirObj = Object();
	Ref<Light> lightDir = lightDirObj.AddComponent<Light>();
	lightDir->color = glm::vec3(1);
	lightDir->intensity = 1.0f;
	lightDirObj.position = glm::vec3(1, 1, 0);

	Object lightDirObj2 = Object();
	Ref<Light> lightDir2 = lightDirObj2.AddComponent<Light>();
	lightDir2->color = glm::vec3(0.5,0.5,1);
	lightDir2->intensity = 0.25f;
	lightDirObj2.position = glm::vec3(-1, 1, 0);

	Object light2 = Object();
	Ref<Light> lightComp2 = light2.AddComponent<Light>();
	lightComp2->color = glm::vec3(1,0,0);
	lightComp2->type = LightType::Point;
	lightComp2->intensity = 0.0f;

	while (!WindowManager::Closing()) {
		if (Input::GetMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			glfwSetInputMode(WindowManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		std::cout << Input::GetMouseDelta().x << std::endl;

		cam.rotation.z += Input::GetMouseDelta().x/10;
		cam.rotation.x += Input::GetMouseDelta().y/10;

		if (Input::GetKey(GLFW_KEY_L)) {
			cam.rotation.z += 0.75f;
		}
		if (Input::GetKey(GLFW_KEY_J)) {
			cam.rotation.z -= 0.75f;
		}
		if (Input::GetKey(GLFW_KEY_K)) {
			cam.rotation.x += 0.75f;
		}
		if (Input::GetKey(GLFW_KEY_I)) {
			cam.rotation.x -= 0.75f;
		}

		if (Input::GetKey(GLFW_KEY_LEFT_SHIFT)) {
			camSpeed = baseCamSpeed * 10;
		}
		else {
			camSpeed = baseCamSpeed;
		}

		if (Input::GetKey(GLFW_KEY_W)) {
			cam.position += cam.GetForward() * camSpeed;
		}
		if (Input::GetKey(GLFW_KEY_S)) {
			cam.position -= cam.GetForward() * camSpeed;
		}
		if (Input::GetKey(GLFW_KEY_A)) {
			cam.position += cam.GetLeft() * camSpeed;
		}
		if (Input::GetKey(GLFW_KEY_D)) {
			cam.position -= cam.GetLeft() * camSpeed;
		}
		if (Input::GetKey(GLFW_KEY_R)) {
			cam.position -= cam.GetUp() * camSpeed;
		}
		if (Input::GetKey(GLFW_KEY_F)) {
			cam.position += cam.GetUp() * camSpeed;
		}

		if (Input::GetKey(GLFW_KEY_U)) {
			shader = Ref<Shader>(new Shader("diffuse.shader"));
			mathouseground->SetShader(shader);
			mathouseground->SetVec3("objectColor", glm::vec3(1));
			mathouseground->SetFloat("specularIntensity", 0.25);
			mathouseground->SetTexture("diffuseMap", housegroundtexture);

			mat->SetShader(shader);
			mat->SetVec3("objectColor", glm::vec3(1));
			mat->SetFloat("specularIntensity", 1);
			mat->SetTexture("diffuseMap", housetexture);
		}
		if (Input::GetKey(GLFW_KEY_O)) {
			lightDir->color = glm::vec3(1, 0, 0);
			lightDir2->color = glm::vec3(1, 0, 0);
		}
		if (Input::GetKey(GLFW_KEY_P)) {
			lightDir->color = glm::vec3(1);
			lightDir2->color = glm::vec3(0.5, 0.5, 1);
		}

		long long now = _Query_perf_counter();

		objDir.position = glm::vec3(0, sin((double)now / 50000000) * 3, cos((double)now / 50000000) * 3);
		lightDirObj.position = objDir.position;
		lightDirObj2.position = -objDir.position;
		lightDir->intensity = lightDirObj.position.y / 3;
		lightDir2->intensity = lightDirObj2.position.y/3*0.25;
		objPoint.position = glm::vec3(cos((double)now / 10000000 + 3.14) * 3, 2, sin((double)now / 10000000 + 3.14) * 3);
		light2.position = objPoint.position;
		
		glViewport(0, 0, ShadowWidth, ShadowHeight);
		FBO depthBuffer = FBO();
		depthBuffer.GenenerateTexture(ShadowWidth, ShadowWidth, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_CLAMP_TO_BORDER, 1);

		Renderer::Clear();
		glEnable(GL_DEPTH_TEST);
		//Renderer::DrawSceneShadowMap(lightComp, depthShader);

		depthBuffer.Unbind();
		depthBuffer.Delete();

		glActiveTexture(GL_TEXTURE0);
		depthBuffer.BindTexture(0);

		glViewport(0, 0, Renderer::resolution.x, Renderer::resolution.y);

		shader->Activate();

		shader->SetIntParameter("s_LightCount", (int)Renderer::lights.size());

		for (int i = 0; i < (int)Renderer::lights.size(); i++) {
			shader->SetIntParameter((std::stringstream() << "s_Lights[" << i << "].Type").str(), (int)Renderer::lights[i]->type);
			shader->SetVec3Parameter((std::stringstream()<<"s_Lights["<<i<<"].Position").str(), Renderer::lights[i]->object->position);
			shader->SetVec3Parameter((std::stringstream()<<"s_Lights["<<i<<"].Color").str(), Renderer::lights[i]->color);
			shader->SetFloatParameter((std::stringstream()<<"s_Lights["<<i<<"].Intensity").str(), Renderer::lights[i]->intensity);
		}
		
		shader->SetFloatParameter("s_AmbientIntensity", 0.2);
		shader->SetVec3Parameter("s_AmbientColor", glm::vec3(1));
		shader->SetVec3Parameter("s_ViewPos", cam.position);
		shader->SetFloatParameter("s_Time", (double)now / 10000000);
		shader->SetFloatParameter("s_DeltaTime", (double)(now - start) / 10000000);
		
		FBO pixelBuffer = FBO();
		pixelBuffer.GenenerateTexture2(Renderer::resolution.x, Renderer::resolution.y, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_FLOAT, GL_REPEAT, 1);
		
		RBO RBO1 = RBO(Renderer::resolution.x, Renderer::resolution.y);
		RBO1.BindToFBO();
		RBO1.Unbind();

		glActiveTexture(GL_TEXTURE5);
		depthBuffer.BindTexture(0);
		
		Renderer::Clear();
		Renderer::DrawScene();

		pixelBuffer.Unbind();
		pixelBuffer.Delete();
		RBO1.Delete();
		
		glActiveTexture(GL_TEXTURE0);
		pixelBuffer.BindTexture(0);
		
		pixel->Activate();
		double exposure = cos((double)(now - start) / 10000000 / 5) * 0.3 + 0.4;
		pixel->SetFloatParameter("exposure", 0.05);

		Renderer::Clear();
		glDisable(GL_DEPTH_TEST);
		Renderer::DrawQuad(pixel);

		depthBuffer.DeleteTexture(-1);
		pixelBuffer.DeleteTexture(-1);

		WindowManager::Display();
		Input::Poll();
		glfwPollEvents();
	}

	shader->Delete();

	WindowManager::Destroy();
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