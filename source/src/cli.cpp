#include "cli.h"
#include <sstream>
#include <iostream>
#include <QApplication>
#include <QDebug>
#include <QFile>
using namespace std;
using namespace cgcore;

#define RENDER \
    shape->render(rawImg);\
    shapes[shape->id]=shape

#define NEWSHAPE \
    Shape* shape = new Shape();\
    shape->color = color;\
    shape->id = id

Cli::Cli() : smdCmd(-1) { }
Cli::~Cli() {
	for (auto& i : shapes) {
		delete i;
	}
	shapes.clear();
}

bool Cli::handleCmd(string _cmd) {
	stringstream ss(_cmd);
	switch (smdCmd) {//  �����������ĵڶ�������
	case 0: {
		NEWSHAPE;
		shape->aType = shape->dda;
		shape->ctrlp.resize(n);
		shape->sType = shape->polygon;
		int x, y;
		for (int i = 0; i < n; i++) {
			ss >> x >> y;
			shape->ctrlp[i].setX(x);
			shape->ctrlp[i].setY(y);
		}
		RENDER;

		smdCmd = -1;
		return true;
	}
	case 1: {
		NEWSHAPE;
		shape->aType = shape->bresenham;
		shape->ctrlp.resize(n);
		shape->sType = shape->polygon;
		int x, y;
		for (int i = 0; i < n; i++) {
			ss >> x >> y;
			shape->ctrlp[i].setX(x);
			shape->ctrlp[i].setY(y);
		}
		RENDER;
		smdCmd = -1;
		return true;
	}
	case 2: {
		smdCmd = -1;
		NEWSHAPE;
		shape->aType = shape->bezier;
		shape->ctrlp.resize(n);
		shape->sType = shape->curve;
		int x, y;
		for (int i = 0; i < n; i++) {
			ss >> x >> y;
			shape->ctrlp[i].setX(x);
			shape->ctrlp[i].setY(y);
		}
		RENDER;
		return true;
	}
	case 3: {
		smdCmd = -1;
		NEWSHAPE;
		shape->aType = shape->bspline;
		shape->ctrlp.resize(n);
		shape->sType = shape->curve;
		int x, y;
		for (int i = 0; i < n; i++) {
			ss >> x >> y;
			shape->ctrlp[i].setX(x);
			shape->ctrlp[i].setY(y);
		}
		RENDER;
		return true;
	}
	default:
		break;
	}
	string curWord;
	ss >> curWord;
	switch (cmd2cmd(curWord)) {
	case RESETCANVAS:
		for (auto& i : shapes) {
			delete i;
		}
		shapes.clear();
		tmpx.resize(2);
		for (int i = 0; i < 2; i++) {
			if (ss.eof()) {
				//  TODO: �����������
				return false;
			}
			ss >> tmpx[i];
		}
		w = tmpx[0]; h = tmpx[1];
		if (!(//  Ҫ�����100��1000����10000�Ա�����
			(w > 0 && w < 10000)
			&&
			(h > 0 && h < 10000))
			) {
			//  TODO: �����������
			return false;
		}
		//Proc::resetCanvas(w, h, &rawImg);
		rawImg = QImage(w, h, QImage::Format::Format_RGB888);
		rawImg.fill(Qt::white);
		return true;
	case SAVECANVAS:
		if (ss.eof()) {
			//  TODO: �����������
			return false;
		}
		ss >> curWord;
		rawImg.save((curWord + ".bmp").c_str());
		return true;
	case SETCOLOR:
		tmpx.resize(3);
		for (int i = 0; i < 3; i++) {
			if (ss.eof()) {
				//  TODO: �����������
				return false;
			}
			ss >> tmpx[i];
			if (!(tmpx[i] >= 0 && tmpx[i] <= 255)) {
				//  TODO: �����������
				return false;
			}
		}
		//  �����ţ�ΰ��ı����������ζ���Ĵ����Ż�����
		R = tmpx[0]; G = tmpx[1]; B = tmpx[2];
		color = qRgb(R, G, B);
		return true;
	case DRAWLINE: {
		if (rawImg.isNull()) {
			//  TODO: �����������
			return false;
		}
		tmpx.resize(4);
		if (ss.eof()) {
			//  TODO: �����������
			return false;
		}
		ss >> id;
		for (int i = 0; i < 4; i++) {
			if (ss.eof()) {
				//  TODO: �����������
				return false;
			}
			ss >> tmpx[i];
			continue;	//  ��������Ĵ��룬�����������飬QImage::setPixel��Խ�紦��
			if (i % 2 == 0) {	// ��������
				if (!(tmpx[i] >= 0 && tmpx[i] < rawImg.width())) {
					//  TODO: �����������
					return false;
				}
			}
			else {				//  ��������
				if (!(tmpx[i] >= 0 && tmpx[i] < rawImg.height())) {
					//  TODO: �����������
					return false;
				}
			}
		}
		if (ss.eof()) {
			//  TODO: �����������
			return false;
		}
		ss >> curWord;
		switch (cmd2algorithm(curWord)) {
		case DDA: {
			NEWSHAPE;
			shape->aType = shape->dda;
			shape->sType = shape->line;
			shape->ctrlp.push_back(QPoint(tmpx[0], tmpx[1]));
			shape->ctrlp.push_back(QPoint(tmpx[2], tmpx[3]));
			RENDER;
			return true;
		}
		case BRESENHAM: {
			NEWSHAPE;
			shape->aType = shape->bresenham;
			shape->sType = shape->line;
			shape->ctrlp.push_back(QPoint(tmpx[0], tmpx[1]));
			shape->ctrlp.push_back(QPoint(tmpx[2], tmpx[3]));
			RENDER;
			return true;
		}
		default:
			//  TODO: �����������
			return false;
		}
	}
	case DRAWPOLYGON:
		if (ss.eof()) {
			//  TODO: �����������
			return false;
		}
		ss >> id;
		if (ss.eof()) {
			//  TODO: �����������
			return false;
		}
		ss >> n;
		if (ss.eof()) {
			//  TODO: �����������
			return false;
		}
		ss >> curWord;
		switch (cmd2algorithm(curWord)) {
		case DDA:
			smdCmd = 0;
			return true;
		case BRESENHAM:
			smdCmd = 1;
			return true;
		}
	case DRAWELLIPSE: {
		if (rawImg.isNull()) {
			//  TODO: �����������
			return false;
		}
		tmpx.resize(4);
		if (ss.eof()) {
			//  TODO: �����������
			return false;
		}
		ss >> id;
		for (int i = 0; i < 4; i++) {
			if (ss.eof()) {
				//  TODO: �����������
				return false;
			}
			ss >> tmpx[i];
			if (tmpx[i] < 0) {
				//  TODO: �����������
				return false;
			}
		}
		NEWSHAPE;
		shape->sType = shape->ellipse;
		shape->ctrlp.push_back(QPoint(tmpx[0], tmpx[1]));
		shape->ctrlp.push_back(QPoint(tmpx[2], tmpx[3]));
		RENDER;
		return true;
	}
	case DRAWCURVE:
		if (ss.eof()) {
			//  TODO: �����������
			return false;
		}
		ss >> id;
		if (ss.eof()) {
			//  TODO: �����������
			return false;
		}
		ss >> n;
		if (ss.eof()) {
			//  TODO: �����������
			return false;
		}
		ss >> curWord;
		switch (cmd2algorithm(curWord)) {
		case BEZIER:
			smdCmd = 2;
			return true;
		case B_SPLINE:
			smdCmd = 3;
			return true;
		}
	case TRANSLATE: {
		int dx, dy;
		ss >> id >> dx >> dy;
		auto shape = shapes.find(id);
		if (shape == shapes.end()) {
			return false;
		}
		(*shape)->translate(dx, dy);
		repaintAll();
		return true;
	}
	case ROTATE: {
		int x, y, r;
		ss >> id >> x >> y >> r;
		auto shape = shapes.find(id);
		if (shape == shapes.end()) {
			return false;
		}
		(*shape)->rotate(x, y, r);
		repaintAll();
		return true;
	}
	case SCALE: {
		int x, y; float s;
		ss >> id >> x >> y >> s;
		auto shape = shapes.find(id);
		if (shape == shapes.end()) {
			return false;
		}
		(*shape)->scale(x, y, s, s);
		repaintAll();
		return true;
	}
	case CLIP: {
		int x1, y1, x2, y2;
		ss >> id >> x1 >> y1 >> x2 >> y2;
		auto shape = shapes.find(id);
		if (shape == shapes.end()) {
			return false;
		}
		ss >> curWord;
		switch (cmd2algorithm((curWord))) {
		case COHEN_SUTHERLAND: {
			(*shape)->clip(x1, y1, x2, y2, (*shape)->cohen_sutherland);
		}
		case LIANG_BARSKY: {
			(*shape)->clip(x1, y1, x2, y2, (*shape)->liang_barsky);
		}
		default:
			break;
		}
		repaintAll();
		return true;
	}
	default:
		//  TODO: �����������
		return false;
	}
}

bool Cli::handleScript(const char* filename) {
	QFile freader(filename);
	if (!freader.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	QString tmp;
	bool flag(true);
	while (!freader.atEnd()) {
		tmp = freader.readLine();
		tmp = tmp.simplified();
		qDebug() << tmp << endl;
		if (tmp.length() < 3) {
			continue;
		}
		tmp = tmp.simplified();
		if (!handleCmd(tmp.toLower().toStdString())) {
			flag = false;
		}
	}
	return flag;
}

void Cli::repaintAll() {
	rawImg = QImage(rawImg.size(), QImage::Format::Format_RGB888);
	rawImg.fill(Qt::white);
	for (auto& i : shapes) {
		i->render(rawImg);
	}
}
