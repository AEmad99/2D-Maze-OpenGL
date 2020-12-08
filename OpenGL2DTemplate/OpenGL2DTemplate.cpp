#include <stdio.h>
#include <math.h>
#include <random>
#include <string.h>
#include <string>
#include <ctime>
#include <Windows.h>
#include <MMSystem.h>
#include <glut.h>

// maze
int addedLanes = 1;
int lanes = 0;
int borderNumbers = 0;
double bordersArr[7];
double gapsArr[7];
int gapW = 60;
int gapH = 15;
int laneSize = 0;
int borderSize = 10;
int firstX = 50;
int lastX = 750;
int firstY = 50;
int lastY = 750;
// player attributes
int playerSize = 10;
int playerSpeed = 3;
double xPos = 400;
double yPos = 70;
int direction = 0;
// score and time
int score = 0;
int timeRem = 60;
// coins
double coinArrX[7];
double coinArrY[7];
int coinSize = 15;
double rotDeg = 0;
// power ups
double xArr[2];
double yArr[2];
int powerupSize = 10;
int speedUp = 8;
int speedUpTime = 0;
bool taken = false;
// synchronization
int tik = 1;
// game ending
bool end = false;
bool win = false;
char* winner = "";
char* description = "Control the little mouse to eat cheese coins and reach your hole in the wall";

void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}

	glutPostRedisplay();
}

void Animation() {
	//coin rotation
	rotDeg+= 0.5;

	//coin collection
	for (int j = 0; j < borderNumbers; j++) {
		if ((yPos <= coinArrY[j] + coinSize) && (yPos >= coinArrY[j] - coinSize)) {
			if ((xPos >= coinArrX[j] - coinSize) && (xPos <= coinArrX[j] + coinSize)) {
				coinArrX[j] = 70000;
				score++;
				PlaySound(TEXT("Coin.wav"), NULL, SND_ASYNC);
			}
		}
	}

	//power collection
	//speed up
	if ((yPos <= yArr[1] + 10) && (yPos >= yArr[1] - 10)) {
		if ((xPos >= xArr[1] - 10) && (xPos <= xArr[1] + 10)) {
			taken = true;
			playerSpeed = speedUp;
			speedUpTime = 5;
			xArr[1] = 7000000;
			PlaySound(TEXT("Speed.wav"), NULL, SND_ASYNC);
		}
	}
	//gates
	if ((yPos <= yArr[0] + 10) && (yPos >= yArr[0] - 10)) {
		if ((xPos >= xArr[0] - 10) && (xPos <= xArr[0] + 10)) {
			xArr[0] = 7000000;
			PlaySound(TEXT("Gate.wav"), NULL, SND_ASYNC);
			for (int i = 0; i < borderNumbers; i++) {
				if (!(yArr[0] > bordersArr[i])) {
					gapsArr[i] = xPos - 30;
					break;
				}
			}
		}
	}
	//collisions
	//edges
	if (yPos + 10 > 750) {
		PlaySound(TEXT("Wall2.wav"), NULL, SND_ASYNC);
		yPos = 740;
	}
	if (yPos - 10 < 50) {
		PlaySound(TEXT("Wall2.wav"), NULL, SND_ASYNC);
		yPos = 60;
	}
	if (xPos + 10 > 750) {
		PlaySound(TEXT("Wall2.wav"), NULL, SND_ASYNC);
		xPos = 740;
	}
	if (xPos - 10 < 50) {
		PlaySound(TEXT("Wall2.wav"), NULL, SND_ASYNC);
		xPos = 60;
	}

	//lane borders
	for (int i = 0; i < borderNumbers; i++) {
		if ((yPos + 10 > bordersArr[i]) && (yPos + 10 < bordersArr[i]+10)) {
			if ((xPos + 10 < gapsArr[i] + gapW) && (xPos - 10 > gapsArr[i])) {

			}
			else {
				PlaySound(TEXT("Wall2.wav"), NULL, SND_ASYNC);
				yPos = bordersArr[i]-10;
			}
		}
		if ((yPos - 10 < bordersArr[i]+10) && (yPos - 10 > bordersArr[i])) {
			if ((xPos + 10 < gapsArr[i] + gapW) && (xPos - 10 > gapsArr[i])) {

			}
			else {
				PlaySound(TEXT("Wall2.wav"), NULL, SND_ASYNC);
				yPos = bordersArr[i] + 20;
			}
		}
		if ((yPos > bordersArr[i]) && (yPos < bordersArr[i]+10)) {
			if (!((xPos + 10 < gapsArr[i]+ gapW) && (xPos - 10 > gapsArr[i]))) {
				PlaySound(TEXT("Wall2.wav"), NULL, SND_ASYNC);
				yPos = bordersArr[i]-10;
			}
		}
	}

	// check if the game ended with a win
	if ((xPos >= 360) && (xPos <= 400)) {
		if (yPos <= 720 && yPos >= 680) {
			win = true;
			end = true;
		}
	}

	glutPostRedisplay();
}

void Timer(int value) {
	if (tik % 7 == 0 && end == false) {
		// update the location of gaps
		for (int i = 0; i < borderNumbers; i++) {

			gapsArr[i] = rand() % 640 + 50;
		}
		glutPostRedisplay();
		glutTimerFunc(0.00001 * 1000, Timer, 0);
	}
	else {
		if (timeRem > 0 && end == false) {
			timeRem--;
		}
		if (timeRem > 0 && end == false && taken == true) {
			speedUpTime--;
		}
		if (speedUpTime == 0) {
			playerSpeed = 3;
			taken = false;
		}

		if (timeRem == 0) {
			win == false;
			end = true;
		}

		glutPostRedisplay();
		glutTimerFunc(1 * 1000, Timer, 0);
	}

	if (tik % 12 == 0 && end == false) {
		for (int q = 0; q < 2; q++) {
			xArr[q] = rand() % 640 + 70;
			yArr[q] = 50 + ((rand() % borderNumbers ) * laneSize) + laneSize / 2;
			glutPostRedisplay();
		}
	}

	if (end == true) {
		if (win == false) {
			PlaySound(TEXT("Lost.wav"), NULL, SND_ASYNC);
		}
		else {
			PlaySound(TEXT("Win.wav"), NULL, SND_ASYNC);
		}
	}

	tik++;

	

}

void Key(unsigned char key, int x, int y)
{
	if (end == false) {
		// pressing a directional key for the player
		if (key == 'w')
		{
			yPos += playerSpeed;
			direction = 0;
		}

		if (key == 'a')
		{
			direction = 90;
			xPos -= playerSpeed;

		}

		if (key == 's')
		{
			yPos -= playerSpeed;
			direction = 180;
		}

		if (key == 'd')
		{
			xPos += playerSpeed;
			direction = 270;
		}


		glutPostRedisplay();
	}
}

// used to draw line strips
void drawLineShape(double x, double y, int n, int length)
{
	while (n > 0) {
		glBegin(GL_LINE_STRIP);
		glVertex2f(x, y);
		glVertex2f(x + length, y);
		y++;
		n--;
		glEnd();
	}
}

// used to draw rectangles 
void drawRect(double x, double y, int w, int h)
{
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}

// used to draw circles
void drawCircle(double x, double y, float r)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.5, 0.5, 0.5);

	// this is the border
	glPushMatrix();
	glColor3f(0.2, 0.5, 0.8);
	drawRect(0, 0, 50, 800);
	drawLineShape(50, 750, 50, 700);
	drawRect(750, 0, 50, 800);
	drawLineShape(50, 0, 50, 700);
	glPopMatrix();

	// lane borders
	for (int i = 1; i <= borderNumbers; i++) {
		glPushMatrix();
		//lane border
		glColor3f(1, 0, 0);
		drawLineShape(50, 50 + (laneSize * i), borderSize, 700);
		bordersArr[i - 1] = 50 + (laneSize * i);
		//bridges
		glColor3f(0, 0, 0);
		drawRect(gapsArr[i-1], (50 + (laneSize * i))-2 ,gapW,gapH);
		glPopMatrix();
	}

	// player
	//body
	glPushMatrix();
	glPushMatrix();
	glTranslated(xPos, yPos, 0);
	glRotated(direction, 0, 0, 1);
	glColor3f(0.69, 0.69, 0.69);
	drawCircle(0, 0, 10); // circle
	drawRect(5, 0, 5, 10); //polygon
	drawRect(-10, 0, 5, 10);
	drawLineShape(-10, 0, 10, 5); //line strip
	glColor3f(1, 0.75, 0.79);
	glBegin(GL_TRIANGLES); // triangles
	glVertex2f(5, 10);
	glVertex2f(10, 10);
	glVertex2f(7.5, 12);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(-5, 10);
	glVertex2f(-10, 10);
	glVertex2f(-7.5, 12);
	glEnd();
	glPushMatrix();
	glColor3f(1, 0.75, 0.79);
	glBegin(GL_LINES);
	glVertex2f(0, -10);
	glVertex2f(3, -12);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(3, -12);
	glVertex2f(0, -14);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0, -14);
	glVertex2f(3, -16);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(3, -16);
	glVertex2f(0, -18);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0, -18);
	glVertex2f(3, -20);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(3, -20);
	glVertex2f(0, -22);
	glEnd();
	// head
	glPushMatrix();
	glColor3f(1,1,1);
	drawCircle(5,3,3);
	drawCircle(-5, 3, 3);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS); //points4
	glColor3f(0, 0, 0);
	glVertex2f(5, 3);
	glVertex2f(-5, 3);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//coin spawning
	for (int j = 0; j < borderNumbers; j++) {
		glPushMatrix();
		glTranslated(coinArrX[j], coinArrY[j], 0);
		glRotated(rotDeg, 0, 0, 1);
		glColor3f(0.38, 0.78, 0.69);
		drawCircle(0, 0, coinSize);
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(0, 14.2);
		glVertex2f(12.2, 0);
		glVertex2f(-10.2, -10.2);
		glEnd();
		glColor3f(1,1,0);
		glBegin(GL_TRIANGLES);
		glVertex2f(0,14);
		glVertex2f(12,0);
		glVertex2f(-10,-10);
		glEnd();
		glColor3f(0, 0, 0);
		drawCircle(4, 5, 1.6);
		drawCircle(0, -2, 1.6);
		drawCircle(5, 1, 1.6);
		glColor3f(0.38, 0.78, 0.69);
		drawCircle(4, 5, 1.5);
		drawCircle(0, -2, 1.5);
		drawCircle(5, 1, 1.5);
		glPopMatrix();
	}

	//for powerups break them up for each power up
	// gate power
	glPushMatrix();
	glColor3f(0.38, 0.78, 0.69);
	drawCircle(xArr[0], yArr[0], 10);
	glColor3f(0.71, 0.42, 0.42);
	glBegin(GL_LINES);
	glVertex2f(xArr[0]-3,yArr[0]);
	glVertex2f(xArr[0]+3,yArr[0]);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(xArr[0] - 3,yArr[0]+4);
	glVertex2f(xArr[0] - 3,yArr[0]-4);
	glVertex2f(xArr[0]-6,yArr[0]);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(xArr[0] + 3, yArr[0] + 4);
	glVertex2f(xArr[0] + 3, yArr[0] - 4);
	glVertex2f(xArr[0] + 6, yArr[0]);
	glEnd();
	glPopMatrix();
	//speed power
	glPushMatrix();
	glColor3f(0.38, 0.78, 0.69);
	drawCircle(xArr[1] + 5, yArr[1] + 8, 10);
	glColor3f(0.71, 0.42, 0.42);
	drawRect(xArr[1]+2.5, yArr[1], powerupSize-5, powerupSize);
	glBegin(GL_TRIANGLES);
	glVertex2f(xArr[1] - 2, yArr[1]+powerupSize);
	glVertex2f(xArr[1] + powerupSize + 2, yArr[1]+powerupSize);
	glVertex2f((xArr[1]+xArr[1]+powerupSize)/2,yArr[1]+10+powerupSize);
	glEnd();
	glPopMatrix();

	//display powerups
	if ((taken == true) & (end == false)) {
		glColor3f(1, 0.87, 0);
		glPushMatrix();
		char* power[20];
		sprintf((char*)power, "Speed for: %d", speedUpTime);
		print(350, 775, (char*)power);
		glPopMatrix();
	}
	
	// score and time
	glPushMatrix();
	glColor3f(1, 1, 1);
	char* t[20];
	sprintf((char*)t, "Time: %d", timeRem);
	print(100, 775, (char*)t);
	char* s[20];
	sprintf((char*)s, "Score: %d", score);
	print(250, 775, (char*)s);
	glPopMatrix();

	//this is for the end screen
	if (end == true) {
		if (win == false) {
			winner = "Sadly.. You Lost";
			print(550, 775, (char*)winner);
		}
		else {
			winner = "CongRATs! You Win";			print(550, 775, (char*)winner);
		}
	}

	//this is the goal
	// the goal
	glPushMatrix();
	glColor3f(0.420, 0.69, 0.720);
	drawRect(360, 680, 40, 40);
	glBegin(GL_TRIANGLES);
	glVertex2f(360,720);
	glVertex2f(400,720);
	glVertex2f(380,730);
	glEnd();
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2d(380, 730);
	glVertex2d(360, 720);
	glVertex2d(360, 680);
	glVertex2d(400, 680);
	glVertex2d(400, 720);
	glEnd();
	glColor3f(0.647, 0.1647, 0.1647);
	drawRect(370, 680, 20, 20);
	drawCircle(380, 700, 10);
	glPopMatrix();

	glPushMatrix();
	print(35, 25, description);
	glPopMatrix();

	glFlush();
}

void main(int argc, char** argr) {
	srand(time(NULL));
	glutInit(&argc, argr);
	lanes = rand() % addedLanes + 5;
	borderNumbers = lanes - 1;
	laneSize = (700 / lanes);
	for (int j = 0; j < borderNumbers; j++) {
		double random = rand() % 640 + 50;
		gapsArr[j] = random;
	}

	//random coins
	for (int k = 0; k < borderNumbers; k++) {
		double random = rand() % 640 + 70;
		coinArrX[k] = random;
	}
	for (int z = 0; z < borderNumbers; z++) {
		double random = 50 + ((rand() % borderNumbers) * laneSize) + laneSize/2;
		coinArrY[z] = random;
	}

	//random power ups
	for (int y = 0; y < 2; y++) {
		double random = rand() % 640 + 70;
		xArr[y] = random;
	}
	for (int u = 0; u < 2; u++) {
		double random = 50 + ((rand() % borderNumbers)*laneSize) + laneSize / 2;
		yArr[u] = random;
	}
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Mouse Eating Cheese Coins");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Key);
	glutTimerFunc(0, Timer, 0);
	glutIdleFunc(Animation);
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, 800, 0.0, 800);

	glutMainLoop();
}
