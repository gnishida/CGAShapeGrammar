#include "GLWidget3D.h"
#include "MainWindow.h"
#include "OBJLoader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "GrammarParser.h"
#include <map>
#include "Rectangle.h"
#include "Circle.h"
#include "Polygon.h"
#include "UShape.h"
#include "GLUtils.h"
#include <QDir>
#include <QTextStream>
#include <iostream>
#include "EDLinesLib.h"

GLWidget3D::GLWidget3D(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers)) {
	// 光源位置をセット
	// ShadowMappingは平行光源を使っている。この位置から原点方向を平行光源の方向とする。
	light_dir = glm::normalize(glm::vec3(-4, -5, -8));

	// シャドウマップ用のmodel/view/projection行列を作成
	glm::mat4 light_pMatrix = glm::ortho<float>(-50, 50, -50, 50, 0.1, 200);
	glm::mat4 light_mvMatrix = glm::lookAt(-light_dir * 50.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	light_mvpMatrix = light_pMatrix * light_mvMatrix;

	// initialize stylized polylines
	style_polylines.resize(10);
	style_polylines[0].push_back(glm::vec2(-0.025, -0.025));
	style_polylines[0].push_back(glm::vec2(0.3, 0.035));
	style_polylines[0].push_back(glm::vec2(0.6, 0.05));
	style_polylines[0].push_back(glm::vec2(0.85, 0.04));
	style_polylines[0].push_back(glm::vec2(1.01, 0.02));

	style_polylines[1].push_back(glm::vec2(-0.01, 0.01));
	style_polylines[1].push_back(glm::vec2(0.13, -0.01));
	style_polylines[1].push_back(glm::vec2(0.27, -0.02));
	style_polylines[1].push_back(glm::vec2(0.7, -0.02));
	style_polylines[1].push_back(glm::vec2(0.81, 0));
	style_polylines[1].push_back(glm::vec2(1.02, 0));

	style_polylines[2].push_back(glm::vec2(-0.02, 0.0));
	style_polylines[2].push_back(glm::vec2(0.12, 0.01));
	style_polylines[2].push_back(glm::vec2(0.37, 0.02));
	style_polylines[2].push_back(glm::vec2(0.6, 0.02));
	style_polylines[2].push_back(glm::vec2(0.77, 0.01));
	style_polylines[2].push_back(glm::vec2(0.91, 0.005));
	style_polylines[2].push_back(glm::vec2(0.99, -0.01));

	style_polylines[3].push_back(glm::vec2(-0.02, 0.0));
	style_polylines[3].push_back(glm::vec2(0.57, -0.01));
	style_polylines[3].push_back(glm::vec2(0.8, -0.01));
	style_polylines[3].push_back(glm::vec2(1.01, 0.01));

	style_polylines[4].push_back(glm::vec2(-0.01, 0.0));
	style_polylines[4].push_back(glm::vec2(0.13, -0.01));
	style_polylines[4].push_back(glm::vec2(0.23, -0.02));
	style_polylines[4].push_back(glm::vec2(0.31, -0.02));
	style_polylines[4].push_back(glm::vec2(0.38, -0.01));
	style_polylines[4].push_back(glm::vec2(0.46, 0.0));
	style_polylines[4].push_back(glm::vec2(0.61, 0.02));
	style_polylines[4].push_back(glm::vec2(0.68, 0.03));
	style_polylines[4].push_back(glm::vec2(0.8, 0.03));
	style_polylines[4].push_back(glm::vec2(0.88, 0.02));
	style_polylines[4].push_back(glm::vec2(0.97, 0.01));

	style_polylines[5].push_back(glm::vec2(0.05, -0.04));
	style_polylines[5].push_back(glm::vec2(0.29, -0.03));
	style_polylines[5].push_back(glm::vec2(0.47, -0.01));
	style_polylines[5].push_back(glm::vec2(0.59, 0.02));
	style_polylines[5].push_back(glm::vec2(0.75, 0.03));
	style_polylines[5].push_back(glm::vec2(1.03, 0.04));

	style_polylines[6].push_back(glm::vec2(-0.02, 0.04));
	style_polylines[6].push_back(glm::vec2(0.16, -0.01));
	style_polylines[6].push_back(glm::vec2(0.42, -0.06));
	style_polylines[6].push_back(glm::vec2(0.65, -0.07));
	style_polylines[6].push_back(glm::vec2(0.83, -0.04));
	style_polylines[6].push_back(glm::vec2(0.98, -0.02));

	style_polylines[7].push_back(glm::vec2(0.0, 0.0));
	style_polylines[7].push_back(glm::vec2(0.24, 0.02));
	style_polylines[7].push_back(glm::vec2(0.59, 0.03));
	style_polylines[7].push_back(glm::vec2(0.79, 0.01));
	style_polylines[7].push_back(glm::vec2(0.91, -0.01));
	style_polylines[7].push_back(glm::vec2(1.02, -0.04));

	style_polylines[8].push_back(glm::vec2(-0.01, -0.02));
	style_polylines[8].push_back(glm::vec2(0.15, 0.0));
	style_polylines[8].push_back(glm::vec2(0.28, 0.02));
	style_polylines[8].push_back(glm::vec2(0.44, 0.01));
	style_polylines[8].push_back(glm::vec2(0.59, 0.0));
	style_polylines[8].push_back(glm::vec2(0.74, -0.03));
	style_polylines[8].push_back(glm::vec2(0.81, -0.04));
	style_polylines[8].push_back(glm::vec2(0.89, -0.04));
	style_polylines[8].push_back(glm::vec2(0.98, -0.03));

	style_polylines[9].push_back(glm::vec2(0.02, -0.02));
	style_polylines[9].push_back(glm::vec2(0.41, -0.03));
	style_polylines[9].push_back(glm::vec2(0.56, -0.04));
	style_polylines[9].push_back(glm::vec2(0.68, -0.03));
	style_polylines[9].push_back(glm::vec2(0.78, -0.02));
	style_polylines[9].push_back(glm::vec2(0.85, -0.01));
	style_polylines[9].push_back(glm::vec2(0.94, 0.0));
	style_polylines[9].push_back(glm::vec2(0.96, 0.02));
}

/**
 * This event handler is called when the mouse press events occur.
 */
void GLWidget3D::mousePressEvent(QMouseEvent *e) {
	camera.mousePress(e->x(), e->y());
}

/**
 * This event handler is called when the mouse release events occur.
 */
void GLWidget3D::mouseReleaseEvent(QMouseEvent *e) {
}

/**
 * This event handler is called when the mouse move events occur.
 */
void GLWidget3D::mouseMoveEvent(QMouseEvent *e) {
	if (e->buttons() & Qt::LeftButton) { // Rotate
		camera.rotate(e->x(), e->y());
	} else if (e->buttons() & Qt::MidButton) { // Move
		camera.move(e->x(), e->y());
	} else if (e->buttons() & Qt::RightButton) { // Zoom
		camera.zoom(e->x(), e->y());
	}

	updateGL();
}

/**
 * This function is called once before the first call to paintGL() or resizeGL().
 */
void GLWidget3D::initializeGL() {
	renderManager.init("../shaders/vertex.glsl", "../shaders/geometry.glsl", "../shaders/fragment.glsl", true, 4096);

	// set the clear color for the screen
	//glClearColor(1, 1, 1, 1);
	glClearColor(0.95, 0.95, 0.95, 1.0);

	system.modelMat = glm::rotate(glm::mat4(), -3.1415926f * 0.5f, glm::vec3(1, 0, 0));

	/*
	std::vector<Vertex> vertices;
	glutils::drawGrid(60, 60, 1, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::rotate(glm::mat4(), -3.1415926f * 0.5f, glm::vec3(1, 0, 0)), vertices);
	renderManager.addObject("grid", "", vertices);
	*/
}

/**
 * This function is called whenever the widget has been resized.
 */
void GLWidget3D::resizeGL(int width, int height) {
	height = height ? height : 1;
	glViewport(0, 0, width, height);
	camera.updatePMatrix(width, height);

	//rb.update(width, height);
	renderManager.resize(width, height);
}

/**
 * This function is called whenever the widget needs to be painted.
 */
void GLWidget3D::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	// Model view projection行列をシェーダに渡す
	glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"),  1, GL_FALSE, &camera.mvpMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"),  1, GL_FALSE, &camera.mvMatrix[0][0]);

	// pass the light direction to the shader
	//glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
	glUniform3f(glGetUniformLocation(renderManager.program, "lightDir"), light_dir.x, light_dir.y, light_dir.z);
	
	drawScene(0);
}

/**
 * Draw the scene.
 */
void GLWidget3D::drawScene(int drawMode) {
	if (drawMode == 0) {
		glUniform1i(glGetUniformLocation(renderManager.program, "depthComputation"), 0);
	} else {
		glUniform1i(glGetUniformLocation(renderManager.program, "depthComputation"), 1);
	}

	renderManager.renderAll();
}

void GLWidget3D::loadCGA(char* filename) {
	renderManager.removeObjects();

	float offset_x = 0.0f;
	float offset_y = 0.0f;

#if 0
	{ // for window
		camera.pos = glm::vec3(0, 0, 7);
		camera.updateMVPMatrix();
		float object_width = 2.0f;
		float object_depth = 1.0f;

		cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));

		//cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for building
		camera.pos = glm::vec3(0, 5, 80);
		camera.updateMVPMatrix();

		float object_width = 28.0f;
		float object_depth = 28.0f;

		float offset_x = 0.0f;
		float offset_y = 0.0f;
		float offset_z = 0.0f;
		cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, offset_z)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));

		//cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for roof
		float object_width = 18.0f;
		float object_depth = 12.0f;

		float offset_x = 0.0f;
		float offset_y = 0.0f;
		cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));

		//cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for circular roof
		float object_width = 12.0f;
		float object_depth = 12.0f;

		float offset_x = 0.0f;
		float offset_y = 0.0f;
		cga::Circle* start = new cga::Circle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 1
	{ // for ledge
		float object_width = 12.0f;
		float object_depth = 0.5f;

		float offset_x = 0.0f;
		float offset_y = 0.0f;
		cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));

		//cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for building G
		float object_width = 10.0f;
		float object_depth = 10.0f;// 8.0f;

		cga::Rectangle* start1 = new cga::Rectangle("Start1", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 1.5, offset_y - (float)object_depth * 1.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start1));
		cga::Rectangle* start2 = new cga::Rectangle("Start2", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 0.5, offset_y - (float)object_depth * 1.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start2));
		cga::Rectangle* start3 = new cga::Rectangle("Start3", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x + (float)object_width * 0.5, offset_y - (float)object_depth * 1.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start3));

		cga::Rectangle* start4 = new cga::Rectangle("Start4", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 1.5, offset_y - (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start4));
		cga::Rectangle* start5 = new cga::Rectangle("Start5", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 0.5, offset_y - (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start5));
		cga::Rectangle* start6 = new cga::Rectangle("Start6", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x + (float)object_width * 0.5, offset_y - (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start6));

		cga::Rectangle* start7 = new cga::Rectangle("Start7", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 1.5, offset_y + (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start7));
		cga::Rectangle* start8 = new cga::Rectangle("Start8", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width * 0.5, offset_y + (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start8));
		cga::Rectangle* start9 = new cga::Rectangle("Start9", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x + (float)object_width * 0.5, offset_y + (float)object_depth * 0.5, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start9));
	}
#endif

#if 0
	{ // for building R
		float object_width = 32.0f;
		float object_depth = 24.0f;

		cga::UShape* start = new cga::UShape("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, 10, 8, glm::vec3(1, 1, 1));

		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for building T
		float object_depth = 14.0f;// 8.0f;

		cga::Rectangle* start1 = new cga::Rectangle("Start1", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)50 * 0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), 50, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start1));
		cga::Rectangle* start2 = new cga::Rectangle("Start2", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)50 * 0.5f, offset_y - (float)object_depth*0.5f, 20)), glm::mat4(), 40, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start2));
		cga::Rectangle* start3 = new cga::Rectangle("Start3", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)50 * 0.5f, offset_y - (float)object_depth*0.5f, 25)), glm::mat4(), 35, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start3));
		cga::Rectangle* start4 = new cga::Rectangle("Start4", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)50 * 0.5f, offset_y - (float)object_depth*0.5f, 30)), glm::mat4(), 25, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start4));
	}
#endif

#if 0
	{ // for building Paris
		float object_width = 28.0f;
		float object_depth = 20.0f;

		cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));

		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}
#endif

#if 0
	{ // for building Griffith Observatory
		float object_width = 10.0f;
		float object_depth = 10.0f;

		cga::Circle* start1 = new cga::Circle("Start1", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(- (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start1));
		cga::Rectangle* start2 = new cga::Rectangle("Start2", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-3, -15, 0)), glm::mat4(), 6, 10, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start2));
		cga::Rectangle* start3 = new cga::Rectangle("Start3", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-12, -14, 0)), glm::mat4(), 9, 6, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start3));
		cga::Rectangle* start4 = new cga::Rectangle("Start4", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(3, -14, 0)), glm::mat4(), 9, 6, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start4));
		cga::Circle* start5 = new cga::Circle("Start5", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-14, -13, -0)), glm::mat4(), 4, 4, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start5));
		cga::Circle* start6 = new cga::Circle("Start6", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(10, -13, 0)), glm::mat4(), 4, 4, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start6));

	}
#endif

	try {
		cga::Grammar grammar;
		cga::parseGrammar(filename, grammar);
		//system.randomParamValues(grammar);
		system.derive(grammar, true);
		std::vector<boost::shared_ptr<glutils::Face> > faces;
		system.generateGeometry(faces);
		renderManager.addFaces(faces);
		//renderManager.centerObjects();
	} catch (const std::string& ex) {
		std::cout << "ERROR:" << std::endl << ex << std::endl;
	} catch (const char* ex) {
		std::cout << "ERROR:" << std::endl << ex << std::endl;
	}
	
	renderManager.updateShadowMap(this, light_dir, light_mvpMatrix);

	updateGL();
}

void GLWidget3D::generateBuildingImages(int image_width, int image_height, bool grayscale) {
	QDir dir("..\\cga\\building\\");

	if (!QDir("results").exists()) QDir().mkdir("results");

	srand(0);
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		int count = 0;

		if (!QDir("results/" + fileInfoList[i].baseName()).exists()) QDir().mkdir("results/" + fileInfoList[i].baseName());

		QFile file("results/" + fileInfoList[i].baseName() + "/parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (int object_width = 4; object_width <= 28; object_width += 2) {
			for (int object_depth = 4; object_depth <= 28; object_depth += 2) {
				for (int pitch_angle = 25; pitch_angle <= 35; pitch_angle += 5) {
					for (int yaw_angle = -50; yaw_angle <= -40; yaw_angle += 5) {
						// change camera view direction
						camera.xrot = pitch_angle;//35.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
						camera.yrot = yaw_angle;//-45.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
						camera.zrot = 0.0f;
						camera.pos = glm::vec3(0, 5, 80);
						camera.updateMVPMatrix();

						for (int offset_x = -8; offset_x <= 8; offset_x += 2) {
							for (int offset_y = -8; offset_y <= 8; offset_y += 2) {
								// sample only half the total combinations
								if (rand() % 2 != 0) continue;

								for (int k = 0; k < 1; ++k) { // 1 images (parameter values are randomly selected) for each width and height				
									std::vector<float> param_values;

									renderManager.removeObjects();

									// generate a window
									cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y - (float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
									system.stack.push_back(boost::shared_ptr<cga::Shape>(start));

									try {
										cga::Grammar grammar;
										cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
										param_values = system.randomParamValues(grammar);
										system.derive(grammar, true);
										std::vector<boost::shared_ptr<glutils::Face> > faces;
										system.generateGeometry(faces);
										renderManager.addFaces(faces);
										//renderManager.centerObjects();
									}
									catch (const std::string& ex) {
										std::cout << "ERROR:" << std::endl << ex << std::endl;
									}
									catch (const char* ex) {
										std::cout << "ERROR:" << std::endl << ex << std::endl;
									}

									// render a window
									glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
									glEnable(GL_DEPTH_TEST);
									glDisable(GL_TEXTURE_2D);

									glUniform1i(glGetUniformLocation(renderManager.program, "seed"), rand() % 100);

									// Model view projection行列をシェーダに渡す
									glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"), 1, GL_FALSE, &camera.mvpMatrix[0][0]);
									glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"), 1, GL_FALSE, &camera.mvMatrix[0][0]);

									// pass the light direction to the shader
									//glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
									glUniform3f(glGetUniformLocation(renderManager.program, "lightDir"), light_dir.x, light_dir.y, light_dir.z);

									drawScene(0);

									QImage img = this->grabFrameBuffer();
									cv::Mat source(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
									cv::Mat mat;
									EDLine(source, mat, grayscale);

									// 画像を縮小
									cv::resize(mat, mat, cv::Size(256, 256));
									cv::threshold(mat, mat, 250, 255, CV_THRESH_BINARY);

									cv::resize(mat, mat, cv::Size(image_width, image_height));
									cv::threshold(mat, mat, 250, 255, CV_THRESH_BINARY);

									// 画像が有効なら保存
									if (isImageValid(mat)) {
										// put depth, width at the begining of the param values array
										param_values.insert(param_values.begin() + 0, (float)(offset_x + 8) / 16.0f);
										param_values.insert(param_values.begin() + 1, (float)(offset_y + 8) / 16.0f);
										param_values.insert(param_values.begin() + 2, (float)(object_width - 4) / 24.0f);
										param_values.insert(param_values.begin() + 3, (float)(object_depth - 4) / 24.0f);

										// set filename
										QString filename = "results/" + fileInfoList[i].baseName() + "/" + QString("image_%1.png").arg(count, 6, 10, QChar('0'));

										cv::imwrite(filename.toUtf8().constData(), mat);

										// write all the param values to the file
										for (int pi = 0; pi < param_values.size(); ++pi) {
											if (pi > 0) {
												out << ",";
											}
											out << param_values[pi];
										}
										out << "\n";

										count++;
									}

								}


							}
						}
					}
				}
			}
		}

		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}

void GLWidget3D::generateRoofImages(int image_width, int image_height, bool grayscale) {
	QDir dir("..\\cga\\roof\\");

	if (!QDir("results").exists()) QDir().mkdir("results");

	srand(0);
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	const int NUM_RECTANGULAR_TYPE = 5;

	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		int count = 0;

		if (!QDir("results/" + fileInfoList[i].baseName()).exists()) QDir().mkdir("results/" + fileInfoList[i].baseName());

		QFile file("results/" + fileInfoList[i].baseName() + "/parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (float object_width = 4.0f; object_width <= 28.0f; object_width += 4.0f) {
			for (float object_depth = 4.0f; object_depth <= 28.0f; object_depth += 4.0f) {
				if (i >= NUM_RECTANGULAR_TYPE && object_width != object_depth) continue;

				for (int pitch_angle = 25; pitch_angle <= 35; pitch_angle += 5) {
					for (int yaw_angle = -50; yaw_angle <= -40; yaw_angle += 5) {
						// change camera view direction
						camera.xrot = pitch_angle;//35.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
						camera.yrot = yaw_angle;//-45.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
						camera.zrot = 0.0f;
						camera.pos = glm::vec3(0, 5, 80);
						camera.updateMVPMatrix();

						int numSamples = 5;
						if (i >= NUM_RECTANGULAR_TYPE) numSamples *= 7;
						for (int k = 0; k < numSamples; ++k) { // 1 images (parameter values are randomly selected) for each width and height
							std::vector<float> param_values;

							renderManager.removeObjects();

							// generate a roof base
							if (i < NUM_RECTANGULAR_TYPE) { // rectangular base
								cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
								system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
							}
							else {	// circular base
								cga::Circle* start = new cga::Circle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
								system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
							}

							// generate a roof
							try {
								cga::Grammar grammar;
								cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
								param_values = system.randomParamValues(grammar);
								system.derive(grammar, true);
								std::vector<boost::shared_ptr<glutils::Face> > faces;
								system.generateGeometry(faces);
								renderManager.addFaces(faces);
							}
							catch (const std::string& ex) {
								std::cout << "ERROR:" << std::endl << ex << std::endl;
							}
							catch (const char* ex) {
								std::cout << "ERROR:" << std::endl << ex << std::endl;
							}

							// render the roof
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
							glEnable(GL_DEPTH_TEST);
							glDisable(GL_TEXTURE_2D);

							glUniform1i(glGetUniformLocation(renderManager.program, "seed"), rand() % 100);

							// Model view projection行列をシェーダに渡す
							glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"), 1, GL_FALSE, &camera.mvpMatrix[0][0]);
							glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"), 1, GL_FALSE, &camera.mvMatrix[0][0]);

							// pass the light direction to the shader
							//glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
							glUniform3f(glGetUniformLocation(renderManager.program, "lightDir"), light_dir.x, light_dir.y, light_dir.z);

							drawScene(0);

							// replace the edges by sketchy polylines
							QImage img = this->grabFrameBuffer();
							cv::Mat source(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
							cv::Mat mat;
							EDLine(source, mat, grayscale);

							// 画像を縮小
							cv::resize(mat, mat, cv::Size(256, 256));
							cv::threshold(mat, mat, 250, 255, CV_THRESH_BINARY);

							cv::resize(mat, mat, cv::Size(image_width, image_height));
							cv::threshold(mat, mat, 250, 255, CV_THRESH_BINARY);

							// set filename
							QString filename = "results/" + fileInfoList[i].baseName() + "/" + QString("image_%1.png").arg(count, 6, 10, QChar('0'));

							cv::imwrite(filename.toUtf8().constData(), mat);

							// write all the param values to the file
							for (int pi = 0; pi < param_values.size(); ++pi) {
								if (pi > 0) {
									out << ",";
								}
								out << param_values[pi];
							}
							out << "\n";



							count++;
						}
					}
				}
			}
		}

		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}

void GLWidget3D::generateWindowImages(int image_width, int image_height, bool grayscale) {
	QDir dir("..\\cga\\window\\");

	if (!QDir("results").exists()) QDir().mkdir("results");

	srand(0);
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	camera.xrot = 90.0f;
	camera.yrot = 0.0f;
	camera.zrot = 0.0f;
	camera.pos = glm::vec3(0, 0, 5.0f);
	camera.updateMVPMatrix();

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		int count = 0;

		if (!QDir("results/" + fileInfoList[i].baseName()).exists()) QDir().mkdir("results/" + fileInfoList[i].baseName());

		QFile file("results/" + fileInfoList[i].baseName() + "/parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (float object_width = 0.3f; object_width <= 3.0f; object_width += 0.1f) {
			for (float object_height = 1.0f; object_height <= 1.0f; object_height += 0.1f) {
				for (int k = 0; k < 10; ++k) { // 1 images (parameter values are randomly selected) for each width and height
					std::vector<float> param_values;

					renderManager.removeObjects();

					// generate a window
					cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
					system.stack.push_back(boost::shared_ptr<cga::Shape>(start));

					try {
						cga::Grammar grammar;
						cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
						param_values = system.randomParamValues(grammar);
						system.derive(grammar, true);
						std::vector<boost::shared_ptr<glutils::Face> > faces;
						system.generateGeometry(faces);
						renderManager.addFaces(faces);
						//renderManager.centerObjects();
					}
					catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}


					// put a background plane
					std::vector<Vertex> vertices;
					glutils::drawQuad(100, 100, glm::vec4(1, 1, 1, 1), glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(0, 0, -10)), vertices);
					renderManager.addObject("background", "", vertices);

					// render a window
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glEnable(GL_DEPTH_TEST);
					glDisable(GL_TEXTURE_2D);

					glUniform1i(glGetUniformLocation(renderManager.program, "seed"), rand() % 100);

					// Model view projection行列をシェーダに渡す
					glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"), 1, GL_FALSE, &camera.mvpMatrix[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"), 1, GL_FALSE, &camera.mvMatrix[0][0]);

					// pass the light direction to the shader
					//glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
					glUniform3f(glGetUniformLocation(renderManager.program, "lightDir"), light_dir.x, light_dir.y, light_dir.z);

					drawScene(0);

					QImage img = this->grabFrameBuffer();
					cv::Mat source(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
					cv::Mat mat;
					EDLine(source, mat, grayscale);

					// 画像を縮小
					cv::resize(mat, mat, cv::Size(256, 256));
					cv::threshold(mat, mat, 250, 255, CV_THRESH_BINARY);

					cv::resize(mat, mat, cv::Size(image_width, image_height));
					cv::threshold(mat, mat, 250, 255, CV_THRESH_BINARY);

					// put depth, width at the begining of the param values array
					/*
					param_values.insert(param_values.begin() + 2, (float)(object_width - 1) / 4.0f);
					param_values.insert(param_values.begin() + 3, (float)(object_height - 1) / 3.0f);
					*/

					// set filename
					QString filename = "results/" + fileInfoList[i].baseName() + "/" + QString("image_%1.png").arg(count, 6, 10, QChar('0'));

					cv::imwrite(filename.toUtf8().constData(), mat);

					// write all the param values to the file
					for (int pi = 0; pi < param_values.size(); ++pi) {
						if (pi > 0) {
							out << ",";
						}
						out << param_values[pi];
					}
					out << "\n";



					count++;
				}
			}
		}

		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}

void GLWidget3D::generateLedgeImages(int image_width, int image_height, bool grayscale) {
	QDir dir("..\\cga\\ledge\\");

	if (!QDir("results").exists()) QDir().mkdir("results");

	srand(0);
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	camera.xrot = 0.0f;
	camera.yrot = 0.0f;
	camera.zrot = 0.0f;
	camera.pos = glm::vec3(0, 0, 3);
	camera.updateMVPMatrix();

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		int count = 0;

		if (!QDir("results/" + fileInfoList[i].baseName()).exists()) QDir().mkdir("results/" + fileInfoList[i].baseName());

		QFile file("results/" + fileInfoList[i].baseName() + "/parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (float object_width = 3.0f; object_width <= 3.0f; object_width += 0.1f) {
			for (float object_height = 0.5f; object_height <= 0.5f; object_height += 0.1f) {
				for (int k = 0; k < 100; ++k) { // 1 images (parameter values are randomly selected) for each width and height
					std::vector<float> param_values;

					renderManager.removeObjects();

					// generate a window
					//cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), -3.141592f * 0.5f, glm::vec3(0, 1, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
					cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), 3.141592f * 0.5f, glm::vec3(0, 1, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
					system.stack.push_back(boost::shared_ptr<cga::Shape>(start));

					try {
						cga::Grammar grammar;
						cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
						param_values = system.randomParamValues(grammar);
						system.derive(grammar, true);
						std::vector<boost::shared_ptr<glutils::Face> > faces;
						system.generateGeometry(faces);
						renderManager.addFaces(faces);
						//renderManager.centerObjects();
					}
					catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}
					catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}


					// put a background plane
					/*
					std::vector<Vertex> vertices;
					glutils::drawQuad(100, 100, glm::vec4(1, 1, 1, 1), glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(0, 0, -10)), vertices);
					renderManager.addObject("background", "", vertices);
					*/

					// render a window
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glEnable(GL_DEPTH_TEST);
					glDisable(GL_TEXTURE_2D);

					glUniform1i(glGetUniformLocation(renderManager.program, "seed"), rand() % 100);

					// Model view projection行列をシェーダに渡す
					glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"), 1, GL_FALSE, &camera.mvpMatrix[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"), 1, GL_FALSE, &camera.mvMatrix[0][0]);

					// pass the light direction to the shader
					//glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
					glUniform3f(glGetUniformLocation(renderManager.program, "lightDir"), light_dir.x, light_dir.y, light_dir.z);

					drawScene(0);

					QImage img = this->grabFrameBuffer();
					cv::Mat source(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
					cv::Mat mat;
					EDLine(source, mat, grayscale);

					// 画像を縮小
					cv::resize(mat, mat, cv::Size(256, 256));
					cv::threshold(mat, mat, 250, 255, CV_THRESH_BINARY);

					cv::resize(mat, mat, cv::Size(image_width, image_height));
					cv::threshold(mat, mat, 250, 255, CV_THRESH_BINARY);

					// set filename
					QString filename = "results/" + fileInfoList[i].baseName() + "/" + QString("image_%1.png").arg(count, 6, 10, QChar('0'));

					cv::imwrite(filename.toUtf8().constData(), mat);

					// write all the param values to the file
					for (int pi = 0; pi < param_values.size(); ++pi) {
						if (pi > 0) {
							out << ",";
						}
						out << param_values[pi];
					}
					out << "\n";



					count++;
				}
			}
		}

		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}

void GLWidget3D::generateSimpleShapeImages(int image_width, int image_height, float scale) {
	QDir dir("..\\cga\\simple_shapes\\");

	if (!QDir("results").exists()) QDir().mkdir("results");

	srand(0);
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	int origWidth = width();
	int origHeight = height();
	resize(512, 512);
	resizeGL(512, 512);

	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		int count = 0;

		if (!QDir("results/" + fileInfoList[i].baseName()).exists()) QDir().mkdir("results/" + fileInfoList[i].baseName());

		QFile file("results/" + fileInfoList[i].baseName() + "/parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (int object_width = 4; object_width <= 16; object_width += 4) {
			for (int object_depth = 4; object_depth <= 16; object_depth += 4) {
				for (int pitch_angle = 25; pitch_angle <= 35; pitch_angle += 5) {
					for (int yaw_angle = -50; yaw_angle <= -40; yaw_angle += 5) {
						// change camera view direction
						camera.xrot = pitch_angle;//35.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
						camera.yrot = yaw_angle;//-45.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
						camera.zrot = 0.0f;
						camera.pos = glm::vec3(0, 0, 40.0f);
						camera.updateMVPMatrix();

						for (int offset_x = -8; offset_x <= 8; offset_x += 4) {
							for (int offset_y = -8; offset_y <= 8; offset_y += 4) {

								for (int k = 0; k < 5; ++k) { // 1 images (parameter values are randomly selected) for each width and height				
									std::vector<float> param_values;

									renderManager.removeObjects();

									// generate a window
									cga::Rectangle* start = new cga::Rectangle("Start", "", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(offset_x - (float)object_width*0.5f, offset_y -(float)object_depth*0.5f, 0)), glm::mat4(), object_width, object_depth, glm::vec3(1, 1, 1));
									system.stack.push_back(boost::shared_ptr<cga::Shape>(start));

									try {
										cga::Grammar grammar;
										cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
										param_values = system.randomParamValues(grammar);
										system.derive(grammar, true);
										std::vector<boost::shared_ptr<glutils::Face> > faces;
										system.generateGeometry(faces);
										renderManager.addFaces(faces);
										//renderManager.centerObjects();
									}
									catch (const std::string& ex) {
										std::cout << "ERROR:" << std::endl << ex << std::endl;
									}
									catch (const char* ex) {
										std::cout << "ERROR:" << std::endl << ex << std::endl;
									}

									// render a window
									glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
									glEnable(GL_DEPTH_TEST);
									glDisable(GL_TEXTURE_2D);

									glUniform1i(glGetUniformLocation(renderManager.program, "seed"), rand() % 100);

									// Model view projection行列をシェーダに渡す
									glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"), 1, GL_FALSE, &camera.mvpMatrix[0][0]);
									glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"), 1, GL_FALSE, &camera.mvMatrix[0][0]);

									// pass the light direction to the shader
									//glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
									glUniform3f(glGetUniformLocation(renderManager.program, "lightDir"), light_dir.x, light_dir.y, light_dir.z);

									drawScene(0);

									QImage img = this->grabFrameBuffer();
									cv::Mat source(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
									cv::Mat mat;
									EDLine(source, mat, scale);
									//QString filename = "results/" + fileInfoList[i].baseName() + "/" + QString("image_%1.png").arg(count, 4, 10, QChar('0'));
									//cv::imwrite(filename.toUtf8().constData(), result);

									// スケッチ線を使わず、直線で描画
									// 128x128、グレースケースの画像に変換
									/*
									QImage img = grabFrameBuffer();
									cv::Mat mat(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
									cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
									*/
									cv::resize(mat, mat, cv::Size(256, 256));
									cv::threshold(mat, mat, 250, 255, CV_THRESH_BINARY);
									cv::resize(mat, mat, cv::Size(128, 128));
									cv::threshold(mat, mat, 250, 255, CV_THRESH_BINARY);

									// 画像が有効なら保存
									if (isImageValid(mat)) {
										// put depth, width at the begining of the param values array
										param_values.insert(param_values.begin() + 0, (float)(offset_x + 8) / 16.0f);
										param_values.insert(param_values.begin() + 1, (float)(offset_y + 8) / 16.0f);
										param_values.insert(param_values.begin() + 2, (float)(object_width - 4) / 12.0f);
										param_values.insert(param_values.begin() + 3, (float)(object_depth - 4) / 12.0f);

										// set filename
										QString filename = "results/" + fileInfoList[i].baseName() + "/" + QString("image_%1.png").arg(count, 5, 10, QChar('0'));

										cv::imwrite(filename.toUtf8().constData(), mat);

										// write all the param values to the file
										for (int pi = 0; pi < param_values.size(); ++pi) {
											if (pi > 0) {
												out << ",";
											}
											out << param_values[pi];
										}
										out << "\n";

										count++;
									}

								}


							}
						}
					}
				}
			}
		}

		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}

void GLWidget3D::test() {
	/*
	this->resize(512, 512);
	resizeGL(512, 512);
	updateGL();

					cv::Mat result;
					EDLine(result, 0.5f);
					cv::imwrite("result.png", result);
					*/
}

/**
 * http://www.ceng.anadolu.edu.tr/CV/EDLines/
 */
void GLWidget3D::EDLine(const cv::Mat& source, cv::Mat& result, bool grayscale) {
	//QImage img = this->grabFrameBuffer();

	//cv::Mat mat(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());

	unsigned char* image = (unsigned char *)malloc(source.cols * source.rows);
	for (int r = 0; r < source.rows; ++r) {
		for (int c = 0; c < source.cols; ++c) {
			image[c + r*source.cols] = (source.at<cv::Vec4b>(r, c)[0] + source.at<cv::Vec4b>(r, c)[1] + source.at<cv::Vec4b>(r, c)[2]) / 3 < 240 ? 255 : 0;
		}
	}

	int noLines;
	LS *lines = DetectLinesByED(image, source.cols, source.rows, &noLines);
	free(image);

	// I use simple workaround for now.
	std::vector<std::pair<glm::vec2, glm::vec2> > edges(noLines);
	for (int i = 0; i < noLines; ++i) {
		edges[i] = std::make_pair(glm::vec2(lines[i].sx, lines[i].sy), glm::vec2(lines[i].ex, lines[i].ey));;
	}
	free(lines);
	bool erased;
	while (true) {
		erased = false;
		for (int i = 0; i < edges.size() && !erased; ++i) {
			for (int j = i + 1; j < edges.size() && !erased; ++j) {
				if (glm::length(edges[i].first - edges[j].first) < 10 && glm::length(edges[i].second - edges[j].second) < 10) {
					edges.erase(edges.begin() + j);
					erased = true;
				} else if (glm::length(edges[i].first - edges[j].second) < 10 && glm::length(edges[i].second - edges[j].first) < 10) {
					edges.erase(edges.begin() + j);
					erased = true;
				} else {
					if (fabs(glm::dot(glm::normalize(edges[i].first - edges[i].second), glm::normalize(edges[j].first - edges[j].second))) > 0.99) {
						glm::vec2 norm1(-(edges[i].first - edges[i].second).y, (edges[i].first - edges[i].second).x);
						glm::vec2 norm2(-(edges[j].first - edges[j].second).y, (edges[j].first - edges[j].second).x);
						norm1 = glm::normalize(norm1);
						norm2 = glm::normalize(norm2);
						if (fabs(glm::dot(norm1, edges[i].first) - glm::dot(norm2, edges[j].first)) < 3) {	// two lines are parallel and close!!
							if (fabs(edges[i].first.x - edges[i].second.x) > fabs(edges[i].first.y - edges[i].second.y)) {	// like horizontal line
								float x1s = std::min(edges[i].first.x, edges[i].second.x);
								float x1e = std::max(edges[i].first.x, edges[i].second.x);
								float x2s = std::min(edges[j].first.x, edges[j].second.x);
								float x2e = std::max(edges[j].first.x, edges[j].second.x);
								if (x2s >= x1s && x2s <= x1e && x2e >= x1s && x2e <= x1e) {
									edges.erase(edges.begin() + j);
									erased = true;
								} else if (x1s >= x2s && x1s <= x2e && x1e >= x2s && x1e <= x2e) {
									edges.erase(edges.begin() + i);
									erased = true;
								}
							} else {	// like vertical line
								float y1s = std::min(edges[i].first.y, edges[i].second.y);
								float y1e = std::max(edges[i].first.y, edges[i].second.y);
								float y2s = std::min(edges[j].first.y, edges[j].second.y);
								float y2e = std::max(edges[j].first.y, edges[j].second.y);
								if (y2s >= y1s && y2s <= y1e && y2e >= y1s && y2e <= y1e) {
									edges.erase(edges.begin() + j);
									erased = true;
								} else if (y1s >= y2s && y1s <= y2e && y1e >= y2s && y1e <= y2e) {
									edges.erase(edges.begin() + i);
									erased = true;
								}
							}
						}
					}
				}
			}
		}

		if (!erased) break;
	}

	if (grayscale) {
		result = cv::Mat(source.rows, source.cols, CV_8U, cv::Scalar(255));
	}
	else {
		result = cv::Mat(source.rows, source.cols, CV_8UC3, cv::Scalar(255, 255, 255));
	}

	for (int i = 0; i < edges.size(); ++i) {
		int polyline_index = rand() % style_polylines.size();

		draw2DPolyline(result, edges[i].first, edges[i].second, polyline_index);
	}
}

void GLWidget3D::draw2DPolyline(cv::Mat& img, const glm::vec2& p0, const glm::vec2& p1, int polyline_index) {
	float theta = atan2(p1.y - p0.y, p1.x - p0.x);
	float scale = glm::length(p1 - p0);

	cv::Mat_<float> R(2, 2);
	R(0, 0) = scale * cosf(theta);
	R(0, 1) = -scale * sinf(theta);
	R(1, 0) = scale * sinf(theta);
	R(1, 1) = scale * cosf(theta);

	cv::Mat_<float> A(2, 1);
	A(0, 0) = p0.x;
	A(1, 0) = p0.y;

	for (int i = 0; i < style_polylines[polyline_index].size() - 1; ++i) {
		cv::Mat_<float> X0(2, 1);
		X0(0, 0) = style_polylines[polyline_index][i].x;
		X0(1, 0) = style_polylines[polyline_index][i].y;
		cv::Mat_<float> T0 = R * X0 + A;

		cv::Mat_<float> X1(2, 1);
		X1(0, 0) = style_polylines[polyline_index][i+1].x;
		X1(1, 0) = style_polylines[polyline_index][i+1].y;
		cv::Mat_<float> T1 = R * X1 + A;

		cv::line(img, cv::Point(T0(0, 0), T0(1, 0)), cv::Point(T1(0, 0), T1(1, 0)), cv::Scalar(0), 1, CV_AA);
	}
}

bool GLWidget3D::isImageValid(const cv::Mat& image) {
	cv::Mat tmp;

	if (image.channels() == 1) {
		cv::reduce(image, tmp, 0, CV_REDUCE_MIN);
		if (tmp.at<uchar>(0, 0) == 0) return false;
		if (tmp.at<uchar>(0, tmp.cols - 1) == 0) return false;

		cv::reduce(image, tmp, 1, CV_REDUCE_MIN);
		if (tmp.at<uchar>(0, 0) == 0) return false;
		if (tmp.at<uchar>(tmp.rows - 1, 0) == 0) return false;

		// at least one pixel has to be black
		cv::reduce(tmp, tmp, 0, CV_REDUCE_MIN);
		if (tmp.at<uchar>(0, 0) == 0) return true;
		else return false;
	}
	else {
		cv::reduce(image, tmp, 0, CV_REDUCE_MIN);
		if (tmp.at<cv::Vec3b>(0, 0)[0] == 0) return false;
		if (tmp.at<cv::Vec3b>(0, tmp.cols - 1)[0] == 0) return false;

		cv::reduce(image, tmp, 1, CV_REDUCE_MIN);
		if (tmp.at<cv::Vec3b>(0, 0)[0] == 0) return false;
		if (tmp.at<cv::Vec3b>(tmp.rows - 1, 0)[0] == 0) return false;

		// at least one pixel has to be black
		cv::reduce(tmp, tmp, 0, CV_REDUCE_MIN);
		if (tmp.at<cv::Vec3b>(0, 0)[0] == 0) return true;
		else return false;
	}
}