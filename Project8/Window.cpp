#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	articulacion7 = 0.0f;
	articulacion8 = 0.0f;
	articulacion9 = 0.0f;
	articulacionHelicoptero = 0.0f;
	llanta1 = 0.0f;
	llanta2 = 0.0f;
	llanta3 = 0.0f;
	llanta4 = 0.0f;
	muevex = 2.0f;
	isPrendido = false;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica08:Iluminacion2", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex -= 1.0;
	}
	

	if (key == GLFW_KEY_F)
	{
		theWindow->articulacion1 += 10.0;
	}
	if (key == GLFW_KEY_G)
	{
		theWindow->articulacion2 -= 10.0;
	}
	//Abre cofre
	if (key == GLFW_KEY_H)
	{
		theWindow->articulacion3 += 10.0;
	}
	//Cirerra cofre
	if (key == GLFW_KEY_J)
	{
		theWindow->articulacion4 -= 10.0;
	}
	//AVnanza caro
	if (key == GLFW_KEY_K)
	{
		theWindow->articulacion5 += 10.0;
		theWindow->luzDelante = 1.0;
		theWindow->lusTrazera = 0.0;
	}
	//Retrocede carro
	if (key == GLFW_KEY_L)
	{
		theWindow->articulacion6 -= 10.0;
		theWindow->lusTrazera = 1.0;
		theWindow->luzDelante = 0.0;

	}
	if (key == GLFW_KEY_Z)
	{
		theWindow->articulacion7 += 10.0;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow->articulacion8 += 10.0;
	}
	if (key == GLFW_KEY_C)
	{
		theWindow->articulacionHelicoptero -= 10.0;
	}
	if (key == GLFW_KEY_V)
	{
		theWindow->articulacionHelicoptero += 10.0;
	}
	//Prende apaga gru
	if (key == GLFW_KEY_U)
	{
		theWindow->isPrendidoGru = false;
	}
	if (key == GLFW_KEY_I)
	{
		theWindow->isPrendidoGru = true;
	}
	//Prende apagar lampara lava
	if (key == GLFW_KEY_O)
	{
		theWindow->isPrendido = false;
	}
	if (key == GLFW_KEY_P)
	{
		theWindow->isPrendido = true;
	}
	
	





	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
