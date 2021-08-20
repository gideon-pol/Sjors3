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
#include "postprocessing.h"
#include "OBJloader.h"
#include "meshrenderer.h"
#include "windowmanager.h"
#include "input.h"

const int ShadowWidth = 4096;
const int ShadowHeight = 4096;

float baseCamSpeed = 0.0075f;
float camSpeed = 0.075f;

Camera cam = Camera(70, Renderer::resolution.x, Renderer::resolution.y);

long long oldTime;

int main() {
	cam.position = glm::vec3(0, 2, -1);
	long long start = _Query_perf_counter();
	oldTime = start;

	glfwInit();

	if (!WindowManager::CreateWindow("Window", Renderer::resolution)) {
		std::cout << "Couldn't create window\n" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSwapInterval(1);
	gladLoadGL();

	glViewport(0, 0, 1000, 1000);

	Ref<Shader> shader(new Shader("assets/shaders/diffuse.shader"));
	Ref<Shader> quad(new Shader("assets/shaders/quad.shader"));
	Ref<Shader> depthShader(new Shader("assets/shaders/depth.shader"));
	Ref<Shader> pixel(new Shader("assets/shaders/pixel.vert", "assets/shaders/pixel.frag"));
	Ref<Shader> threshold(new Shader("bloomthreshold.shader"));

	Ref<Texture> wknd(new Texture("assets/textures/weeknd.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> housetexture(new Texture("assets/textures/housetexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> housetreetexture(new Texture("assets/textures/housetreetexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> housegroundtexture(new Texture("assets/textures/housegroundtexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> normalMap(new Texture("assets/textures/normalmap2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> normalMapGround(new Texture("assets/textures/normalmapground.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));

	Renderer::Init();
	Renderer::activeCamera = &cam;

	Input::Init();

	std::cout << std::endl << "Starting first object init" << std::endl;
	Object obj = Object();
	obj.position = glm::vec3(0, 0, 0);
	Ref<MeshRenderer> mrenderer = obj.AddComponent<MeshRenderer>();
	Ref<Mesh> mesh = loadObj("assets/models/house.obj");
	mrenderer->SetMesh(mesh);
	Ref<Material> mat(new Material());
	mat->SetShader(shader);
	mat->SetVec3("objectColor", glm::vec3(1));
	mat->SetFloat("specularIntensity", 1);
	mat->SetTexture("diffuseMap", housetexture);
	//mat->SetTexture("normalMap", normalMap);
	Ref<Material> mathousetree(new Material(mat));
	mathousetree->SetTexture("diffuseMap", housetreetexture);
	mathousetree->SetFloat("specularIntensity", 0);
	Ref<Material> mathouseground(new Material(mat));
	mathouseground->SetFloat("specularIntensity", 0.25);
	mathouseground->SetTexture("diffuseMap", housegroundtexture);
	mathouseground->SetTexture("normalMap", normalMapGround);
	mathouseground->SetFloat("normalMapStrength", 0.25f);
	mrenderer->materials[0] = mat;
	mrenderer->materials[1] = mathousetree;
	mrenderer->materials[2] = mat;
	mrenderer->materials[3] = mathouseground;
	std::cout << "Finished first object init" << std::endl;

	std::cout << std::endl << "Starting second object init" << std::endl;
	Ref<Mesh> smallSphere = loadObj("assets/models/smallsphere.obj");
	Object objDir = Object();
	objDir.position = glm::vec3(-2, 2, 0);
	Ref<MeshRenderer> mrenderer2 = objDir.AddComponent<MeshRenderer>();
	mrenderer2->SetMesh(smallSphere);
	mrenderer2->castShadows = false;
	Ref<Material> mat2(new Material());
	mat2->SetShader(shader);
	mat2->SetVec3("objectColor", glm::vec3(1));
	mat2->SetTexture("diffuseMap", wknd);
	mat2->SetVec3("emission", glm::vec3(2.5, 5, 10));
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
	Ref<Mesh> mesh1 = loadObj("assets/models/teapot.obj");
	Object obj1 = Object();
	obj1.position = glm::vec3(0, 5, 0);
	Ref<MeshRenderer> mrenderer1 = obj1.AddComponent<MeshRenderer>();
	Ref<Material> matw(new Material(mat));
	matw->SetVec3("objectColor", glm::vec3(1));
	matw->SetTexture("diffuseMap", housetexture);
	matw->SetTexture("normalMap", normalMap);
	matw->SetFloat("normalMapStrength", 1.0f);
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
		
	Object lightPointObj = Object();
	Ref<Light> lightPoint = lightPointObj.AddComponent<Light>();
	lightPoint->color = glm::vec3(1,1,0);
	lightPoint->type = LightType::Point;
	lightPoint->intensity = 0.0f;
	lightPointObj.position = glm::vec3(2, 0.5, 0);

	Ref<Bloom> bloomLayer = Ref<Bloom>(new Bloom());
	bloomLayer->quality = 4;
	Ref<Pixelate> pixelateLayer = Ref<Pixelate>(new Pixelate());
	pixelateLayer->pixelateIntensity = 4;
	Ref<ColorGrading> colorGradingLayer = Ref<ColorGrading>(new ColorGrading());
	colorGradingLayer->contrast = 1.75f;
	colorGradingLayer->saturation = 1.5f;
	colorGradingLayer->brightness = -0.9f;
	PostProcessing::AddLayer<Pixelate>(pixelateLayer);
	PostProcessing::AddLayer<Bloom>(bloomLayer);
	PostProcessing::AddLayer<ColorGrading>(colorGradingLayer);

	FBO depthBuffer = FBO();
	depthBuffer.GenenerateTexture(ShadowWidth, ShadowWidth, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_CLAMP_TO_BORDER, 1);

	FBO pixelBuffer = FBO();
	pixelBuffer.GenenerateTexture(Renderer::resolution.x, Renderer::resolution.y, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_FLOAT, GL_CLAMP_TO_EDGE, 1);

	RBO RBO1 = RBO(Renderer::resolution.x, Renderer::resolution.y);
	RBO1.BindToFBO();
	RBO1.Unbind();

	pixelBuffer.Unbind();

	while (!WindowManager::Closing()) {
		if (Input::GetMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			glfwSetInputMode(WindowManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		cam.rotation.z += Input::GetMouseDelta().x/20;
		cam.rotation.x += Input::GetMouseDelta().y/20;

		if (Input::GetKey(GLFW_KEY_L)) {
			cam.rotation.z += 0.25f;
		}
		if (Input::GetKey(GLFW_KEY_J)) {
			cam.rotation.z -= 0.25f;
		}
		if (Input::GetKey(GLFW_KEY_K)) {
			cam.rotation.x += 0.25f;
		}
		if (Input::GetKey(GLFW_KEY_I)) {
			cam.rotation.x -= 0.25f;
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

		if (Input::GetKey(GLFW_KEY_I)) {
			pixel = Ref<Shader>(new Shader("assets/shaders/pixel.vert", "assets/shaders/pixel.frag"));
		}
		
		if (Input::GetKey(GLFW_KEY_U)) {
			shader = Ref<Shader>(new Shader("assets/shaders/diffuse.shader"));
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
		long long delta = now - oldTime;
		oldTime = now;

		std::cout << "FPS: " << 1 / ((double)delta/10000000) << std::endl;
		
		objDir.position = glm::vec3(0, sin((double)now / 50000000) * 3, cos((double)now / 50000000) * 3);
		lightDirObj.position = glm::vec3(0, sin((double)now / 50000000) * 3, cos((double)now / 50000000) * 3);
		lightDirObj2.position = -glm::vec3(0, sin((double)now / 50000000) * 3, cos((double)now / 50000000) * 3);
		lightDir->intensity = lightDirObj.position.y / 3;
		lightDir2->intensity = lightDirObj2.position.y/3*0.25;
		//objPoint.position = glm::vec3(cos((double)now / 10000000 + 3.14) * 3, 2, sin((double)now / 10000000 + 3.14) * 3);
		lightPointObj.position = glm::vec3(2, 0.5, cos((double)now / 10000000 + 3.14));
		
		//lightPointObj.position = objPoint.position;
		glViewport(0, 0, ShadowWidth, ShadowHeight);


		depthBuffer.Bind();
		glEnable(GL_DEPTH_TEST);
		Renderer::Clear();	
		//Renderer::DrawSceneShadowMap(lightComp, depthShader);
		depthBuffer.Unbind();

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

		glActiveTexture(GL_TEXTURE5);
		//depthBuffer.BindTexture(0); //ALWAYS BIND TEXTURE AFTER CREATION OF FRAMEBUFFER TEXTURES

		pixelBuffer.Bind();
		glEnable(GL_DEPTH_TEST);
		Renderer::Clear();
		Renderer::DrawScene();
		pixelBuffer.Unbind();

		Ref<Texture> tex(new Texture(pixelBuffer.TexID[0]));
		Ref<Texture> bloomedTex = PostProcessing::Apply(tex);

		quad->Activate();

		glActiveTexture(GL_TEXTURE0);
		bloomedTex->Bind();

		Renderer::Clear();
		glDisable(GL_DEPTH_TEST);
		Renderer::DrawQuad(quad);

		//bloomedTex->Delete();

		WindowManager::Display();
		Input::Poll();
		glfwPollEvents();
	}

	shader->Delete();

	WindowManager::Destroy();
	glfwTerminate();
}