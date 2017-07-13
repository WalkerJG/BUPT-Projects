#include <glut.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "glut.h"
#include <windows.h>
#pragma comment (lib, "glut32.lib")

#define checkImageWidth 64
#define checkImageHeight 64

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;

static GLfloat xrot = 0;
static GLfloat yrot = 0;

//�������ܣ����Ƶ�������ͼ��
		void makeCheckImage(void)
		{
			int i, j, c;
			float ti, tj;
			for (i = 0; i < checkImageHeight; i++) {
				ti = 2.0*3.14159265*i / checkImageHeight;
				for (j = 0; j < checkImageWidth; j++) {
					tj = 2.0*3.14159265*j / checkImageWidth;
					c = ((((i & 0x16) == 0) ^ ((j & 0x16)) == 0)) * 255;
					checkImage[i][j][0] = (GLubyte)127 * (1.0 + sin(i));
					checkImage[i][j][1] = (GLubyte)127 * (1.0 + cos(ti));
					checkImage[i][j][2] = (GLubyte)127 * (1.0 + sin(ti+tj));
					checkImage[i][j][3] = (GLubyte)255;
				}
			}

		}

//�������ܣ���ʼ��
void SetupRC(void)
{
	glEnable(GL_LIGHTING);		//���õ�Դ
	glEnable(GL_LIGHT0);		//����0�ŵ�
	glShadeModel(GL_SMOOTH);    //���ù⻬��ɫģʽ
	glEnable(GL_DEPTH_TEST);	//������Ȳ���
	glEnable(GL_NORMALIZE);		//���÷�����
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);   //Ϊɫ�ʻ�����ָ�����������ֵ

											//���ñ�����ϵ�����
	GLfloat mat_ambient[] = { 0.6f,0.6f,0.6f,1.0f };
	GLfloat mat_diffuse[] = { 0.5f,0.5f,0.5f,1.0f };
	GLfloat mat_specular[] = { 1.0f,1.0f,1.0f,1.0f, };
	GLfloat mat_shininess[] = { 40.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);		//ָ�����������ǿ��
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);		//�������ǿ��
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);		//���淴���ǿ��
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);	//���淴���Ļ��ǿ��

															//����
	makeCheckImage();						//���Ƶ�������ͼ��
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	//�������ش洢ģʽ
	glGenTextures(1, &texName);				//�����������������Ϊ1
	glBindTexture(GL_TEXTURE_2D, texName);	//������

											//�����˲���ͼ�������ͼ��ռ�ӳ�䵽֡����ͼ��ռ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
		checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
}

//�������ܣ�����ǽ��
void DrawWall(void)
{
	glPushMatrix();                //�����ջѹ��
	glTranslated(1.2, 0.01, 1.2);  //���任�����뵱ǰ�������,ʹԭ���ƶ���������ֵ���꣩ 
	glScaled(2.4, 0.02, 2.4);      //�����������뵱ǰ�������,����ǰͼ����x,y,z��ֱ�Ŵ�Ϊԭ���ģ�����ֵ����
	glutSolidCube(0.2);            //size=1.0��ʵ��������
	glPopMatrix();                 //�����ջ����
}

//�������ܣ�������������
void DrawPillar(void)
{
	glPushMatrix();
	glTranslated(0, 0.15, 0);
	glScaled(0.02, 0.3, 0.02);
	glutSolidCube(1.0);
	glPopMatrix();
}

//�������ܣ���������
void DrawTable(void)
{
	glPushMatrix();
	glTranslated(0.05, 0.3, 0.05);
	glScaled(0.6, 0.02, 0.6);
	glutSolidCube(1.0);		//��������
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.275, 0, 0.275);
	DrawPillar();			//��������
	glTranslated(0, 0, -0.45);
	DrawPillar();			//��������
	glTranslated(-0.45, 0, 0.45);
	DrawPillar();			//��������
	glTranslated(0, 0, -0.45);
	DrawPillar();			//��������
	glPopMatrix();
}

//�������ܣ���ͼ
		void RenderScene(void)
		{
			GLfloat light_position[] = { 2.0f, 6.0f, 3.0f, 0.0f };
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);  //ָ��0�Ź�Դ��λ��

			glMatrixMode(GL_PROJECTION);                       //��ͶӰ����Ӧ�����ľ������
			glLoadIdentity();			                       //����ǰ���û�����ϵ��ԭ���Ƶ�����Ļ����
			GLfloat h = 1.0;			                       //���ڵ�һ��߶�
			glOrtho(-h * 64 / 48.0, h * 64 / 48.0, -h, h, 0.1, 100.0); //����ǰ�Ŀ��ӿռ�����Ϊ��ͶӰ�ռ䣺���ң��£��ϣ�����Զ��

			glMatrixMode(GL_MODELVIEW);	                       //��ģ���Ӿ������ջӦ�����ľ������
			glLoadIdentity();
			gluLookAt(2, 1.6, 2, 0, 0.2, 0, 0, 1, 0);		//���ù۲�����ϵ
	
															//��ʼ����
			glRotatef(xrot, 1.0f, 0.0f, 0.0f);				//��ת�ᾭ��ԭ�㣬����Ϊ(1,0,0),��ת�Ƕ�Ϊxrot�������������ֶ���
			glRotatef(yrot, 0.0f, 1.0f, 0.0f);              //��ת�ᾭ��ԭ�㣬����Ϊ(0,1,0)����ת�Ƕ�Ϊyrot
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glPushMatrix();
			glTranslated(1.0, 0, 1.0);
			DrawTable();						//������
			glPopMatrix();

			glPushMatrix();
			glTranslated(0.4, 0.35, 1.7);
			glScaled(0.03, 0.7, 0.03);
			glutSolidCube(2.0);					//��������
			glPopMatrix();

			glPushMatrix();
			glTranslated(1.2, 0.4, 1.3);
			glRotated(180, 0, 200, 180);
			glutSolidCone(0.15, 0.25f, 30, 25);	//����Բ׶
			glPopMatrix();

			glPushMatrix();
			glTranslated(0.2, 0.1, 0.85);
			glScaled(0.8, 0.8, 1.0);
			glutSolidCube(0.2);		//���ƶ�����
			glPopMatrix();

			//glPushMatrix();
			//glTranslated(0.5, 0.8, 0.85);
			//glScaled(0.8, 0.8, 1.0);
			//glutWireCube(1.0);		//���ƶ�������
			//glPopMatrix();


			glPushMatrix();
			glTranslated(0.9, 0.4, 0.9);
			glutSolidSphere(0.1, 15, 50);		//��������
			glPopMatrix();


			//��������
			glEnable(GL_TEXTURE_2D);		//��������
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//ӳ�䷽ʽ

			glBegin(GL_QUADS);				//���Ƶ���
											//�����ı��β�������ǰ��������Ӧ����������
			glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
			glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, 2.4);
			glTexCoord2f(1.0, 1.0); glVertex3f(2.4, 0.0, 2.4);
			glTexCoord2f(1.0, 0.0); glVertex3f(2.4, 0.0, 0.0);
			glEnd();

			glDisable(GL_TEXTURE_2D);		//�ر�����
			glFlush();					//��ͼ�����ʾ����Ļ��
		}

//�������ܣ�������������Ϣ
		void SpecialKeys(int key, int x, int y)
		{
			switch (key) {
			case GLUT_KEY_UP:		//�ϼ�
				xrot += 1.0;
				break;
			case GLUT_KEY_DOWN:		//�¼�
				xrot -= 1.0;
				break;
			case GLUT_KEY_LEFT:		//���
				yrot += 1.0;
				break;
			case GLUT_KEY_RIGHT:	//�Ҽ�
				yrot -= 1.0;
				break;
			default:
				break;
			}
			glutPostRedisplay();		//��ǵ�ǰ������Ҫ���»���
			glFlush();					//��ͼ�����ʾ����Ļ��
		}

		//�������ܣ��ı䴰�ڴ�С
		void ChangeSize(int w, int h)
		{
			GLfloat lightPos[] = { -50.f, 50.0f, 100.0f, 1.0f };
			GLfloat nRange = 1.9f;

			if (h == 0)
				h = 1;

			glViewport(0, 0, w, h);		//����������Ļ�ϵĴ��ڴ�С

			glMatrixMode(GL_PROJECTION);//��̲�������ͶӰ�任��Χ��
			glLoadIdentity();			//���õ�ǰ����Ϊ��λ����

										//����ͶӰ
			if (w <= h)
				glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);
			else
				glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

			glMatrixMode(GL_MODELVIEW);			//ѡ��ģ�͹۲����
			glLoadIdentity();					//���õ�ǰ����Ϊ��λ����

			glLightfv(GL_LIGHT0, GL_POSITION, lightPos);//���¶����Դ
		}

void main()
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);	//˫����
	glutInitWindowSize(700, 350);		//���ڴ�С
	glutInitWindowPosition(300, 100);	//����λ��
	glutCreateWindow("�����ͼ��ѧ_2014211292_���н�");		//��������

	SetupRC();							//�Զ���ĳ�ʼ������
	glutDisplayFunc(RenderScene);		//��ʾ�ص����������ڻ�ͼ
	glutReshapeFunc(ChangeSize);		//����ı䴰�ڴ�С
	glutSpecialFunc(SpecialKeys);		//������������Ϣ

	glutMainLoop();						//��GLUT��ܿ�ʼ����, �������¼�
}


