
import roslib
import geometry_msgs.msg
from geometry_msgs.msg import Quaternion
import rospy
import tf
#import tf2_py
import math
from tf.transformations import euler_from_quaternion
# import tf things


if __name__ == '__main__':
    rospy.init_node('tf_oculus2quad')
    listener = tf.TransformListener()
    oculus_pub = rospy.Publisher('/get_quat',geometry_msgs.msg.Quaternion)
    rospy.sleep(3.0) # This sleep is needed because of a bug of tf
    rate = rospy.Rate(10.0)
    listener.waitForTransform("/oculus", "/map", rospy.Time(), rospy.Duration(4.0))
    while not rospy.is_shutdown():
        #now = rospy.Time().now()
        listener.waitForTransform("/oculus", "/map", rospy.Time(), rospy.Duration(4.0))
        try:
            (trans,rot) = listener.lookupTransform('/oculus', '/map', rospy.Time())
        except (tf.LookupException, tf.ConnectivityException):
            continue 
        curr_quat = Quaternion(*rot)
        #curr_pose.header.frame_id = 'base_link'
        #curr_pose.header.stamp = rospy.Time.now()
        #print "rot: \n" + str(rot)
        #curr_pose.pose.orientation = Quaternion(*rot)
        #curr_pose.pose.position = Point(0.1, 0.0, 2.0)
        oculus_pub.publish(curr_quat)

        rate.sleep()
