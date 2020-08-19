/* 평행주차 
 * Used Functions:
 * void timedStop(int specifiedTime)
 * void timedDrive(int rightLeft, float forwardBack, int specifiedTime, bool enableIR)
 * void park(int rightLeft, float forwardBack, int specifiedDistance, bool enableIR)
 * 
 * Remember 
 * forwardBack == 1 -> 100% speed in forward direction
 * specifiedTime == 10 -> 1 second 
 * specifiedDistance == 120 -> 12CM
 */

void parallelParking()
{   
  //정지 1초
  timedStop(10);

  //왼쪽으로 꺾어 선에 붙은 후 직진
  timedDrive(-1, 0.4, 1, true);
  timedDrive(0, 1, 13, true);

  //우회전, 후진
  timedDrive(1, -1, 10, true);
  
  //좌회전, 후진 
  park(-1, -1, 130, false);

  //좌회전, 주차장 탈출
  timedDrive(-1, 1, 8, true);

  //우회전
  timedDrive(1, 1, 5, true);
}
