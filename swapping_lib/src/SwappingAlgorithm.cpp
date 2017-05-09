//
// Created by Ammar Nahari on 5/5/16.
//


#include <SwappingAlgorithm.h>

SwappingAlgorithm::SwappingAlgorithm(ros::NodeHandle nodeHandle, RobotMove robotMove, BinManager binManager) : nh(nodeHandle)
{
    robotMovePtr.allocate_shared(robotMove);
    binManagerPtr.allocate_shared(binManager);


    ros::spin();
}

bool SwappingAlgorithm::swapParts(vector<pair<Part, Part>> action) {

    bool found_storage = true; //triggers false when no storage available on tray
    int correction_counter= action.size-1; // counts the movements done 

    //look for storage location on AGV
    //Since we don't have AGV manager yet we are using Bim Manager instead
    //save the storage location in variable temp_part 
    found_storage = binManagerPtr.AllLocationsForPut(temp_part.name, temp_part.location);

    if(!found_storage){
        // if there is no storage space on tray return false
        ROS_INFO("Coudln't find a storage location on tray");
        //TODO: we could call kevins service and embty the tray here
        return false;
    }
    else{
        //if storage exist :move first part in the parts vector to storage
        robotMovePtr.enableAsync();
        robotMovePtr.move(action[0].first, temp_part);

        ROS_INFO("Moving first part to temp. storage");

        while (!robotMovePtr.actionFinished()){  
            ros::Duration(0.3).sleep();
        }

        while(correction_counter<0){
            //stay in the loop until all parts are in the right location

            for(int i=1; i<action.size ; i++){

                //iterate between all the parts and do any possible correction movement
                if(binManagerPtr.AllLocationsForTake(action[i].second.type,action[i].second.location)){
                    // if the condition is true: it means the ith element can be moved to the target position
                    //command the movement
                    robotMovePtr.enableAsync();
                    robotMovePtr.move(action[i].first, action[i].second);

                    ROS_INFO("Moving part %d", i);

                    while (!robotMovePtr.actionFinished()){  
                        ros::Duration(0.3).sleep();
                    }
                    correction_counter--;// decrease the corrected parts counter by one
                }        
            }
        }

        //when out of the while loop: it means we succeed in moving all the parts to their location exept for the first part
        //move the first part from the temporary location to its target location
        robotMovePtr.enableAsync();
        robotMovePtr.move(temp_part, action[0].second);

        ROS_INFO("Moving first part from  temp. storage to target location");

        while (!robotMovePtr.actionFinished()){  
            ros::Duration(0.3).sleep();
        }        
       
    }

    ROS_INFO("Done swapping all the pairs");

    ROS_INFO("exiting");

    return true;  
}

