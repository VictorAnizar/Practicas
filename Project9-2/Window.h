#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLint getNumeroDadoRandom() { return numeroDadoRandom; }
	GLboolean getTiradaDado() { return tiradaDado; }
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }
	GLfloat getarticulacion7() { return articulacion7; }
	GLfloat getarticulacion8() { return articulacion8; }
	GLfloat getarticulacion9() { return articulacion9; }
	GLfloat getarticulacionHelicoptero() { return articulacionHelicoptero; }
	GLfloat getisPrendido() { return isPrendido; }
	GLfloat getisPrendidoGru() { return isPrendidoGru; }
	GLfloat getllanta1() { return llanta1; }
	GLfloat getllanta2() { return llanta2; }
	GLfloat getllanta3() { return llanta3; }
	GLfloat getllanta4() { return llanta4; }

	GLint getLuzDelante() { return luzDelante; }
	GLint getLuzTrasera() { return lusTrazera; }


	GLfloat getmuevex() { return muevex; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLboolean tiradaDado;
	GLint numeroDadoRandom;
	GLfloat articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6, articulacion7, articulacion8, articulacion9, articulacionHelicoptero, llanta1, llanta2, llanta3, llanta4;
	GLboolean isPrendido;
	GLboolean isPrendidoGru;
	GLint luzDelante = 1;
	GLint lusTrazera = 0;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};
