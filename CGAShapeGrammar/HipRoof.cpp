#include "HipRoof.h"
#include <boost/shared_ptr.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_straight_skeleton_2.h>
#include "GLUtils.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K ;
typedef K::Point_2 KPoint;
typedef CGAL::Polygon_2<K> Polygon_2 ;
typedef CGAL::Straight_skeleton_2<K> Ss ;
typedef boost::shared_ptr<Ss> SsPtr ;

namespace cga {

HipRoof::HipRoof(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, const std::vector<glm::vec2>& points, float angle, const glm::vec3& color) {
	this->_name = name;
	this->_removed = false;
	this->_pivot = pivot;
	this->_modelMat = modelMat;
	this->_points = points;
	this->_angle = angle;
	this->_color = color;
}

Shape* HipRoof::clone(const std::string& name) {
	Shape* copy = new HipRoof(*this);
	copy->_name = name;
	return copy;
}

void HipRoof::generate(RenderManager* renderManager, bool showScopeCoordinateSystem) {
	std::vector<Vertex> vertices;

	Polygon_2 poly;
	for (int i = 0; i < _points.size(); ++i) {
		poly.push_back(KPoint(_points[i].x, _points[i].y));
	}

	// You can pass the polygon via an iterator pair
	SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly);

	for (auto face = iss->faces_begin(); face != iss->faces_end(); ++face) {
		// 各faceについて、ポリゴンを生成する
		auto edge0 = face->halfedge();
		auto edge = edge0;

		// 最初のエッジを保存する
		glm::vec2 p0, p1;
		bool first = true;

		glm::vec3 prev_p;

		do {
			auto head = edge->vertex();
			auto tail = edge->opposite()->vertex();

			if (edge->is_bisector()) {
				if (edge->is_inner_bisector()) { // 外側に接続されていない分割線
				} else { // 外側と接続されている分割線
				}
			} else { // 一番外側のボーダー
			}
			//cv::line(m, cv::Point(tail->point().x(), tail->point().y()), cv::Point(head->point().x(), head->point().y()), color, 1);

			if (first) {
				p0 = glm::vec2(tail->point().x(), tail->point().y());
				p1 = glm::vec2(head->point().x(), head->point().y());
				first = false;

				prev_p = glm::vec3(p1, 0);
			} else {
				glm::vec2 p2 = glm::vec2(head->point().x(), head->point().y());

				if (p2 != p0) {
					// p2の高さを計算
					float z = glutils::distance(p0, p1, p2) * tanf(_angle * M_PI / 180.0f);

					// 三角形を作成
					glm::vec3 v0 = glm::vec3(_pivot * _modelMat * glm::vec4(p0, 0, 1));
					glm::vec3 v1 = glm::vec3(_pivot * _modelMat * glm::vec4(prev_p, 1));
					glm::vec3 v2 = glm::vec3(_pivot * _modelMat * glm::vec4(p2, z, 1));

					glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

					vertices.push_back(Vertex(v0, normal, _color));
					vertices.push_back(Vertex(v1, normal, _color));
					vertices.push_back(Vertex(v2, normal, _color));
	
					prev_p = glm::vec3(p2, z);
				}
			}
		} while ((edge = edge->next()) != edge0);
	}

	renderManager->addObject(_name.c_str(), "", vertices);

	if (showScopeCoordinateSystem) {
		drawAxes(renderManager, _pivot * _modelMat);
	}
}

}
