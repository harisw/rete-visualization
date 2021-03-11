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
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// SimulationDlg message handlers


BOOL SimulationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	MoveWindow(50, 30, WIND_WIDTH, WIND_HEIGHT);
	m_alpha_list_ctrl.InsertColumn(0, _T("AlphaNode Rules"), LVCFMT_LEFT, 600);
	m_beta_list_ctrl.InsertColumn(0, _T("BetaNode Rules"), LVCFMT_LEFT, 1000);

	DWORD dwStyle;
	dwStyle = GetDlgItem(IDC_LIST4)->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_FULLROWSELECT|LVS_REPORT|LVS_EX_GRIDLINES;
	GetDlgItem(IDC_LIST4)->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
	
	dwStyle = GetDlgItem(IDC_LIST3)->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_REPORT | LVS_EX_GRIDLINES;
	GetDlgItem(IDC_LIST3)->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);


	//updateListCtrl();
	
	CClientDC dc(this);
	m_dcMem.CreateCompatibleDC(&dc);
	//initObjectVisualization();

	bluePen.CreatePen(PS_SOLID, 1, 0x00FFFF00);
	redPen.CreatePen(PS_SOLID, 1, 0x000000FF);
	thickPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	thinPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hollowBrush.CreateStockObject(HOLLOW_BRUSH);
	greenPen.CreatePen(PS_SOLID, 1, 0x00008000);

	CWnd* objWnd = GetDlgItem(IDC_STATIC_OBJECT);
	objWnd->GetClientRect(&objRect);
	objRect.top += 30;
	objRect.left += 1095;
	objRect.right += 1095;

	CWnd* nodeWnd = GetDlgItem(IDC_STATIC_NODE);
	nodeWnd->GetClientRect(&nodesRect);
	nodesRect.top += 30;
	nodesRect.left += 1095;
	nodesRect.right += 1095;
	

	SetTimer(IDT_TIMER_0, 3000, NULL);
	paintMode = 3;
	SetTimer(IDT_TIMER_OBJ_SIMU, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void SimulationDlg::appendTextToEditCtrl(string pszText)
{
	CString strLine;
	// add CR/LF to text
	pszText = "\r\n" + pszText;
	
	wstring temp(pszText.begin(), pszText.end());
	int nLength = m_output_ctrl.GetWindowTextLength();
	m_output_ctrl.SetSel(nLength, nLength);
	m_output_ctrl.ReplaceSel(temp.c_str());
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
			paintMode = 1;
		}
		if (paintMode == 1) {
			populateNodes();
			updateListCtrl();
			Invalidate();
			//InvalidateRect(nodesRect);
		}

		break;
	case IDT_TIMER_OBJ_SIMU:
		mp_threadDlg->ResumeThread();
		KillTimer(IDT_TIMER_OBJ_SIMU);
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void SimulationDlg::OnPaint()
{
	CClientDC nodesDC(GetDlgItem(IDC_STATIC_NODE));

	CDialog::OnPaint();

	findSizeScaling(nodesDC);
	//paintWMNode(nodesDC);
	paintNodeVisual(nodesDC);

	if (paintMode == 3) {
		m_objVisualDlg = new ObjVisualDlg();
		m_objVisualDlg->m_object_location = MFC_FixedMultiThread::objectLocationMap;
		ReteNet::buildNetNode();

		mp_threadDlg = (SimulationThreadDlg*)AfxBeginThread(RUNTIME_CLASS(SimulationThreadDlg),
			0, 0, CREATE_SUSPENDED);
		mp_threadDlg->Setup(m_objVisualDlg, IDD_ObjVisualDlg, SW_SHOW);
	}

	paintMode = 0;
	
}

void SimulationDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	Node* currentNode = findClickedNode(point);

	if (currentNode == nullptr)
		return;

	
	if (currentNode->getType() == "Alpha") {
		AlphaNodeDlg* alpha_detail = new AlphaNodeDlg(this);
		alpha_detail->currNode = dynamic_cast<AlphaNode*>(currentNode);
		m_alphaDlgs.push_back(alpha_detail);

		alpha_detail->Create(IDD_AlphaNodeDlg);
		alpha_detail->ShowWindow(SW_SHOW);
	}
	else {
		BetaNodeDlg* beta_detail = new BetaNodeDlg(this);
		beta_detail->currNode = dynamic_cast<BetaNode*>(currentNode);

		m_betaDlgs.push_back(beta_detail);

		beta_detail->Create(IDD_BetaNodeDlg);
		beta_detail->ShowWindow(SW_SHOW);
	}
	CDialog::OnLButtonDown(nFlags, point);
}


void SimulationDlg::paintWMNode(CClientDC &dc)
{

	int xStart = 0;
	int yAlpha = 0;
	int yBeta = 100;
	int xBeta = 0;
	HBRUSH blackBrush = CreateSolidBrush(0x00000000);
	vector<Node*> unconnectedNodes;
	Node* currNode;

	dc.SelectObject(blackBrush);
	dc.Ellipse(xWM, yAlpha, xWM + wmRad, yAlpha + wmRad);

	wmPos = CPoint(xWM + wmRad, yAlpha + wmRad);

}

void SimulationDlg::paintNodeVisual(CClientDC &dc)
{
	
	findSizeScaling(dc);
	getNodesPosition();
	drawConnections(dc);
	drawNodes(dc);
}

void SimulationDlg::updateListCtrl()
{
	Node* currNode;
	int aIndex = 0;
	int bIndex = 0;
	int nIndex;
	wstring ID, condition;
	m_alpha_list_ctrl.DeleteAllItems();
	m_beta_list_ctrl.DeleteAllItems();

	for (int j = 0; j < m_NodeList.size(); j++)
	{
		currNode = m_NodeList[j];
		if (currNode->getType() == "Alpha") {
			condition = wstring(currNode->justCondition.begin(), currNode->justCondition.end());
			m_alpha_list_ctrl.InsertItem(0, condition.c_str());
		}
		else {
			condition = wstring(currNode->justCondition.begin(), currNode->justCondition.end());
			m_beta_list_ctrl.InsertItem(0, condition.c_str());
		}
	}
}

void SimulationDlg::populateNodes()
{
	m_NodeList.clear();
	for (nodeMap::iterator it = ReteNet::triggered_node.begin(); it != ReteNet::triggered_node.end(); ++it) {
		m_NodeList.push_back(it->second);
	}
}


void SimulationDlg::findSizeScaling(CClientDC &dc)
{
	if (m_NodeList.size() < 70) {
		rad = 47;
		distance = 70;
		wmRad = rad + 20;
		xWM = (WIND_WIDTH / 2) - 450;
		xCorrection = 25;
		yCorrection = 40;
		dc.SelectObject(thickPen);
	}
	else if (m_NodeList.size() < 100) {
		rad = 40;
		distance = 55;
		wmRad = rad + 10;
		xWM = (WIND_WIDTH / 2) - 150;
		xCorrection = 15;
		yCorrection = 30;
		dc.SelectObject(thinPen);
	}
	else {
		rad = 35;
		distance = 50;
		wmRad = rad + 5;
		xWM = (WIND_WIDTH / 2);
		xCorrection = 10;
		yCorrection = 20;
		dc.SelectObject(thinPen);
	}
}

void SimulationDlg::getNodesPosition()
{
	int xStart = 50;
	int yAlpha = distance;
	int yBeta = 100;
	int xBeta = 50;

	Node* currNode = nullptr;
	Node* leftInput = nullptr;
	Node* rightInput = nullptr;
	BetaNode* currentBeta = nullptr;

	nodePositions.clear(); //Reset node position
	for (int i = 0; i < m_NodeList.size(); i++) {
		currNode = m_NodeList[i];

		if (currNode->getType() == "Alpha") {

			//STORING POSITION
			currNode->visualPosition = make_pair(xStart, yAlpha);
			nodePositions.push_back(make_pair(xStart, currNode));

			xStart += distance;
		}
		else {
			currentBeta = dynamic_cast<BetaNode*>(currNode); 
			leftInput = currentBeta->leftSourcePair.second;
			rightInput = currentBeta->rightSourcePair.second;

			if (leftInput != nullptr && rightInput != nullptr)
				xBeta = (leftInput->visualPosition.first + rightInput->visualPosition.first) / 2;
			else
				xBeta = leftInput != nullptr ? leftInput->visualPosition.first : rightInput->visualPosition.first;

			yBeta = max(leftInput->visualPosition.second, rightInput->visualPosition.second) + distance;


			CPoint pointCandidate = getPosition(xBeta, yBeta);
			xBeta = pointCandidate.x;
			yBeta = pointCandidate.y;


			currNode->visualPosition = make_pair(xBeta, yBeta);
			nodePositions.push_back(make_pair(xBeta, currNode));
			
		}

		
		leftInput = nullptr;
		rightInput = nullptr;
	}
}

void SimulationDlg::drawConnections(CClientDC& dc)
{
	Node* currNode = nullptr;
	Node* leftInput = nullptr;
	Node* rightInput = nullptr;
	Node* prevNode = nullptr;
	AlphaNode* currentAlpha = nullptr;
	BetaNode* currentBeta = nullptr;
	vector<Node*> nextNodes;
	oldPen = (CPen*)dc.SelectObject(&m_oPen);

	for (int i = 0; i < m_NodeList.size(); i++) {
		currNode = m_NodeList[i];

		if (currNode->getType() == "Alpha") {
			currentAlpha = dynamic_cast<AlphaNode*>(currNode);

			prevNode = currentAlpha->getPrevNode().second;
			if (prevNode != nullptr && prevNode->isVisualized()) {
				dc.MoveTo(prevNode->visualPosition.first + xCorrection, prevNode->visualPosition.second + yCorrection);		//DRAWING LINE
				dc.LineTo(currNode->visualPosition.first + xCorrection, currNode->visualPosition.second + yCorrection);
			}
			nextNodes = currentAlpha->getNextPairs();
			for (int j = 0; j < nextNodes.size(); j++) {
				if (nextNodes[j]->visualPosition == make_pair(0, 0))
					continue;
				dc.MoveTo(currNode->visualPosition.first + xCorrection, currNode->visualPosition.second + yCorrection);
				dc.LineTo(nextNodes[j]->visualPosition.first + xCorrection, nextNodes[j]->visualPosition.second + yCorrection);
			}
		}
		else {
			currentBeta = dynamic_cast<BetaNode*>(currNode);
			leftInput = currentBeta->leftSourcePair.second;
			rightInput = currentBeta->rightSourcePair.second;

			if (leftInput != nullptr && leftInput->isVisualized()) {
				dc.MoveTo(leftInput->visualPosition.first + xCorrection, leftInput->visualPosition.second + yCorrection);
				dc.LineTo(currNode->visualPosition.first + xCorrection, currNode->visualPosition.second + yCorrection);
			}
			if (rightInput != nullptr && rightInput->isVisualized()) {
				dc.MoveTo(rightInput->visualPosition.first + xCorrection, rightInput->visualPosition.second + yCorrection);
				dc.LineTo(currNode->visualPosition.first + xCorrection, currNode->visualPosition.second + yCorrection);
			}
			nextNodes = currentBeta->getNextPairs();
			for (int j = 0; j < nextNodes.size(); j++) {
				if (nextNodes[j]->visualPosition == make_pair(0, 0))
					continue;
				dc.MoveTo(currNode->visualPosition.first + xCorrection, currNode->visualPosition.second + yCorrection);
				dc.LineTo(nextNodes[j]->visualPosition.first + xCorrection, nextNodes[j]->visualPosition.second + yCorrection);
			}
		}

		leftInput = nullptr;
		rightInput = nullptr;
		nextNodes = {};
	}
}

void SimulationDlg::drawNodes(CClientDC& dc)
{
	HBRUSH redBrush = CreateSolidBrush(0x000000FF);
	HBRUSH blueBrush = CreateSolidBrush(0x00FF0000);


	Node* currNode = nullptr;
	CPoint currPosition;
	for (int i = 0; i < m_NodeList.size(); i++) {
		currNode = m_NodeList[i];

		if (currNode->getType() == "Alpha")
			dc.SelectObject(redBrush);
		else
			dc.SelectObject(blueBrush);
		
		currPosition = CPoint(currNode->visualPosition.first, currNode->visualPosition.second);

		dc.Ellipse(currPosition.x, currPosition.y, currPosition.x + rad, currPosition.y + rad);

		visualizedNode.push_back(currNode);
	}
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

void SimulationDlg::OnDestroy()
{
	m_objVisualDlg->PostMessage(WM_QUIT);
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}






//     !!!!!!!!!!!!!!! BACKUP !!!!!!!!!!!
//void SimulationDlg::paintWMNode(CPaintDC& dc)
//{
//
//	int xStart = 0;
//	int yAlpha = 0;
//	int yBeta = 100;
//	int xBeta = 0;
//	HBRUSH blackBrush = CreateSolidBrush(0x00000000);
//
//
//	vector<Node*> unconnectedNodes;
//	Node* currNode;
//
//
//	dc.SelectObject(blackBrush);
//	dc.Ellipse(xWM, yAlpha, xWM + wmRad, yAlpha + wmRad);
//
//	wmPos = CPoint(xWM + wmRad, yAlpha + wmRad);
//
//}
//void SimulationDlg::paintNodeVisual(CPaintDC& dc)
//{
//
//	findSizeScaling(dc);
//	getNodesPosition();
//	drawConnections(dc);
//	drawNodes(dc);
//}
//void SimulationDlg::findSizeScaling(CPaintDC& dc)
//{
//	if (m_NodeList.size() < 70) {
//		rad = 47;
//		distance = 70;
//		wmRad = rad + 20;
//		xWM = (WIND_WIDTH / 2) - 450;
//		xCorrection = 25;
//		yCorrection = 40;
//		dc.SelectObject(thickPen);
//	}
//	else if (m_NodeList.size() < 100) {
//		rad = 40;
//		distance = 55;
//		wmRad = rad + 10;
//		xWM = (WIND_WIDTH / 2) - 150;
//		xCorrection = 15;
//		yCorrection = 30;
//		dc.SelectObject(thinPen);
//	}
//	else {
//		rad = 35;
//		distance = 50;
//		wmRad = rad + 5;
//		xWM = (WIND_WIDTH / 2);
//		xCorrection = 10;
//		yCorrection = 20;
//		dc.SelectObject(thinPen);
//	}
//}
//void SimulationDlg::drawConnections(CPaintDC& dc)
//{
//	Node* currNode = nullptr;
//	Node* leftInput = nullptr;
//	Node* rightInput = nullptr;
//	Node* prevNode = nullptr;
//	AlphaNode* currentAlpha = nullptr;
//	BetaNode* currentBeta = nullptr;
//	vector<Node*> nextNodes;
//	oldPen = (CPen*)dc.SelectObject(&m_oPen);
//
//	for (int i = 0; i < m_NodeList.size(); i++) {
//		currNode = m_NodeList[i];
//
//		if (currNode->getType() == "Alpha") {
//			currentAlpha = dynamic_cast<AlphaNode*>(currNode);
//
//			prevNode = currentAlpha->getPrevNode().second;
//			if (prevNode != nullptr) {
//				dc.MoveTo(prevNode->visualPosition.first + xCorrection, prevNode->visualPosition.second + yCorrection);		//DRAWING LINE
//				dc.LineTo(currNode->visualPosition.first + xCorrection, currNode->visualPosition.second + yCorrection);
//			}
//			nextNodes = currentAlpha->getNextPairs();
//			for (int j = 0; j < nextNodes.size(); j++) {
//				if (nextNodes[j]->visualPosition == make_pair(0, 0))
//					continue;
//				dc.MoveTo(currNode->visualPosition.first + xCorrection, currNode->visualPosition.second + yCorrection);
//				dc.LineTo(nextNodes[j]->visualPosition.first + xCorrection, nextNodes[j]->visualPosition.second + yCorrection);
//			}
//		}
//		else {
//			currentBeta = dynamic_cast<BetaNode*>(currNode);
//			leftInput = currentBeta->leftSourcePair.second;
//			rightInput = currentBeta->rightSourcePair.second;
//
//			if (leftInput != nullptr) {
//				dc.MoveTo(leftInput->visualPosition.first + xCorrection, leftInput->visualPosition.second + yCorrection);
//				dc.LineTo(currNode->visualPosition.first + xCorrection, currNode->visualPosition.second + yCorrection);
//			}
//			if (rightInput != nullptr) {
//				dc.MoveTo(rightInput->visualPosition.first + xCorrection, rightInput->visualPosition.second + yCorrection);
//				dc.LineTo(currNode->visualPosition.first + xCorrection, currNode->visualPosition.second + yCorrection);
//			}
//			nextNodes = currentBeta->getNextPairs();
//			for (int j = 0; j < nextNodes.size(); j++) {
//				if (nextNodes[j]->visualPosition == make_pair(0, 0))
//					continue;
//				dc.MoveTo(currNode->visualPosition.first + xCorrection, currNode->visualPosition.second + yCorrection);
//				dc.LineTo(nextNodes[j]->visualPosition.first + xCorrection, nextNodes[j]->visualPosition.second + yCorrection);
//			}
//		}
//
//		leftInput = nullptr;
//		rightInput = nullptr;
//		nextNodes = {};
//	}
//}
//void SimulationDlg::drawNodes(CPaintDC& dc)
//{
//	HBRUSH redBrush = CreateSolidBrush(0x000000FF);
//	HBRUSH blueBrush = CreateSolidBrush(0x00FF0000);
//
//
//	Node* currNode = nullptr;
//	CPoint currPosition;
//	for (int i = 0; i < m_NodeList.size(); i++) {
//		currNode = m_NodeList[i];
//
//		if (currNode->getType() == "Alpha")
//			dc.SelectObject(redBrush);
//		else
//			dc.SelectObject(blueBrush);
//
//		currPosition = CPoint(currNode->visualPosition.first, currNode->visualPosition.second);
//
//		dc.Ellipse(currPosition.x, currPosition.y, currPosition.x + rad, currPosition.y + rad);
//
//		visualizedNode.push_back(currNode);
//	}
//}