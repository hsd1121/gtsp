#ifndef GTSP_HPP
#define GTSP_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <list>

#include <math.h>
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
	void setPointCloud(pcl::PointCloud<pcl::PointXYZ> in);
	void computeTour();

	std::list<int> tour_order;
};

#endif