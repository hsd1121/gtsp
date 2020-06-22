#include <ros/ros.h>
#include <GTSP.h>
#include <sensor_msgs/PointCloud2.h>
#include <gtsp/Tour.h>
#include <pcl_conversions/pcl_conversions.h>
#include <vector>

class GTSPROSWrapper
{
private:
	std::unique_ptr<GTSP> gtsp_;
	ros::Subscriber voxel_point_cloud_subscriber_;
	ros::Publisher tour_publisher_;
	int publish_count;
public:
	GTSPROSWrapper(ros::NodeHandle *nh)
	{
		this->gtsp_ = std::unique_ptr<GTSP>(new GTSP(true));
		this->voxel_point_cloud_subscriber_ = nh->subscribe("gtsp_point_cloud", 10, &GTSPROSWrapper::callbackPointCloud, this);
        this->tour_publisher_ = nh->advertise<gtsp::Tour>("gtsp_tour_list", 1);
        this->publish_count = 0;
	}

	void callbackPointCloud(const sensor_msgs::PointCloud2ConstPtr& input)
	{
		pcl::PCLPointCloud2 pcl_pc2;
	    pcl_conversions::toPCL(*input, pcl_pc2);
	   	pcl::PointCloud<pcl::PointXYZ>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	    pcl::fromPCLPointCloud2(pcl_pc2, *temp_cloud);
    	this->gtsp_->setPointCloud(*temp_cloud);
    	this->gtsp_->computeTour();

    	gtsp::Tour msg;
    	std::vector<int> tour;
    	for (std::list<int>::const_iterator iterator = this->gtsp_->tour_order.begin(), end = this->gtsp_->tour_order.end(); iterator != end; ++iterator) {
    		tour.push_back(*iterator);
		}
    	msg.tour = tour;
    	msg.header.seq = publish_count;
    	msg.header.stamp = ros::Time::now();
    	this->tour_publisher_.publish(msg);
    	publish_count++;
	}
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "gtsp_solver");
    ros::NodeHandle nh;

    GTSPROSWrapper gtsp_wrapper(&nh);
    ROS_INFO("GTSP wrapper is now started");

    ros::spin();
}