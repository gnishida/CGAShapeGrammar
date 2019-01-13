#pragma once

#include "RoadGraph.h"
#include <QString>
#include <BuildingParam.h>

class OSMImporter {
public:
	OSMImporter();
	~OSMImporter();

	static void import(const QString& filename, RoadGraph& roads, std::vector<BuildingParam>& buildingParams);
	static glm::vec2 convertLatLonToUTM(double lat, double lon, double center_lat, double center_lon);
};

