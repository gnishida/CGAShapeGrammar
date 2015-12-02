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
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class MainWindow;

class GLWidget3D : public QGLWidget {
public:
	GLWidget3D(QWidget *parent = 0);

	void drawScene(int drawMode);
	void loadCGA(char* filename);
	void generateWindowImages(int image_width, int image_height, bool grayscale);
	void generateBuildingImages(int image_width, int image_height, bool grayscale);
	void generateSimpleShapeImages(int image_width, int image_height, float scale);
	void test();
	void EDLine(const cv::Mat& source, cv::Mat& result, bool grayscale);
	void draw2DPolyline(cv::Mat& img, const glm::vec2& p0, const glm::vec2& p1, int polyline_index);
	bool isImageValid(const cv::Mat& image);

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

