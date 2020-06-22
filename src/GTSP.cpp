#include <GTSP.h>

GTSP::GTSP(bool use_TSP)
{
	this->TSP = use_TSP;
}

GTSP::~GTSP()
{
	
}

void GTSP::setPointCloud(pcl::PointCloud<pcl::PointXYZ> in)
{
	pcl::copyPointCloud(in, this->voxel_path_cloud);
	this->dimension = this->voxel_path_cloud.size();
}

void GTSP::computeTour()
{
	std::ofstream outfile("/tmp/temp.gtsp");
	outfile << "NAME: Temp" << std::endl;
	outfile << "TYPE: AGTSP" << std::endl;
	outfile << "COMMENT: temporary GTSP file" << std::endl;
	outfile << "DIMENSION: " << this->dimension << std::endl;
	if(this->TSP)
		outfile << "GTSP_SETS: " << this->dimension << std::endl;
	outfile << "EDGE_WEIGHT_TYPE: EXPLICIT" << std::endl;
	outfile << "EDGE_WEIGHT_FORMAT: FULL_MATRIX" << std::endl;
	outfile << "EDGE_WEIGHT_SECTION" << std::endl;
	for(int i = 0; i < dimension; i++)
	{
		for(int j = 0; j < dimension; j++)
		{
			if(i == j)
				outfile << "999999 ";
			else {
				double edge_cost = ((voxel_path_cloud.points[i].x - voxel_path_cloud.points[j].x)*(voxel_path_cloud.points[i].x - voxel_path_cloud.points[j].x)) + 
				                   ((voxel_path_cloud.points[i].y - voxel_path_cloud.points[j].y)*(voxel_path_cloud.points[i].y - voxel_path_cloud.points[j].y)) +
				                   ((voxel_path_cloud.points[i].z - voxel_path_cloud.points[j].z)*(voxel_path_cloud.points[i].z - voxel_path_cloud.points[j].z));
				edge_cost = sqrt(edge_cost);
				int rounded_edge_cost = (edge_cost * 100);
				outfile << rounded_edge_cost << " ";
			}
		}
		outfile << std::endl;
	}
	outfile << "GTSP_SET_SECTION:" << std::endl;
	if(this->TSP) {
		for(int i = 0; i < dimension; i++)
		{
			outfile << (i+1) << " " << (i+1) << " -1" << std::endl;
		}
	}
	outfile << "EOF" << std::endl;
	outfile.close();
	int solver_output = system("/home/naik/catkin_ws/src/gtsp/src/GLNScmd.jl /tmp/temp.gtsp -mode=fast -output=/tmp/tour.txt");

	std::ifstream infile("/tmp/tour.txt");
	std::string line;
	std::string delimiter = ":";
	std::string token;
	size_t pos = 0;
	std::string tour;
	while(std::getline(infile, line))
	{
		while((pos = line.find(delimiter)) != std::string::npos)
		{
			token = line.substr(0,pos);
			token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
			line.erase(0, pos + delimiter.length());
		}
		if(token == "Tour")
			tour = line;
		pos = 0;
	}
	tour.erase(std::remove_if(tour.begin(), tour.end(), ::isspace), tour.end());
	tour.erase(std::remove(tour.begin(), tour.end(), '['), tour.end());
	tour.erase(std::remove(tour.begin(), tour.end(), ']'), tour.end());
	this->tour_order.clear();

	std::stringstream s_stream(tour);
	while(s_stream.good())
	{
		std::string substr;
		std::getline(s_stream, substr, ',');
		this->tour_order.push_back(std::stoi(substr));
	}
}