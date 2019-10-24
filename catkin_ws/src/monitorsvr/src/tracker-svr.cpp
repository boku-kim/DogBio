/**
****************************************************************************************************
* @file tracker-svr.cpp
* @brief Main entry function for Dog tracker implementation.
* @author 
* @date Created Oct 01, 2019
* @see Coding standard guideline
****************************************************************************************************
*/
#if defined(_OS_WINDOWS) && defined(WIN32)
#include "stdafx.h"
#include "os_windows.h"
#elif defined(_OS_LINUX)
#include "os_linux.h"
#else
#error Unknown OS - not supported
#endif /* _OS_WINDOWS || _OS_LINUX*/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"
#include "darknet_ros_msgs/BoundingBoxes.h"
#include "darknet_ros_msgs/BoundingBox.h"
#include "SignalHandler.h"
#include "MonitorSvr.h"
#include "Utils.h"

#define THIS_NODE_NAME              "monitorsvr"
#define SUBSCRIBE_TOPIC_NAME        "/scan"
#define FOUNDOBJECT_TOPIC_NAME		"/darknet_ros/found_object"
#define BOUNDINGBOX_TOPIC_NAME		"/darknet_ros/bounding_boxes"

using namespace std;

#ifdef _TEST_DATA_SUPPORT
pthread_t g_test_tid = 0;
#endif // _TEST_DATA_SUPPORT

DT_STATUS InitiateApplication();
DT_STATUS TerminateApplication();
void system_size_check();


#define MAIN_SERVER_STOP_DELAY_TIME     5   // seconds

#ifdef _TEST_DATA_SUPPORT
static int s_test_data_support_loop = 1;
std_msgs::String::ConstPtr tmpsg;
darknet_ros_msgs::BoundingBoxes::ConstPtr boundingboxes;

int object_cnt=0;
string target = "apple";
//TODO FreeSendData
static void FreeSendData(DtSendDataWrap_t* pWrap)
{
    DtDetectedArea_t* pTemp = NULL;
    DtDetectedArea_t* pTarget = NULL;
    
    if (pWrap)
    {
        pTarget = pWrap->detected_info;
        while (pTarget != NULL)
        {
            pTemp = pTarget->pNext;
            DT_FREE(pTarget);
            pTarget = pTemp;
        }
        DT_FREE(pWrap);
    }
}
//TODO dt_test_data_supporter
void* dt_test_data_supporter(void* data)
{
    MonitorServer* pMonitor = NULL;
    DtSendDataWrap_t* pItem = NULL;
    DtDetectedArea_t* detected_area = NULL;
    DtDetectedArea_t* area_next_store = NULL;
    DWORD idx = 0;
    DWORD success = 1;
    
    pMonitor = MonitorServer::GetInstance();
    if (pMonitor)
    {
        while (s_test_data_support_loop)
        {
            pItem = (DtSendDataWrap_t*)malloc(sizeof(DtSendDataWrap_t));
            if (pItem)
            {
                area_next_store = NULL;
                memset(pItem, 0x00, sizeof(DtSendDataWrap_t));
                pItem->steering_angle = 5.5;
                pItem->speed = 20;
                pItem->detected_count = object_cnt;
                for (idx = 0; idx < pItem->detected_count; ++idx)
                {
                    detected_area = (DtDetectedArea_t*)malloc(sizeof(DtDetectedArea_t));
                    if (detected_area)
                    {
                        memset(detected_area, 0x00, sizeof(DtDetectedArea_t));
                        detected_area->x = 100;
                        detected_area->y = 100;
                        detected_area->w = 200;
                        detected_area->h = 200;
                        detected_area->cls = 10;
                        ;
                        if (area_next_store != NULL)
                        {
                            area_next_store->pNext = detected_area;
                        }
                        else
                        {
                            pItem->detected_info = detected_area;
                        }
                        area_next_store = detected_area;
                    }
                    else
                    {
                        success = 0;
                    } // end if (detected_area)
                } // end for(...)
                if (success)
                {
                    pMonitor->AddItem(pItem);
                    DogTrack_SleepMs(300);
                }
                else
                {
                    FreeSendData(pItem);
                }
            } // end if (pItem)
        } // end while (s_test_data_support_loop)
    } // end if (pMonitor)
    return NULL;
}
#endif // _TEST_DATA_SUPPORT

//TODO InitiateApplication()
DT_STATUS InitiateApplication()
{
    DT_CHECK_POINT("signal handler will be starting.");
    SignalHandler* pSigHandler = SignalHandler::GetInstance();
    if (pSigHandler == NULL)
    {
        return DT_STATUS_FAIL;
    }

    MonitorServer* pMonitorSvr = MonitorServer::GetInstance();
    if (pMonitorSvr == NULL)
    {
        return DT_STATUS_FAIL;
    }

#ifdef _TEST_DATA_SUPPORT
    if (pthread_create(&g_test_tid, 
                       NULL, 
                       &dt_test_data_supporter, 
                       (void *)NULL) != 0)
    {
        DT_CHECK_POINT("Creating test-data-supporter was failed.");
        g_test_tid = 0;
    }
#endif // _TEST_DATA_SUPPORT

	return DT_STATUS_OK;
}

//TODO TerminateApplication
DT_STATUS TerminateApplication()
{
#ifdef _TEST_DATA_SUPPORT
    if (g_test_tid)
    {
        s_test_data_support_loop = 0;
        pthread_join(g_test_tid, NULL);
        g_test_tid = 0;
    }
#endif // _TEST_DATA_SUPPORT

    MonitorServer::ReleaseInstance();
    SignalHandler::ReleaseInstance();
	return DT_STATUS_OK;
}
//TODO system_size_check()
void system_size_check()
{
#if defined(WIN32)
    DT_CHECK_POINT("Windows 32 bit");
#endif /* WIN32 */
    DT_CHECK_POINT("unsigned char(%u bytes), unsigned short(%u bytes), unsigned int(%u bytes), unsigned long(%u bytes), unsigned long long(%u bytes), unsigned long int(%u bytes), unsigned(%u bytes)",
        sizeof(unsigned char), sizeof(unsigned short), sizeof(unsigned int), sizeof(unsigned long), sizeof(unsigned long long), sizeof(unsigned long int), sizeof(unsigned));
    DT_CHECK_POINT("size for application >> (BYTE size:%u bytes) (WORD size:%u bytes) (DWORD size:%u bytes)",
        sizeof(BYTE), sizeof(WORD), sizeof(DWORD));
}
//TODO machine_info_Callback
void machine_info_Callback(const std_msgs::String::ConstPtr& msg)
{
    tmpsg=msg;
    ROS_INFO("received : [%s]", msg->data.c_str());
    DT_CHECK_POINT(tmpsg->data.c_str());
}


void machine_info_Callback_FoundObject(const std_msgs::Int8& msg)
{
    //tmpsg=msg;
	//DT_CHECK_POINT(msg.data);
	object_cnt=(int)msg.data;
	ROS_INFO("received object cnt : [%d]",msg.data);	
}
void machine_info_Callback_BoundingBoxes(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg)
{
	boundingboxes=msg;
	cout<<"**************bounding boxes***********"<<endl<<endl;
	for(int i=0;i<object_cnt;i++){
		cout<<"("<<i+1<<")"<<endl;
		cout<<"Bouding Boxes (header):" << boundingboxes->header <<endl;
    	cout<<"Bouding Boxes (image_header):" << boundingboxes->image_header <<endl;
    	cout<<"Bouding Boxes (Class):" << boundingboxes->bounding_boxes[i].Class <<endl;
    	cout<<"Bouding Boxes (xmin):" << boundingboxes->bounding_boxes[i].xmin <<endl;
    	cout<<"Bouding Boxes (xmax):" << boundingboxes->bounding_boxes[i].xmax <<endl;
    	cout<<"Bouding Boxes (ymin):" << boundingboxes->bounding_boxes[i].ymin <<endl;
   	 	cout<<"Bouding Boxes (ymax):" << boundingboxes->bounding_boxes[i].ymax <<endl<<endl;
	}

}
//TODO main
/**
****************************************************************************************************
* main
****************************************************************************************************
*/
int main(int argc, char* argv[])
{
    DWORD uDelay = 0;
	DT_STATUS ret = DT_STATUS_OK;
#ifdef _DEBUG
    system_size_check();
#endif /* _DEBUG */

    // Initizting System Base
	DT_CHECK_POINT("Initiating Application Base.");
	ret = InitiateApplication();
	if (ret != DT_STATUS_OK)
	{
		DT_CHECK_POINT("Failed to initiate application(%d).", ret);
		/* //[[ comment___BEGIN -- tmep.nohgan
		g_main_loop_quit(main_loop);
        g_main_loop_unref(main_loop);
		main_loop = NULL;
		//*/ //]] comment___END -- tmep.nohgan
		TerminateApplication();
		return ret;
	}
    DT_CHECK_POINT("Initiating Application Base was succeeded.");

	// Init signal catch
	DT_CHECK_POINT("Starting Signal Handler.");
	ret = SignalHandler::GetInstance()->StartHandler(NULL);
	if (ret != DT_STATUS_OK)
	{
		DT_CHECK_POINT("Failed to start signal handler(%d).", ret);
		/* //[[ comment___BEGIN -- tmep.nohgan
		g_main_loop_quit(main_loop);
        g_main_loop_unref(main_loop);
		main_loop = NULL;
		//*/ //]] comment___END -- tmep.nohgan
		TerminateApplication();
		return ret;
	}

    ///< ROS initialization function.
    ros::init(argc, argv, THIS_NODE_NAME);

    ///< NodeHandle is the main access point to communications with the ROS system
    ros::NodeHandle hThidNode;

    ///< how you tell ROS that you want to receive messages on a given topic.  
    ros::Subscriber FoundObjectSubscriber = hThidNode.subscribe(FOUNDOBJECT_TOPIC_NAME, 1000, machine_info_Callback_FoundObject);
	ros::Subscriber BoundingBoxSubscriber = hThidNode.subscribe(BOUNDINGBOX_TOPIC_NAME, 1000, machine_info_Callback_BoundingBoxes);
    //ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

    ///< will enter a loop, pumping callbacks.
    ros::spin();

	// Init glib mainloop
	/* //[[ comment___BEGIN -- tmep.nohgan
	GMainLoop *main_loop = NULL;
    printf("starting\n");

	main_loop = g_main_loop_new(NULL, FALSE);
	if (main_loop == NULL)
	{
		DT_CHECK_POINT("Failed to create main event loop.");
		return DT_STATUS_EVENT_LOOP_INVALID;
	}
	//*/ //]] comment___END -- tmep.nohgan

	

	// Running Main Event Loop
	/* //[[ comment___BEGIN -- tmep.nohgan
	DT_CHECK_POINT("Running Main Event Loop.");
	if (main_loop)
	{
		g_main_loop_run(main_loop);
		g_main_loop_unref(main_loop);
		main_loop = NULL;
	}	
	//*/ //]] comment___END -- tmep.nohgan
    DT_CHECK_POINT("Server Will be closing.");

	// Wait for Signal Catcher Thread's terminating.
	DT_CHECK_POINT("Stopping Signal Handler.");
	ret = SignalHandler::GetInstance()->StopHandler();
	if (ret != DT_STATUS_OK)
	{
		DT_CHECK_POINT("Failed to stopping signal handler(%d).", ret);
		TerminateApplication();
		return ret;
	}

	DT_CHECK_POINT("Terminating Application Base.");
	ret = TerminateApplication();
	if (ret != DT_STATUS_OK)
	{
		DT_CHECK_POINT("Failed to terminating network environment(%d).", ret);
        return ret;
	}

	DT_CHECK_POINT("Waiting %d seconds for network server's terminating gracefully.", 
	    MAIN_SERVER_STOP_DELAY_TIME);
	while (uDelay < MAIN_SERVER_STOP_DELAY_TIME)
	{
        DogTrack_SleepMs(1000);
        ++uDelay;
        DT_CHECK_POINT("Elapsed delay time : %u seconds", uDelay);
	}
    
    DT_CHECK_POINT("process will be exit.");
    return ret;
}

