#pragma once

#include <glew.h>
#include "Shader.h"
#include "Vertex.h"
#include <QGLWidget>
#include <QMouseEvent>
#include "Camera.h"
#include "ShadowMapping.h"
#include "RenderManager.h"
#include "CGA.h"

class MainWindow;

class GLWidget3D : public QGLWidget {
public:
	GLWidget3D(QWidget *parent = 0);

	void drawScene(int drawMode);

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();    
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

public:
	Camera camera;
	glm::vec3 light_dir;
	glm::mat4 light_mvpMatrix;

	RenderManager renderManager;
	bool showWireframe;

	cga::CGA system;
};

