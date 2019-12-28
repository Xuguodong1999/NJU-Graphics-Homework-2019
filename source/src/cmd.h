#pragma once

#include <string>
#pragma execution_character_set("UTF-8")
namespace cgcore {
	enum KeyWords {
		//  ָ����
		RESETCANVAS = 0,
		SAVECANVAS,
		SETCOLOR,
		DRAWLINE,
		DRAWPOLYGON,
		DRAWELLIPSE,
		DRAWCURVE,
		TRANSLATE,
		ROTATE,
		SCALE,
		CLIP,
		//  ������
		NOTFOUND,
		//  �㷨���
		DDA,
		BRESENHAM,
		BEZIER,
		B_SPLINE,
		COHEN_SUTHERLAND,
		LIANG_BARSKY
	};
	int cmd2cmd(const std::string& _cmd);
	int cmd2algorithm(const std::string& _cmd);
};
