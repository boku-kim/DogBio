/*
 *  YDLIDAR SYSTEM
 *  YDLIDAR ROS Node Client
 *
 *  Copyright 2015 - 2018 EAI TEAM
 *  http://www.ydlidar.com
 *
 */

/*****이것은 임소현이 그냥 일단 에러를 없애고자 수정하던 문서입니다.
 *****백업파일에 원래 코드가 있습니다.
 *****이 코드 subsc.cpp 파일에만 저장해주시고 다시 백업파일 작업하시면 됩니다.*******/

#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "../../../../jetsonRACECAR/research/throttleControl/JHPWMPCA9685.h"
#include <math.h>
#include <iostream>
#include <signal.h>
#include <pthread.h>


PCA9685 *pca9685;

//pthread pointer 전역으로 선언
pthread_mutex_t g_MutextForSharedInfo;
pthread_t t1,t2;
void *thread_increment (void *msg);
void *thread_result;
char thread1[]="contral";
char thread2[]="value";



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

#define RAD2DEG(x) ((x)*180./M_PI)

// The Steering Servo is plugged into the follow PWM channel
#define STEERING_CHANNEL 0
// The ESC is plugged into the following PWM channel
#define ESC_CHANNEL 1

float currentPWM = PWM_NEUTRAL;
int currentChannel = ESC_CHANNEL;

/*
* pthread create를 위한 함수
*/
void * thread_increment(void *arg)
{



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

//Add Thread
struct laserscan_msgs//sensor_msgs::LaserScan
{
    float angle_min;
    float angle_increment;
    float *ranges;
};

typedef struct test_data
{
    laserscan_msgs lidar_info;

    // **yolo data 변수 추가 부분
}SensorInfo;


int do_Logic()
{

    /*
    SensorInfo SharedInfo_t;
    pthread_mutex_lock(&g_MutextForSharedInfo);
    SharedInfo_t = stru_SharedInfo;
    pthread_mutex_unlock(&g_MutextForSharedInfo);


    if (SharedInfo_t->lidar_info->scantime >  0)
    {

    }
    else
    {

    }

*/
}

/*int yolo()
{
    pthread_mutex_lock(&g_MutextForSharedInfo);
    stru_SharedInfo->yolo_info = info;
    pthread_mutex_unlock(&g_MutextForSharedInfo);
}*/




void scanCallback(const sensor_msgs::LaserScan::ConstPtr &scan)
{
  int count = scan->scan_time / scan->time_increment;
  printf("[YDLIDAR INFO]: I heard a laser scan %s[%d]:\n", scan->header.frame_id.c_str(), count);
  printf("[YDLIDAR INFO]: angle_range : [%f, %f]\n", RAD2DEG(scan->angle_min),
         RAD2DEG(scan->angle_max));

  pthread_mutex_lock(&g_MutextForSharedInfo);
  //SensorInfo.lidar_info = scan_lidar;
  //SensorInfo.test_data = scan_yolo;
  pthread_mutex_unlock(&g_MutextForSharedInfo);




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
        printf("Left_obstacle = %f\n", degree);
    }
    // 우측에 장애물이 가까워 지는 경우
    else if(degree > STEER_MIN && degree < STEER_MAX && distance < 0.5)
    {
        currentChannel = STEERING_CHANNEL;
        currentPWM = PWM_RIGHT;
        pca9685->setPWM(currentChannel, 0, currentPWM);
        printf("Right_obstacle = %f\n", degree);
    }
    // 정면에 장애물을 인식하는 경우
    else if(degree > 175 || degree < -175)
    {
        currentChannel = ESC_CHANNEL;
        printf("[YDLIDAR INFO]: angle : [%f, %f] i=%d\n", degree, distance, i);

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
    }
  }
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
       printf("PCA9685 Read Byte error: %d",errno) ;
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
       printf("PCA9685 Write Byte error: %d",errno) ;
       error = errno ;
       toReturn = -1 ;
   }
}

/*
** when user press ctrl-c, car stop and ros shutdown
*/
void sigint_handler(int sig)
{

   currentPWM = PWM_NEUTRAL;

   pca9685->setPWM(ESC_CHANNEL, 0, currentPWM);
   sleep(1);
   pca9685->setPWM(STEERING_CHANNEL, 0, currentPWM);
   sleep(1);

   ros::shutdown();
}

int main(int argc, char **argv)
{
  initialset();
  signal(SIGINT,sigint_handler);          // ctrl +"C" 입력시 종료

  //Mutex init
  int state =  pthread_mutex_init(&g_MutextForSharedInfo, NULL);
  if(state)
  {
     // ErrorHandling("pthread_mutex_init() error");
      printf("mutex lilt fail \n");
      exit(1);
  }

  //
  // create two thread
  pthread_create(&t1, NULL, thread_increment, &thread1);
  pthread_create(&t2, NULL, thread_increment, &thread2);

  // thread 생성 완료될 때까지 지연
  pthread_join(t1, &thread_result);
  pthread_join(t2, &thread_result);

  //destroy Mutex
  pthread_mutex_destroy(&g_MutextForSharedInfo);

  ros::init(argc, argv, "ydlidar_client");
  ros::NodeHandle n;
 //<sensor_msgs::LaserScan> ??
  ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);
  ros::spin();

  SensorInfo stru_SharedInfo;




  return 0;
}

