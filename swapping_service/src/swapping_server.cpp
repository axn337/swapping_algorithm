//Code editted by Ammar Nahari axn337
//04/23/2017
//Modified service structure from Kevin Bradner's code trey_cleaning_service

//swapping_server is a code that uses service msgs to recieve two parts locations and types and then perform a swapp in location between them
//The concipts is that the client should send two parts location on agv, the server moves one part to a temporary storage location on tray, the other parts moes to the initial location of the first part, and finally the pfirst part moves from the temporary storage location to the initial location of the second pose. 
// if the service is done, response is sent to the client as true. If otherwise, the response is false.

#include <ros/ros.h>
#include <cwru_ariac/Part.h>
#include <cwru_ariac/InventoryServiceMsg.h>
#include <cwru_ariac/SwappingServiceMsg.h>
#include "CameraEstimator.h"
#include <iostream>
#include <string>


ros::ServiceClient storageFinderClient;
cwru_ariac::InventoryServiceMsg storageFingerMsg;
cwru_ariac::SwappingServiceMsg trayClearMsg;


bool swapCallback(cwru_ariac::SwappingServiceMsgRequest& request, cwru_ariac::TrayClearServiceMsgResponse& response){

	
	//Part part_1,part_2,storage;
	//RobotMove robotMove(nh);

	//should recive two poses
	std::vector<string> part_names_vec(request.part_names, request.part_names + sizeof request.part_names / sizeof request.part_names[0]);

	std::vector<geometry_msgs/PoseStamped> initial_poses_vec(request.initial_poses, request.initial_poses + sizeof request.initial_poses / sizeof request.initial_poses[0]);




	//save the location of part_1 and part_2 and the temporary location
    geometry_msgs::PoseStamped temp_location,final_pose1,final_pose2;

    storageFingerMsg.request.part_name = part_name;

    temp_location.pose.position.x=storageFingerMsg.response.poses[0].pose.position.x;
    temp_location.pose.position.y=storageFingerMsg.response.poses[0].pose.position.y;
    temp_location.pose.position.z=storageFingerMsg.response.poses[0].pose.position.z;
    temp_location.pose.orientation.x=storageFingerMsg.response.poses[0].pose.orientation.x;
    temp_location.pose.orientation.y=storageFingerMsg.response.poses[0].pose.orientation.y;
    temp_location.pose.orientation.z=storageFingerMsg.response.poses[0].pose.orientation.z;
    temp_location.pose.orie.ntation.w=storageFingerMsg.response.poses[0].pose.orientation.w;

    final_pose1.pose.position.x=initial_poses_vec[1].pose.position.x;
    final_pose1.pose.position.y=initial_poses_vec[1].pose.position.y;
    final_pose1.pose.position.z=initial_poses_vec[1].pose.position.z;
    final_pose1.pose.orientation.x=initial_poses_vec[1].pose.orientation.x;
    final_pose1.pose.orientation.y=initial_poses_vec[1].pose.orientation.y;
    final_pose1.pose.orientation.z=initial_poses_vec[1].pose.orientation.z;
    final_pose1.pose.orientation.w=initial_poses_vec[1].pose.orientation.w;

    final_pose2.pose.position.x=initial_poses_vec[0].pose.position.x;
    final_pose2.pose.position.y=initial_poses_vec[0].pose.position.y;
    final_pose2.pose.position.z=initial_poses_vec[0].pose.position.z;
    final_pose2.pose.orientation.x=initial_poses_vec[0].pose.orientation.x;
    final_pose2.pose.orientation.y=initial_poses_vec[0].pose.orientation.y;
    final_pose2.pose.orientation.z=initial_poses_vec[0].pose.orientation.z;
    final_pose2.pose.orientation.w=initial_poses_vec[0].pose.orientation.w;


    //populate a goal message for manipulation
    robotMove.enableAsync();
    robotMove.move(initial_poses_vec[0], temp_location);


	while (!robotMove.actionFinished())
    {  
        ROS_INFO("waiting for result");
        ros::Duration(1).sleep();
    }
    
    robotMove.enableAsync();
    robotMove.move(initial_poses_vec[1], final_pose2);

	while (!robotMove.actionFinished())
    {  
        ROS_INFO("waiting for result");
        ros::Duration(1).sleep();
    }

    robotMove.enableAsync();
    robotMove.move(temp_location, final_pose1);

	while (!robotMove.actionFinished())
    {  
        ROS_INFO("waiting for result");
        ros::Duration(1).sleep();
    }

	return 0;
}
	


int main(int argc, char **argv){
	
	//initialize node
	ros::init(argc, argv, "swapping_server");
	ros::NodeHandle n;
	
	//set up communications with the location finder node
	storageFinderClient = n.serviceClient<cwru_ariac::InventoryServiceMsg>("look_up_parts_space");
	while (!client.exists()) {
		ROS_WARN("waiting for location finder service...");
		ros::Duration(1.0).sleep();
    }
    ROS_INFO("connected to temporary location finder service");

    //set up a server to take requests of parts to clear
	ros::ServiceServer service = nh.advertiseService("swapping_server",swapCallback);
	ROS_INFO("ready to accept two parts");

	ros::spin();
	return 0;
}