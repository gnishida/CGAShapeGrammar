#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

namespace procedural_generation {

	class BoundingBox {
	public:
		BoundingBox();
		BoundingBox(const std::vector<glm::vec2>& points) {
			_minPt.x = std::numeric_limits<float>::max();
			_minPt.y = std::numeric_limits<float>::max();
			_maxPt.x = -std::numeric_limits<float>::max();
			_maxPt.y = -std::numeric_limits<float>::max();
			for (const auto& p : points) {
				_minPt.x = std::min(_minPt.x, p.x);
				_minPt.y = std::min(_minPt.y, p.y);
				_maxPt.x = std::max(_maxPt.x, p.x);
				_maxPt.y = std::max(_maxPt.y, p.y);
			}
		}
		float minX() { return _minPt.x; }
		float minY() { return _minPt.y; }
		float maxX() { return _maxPt.x; }
		float maxY() { return _maxPt.y; }
		void addPoint(const glm::vec2& point);
		float width() { return _maxPt.x - _minPt.x; }
		float height() { return _maxPt.y - _minPt.y; }
		float centerX() { return (_minPt.x + _maxPt.x) * 0.5f; }
		float centerY() { return (_minPt.y + _maxPt.y) * 0.5f; }
		glm::vec2 center() { return (_maxPt + _minPt) * 0.5f; }
		float area() { return width() * height(); }
		bool within(const glm::vec2& pt) {
			if (pt.x >= _minPt.x && pt.x <= _maxPt.x && pt.y >= _minPt.y && pt.y <= _maxPt.y) return true;
			else return false;
		}
		bool Intersect(const BoundingBox& bbox) {
			if (_minPt.x > bbox._maxPt.x) return false;
			if (_maxPt.x < bbox._minPt.x) return false;
			if (_minPt.y > bbox._maxPt.y) return false;
			if (_maxPt.y < bbox._minPt.y) return false;
			return true;
		}

	private:
		glm::vec2 _minPt;
		glm::vec2 _maxPt;
	};

}