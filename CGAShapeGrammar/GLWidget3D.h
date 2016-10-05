#pragma once

#include <glew.h>
#include "Shader.h"
#include "Vertex.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include "Camera.h"
#include "ShadowMapping.h"
#include "RenderManager.h"
#include "CGA.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class MainWindow;

class GLWidget3D : public QGLWidget {
public:
	MainWindow* mainWin;

	// camera
	Camera camera;
	glm::vec3 light_dir;
	glm::mat4 light_mvpMatrix;

	// rendering engine
	RenderManager renderManager;

	// key status
	bool shiftPressed;
	bool ctrlPressed;

	std::vector<std::vector<glm::vec2>> style_polylines;
	boost::shared_ptr<QTimer> rotationTimer;

public:
	GLWidget3D(MainWindow *parent = 0);

	void drawScene();
	void render();
	void loadCGA(char* filename);
	void draw2DPolyline(QImage& img, const glm::vec2& p0, const glm::vec2& p1, int polyline_index);
	void rotationStart();
	void rotationEnd();

	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

public slots:
	void camera_update();

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent* e);

};

