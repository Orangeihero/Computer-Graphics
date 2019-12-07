#define GLUT_DISABLE_ATEXIT_HACK
#include<stdio.h>
#include<math.h>
#include<gl/glut.h>

const GLfloat PI = 3.1415926536f;  //定义圆周率

void display();  //绘制五星红旗
void DrawStar(GLfloat cx, GLfloat cy, GLfloat mx, GLfloat my);  //其中cx，cy为五角星中心的横坐标和纵坐标，mx，my为五角星其中一个顶点的坐标

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);  //对glut函数库进行初始化
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);  //指定glutCreateWindow函数将要创建的窗口显示模式 RGB模式 双缓冲
	glutInitWindowPosition(100, 100);  //设置窗口位置，窗口的左上角相对于整个屏幕的位置
	glutInitWindowSize(400, 400);  //设置窗口大小（有可能被其它窗口覆盖）
	glutCreateWindow("五星红旗");  //设置窗口标题
	glutDisplayFunc(display);  //注册绘制回调函数，指定当窗口内容需要重绘时要调用的函数
	glutMainLoop();  //glut事件处理循环
	return 0;

}

void DrawStar(GLfloat cx, GLfloat cy, GLfloat mx, GLfloat my)  //其中cx，cy为五角星中心的横坐标和纵坐标，mx，my为五角星其中一个顶点的坐标
{
	int i;
	const GLfloat sin_72 = sin(PI * 0.4), cos_72 = cos(PI * 0.4);  //计算得到sin72°，cos72°的值
	const GLfloat sin_36 = sin(PI * 0.2), cos_36 = cos(PI * 0.2);  //计算得到sin36°，cos36°的值
	GLfloat long_length = sqrt((mx - cx) * (mx - cx) + (my - cy) * (my - cy));  //计算得到外点距离五角星中心的距离（半径）
	GLfloat sin_original = (my - cy) / long_length, cos_original = (mx - cx) / long_length;  //计算得到（mx,my）的初始点极坐标的sin，cos值
	GLfloat short_length = long_length * cos_72 / cos_36;  //计算得到内点距离五角星中心的距离（半径）

	GLfloat point[11][2];  //用来储存点的坐标
	GLfloat sin_temp, cos_temp;

	point[0][0] = mx, point[0][1] = my;
	point[10][0] = cx, point[10][1] = cy;

	//将外点的坐标存入数组中，存入偶数下标
	for (i = 1; i <= 4; i++) {
		sin_temp = sin_original * cos_72 + cos_original * sin_72;  //计算出旋转后的外点极坐标sin值
		cos_temp = cos_original * cos_72 - sin_original * sin_72;  //计算出旋转后的外点极坐标sin值
		point[2 * i][0] = cx + long_length * cos_temp;  //计算出旋转后的外点横坐标
		point[2 * i][1] = cy + long_length * sin_temp;  //计算出旋转后的外点纵坐标
		sin_original = sin_temp;  //改变原始点的sin值
		cos_original = cos_temp;  //改变原始点的cos值
	}

	sin_original = (my - cy) / long_length;
	cos_original = (mx - cx) / long_length;

	//将内点的坐标存入数组中，存入奇数下标
	for (i = 1; i <= 5; i++) {
		if (i == 1) {
			sin_temp = sin_original * cos_36 + cos_original * sin_36;  //计算出旋转后的内点极坐标sin值
			cos_temp = cos_original * cos_36 - sin_original * sin_36;  //计算出旋转后的内点极坐标sin值
		}
		else {
			sin_temp = sin_original * cos_72 + cos_original * sin_72;  //计算出旋转后的内点极坐标sin值
			cos_temp = cos_original * cos_72 - sin_original * sin_72;  //计算出旋转后的内点极坐标sin值
		}
		point[2 * i - 1][0] = cx + short_length * cos_temp;  //计算出旋转后的内点横坐标
		point[2 * i - 1][1] = cy + short_length * sin_temp;  //计算出旋转后的内点纵坐标
		sin_original = sin_temp;  //改变原始点的sin值
		cos_original = cos_temp;  //改变原始点的cos值
	}

	//设置黄色
	glColor3f(1, 1, 0);

	//绘制十个三角形
	glBegin(GL_TRIANGLES);
	for (i = 0; i < 10; i++) {
		glVertex2fv(point[i % 10]);
		glVertex2fv(point[(i + 1) % 10]);
		glVertex2fv(point[10]);
	}
	glEnd();
}

void display()  //绘制五星红旗
{
	glClear(GL_COLOR_BUFFER_BIT);  //清除颜色缓存

	//绘制红旗
	glColor3f(1, 0, 0);  //设置红色
	glBegin(GL_QUADS);
	glVertex2f(-0.9, 0.6);
	glVertex2f(0.9, 0.6);
	glVertex2f(0.9, -0.6);
	glVertex2f(-0.9, -0.6);
	glEnd();

	//绘制五角星
	DrawStar(-0.60, 0.30, -0.60, 0.48);
	DrawStar(-0.30, 0.48, -0.24, 0.48);
	DrawStar(-0.18, 0.36, -0.24, 0.36);
	DrawStar(-0.18, 0.18, -0.18, 0.24);
	DrawStar(-0.30, 0.06, -0.24, 0.06);

	glutSwapBuffers();  //交换缓冲区

}





