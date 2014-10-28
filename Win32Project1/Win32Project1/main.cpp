#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include "glew.h"
#include <gl\GL.h>
#include <gl\GLU.h>
#include <assert.h>
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>
#include "lodepng.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "Sprite.h"

HDC hDc;

RECT wSize{ 100, 100, 800, 800 };
static wchar_t wClassName[] = L"testClassName";
static wchar_t wTitleName[] = L"testTitleName";

GLuint loadShaders(const char* vsPath, const char* fsPath)
{
	//shader creation
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	if (vsPath != " ")
	{
		std::ifstream vertexShaderStream(vsPath, std::ios::in);
		if (vertexShaderStream.is_open())
		{
			std::string line = "";
			while (std::getline(vertexShaderStream, line))
				vertexShaderCode += "\n" + line;
			vertexShaderStream.close();
		}
	}

	if (fsPath != " ")
	{
		std::ifstream fragmentShaderStream(fsPath, std::ios::in);
		if (fragmentShaderStream.is_open())
		{
			std::string line = "";
			while (std::getline(fragmentShaderStream, line))
				fragmentShaderCode += "\n" + line;
			fragmentShaderStream.close();
		}
	}

	//shader compiles
	char const *vertexShaderPointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderId, 1, &vertexShaderPointer, NULL);
	glCompileShader(vertexShaderId);

	char const *fragmentShaderPointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderId, 1, &fragmentShaderPointer, NULL);
	glCompileShader(fragmentShaderId);

	//check
	GLint compileResult;

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &compileResult);
	assert(compileResult == GL_TRUE);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &compileResult);
	assert(compileResult == GL_TRUE);


	//program id linking
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	//chec
	glGetProgramiv(programId, GL_LINK_STATUS, &compileResult);
	assert(compileResult == GL_TRUE);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}


//vertex array
GLfloat vertices[] =
{
//  Position(3)				 Color(3)			 Coordinates(3)
	-0.5f,-0.5f,-0.5f,		 1.0f, 0.0f, 0.0f,	 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		 1.0f, 0.0f, 0.0f,	 0.0f, -1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		 1.0f, 0.0f, 0.0f,	 -1.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		 1.0f, 0.0f, 0.0f,	 -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		 1.0f, 0.0f, 0.0f,	 0.0f, 0.0f, -1.0f,
	-0.5f, 0.5f, 0.5f,		 1.0f, 0.0f, 0.0f,	 0.0f, -1.0f, -1.0f,
	0.5f, 0.5f, 0.5f,		 1.0f, 0.0f, 0.0f,	 -1.0f, -1.0f, -1.0f,
	0.5f, -0.5f, 0.5f,		 1.0f, 0.0f, 0.0f,	 -1.0f, 0.0f, -1.0f
};

GLuint indices[] =
{
	4, 5, 1, 0,
	5, 6, 2, 1,
	6, 7, 3, 2,
	7, 4, 0, 3,
	0, 1, 2, 3,
	7, 6, 5, 4
};

//DRAW
void renderFrame(GLfloat asd, float time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform1f(asd, time);
	
	glDrawElements(GL_QUADS, 32, GL_UNSIGNED_INT, 0);
}

//VBO creation
GLuint createVbo(GLfloat *vertices, GLint size)
{
	GLuint vboId;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	return vboId;
}

GLuint createEbo(GLuint *indices, GLint size)
{
	GLuint eboId;
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	return eboId;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int pixelFormat;
	HGLRC hGlrc;
	PIXELFORMATDESCRIPTOR pfd;

	switch (uMsg)
	{
	case WM_CREATE:
		pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL, PFD_TYPE_RGBA,
			24,
			24
		};

		hDc = GetDC(hWnd);
		pixelFormat = ChoosePixelFormat(hDc, &pfd);
		SetPixelFormat(hDc, pixelFormat, &pfd);
		hGlrc = wglCreateContext(hDc);
		wglMakeCurrent(hDc, hGlrc);

		break;

	case WM_DESTROY:
		hGlrc = nullptr;
		wglDeleteContext(hGlrc);
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, NULL, NULL);
		}
		return 0;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

int main()
{
	//window create
	WNDCLASSEX window;
	MSG uMsg;
	HWND hWnd;
	HINSTANCE hInstance;

	bool quit = false;

	hInstance = GetModuleHandle(nullptr);

	window.cbSize = sizeof(WNDCLASSEX);
	window.style = CS_OWNDC;
	window.lpfnWndProc = WndProc;
	window.hInstance = hInstance;
	window.lpszClassName = wClassName;
	window.lpszMenuName = NULL;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hIcon = NULL;
	window.hCursor = LoadCursor(NULL, IDC_ARROW);
	window.hbrBackground = NULL;
	window.hIconSm = LoadIcon(window.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	RegisterClassEx(&window);
	AdjustWindowRect(&wSize, WS_CAPTION, false);
	hWnd = CreateWindowEx(NULL, wClassName, wTitleName, WS_OVERLAPPEDWINDOW, wSize.left, wSize.top, wSize.right, wSize.bottom, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, L"Window creation failed!", wTitleName, NULL);
		return 1;
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	//GLEW init
	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL, L"glewInit() failure", NULL, NULL);
		return -1;
	}

	//create VBO
	int size = sizeof(vertices);
	int m_vboId = createVbo(&vertices[0], size);

	size = sizeof(indices);
	int m_eboId = createEbo(&indices[0], size);

	//textures
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode (image, width, height, "testings.png");
	assert(error == 0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

	//shaders
	GLuint programId = loadShaders("Data/VertexShader.txt", "Data/FragmentShader.txt");
	glUseProgram(programId);

	GLuint posAttrib = glGetAttribLocation(programId, "position");   //"position" talteen, "käyttäjä" määrittää
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	//size tarviiiiiii, (FLOAT TULEVAISUUDESSA WRÄPÄTÄÄN), stride TALTEEN, viimesen voi laskea itse 

	GLuint colAttrib = glGetAttribLocation(programId, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

	GLuint texAttrib = glGetAttribLocation(programId, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));

	//const GLuint projLocation = glGetUniformLocation(programId, "proj");

	GLfloat asd = glGetUniformLocation(programId, "time");

	float time = 0;

	//ORTHO CAMERA
	/*
	const glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(wSize.bottom), static_cast<float>(wSize.right), 0.0f, 0.0f, 1.0f);
	glUseProgram(programId);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, reinterpret_cast<const float*>(&projection));
	*/

	//main message loop
	while (!quit)
	{
		time += 0.1f;
		while (PeekMessage(&uMsg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (uMsg.message == WM_QUIT)
			{
				quit = true;
				break;
			}
			TranslateMessage(&uMsg);
			DispatchMessage(&uMsg);

		}
		renderFrame(asd, time);
		SwapBuffers(hDc);
	}
	glDeleteTextures(1, &texture);
}
