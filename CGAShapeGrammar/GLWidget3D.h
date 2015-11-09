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
#include <opencv/cv.h>
#include <opencv/highgui.h>

class MainWindow;

class GLWidget3D : public QGLWidget {
public:
	GLWidget3D(QWidget *parent = 0);

	void drawScene(int drawMode);
	void loadCGA(char* filename);
	void generateWindowImages(int image_width, int image_height, bool invertImage, bool blur);
	void generateBuildingImages(int image_width, int image_height, bool invertImage, bool blur);
	void generateSimpleShapeImages(int image_width, int image_height, float scale);
	void test();
	void EDLine(cv::Mat& result, float scale);
	void draw2DPolyline(cv::Mat& img, const glm::vec2& p0, const glm::vec2& p1, int polyline_index);

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();    
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

public:
	static enum { RENDERING_MODE_REGULAR = 0, RENDERING_MODE_LINE };

public:
	Camera camera;
	glm::vec3 light_dir;
	glm::mat4 light_mvpMatrix;

	RenderManager renderManager;

	cga::CGA system;
	std::vector<std::vector<glm::vec2> > style_polylines;
};

