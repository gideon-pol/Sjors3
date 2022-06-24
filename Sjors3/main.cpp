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

Ref<Shader> diffuseShader;
Ref<Shader> quadShader;
Ref<Shader> depthShader;

Ref<Skybox> skybox;

void handleInput();

Timer fpsTimer = Timer();
Timer startTimer = Timer();

int main() {
	cam.position = glm::vec3(0, 2, -1);
	uint64_t start = _Query_perf_counter();

	glfwInit();

	if (!WindowManager::CreateWindow("Sjors3", Renderer::resolution)) {
		std::cout << "Couldn't create window\n" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSwapInterval(1);
	gladLoadGL();

	glViewport(0, 0, 1000, 1000);

	Renderer::Init();
	Renderer::activeCamera = &cam;

	Input::Init();

	diffuseShader = Ref<Shader>(new Shader("assets/shaders/diffuse.shader"));
	quadShader = Ref<Shader>(new Shader("assets/shaders/quad.shader"));
	depthShader = Ref<Shader>(new Shader("assets/shaders/depth.shader"));

	skybox = Ref<Skybox>(new Skybox("assets/textures/skybox/DaylightBox"));
	skybox->exposure = 0.8;

	Ref<Texture> wknd(new Texture("assets/textures/weeknd.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> housetexture(new Texture("assets/textures/housetexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> housetreetexture(new Texture("assets/textures/housetreetexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> housegroundtexture(new Texture("assets/textures/housegroundtexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> normalMap(new Texture("assets/textures/normalmap2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
	Ref<Texture> normalMapGround(new Texture("assets/textures/normalmapground.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));

	Object house = Object();
	house.position = glm::vec3(0, 0, 0);
	Ref<MeshRenderer> mrenderer = house.AddComponent<MeshRenderer>();
	Ref<Mesh> mesh = loadObj("assets/models/house.obj");
	mrenderer->SetMesh(mesh);
	Ref<Material> mat(new Material());
	mat->SetShader(diffuseShader);
	mat->SetVec3("objectColor", glm::vec3(1));
	mat->SetFloat("specularIntensity", 1);
	mat->SetTexture("diffuseMap", housetexture);
	Ref<Material> mathousetree(new Material(mat));
	mathousetree->SetTexture("diffuseMap", housetreetexture);
	mathousetree->SetFloat("specularIntensity", 0);
	Ref<Material> mathouseground(new Material(mat));
	mathouseground->SetFloat("specularIntensity", 0.25);
	mathouseground->SetTexture("diffuseMap", housegroundtexture);
	mathouseground->SetTexture("normalMap", normalMapGround);
	mathouseground->SetFloat("normalMapStrength", 0.25f);
	Ref<Material> mathouseheart(new Material());
	mathouseheart->SetShader(diffuseShader);
	mathouseheart->SetVec3("emission", glm::vec3(10, 2, 2));
	mrenderer->materials[0] = mat;
	mrenderer->materials[1] = mathousetree;
	mrenderer->materials[2] = mat;
	mrenderer->materials[3] = mathouseground;
	mrenderer->materials[4] = mathouseheart;

	Ref<Mesh> smallSphere = loadObj("assets/models/smallsphere.obj");
	Object objDir = Object();
	objDir.position = glm::vec3(-2, 2, 0);
	Ref<MeshRenderer> mrenderer2 = objDir.AddComponent<MeshRenderer>();
	mrenderer2->SetMesh(smallSphere);
	mrenderer2->castShadows = false;
	Ref<Material> mat2(new Material());
	mat2->SetShader(diffuseShader);
	mat2->SetVec3("objectColor", glm::vec3(1));
	mat2->SetTexture("diffuseMap", wknd);
	mat2->SetVec3("emission", glm::vec3(2.5, 5, 10));
	mrenderer2->materials[0] = mat2;

	Ref<Light> lightDir = objDir.AddComponent<Light>();
	lightDir->color = glm::vec3(1);
	lightDir->intensity = 1.0f;
	lightDir->castShadows = true;

	Object objPoint = Object();
	Ref<MeshRenderer> mrendererPoint = objPoint.AddComponent<MeshRenderer>();
	mrendererPoint->SetMesh(smallSphere);
	mrendererPoint->castShadows = false;
	Ref<Material> matPoint(new Material(mat2));
	matPoint->SetVec3("emission", glm::vec3(5, 5, 0));
	matPoint->SetVec3("objectColor", glm::vec3(1));
	mrendererPoint->materials[0] = matPoint;

	Ref<Light> lightPoint = objPoint.AddComponent<Light>();
	lightPoint->color = glm::vec3(1, 1, 0);
	lightPoint->type = LightType::Point;
	lightPoint->intensity = 0.25f;

	Ref<Mesh> mesh1 = loadObj("assets/models/teapot.obj");
	Object obj1 = Object();
	obj1.position = glm::vec3(0, 5, 0);
	Ref<MeshRenderer> mrenderer1 = obj1.AddComponent<MeshRenderer>();
	Ref<Material> matw(new Material(mat));
	matw->SetVec3("objectColor", glm::vec3(1));
	matw->SetTexture("diffuseMap", wknd);
	matw->SetTexture("normalMap", normalMap);
	matw->SetFloat("normalMapStrength", 1.0f);
	mrenderer1->SetMesh(mesh1);
	mrenderer1->materials[0] = matw;

	Ref<Bloom> bloomLayer = Ref<Bloom>(new Bloom());
	bloomLayer->quality = 4;
	bloomLayer->threshold = 1.75;
	Ref<Pixelate> pixelateLayer = Ref<Pixelate>(new Pixelate());
	pixelateLayer->pixelateIntensity = 4;
	Ref<ColorGrading> colorGradingLayer = Ref<ColorGrading>(new ColorGrading());
	colorGradingLayer->contrast = 1.75f;
	colorGradingLayer->saturation = 1.25f;
	colorGradingLayer->brightness = -0.9f;
	PostProcessing::AddLayer<Pixelate>(pixelateLayer);
	PostProcessing::AddLayer<Bloom>(bloomLayer);
	PostProcessing::AddLayer<ColorGrading>(colorGradingLayer);

	FBO depthBuffer = FBO();
	depthBuffer.GenenerateTexture(Renderer::resolution.x, Renderer::resolution.y, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_CLAMP_TO_BORDER, 1);

	FBO pixelBuffer = FBO();
	pixelBuffer.GenenerateTexture(Renderer::resolution.x, Renderer::resolution.y, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, 1);

	RBO RBO1 = RBO(Renderer::resolution.x, Renderer::resolution.y);
	RBO1.BindToFBO();
	RBO1.Unbind();

	pixelBuffer.Unbind();

	fpsTimer.Start();
	startTimer.Start();

	while (!WindowManager::Closing()) {
		pixelateLayer->pixelateIntensity += Input::GetMouseScrollDelta().y / 2;

		double deltaTime = fpsTimer.Measure();
		fpsTimer.Reset();

		std::cout << 1/deltaTime << std::endl;
		
		handleInput();

		double now = startTimer.Measure();
		objDir.position = glm::vec3(0, 5, cos(now / 5) * 3);
		objPoint.position = glm::vec3(2, 0.5, cos(now + 3.14));

		depthBuffer.Bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		Renderer::Clear();
		Renderer::DrawSceneShadowMaps(depthShader);
		depthBuffer.Unbind();
		
		glViewport(0, 0, Renderer::resolution.x, Renderer::resolution.y);
		
		diffuseShader->Activate();
		diffuseShader->SetIntParameter("s_LightCount", (int)Renderer::lights.size());

		for (int i = 0; i < (int)Renderer::lights.size(); i++) {
			diffuseShader->SetIntParameter((std::stringstream()<<"s_Lights["<<i<<"].Type").str(), (int)Renderer::lights[i]->type);
			diffuseShader->SetVec3Parameter((std::stringstream()<<"s_Lights["<<i<<"].Position").str(), Renderer::lights[i]->object->position);
			diffuseShader->SetVec3Parameter((std::stringstream()<<"s_Lights["<<i<<"].Color").str(), Renderer::lights[i]->color);
			diffuseShader->SetFloatParameter((std::stringstream()<<"s_Lights["<<i<<"].Intensity").str(), Renderer::lights[i]->intensity);
			diffuseShader->SetIntParameter((std::stringstream()<<"s_Lights["<<i<<"].CastShadows").str(), (int)Renderer::lights[i]->castShadows);
			if (Renderer::lights[i]->type == LightType::Directional && Renderer::lights[i]->castShadows) {
				glm::mat4 lightSpaceMat = Renderer::lights[i]->GetDirectionalLightMatrix();
				diffuseShader->SetMat4Parameter((std::stringstream() << "s_LightSpaceMat").str(), glm::value_ptr(lightSpaceMat));
			}
		}
		
		diffuseShader->SetFloatParameter("s_AmbientIntensity", 0.2);
		diffuseShader->SetVec3Parameter("s_AmbientColor", glm::vec3(1));
		diffuseShader->SetVec3Parameter("s_ViewPos", cam.position);
		diffuseShader->SetFloatParameter("s_Time", startTimer.Measure());
		diffuseShader->SetFloatParameter("s_DeltaTime", deltaTime);

		glActiveTexture(GL_TEXTURE31);
		depthBuffer.BindTexture(0); //ALWAYS BIND TEXTURE AFTER CREATION OF FRAMEBUFFER TEXTURES

		diffuseShader->SetIntParameter("s_ShadowMap", 31);

		pixelBuffer.Bind();
		glEnable(GL_DEPTH_TEST);
		Renderer::Clear();
		Renderer::DrawSkybox(skybox);
		Renderer::DrawScene();
		pixelBuffer.Unbind();

		Ref<Texture> tex(new Texture(pixelBuffer.TexID[0]));
		Ref<Texture> bloomedTex = PostProcessing::Apply(tex);
		
		quadShader->Activate();

		glActiveTexture(GL_TEXTURE0);
		bloomedTex ->Bind();

		Renderer::Clear();
		glDisable(GL_DEPTH_TEST);
		Renderer::DrawQuad(quadShader);
		glEnable(GL_DEPTH_TEST);

		WindowManager::Display();
		Input::Poll();
		glfwPollEvents();
	}

	WindowManager::Destroy();
	glfwTerminate();
}

void handleInput() {
	if (Input::GetMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		glfwSetInputMode(WindowManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	cam.rotation.z += Input::GetMouseDelta().x / 20;
	cam.rotation.x += Input::GetMouseDelta().y / 20;

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
	if (Input::GetKey(GLFW_KEY_U)) {
		diffuseShader->Delete();
		diffuseShader = Ref<Shader>(new Shader("assets/shaders/diffuseShader.shader"));
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
}