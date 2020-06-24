#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <gtsp/GTSPData.h>
#include <pcl_conversions/pcl_conversions.h>

int main(int argc, char **argv)
{
	ros::init(argc, argv, "gtsp_point_cloud_publisher");
	ros::NodeHandle nh;
	ros::Publisher point_cloud_publisher = nh.advertise<gtsp::GTSPData>("gtsp_data", 1);

	gtsp::GTSPData data;

	pcl::PointCloud<pcl::PointXYZ> cloud;
	pcl::PointXYZ point1(0,0,0);
	pcl::PointXYZ point2(2,3,4);
	pcl::PointXYZ point3(-2,-2,-2);
	cloud.push_back(point1);
	cloud.push_back(point2);
	cloud.push_back(point3);

	pcl::PCLPointCloud2 pcl_pc;
	pcl::toPCLPointCloud2(cloud, pcl_pc);
	sensor_msgs::PointCloud2 output;
    pcl_conversions::fromPCL(pcl_pc, output);
    
    std::vector<int> pointClusterMapping{1,2,1};
    data.points = output;
    data.numClusters = 2;
    data.pointClusterMapping = pointClusterMapping;
	ros::Rate loop_rate(0.2);
	while(ros::ok())
	{
		point_cloud_publisher.publish(data);
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}