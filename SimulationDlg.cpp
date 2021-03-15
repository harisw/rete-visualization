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

	m_object_location = MFC_FixedMultiThread::objectLocationMap;
	ReteNet::buildNetNode();
	initObjectVisualization();

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
	
	m_output_ctrl.ShowScrollBar(SB_VERT, TRUE);

	//SetTimer(IDT_TIMER_0, 500, NULL);
	paintMode = 3;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void SimulationDlg::appendTextToEditCtrl(string pszText)
{
	CString strLine;
	// add CR/LF to text
	pszText = "\r\n" + pszText;
	
	eventCounter++;


	wstring temp(pszText.begin(), pszText.end());
	//wstring temp(to_wstring(eventCounter) + L"\r\n");
	int nLength = m_output_ctrl.GetWindowTextLength();
	m_output_ctrl.SetSel(nLength, nLength);
	//temp = eventCounter + L" " + temp;
	m_output_ctrl.ReplaceSel(temp.c_str());
	/*if (eventCounter % 5 == 0)
		cout << "Event : " << eventCounter << endl;*/
}

void SimulationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	string output;


	switch (nIDEvent)
	{
	//case IDT_TIMER_0:
	//	 
	//	while (!ReteNet::triggered_ev.empty()) {
	//		
	//		output = ReteNet::triggered_ev.front();
	//		appendTextToEditCtrl(output);
	//		ReteNet::triggered_ev.pop();
	//		paintMode = 1;
	//	}
	//	if (paintMode == 1) {
	//		updateNodes();
	//		if (!nodeUpdate && !initialRete)
	//			break;
	//		updateListCtrl();
	//		CClientDC nodesDC(GetDlgItem(IDC_STATIC_NODE));
	//		paintNodeVisual(nodesDC);
	//		initialRete = false;
	//		//InvalidateRect(nodesRect); //right one
	//	}


	//	break;
	case IDT_TIMER_OBJ_SIMU:
		if (global_itt >= m_object_location[0].size()) {
			//Invalidate(false);
			KillTimer(IDT_TIMER_OBJ_SIMU);
			return;
		}
		drawObjects();
		//if (has_drawn)
		//paintMode = 2;
		//Invalidate(objRect);
		//KillTimer(IDT_TIMER_OBJ_SIMU);
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void SimulationDlg::OnPaint()
{
	CDialog::OnPaint();


	if (paintMode == 3) {
		SetTimer(IDT_TIMER_OBJ_SIMU, 100, NULL);
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
	//paintWMNode(nodesDC);
	getNodesPosition();
	drawConnections(dc);
	drawNodes(dc);
	lastUpdateIndex = m_NodeList.size();
	nodeUpdate = false;
}

void SimulationDlg::updateListCtrl()
{
	Node* currNode;
	int aIndex = 0;
	int bIndex = 0;
	int nIndex;
	wstring ID, condition;
	vector<Node*> triggered_vect = ReteNet::triggered_node_vect;
	for (int j = lastTriggeredVect; j < triggered_vect.size(); j++)
	{
		currNode = triggered_vect[j];
		condition = wstring(currNode->justCondition.begin(), currNode->justCondition.end());

		if (currNode->getType() == "Alpha")
			m_alpha_list_ctrl.InsertItem(0, condition.c_str());
		else
			m_beta_list_ctrl.InsertItem(0, condition.c_str());
	}
	lastTriggeredVect = triggered_vect.size();
	/*for (int j = 0; j < m_NodeList.size(); j++)
	{
		currNode = m_NodeList[j];
		condition = wstring(currNode->justCondition.begin(), currNode->justCondition.end());

		if (currNode->getType() == "Alpha")
			m_alpha_list_ctrl.InsertItem(0, condition.c_str());
		else
			m_beta_list_ctrl.InsertItem(0, condition.c_str());
	}*/
}

void SimulationDlg::updateNodes()
{	
	for (nodeMap::iterator it = ReteNet::triggered_node.begin(); it != ReteNet::triggered_node.end(); ++it) {
		if (!initialRete) {
			if (find(m_NodeList.begin(), m_NodeList.end(), it->second) == m_NodeList.end()) {
				nodeUpdate = true;
				m_NodeList.push_back(it->second);
			}
		}
		else {
			m_NodeList.push_back(it->second);
		}
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
	int yAlpha = distance;
	int yBeta = 100;
	int xBeta = 50;

	Node* currNode = nullptr;
	Node* leftInput = nullptr;
	Node* rightInput = nullptr;
	BetaNode* currentBeta = nullptr;

	nodePositions.clear(); //Reset node position
	for (int i = lastUpdateIndex; i < m_NodeList.size(); i++) {
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

	for (int i = lastUpdateIndex; i < m_NodeList.size(); i++) {
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
	for (int i = lastUpdateIndex; i < m_NodeList.size(); i++) {
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
	
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}

void SimulationDlg::drawCQVessel(CClientDC& dc)
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

void SimulationDlg::initObjectVisualization()
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

void SimulationDlg::drawObjects()
{
	CClientDC dc(GetDlgItem(IDC_STATIC_OBJECT));
	drawCQVessel(dc);

	char buff[10];
	//int a = 01;
	int counter = 0;
	dc.SelectObject(&hollowBrush);
	float first_loc = 0;
	float second_loc = 0;
	string output;
	CClientDC nodesDC(GetDlgItem(IDC_STATIC_NODE));
	if (m_object_location.size() > 0) {
		while (global_itt < m_object_location[0].size() && counter < cycle_step) {
		//while (global_itt < m_object_location[0].size()) {
			for (int i = 0; i < m_object_location.size(); i++) {
				//float first_loc = m_object_location[i][global_itt].first * x_norm - (max_first - max_w);
				//float second_loc = m_object_location[i][global_itt].second * y_norm - (max_second - max_h);

				first_loc = m_object_location[i][global_itt].first;
				second_loc = m_object_location[i][global_itt].second;

				if (i == 0) {
					dc.SelectObject(&bluePen);
#ifdef DEBUGGING_MODE
					CString cs(_itoa(global_itt, buff, 10));
					dc.TextOutW(first_loc + 20, second_loc, cs);
#endif // DEBUGGING_MODE
#ifndef DEBUGGING_MODE
					if (global_itt % 10 == 0) {
						CString cs(_itoa(global_itt, buff, 10));
						dc.TextOutW(first_loc + 20, second_loc, cs);
					}
#endif // !DEBUGGING_MODE

					rad = 2;
				}
				else {
					dc.SelectObject(&redPen);
					rad = 2;
				}
				dc.Ellipse(first_loc - rad, second_loc - rad, first_loc + rad, second_loc + rad);
				//Invalidate();
				Sleep(84);
				//Sleep(24);
			}

			while (!ReteNet::triggered_ev.empty()) {
				
				output = ReteNet::triggered_ev.front();
				appendTextToEditCtrl(output);
				ReteNet::triggered_ev.pop();
				paintMode = 1;
			}
			if (paintMode == 1) {
				updateNodes();
				updateListCtrl();
				if (nodeUpdate || initialRete) {
					
					paintNodeVisual(nodesDC);
					initialRete = false;
				}
				//InvalidateRect(nodesRect); //right one
			}

			global_itt++;
			counter++;
		}

		CString cs(_itoa(global_itt, buff, 10));
		dc.TextOutW(first_loc + 20, second_loc, cs);
		has_drawn = true;
	}
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