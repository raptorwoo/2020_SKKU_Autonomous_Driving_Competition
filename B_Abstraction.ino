/*
 * **Important**
 * Abstraction1
 * Abstraction layer for missions, for example, parallel/backward parking. 
 * Performs Designated Task for a given time period
 */

//타이머 세팅
inline void setTimer()
{
  iTimeCount = 0;
  bInMission = true;
  MsTimer2::set(100, sense); //100us period
  MsTimer2::start();
}

//타이머 중지
inline void stopTimer()
{
  bInMission = false;
  MsTimer2::stop();
  iTimeCount = 0;
}


//Stops for a designated amount of time 
//specifiedTime == 10 means specifiedTime = 1 second
void timedStop(int specifiedTime)
{
  setTimer();
  while(iTimeCount <= specifiedTime) 
  { 
    compute_steering = 0;
    compute_speed = 0;
    nextMove();
  }
  stopTimer();
}




//정해진 시간동안 주행하며 센서와 물체간 거리 유지 - 초음파센서 값 필요 없으면 ultrasonicSensor = 0 으로 설정
//Drives for a designated amount of time. Response to ultrasonic and IR sensors included
//ultrasonic sensor ==>    -1 : Left, 0 : Center, 1 : Right, null : none
inline void timedDrive(float rightLeft, float forwardBack, int specifiedTime, int sensorPosition, bool senseLine, int caliConstant)
{
  setTimer(); 
  
  //주어진 시간동안 함수 실행
  while(iTimeCount <= specifiedTime)
  {
    compute_steering = rightLeft;
    compute_speed = forwardBack;

    //거리두기
    calibrate(sensorPosition, caliConstant);

    //IR센서를 이용해 라인 벗어나지 않도록
    if(senseLine == true) 
    {
      lineSensed();
    }   
    nextMove();
  }
  
  stopTimer(); 
}


//특정 센서가 정해진 거리에 닿으면 정지
//when parking using ultrasonic sensor
//ultrasonicSensor::: -1 : Left
//closerThan::: True : closer than specified position -> stops vehicle 
//              False : Further than specified Position -> stops vehicle
void distancedDrive(float rightLeft, float forwardBack, int sensorPosition, bool untilCloser, int specifiedDistance)
{
  setTimer();
  int stopDistance = 1000;  
  
  //move until specified distance
  while (1)
  {
    if(sensorPosition == -1) stopDistance = gfLeftDistance;
    else if(sensorPosition == 0) stopDistance = gfCenterDistance;
    else if(sensorPosition == 1) stopDistance = gfRightDistance;
    
    compute_steering = rightLeft;
    compute_speed = forwardBack;

    if(untilCloser == true)
    {
      if(stopDistance < specifiedDistance) 
        break;
    }
    else //until further
    {
      if(stopDistance > specifiedDistance) 
        break;
    }

    calibrate(sensorPosition * -1, 110);
    
    //when line sensed
    lineSensed();

//    //Obtain array containing slope information
//    faLeftSlope[cnt] = gfLeftDistance;
//    faRightSlope[cnt] = gfRightDistance;
//    cnt++;
    
    nextMove();
  }

  stopTimer();
}


//후진주차 전용 후진로직
void reverse(bool bWillSenseIR, int sensorPosition)
{
  int parkingTimer = 0;
  setTimer(); //센싱 및 타이밍 시작

  //정지선 다다를때까지 후진
  //move backwards until line is sensed by both sensors
  while(gbLeftIR == detect_ir || gbRightIR == detect_ir)
  {
    compute_steering = 0;
    compute_speed = -0.4; 

    //초음파센서 값 읽어 후진할 때 거리 유지 
    calibrate(sensorPosition, 100);
    
    //선 밟았을 때
    if(bWillSenseIR == true) 
    {
     //when line sensed
     lineSensed();
    }

    //주차장 벽 양쪽 센싱 
    if(gfLeftDistance < 200 && gfRightDistance < 200) 
    {
      parkingTimer++;

      //센싱 후 약 2초간 더 후진 후 멈춤
      if(parkingTimer >= 2000){
        compute_steering = 0;
        compute_speed = 0;
        nextMove();
        break;
      } 
    } 
    nextMove();
  } 
  stopTimer();
}
