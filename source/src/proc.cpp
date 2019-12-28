#include "proc.h"
#include <iostream>
#include <cmath>
#include <cassert>
#include <QPoint>
#include <QDebug>
using namespace std;

//  ����������������֡�����������buffer
#define TOBUFFER(x,y) buffer.push_back(QPoint(x,y))

namespace cgcore {
	/*��buffer��乹��ֱ��(x1,y1)-(x2,y2)�ĵ㣬clear���������Ƿ����֡����*/
	void Proc::drawLineByDDA(
		int x1, int y1, int x2, int y2, std::vector<QPoint>& buffer, bool clear
	) {
		if (clear)
			buffer.clear();				//  ���֡����
		int stepx(1), stepy(1);			//  ��������������
		if (x1 > x2) stepx = -1;
		if (y1 > y2) stepy = -1;
		if (x1 == x2) {					//  �����ֱ�ߵ��Ż�
			while (y1 != y2) {			//  װ��֡����
				TOBUFFER(x1, y1);
				y1 += stepy;
			}
			TOBUFFER(x1, y1);
		}
		else if (y1 == y2) {			//  ���ˮƽ�ߵ��Ż�
			while (x1 != x2) {
				TOBUFFER(x1, y1);
				x1 += stepx;
			}
			TOBUFFER(x1, y1);
		}
		else {											//  ����ʵ����ֵ��ַ����㷨
			double dx(x2 - x1), dy(y2 - y1);			//  ���ڼ��㵥λ����
			double x(fabs(dx)), y(fabs(dy));
			double length(x > y ? x : y);				//  ���ȡ������
			dx /= length;	dy /= length;				//  ��õ�λ����
			x = x1; y = y1;								//  �����ʼ��λ��
			int i(length);
			while ((i--) >= 0) {						//  ѭ������
				TOBUFFER(x, y);
				x += dx;
				y += dy;
			}
		}
	}
	/*��buffer��乹��ֱ��(x1,y1)-(x2,y2)�ĵ㣬clear���������Ƿ����֡����*/
	void Proc::drawLineByBresenham(
		int x1, int y1, int x2, int y2, std::vector<QPoint>& buffer, bool clear
	) {
		if (clear)
			buffer.clear();	//  ���֡����
		int stepx(1), stepy(1);
		if (x1 > x2) stepx = -1;
		if (y1 > y2) stepy = -1;
		if (x1 == x2) {		//  �����ֱ�ߵ��Ż�
			while (y1 != y2) {
				TOBUFFER(x1, y1);
				y1 += stepy;
			}
			TOBUFFER(x1, y1);
			return;
		}
		else if (y1 == y2) {//  ���ˮƽ�ߵ��Ż�
			while (x1 != x2) {
				TOBUFFER(x1, y1);
				x1 += stepx;
			}
			TOBUFFER(x1, y1);
			return;
		}
		int x(x1), y(y1), dx(abs(x2 - x1)), dy(abs(y2 - y1));
		if (dx == dy) {	//  ��ԶԽ��ߵ��Ż�
			while (x != x2) {
				TOBUFFER(x, y);
				x += stepx; y += stepy;
			}
			TOBUFFER(x, y);
		}//  ��ʽ��ʼBresenham�㷨
		else if (dx > dy) {
			int p(2 * dy - dx), twody(2 * dy), twody_2dx(2 * (dy - dx)), i(dx);
			while ((i--) >= 0) {
				TOBUFFER(x, y);
				x += stepx;
				if (p < 0)
					p += twody;
				else {
					p += twody_2dx; y += stepy;
				}
			}
		}
		else {//  ���б�������
			int p(2 * dx - dy), twodx(2 * dx), twodx_2dy(2 * (dx - dy)), i(dy);
			while ((i--) >= 0) {
				TOBUFFER(x, y);
				y += stepy;
				if (p < 0)
					p += twodx;
				else {
					p += twodx_2dy; x += stepx;
				}
			}
		}
	}
	/*��buffer��乹�ɶ����{xi,yi}�ĵ�*/
	void Proc::drawPolygonByDDA(
		const std::vector<int>& xs, const std::vector<int>& ys, std::vector<QPoint>& buffer
	) {
		buffer.clear();
		assert(xs.size() == ys.size());
		if (xs.size() == 0)
			return;
		size_t seclast(xs.size() - 1);
		vector<QPoint> tmp;
		for (size_t i = 0; i < seclast; i++) {
			drawLineByDDA(xs[i], ys[i], xs[i + 1], ys[i + 1], buffer, false);
		}
		drawLineByDDA(xs.front(), ys.front(), xs.back(), ys.back(), buffer, false);
	}
	/*��buffer��乹�ɶ����{xi,yi}�ĵ�*/
	void Proc::drawPolygonByBresenham(
		const std::vector<int>& xs, const std::vector<int>& ys, std::vector<QPoint>& buffer
	) {
		buffer.clear();
		assert(xs.size() == ys.size());
		if (xs.size() == 0)
			return;
		size_t seclast(xs.size() - 1);
		vector<QPoint> tmp;
		for (size_t i = 0; i < seclast; i++) {
			drawLineByBresenham(xs[i], ys[i], xs[i + 1], ys[i + 1], buffer, false);
		}
		drawLineByBresenham(xs.front(), ys.front(), xs.back(), ys.back(), buffer, false);
	}
	/*��buffer��乹����Բ{xi,yi}�ĵ�*/
	void Proc::drawEllipse(
		int x0, int y0, int rx, int ry, std::vector<QPoint>& buffer
	) {
		buffer.clear();						//  ���֡����
		if (rx == ry) {						//  ��׼Բ�㷨
			int x(0), y(rx), p(3 - 2 * rx);	//  ��������
			while (x <= y) {
				TOBUFFER(x0 + x, y0 + y);
				TOBUFFER(x0 - x, y0 - y);
				TOBUFFER(x0 + x, y0 - y);
				TOBUFFER(x0 - x, y0 + y);
				TOBUFFER(x0 + y, y0 + x);
				TOBUFFER(x0 - y, y0 + x);
				TOBUFFER(x0 - y, y0 - x);
				TOBUFFER(x0 + y, y0 - x);
				if (p >= 0) {
					p += 4 * (x - y) + 10;
					y--;
				}
				else {
					p += 4 * x + 6;
				}
				x++;
			}
		}
		else if (rx > ry) {					//  �е���Բ�㷨
			int x(0), y(ry);
			double pk(0);
			int ry2(ry * ry), rx2(rx * rx), rx2ry2(rx2 * ry2);
			TOBUFFER(x0 + x, y0 + y);
			TOBUFFER(x0 - x, y0 - y);
			TOBUFFER(x0 + x, y0 - y);
			TOBUFFER(x0 - x, y0 + y);
			pk = ry2 - rx2 * ry + rx2 / 4.0;
			while (ry2 * x <= rx2 * y) {
				x++;
				if (pk < 0) {
					pk += (2 * ry2 * x + ry2);
				}
				else {
					y--;
					pk += (2 * ry2 * x - 2 * rx2 * y + ry2);
				}
				TOBUFFER(x0 + x, y0 + y);
				TOBUFFER(x0 - x, y0 - y);
				TOBUFFER(x0 + x, y0 - y);
				TOBUFFER(x0 - x, y0 + y);
			}
			pk = ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1.0) * (y - 1.0) - rx2ry2;
			while (y > 0) {
				y--;
				if (pk > 0) {
					pk += (-2 * rx2 * y + rx2);
				}
				else {
					x++;
					pk += (2 * ry2 * x - 2 * rx2 * y + rx2);
				}
				TOBUFFER(x0 + x, y0 + y);
				TOBUFFER(x0 - x, y0 - y);
				TOBUFFER(x0 + x, y0 - y);
				TOBUFFER(x0 - x, y0 + y);
			}
		}
		else {
			swap(x0, y0); swap(rx, ry);	//  �ȷ�����������
			int x(0), y(ry);			//  ��ִ�� rx > ry ���е���Բ�㷨
			double pk(0);
			int ry2(ry * ry), rx2(rx * rx), rx2ry2(rx2 * ry2);
			TOBUFFER(y0 + y, x0 + x);
			TOBUFFER(y0 - y, x0 - x);
			TOBUFFER(y0 - y, x0 + x);
			TOBUFFER(y0 + y, x0 - x);
			pk = ry2 - rx2 * ry + rx2 / 4.0;
			while (ry2 * x <= rx2 * y) {
				x++;
				if (pk < 0) {
					pk += (2 * ry2 * x + ry2);
				}
				else {
					y--;
					pk += (2 * ry2 * x - 2 * rx2 * y + ry2);
				}
				TOBUFFER(y0 + y, x0 + x);
				TOBUFFER(y0 - y, x0 - x);
				TOBUFFER(y0 - y, x0 + x);
				TOBUFFER(y0 + y, x0 - x);
			}
			pk = ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1.0) * (y - 1.0) - rx2ry2;
			while (y > 0) {
				y--;
				if (pk > 0) {
					pk += (-2 * rx2 * y + rx2);
				}
				else {
					x++;
					pk += (2 * ry2 * x - 2 * rx2 * y + rx2);
				}
				TOBUFFER(y0 + y, x0 + x);
				TOBUFFER(y0 - y, x0 - x);
				TOBUFFER(y0 - y, x0 + x);
				TOBUFFER(y0 + y, x0 - x);
			}
		}
	}

	double bzGetPX(double u, int i, int r, QVector <QPoint>& pointSet) {
		if (r == 0) return pointSet[i].x();
		return (1 - u) * bzGetPX(u, i, r - 1, pointSet) + u * bzGetPX(u, i + 1, r - 1, pointSet);
	}

	/*��buffer��乹�ɱ���������{xi,yi}�ĵ�*/
	void Proc::drawCurveByBezier(
		const std::vector<int>& xs, const std::vector<int>& ys, std::vector<QPoint>& buffer
	) {
		buffer.clear();
		vector<QPointF> input;
		for (size_t i = 0; i < xs.size(); i++) {
			input.push_back(QPointF(xs.at(i), ys.at(i)));
		}
		
		int n = input.size();
		if (n == 2) {
			drawLineByBresenham(xs[0], ys[0], xs[1], ys[1], buffer);
			return;
		}
		vector<QPointF> p; p.assign(input.begin(), input.end());
		//  Ϊ�˱�������ۻ���ȫ��ʹ�ø���������
		QPointF tmp = p[0];
		int div = sqrt(n); if (div < 1)div = 1;
        for (double t = 0; t <= 1; t += 0.01 / div) {
			p.assign(input.begin(), input.end());
			for (int i = 1; i < n; i++) {			// ���ѭ��n-1�Σ�����n-1��t�ȷ�
				for (int j = 0; j < n - i; j++) {	//ÿ��ѭ�������n-1,n-2,...,1���зֵ�
					p[j] = (1.0 - t) * p[j] + t * p[j + 1];
				}
			}
			drawLineByBresenham( tmp.x(), tmp.y(), p[0].x(), p[0].y(), buffer, false);
			tmp = p[0];
		}

	}
	/*�ݹ����n������*/
	/*de Boor-Cox���ƶ���*/
	double Proc::bspline(double* U, double u, int i, int k) {
		double result;
		if (k == 1) {
			if (U[i] < u && u < U[i + 1])
				result = 1;
			else
				result = 0;
		}
		else {//  �������������Լ���� 0/0=0
			result = 0;
			if (i + k - 1 != i)// Ҫ�� U[i + k - 1] - U[i] ��= 0
				result += (u - U[i]) / (U[i + k - 1] - U[i]) * bspline(U, u, i, k - 1);
			if (i + k != i + 1)// Ҫ�� U[i + k] - U[i + 1] ��= 0
				result += (U[i + k] - u) / (U[i + k] - U[i + 1]) * bspline(U, u, i + 1, k - 1);
		}
		return result;
	}
	/*��buffer��乹��B��������{xi,yi}�ĵ�*/
	void Proc::drawCurveByBSpline(
		const std::vector<int>& xs, const std::vector<int>& ys, std::vector<QPoint>& buffer
	) {
		buffer.clear();
		vector<QPointF> input, tmpBuf;
		for (size_t i = 0; i < xs.size(); i++) {
			input.push_back(QPointF(xs.at(i), ys.at(i)));
		}
		// ���ÿ��Ͻ��Ķ���������
		if (input.size() < 4)return;//  �ĸ���֮�ڲ�����ͼ
		int k = 4;
		int n = input.size() - 1;// ��ʱ����n+1�����Ƶ�
		double* U = new double[n + k + 1];
		//����u��ȡֵ��Χ��n+k+1�������ڵ�����ֵ�ֳ�n+k��������
		//����U��ȡֵ��u = U[k - 1]����֤������ͬһ��������������������ǵı�ֵ
		for (int i = 0; i < n + k + 1; i++)
			U[i] = i;
		QPointF lastP(input[0]);
		int div = sqrt(n);
		if (div < 1)div = 1;
		// �����ڴӽڵ�ֵ[u k-1, u n+1]������
		for (double u = U[k - 1]; u < U[n + 1]; u += 0.01 / div) {
			QPointF curP(0, 0);
			for (int i = 0; i < n + 1; i++)
				curP += input[i] * bspline(U, u, i, k);
			if (fabs(curP.x()) > 0.0001 || fabs(curP.y()) > 0.0001)
				tmpBuf.push_back(curP);
		}
		delete[] U; U = nullptr;

		//  ֱ�����ù�ʽ���㣬������֤����ݹ��㷨����ȷ��
		//����ʽ�������磬���ǵó����ˣ���һ�ù������ʽ����д�ʼ��Ż���
		//auto N3 = [=](int i, double u) {
		//	double t = u - i;
		//	double a = 1.0 / 6;
		//	if (0 <= t && t < 1)
		//		return a * t * t * t;
		//	if (1 <= t && t < 2)
		//		return a * (-3��0 * pow(t - 1, 3) + 3 * pow(t - 1, 2) + 3 * (t - 1) + 1);
		//	if (2 <= t && t < 3)
		//		return a * (3 * pow(t - 2, 3) - 6 * pow(t - 2, 2) + 4);
		//	if (3 <= t && t < 4)
		//		return a * pow(4 - t, 3);
		//	return 0.0;
		//};
		//int k = 3;
		//int n = input.size();
		//if (n < 4)return;//  �ĸ���֮�ڲ�����ͼ
		//bool isFirst = true;
		//QPointF lastPoint;
		//for (double u = k; u < n; u += 0.01) {
		//	QPointF curPoint(0, 0);
		//	for (int i = 0; i < n; i++) {
		//		curPoint += input[i] * N3(i, u);
		//	}
		//	tmpBuf.push_back(curPoint);
		//	lastPoint = curPoint;
		//}
		if (tmpBuf.size() == 1) {
			drawLineByBresenham(tmpBuf[0].x(), tmpBuf[0].y(), tmpBuf[0].x(), tmpBuf[0].y(), buffer, false);
		}
		else
			for (size_t i = 0; i < tmpBuf.size() - 1; i++) {
				drawLineByBresenham(tmpBuf[i].x(), tmpBuf[i].y(), tmpBuf[i + 1].x(), tmpBuf[i + 1].y(), buffer, false);
			}
	}

	QPoint Proc::getIntersection(QPoint a, double m, short code,
		int xmin, int ymin, int xmax, int ymax) {
		QPoint p;
		//  �������£��ⷽ���㽻��
		if ((code & 0b0100)) {
			p.setX(round(xmin));
			p.setY(round(a.y() + (p.x() - a.x()) * m));
		}
		else if ((code & (0b1000))) {
			p.setX(round(xmax));
			p.setY(round(a.y() + (p.x() - a.x()) * m));
		}
		else if ((code & (0b0001))) {
			p.setY(round(ymax));
			p.setX(round(a.x() + (p.y() - a.y()) / m));
		}
		else if ((code & (0b0010))) {
			p.setY(round(ymin));
			p.setX(round(a.x() + (p.y() - a.y()) / m));
		}
		return p;
	}
	/*�޸�ctrlpΪ�����ھ���(x1,y1)(x2,y2)�е��߶ζ˵�*/
	void Proc::clipByCohenSutherland(
		int x1, int y1, int x2, int y2, std::vector<QPoint>& ctrlp
	) {
		cout << "enter" << endl;
		auto getRegionCode = [=](const QPoint& point) {
			//y�� ywmax ����1λ��1��������0��
			//y�� ywmin ����2λ��1��������0��
			//x �� xwmin ����3λ��1��������0��
			//x �� xwmax ����4λ��1��������0��
			short code(0b0000);
			if (point.y() > y2)
				code |= 0b0001;
			if (point.y() < y1)
				code |= 0b0010;
			if (point.x() < x1)
				code |= 0b0100;
			if (point.x() > x2)
				code |= 0b1000;
			return code;
		};
		QPoint p(ctrlp[0]), q(ctrlp[1]);
		short pcode(getRegionCode(p)),
			qcode(getRegionCode(q));
		double m = (q.y() - p.y()) / (q.x() - p.x() + 0.000000000001);
		bool done(false), isIn(false);
		do {
			pcode = getRegionCode(p);
			qcode = getRegionCode(q);
			if (pcode == 0 && qcode == 0) {
				done = 1; isIn = 1;
			}
			else if (pcode & qcode)
				done = 1;
			else {
				if (pcode)
					p = getIntersection(p, m, pcode, x1, y1, x2, y2);
				if (qcode)
					q = getIntersection(q, m, qcode, x1, y1, x2, y2);
			}
		} while (!done);
		if (isIn == 1) {
			ctrlp[0] = p; ctrlp[1] = q;
		}
		else {
			ctrlp[0] = ctrlp[1] = QPoint(-1, -1);
		}
	}
	/*�޸�ctrlpΪ�����ھ���(x1,y1)(x2,y2)�е��߶ζ˵�*/
	void Proc::clipByLiangBarsky(
		int x1, int y1, int x2, int y2, std::vector<QPoint>& ctrlp
	) {
		int x(ctrlp[0].x()), y(ctrlp[0].y()),
			xend(ctrlp[1].x()), yend(ctrlp[1].y());
		double dx(xend - x);
		double dy(yend - y);
		double p[4];
		p[0] = -dx; p[1] = -p[0];
		p[2] = -dy; p[3] = -p[2];
		double q[4];
		q[0] = x - x1; q[1] = x2 - x;
		q[2] = y - y1; q[3] = y2 - y;
		double u, u1(0), u2(1);
		for (size_t k = 0; k < 4; k++) {//  �ĸ��߽�
			u = q[k] / p[k];
			if (p[k] < 0) {
				if (u > u2)
					return;//  ����
				if (u > u1)
					u1 = u;
			}
			else if (p[k] > 0) {
				if (u < u1)
					return;
				if (u < u2)
					u2 = u;
			}
			else if (q[k] < 0)
				return;
		}
		ctrlp[0] = QPoint(x + u1 * dx, y + u1 * dy);
		ctrlp[1] = QPoint(x + u2 * dx, y + u2 * dy);
	}
	/*��ctrlbuffer�еĵ�ƽ��(x,y)�������ctrlbuffer�ǿ��Ƶ㣬����ֱ�ߵĶ˵㣬��Բ�����ĵ�*/
	void Proc::translate(
		int x, int y, std::vector<QPoint>& ctrlbuffer
	) {
		for (auto& i : ctrlbuffer) {
			i.setX(i.x() + x);
			i.setY(i.y() + y);
		}
	}
	/*��ctrlbuffer�еĵ���(x,y)Ϊ����˳ʱ����ת�Ƕ�r�������ctrlbuffer�ǿ��Ƶ㣬����ֱ�ߵĶ˵㣬��Բ�����ĵ�*/
	void Proc::rotate(
		int x, int y, int r, std::vector<QPoint>& ctrlbuffer
	) {
		const double pi = 3.1415926;
		double cosr(cos(r * pi / 180.0)), sinr(sin(r * pi / 180.0));
		for (auto& i : ctrlbuffer) {
			int x0 = i.x(), y0 = i.y();
			i.setX(x + 1.0 * (x0 - x) * cosr - 1.0 * (y0 - y) * sinr);
			i.setY(y + 1.0 * (x0 - x) * sinr + 1.0 * (y0 - y) * cosr);
		}
	}
	/*��ctrlbuffer�еĵ���(x,y)Ϊ���ķ���s�������ctrlbuffer�ǿ��Ƶ㣬����ֱ�ߵĶ˵㣬��Բ�����ĵ�*/
	void Proc::scale(
		int x, int y, float sx, float sy, std::vector<QPoint>& ctrlbuffer
	) {
		//A(Xi,Yi)-----B(X,Y)---------------C(a,b)
		//(Xi-X)*Sx=(a-X)
		for (auto& i : ctrlbuffer) {
			i.setX(x + (i.x() - x) * sx);
			i.setY(y + (i.y() - y) * sy);
			//cout << i.x() << "," << i.y() << endl;
		}
	}

	Shape::Shape() :color(Qt::black), id(-1), isModified(false) {

	}

	//  �ж�����Ƿ�ѡ�����ͼԪ
	bool Shape::contains(const QPoint& point) {
		return boundingRect.contains(point);
	}
	//  ͼԪ�༭����������class Proc�еĺ������¿��Ƶ㣬����filbuffer���»�����
	void Shape::translate(int x, int y) {
		if (sType == ellipse) {
			vector<QPoint> tmp = { ctrlp[0] };
			Proc::translate(x, y, tmp);
			ctrlp[0] = tmp[0];
			return;
		}
		Proc::translate(x, y, ctrlp);
	}
	void Shape::rotate(int x, int y, int r) {
		if (sType == ellipse) {
			vector<QPoint> tmp;
			tmp.push_back(ctrlp[0]);
			Proc::rotate(x, y, r, tmp);
			ctrlp[0] = tmp[0];
			return;
		}
		Proc::rotate(x, y, r, ctrlp);
	}
	void Shape::scale(int x, int y, float sx, float sy) {
		if (sType == ellipse) {
			vector<QPoint> tmp;
			tmp.push_back(ctrlp[0]);
			Proc::scale(x, y, sx, sy, tmp);
			ctrlp[0] = tmp[0];
			ctrlp[1] *= angle;
			return;
		}
		Proc::scale(x, y, sx, sy, ctrlp);
	}
	void Shape::clip(int x1, int y1, int x2, int y2, AlgorithmType _atype) {
		if (sType != line)
			return;
		switch (_atype) {
		case cohen_sutherland: {
			//Proc::clipByLiangBarsky(x1, y1, x2, y2, ctrlp);
			Proc::clipByCohenSutherland(x1, y1, x2, y2, ctrlp);
			break;
		}
		case liang_barsky: {
			Proc::clipByLiangBarsky(x1, y1, x2, y2, ctrlp);
			break;
		}
		default: {
			break;
		}
		}
	}
	void Shape::fillBuffer() {
		switch (sType) {
		case line: {
			switch (aType) {
			case dda: {
				Proc::drawLineByDDA(ctrlp[0].x(), ctrlp[0].y(), ctrlp[1].x(), ctrlp[1].y(), buffer); break;
			}
			case bresenham: {
				Proc::drawLineByBresenham(ctrlp[0].x(), ctrlp[0].y(), ctrlp[1].x(), ctrlp[1].y(), buffer); break;
			}
			default: {
				Proc::drawLineByBresenham(ctrlp[0].x(), ctrlp[0].y(), ctrlp[1].x(), ctrlp[1].y(), buffer); break;
			}
			}
			break;
		}
		case ellipse: {
			Proc::drawEllipse(ctrlp[0].x(), ctrlp[0].y(), ctrlp[1].x(), ctrlp[1].y(), buffer);
			break;
		}
		case polygon: {
			vector<int> xs, ys;
			for (auto& i : ctrlp) {
				xs.push_back(i.x());
				ys.push_back(i.y());
			}
			switch (aType) {
			case dda: {
				Proc::drawPolygonByDDA(xs, ys, buffer); break;
			case bresenham: {
				Proc::drawPolygonByBresenham(xs, ys, buffer); break;
			}
			default: {
				Proc::drawPolygonByBresenham(xs, ys, buffer); break;
			}
			}
			}
			break;
		}
		case curve: {
			vector<int> xs, ys;
			for (auto& i : ctrlp) {
				xs.push_back(i.x());
				ys.push_back(i.y());
			}
			switch (aType) {
			case bezier: {
				Proc::drawCurveByBezier(xs, ys, buffer);
				//qDebug()<<"ha?";
				break;
			}
			case bspline: {
				Proc::drawCurveByBSpline(xs, ys, buffer);
				break;
			}
			default: {
				Proc::drawCurveByBezier(xs, ys, buffer);
				break;
			}
			}
			break;
		}
		default:
			break;
		}
	}
	void Shape::render(QImage& img) {
		fillBuffer();
		int xmin, ymin, xmax, ymax;
		xmin = ymin = std::numeric_limits<int>::max();
		xmax = ymax = std::numeric_limits<int>::min();
		for (auto& i : buffer) {
			img.setPixel(i, color);
			xmin = xmin < i.x() ? xmin : i.x();
			ymin = ymin < i.y() ? ymin : i.y();
			xmax = xmax > i.x() ? xmax : i.x();
			ymax = ymax > i.y() ? ymax : i.y();
		}
		boundingRect = QRect(xmin, ymin, xmax - xmin, ymax - ymin);
	}
};
