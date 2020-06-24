#ifndef GTSP_HPP
#define GTSP_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <list>
#include <vector>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/io.h>

class GTSP
{

private:
	pcl::PointCloud<pcl::PointXYZ> voxel_path_cloud;
	int dimension;
	bool TSP;
public:
	GTSP(bool use_TSP);
	~GTSP();
	void setGTSPData(pcl::PointCloud<pcl::PointXYZ> in, int numClusters, std::vector<int> pointClusterMapping);
	void setPointCloud(pcl::PointCloud<pcl::PointXYZ> in);
	void computeTour();
	void setTSP(bool use_TSP);

	std::list<int> tour_order;
	std::vector<int> pointClusterMapping;
	int numClusters;
};

#endif