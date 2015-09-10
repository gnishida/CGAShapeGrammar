#include "GLWidget3D.h"
#include "MainWindow.h"
#include "OBJLoader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "RuleParser.h"
#include <map>
#include "Rectangle.h"
#include "Polygon.h"
#include "GLUtils.h"

GLWidget3D::GLWidget3D(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers)) {
	// 光源位置をセット
	// ShadowMappingは平行光源を使っている。この位置から原点方向を平行光源の方向とする。
	light_dir = glm::normalize(glm::vec3(-4, -5, -8));

	// シャドウマップ用のmodel/view/projection行列を作成
	glm::mat4 light_pMatrix = glm::ortho<float>(-50, 50, -50, 50, 0.1, 200);
	glm::mat4 light_mvMatrix = glm::lookAt(-light_dir * 50.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	light_mvpMatrix = light_pMatrix * light_mvMatrix;
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
	renderManager.init("../shaders/vertex.glsl", "../shaders/geometry.glsl", "../shaders/fragment.glsl", 8192);
	showWireframe = true;
	showScopeCoordinateSystem = true;

	// set the clear color for the screen
	qglClearColor(QColor(113, 112, 117));

	system.modelMat = glm::rotate(glm::mat4(), -3.1415926f * 0.5f, glm::vec3(1, 0, 0));

	std::vector<Vertex> vertices;
	glutils::drawGrid(30, 30, 1, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::rotate(glm::mat4(), -3.1415926f * 0.5f, glm::vec3(1, 0, 0)), vertices);
	renderManager.addObject("grid", "", vertices);
}

/**
 * This function is called whenever the widget has been resized.
 */
void GLWidget3D::resizeGL(int width, int height) {
	height = height ? height : 1;
	glViewport(0, 0, width, height);
	camera.updatePMatrix(width, height);
}

/**
 * This function is called whenever the widget needs to be painted.
 */
void GLWidget3D::paintGL() {
	renderManager.updateShadowMap(this, light_dir, light_mvpMatrix);

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
		glUniform1i(glGetUniformLocation(renderManager.program, "shadowState"), 1);
	} else {
		glUniform1i(glGetUniformLocation(renderManager.program, "shadowState"), 2);
	}
	
	if (showScopeCoordinateSystem) {
		renderManager.renderAll(showWireframe);
	} else {
		renderManager.renderAllExcept("axis", showWireframe);
	}
}

void GLWidget3D::loadCGA(char* filename) {
	renderManager.removeObjects();

	std::list<cga::Shape*> stack;
	/*{
		cga::Rectangle* lot = new cga::Rectangle("Lot", glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::mat4(), 5, 5, glm::vec3(1, 1, 1));
		stack.push_back(lot);
	}*/

	{
		std::vector<glm::vec2> points;
		points.push_back(glm::vec2(0, 0));
		points.push_back(glm::vec2(2, 0));
		points.push_back(glm::vec2(2, 2));
		points.push_back(glm::vec2(5, 2));
		points.push_back(glm::vec2(5, 5));
		points.push_back(glm::vec2(0, 5));
		cga::Polygon* lot = new cga::Polygon("Lot", glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::mat4(), points, glm::vec3(1, 1, 1), "");
		stack.push_back(lot);
	}

	try {
		std::map<std::string, cga::Rule> rules = cga::parseRule(filename);
		system.generate(&renderManager, rules, stack, true);
	} catch (const char* ex) {
		std::cout << "ERROR:" << std::endl << ex << std::endl;
		return;
	}
	
	std::vector<Vertex> vertices;
	glutils::drawGrid(30, 30, 1, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::rotate(glm::mat4(), -3.1415926f * 0.5f, glm::vec3(1, 0, 0)), vertices);
	renderManager.addObject("grid", "", vertices);

	updateGL();
}
