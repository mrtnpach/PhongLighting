// imgui stuff
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// GLFW and glad
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Project includes
#include "LabDebug.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "STBLoading.h"
#include "Camera.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Dimensions
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 800;

// Functions
void FramebufferSizeCallback(GLFWwindow*, int, int);
void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void ToggleEditMode(GLFWwindow* window);
void SetDirectionalLight(bool enabled, Shader* shader, glm::vec3 direction, glm::vec3 color, glm::vec3 ambientColor, float ambientIntensity);
void SetPointLight(bool enabled, Shader* shader, glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic);
void SetSpotLight(bool enabled, Shader* shader, glm::vec3 position, glm::vec3 direction, glm::vec3 color, float innerConeCos, float outerConeCos);

// Camera
Camera camera(glm::vec3(2.15f, 1.0f, 2.15f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -25.0f);
bool movementEnabled = true;
bool firstMouse = true;
float lastX = WINDOW_WIDTH / 2;
float lastY = WINDOW_HEIGHT / 2;
bool spotLight = false;

// Time values
float DeltaTime = 0.0f;
float LastFrame = 0.0f;

int main()
{
	if (glfwInit())
	{
		/*Settings******************************************************************************************/

		ShowLabMessage("GLFW initiated");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			ShowLabMessage("GLFW:ERROR -> Failed to create GLFW window");
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, MouseCallback);
		glfwSetKeyCallback(window, KeyCallback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			ShowLabMessage("GLAD:ERROR -> Failed to initialize GLAD");
			return -1;
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(DebugCallback, 0);

		//  These are later used for the Screen Space transformation (Viewport transform). NDC -> Screen coordinates
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
		glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

		/*Center cube***************************************************************************************/

		float cubeVertexData[] = {
			// Vertex pos.			// Texture coord.	// Normal
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f, 0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,			0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,			0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,			0.0f, 0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,			0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,			0.0f, 0.0f, -1.0f,
														
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,			0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,			0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,			0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,			0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,			0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,			0.0f, 0.0f, 1.0f,
														
			-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,			-1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,			-1.0f, 0.0f, 0.0f,
														
			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,			1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,			1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,			1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,			1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,			1.0f, 0.0f, 0.0f,
														
			-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,			0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,			0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,			0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,			0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,			0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,			0.0f, -1.0f, 0.0f,
														
			-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,			0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,			0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,			0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,			0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,			0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,			0.0f, 1.0f, 0.0f
		};

		// Generate VAO
		unsigned int cubeVAO;
		glGenVertexArrays(1, &cubeVAO);

		// Bind VAO to configure it
		glBindVertexArray(cubeVAO);

		// VBO creation and configuration
		VertexBuffer* cubeVBO = new VertexBuffer(cubeVertexData, sizeof(cubeVertexData));
		
		// Cube attributes
		// Vertex coordinates
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// Texture coordinates (UV)
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// Normals
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		cubeVBO->Unbind();
		
		// Load textures, operation result stored in loadingSuccessful
		bool loadingSuccessful;
		unsigned int containerDiffuse;
		load2DTexture(containerDiffuse, "textures/container.png", true, loadingSuccessful);

		unsigned int containerSpecular;
		load2DTexture(containerSpecular, "textures/container_specular.png", true, loadingSuccessful);

		unsigned int smudgesSpecular;
		load2DTexture(smudgesSpecular, "textures/smudges.jpg", false, loadingSuccessful);

		Shader cubeShader = Shader("shaders/VertShader.txt", "shaders/FragShader.txt");
		cubeShader.Use();
		glActiveTexture(GL_TEXTURE0); // Activates texture unit. Texture units allow to use multiple textures in a shader
		glBindTexture(GL_TEXTURE_2D, containerDiffuse); // Subsequent texture binding binds the texture to the unitglActiveTexture(GL_TEXTURE0); 
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, containerSpecular);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, smudgesSpecular);
		cubeShader.SetInt("fMaterial.diffuse", 0);

		/*Light cube****************************************************************************************/

		// Reusing the cubes vertex data is more efficient than using a separate 
		// VBO for the light cube
		float lightVertexData[] = {
			// Vertex pos.			// Texture coord.
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		unsigned int lightVAO;
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);

		VertexBuffer* lightVBO = new VertexBuffer(lightVertexData, sizeof(lightVertexData));

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		lightVBO->Unbind();

		Shader lightShader = Shader("shaders/CubeLightVert.txt", "shaders/CubeLightFrag.txt");

		/***************************************************************************************************/

		// Rarely changes, so we leave it outside of the rendering loop
		glm::mat4 projectionMatrix;
		projectionMatrix = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		
		cubeShader.Use();
		cubeShader.SetMat4("projectionMatrix", projectionMatrix);
		lightShader.Use();	// Bind shader to set uniforms!
		lightShader.SetMat4("projectionMatrix", projectionMatrix);

		/************************************************************************************************************/

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& imguiIO = ImGui::GetIO();
		(void)imguiIO;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");

		// Material
		bool texturedCube = true;
		float diffuseColor[3] = { 0.05f, 0.05f, 0.05f };
		float specularColor[3] = { 0.50f, 0.50f, 0.50f };
		float specularShininess = 32.0f;
		float specularIntensity = 1.0f;

		// Directional light:
		bool directionalLight = true; // Used to toggle on/off
		glm::vec3 sunColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 sunDirection = glm::vec3(0.2f, -1.0f, -1.0f);
		glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
		float ambientIntensity = 0.4f;

		// Point light:
		bool pointLight = true; // Used to toggle on/off
		bool pointLightSource = true;
		glm::vec3 pointLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		float radius = 2.0f; // Distance from the center cube
		glm::vec3 pointLightPosition = glm::vec3(0.0f);
		float pointLightConstant = 1.0f;
		float pointLightLinear = 0.14f;
		float pointLightQuadratic = 0.07f;

		// Spot light
		glm::vec3 spotLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		float minRadiusCos = glm::cos(glm::radians(10.0));
		float maxRadiusCos = glm::cos(glm::radians(25.0));

		// Iteration of render loop is a frame
		while (!glfwWindowShouldClose(window))
		{
			// Time processing
			float currentFrame = static_cast<float>(glfwGetTime());
			DeltaTime = currentFrame - LastFrame;
			LastFrame = currentFrame;

			// Input processing
			ProcessInput(window);

			// Rendering
			glClearColor(0, 0, 0, 1.0f); // Specifies clearing color, state-setting
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This one clears, state-using
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			glm::mat4 viewMatrix;
			viewMatrix = camera.GetViewMatrix(); // World space up has y = 1
			glm::mat4 modelMatrix = glm::mat4(1.0f); // 4x4 indentity matrix

			// Point light position
			float xPos = cos(glfwGetTime()) * radius;
			float zPos = sin(glfwGetTime()) * radius;
			float yPos = cos(glfwGetTime()) * sin(glfwGetTime());
			pointLightPosition = glm::vec3(xPos, yPos, zPos);

			cubeShader.Use();

			// Textured cube
			cubeShader.SetMat4("viewMatrix", viewMatrix);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f));
			cubeShader.SetMat4("modelMatrix", modelMatrix);
			cubeShader.SetVec3("fViewPosition", &(camera.Position()[0]));

			// Material
			cubeShader.SetBool("material.textured", texturedCube);
			if (texturedCube)
			{
				cubeShader.SetInt("material.diffuse", 0);
				cubeShader.SetInt("material.specular", 1);
			}
			else
			{
				cubeShader.SetVec3("material.color", diffuseColor);
				cubeShader.SetInt("material.specular", 2);
			}
			cubeShader.SetFloat("material.specularIntensity", specularIntensity);
			cubeShader.SetFloat("material.shininess", specularShininess);
			SetDirectionalLight(!spotLight, &cubeShader, sunDirection, sunColor, ambientColor, ambientIntensity);
			SetPointLight(!spotLight, &cubeShader, pointLightPosition, pointLightColor, pointLightConstant, pointLightLinear, pointLightQuadratic);
			SetSpotLight(spotLight, &cubeShader, camera.Position(), camera.Front(), spotLightColor, minRadiusCos, maxRadiusCos);
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Render light cube
			if (pointLightSource && !spotLight)
			{
				lightShader.Use();
				lightShader.SetMat4("projectionMatrix", projectionMatrix);
				lightShader.SetMat4("viewMatrix", viewMatrix);
				modelMatrix = glm::mat4(1.0f);
				modelMatrix = glm::translate(modelMatrix, pointLightPosition);
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.05, 0.05));
				lightShader.SetMat4("modelMatrix", modelMatrix);
				lightShader.SetVec3("fLightColor", &pointLightColor[0]);
				glBindVertexArray(lightVAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			ImGui::Begin("Settings");
			ImGui::TextColored(ImVec4(1.0, 0.6, 0.20, 1.0), "Point light:");
			// Point light:			
			ImGui::Checkbox("Show source", &pointLightSource);
			ImGui::ColorEdit3("Color", &pointLightColor[0]);
			ImGui::Text("(Setting the color to black disables the point light)");
			ImGui::DragFloat("Distance", &radius, 0.05f, 2.0f, 10.0f);
			// Directional light:
			ImGui::Separator();
			ImGui::TextColored(ImVec4(1.0, 0.6, 0.20, 1.0), "Directional light:");
			ImGui::DragFloat3("Direction", &sunDirection[0], 0.01f, -1.0f, 1.0f);
			ImGui::ColorEdit3("Light Color", &sunColor[0]); // Consider usign this color for light calculations
			ImGui::Text("(Setting the color to black disables the directional light)");
			ImGui::ColorEdit3("Ambient color", &ambientColor[0]); // Consider usign this color for light calculations
			ImGui::DragFloat("Ambient strength", &ambientIntensity, 0.01f, 0.0f, 2.0);
			// Material
			ImGui::Separator();
			ImGui::TextColored(ImVec4(1.0, 0.6, 0.20, 1.0), "Cube:");
			ImGui::Checkbox("Textured cube", &texturedCube);
			if (!texturedCube)
			{
				ImGui::ColorEdit3("Cube color", diffuseColor);
			}
			ImGui::DragFloat("Specular intensity", &specularIntensity, 0.01f, 0.1f, 1.0f);
			ImGui::DragFloat("Specular shine", &specularShininess, 1.0f, 1.0f, 255.0f);
			ImGui::End();		

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Swap windows buffers and events
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// Finalize ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// Deallocate resources
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteVertexArrays(1, &lightVAO);

		// Delete shaders
		glDeleteProgram(cubeShader.GetId());
		glDeleteProgram(lightShader.GetId());

		delete cubeVBO;
		delete lightVBO;
		//delete recEBO;

		glfwTerminate();
		ShowLabMessage("GLFW terminated");
	}

	return 1;
}

void SetDirectionalLight(bool enabled, Shader* shader, glm::vec3 direction, glm::vec3 color, glm::vec3 ambientColor, float ambientIntensity)
{
	if (!enabled)
	{
		color = glm::vec3(0.0f);
		ambientIntensity = 0.0f;
	}
	shader->SetVec3("dirLight.direction", &direction[0]);
	shader->SetVec3("dirLight.diffuse", &color[0]);
	shader->SetVec3("dirLight.specular", &color[0]);
	shader->SetVec3("dirLight.ambient", &ambientColor[0]);
	shader->SetFloat("dirLight.ambientIntensity", ambientIntensity);
}

void SetPointLight(bool enabled, Shader* shader, glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic)
{
	if (!enabled) color = glm::vec3(0.0f);

	shader->SetVec3("pointLights[0].position", &position[0]);
	shader->SetVec3("pointLights[0].ambient", &color[0]);
	shader->SetVec3("pointLights[0].diffuse", &color[0]);
	shader->SetVec3("pointLights[0].specular", &color[0]);
	shader->SetFloat("pointLights[0].constant", constant);
	shader->SetFloat("pointLights[0].linear", linear);
	shader->SetFloat("pointLights[0].quadratic", quadratic);
}

void SetSpotLight(bool enabled, Shader* shader, glm::vec3 position, glm::vec3 direction, glm::vec3 color, float innerConeCos, float outerConeCos)
{
	if (!enabled) color = glm::vec3(0.0f);

	shader->SetVec3("spotLight.position", &position[0]);
	shader->SetVec3("spotLight.direction", &direction[0]);
	shader->SetVec3("spotLight.ambient", &color[0]);
	shader->SetVec3("spotLight.diffuse", &color[0]);
	shader->SetVec3("spotLight.specular", &color[0]);
	shader->SetFloat("spotLight.minRadiusCos", innerConeCos);
	shader->SetFloat("spotLight.maxRadiusCos", outerConeCos);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	const float cameraSpeed = 10.0f * DeltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (movementEnabled)
	{
		// Camera movement
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboardInput(Camera::FORWARD, DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboardInput(Camera::BACKWARD, DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboardInput(Camera::LEFT, DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboardInput(Camera::RIGHT, DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			camera.ProcessKeyboardInput(Camera::UP, DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera.ProcessKeyboardInput(Camera::DOWN, DeltaTime);
	}
}

void MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;	// Reversed because y = 0 is at the bottom on view space, not inverted camera
									// Switching the order around would cause the world to rotate up when the mouse moves up
	lastX = xPos;
	lastY = yPos;

	if (movementEnabled)
		camera.ProcessMouseMovement(xoffset, yoffset);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_LEFT_SHIFT:
				ToggleEditMode(window); break;
			case GLFW_KEY_F:
				spotLight = !spotLight; break;
		}
	}
}

void ToggleEditMode(GLFWwindow* window)
{
	static bool cursorCaptured = true;
	// if on free camera mode
	if (cursorCaptured)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		cursorCaptured = false;
		firstMouse = true;
		movementEnabled = false;
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		cursorCaptured = true;
		movementEnabled = true;
	}
}