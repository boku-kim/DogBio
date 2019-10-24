/*
 *  YDLIDAR SYSTEM
 *  YDLIDAR ROS Node Client
 *
 *  Copyright 2015 - 2018 EAI TEAM
 *  http://www.ydlidar.com
 *
 */

/*****이것은 임소현이 그냥 일단 에러를 없애고자 수정하던 문서입니다.
 ***** + ctrl-c를 누르면 제대로 종료시키게 수정했습니다.
 ***** + 각도로 거리값을 받는게 아닌 배열로 받은걸 바로 이용하려고 바꾸었습니다.
 ***** + "JHPWMPCA9685.h"헤더가 너무 긴게 꼴보기 싫어서 include에 포함시켰습니다.
 ***** + mutex를 전역으로 선언하였습니다. -> 선언은 main 에
 ***** + stru_sharedInfo에 거리 정보를 넣었고, 이 구조체에 접근 하는것을 뮤텍스로 제어(?)했습니다.  (하지만 사실 뭔지 모르겠습니다.)
 ***** + 각 위치별 평균값으로 받아서 했는데.... 별로인듯... -> 수정필요
 ***** + 알고리즘 어케짜냐...
 ***** + 뮤텍스랑 스레드 어떤식으로 짜야할지 감이 잘 오지 않
 *****백업파일에 원래 코드가 있습니다. (초기 코드)
 *****이 코드 subsc.cpp 파일에만 저장해주시고 다시 백업파일 작업하시면 됩니다.*******/

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

// shared area를 위한 Mutex 전역 변수 선언
pthread_mutex_t g_MutextForSharedInfo; //= PTHREAD_MUTEX_INITIALIZER;

/*  //일단 어떻게 쓸지 모르겠음...
pthread_t t1,t2;
void *thread_increment (void *msg);
void *thread_result;
char thread1[]="control";
char thread2[]="value";
*/


// At 50 hz a frame is 20 milliseconds
// The PCA9685 PWM Driver is 12 bits
// Full Reverse signal to the ESC is 1 ms
// Neutral signal to the ESC is 1.5 ms
// Full PWM signal to the ESC is 2 ms

// The nomenclature is for the ESC throttle, for the steering neutral is center,
// full forward is right, full reverse is left

#define PWM_FULL_REVERSE 204 // 1ms/20ms * 4096
#define PWM_NEUTRAL 307      // 1.5ms/20ms * 4096
#define PWM_FULL_FORWARD 409 // 2ms/20ms * 4096
#define PWM_MIDDLE 312       // Normal moving velocity
#define PWM_HIGH 320         // Fast moving velocity
#define PWM_LEFT 227         // Left moving
#define PWM_RIGHT 387        // Right moving
#define STEER_MIN 90         // Steering min. Angle
#define STEER_MAX 165        // Steering max. Angle


//#define RAD2DEG(x) ((x)*180./M_PI)   //각도 쓰기 싫어서 지움.

// The Steering Servo is plugged into the follow PWM channel
#define STEERING_CHANNEL 0
// The ESC is plugged into the following PWM channel
#define ESC_CHANNEL 1

float currentPWM = PWM_NEUTRAL;
int currentChannel = ESC_CHANNEL;

/*
* pthread create를 위한 함수
void * thread_increment(void *arg)      //thread가 수행하는 함수인데 여기서 무슨 쓸모인지 모르겠음...
{

    printf("%s\n",arg);

    pthread_mutex_lock(&g_MutextForSharedInfo);

    pthread_mutex_unlock(&g_MutextForSharedInfo);

}
*/

//Add Thread

/*
 * 라이더에서 받는 정보들을 담을 구조체...
*/
struct laserscan_msgs//sensor_msgs::LaserScan
{
    float center_dis;
    float left_dis;
    float right_dis;
    //float distanse;
    //float *ranges;
};

struct yolo_msgs//sensor_msgs::LaserScan
{
    // **yolo data 변수 추가 부분
	//float frameWidth = cam_image->image.size().width;
        //float frameHeight = cam_image->image.size().height;
	float dog_xmax; 
	float dog_xmin; 
	float dog_ymax; 
	float dog_ymin; 
	float object_cp; 

};



/*
 * 라이더랑 yolo에서 주는 정보들을 함꼐 관리항 구조체
 * 이걸 뮤텍스로 관리(?) 하는둣?
*/
typedef struct test_data
{
    laserscan_msgs lidar_info;  //lidar data
    yolo_msgs yolo_info;  //yolo data

}AroundInfo;



AroundInfo stru_sharedInfo;

int do_Logic()
{

    AroundInfo shared_info;
    pthread_mutex_lock(&g_MutextForSharedInfo);
    //shared_info 영역
    shared_info = stru_sharedInfo;
    pthread_mutex_unlock(&g_MutextForSharedInfo);

    currentChannel = STEERING_CHANNEL;

    if (shared_info.lidar_info.left_dis < 0.5)
    {

       currentPWM = PWM_LEFT;
      // printf("Left_obstacle = %f \n", shared_info.lidar_info.left_dis);
       printf("Left-");

    }
    else if (shared_info.lidar_info.right_dis < 0.5)
    {

       currentPWM = PWM_RIGHT;
      // printf("Right_obstacle = %f\n", shared_info.lidar_info.right_dis);
       printf("right-");

    }

   else
   {
        currentPWM = PWM_NEUTRAL;
        printf("mid-");
   }

    pca9685->setPWM(currentChannel, 0, currentPWM);


       currentChannel = ESC_CHANNEL;
    //   printf("[YDLIDAR INFO]: angle : [%f, %f] i=%d\n", degree, distance, i);

       float distance = shared_info.lidar_info.center_dis;

       if(distance < 0.3)
       {
           currentPWM = PWM_NEUTRAL;
           printf("1. currentPWM1 = %f\n", currentPWM);
       }
       else if(distance >= 0.3 && distance < 0.8)
       {
           currentPWM = PWM_MIDDLE;
           printf("2. currentPWM2 = %f\n", currentPWM);
       }
       else if(distance >= 0.8)
       {
           currentPWM = PWM_HIGH;
           printf("3. currentPWM3 = %f\n", currentPWM);
       }
       pca9685->setPWM(currentChannel, 0, currentPWM);
       //printf("currunetCH: %d\n", currentChannel);

}


/*
**무슨 함수인지 모르겠는데 빌드할때 에러 떠서 주석처리 했습니다.
**주어진 코드 봐서는 아마 yolo 결과값을 'stru_sharedInfo' struct에 넣으려고 한거 아닐까 생각중,,,?
*/
/*int yolo()
{
    pthread_mutex_lock(&g_MutextForSharedInfo);
    stru_SharedInfo->yolo_info = info;
    pthread_mutex_unlock(&g_MutextForSharedInfo);
}*/




void scanCallback(const sensor_msgs::LaserScan::ConstPtr &scan)
{
  //int count = scan->scan_time / scan->time_increment;
  //printf("[YDLIDAR INFO]: I heard a laser scan %s[%d]:\n", scan->header.frame_id.c_str(), count);
 // printf("[YDLIDAR INFO]: angle_range : [%f, %f]\n", RAD2DEG(scan->angle_min),RAD2DEG(scan->angle_max));

  float mid_avr = 0;
  float left_avr = 0;
  float right_avr = 0;

/*pthread_mutex_lock(&g_MutextForSharedInfo);
  stru_SharedInfo.lidar_info = scan_lidar;
  pthread_mutex_unlock(&g_MutextForSharedInfo);


*/


  for(int i=0;i<20;i++)
  {
      mid_avr += scan->ranges[i];
  }

  for(int i=30;i<160;i++)
  {
      right_avr += scan->ranges[i];
  }

  for(int i=1200;i<1330;i++)
  {
      left_avr += scan->ranges[i];
  }

  for(int i = 1340;i<1360;i++)
  {
      mid_avr += scan->ranges[i];
  }

  pthread_mutex_lock(&g_MutextForSharedInfo);
  stru_sharedInfo.lidar_info.center_dis = mid_avr/40.0;
  stru_sharedInfo.lidar_info.left_dis = left_avr/130.0;
  stru_sharedInfo.lidar_info.right_dis = right_avr/130.0;
  pthread_mutex_unlock(&g_MutextForSharedInfo);

  do_Logic();

  /*
     for (int i = 0; i < count; i+=10)
  {
      float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
      float distance = scan->ranges[i];

       if(degree > 179 || degree < -179)
           printf("front degree= %f distance = %f  arr[%d]\n", degree,distance,i);

       //else if(degree < 175 && degree > 100)

        //   printf("one degree= %f distance = %f  arr[%d]\n", degree,distance,i);

     //  else if(degree > -175 && degree < -100)

        //   printf("side degree= %f distance = %f  arr[%d]\n", degree,distance,i);

  }

*/
  /*

  for (int i = 0; i < count; i++)
  {
    float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
    float distance = scan->ranges[i];

    // 좌측에 장애물이 가까워 지는 경우
    if(degree < -STEER_MIN && degree > -STEER_MAX && distance < 0.5)
    {
        currentChannel = STEERING_CHANNEL;
        currentPWM = PWM_LEFT;
        pca9685->setPWM(currentChannel, 0, currentPWM);
        printf("Left_obstacle = %f \n", degree);
    }
    // 우측에 장애물이 가까워 지는 경우
    else if(degree > STEER_MIN && degree < STEER_MAX && distance < 0.5)
    {
        currentChannel = STEERING_CHANNEL;
        currentPWM = PWM_RIGHT;
        pca9685->setPWM(currentChannel, 0, currentPWM);
        //printf("Right_obstacle = %f\n", degree);
    }
    // 정면에 장애물을 인식하는 경우
    else if(degree > 175 || degree < -175)
    {
        currentChannel = ESC_CHANNEL;
     //   printf("[YDLIDAR INFO]: angle : [%f, %f] i=%d\n", degree, distance, i);

        if(distance < 0.3)
        {
            currentPWM = PWM_NEUTRAL;
       //     printf("1. currentPWM1 = %f\n", currentPWM);
        }
        else if(distance >= 0.3 && distance < 0.8)
        {
            currentPWM = PWM_MIDDLE;
         //   printf("2. currentPWM2 = %f\n", currentPWM);
        }
        else if(distance >= 0.8)
        {
            currentPWM = PWM_HIGH;
         //   printf("3. currentPWM3 = %f\n", currentPWM);
        }
        pca9685->setPWM(currentChannel, 0, currentPWM);
    }
  }
  */
}

//yolo_Callback
void dogCallback(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg)
{

        // strcmp(msg->boundingboxes[i].class,"dog");
        /* 
	printf("[object]: %s \n [x_min]: %d \n [y_min]: %d \n [x_max]: %d \n [y_max]: %d \n", boundingBox.Class, boundingBox.xmin,boundingBox.ymin, boundingBox.xmax, boundingBox.ymax);
*/

     pthread_mutex_lock(&g_MutextForSharedInfo);
     if(msg->bounding_boxes[0].Class.compare(target) == 0)
     {

	cout<<">>>>TARGET (Class):" << msg->bounding_boxes[0].Class <<endl;
	cout<<"Bounding Boxes (Header):" << msg->header <<endl;
	cout<<"Bounding Boxes (image_header):" << msg->image_header <<endl;	
	cout<<"Bounding Boxes (Class):" << msg->bounding_boxes[0].Class <<endl;
	cout<<"Bounding Boxes (xmax):" << msg->bounding_boxes[0].xmax <<endl;
	cout<<"Bounding Boxes (xmin):" << msg->bounding_boxes[0].xmin <<endl;
	cout<<"Bounding Boxes (ymax):" << msg->bounding_boxes[0].ymax <<endl;
	cout<<"Bounding Boxes (ymin):" << msg->bounding_boxes[0].ymin <<endl;
	

	cout << "\033[2J\033[1;1H";		//clear terminal
     }
     	stru_sharedInfo.yolo_info.dog_xmax = msg->bounding_boxes[0].xmax;
	stru_sharedInfo.yolo_info.dog_xmin = msg->bounding_boxes[0].xmin;
	stru_sharedInfo.yolo_info.dog_ymax = msg->bounding_boxes[0].ymax;
	stru_sharedInfo.yolo_info.dog_ymin = msg->bounding_boxes[0].ymin;
	stru_sharedInfo.yolo_info.object_cp = (stru_sharedInfo.yolo_info.dog_xmax + stru_sharedInfo.yolo_info.dog_xmin)/2;

     pthread_mutex_unlock(&g_MutextForSharedInfo);
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

PCA9685::~PCA9685() {
    closePCA9685() ;
}

bool PCA9685::openPCA9685()
{
    char fileNameBuffer[32];
    sprintf(fileNameBuffer,"/dev/i2c-%d", kI2CBus);
    kI2CFileDescriptor = open(fileNameBuffer, O_RDWR);
    if (kI2CFileDescriptor < 0) {
        // Could not open the file
       error = errno ;
       return false ;
    }
    if (ioctl(kI2CFileDescriptor, I2C_SLAVE, kI2CAddress) < 0) {
        // Could not open the device on the bus
        error = errno ;
        return false ;
    }
    return true ;
}

void PCA9685::closePCA9685()
{
    if (kI2CFileDescriptor > 0) {
        close(kI2CFileDescriptor);
        // WARNING - This is not quite right, need to check for error first
        kI2CFileDescriptor = -1 ;
        currentPWM = PWM_NEUTRAL;
        pca9685->setPWM(currentChannel, 0, currentPWM);
    }
}

void PCA9685::reset () {
    writeByte(PCA9685_MODE1, PCA9685_ALLCALL );
    writeByte(PCA9685_MODE2, PCA9685_OUTDRV) ;
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
       delete pca9685;


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
/*
  //
  // create two thread
  pthread_create(&t1, NULL, thread_increment, &thread1);
  pthread_create(&t2, NULL, thread_increment, &thread2);

  // thread 생성 완료될 때까지 지연
  pthread_join(t1, &thread_result);
  pthread_join(t2, &thread_result);
*/
  //destroy Mutex
  //pthread_mutex_destroy(&g_MutextForSharedInfo);

  ros::init(argc, argv, "ydlidar_client");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);


  //yolo_subscribe
  ros::init(argc, argv, "dog_tracking");
  ros::NodeHandle nh;
// ros::Subscriber sub = n.subscribe("keys", 1, Callback);
  ros::Subscriber dog_tracking_sub = nh.subscribe("/darknet_ros/bounding_boxes", 1, dogCallback);
  



  ros::spin();




  return 0;
}

