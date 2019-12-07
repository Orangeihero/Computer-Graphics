#define GLUT_DISABLE_ATEXIT_HACK
#include<stdio.h>
#include<math.h>
#include<gl/glut.h>

const GLfloat PI = 3.1415926536f;  //����Բ����

void display();  //�������Ǻ���
void DrawStar(GLfloat cx, GLfloat cy, GLfloat mx, GLfloat my);  //����cx��cyΪ��������ĵĺ�����������꣬mx��myΪ���������һ�����������

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);  //��glut��������г�ʼ��
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);  //ָ��glutCreateWindow������Ҫ�����Ĵ�����ʾģʽ RGBģʽ ˫����
	glutInitWindowPosition(100, 100);  //���ô���λ�ã����ڵ����Ͻ������������Ļ��λ��
	glutInitWindowSize(400, 400);  //���ô��ڴ�С���п��ܱ��������ڸ��ǣ�
	glutCreateWindow("���Ǻ���");  //���ô��ڱ���
	glutDisplayFunc(display);  //ע����ƻص�������ָ��������������Ҫ�ػ�ʱҪ���õĺ���
	glutMainLoop();  //glut�¼�����ѭ��
	return 0;

}

void DrawStar(GLfloat cx, GLfloat cy, GLfloat mx, GLfloat my)  //����cx��cyΪ��������ĵĺ�����������꣬mx��myΪ���������һ�����������
{
	int i;
	const GLfloat sin_72 = sin(PI * 0.4), cos_72 = cos(PI * 0.4);  //����õ�sin72�㣬cos72���ֵ
	const GLfloat sin_36 = sin(PI * 0.2), cos_36 = cos(PI * 0.2);  //����õ�sin36�㣬cos36���ֵ
	GLfloat long_length = sqrt((mx - cx) * (mx - cx) + (my - cy) * (my - cy));  //����õ���������������ĵľ��루�뾶��
	GLfloat sin_original = (my - cy) / long_length, cos_original = (mx - cx) / long_length;  //����õ���mx,my���ĳ�ʼ�㼫�����sin��cosֵ
	GLfloat short_length = long_length * cos_72 / cos_36;  //����õ��ڵ������������ĵľ��루�뾶��

	GLfloat point[11][2];  //��������������
	GLfloat sin_temp, cos_temp;

	point[0][0] = mx, point[0][1] = my;
	point[10][0] = cx, point[10][1] = cy;

	//������������������У�����ż���±�
	for (i = 1; i <= 4; i++) {
		sin_temp = sin_original * cos_72 + cos_original * sin_72;  //�������ת�����㼫����sinֵ
		cos_temp = cos_original * cos_72 - sin_original * sin_72;  //�������ת�����㼫����sinֵ
		point[2 * i][0] = cx + long_length * cos_temp;  //�������ת�����������
		point[2 * i][1] = cy + long_length * sin_temp;  //�������ת������������
		sin_original = sin_temp;  //�ı�ԭʼ���sinֵ
		cos_original = cos_temp;  //�ı�ԭʼ���cosֵ
	}

	sin_original = (my - cy) / long_length;
	cos_original = (mx - cx) / long_length;

	//���ڵ��������������У����������±�
	for (i = 1; i <= 5; i++) {
		if (i == 1) {
			sin_temp = sin_original * cos_36 + cos_original * sin_36;  //�������ת����ڵ㼫����sinֵ
			cos_temp = cos_original * cos_36 - sin_original * sin_36;  //�������ת����ڵ㼫����sinֵ
		}
		else {
			sin_temp = sin_original * cos_72 + cos_original * sin_72;  //�������ת����ڵ㼫����sinֵ
			cos_temp = cos_original * cos_72 - sin_original * sin_72;  //�������ת����ڵ㼫����sinֵ
		}
		point[2 * i - 1][0] = cx + short_length * cos_temp;  //�������ת����ڵ������
		point[2 * i - 1][1] = cy + short_length * sin_temp;  //�������ת����ڵ�������
		sin_original = sin_temp;  //�ı�ԭʼ���sinֵ
		cos_original = cos_temp;  //�ı�ԭʼ���cosֵ
	}

	//���û�ɫ
	glColor3f(1, 1, 0);

	//����ʮ��������
	glBegin(GL_TRIANGLES);
	for (i = 0; i < 10; i++) {
		glVertex2fv(point[i % 10]);
		glVertex2fv(point[(i + 1) % 10]);
		glVertex2fv(point[10]);
	}
	glEnd();
}

void display()  //�������Ǻ���
{
	glClear(GL_COLOR_BUFFER_BIT);  //�����ɫ����

	//���ƺ���
	glColor3f(1, 0, 0);  //���ú�ɫ
	glBegin(GL_QUADS);
	glVertex2f(-0.9, 0.6);
	glVertex2f(0.9, 0.6);
	glVertex2f(0.9, -0.6);
	glVertex2f(-0.9, -0.6);
	glEnd();

	//���������
	DrawStar(-0.60, 0.30, -0.60, 0.48);
	DrawStar(-0.30, 0.48, -0.24, 0.48);
	DrawStar(-0.18, 0.36, -0.24, 0.36);
	DrawStar(-0.18, 0.18, -0.18, 0.24);
	DrawStar(-0.30, 0.06, -0.24, 0.06);

	glutSwapBuffers();  //����������

}





