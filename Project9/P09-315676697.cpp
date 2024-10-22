/*
Pr�ctica 8: Iluminaci�n 2
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;

Texture AmTexture;
Texture AzTexture;
Texture RoTexture;
Texture VeTexture;


Texture dado10CarasTexture;


Model Kitt_M;
Model Llanta_M;
Model BlackhawkBase;
Model BlackhawkHelices;




Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado10Caras()
{
	unsigned int decagonoIndices[] = {
		//cara1
		0, 1, 2,
		2, 3, 0,

		//cara2
		4, 5, 6,
		6, 7, 4,

		//cara3
		8, 9, 10,
		10, 11, 8,

		//cara4
		12, 13, 14,
		14, 15, 12,

		//cara5
		16, 17, 18,
		18, 19, 16,

		//cara6
		20, 21, 22,
		22, 23, 20,

		//cara7
		24, 25, 26,
		26, 27, 24,

		//cara8
		28, 29, 30,
		30, 31, 28,

		//cara9
		32, 33, 34,
		34, 35, 32,

		//cara10
		36, 37, 38,
		38, 39, 36
	};

	GLfloat decagonoVertices[] = {
		//x		y		z			S		T			NX		NY		NZ
		//Cara 10
		-0.36f,	0.24f,	-0.6f,		0.25f,	0.36f,		0.0f,	1.0f,	-0.3f,
		-0.57f,	-0.5f,	0.42f,		0.47f,	0.53f,		0.0f,	1.0f,	-0.3f,
		-1.0f,	-0.16f,	0.68f,		0.52f,	0.36f,		0.0f,	1.0f,	-0.3f,
		-1.04f,	0.24f,	0.42f,		0.46f,	0.16f,		0.0f,	1.0f,	-0.3f,

		//Cara 8
		-0.36f,	0.24f,  -0.6f,		0.25f,  0.36f,		0.0f,	1.0f,	-0.3f,
		-1.04f,	0.24f,  0.42f,		0.29f,	0.80f,		0.0f,	1.0f,	-0.3f,
		-1.0f,	0.64f,  0.68f,		0.42f,  0.73f,		0.0f,	1.0f,	-0.3f,
		-0.57f,	0.89f,  0.42f,		0.47f,	0.55f,		0.0f,	1.0f,	-0.3f,

		// Cara 2
		-0.36f,	0.24f,  -0.6f,		0.25f, 0.36f,		0.0f,	1.0f,	-0.5f,
		-0.57f,	0.89f,  0.42f,		0.29f,	0.671f,		0.0f,	1.0f,	-0.5f,
		-0.14f,	0.89f,  0.68f,		0.18f,	0.82f,		0.0f,	1.0f,	-0.5f,
		0.20f,  0.64f,  0.42f,		0.29f,	0.78f,		0.0f,	1.0f,	-0.5f,

		//Crara 6
		-0.36f,	0.24f,  -0.51f,		0.25f, 0.36f,		0.0f,	1.0f,	-0.5f,
		0.20f,  0.64f,  0.42f,		0.007f,	0.32f,		0.0f,	1.0f,	-0.5f,
		0.33f,	0.24f,  0.68f,		0.0f,  0.53f,		0.0f,	1.0f,	-0.5f,
		0.20f,	-0.16f,  0.42f,		0.09f,	0.67f,		0.0f,	1.0f,	-0.5f,

		//Cara 4
		-0.36f,	0.24f,  -0.51f,		0.25f, 0.36f,		0.0f,	1.0f,	0.5f,
		0.2f,	-0.16f,  0.42f,		0.007f,	0.32f,		0.0f,	1.0f,	0.5f,
		-0.14f,	-0.41f,  0.68f,		0.002f,	0.113f,		0.0f,	1.0f,	0.5f,
		-0.57f,  -0.41f,  0.42f,    0.12f,	0.0f,		0.0f,	1.0f,	0.5f,

		//Cara 5
		-0.36f,  0.24f,  1.60f,		0.74f,	0.53f,		0.0f,	1.0f,	-0.5f,
		-0.91f,	-0.16f,  0.68f,		0.52f,	0.73f,		0.0f,	1.0f,	-0.5f,
		-1.04f,	0.24f,  0.43f,		0.47f,	0.54f,		0.0f,	1.0f,	-0.5f,
		-0.91f,	0.64f,  0.68f,		0.52f,	0.35f,		0.0f,	1.0f,	-0.5f,


		//Cara 7
		-0.36f,  0.24f,  1.6f,		0.74f,	0.53f,		0.0f,	1.0f,	-0.5f,
		-0.91f,	0.64f,  0.68f,		0.52f, 0.73f,		0.0f,	1.0f,	-0.5f,
		-0.57f,	0.89f,  0.42f,		0.58f,	0.92f,		0.0f,	1.0f,	-0.5f,
		-0.14f,	0.89f,  0.68f,		0.70f,	0.96f,		0.0f,	1.0f,	-0.5f,


		//Cara 1
		-0.36f,  0.24f,  1.6f,		0.74f,	0.53f,		0.0f,	1.0f,	-0.5f,
		-0.14f,	0.89f,  0.68f,		0.70f,	0.96f,		0.0f,	1.0f,	-0.5f,
		0.2f,  0.64f,  0.42f,		0.82f,	0.99f,		0.0f,	1.0f,	-0.5f,
		0.33f,	0.24f,  0.68f,		0.91f, 0.85f,		0.0f,	1.0f,	-0.5f,


		//Cara 3
		-0.36f,  0.24f,  1.6f,		0.74f,	0.53f,  	0.0f,	1.0f,	-0.5f,
		0.33f,	0.24f,  0.68f,		0.91f, 0.85f,		0.0f,	1.0f,	-0.5f,
		0.2f,	-0.16f,  0.42f,		0.99f,	0.70f,		0.0f,	1.0f,	-0.5f,
		-0.14f,	-0.41f,  0.68f,		0.98f,	0.49f,		0.0f,	1.0f,	-0.5f,


		//Cara 9
		-0.36f,  0.24f,  1.6f,		0.74f,	0.53f,		0.0f,	1.0f,	-0.5f,
		-0.14f,	-0.41f,  0.68f,		0.98f,	0.499f,		0.0f,	1.0f,	-0.5f,
		-0.57f,  -0.41f, 0.42f,		0.98f,	0.27f,		0.0f,	1.0f,	-0.5f,
		-0.91f,	-0.16f,  0.68f,		0.86f,	0.16f,		0.0f,	1.0f,	-0.5f,

	};

	// Crear el objeto de malla para el dec�gono
	Mesh* decagono = new Mesh();
	decagono->CreateMesh(decagonoVertices, decagonoIndices, 42 * 8, 22 * 3); // 
	meshList.push_back(decagono);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado10Caras();


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AmTexture = Texture("Textures/amarillo.png");
	AmTexture.LoadTextureA();
	RoTexture = Texture("Textures/rojo.png");
	RoTexture.LoadTextureA();
	AzTexture = Texture("Textures/azul.png");
	AzTexture.LoadTextureA();
	VeTexture = Texture("Textures/verde.png");
	VeTexture.LoadTextureA();



	dado10CarasTexture = Texture("Textures/Dado10Caras.tga");
	dado10CarasTexture.LoadTextureA();


	BlackhawkBase = Model();
	BlackhawkBase.LoadModel("Models/HelicopteroBase.obj");
	BlackhawkHelices = Model();
	BlackhawkHelices.LoadModel("Models/HelicopteroHelices.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual

	pointLights[0] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//Luz blanca
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.3f,
		0.0f, 0.0f, 0.0f,
		0.2f, 0.01f, 0.001f);
	pointLightCount++;

	//Luz "apagada"
	pointLights[2] = PointLight(0.0f, 0.0f, 0.0f,
		0.5f, 0.3f,
		0.0f, 0.0f, 0.0f,
		0.2f, 0.01f, 0.001f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(0.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	//luz fija AZUL
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	//Luz amarilla
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	bool  spinDado=true;
	float rotateDadoX=0.0f;
	float rotateDadoY = 0.0f;
	float rotateDadoZ = 0.0f;
	float posicionDadoY = 3.0f;
	float rotateDadoOffset=0.1f;


	float movHeliX = 0.0f;
	float movHeliZ = 0.0f;
	bool avanzaX = true;
	bool avanzaZ = false;
	float rotacionHeli = 0.0f;
	float rotacionHelicopter = 1.0f;

	float rotacionHeliOffset = 15.0f;

	float movOffset = 0.3f;
	int vueltas=0;
	

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		

		rotacionHeli += rotacionHeliOffset * deltaTime;
		//Logica tirada dado
		if (mainWindow.getTiradaDado())
		{
			GLint NumeroObtendo = mainWindow.getNumeroDadoRandom();
			printf("N�mero random: %d\n", NumeroObtendo);

			switch (NumeroObtendo)
			{
			case 1:
				rotateDadoX = 40.2375f; 
				rotateDadoY = 56.72f; 
				rotateDadoZ = 50.0f;
				break;

			case 2:
				rotateDadoX = 30.145f; 
				rotateDadoY = -70.18f; 
				rotateDadoZ = 70.0f;
				break;

			case 3:
				rotateDadoX = -20.225f; 
				rotateDadoY = -40.36f; 
				rotateDadoZ = 10.0f;
				break;

			case 4:
				rotateDadoX = -11.125f; 
				rotateDadoY = -40.54f; 
				rotateDadoZ = 0.0f;
				break;

			case 5:
				rotateDadoX = -50.215f; 
				rotateDadoY = 30.0f; 
				rotateDadoZ = 40.0f;
				break;

			case 6:
				rotateDadoX = -16.125f; 
				rotateDadoY = -90.91f; 
				rotateDadoZ = 90.0f;
				break;

			case 7:
				rotateDadoX = -45.225f; 
				rotateDadoY = 50.36f; 
				rotateDadoZ = 60.0f;
				break;

			case 8:
				rotateDadoX = -59.125f; 
				rotateDadoY = 20.54f; 
				rotateDadoZ = 45.0f;
				break;

			case 9:
				rotateDadoX = -30.2375f; 
				rotateDadoY = 60.72f; 
				rotateDadoZ = 25.0f;
				break;

			case 10:
				rotateDadoX = -20.145f; 
				rotateDadoY = 30.18f; 
				rotateDadoZ = 15.0f;
				break;

			default:
				break;
			}
		}

		
		
		//logica helicpotero
		if (avanzaX && movHeliX <= 90.0f && movHeliZ == 0.0f) { //Derecha
			movHeliX += movOffset * deltaTime;
		}
		if (avanzaX && movHeliX > 90.0f && movHeliZ == 0.0f) { //Esquina
			avanzaX = false;
			avanzaZ = true;
			rotacionHelicopter += 90;
		}

		if (avanzaZ && movHeliZ <= 110.0f && movHeliX > 90.0f) { //Abajo
			movHeliZ += movOffset * deltaTime;
		}

		if (avanzaZ && movHeliZ >= 110.0f && movHeliX >= 90.0f) { //Esquina
			avanzaX = true;
			avanzaZ = false;
			movHeliX = 90.0f;
			movHeliZ = 110.0f;
			rotacionHelicopter += 90;

		}


		//Aqui se rompe porque si empieza el decremento pero una vez hecho se sale y compienza a validar todos los demas entonces aqui ya no se realiza el decremento por segunda vez
		if (avanzaX && movHeliX >= 90.0f && movHeliZ >= 110.0f) { //Izquierda
			movHeliX -= movOffset * deltaTime;
		}

		

		
		
		

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		/*	glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		*/
		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//casillas de tablero

		//casillas rojas
		RoTexture.UseTexture();

		model = glm::mat4(1.0); //casilla 1
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 4
		model = glm::translate(model, glm::vec3(30.3f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 7
		model = glm::translate(model, glm::vec3(60.6f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 10
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 13
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -30.3f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 19
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -90.9f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 24
		model = glm::translate(model, glm::vec3(60.6f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 28
		model = glm::translate(model, glm::vec3(20.2f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 34
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -70.7f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 38
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -30.3f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//casillas amarillas
		AmTexture.UseTexture();

		model = glm::mat4(1.0); //casilla 2
		model = glm::translate(model, glm::vec3(10.1f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 6
		model = glm::translate(model, glm::vec3(50.5f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 9
		model = glm::translate(model, glm::vec3(80.8f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 12
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -20.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 14
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -40.4f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 17
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -70.7f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 20
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -101.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 25
		model = glm::translate(model, glm::vec3(50.5f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 30
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 32
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -90.9f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 37
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -40.4f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//casillas azules
		AzTexture.UseTexture();

		model = glm::mat4(1.0); //casilla 3
		model = glm::translate(model, glm::vec3(20.2f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 8
		model = glm::translate(model, glm::vec3(70.7f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 11
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -10.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 15
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -50.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 18
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -80.8f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 22
		model = glm::translate(model, glm::vec3(80.8f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 26
		model = glm::translate(model, glm::vec3(40.4f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 31
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -101.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 36
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -50.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 39
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -10.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//casillas verdes
		VeTexture.UseTexture();

		model = glm::mat4(1.0); //casilla 5
		model = glm::translate(model, glm::vec3(40.4f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 16
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -60.6f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 21
		model = glm::translate(model, glm::vec3(90.9f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 23
		model = glm::translate(model, glm::vec3(70.7f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 27
		model = glm::translate(model, glm::vec3(30.3f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 29
		model = glm::translate(model, glm::vec3(10.1f, 0.2f, -111.1f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 33
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -80.8f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 35
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -60.6f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0); //casilla 39
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -20.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//-------------------------Dado 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-22.5f,posicionDadoY, -7.0f));
		model = glm::scale(model, glm::vec3(5.0, 5.0, 5.0));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotateDadoX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotateDadoZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotateDadoY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado10CarasTexture.UseTexture();
		meshList[4]->RenderMesh();



		//-------------------------Helicoptero
			
	
		model = glm::mat4(1.0);
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 25.0f, -110.0));
		model = glm::translate(model, glm::vec3(movHeliX, 0.0f, movHeliZ));

		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90*rotacionHelicopter * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		
		modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BlackhawkBase.RenderModel();

		//Luz que sigue al helicoptero
		glm::vec3 centro = glm::vec3(model[3][0] + 0.0f, model[3][1] + 0.0f, model[3][2] + 0.0f);
		spotLights[2].SetFlash(centro, glm::vec3(0.0f, -1.0f, 0.0f));

		model = modelaux;

		model = glm::translate(model, glm::vec3(0.0f , 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, rotacionHeli * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BlackhawkHelices.RenderModel();

		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
