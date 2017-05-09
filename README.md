# ammar_ros_service

A node that deal with service-client communication method. This package is the third assignment in the course EECS-476. It has two cpp files: my_path_service and my_path_client. In my_path_client, a number of poses are specified and sent to my_path_client, where it communicate with the stdr_simulator to move the robot according to the specified poses taken from my_path_client.

## Example usage/Running tests

Running the samulator:
roslaunch stdr_launchers server_with_map_and_gui_plus_robot.launch

Running the service:
rosrun ammar_ros_service my_path_service

Running the service client:
rosrun ammar_ros_service my_path_client



    
