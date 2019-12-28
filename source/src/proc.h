#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <QPoint>
#include <QRgb>
//  ��QRect������궨λͼԪ
//  ��λͼԪ�ı�׼��������BSP�����Ĳ���֮���
//  ����Qtû��¶����BSP�Ľӿڣ���Ҳ�����Լ�д��
//  ���Բ���Ч����low�ı�������ͼԪ�ķ�����λ���
#include <QRect>
#include <QImage>
#pragma execution_character_set("UTF-8")
#define ERRORINT -0x3f3f3f3f
namespace cgcore {
	class Proc {
	public:
		/*��buffer��乹��ֱ��(x1,y1)-(x2,y2)�ĵ㣬clear���������Ƿ����֡����*/
		static void drawLineByDDA(
			int x1, int y1, int x2, int y2, std::vector<QPoint>& buffer, bool clear = true
		);
		/*��buffer��乹��ֱ��(x1,y1)-(x2,y2)�ĵ㣬clear���������Ƿ����֡����*/
		static void drawLineByBresenham(
			int x1, int y1, int x2, int y2, std::vector<QPoint>& buffer, bool clear = true
		);
		/*��buffer��乹�ɶ����{xi,yi}�ĵ�*/
		static void drawPolygonByDDA(
			const std::vector<int>& xs, const std::vector<int>& ys, std::vector<QPoint>& buffer
		);
		/*��buffer��乹�ɶ����{xi,yi}�ĵ�*/
		static void drawPolygonByBresenham(
			const std::vector<int>& xs, const std::vector<int>& ys, std::vector<QPoint>& buffer
		);
		/*��buffer��乹����Բ{xi,yi}�ĵ�*/
		static void drawEllipse(
			int x0, int y0, int rx, int ry, std::vector<QPoint>& buffer
		);
		/*��buffer��乹�ɱ���������{xi,yi}�ĵ�*/
		static void drawCurveByBezier(
			const std::vector<int>& xs, const std::vector<int>& ys, std::vector<QPoint>& buffer
		);
		/*��buffer��乹��B��������{xi,yi}�ĵ�*/
		static void drawCurveByBSpline(
			const std::vector<int>& xs, const std::vector<int>& ys, std::vector<QPoint>& buffer
		);

		/*�޸�ctrlpΪ�����ھ���(x1,y1)(x2,y2)�е��߶ζ˵�*/
		static void clipByCohenSutherland(
			int x1, int y1, int x2, int y2, std::vector<QPoint>& ctrlp
		);
		/*�޸�ctrlpΪ�����ھ���(x1,y1)(x2,y2)�е��߶ζ˵�*/
		static void clipByLiangBarsky(
			int x1, int y1, int x2, int y2, std::vector<QPoint>& ctrlp
		);
		/*��ctrlbuffer�еĵ�ƽ��(x,y)�������ctrlbuffer�ǿ��Ƶ㣬����ֱ�ߵĶ˵㣬��Բ�����ĵ�*/

		static void translate(
			int x, int y, std::vector<QPoint>& ctrlbuffer
		);
		/*��ctrlbuffer�еĵ���(x,y)Ϊ����˳ʱ����ת�Ƕ�r�������ctrlbuffer�ǿ��Ƶ㣬����ֱ�ߵĶ˵㣬��Բ�����ĵ�*/
		static void rotate(
			int x, int y, int r, std::vector<QPoint>& ctrlbuffer
		);
		/*��ctrlbuffer�еĵ���(x,y)Ϊ���ķ���s�������ctrlbuffer�ǿ��Ƶ㣬����ֱ�ߵĶ˵㣬��Բ�����ĵ�*/
		static void scale(
			int x, int y, float sx, float sy, std::vector<QPoint>& ctrlbuffer
		);
	private:
		/*�󽻣�û�н��㷵��(ERRORINT,ERRORINT)*/
		static QPoint getIntersection(QPoint a, double m, short code,
			int xmin, int ymin, int xmax, int ymax);
		/*�ݹ����n������*/
		static double bspline(double* U, double u, int i, int k);
	};

	class Shape {
	public:
		enum ShapeType { line = 0, polygon, ellipse, curve } sType;
		enum AlgorithmType { bresenham = 0, dda, bezier, bspline, cohen_sutherland, liang_barsky }aType;
		Shape();
		//  Լ�����Ƶ�ı��
		//  ֱ�ߣ�size=2,���������˵�
		//  ��Բ��size=2��������x0,y0����rx,ry��
		std::vector<QPoint> ctrlp;
		std::vector<QPoint> _ctrlp; double angle = 0;
		std::vector<QPoint> buffer;
		QRgb color;
		int id;
		QRect boundingRect;
		bool isModified;
		//  �ж�����Ƿ�ѡ�����ͼԪ
		bool contains(const QPoint& point);
		//  ͼԪ�༭����������class Proc�еĺ������¿��Ƶ㣬����filbuffer���»�����
		void translate(int x, int y);
		void rotate(int x, int y, int r);
		void scale(int x, int y, float sx, float sy);
		void clip(int x1, int y1, int x2, int y2, AlgorithmType _atype);
		void render(QImage& img);
	private:
		void fillBuffer();
	};

};
