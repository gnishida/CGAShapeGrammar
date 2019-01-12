#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

namespace utils {
	
	bool isNumber(const std::string& str);
	void computeMean(const std::vector<std::vector<float>>& values, std::vector<float>& mean);
	void computeVariance(const std::vector<std::vector<float>>& values, const std::vector<float>& mean, std::vector<float>& var);
	float genRand();
	float genRand(float v);
	float genRand(float a, float b);
	int genIntRand(int v);
	int genIntRand(int a, int b);
	bool segmentSegmentIntersect(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d, float *tab, float *tcd, bool segmentOnly, glm::vec2& intPoint);

	glm::vec2 projectPoint(int screen_width, int screen_height, const glm::vec3& p, const glm::mat4& mvpMatrix);

	void extractEdges(const cv::Mat& img, std::vector<std::pair<glm::vec2, glm::vec2>>& edges);
	void cleanEdges(std::vector<std::pair<glm::vec2, glm::vec2>>& edges, float maxLineGap, float theta);
	void cleanContours(std::vector<std::pair<glm::vec2, glm::vec2>>& edges, float maxLineGap, float theta);

	void resizeImage(cv::Mat& img, const cv::Size& size);
	void blueImage(cv::Mat& img);
	void blendImages(cv::Mat& img1, const cv::Mat& img2, const cv::Scalar& transparent_color);
	void translateImage(cv::Mat& img, int offset_x, int offset_y);

	double deg2rad(double deg);
	double rad2deg(double rad);
}
