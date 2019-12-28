#include "cmd.h"
#include <map>
#include <iostream>

using namespace std;

namespace cgcore {
	static map<string, int> cmdTable = {
		/*���û�����
				resetCanvas width height
				��յ�ǰ���������������ÿ��
				width, height: int
				100 <= width, height <= 1000*/
		{"resetcanvas",RESETCANVAS},
		/*���滭����
				saveCanvas name
				����ǰ��������Ϊλͼname.bmp
				name: string*/
		{"savecanvas",SAVECANVAS},
		/*���û�����ɫ��
				setColor R G B
				R, G, B: int
				0 <= R, G, B <= 255*/
		{"setcolor",SETCOLOR},
		/*�����߶Σ�
				drawLine id x1 y1 x2 y2 algorithm
				id: int
				ͼԪ��ţ�ÿ��ͼԪ�ı����Ψһ��
				x1, y1, x2, y2: int
				��㡢�յ�����
				algorithm: string
				����ʹ�õ��㷨��������DDA���͡�Bresenham��*/
		{"drawline",DRAWLINE},
		/*���ƶ���Σ�
				drawPolygon id n algorithm
				x1 y1 x2 y2 �� xn yn
				id: int
				ͼԪ��ţ�ÿ��ͼԪ�ı����Ψһ��
				n: int
				������
				x1, y1, x2, y2 ... : int
				��������
				algorithm: string
				����ʹ�õ��㷨��������DDA���͡�Bresenham��*/
		{"drawpolygon",DRAWPOLYGON},
		/*������Բ���е�Բ�����㷨����
				drawEllipse id x y rx ry
				id: int
				ͼԪ��ţ�ÿ��ͼԪ�ı����Ψһ��
				x, y: int
				Բ������
				rx, ry: int
				������뾶*/
		{"drawellipse",DRAWELLIPSE},
		/*�������ߣ�
				drawCurve id n algorithm
				x1 y1 x2 y2 �� xn yn
				id: int
				ͼԪ��ţ�ÿ��ͼԪ�ı����Ψһ��
				n: int
				���Ƶ�����
				x1, y1, x2, y2 ... : int
				���Ƶ�����
				algorithm: string
				����ʹ�õ��㷨��������Bezier���͡�B-spline��*/
		{"drawcurve",DRAWCURVE},
		/*��ͼԪƽ�ƣ�
				translate id dx dy
				id: int
				Ҫƽ�Ƶ�ͼԪ���
				dx, dy: int
				ƽ������*/
		{"translate",TRANSLATE},
		/*��ͼԪ��ת��
				rotate id x y r
				id: int
				Ҫ��ת��ͼԪ���
				x, y: int
				��ת����
				r: int
				˳ʱ����ת�Ƕȣ��㣩*/
		{"rotate",ROTATE},
		/*��ͼԪ���ţ�
				scale id x y s
				id: int
				Ҫ���ŵ�ͼԪ���
				x, y: int
				��������
				s: float
				���ű���*/
		{"scale",SCALE},
		/*���߶βü���
				clip id x1 y1 x2 y2 algorithm
				id: int
				Ҫ�ü���ͼԪ���
				x1, y1, x2, y2: int
				�ü��������¡����Ͻ�����
				algorithm: string
				�ü�ʹ�õ��㷨��������Cohen-Sutherland���͡�Liang-Barsky��*/
		{"clip",CLIP}
	};

	static map<string, int> algorithmTable = {
		{"dda",DDA},
		{"bresenham",BRESENHAM},
		{"bezier",BEZIER},
		{"b-spline",B_SPLINE},
		{"cohen-sutherland",COHEN_SUTHERLAND},
		{"liang-barsky",LIANG_BARSKY}
	};

	int cmd2cmd(const string& _cmd) {
		string cmd;
		for (auto i : _cmd) {
			cmd.push_back(tolower(i));
		}
		map<string, int>::iterator iter = cmdTable.find(cmd);
		if (iter == cmdTable.end())
			return NOTFOUND;
		return iter->second;
	}

	int cmd2algorithm(const string& _cmd) {
		string cmd;
		for (auto i : _cmd) {
			cmd.push_back(tolower(i));
		}
		map<string, int>::iterator iter = algorithmTable.find(cmd);
		if (iter == cmdTable.end())
			return NOTFOUND;
		return iter->second;
	}

};
