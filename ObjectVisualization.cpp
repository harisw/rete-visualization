// ObjectVisualization.cpp : implementation file
//

#include "pch.h"
#include "RETE_multinode_app.h"
#include "ObjectVisualization.h"
#include "afxdialogex.h"


// ObjectVisualization dialog

IMPLEMENT_DYNAMIC(ObjectVisualization, CDialog)

ObjectVisualization::ObjectVisualization(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ObjectVisualization, pParent)
{

}

ObjectVisualization::~ObjectVisualization()
{
}

void ObjectVisualization::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ObjectVisualization, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ObjectVisualization message handlers


BOOL ObjectVisualization::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// TODO:  Add extra initialization here
	m_object_location = MFC_FixedMultiThread::objectLocationMap;

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
		}
	}

	float delta_first = max_first - min_first;
	float scale_first = (max_w - min_w) / delta_first;

	float delta_second = max_second - min_second;
	float scale_second = (max_h - min_h) / delta_second;

	x_norm = scale_first;
	y_norm = scale_second;

	ReteNet::buildNetNode();

	spatialNodePolygon = SpatialNodeIndexing::getExistingPolygons();

	//cannot think
	color_hex_map["Aqua"] = 0x00FFFF00;
	color_hex_map["Black"] = 0x00000000;
	color_hex_map["Blue"] = 0x00FF0000;
	color_hex_map["Cream"] = 0x00F0FBFF;
	color_hex_map["Grey"] = 0x00808080;
	color_hex_map["Fuchsia"] = 0x00FF00FF;
	color_hex_map["Green"] = 0x00008000;
	color_hex_map["LimeGreen"] = 0x0000FF00;
	color_hex_map["Maroon"] = 0x00000080;
	color_hex_map["Navy"] = 0x00800000;
	color_hex_map["OliveGreen"] = 0x00008080;
	color_hex_map["Purple"] = 0x00008080;
	color_hex_map["Red"] = 0x000000FF;
	color_hex_map["Silver"] = 0x00C0C0C0;
	color_hex_map["Teal"] = 0x00808000;
	color_hex_map["White"] = 0x00FFFFFF;

	has_drawn = false;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void ObjectVisualization::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages
	int rad = 10;

	CPen pen_blue, pen_red;
	CBrush brush;
	// Magenta pen
	//pen.CreatePen()

	pen_blue.CreatePen(PS_SOLID, 1, 0x00FFFF00);
	pen_red.CreatePen(PS_SOLID, 1, 0x000000FF);
	//pen.CreatePen(PS_SOLID, 1, );
	// "transparent" brush
	brush.CreateStockObject(HOLLOW_BRUSH);
	dc.SelectObject(&brush);


	drawCQVessel(dc);

	char buff[10];
	//int a = 01;
	int counter = 0;
	if (!has_drawn) {
		if (m_object_location.size() > 0) {
			while (global_itt < m_object_location[0].size() && counter < cycle_step) {

				for (int i = 0; i < m_object_location.size(); i++) {
					//float first_loc = m_object_location[i][global_itt].first * x_norm - (max_first - max_w);
					//float second_loc = m_object_location[i][global_itt].second * y_norm - (max_second - max_h);

					float first_loc = m_object_location[i][global_itt].first;
					float second_loc = m_object_location[i][global_itt].second;

					if (i == 0) {
						//pen.CreatePen(PS_SOLID, 1, color_hex_map["Aqua"]);
						//pen.CreatePen(PS_SOLID, 1, 0x00FFFF00);
						dc.SelectObject(&pen_blue);
						if (global_itt % 10 == 0) {
							CString cs(_itoa(global_itt, buff, 10));
							dc.TextOutW(first_loc + 20, second_loc, cs);
						}

						rad = 2;
					}
					else {
						//pen.CreatePen(PS_SOLID, 1, color_hex_map["Red"]);
						//pen.CreatePen(PS_SOLID, 1, 0x000000FF);
						dc.SelectObject(&pen_red);
						rad = 2;
					}

					// rectangle with magenta frame and "transparent" background
					dc.Ellipse(first_loc - rad, second_loc - rad, first_loc + rad, second_loc + rad);
					//dc.Rectangle(0, 0, m_object_location[0]., 20);

					Invalidate();
					Sleep(20);
				}

				global_itt++;
				counter++;
			}

			has_drawn = true;
		}
	}
}

void ObjectVisualization::drawCQVessel(CPaintDC& dc)
{
	CPen pen_green;
	pen_green.CreatePen(PS_SOLID, 1, 0x00008000);
	dc.SelectObject(&pen_green);
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