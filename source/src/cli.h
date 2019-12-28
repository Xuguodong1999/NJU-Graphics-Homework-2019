#pragma once
#include "cmd.h"
#include "proc.h"
#include <string>
#include <vector>
#include <QColor>
#include <QImage>
#include <QMap>
#pragma execution_character_set("UTF-8")
using namespace std;

class Cli {
public:
	Cli();
	~Cli();
	bool handleCmd(std::string _cmd = std::string("resetcanvas 100 100"));
	bool handleScript(const char* filename = "");
private:
	QImage rawImg;
	int smdCmd;//  ������xx�Ķ������� 0��dda����� 1��breshman����� 2������������ 3��B����
	int						//  ����һ�ѳ��ñ���
		w, h,				//  ԭʼͼ�񳤿�
		newW, newH,			//  ��ͼ�񳤿�
		R, G, B, alpha,		//  ������ɫ
		id, n,				//  ��ǩ
		angleR;				//  ˳ʱ����ת�Ƕ�
	std::vector<int> tmpx;	//  ��ʱ�ռ�
	QRgb color;
	QMap<int, cgcore::Shape*> shapes;
	void repaintAll();
};

