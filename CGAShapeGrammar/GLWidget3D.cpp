#include "GLWidget3D.h"
#include "MainWindow.h"
#include "OBJLoader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "GrammarParser.h"
#include <map>
#include "Rectangle.h"
#include "Polygon.h"
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
	glm::mat4 light_pMatrix = glm::ortho<float>(-100, 100, -100, 100, 0.1, 200);
	glm::mat4 light_mvMatrix = glm::lookAt(-light_dir * 50.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	light_mvpMatrix = light_pMatrix * light_mvMatrix;

	// initialize stylized polylines
	style_polylines.resize(5);
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
	renderManager.init("../shaders/vertex.glsl", "../shaders/geometry.glsl", "../shaders/fragment.glsl", false);

	// set the clear color for the screen
	qglClearColor(QColor(113, 112, 117));

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

	/*
	std::vector<Vertex> vertices;
	glutils::drawGrid(60, 60, 1, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::rotate(glm::mat4(), -3.1415926f * 0.5f, glm::vec3(1, 0, 0)), vertices);
	renderManager.addObject("grid", "", vertices);
	*/

	/*{ // for tutorial
		cga::Rectangle* lot = new cga::Rectangle("Lot", glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::mat4(), 35, 15, glm::vec3(1, 1, 1));
		system.stack.push_back(lot);
	}*/

	float object_width = 18.0f;
	float object_height = 10.0f;

	{ // for parthenon
		cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}

	/*{ // This is for test.
		cga::Rectangle* lot = new cga::Rectangle("Lot", glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::mat4(), 5, 5, glm::vec3(1, 1, 1));
		system.stack.push_back(lot);
	}*/

	/*{
		std::vector<glm::vec2> points;
		points.push_back(glm::vec2(0, 0));
		points.push_back(glm::vec2(2, 0));
		points.push_back(glm::vec2(2, 2));
		points.push_back(glm::vec2(5, 2));
		points.push_back(glm::vec2(5, 5));
		points.push_back(glm::vec2(0, 5));
		cga::Polygon* lot = new cga::Polygon("Lot", glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::mat4(), points, glm::vec3(1, 1, 1), "");
		system.stack.push_back(lot);
	}*/

	try {
		cga::Grammar grammar;
		cga::parseGrammar(filename, grammar);
		system.randomParamValues(grammar);
		system.derive(grammar);
		system.generateGeometry(&renderManager);
		renderManager.centerObjects();
	} catch (const std::string& ex) {
		std::cout << "ERROR:" << std::endl << ex << std::endl;
	} catch (const char* ex) {
		std::cout << "ERROR:" << std::endl << ex << std::endl;
	}
	
	renderManager.updateShadowMap(this, light_dir, light_mvpMatrix);

	updateGL();
}

void GLWidget3D::generateImages(int image_width, int image_height, bool invertImage, bool blur) {
	QDir dir("..\\cga\\windows\\");
	//QDir dir("..\\cga\\windows_low_LOD\\");

	if (!QDir("results").exists()) QDir().mkdir("results");

	srand(0);
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	camera.xrot = 90.0f;
	camera.yrot = 0.0f;
	camera.zrot = 0.0f;
	camera.pos = glm::vec3(0, 0, 2.5f);
	camera.updateMVPMatrix();

	int origWidth = width();
	int origHeight = height();
	resize(image_width, image_height);
	resizeGL(image_width, image_height);

	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		int count = 0;
	
		if (!QDir("results/" + fileInfoList[i].baseName()).exists()) QDir().mkdir("results/" + fileInfoList[i].baseName());

		QFile file("results/" + fileInfoList[i].baseName() + "/parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (float object_width = 1.0f; object_width <= 2.6f; object_width += 0.05f) {
			for (float object_height = 1.0f; object_height <= 1.8f; object_height += 0.05f) {
				for (int k = 0; k < 2; ++k) { // 1 images (parameter values are randomly selected) for each width and height
					std::vector<float> param_values;


					renderManager.removeObjects();

					// generate a window
					cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
					system.stack.push_back(boost::shared_ptr<cga::Shape>(start));

					try {
						cga::Grammar grammar;
						cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
						param_values = system.randomParamValues(grammar);
						system.derive(grammar, true);
						system.generateGeometry(&renderManager);
						renderManager.centerObjects();
					} catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					} catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}

					// put width/height at the begining of the param values array
					param_values.insert(param_values.begin(), object_width / object_height);

					// write all the param values to the file
					for (int pi = 0; pi < param_values.size(); ++pi) {
						if (pi > 0) {
							out << ",";
						}
						out << param_values[pi];
					}
					out << "\n";

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
					glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"),  1, GL_FALSE, &camera.mvpMatrix[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"),  1, GL_FALSE, &camera.mvMatrix[0][0]);

					// pass the light direction to the shader
					//glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
					glUniform3f(glGetUniformLocation(renderManager.program, "lightDir"), light_dir.x, light_dir.y, light_dir.z);
	
					drawScene(0);

					cv::Mat result;
					EDLine(result, 0.5f);
					QString filename = "results/" + fileInfoList[i].baseName() + "/" + QString("image_%1.png").arg(count, 4, 10, QChar('0'));
					cv::imwrite(filename.toUtf8().constData(), result);

					count++;
				}
			}
		}

		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}

void GLWidget3D::generateBuildingImages(int image_width, int image_height, bool invertImage, bool blur) {
	QDir dir("..\\cga\\building\\");
	//QDir dir("..\\cga\\building_low_LOD\\");

	if (!QDir("results").exists()) QDir().mkdir("results");

	srand(0);
	renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;

	int origWidth = width();
	int origHeight = height();
	resize(image_width, image_height);
	resizeGL(image_width, image_height);

	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		int count = 0;
	
		if (!QDir("results/" + fileInfoList[i].baseName()).exists()) QDir().mkdir("results/" + fileInfoList[i].baseName());

		QFile file("results/" + fileInfoList[i].baseName() + "/parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (float object_width = 14.0f; object_width <= 22.0f; object_width += 1.0f) {
			for (float object_height = 8.0f; object_height <= 16.0f; object_height += 1.0f) {
				for (int k = 0; k < 16; ++k) { // 1 images (parameter values are randomly selected) for each width and height
					// change camera view direction
					camera.xrot = 35.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
					camera.yrot = -45.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
					camera.zrot = 0.0f;
					camera.pos = glm::vec3(0, 0, 2.3f);
					camera.updateMVPMatrix();
					
					std::vector<float> param_values;
					
					renderManager.removeObjects();

					// generate a window
					cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
					system.stack.push_back(boost::shared_ptr<cga::Shape>(start));

					try {
						cga::Grammar grammar;
						cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
						param_values = system.randomParamValues(grammar);
						system.derive(grammar, true);
						system.generateGeometry(&renderManager);
						renderManager.centerObjects();
					} catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					} catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}

					// put width/height at the begining of the param values array
					param_values.insert(param_values.begin(), object_width / object_height);

					// write all the param values to the file
					for (int pi = 0; pi < param_values.size(); ++pi) {
						if (pi > 0) {
							out << ",";
						}
						out << param_values[pi];
					}
					out << "\n";

					// render a window
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glEnable(GL_DEPTH_TEST);
					glDisable(GL_TEXTURE_2D);

					glUniform1i(glGetUniformLocation(renderManager.program, "seed"), rand() % 100);

					// Model view projection行列をシェーダに渡す
					glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"),  1, GL_FALSE, &camera.mvpMatrix[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"),  1, GL_FALSE, &camera.mvMatrix[0][0]);

					// pass the light direction to the shader
					//glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
					glUniform3f(glGetUniformLocation(renderManager.program, "lightDir"), light_dir.x, light_dir.y, light_dir.z);
	
					drawScene(0);

					cv::Mat result;
					EDLine(result, 0.5f);
					QString filename = "results/" + fileInfoList[i].baseName() + "/" + QString("image_%1.png").arg(count, 4, 10, QChar('0'));
					cv::imwrite(filename.toUtf8().constData(), result);

					count++;
				}
			}
		}

		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}

void GLWidget3D::hoge() {
	this->resize(256, 256);
	resizeGL(256, 256);
	updateGL();
}

/**
 * http://www.ceng.anadolu.edu.tr/CV/EDLines/
 */
void GLWidget3D::EDLine(cv::Mat& result, float scale) {
	QImage img = this->grabFrameBuffer();

	cv::Mat mat(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());

	unsigned char* image = (unsigned char *)malloc(mat.cols * mat.rows);
	for (int r = 0; r < mat.rows; ++r) {
		for (int c = 0; c < mat.cols; ++c) {
			image[c+r*mat.cols] = (mat.at<cv::Vec4b>(r, c)[0] + mat.at<cv::Vec4b>(r, c)[1] + mat.at<cv::Vec4b>(r, c)[2]) / 3 < 240 ? 255: 0;
		}
	}

	int noLines;
	LS *lines = DetectLinesByED(image, mat.cols, mat.rows, &noLines);

	result = cv::Mat(mat.rows * scale, mat.cols * scale, CV_8UC3, cv::Scalar(255, 255, 255));

	for (int i = 0; i < noLines; ++i) {
		int polyline_index = rand() % style_polylines.size();

		draw2DPolyline(result, glm::vec2(lines[i].sx * scale, lines[i].sy * scale), glm::vec2(lines[i].ex * scale, lines[i].ey * scale), polyline_index);
	}

	free(image);
	free(lines);
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

		cv::line(img, cv::Point(T0(0, 0), T0(1, 0)), cv::Point(T1(0, 0), T1(1, 0)), cv::Scalar(0, 0, 0), 1, CV_AA);
	}
}