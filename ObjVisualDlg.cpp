// ObjVisualDlg.cpp : implementation file
//

#include "pch.h"
#include "RETE_multinode_app.h"
#include "ObjVisualDlg.h"
#include "afxdialogex.h"


// ObjVisualDlg dialog

IMPLEMENT_DYNAMIC(ObjVisualDlg, CDialogEx)

ObjVisualDlg::ObjVisualDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ObjVisualDlg, pParent)
{

}

ObjVisualDlg::~ObjVisualDlg()
{
}

void ObjVisualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ObjVisualDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ObjVisualDlg message handlers


void ObjVisualDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	drawObjects(dc);
	//CDialog::OnPaint();
}


BOOL ObjVisualDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here

	initObjectVisualization();

	hollowBrush.CreateStockObject(HOLLOW_BRUSH);
	greenPen.CreatePen(PS_SOLID, 1, 0x00008000);
	bluePen.CreatePen(PS_SOLID, 1, 0x00FFFF00);
	redPen.CreatePen(PS_SOLID, 1, 0x000000FF);
	MoveWindow(1145, 68, 700, 600);
	initObjectVisualization();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void ObjVisualDlg::drawCQVessel(CPaintDC& dc)
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

void ObjVisualDlg::initObjectVisualization()
{
	/*m_object_location = MFC_FixedMultiThread::objectLocationMap;*/

	for (int i = 0; i < m_object_location.size(); i++) {
		for (int j = 0; j < m_object_location[i].size(); j++) {
			if (m_object_location[i][j].first < min_first)
				min_first = m_object_location[i][j].first;
			if (m_object_location[i][j].first > max_first)
				max_first = m_object_location[i][j].first;

			if (m_object_location[i][j].second < min_second)
				min_second = m_object_location[i][j].second;
			if (m_object_location[i][j].second > max_second)
				max_second = m_object_location[i][j].second;

			m_object_location[i][j].first += xCorrection;
			m_object_location[i][j].second += yCorrection;
		}
	}

	float delta_first = max_first - min_first;
	float scale_first = (max_w - min_w) / delta_first;

	float delta_second = max_second - min_second;
	float scale_second = (max_h - min_h) / delta_second;

	x_norm = scale_first;
	y_norm = scale_second;

	//ReteNet::buildNetNode();

	spatialNodePolygon = SpatialNodeIndexing::getExistingPolygons();

	has_drawn = false;
	return;
}

void ObjVisualDlg::drawObjects(CPaintDC& dc)
{
	drawCQVessel(dc);

	char buff[10];
	//int a = 01;
	int counter = 0;
	dc.SelectObject(&hollowBrush);
	//dc.SelectObject(&redPen);
	//if (!has_drawn) {
	if (m_object_location.size() > 0) {
		//while (global_itt < m_object_location[0].size() && counter < cycle_step) {
		while (global_itt < m_object_location[0].size()) {
			for (int i = 0; i < m_object_location.size(); i++) {
				//float first_loc = m_object_location[i][global_itt].first * x_norm - (max_first - max_w);
				//float second_loc = m_object_location[i][global_itt].second * y_norm - (max_second - max_h);

				float first_loc = m_object_location[i][global_itt].first;
				float second_loc = m_object_location[i][global_itt].second;


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
				//Sleep(74);
				Sleep(24);
			}

			global_itt++;
			counter++;
		}

		has_drawn = true;
	}
	//}
}