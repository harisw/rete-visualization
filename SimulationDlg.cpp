// SimulationDlg.cpp : implementation file
//

#include "pch.h"
#include "RETE_multinode_app.h"
#include "SimulationDlg.h"
#include "afxdialogex.h"


// SimulationDlg dialog

IMPLEMENT_DYNAMIC(SimulationDlg, CDialogEx)

SimulationDlg::SimulationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SimulationDlg, pParent)
{

}

SimulationDlg::~SimulationDlg()
{
}

void SimulationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST2, rule_list_ctrl);
	DDX_Control(pDX, IDC_LIST4, m_alpha_list_ctrl);
	DDX_Control(pDX, IDC_LIST3, m_beta_list_ctrl);
	DDX_Control(pDX, IDC_EDIT1, m_output_ctrl);
}


BEGIN_MESSAGE_MAP(SimulationDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// SimulationDlg message handlers


BOOL SimulationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	MoveWindow(50, 30, WIND_WIDTH, WIND_HEIGHT);
	m_alpha_list_ctrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 90);
	m_alpha_list_ctrl.InsertColumn(1, _T("Condition"), LVCFMT_LEFT, 380);
	m_beta_list_ctrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 90);
	m_beta_list_ctrl.InsertColumn(1, _T("Condition"), LVCFMT_LEFT, 400);

	Node* currNode;
	int aIndex = 0;
	int bIndex = 0;
	int nIndex;
	wstring ID, condition;
	for (int j = 0; j < m_NodeList.size(); j++)
	{
		currNode = m_NodeList[j];
		if (currNode->getType() == "Alpha") {
			ID = to_wstring(currNode->getID());
			condition = wstring(currNode->justCondition.begin(), currNode->justCondition.end());
			nIndex = m_alpha_list_ctrl.InsertItem(0, ID.c_str());
			m_alpha_list_ctrl.SetItemText(nIndex, 1, condition.c_str());
		}
		else {
			ID = to_wstring(currNode->getID());
			condition = wstring(currNode->justCondition.begin(), currNode->justCondition.end());
			nIndex = m_beta_list_ctrl.InsertItem(0, ID.c_str());
			m_beta_list_ctrl.SetItemText(nIndex, 1, condition.c_str());
		}
	}
	
	CClientDC dc(this);
	m_dcMem.CreateCompatibleDC(&dc);

	SetTimer(IDT_TIMER_0, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void SimulationDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void SimulationDlg::appendTextToEditCtrl(string pszText)
{
	CString strLine;
	// add CR/LF to text
	pszText = "\r\n" + pszText;
	//strLine.Format(_T("\r\n%s"), pszText);
	
	wstring temp(pszText.begin(), pszText.end());
	// get the initial text length
	int nLength = m_output_ctrl.GetWindowTextLength();
	// put the selection at the end of text
	m_output_ctrl.SetSel(nLength, nLength);
	// replace the selection
	m_output_ctrl.ReplaceSel(temp.c_str());
	//m_output_ctrl.ReplaceSel(pszText.c_str());
}

void SimulationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	string output;
	switch (nIDEvent)
	{
	case IDT_TIMER_0:
		
		while (!ReteNet::triggered_ev.empty()) {
			output = ReteNet::triggered_ev.front();
			appendTextToEditCtrl(output);
			ReteNet::triggered_ev.pop();
		}
		return;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void SimulationDlg::OnPaint()
{
	CClientDC dc(GetDlgItem(IDC_STATIC));
	/*CWnd* pImage = GetDlgItem(IDC_STATIC);
	CRect rc;
	pImage->GetWindowRect(rc);*/
	//CPen* oldPen;

	int wmRad, xWM;

	//rad = WIND_WIDTH / (m_NodeList.size() * RAD_CONST);
	//distance = (WIND_WIDTH / m_NodeList.size()) * DIST_CONST;
	if (m_NodeList.size() < 70) {
		rad = 47;
		distance = 70;
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

		//std::cout << i << endl;
		if (i >= 30)
			std::cout << "sini" << endl;

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
					dc.LineTo(prevBeta->visualPosition.first + xCorrection, prevBeta->visualPosition.second + yCorrection);		//DRAWING LINE
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

			cout << i + 1 << ".  Beta" << endl;

		}

		visualizedNode.push_back(currNode);
	}					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	CDialog::OnPaint();
}

CPoint SimulationDlg::getPosition(int x, int y)
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

void SimulationDlg::connectNodes(Node*& currNode, vector<Node*>& unconnectedNodes, CClientDC& dc)
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

bool comparator(pair<int, Node*> a, pair<int, Node*> b) {
	return a.second->visualPosition.first < b.second->visualPosition.first;
}

Node* SimulationDlg::findClickedNode(CPoint point)
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