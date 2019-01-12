#pragma once

#include <QString>
#include <BuildingParam.h>

class OSMImporter {
public:
	OSMImporter();
	~OSMImporter();

	static void import(const QString& filename, std::vector<std::vector<glm::vec2>>& roads, std::vector<BuildingParam>& buildingParams);
	static glm::vec2 convertLatLonToUTM(double lat, double lon, double center_lat, double center_lon);
};

