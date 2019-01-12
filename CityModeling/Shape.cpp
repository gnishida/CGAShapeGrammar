#include "Shape.h"

namespace procedural_generation {

	void DecodeSplitSizes(float size, const StringValuePairVec& nameAndSizes, std::vector<std::pair<std::string, float>>& decodedNameAndSizes) {
		float regular_sum = 0.0f;
		float floating_sum = 0.0f;
		float floating_scale = 0.0f;
		float repeat_unit = 0.0f;
		int repeat_num = 0;
		float repeat_scale = 0.0f;

		for (int i = 0; i < nameAndSizes.size(); ++i) {
			if (nameAndSizes[i].second.repeat) {
				if (nameAndSizes[i].second.type == Value::TYPE_ABSOLUTE) {
					repeat_unit += nameAndSizes[i].second.value;
				}
				else if (nameAndSizes[i].second.type == Value::TYPE_RELATIVE) {
					repeat_unit += nameAndSizes[i].second.value * size;
				}
				else if (nameAndSizes[i].second.type == Value::TYPE_FLOATING) {
					repeat_unit += nameAndSizes[i].second.value;
				}
			}
			else {
				if (nameAndSizes[i].second.type == Value::TYPE_ABSOLUTE) {
					regular_sum += nameAndSizes[i].second.value;
				}
				else if (nameAndSizes[i].second.type == Value::TYPE_RELATIVE) {
					regular_sum += nameAndSizes[i].second.value * size;
				}
				else if (nameAndSizes[i].second.type == Value::TYPE_FLOATING) {
					floating_sum += nameAndSizes[i].second.value;
				}
			}
		}

		// calculate how many times the sections with repeat flag be repeated
		if (repeat_unit > 0) {
			repeat_num = std::max(0.0f, (size - regular_sum) / repeat_unit + 0.5f);
			if (repeat_num > 0) {
				repeat_scale = std::max(0.0f, (size - regular_sum) / (float)repeat_num / repeat_unit);

				// if the scale for the repeating sections is too small, remove all the repeating sections
				if (repeat_scale < 0.5f) {
					repeat_num = 0;
				}
				else {
					// remove all the floating sections
					floating_sum = 0;
				}
			}
		}

		// calculate the scale for the floaing sections
		if (floating_sum > 0) {
			floating_scale = std::max(0.0f, size - regular_sum) / floating_sum;
		}

		if (regular_sum <= size && (floating_sum > 0 || repeat_num > 0)) {
			for (int i = 0; i < nameAndSizes.size(); ++i) {
				if (nameAndSizes[i].second.repeat) {
					float s = 0;
					if (nameAndSizes[i].second.type == Value::TYPE_ABSOLUTE) {
						s = nameAndSizes[i].second.value;
					}
					else if (nameAndSizes[i].second.type == Value::TYPE_RELATIVE) {
						s = nameAndSizes[i].second.value * size;
					}
					else if (nameAndSizes[i].second.type == Value::TYPE_FLOATING) {
						s = nameAndSizes[i].second.value;
					}
					s *= repeat_scale;
					for (int k = 0; k < repeat_num; ++k) {
						decodedNameAndSizes.push_back({ nameAndSizes[i].first, s });
					}
				}
				else {
					if (nameAndSizes[i].second.type == Value::TYPE_ABSOLUTE) {
						decodedNameAndSizes.push_back({ nameAndSizes[i].first, nameAndSizes[i].second.value });
					}
					else if (nameAndSizes[i].second.type == Value::TYPE_RELATIVE) {
						decodedNameAndSizes.push_back({ nameAndSizes[i].first, nameAndSizes[i].second.value * size });
					}
					else if (nameAndSizes[i].second.type == Value::TYPE_FLOATING) {
						decodedNameAndSizes.push_back({ nameAndSizes[i].first, nameAndSizes[i].second.value * floating_scale });
					}
				}
			}
		}
		else {
			// scale the regular sections to fit to the size
			float scale = size / regular_sum;
			for (int i = 0; i < nameAndSizes.size(); ++i) {
				if (!nameAndSizes[i].second.repeat) {
					if (nameAndSizes[i].second.type == Value::TYPE_ABSOLUTE) {
						decodedNameAndSizes.push_back({ nameAndSizes[i].first, nameAndSizes[i].second.value * scale });
					}
					else if (nameAndSizes[i].second.type == Value::TYPE_RELATIVE) {
						decodedNameAndSizes.push_back({ nameAndSizes[i].first, nameAndSizes[i].second.value * size * scale/*  */ });
					}
				}
			}
		}
	}

	void Shape::rotate(float xAngle, float yAngle, float zAngle) {
		_modelMat = glm::rotate(_modelMat, xAngle * glm::pi<float>() / 180.0f, glm::vec3(1, 0, 0));
		_modelMat = glm::rotate(_modelMat, yAngle * glm::pi<float>() / 180.0f, glm::vec3(0, 1, 0));
		_modelMat = glm::rotate(_modelMat, zAngle * glm::pi<float>() / 180.0f, glm::vec3(0, 0, 1));
	}

	void Shape::translate(TranslateMode translateMode, CoordinateSystem coordSystem, float x, float y, float z) {
		if (translateMode == TranslateMode::ABSOL) {
			if (coordSystem == CoordinateSystem::WORLD) {
				_modelMat[3].x = x;
				_modelMat[3].y = y;
				_modelMat[3].z = z;
			}
			else if (coordSystem == CoordinateSystem::OBJECT) {
				_modelMat = glm::translate(_modelMat, glm::vec3(x, y, z));
			}
		}
		else if (translateMode == TranslateMode::RELAT) {
			if (coordSystem == CoordinateSystem::WORLD) {
				_modelMat[3].x += x;
				_modelMat[3].y += y;
				_modelMat[3].z += z;
			}
			else if (coordSystem == CoordinateSystem::OBJECT) {
				_modelMat = glm::translate(_modelMat, glm::vec3(x, y, z));
			}
		}
	}

}
