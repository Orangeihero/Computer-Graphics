#include <stdlib.h>
#include<Windows.h>
#include<stdio.h>
#include "gl/glut.h"

#define BITMAP_ID 0x4D42
#define Height 16
#define Width 16

GLubyte image[Height][Width][3];// 图像数据
GLuint texture[4];

float fTranslate;
float fRotate;
float fScale = 1.0f;
bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

int teapot_tex = 0;
int table_tex = 1;

// 纹理标示符数组，保存两个纹理的标示符
// 描述: 通过指针，返回filename 指定的bitmap文件中数据。
// 同时也返回bitmap信息头.（不支持-bit位图）

// 读纹理图片
unsigned char *loadBitmapFile(const char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;// 文件指针
	BITMAPFILEHEADER bitmapFileHeader;// bitmap文件头
	unsigned char* bitmapImage;// bitmap图像数据
	int imageIdx = 0;// 图像位置索引
	unsigned char tempRGB;// 交换变量

	// 以“二进制+读”模式打开文件filename 
	// 需要用到fopen_s
	errno_t err;
	err = fopen_s(&filePtr, filename, "rb");
	if (filePtr == NULL) {
		printf("file not open\n");
		return NULL;
	}
	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}
	// 读入bitmap信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// 为装载图像数据创建足够的内存
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// 验证内存是否创建成功
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// 确认读入成功
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}
	// 由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// 关闭bitmap图像文件
	fclose(filePtr);
	return bitmapImage;
}

// 加载纹理的函数
void texLoad(int i, const char * filename)
{
	unsigned char* bitmapData;// 纹理数据
	BITMAPINFOHEADER bitmapInfoHeader;// bitmap信息头

	if (filename == "Spot.bmp") {
		BITMAPINFOHEADER bitmapInfoHeader1;// bitmap信息头
		unsigned char* bitmapData1;
		unsigned char* bitmapData2;
		int imageIdx = 0;
		bitmapData1 = loadBitmapFile(filename, &bitmapInfoHeader);
		bitmapData2 = loadBitmapFile("Crack.bmp", &bitmapInfoHeader1);

		bitmapData = new unsigned char[bitmapInfoHeader.biSizeImage];
		for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx++) {
			bitmapData[imageIdx] = bitmapData1[imageIdx] * 0.5 + bitmapData2[imageIdx] * 0.5;
		}
	}
	else {
		bitmapData = loadBitmapFile(filename, &bitmapInfoHeader);
	}

	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0,//mipmap层次(通常为，表示最上层) 
		GL_RGB,//我们希望该纹理有红、绿、蓝数据
		bitmapInfoHeader.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapData);//bitmap数据指针  
}


// 定义纹理的函数
void initTexture()
{
	glGenTextures(4, texture); // 第一参数是需要生成标示符的个数, 第二参数是返回标示符的数组
	texLoad(0, "Monet.bmp");
	texLoad(1, "Crack.bmp");
	texLoad(3, "Spot.bmp");

	// 下面生成自定义纹理

	// 生成红黑相间的图像
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			int x;
			if ((i < 4 || (i >= 8 && i < 12)) && (j < 4 || (j >= 8 && j < 12)) || (i >= 4 && i < 8 || (i >= 12 && i < 16)) && (j >= 4 && j < 8 || (j >= 12 && j < 16))) x = 0;
			else x = 255;
			image[i][j][0] = (GLubyte)x;
			image[i][j][1] = 0;
			image[i][j][2] = 0;
		}
	}
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //设置像素存储模式控制所读取的图像数据的行对齐方式.
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大过滤，线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//缩小过滤，线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S方向重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//T方向重复
}


void drawCube() {
	GLfloat vertex[8][3] = { // 长方体的八个顶点
		{ 0.5, 0.5, 0.5 },
		{ 0.5, 0.5, -0.5 },
		{ -0.5, 0.5, -0.5 },
		{ -0.5, 0.5, 0.5 },
		{ 0.5, -0.5, 0.5 },
		{ 0.5, -0.5, -0.5 },
		{ -0.5, -0.5, -0.5 },
		{ -0.5, -0.5, 0.5 }
	};

	GLint flat[6][4] = { // 长方体六个面的四个顶点
		{ 0, 1, 2, 3 },
		{ 4, 5, 6, 7 },
		{ 0, 1, 5, 4 },
		{ 2, 3, 7, 6 },
		{ 3, 0, 4, 7 },
		{ 1, 2, 6, 5 }
	};

	GLint textcoor[4][2] = {
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },
		{ 0, 1 }
	};

	glBegin(GL_QUADS); // 设置正方形绘制模式
	int i, j;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 4; j++) {
			glTexCoord2iv(textcoor[j]);
			glVertex3fv(vertex[flat[i][j]]);
		}
	}
	glEnd();
}

void drawLeg()
{
	glScalef(1, 1, 3);
	drawCube();
}

void drawTable() // 桌子
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[teapot_tex]);//选择纹理texture[status]

	glPushMatrix();
	glTranslatef(0, 0, 4 + 1);
	glRotatef(90, 1, 0, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//设置纹理受光照影响
	glutSolidTeapot(1);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);//关闭纹理texture[status]

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[table_tex]);//选择纹理texture[status]
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//设置纹理不受光照影响

	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	drawLeg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	drawLeg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	drawLeg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	drawLeg();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D); //关闭纹理texture[status2]
}

void updateView(int width, int height)
{
	glViewport(0, 0, width, height);//设置视窗大小  

	glMatrixMode(GL_PROJECTION);//设置矩阵模式为投影   
	glLoadIdentity();//初始化矩阵为单位矩阵      

	float whRatio = (GLfloat)width / (GLfloat)height;//设置显示比例    
	if (bPersp) 
		gluPerspective(45.0f, whRatio, 0.1f, 100.0f); //透视投影      
	else 
		glOrtho(-3, 3, -3, 3, -100, 100);//正投影  

	glMatrixMode(GL_MODELVIEW);//设置矩阵模式为模型  
}

void reshape(int width, int height)
{
	if (height == 0)//如果高度为0  
	{
		height = 1;//让高度为1（避免出现分母为0的现象）  
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth); //更新视角  
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = { 0, 0, 8 };
float center[] = { 0, 0, 0 };

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; break; }

	case ' ': {bAnim = !bAnim; break; }
	case 'o': {bWire = !bWire; break; }

	case 'a': { // 左移
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
	}
	case 'd': { // 右移
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
	}
	case 'w': { // 上移
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
	}
	case 's': { // 下移
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
	}
	case 'z': { // 前移
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
	}
	case 'c': { // 后移
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
	}
	case 't': { // 改变茶壶纹理
		if (teapot_tex == 0) teapot_tex = 2;
		else if(teapot_tex == 2) teapot_tex = 0;
		break;
	}
	case 'r': { // 改变桌子纹理
		if (table_tex == 1) table_tex = 3;
		else if (table_tex == 3) table_tex = 1;
		break;
	}
	}

	updateView(wHeight, wWidth);
}


void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓存和深度缓存   
	glLoadIdentity();//初始化矩阵为单位矩阵    

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//设置多边形绘制模式：正反面，线型  
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//设置多边形绘制模式：正反面，填充      
	}

	glEnable(GL_DEPTH_TEST);//开启深度测试    
	glEnable(GL_LIGHTING);//开启光照模式   

	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { 5,5,5,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);//光源位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);//定义白色 
	glEnable(GL_LIGHT0);//开启第0号光源   

	glRotatef(fRotate, 0, 1.0f, 0);//旋转
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);//缩放
	drawTable();//绘制场景

	if (bAnim) fRotate += 0.5f;//旋转因子改变
	glutSwapBuffers(); //交换缓冲区
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	initTexture();
	glutMainLoop();
	return 0;
}




