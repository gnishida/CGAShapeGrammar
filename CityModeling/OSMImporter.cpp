#include "OSMImporter.h"
#include <unordered_map>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <QFile>
#include <QDomDocument>

OSMImporter::OSMImporter() {
}

OSMImporter::~OSMImporter() {
}

void OSMImporter::import(const QString& filename, std::vector<std::vector<glm::vec2>>& roads, std::vector<BuildingParam>& buildingParams) {
	QDomDocument doc;
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file)) return;

	std::unordered_map<unsigned long long, glm::vec2> nodes;
	roads.clear();
	buildingParams.clear();

	double centerlat = 0;
	double centerlon = 0;

	QDomElement root = doc.documentElement();
	QDomElement element = root.firstChild().toElement();
	while (!element.isNull()) {
		if (element.tagName() == "bounds") {
			double minlat = element.attribute("minlat").toDouble();
			double maxlat = element.attribute("maxlat").toDouble();
			double minlon = element.attribute("minlon").toDouble();
			double maxlon = element.attribute("maxlon").toDouble();

			centerlat = (minlat + maxlat) * 0.5f;
			centerlon = (minlon + maxlon) * 0.5f;
		}
		else if (element.tagName() == "node") {
			unsigned long long id = element.attribute("id").toULongLong();
			double lon = element.attribute("lon").toDouble();
			double lat = element.attribute("lat").toDouble();
			nodes[id] = convertLatLonToUTM(lat, lon, centerlat, centerlon);
		}
		else if (element.tagName() == "way") {
			if (!element.hasAttribute("visible") || element.attribute("visible") == "true") {
				std::vector<glm::vec2> polygon;
				float height = 6.0f;
				QString type;

				QDomElement child_element = element.firstChild().toElement();
				while (!child_element.isNull()) {
					if (child_element.tagName() == "nd") {
						unsigned long long id = child_element.attribute("ref").toULongLong();
						polygon.push_back(nodes[id]);
					}
					else if (child_element.tagName() == "tag") {
						QString k = child_element.attribute("k");
						if (k == "building") type = "building";
						else if (k == "highway") type = "highway";
					}

					child_element = child_element.nextSiblingElement();
				}

				if (type == "building") {
					buildingParams.push_back({ polygon, height });
				}
				else if (type == "highway") {
					roads.push_back(polygon);
				}
			}
		}

		element = element.nextSiblingElement();
	}
}

glm::vec2 OSMImporter::convertLatLonToUTM(double lat, double lon, double center_lat, double center_lon) {
	const double radius = 6379000;
	double dlat = (lat - center_lat) / 180 * glm::pi<double>();
	double dlon = (lon - center_lon) / 180 * glm::pi<double>();

	double x = radius * std::cos(center_lon) * dlon;
	double y = radius * dlat;
	return{ x, y };
}