#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <iostream>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
	ROS_INFO_STREAM("moving robot toward ball");
	
	//request linear_x and angular_z
	ball_chaser::DriveToTarget srv;
	srv.request.linear_x = lin_x;
	srv.request.angular_z = ang_z;
	client.call(srv);
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    int loc = 5;		// -1 for left, 0 for center, 1 for right; 5 for no ball
	float row = img.height;    // is the number of rows
	float col = img.width;	//is the number of columns
	float istep = img.step;
	float htxstep = istep * row;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
	for(int i=0; i< (img.height*img.step); (i+=3)){
		//ROS_INFO("dets- i:%1.2f, p1: %1.2f, p2: %1.2f, p3: %1.2f", (float)i, (float)img.data[i], (float)img.data[i+1], (float)img.data[i+2]);
		if(img.data[i] == 255 && img.data[i+1] == 255 && img.data[i+2] == 255){
	//ROS_INFO("image dets- ht:%1.2f, wd: %1.2f, step: %1.2f", (float)img.height, (float)img.width, (float)img.step);
		//ROS_INFO("i-dets- data:%1.2f, data2: %1.2f, data3: %1.2f", (float)img.data[i], (float)img.data[i+1], (float)img.data[i+3]);
		//if the pixel is white, find out where it is
			if((i%img.step) <= (img.step/3)){
				loc = -1;
				break;
				}
			else if(((i%img.step) >= (img.step/3)) && ((i%img.step)<=(2*img.step/3)))	{
				loc = 0;
				break;	   
	    			}
			else {loc = 1;
				break;
		    		}
			}
	}

			if(loc == -1){
				drive_robot(0,0.1);
				//break;			
				}
			else if(loc == 0){
				drive_robot(0.2,0);
				//break;
				}
				//ROS_INFO("u r here");
			else if(loc == 1){
				drive_robot(0,-0.1);
				//break;
				}
			else {drive_robot(0,0);
				//break;}
				}
			
}


int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
