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

void OSMImporter::import(const QString& filename, RoadGraph& roads, std::vector<BuildingParam>& buildingParams) {
	QDomDocument doc;
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file)) return;

	std::unordered_map<unsigned long long, glm::vec2> nodes;
	std::unordered_map<unsigned long long, int> convId;	// convert from OSM ID to our internal id of road vertices
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
			glm::vec2 pos = convertLatLonToUTM(lat, lon, centerlat, centerlon);
			nodes[id] = pos;
			convId[id] = roads.vertices.size();
			roads.vertices.emplace_back(std::make_shared<RoadVertex>(pos));
			roads.edges.resize(roads.vertices.size());
		}
		else if (element.tagName() == "way") {
			if (!element.hasAttribute("visible") || element.attribute("visible") == "true") {
				std::vector<int> nodeList;
				std::vector<glm::vec2> footprint;
				float height = 6.0f;
				QString type;

				QDomElement child_element = element.firstChild().toElement();
				while (!child_element.isNull()) {
					if (child_element.tagName() == "nd") {
						unsigned long long id = child_element.attribute("ref").toULongLong();
						footprint.push_back(nodes[id]);
						nodeList.push_back(convId[id]);
					}
					else if (child_element.tagName() == "tag") {
						QString k = child_element.attribute("k");
						if (k == "building") type = "building";
						else if (k == "highway") type = "highway";
					}

					child_element = child_element.nextSiblingElement();
				}

				if (type == "building") {
					if (footprint.size() >= 3) {
						// if the polygon is closed, remove the last point
						if (footprint.front() == footprint.back()) footprint.pop_back();

						buildingParams.emplace_back(footprint, height);
					}
				}
				else if (type == "highway") {
					if (nodeList.size() >= 2) {
						for (int i = 0; i < nodeList.size() - 1; i++) {
							int node1 = nodeList[i];
							int node2 = nodeList[i + 1];
							std::vector<glm::vec2> polyline = { roads.vertices[node1]->pos, roads.vertices[node2]->pos };
							RoadEdgePtr edge = std::make_shared<RoadEdge>(polyline);
							roads.edges[node1].emplace_back(node2, edge);
						}
					}
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