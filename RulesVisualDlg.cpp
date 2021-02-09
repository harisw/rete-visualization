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
	DDX_Control(pDX, IDC_SCROLLBAR2, m_hbar);
}


BEGIN_MESSAGE_MAP(RulesVisualDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// RulesVisualDlg message handlers

BOOL RulesVisualDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CClientDC dc(this);
	m_dcMem.CreateCompatibleDC(&dc);
	//m_vbar.ShowWindow(false);  //Hide Vertical Scroll Bar
	m_hbar.ShowWindow(false);  //Hide Horizontal Scroll Bar
	MoveWindow(100, 30, WIND_WIDTH, WIND_HEIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void RulesVisualDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages
	CClientDC dc(GetDlgItem(IDC_STATIC));
	/*CWnd* pImage = GetDlgItem(IDC_STATIC);
	CRect rc;
	pImage->GetWindowRect(rc);*/
	//CPen* oldPen;

	int wmRad, xWM;

	//rad = WIND_WIDTH / (m_NodeList.size() * RAD_CONST);
	//distance = (WIND_WIDTH / m_NodeList.size()) * DIST_CONST;
	if (m_NodeList.size() < 70) {
		rad = 65;
		distance = 85;
		wmRad = rad + 20;
		xWM = (WIND_WIDTH / 2) - 450;
		xCorrection = 25;
		yCorrection = 40;

		m_oPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	}
	else if (m_NodeList.size() < 100) {
		rad = 40;
		distance = 55;
		wmRad = rad + 10;
		xWM = (WIND_WIDTH / 2) - 150;
		xCorrection = 15;
		yCorrection = 30;

		m_oPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	}
	else {
		rad = 35;
		distance = 50;
		wmRad = rad + 5;
		xWM = (WIND_WIDTH / 2);

		xCorrection = 10;
		yCorrection = 20;
		m_oPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	}
	sqrtRad = sqrt(rad);
	int xStart = 0;
	int yAlpha = 0;
	int yBeta = 100;
	int xBeta = 0;


	HBRUSH redBrush = CreateSolidBrush(0x000000FF);
	HBRUSH blueBrush = CreateSolidBrush(0x00FF0000);
	HBRUSH blackBrush = CreateSolidBrush(0x00000000);
	HRGN hRgn;

	vector<Node*> unconnectedNodes;
	Node* currNode;


	dc.SelectObject(blackBrush);
	dc.Ellipse(xWM, yAlpha, xWM + wmRad, yAlpha + wmRad);

	wmPos = CPoint(xWM + wmRad, yAlpha + wmRad);


	yAlpha += distance;

	for (int i = 0; i < m_NodeList.size(); i++) {
		currNode = m_NodeList[i];

		/*if (i >= 8)
			cout << "sini" << endl;*/

		if (currNode->getType() == "Alpha") {
			dc.SelectObject(redBrush);

			dc.Ellipse(xStart, yAlpha, xStart + rad, yAlpha + rad);

			//STORING POSITION
			currNode->visualPosition = make_pair(xStart, yAlpha);
			nodePositions.push_back(make_pair(xStart, currNode));

			unconnectedNodes.push_back(currNode);

			Node* prevNode = dynamic_cast<AlphaNode*>(currNode)->getPrevNode().second;
			if (prevNode == NULL) { //CONNECT TO WM
				oldPen = (CPen*)dc.SelectObject(&m_oPen);
				dc.MoveTo(wmPos.x - (rad / 2), wmPos.y - (rad / 2));		//DRAWING LINE
				dc.LineTo(xStart + (rad / 2), yAlpha + (rad / 2));
				dc.SelectObject(oldPen);
			}


			else {
				BetaNode* prevBeta = dynamic_cast<BetaNode*>(prevNode);
				if (prevBeta->visualPosition != make_pair(0, 0)) {
					oldPen = (CPen*)dc.SelectObject(&m_oPen);
					dc.MoveTo(xStart + xCorrection, yAlpha + yCorrection);		//DRAWING LINE
					dc.LineTo(prevBeta->visualPosition.first +xCorrection, prevBeta->visualPosition.second + yCorrection);		//DRAWING LINE
					dc.SelectObject(oldPen);
				}
			}

			xStart += distance;
			cout << i + 1 << ".  Alpha" << endl;
		}
		else {
			dc.SelectObject(blueBrush);

			vector<Node*> inputNodes;
			vector<Node*> nextPairs;
			vector<pair<int, Node*>>::iterator itt;
			for (itt = nodePositions.begin(); itt != nodePositions.end(); itt++) {
				nextPairs = itt->second->getNextPairs();
				if ((nextPairs.size() == 1 && nextPairs[0] == currNode) ||
					find(nextPairs.begin(), nextPairs.end(), currNode) != nextPairs.end()) {
					inputNodes.push_back(itt->second);
				}
			}





			if (inputNodes.size() >= 2)
				xBeta = (inputNodes.front()->visualPosition.first + inputNodes[1]->visualPosition.first) / 2;
			else
				xBeta = inputNodes.front()->visualPosition.first;

			yBeta = max(inputNodes.front()->visualPosition.second, inputNodes.back()->visualPosition.second) + distance;


			CPoint pointCandidate = getPosition(xBeta, yBeta);
			xBeta = pointCandidate.x;
			yBeta = pointCandidate.y;

			dc.Ellipse(xBeta, yBeta, xBeta + rad, yBeta + rad);
			///// END OF DRAWING

			currNode->visualPosition = make_pair(xBeta, yBeta);
			nodePositions.push_back(make_pair(xBeta, currNode));

			connectNodes(currNode, unconnectedNodes, dc);
			visualizedBeta.push_back(dynamic_cast<BetaNode*>(currNode));
			cout << i + 1 << ".  Beta" << endl;

		}

		visualizedNode.push_back(currNode);
	}

	CDialog::OnPaint();
}

CPoint RulesVisualDlg::getPosition(int x, int y)
{
	//return CPoint(x, y);
	CPoint candidate1(x, y + (rad / 4)), candidate2(x + rad, y + (rad / 4));
	if (findClickedNode(candidate1) == nullptr && findClickedNode(candidate2) == nullptr)
		return CPoint(x, y);

	x += (distance / 3);
	y += (distance / 2.5);
	candidate1 = CPoint(x, y + (rad / 4));
	candidate2 = CPoint(x + rad, y + (rad / 4));

	while (findClickedNode(candidate1) != nullptr || findClickedNode(candidate2) != nullptr) {
		x += (distance / 2);
		y += (distance / 2);
		candidate1 = CPoint(x, y + (rad / 4));
		candidate2 = CPoint(x + rad, y + (rad / 4));
	}

	return CPoint(x, y);
}

void RulesVisualDlg::connectNodes(Node*& currNode, vector<Node*>& unconnectedNodes, CClientDC& dc)
{
	///// CONNECTING NODES
	if (currNode->getNextPairs().size() > 0)
		unconnectedNodes.push_back(currNode);
	else {
		oldPen = (CPen*)dc.SelectObject(&m_oPen);
		for (int j = 0; j < unconnectedNodes.size(); j++) {
			Node* unconnNode = unconnectedNodes[j];
			vector<Node*> targetNodes = unconnNode->getNextPairs();
			for (int k = 0; k < targetNodes.size(); k++) {	//CONNECTING WITH NEXT PAIRS
				if (targetNodes[k]->visualPosition == make_pair(0, 0))
					continue;
				dc.MoveTo(unconnNode->visualPosition.first + xCorrection, unconnNode->visualPosition.second + yCorrection);		//DRAWING LINE
				dc.LineTo(targetNodes[k]->visualPosition.first + xCorrection, targetNodes[k]->visualPosition.second + yCorrection);
			}
		}
		dc.SelectObject(oldPen);
		unconnectedNodes.clear();
	}
}

BOOL RulesVisualDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN && GetDlgItem(IDC_STATIC)->GetSafeHwnd() == pMsg->hwnd)
		OnLButtonDown(MK_LBUTTON, pMsg->pt);
	return CDialog::PreTranslateMessage(pMsg);
}


void RulesVisualDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	Node* currentNode = findClickedNode(point);

	if (currentNode == nullptr)
		return;
	if (currentNode->getType() == "Alpha")
		showAlphaWindow(dynamic_cast<AlphaNode*>(currentNode));
	else
		showBetaWindow(dynamic_cast<BetaNode*>(currentNode));

	CDialog::OnLButtonDown(nFlags, point);
}




bool comparator(pair<int, Node*> a, pair<int, Node*> b) {
	return a.second->visualPosition.first < b.second->visualPosition.first;
}

Node* RulesVisualDlg::findClickedNode(CPoint point)
{
	sort(nodePositions.begin(), nodePositions.end(), comparator);
	int l = 0;
	int r = nodePositions.size() - 1;
	int x = point.x;
	int y = point.y;
	//return nullptr;

	int xMin, xMax, yMin, yMax;
	float windowH = WIND_HEIGHT; float windowW = WIND_WIDTH;
	float xDist = 2.5 * rad * (windowH / windowW);
	float yDist = 2.5 * rad * (windowW / (windowH * 5));

	//cout << "CLICKED (" << x << ", " << y << ")" << endl;
	while (l <= r) {
		int m = l + (r - l) / 2;
		xMin = nodePositions[m].first;
		xMax = xMin + xDist;
		if (x >= xMin && x <= xMax) {

			while (1) {		//GOES TO LEFTMOST NODE

				if (m == 0 || (x >= nodePositions[m - 1].first + xDist))
					break;

				m--;
			}

			while (1) {		//FIND RIGHT Y position
				xMin = nodePositions[m].first;
				xMax = xMin + xDist;
				yMin = nodePositions[m].second->visualPosition.second + 5;
				yMax = yMin + yDist;

				if (x >= xMin && x <= xMax
					&& y >= (yMin) && y <= yMax) {
					/*cout << "FROM (" << xMin << ", " << yMin << ")" << endl;
					cout << "TO (" << xMax << ", " << yMax << ")" << endl;*/
					return nodePositions[m].second;
				}
				m++;
				if (m == nodePositions.size())
					return nullptr;
			}

		}
		else if (x > xMin)
			l = m + 1;
		else
			r = m - 1;
	}
	return nullptr;
}

void RulesVisualDlg::showAlphaWindow(AlphaNode* nodeInput)
{
	AlphaNodeDlg dialog_alpha;
	dialog_alpha.currNode = nodeInput;
	dialog_alpha.DoModal();
}

void RulesVisualDlg::showBetaWindow(BetaNode* nodeInput)
{
	BetaNodeDlg dialog_beta;
	dialog_beta.currNode = nodeInput;
	dialog_beta.DoModal();
}