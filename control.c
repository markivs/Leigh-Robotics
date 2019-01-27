#pragma config(Motor,  port2, frontRight, tMotorNormal, openLoop)
#pragma config(Motor,  port10, backRight, tMotorNormal, openLoop)
#pragma config(Motor,  port3, frontLeft, tMotorNormal, openLoop)
#pragma config(Motor,  port4, backLeft, tMotorNormal, openLoop)
#pragma config(Motor,  port6, shoulder, tMotorNormal, openLoop)
#pragma config(Motor,  port5, elbow, tMotorNormal, openLoop)
#pragma config(Motor,  port7, claw, tMotorNormal, openLoop)
#pragma config(Motor,  port1, table, tMotorNormal, openLoop)
#pragma config(Motor,  port8, armBackRight, tMotorNormal, openLoop)
#pragma config(Motor,  port9, armBackLeft, tMotorNormal, openLoop)
// assign remote values to function variables
int lateralX  = Ch4;
int lateralY = Ch3;
int rotational = Ch1;
int shoulderMove = Ch2;
int elbowMoveUp = Btn5U;
int elbowMoveDown = Btn5D;
int clawMoveCL = Btn7D;
int clawMoveOP = Btn7U;
int turnTableRight = Btn6U; 
int turnTableLeft = Btn6D;
int armUp = Btn8U;
int armDown = Btn8D;

void moveRotateCheck(int latX, int latY, int rotation)
{
	int threshold = 5;

	if(abs(rotation) < threshold)
		rotation = 0;
	if(abs(latX) < threshold)
		latX = 0;
	if(abs(latY) < threshold)
		latY = 0;
// to move right
// tr = cc, br = c, tl = cc, c
	motor[frontLeft] = latY - rotation + latX; 
	motor[frontRight] = -latY - rotation + latX;
	motor[backLeft] =  latY - rotation - latX;
	motor[backRight] = latY - rotation - latX;
}
void shoulderCheck(int pos)
{
		int threshold = 5;

		if(abs(pos) < threshold)
			pos = 0;
		motor[shoulder] = -pos;
}
void elbowCheck(int up, int down)
{
	if(up == 1)
		motor[elbow] = 100;
	else if(down == 1)
		motor[elbow] = -100;
	else
		motor[elbow] = 0;
}

void clawCheck(int open, int close)
{
	if(open == 1)
		motor[claw] = 100;
	else if(close == 1)
		motor[claw] = -100;
	else
		motor[claw] = 0;
}
void tableCheck(int left, int right)
{
	if(left == 1)
		motor[table] = 50;
	else if(right == 1)
		motor[table] = -50;
	else
		motor[table] = 0;
}
void bigArmCheck(int up, int down)
{
	if(up == 1)
	{
		motor[armBackLeft] = 100;
		//motor[armFrontLeft] = 100;
		motor[armBackRight] = -100;
		//motor[armFrontRight] = -100;
	}
	else if(down == 1)
	{
		motor[armBackLeft] = -100;
		//motor[armFrontLeft] = -100;
		motor[armBackRight] = 100;
		//motor[armFrontRight] = 100;
	}
	else
	{
		motor[armBackLeft] = 0;
		//motor[armFrontLeft] = 0;
		motor[armBackRight] = 0;
		//motor[armFrontRight] = 0;
	}
}

void rotate(int amt)
{
	moveMotorTarget(frontLeft, amt, 50, true);
	moveMotorTarget(backLeft, amt, 50, true);
	moveMotorTarget(frontRight, amt, -50, true);
	moveMotorTarget(backRight, amt, -50, true);
	waitUntilMotorStop(frontLeft);
	waitUntilMotorStop(backLeft);
	waitUntilMotorStop(frontRight);
	waitUntilMotorStop(backRight);
}
void moveDist(int distance)
{
	moveMotorTarget(frontLeft, distance, 50, true);
	moveMotorTarget(backLeft, distance, 50, true);
	moveMotorTarget(frontRight, distance, 50, true);
	moveMotorTarget(backRight, distance, 50, true);
	waitUntilMotorStop(frontLeft);
	waitUntilMotorStop(backLeft);
	waitUntilMotorStop(frontRight);
	waitUntilMotorStop(backRight);
}

void takeMobile()
{
	int armTravelDist = 10;
	moveMotorTarget(armBackRight, armTravelDist, -100, true);
	moveMotorTarget(armBackLeft, armTravelDist, -100, true);
}
void giveMobile()
{
	int armTravelDist = 10;
	moveMotorTarget(armBackRight, armTravelDist, 100, true);
	moveMotorTarget(armBackLeft, armTravelDist, 100, true);
}
void moveClaw(int clawDist)
{
	motor[claw] = 100;
	moveMotorTarget(claw, clawDist, 100, false); 
}
void moveShoulder(int throttle)
{
	motor[shoulder] = throttle;
}
void moveElbow(int elbowDist)
{
	moveMotorTarget(elbow, elbowDist, 100, true); 
}
void takeCone()
{
	moveClaw(100); //hold shoulder at height, open claw
	moveDist(2);
moveClaw(-100); //move forward a little, close claw
}

void placeConeMobile()
{
	moveShoulder(50);
	moveElbow(-5); //move shoulder up, bend elbow down (above mobile goal)
	moveClaw(10); //release claw
	moveClaw(-10); //reset
	moveElbow(5);
	moveShoulder(0);
}
void autonomous()
{
//	takeCone();
	moveDist(360000);
/*	takeMobile();
	placeConeMobile();
	moveDist(-36);
	rotate(180);
	moveDist(5);
	giveMobile();
	moveDist(-5);*/
}
task main()
{
	waitUntil(vexRT[Btn6D] ==1 && vexRT[Btn6U]==1);
	motor[claw] = 100;
	wait1Msec(1000);
	autonomous();

	while(!(vexRT[Btn7L] == 1 && vexRT[Btn8R]==1 && vexRT[Btn5U]==1 && vexRT[Btn5D]==1 && vexRT[Btn6U]==1 && vexRT[Btn6D]==1))
	{
		moveRotateCheck(vexRT[lateralX], vexRT[lateralY], vexRT[rotational]);
		shoulderCheck(vexRT[shoulderMove]);
		elbowCheck(vexRT[elbowMoveUp], vexRT[elbowMoveDown]);
		clawCheck(vexRT[clawMoveOP], vexRT[clawMoveCL]);
		tableCheck(vexRT[turnTableLeft], vexRT[turnTableRight]);
		bigArmCheck(vexRT[armUp], vexRT[armDown]);
	}
}

