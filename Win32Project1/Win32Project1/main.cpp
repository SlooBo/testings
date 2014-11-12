#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <windowsx.h>
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
#include "glm\gtx\transform.hpp"

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
//  Position(3)				 Color(3)			 Coordinates(2)
	-0.5f,-0.5f,-0.5f,		 1.0f, 0.0f, 0.0f,	 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		 1.0f, 0.0f, 0.0f,	 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f,		 1.0f, 0.0f, 0.0f,	 -1.0f, -1.0f,
	0.5f, -0.5f, -0.5f,		 1.0f, 0.0f, 0.0f,	 -1.0f, 0.0f,
	-0.2f, -0.2f, 0.5f,		 1.0f, 0.0f, 0.0f,	 0.0f, 0.0f,
	-0.2f, 0.2f, 0.5f,		 1.0f, 0.0f, 0.0f,	 0.0f, -1.0f,
	0.2f, 0.2f, 0.5f,		 1.0f, 0.0f, 0.0f,	 -1.0f, -1.0f,
	0.2f, -0.2f, 0.5f,		 1.0f, 0.0f, 0.0f,	 -1.0f, 0.0f,
};

GLuint indices[] =
{
	0, 1, 2,
	0,2,3,	
	4,5,6,
	4,6,7,
	0,1,5,
	0,5,4,
	0,4,7,
	0,7,3,
	3,2,6,
	3,6,7,
	2,1,5,
	2,5,6
	
};

GLfloat vertices2[] = 
{
	//  Position(3)				 Color(3)			 Coordinates(2)
	-.5f, -.0f, 0.5f,			 0.0f, 1.0f, 0.0f,	 0.0f, 0.0f,
	.5f, -.0f, 0.5f,			 0.0f, 1.0f, 0.0f,	 0.0f, -1.0f,
	-.5f, -.0f, -.5f,			 0.0f, 1.0f, 0.0f,	 -1.0f, 0.0f,
	.5f, -.0f, -.5f,			 0.0f, 1.0f, 0.0f,	 -1.0f, -1.0f,

};


GLuint indices2[] = 
{
	0, 1, 2,
	1, 2, 3,
};

GLfloat vertices3[] =
{
	//  Position(3)				 Color(3)			 Coordinates(2)
	0.2f, -.0f, 0.2f,			 1.0f, 0.0f, 0.0f,	 0.0f, 0.0f,
	0.2f, .5f, 0.2f,			 1.0f, 0.0f, 0.0f,	 0.0f, -1.0f,
	0.2f, -.0f, -.2f,			 1.0f, 0.0f, 0.0f,	 -1.0f, 0.0f,
	0.2f, .5f, -.2f,			 1.0f, 0.0f, 0.0f,	 -1.0f, -1.0f,
	-0.2f, -.0f, 0.2f,			 1.0f, 0.0f, 0.0f,	 0.0f, 0.0f,
	-0.2f, .5f, 0.2f,			 1.0f, 0.0f, 0.0f,	 0.0f, -1.0f,
	-0.2f, -.0f, -.2f,			 1.0f, 0.0f, 0.0f,	 -1.0f, 0.0f,
	-0.2f, .5f, -.2f,			 1.0f, 0.0f, 0.0f,	 -1.0f, -1.0f,
};

GLuint indices3[] =
{
	0, 1, 2,
	1, 2, 3,
	4, 5, 6,
	5, 6, 7,
	0, 1, 5,
	0, 5, 4,
	3, 2, 6,
	3, 6, 7,
	2, 1, 5,
	2, 5, 6,
	1, 7, 3,
	1, 5, 7,
};
//DRAW
void renderFrame(GLfloat asd, float time, GLuint count)
{
	glUniform1f(asd, time);
	
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

//VBO creation
GLuint createVbo(GLfloat *vertices, GLint size)
{
	GLuint vboId;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	return vboId;
}

GLuint createEbo(GLuint *indices, GLint size)
{
	GLuint eboId;
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
	return eboId;
}

void init2DTexture(GLint texName, GLint texWidth, GLint texHeight, GLubyte *texPtr)
{
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texPtr);
	glBindTexture(GL_TEXTURE_2D, 0u);
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
		pfd.cStencilBits = 8u;

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

	case WM_MOUSEMOVE:

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
	glEnable(GL_DEPTH_TEST);

	//create VBO
	GLuint vbo;
	GLuint ebo;
	std::vector<GLuint>ebob;
	std::vector<GLuint>vbob;

	int size = sizeof(vertices);
	vbo = createVbo(&vertices[0], size);
	vbob.push_back(vbo);

	size = sizeof(indices);
	ebo = createEbo(&indices[0], size);
	ebob.push_back(ebo);

	size = sizeof(vertices2);
	vbo = createVbo(&vertices2[0], size);
	vbob.push_back(vbo);

	size = sizeof(indices2);
	ebo = createEbo(&indices2[0], size);
	ebob.push_back(ebo);

	size = sizeof(vertices3);
	vbo = createVbo(&vertices3[0], size);
	vbob.push_back(vbo);

	size = sizeof(indices3);
	ebo = createEbo(&indices3[0], size);
	ebob.push_back(ebo);

	//textures
	GLuint texture_diffuse, texture_normal;
	glGenTextures(1, &texture_diffuse);

	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, "testings.png");
	assert(error == 0);

	init2DTexture(texture_diffuse, width, height, image.data());

	glGenTextures(1, &texture_normal);
	std::vector<unsigned char> image2;
	error = lodepng::decode(image2, width, height, "testings_norm.png");
	assert(error == 0);

	init2DTexture(texture_normal, width, height, image2.data());

	//TEST STUFF
	/*
	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	std::vector<unsigned char> image2;
	unsigned width2, height2;
	unsigned error2 = lodepng::decode(image2, width2, height2, "testings_norm.png");
	assert(error == 0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.data());

	glBindTexture(GL_TEXTURE_2D, 0u);
	*/
	//shaders
	GLuint programId = loadShaders("Data/VertexShader.txt", "Data/FragmentShader.txt");
	glUseProgram(programId);

	//////////////UNIFORMIT
	GLfloat asd = glGetUniformLocation(programId, "time");
	float time = 0;
	float rotation = 0;
	GLuint diffuse = glGetUniformLocation(programId, "tex");
	glUniform1i(diffuse, texture_diffuse);
	GLuint normal = glGetUniformLocation(programId, "norm");
	glUniform1i(normal, texture_normal);
	
	GLuint lightpos = glGetUniformLocation(programId, "lightPos");
	glUniform3f(lightpos, 0.0f, 0.0f, 1.0f);
	GLuint resolution = glGetUniformLocation(programId, "Resolution");
	glUniform2f(resolution, 800, 800);
	GLuint lightcolor = glGetUniformLocation(programId, "lightColor");
	glUniform4f(lightcolor, 1.0f, 1.0f, 1.0f, 1.0f);
	GLuint ambientcolor = glGetUniformLocation(programId, "ambientColor");
	glUniform4f(ambientcolor, 0.8f, 0.8f, 0.8f, 0.3f);
	GLuint falloff = glGetUniformLocation(programId, "fallOff");
	glUniform3f(falloff, 0.4f, 7.0f, 30.0f);
	
	//projektio
	GLint projectionIndex = glGetUniformLocation(programId, "unifProjection");
	assert(projectionIndex != -1);

	glm::mat4 projectionTransfrom = glm::perspective(60.0f, static_cast<float>(800) / 800, 0.01f, 1000.0f);
	glUniformMatrix4fv(projectionIndex, 1, GL_FALSE, reinterpret_cast<float*>(&projectionTransfrom));

	//world
	GLint worldIndex = glGetUniformLocation(programId, "unifWorld");
	assert(worldIndex != -1);
	/*
	glm::mat4 worldTransform = glm::translate(glm::vec3(0.0f, 0.0f, -2.0f));
	glUniformMatrix4fv(worldIndex, 1, GL_FALSE, reinterpret_cast<float*>(&worldTransform));
	*/

	GLfloat qwe1 = glGetUniformLocation(programId, "unifAlpha");
	glUniform1f(qwe1, 1.f);

	//view
	GLint viewIndex = glGetUniformLocation(programId, "unifView");
	assert(viewIndex != -1);

	glm::mat4 viewTransform = glm::translate(glm::vec3(0.0f, -2.0f, -5.0f)) * glm::rotate(-30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(viewIndex, 1, GL_FALSE, reinterpret_cast<float*>(&viewTransform));

	/*
	//normal matrix
	GLint normalIndex = glGetUniformLocation(programId, "unifNormal");
	assert(normalIndex != -1);

	glm::mat4 normalTransform = glm::transpose(glm::inverse(viewTransform));
	glUniformMatrix4fv(normalIndex, 1, GL_FALSE, reinterpret_cast<float*>(&normalTransform));
	*/

	GLuint posAttrib = glGetAttribLocation(programId, "position");
	GLuint colAttrib = glGetAttribLocation(programId, "color");
	GLuint texAttrib = glGetAttribLocation(programId, "texcoord");

	int posX, posY;

	//main message loop
	while (!quit)
	{
		time += 0.1f;
		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //STENCIL VITTUUN JOS EI TARVII

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

		rotation++;


		glUseProgram(programId);
		glStencilMask(0x00);

		glEnableVertexAttribArray(posAttrib);
		glEnableVertexAttribArray(colAttrib);
		glEnableVertexAttribArray(texAttrib);

		//stencil options
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glDepthMask(GL_FALSE);
		
		//plane		
		glActiveTexture(GL_TEXTURE0 + texture_diffuse);
		glBindTexture(GL_TEXTURE_2D, texture_diffuse);
	//	glActiveTexture(GL_TEXTURE0 + texture2);
		//glBindTexture(GL_TEXTURE_2D, normal);
		

		glm::mat4 worldTransform = glm::rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(worldIndex, 1, GL_FALSE, reinterpret_cast<float*>(&worldTransform));

		glBindBuffer(GL_ARRAY_BUFFER, vbob[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebob[1]);
		
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		renderFrame(asd, time, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0u);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
		
		//stencil options
		glStencilFunc(GL_EQUAL, 1, 0xff);
		glStencilMask(0x00);
		glDepthMask(GL_TRUE);

		//reflection

		glUniform1f(qwe1, .5f);

		worldTransform = glm::rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::vec3(1.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(worldIndex, 1, GL_FALSE, reinterpret_cast<float*>(&worldTransform));

		glBindBuffer(GL_ARRAY_BUFFER, vbob[2]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebob[2]);

		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		renderFrame(asd, time, 36);

		glBindBuffer(GL_ARRAY_BUFFER, 0u);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);

		//stencil options
		glDisable(GL_STENCIL_TEST);
		glUniform1f(qwe1, 1.f);

		//kuutio
		
		worldTransform = glm::rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(worldIndex, 1, GL_FALSE, reinterpret_cast<float*>(&worldTransform));

		glBindBuffer(GL_ARRAY_BUFFER, vbob[2]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebob[2]);

		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		renderFrame(asd, time, 36);

		glBindBuffer(GL_ARRAY_BUFFER, 0u);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
		
	/*
		//palikka1
		worldTransform = glm::translate(glm::vec3(0.0f, 2.0f, 0.0f))  * glm::rotate(rotation, glm::vec3(1.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(worldIndex, 1, GL_FALSE, reinterpret_cast<float*>(&worldTransform));

		glBindBuffer(GL_ARRAY_BUFFER, vbob[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebob[0]);

		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		renderFrame(asd, time, 36);

		glBindBuffer(GL_ARRAY_BUFFER, 0u);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
		
		//palikka 2
		worldTransform = glm::translate(glm::vec3(1.0, 2.0f, 0.0f))  * glm::rotate(rotation, glm::vec3(1.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(worldIndex, 1, GL_FALSE, reinterpret_cast<float*>(&worldTransform));

		glBindBuffer(GL_ARRAY_BUFFER, vbob[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebob[0]);

		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		renderFrame(asd, time, 36);

		glBindBuffer(GL_ARRAY_BUFFER, 0u);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
	*/
		glUseProgram(0u);
		SwapBuffers(hDc);
	}
}
