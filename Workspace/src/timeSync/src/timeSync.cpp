#include "ros/ros.h"
#include "timeSync.h"
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/Image.h>

using namespace sensor_msgs;
using namespace message_filters;

ros::Publisher left_pub;
ros::Publisher right_pub;

void callback(const ImageConstPtr& left, const ImageConstPtr& right)
{
  left_pub.publish(left);
  right_pub.publish(right);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "timeSync");

  ros::NodeHandle nh;

  left_pub = nh.advertise<sensor_msgs::Image>("/stereo/left/image_raw", 1);
  right_pub = nh.advertise<sensor_msgs::Image>("/stereo/right/image_raw", 1);
  message_filters::Subscriber<Image> left_sub(nh, "/no_sync/left/image_raw", 1);
  message_filters::Subscriber<Image> right_sub(nh, "/no_sync/right/image_raw", 1);

  typedef sync_policies::ApproximateTime<Image, Image> MySyncPolicy;
  // ApproximateTime takes a queue size as its constructor argument, hence MySyncPolicy(10)
  Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), left_sub, right_sub);
  sync.registerCallback(boost::bind(&callback, _1, _2));

  ros::spin();

  return 0;
}