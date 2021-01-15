// RulesVisualDlg.cpp : implementation file
//

#include "pch.h"
#include "RETE_multinode_app.h"
#include "RulesVisualDlg.h"
#include "afxdialogex.h"


// RulesVisualDlg dialog

IMPLEMENT_DYNAMIC(RulesVisualDlg, CDialog)

RulesVisualDlg::RulesVisualDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_RulesVisualDlg, pParent)
{

}

RulesVisualDlg::~RulesVisualDlg()
{
}

void RulesVisualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RulesVisualDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// RulesVisualDlg message handlers


void RulesVisualDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages
	CWnd* pImage = GetDlgItem(IDC_STATIC);
	CRect rc;
	pImage->GetWindowRect(rc);
	int xStart = 0;
	int yAlpha = 0;
	int yBeta = 100;
	rad = 50;
	int xBeta = 0;
	int nAlpha = 0;


	int lastXBeta;
	int lastYBeta;
	int xSum = 0;
	HBRUSH redBrush = CreateSolidBrush(0x000000FF);
	HBRUSH blueBrush = CreateSolidBrush(0x00FF0000);
	vector<int> alphaXs;
	HRGN hRgn;


	for (int i = 0; i < m_NodeList.size(); i++) {
		if (m_NodeList[i]->getType() == "Alpha") {
			alphaXs.push_back(xStart+20);
			hRgn = CreateRoundRectRgn(xStart, yAlpha, xStart + rad, yAlpha + rad, 60, 60);
			//STORING POSITION
			alphaPositions.push_back(make_pair(xStart, m_NodeList[i]->justCondition));

			if (nAlpha == 0) {
				xBeta = xStart;
			}
			xSum += xStart;
			nAlpha++;
			xStart += 80;
			lastYBeta = yBeta;
		}
		else {
			if (!alphaXs.empty()) {
				xBeta = xSum / alphaXs.size();

				hRgn = CreateRoundRectRgn(xBeta, yBeta, xBeta + rad, yBeta + rad, 60, 60);
				for (int j = 0; j < alphaXs.size(); j++) {
					dc.MoveTo(alphaXs[j]+10, yAlpha+40);
					dc.LineTo(xBeta+30, yBeta+30);
				}
				alphaXs.clear(); xSum = 0;
				lastXBeta = xBeta; lastYBeta = yBeta;
			}
			else {
				dc.MoveTo(lastXBeta+30, lastYBeta+40);
				lastYBeta += 80;
				hRgn = CreateRoundRectRgn(lastXBeta, lastYBeta, lastXBeta + rad, lastYBeta + rad, 60, 60);
				
				dc.LineTo(lastXBeta+30, lastYBeta+30);
			}


			nAlpha = 0;
			xBeta += 80;
		}


		HINSTANCE hIns = AfxGetInstanceHandle();
		DeleteObject(hIns);
		if (m_NodeList[i]->getType() == "Alpha")
			FillRgn(pImage->GetDC()->GetSafeHdc(), hRgn, redBrush);
		else
			FillRgn(pImage->GetDC()->GetSafeHdc(), hRgn, blueBrush);

	}

	CDialog::OnPaint();
}


BOOL RulesVisualDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN && GetDlgItem(IDC_STATIC)->GetSafeHwnd() == pMsg->hwnd)
		OnLButtonDown(MK_LBUTTON, pMsg->pt);
	return CDialog::PreTranslateMessage(pMsg);
}

void RulesVisualDlg::displayAlphaDetail(int x)
{
	
	int l = 0;
	int r = alphaPositions.size() - 1;
	while (l <= r) {
		int m = l + (r - l) / 2;
		if (x >= alphaPositions[m].first && x <= alphaPositions[m].first + rad) {
			wstring nodeInfo(alphaPositions[m].second.begin(), alphaPositions[m].second.end());
			MessageBox(nodeInfo.c_str(), L"Judulll", MB_OK);
			return;
		}
		else if (x > alphaPositions[m].first)
			l = m + 1;
		else
			r = m - 1;
	}
}
void RulesVisualDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (point.y <= 100)	//ALPHA NODE
		displayAlphaDetail(point.x);
	CDialog::OnLButtonDown(nFlags, point);
}
