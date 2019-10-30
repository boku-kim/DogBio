/*
 *  YDLIDAR SYSTEM
 *  YDLIDAR ROS Node Client
 *
 *  Copyright 2015 - 2018 EAI TEAM
 *  http://www.ydlidar.com
 *
 */

/***** + ctrl+c를 누르면 종료되도록 수정했습니다.
 ***** + mutex를 전역으로 선언하였습니다. -> 선언은 main 문에 위치
 ***** + stru_sharedInfo에 거리 정보를 넣었고, 이 구조체에 접근 하는것을 뮤텍스로 제어했습니다.
 ***** + 전방 좌우 60도 범위를 sensing하여 가장 가까운 거리를 인식(output: distance, angle)
 ***** + 범위 밖의 값은 default 값(20)으로 처리하여 sensing에서 제외 시킴
 ***** + YOLO와 Lidar에서 받은 data를 Thread로 shared_area로 전달.
 ***** + YOLO를 통해 강아지를 인식하고, 강아지의 방향대로 steering되도록 설정
*/

#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "JHPWMPCA9685.h"
#include <math.h>
#include <iostream>
#include <signal.h>
#include <pthread.h>
#include <std_msgs/String.h>
#include "darknet_ros_msgs/BoundingBoxes.h"
#include "darknet_ros_msgs/BoundingBox.h"

PCA9685 *pca9685;
using namespace std;

//yolo_v3_target
string target = "dog";
bool detect_target = false;

int numClasses = 1;

// shared area를 위한 Mutex 전역 변수 선언
pthread_mutex_t g_MutextForSharedInfo; //= PTHREAD_MUTEX_INITIALIZER;

// At 50 hz a frame is 20 milliseconds
// The PCA9685 PWM Driver is 12 bits
// Full Reverse signal to the ESC is 1 ms
// Neutral signal to the ESC is 1.5 ms
// Full PWM signal to the ESC is 2 ms

// The nomenclature is for the ESC throttle, for the steering neutral is center,
// full forward is right, full reverse is left

#define PWM_FULL_REVERSE 204        // 1ms/20ms * 4096
#define PWM_NEUTRAL 307             // 1.5ms/20ms * 4096
#define PWM_FULL_FORWARD 409        // 2ms/20ms * 4096
#define PWM_MIDDLE 332              // Normal moving velocity
#define PWM_HIGH 334                // Fast moving velocity
#define PWM_LEFT 204                // Left moving
#define PWM_RIGHT 409               // Right moving
#define STOP_DIST 0.3               // Stop distance
#define LOW_SPEED_DIST 1.2          // Low speed distance
#define DANGER_DIST 0.6             // Distance to avoid obstacle.
#define RAD2DEG(x) ((x)*180./M_PI)  // Rad에서 deg로 각도를 변환

// The Steering Servo is plugged into the follow PWM channel
#define STEERING_CHANNEL 0
// The ESC is plugged into the following PWM channel
#define ESC_CHANNEL 1

float currentPWM = PWM_NEUTRAL;
int currentChannel = ESC_CHANNEL;

/*
 * 라이더에서 받는 정보들을 담을 구조체
*/
struct laserscan_msgs
{
    float dist;         // lidar에서 측정한 각 index별 거리
    float deg;          // lidar에서 측정한 각 index별 각
    float obs_dist;     // 장애물 까지의 최단 거리
    float obs_ix;       // 장애물과의 각도
};

struct yolo_msgs
{
    int dog_xmax;
    int dog_xmin;
    int dog_ymax;
    int dog_ymin;
    int object_cp;
    string object_class;
};


// 라이더랑 yolo에서 주는 정보들을 함꼐 관리항 구조체
typedef struct test_data
{
    laserscan_msgs lidar_info;  //lidar data
    yolo_msgs shared_yoloinfo;  //yolo data

}AroundInfo;

AroundInfo stru_sharedInfo;

int do_Logic()
{
    int sub = 0;

    AroundInfo shared_info;
    pthread_mutex_lock(&g_MutextForSharedInfo);
    //shared_info 영역
    shared_info = stru_sharedInfo;
    pthread_mutex_unlock(&g_MutextForSharedInfo);
    string object_class = stru_sharedInfo.shared_yoloinfo.object_class;


    /*
    ** 강아지가 잡히면 우선권 yolo가 가짐
    */
    int xmax = shared_info.shared_yoloinfo.dog_xmax;
    int xmin = shared_info.shared_yoloinfo.dog_xmin;
    int ymax = shared_info.shared_yoloinfo.dog_ymax;
    int ymin = shared_info.shared_yoloinfo.dog_ymin;
    int cp = shared_info.shared_yoloinfo.object_cp;
	
    int x_deg_l = 320 - cp;
    int x_deg_r = cp - 320;

    //if(object_class.compare(target) == 0)

    if(xmax > 0)
    {
        printf("xmax:%d \n, xmin:%d \n, ymax:%d \n, ymin:%d \n, cp:%d \n", xmax,xmin,ymax,ymin,cp);

        for (int i = 0; i < numClasses; i++)
        {

        // left steering
            if( cp > 0 && cp < 216 )
            {
                printf("object is detected leftside");

                currentChannel = STEERING_CHANNEL;
                currentPWM = 307 - ( 0.3 * x_deg_l );
                pca9685->setPWM(currentChannel, 0, currentPWM);
                printf("4. currentPWM_left = %f\n", currentPWM);
            }
            // right steering
            else if( cp > 424 && cp < 640 )
            {
                printf("object is detected rightside");
                currentChannel = STEERING_CHANNEL;
                currentPWM = 307 + ( 0.3 * x_deg_r );
                pca9685->setPWM(currentChannel, 0, currentPWM);
                printf("5. currentPWM_right = %f\n", currentPWM);
            }
            // front
            else if( cp > 216 && cp < 424 )
            {
                printf("object is detected front");
                currentChannel = STEERING_CHANNEL;
                currentPWM = PWM_NEUTRAL;
                pca9685->setPWM(currentChannel, 0, currentPWM);
                printf("2. currentPWM2 = %f\n", currentPWM);
            }
			
                currentChannel = ESC_CHANNEL;

            // stop
            if( xmax-xmin > 500 )
            {
                printf("object is too closed");
                currentPWM = PWM_NEUTRAL;
                pca9685->setPWM(currentChannel, 0, currentPWM);
            }
            //go
            else
            {
                currentPWM = PWM_MIDDLE;
            }

        }

    }

    //강아지가 안잡힐때 LIDAR
    else{
        // Steering 제어
        currentChannel = STEERING_CHANNEL;

        if (shared_info.lidar_info.obs_dist > DANGER_DIST)
        {
            currentPWM = PWM_NEUTRAL;
        }
        else if (shared_info.lidar_info.obs_ix < 0 || shared_info.lidar_info.obs_ix > 1360)
        {
            return -1;
        }
        else if (shared_info.lidar_info.obs_ix > 0 && shared_info.lidar_info.obs_ix < 150)
        {
            currentPWM = PWM_RIGHT;
            printf("Right");
        }
        else if (shared_info.lidar_info.obs_ix > 150 && shared_info.lidar_info.obs_ix < 226)
        {
            sub = 0.588 * (shared_info.lidar_info.obs_ix - 150);
            currentPWM = PWM_RIGHT - sub;
            printf("Right");
        }
        else if (shared_info.lidar_info.obs_ix > 1134 && shared_info.lidar_info.obs_ix < 1210)
        {
            sub = 0.588 * (shared_info.lidar_info.obs_ix - 1020);
            currentPWM = PWM_LEFT + sub;
            printf("Left");
        }
        else if (shared_info.lidar_info.obs_ix > 1210 && shared_info.lidar_info.obs_ix < 1360)
        {
            currentPWM = PWM_LEFT;
            printf("Left");
        }
        pca9685->setPWM(currentChannel, 0, currentPWM);

        // 모터 속도 제어
        currentChannel = ESC_CHANNEL;

        float distance = shared_info.lidar_info.obs_dist;

        if(distance < STOP_DIST)
        {
           currentPWM = PWM_NEUTRAL;
           // printf("1. currentPWM1 = %f\n", currentPWM);
        }
        else if(distance >= STOP_DIST && distance < LOW_SPEED_DIST)
        {
           currentPWM = PWM_MIDDLE;
           // printf("2. currentPWM2 = %f\n", currentPWM);
        }
        else if(distance >= LOW_SPEED_DIST)
        {
           currentPWM = PWM_HIGH;
           // printf("3. currentPWM3 = %f\n", currentPWM);
        }
        pca9685->setPWM(currentChannel, 0, currentPWM);
    }
}


void scanCallback(const sensor_msgs::LaserScan::ConstPtr &scan)
{
    int count = scan->scan_time / scan->time_increment;
    float degree = 0;                 // 측정되는 각도
    float distance = 0;               // 측정되는 거리
    float obs_degree = 0;             // 장애물과의 각도
    float min_dist = 9999;            // 장애물과의 최단 거리
    float min_ix = 9999;              // 최단 거리일 때의 index
    float temp_dist[3] = {1,1,1};     // 에러값 검출용 배열

    // 우측에 장애물과의 최단거리 및 각도 구하기
    for (int i = 1; i < count/6; i += 5)
    {
        degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
        distance = scan->ranges[i];

        printf("[degree, distance]: [%f, %f]\n", degree, distance);

        if(distance == 0)
        {
          distance = 20;
        }
        else
        {
            if (min_dist > distance)
            {
                min_dist = distance;
                min_ix = i;
            }
            else
            {
                min_dist = min_dist;
                min_ix = min_ix;
            }
            // 이전 거리 data를 저장
            temp_dist[2] = temp_dist[1];
            temp_dist[1] = temp_dist[0];
            temp_dist[0] = distance;
        }
    }

    obs_degree = RAD2DEG(scan->angle_min + scan->angle_increment * min_ix);

    printf("[obs_degree, obs_distance]: [%f, %f]\n", obs_degree, min_dist);

    // 좌측에 장애물과의 최단거리 및 각도 구하기
    for (int i = count *5/6; i < count; i += 5)
    {
        degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
        distance = scan->ranges[i];

        printf("[degree, distance]: [%f, %f]\n", degree, distance);

        if(distance == 0)
        {
            distance = 20;
        }
        else
        {
            if (min_dist > distance)
            {
                min_dist = distance;
                min_ix = i;
            }
            else
            {
                min_dist = min_dist;
                min_ix = min_ix;
            }
            // 이전 거리 data를 저장
            temp_dist[2] = temp_dist[1];
            temp_dist[1] = temp_dist[0];
            temp_dist[0] = distance;
        }
    }
    obs_degree = RAD2DEG(scan->angle_min + scan->angle_increment * min_ix);

    printf("[obs_degree, obs_distance]: [%f, %f]\n", obs_degree, min_dist);

    pthread_mutex_lock(&g_MutextForSharedInfo);
    stru_sharedInfo.lidar_info.obs_dist = min_dist;
    stru_sharedInfo.lidar_info.obs_ix = min_ix;
    pthread_mutex_unlock(&g_MutextForSharedInfo);

    do_Logic();

}

//yolo_Callback
void dogCallback(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg)
{
    if(msg->bounding_boxes[0].Class.compare(target) == 0)
    {

        cout<<">>>>TARGET (Class):" << msg->bounding_boxes[0].Class <<endl;

        stru_sharedInfo.shared_yoloinfo.dog_xmax = msg->bounding_boxes[0].xmax;
	stru_sharedInfo.shared_yoloinfo.dog_xmin = msg->bounding_boxes[0].xmin;
	stru_sharedInfo.shared_yoloinfo.dog_ymax = msg->bounding_boxes[0].ymax;
	stru_sharedInfo.shared_yoloinfo.dog_ymin = msg->bounding_boxes[0].ymin;
        stru_sharedInfo.shared_yoloinfo.object_cp = (stru_sharedInfo.shared_yoloinfo.dog_xmax + stru_sharedInfo.shared_yoloinfo.dog_xmin)/2;
	stru_sharedInfo.shared_yoloinfo.object_class = msg->bounding_boxes[0].Class;
	printf("dog_xmax: %d \n", stru_sharedInfo.shared_yoloinfo.dog_xmax); 
	printf("dog_xmin: %d \n", stru_sharedInfo.shared_yoloinfo.dog_xmin); 
	printf("dog_ymax: %d \n", stru_sharedInfo.shared_yoloinfo.dog_ymax); 
	printf("dog_ymin: %d \n", stru_sharedInfo.shared_yoloinfo.dog_ymin);
	printf("dog_xcenter: %d \n", stru_sharedInfo.shared_yoloinfo.object_cp);  
	
	do_Logic();

    }
	
    else
    {

        stru_sharedInfo.shared_yoloinfo.dog_xmax = -1 ;
	stru_sharedInfo.shared_yoloinfo.dog_xmin = -1 ;
	stru_sharedInfo.shared_yoloinfo.dog_ymax = -1 ;
	stru_sharedInfo.shared_yoloinfo.dog_ymin = -1 ;
    }
}
	
void initialset()
{
    pca9685 = new PCA9685();
    int err = pca9685->openPCA9685();
    if(err < 0)
    {
        printf("Error: %d", pca9685->error);
    }
    printf("PCA9685 Device Address: 0x%02X\n",pca9685->kI2CAddress) ;
    pca9685->setAllPWM(0,0);
    pca9685->reset();
    pca9685->setPWMFrequency(50);

    // Set the PWM to "neutral" (1.5ms)
    sleep(1);
    pca9685->setPWM(ESC_CHANNEL,0,PWM_NEUTRAL);
    currentPWM = PWM_NEUTRAL ;

    currentChannel=ESC_CHANNEL ;
}

// Motor 제어 환경 설정
PCA9685::PCA9685(int address)
{
    kI2CBus = 1 ;           // Default I2C bus for Jetson TK1
    kI2CAddress = address ; // Defaults to 0x40 for PCA9685 ; jumper settable
    error = 0 ;
}

PCA9685::~PCA9685()
{
    closePCA9685() ;
}

bool PCA9685::openPCA9685()
{
    char fileNameBuffer[32];
    sprintf(fileNameBuffer,"/dev/i2c-%d", kI2CBus);
    kI2CFileDescriptor = open(fileNameBuffer, O_RDWR);
    if (kI2CFileDescriptor < 0)
    {
        // Could not open the file
       error = errno ;
       return false ;
    }
    if (ioctl(kI2CFileDescriptor, I2C_SLAVE, kI2CAddress) < 0)
    {
        // Could not open the device on the bus
        error = errno ;
        return false ;
    }
    return true ;
}

void PCA9685::closePCA9685()
{
    if (kI2CFileDescriptor > 0)
    {
        close(kI2CFileDescriptor);
        // WARNING - This is not quite right, need to check for error first
        kI2CFileDescriptor = -1 ;
        currentPWM = PWM_NEUTRAL;
        pca9685->setPWM(currentChannel, 0, currentPWM);
    }
}

void PCA9685::reset ()
{
    writeByte(PCA9685_MODE1, PCA9685_ALLCALL);
    writeByte(PCA9685_MODE2, PCA9685_OUTDRV);
    // Wait for oscillator to stabilize
    usleep(5000) ;
}

    // Sets the frequency of the PWM signal
    // Frequency is ranged between 40 and 1000 Hertz
    void PCA9685::setPWMFrequency ( float frequency ) {
    printf("Setting PCA9685 PWM frequency to %f Hz\n",frequency) ;
    float rangedFrequency = fmin(fmax(frequency,40),1000) ;
    rangedFrequency *= 0.9 ;   // Correct for overshoot on PCA9685; The method
                               // described in the datasheet overshoots by 1/0.9
    int prescale = (int)(25000000.0f / (4096 * rangedFrequency) - 0.5f) ;

    // For debugging
    int oldMode = readByte(PCA9685_MODE1) ;
    int newMode = ( oldMode & 0x7F ) | PCA9685_SLEEP ;
    writeByte(PCA9685_MODE1, newMode) ;
    writeByte(PCA9685_PRE_SCALE, prescale) ;
    writeByte(PCA9685_MODE1, oldMode) ;

    // Wait for oscillator to stabilize
    usleep(5000) ;
    writeByte(PCA9685_MODE1, oldMode | PCA9685_RESTART) ;
}

// Channels 0-15
// Channels are in sets of 4 bytes
void PCA9685::setPWM ( int channel, int onValue, int offValue)
{
    writeByte(PCA9685_LED0_ON_L+4*channel, onValue & 0xFF) ;
    writeByte(PCA9685_LED0_ON_H+4*channel, onValue >> 8) ;
    writeByte(PCA9685_LED0_OFF_L+4*channel, offValue & 0xFF) ;
    writeByte(PCA9685_LED0_OFF_H+4*channel, offValue >> 8) ;
}

void PCA9685::setAllPWM (int onValue, int offValue)
{
    writeByte(PCA9685_ALL_LED_ON_L, onValue & 0xFF) ;
    writeByte(PCA9685_ALL_LED_ON_H, onValue >> 8) ;
    writeByte(PCA9685_ALL_LED_OFF_L, offValue & 0xFF) ;
    writeByte(PCA9685_ALL_LED_OFF_H, offValue >> 8) ;
}

// Read the given register
int PCA9685::readByte(int readRegister)
{
    int toReturn = i2c_smbus_read_byte_data(kI2CFileDescriptor, readRegister);
    if (toReturn < 0)
    {
        printf("PCA9685 Read Byte error: %d\n",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
    // For debugging
    return toReturn ;
}

// Write the the given value to the given register
int PCA9685::writeByte(int writeRegister, int writeValue)
{   // For debugging:
    int toReturn = i2c_smbus_write_byte_data(kI2CFileDescriptor, writeRegister, writeValue);
    if (toReturn < 0)
    {
        printf("PCA9685 Write Byte error: %d\n",errno) ;
        error = errno ;
        toReturn = -1 ;
    }
}

/*
** when user press ctrl-c, car stop and ros shutdown
*/
void sigint_handler(int sigint)
{

    currentPWM = PWM_NEUTRAL;

    pca9685->setPWM(ESC_CHANNEL, 0, currentPWM);
    //sleep(1);
    pca9685->setPWM(STEERING_CHANNEL, 0, currentPWM);
    //sleep(1);

    pthread_mutex_destroy(&g_MutextForSharedInfo);


    if(pca9685)
    {
        delete pca9685;
    }

   ros::shutdown();
}

int main(int argc, char **argv)
{
    initialset();
    ros::init(argc, argv, "Subscriber_SigHandler", ros::init_options::NoSigintHandler);     //ctrl+c handler init
    signal(SIGINT,sigint_handler);

    //Mutex init
    int state =  pthread_mutex_init(&g_MutextForSharedInfo, NULL);
    if(state)
    {
        printf("mutex lilt fail \n");
        exit(1);
    }

    ros::init(argc, argv, "ydlidar_client");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);

    //yolo_subscribe
    ros::init(argc, argv, "dog_tracking");
    ros::NodeHandle nh;
    // ros::Subscriber sub = n.subscribe("keys", 1, Callback);
    ros::Subscriber dog_tracking_sub = nh.subscribe("/darknet_ros/bounding_boxes",1, dogCallback);
  
    ros::spin();

    return 0;
}

