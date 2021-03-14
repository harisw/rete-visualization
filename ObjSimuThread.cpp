// ObjSimuThread.cpp : implementation file
//

#include "pch.h"
#include "RETE_multinode_app.h"
#include "ObjSimuThread.h"


// ObjSimuThread

IMPLEMENT_DYNCREATE(ObjSimuThread, CWinThread)

ObjSimuThread::ObjSimuThread()
{
}

ObjSimuThread::~ObjSimuThread()
{
}

BOOL ObjSimuThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int ObjSimuThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(ObjSimuThread, CWinThread)
END_MESSAGE_MAP()


// ObjSimuThread message handlers


int ObjSimuThread::Run()
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinThread::Run();
}

void ObjSimuThread::drawCQVessel(CPaintDC& dc)
{
	dc.SelectObject(&hollowBrush);
	dc.SelectObject(&greenPen);
	for (auto snp : spatialNodePolygon) {
		polygon p = snp.second;

		CPoint* pts = new CPoint[p.outer().size() + 1];
		int start_x, start_y;
		//CPoint pts[p.outer().size()];
		for (int j = 0; j < p.outer().size(); j++) {
			point pt = p.outer().at(j);
			float x = pt.get<0>();
			float y = pt.get<1>();

			pts[j].x = x;
			pts[j].y = y;
		}

		dc.Polygon(pts, p.outer().size());
		//point pt = p.outer().at(0);
	}
}

void ObjSimuThread::drawObjects(CPaintDC& dc)
{
	drawCQVessel(dc);

	char buff[10];
	//int a = 01;
	int counter = 0;
	dc.SelectObject(&hollowBrush);
	float first_loc = 0;
	float second_loc = 0;
	if (m_object_location.size() > 0) {
		//while (global_itt < m_object_location[0].size() && counter < cycle_step) {
		while (global_itt < m_object_location[0].size()) {
			for (int i = 0; i < m_object_location.size(); i++) {
				//float first_loc = m_object_location[i][global_itt].first * x_norm - (max_first - max_w);
				//float second_loc = m_object_location[i][global_itt].second * y_norm - (max_second - max_h);

				first_loc = m_object_location[i][global_itt].first;
				second_loc = m_object_location[i][global_itt].second;

				if (i == 0) {

					//pen.CreatePen(PS_SOLID, 1, color_hex_map["Aqua"]);
					//pen.CreatePen(PS_SOLID, 1, 0x00FFFF00);
					dc.SelectObject(&bluePen);

					if (global_itt % 10 == 0) {
						CString cs(_itoa(global_itt, buff, 10));
						dc.TextOutW(first_loc + 20, second_loc, cs);
					}
					rad = 2;
				}
				else {
					//pen.CreatePen(PS_SOLID, 1, color_hex_map["Red"]);
					//pen.CreatePen(PS_SOLID, 1, 0x000000FF);
					dc.SelectObject(&redPen);
					rad = 2;
				}

				// rectangle with magenta frame and "transparent" background
				dc.Ellipse(first_loc - rad, second_loc - rad, first_loc + rad, second_loc + rad);
				//dc.Rectangle(0, 0, m_object_location[0]., 20);
				//Invalidate();
				Sleep(84);
				//Sleep(24);
			}

			global_itt++;
			counter++;
		}

		CString cs(_itoa(global_itt, buff, 10));
		dc.TextOutW(first_loc + 20, second_loc, cs);

		has_drawn = true;
	}
	//}
}
