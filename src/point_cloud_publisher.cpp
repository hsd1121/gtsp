#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>

int main(int argc, char **argv)
{
	ros::init(argc, argv, "gtsp_point_cloud_publisher");
	ros::NodeHandle nh;
	ros::Publisher point_cloud_publisher = nh.advertise<sensor_msgs::PointCloud2>("gtsp_point_cloud", 1);

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
    
	ros::Rate loop_rate(0.2);
	while(ros::ok())
	{
		point_cloud_publisher.publish(output);
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}