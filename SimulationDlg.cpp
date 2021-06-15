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
	//DDX_Control(pDX, IDC_LIST4, m_alpha_list_ctrl);
	DDX_Control(pDX, IDC_LIST3, m_beta_list_ctrl);
	DDX_Control(pDX, IDC_EDIT1, m_output_ctrl);
}


BEGIN_MESSAGE_MAP(SimulationDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &SimulationDlg::OnBnClickedButton1)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// SimulationDlg message handlers


BOOL SimulationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	ShowWindow(SW_SHOWMAXIMIZED);
	CWnd* parent = this->GetParent();
	parent->GetWindowRect(&windRect);

	m_beta_list_ctrl.InsertColumn(0, _T("Event Triggered"), LVCFMT_LEFT, 1000);
	DWORD dwStyle;
	dwStyle = GetDlgItem(IDC_LIST3)->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_REPORT | LVS_EX_GRIDLINES | WS_CLIPCHILDREN;
	GetDlgItem(IDC_LIST3)->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
	
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
	/*nodesRect.left += 1095;
	nodesRect.right += 1095;*/
	
	m_output_ctrl.ShowScrollBar(SB_VERT, TRUE);
	blinkPen.CreatePen(PS_DOT, 3, RGB(255, 255, 255));

	redBrush.CreateSolidBrush(0x000000FF);
	blueBrush.CreateSolidBrush(0x00FF0000);
	greenBrush.CreateSolidBrush(0x0000FF00);
	highlightedBrush.CreateSolidBrush(0x00EA51FF);
	

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
	case IDT_TIMER_VISNODE:
		paintMode = 3;
		InvalidateRect(nodesRect);
		if (ReteNet::is_execution_done)
			KillTimer(IDT_TIMER_VISNODE);
		updateListCtrl();
		break;
	case IDT_TIMER_OBJ_SIMU:
		if (global_itt >= m_object_location[0].size()) {
			//Invalidate(false);
			m_beta_list_ctrl.DeleteAllItems();
			updateListCtrl();
			KillTimer(IDT_TIMER_OBJ_SIMU);
			return;
		}
		paintMode = 2;
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

	CClientDC nodesDC(GetDlgItem(IDC_STATIC_NODE));
	if (paintMode == 1) {
		m_NodeList = ReteNet::getCopyNodes();
		paintNodeVisual(nodesDC);
		//drawObjects();
		SetTimer(IDT_TIMER_VISNODE, 1500, NULL);
		SetTimer(IDT_TIMER_OBJ_SIMU, 1000, NULL);
	}
	/*if(paintMode == 2)
		drawObjects();*/
	if (paintMode == 3 || highlightMode) {
		//m_NodeList = ReteNet::getCopyNodes();
		paintNodeVisual(nodesDC);
		fill(ReteNet::triggered_node_ID.begin(), ReteNet::triggered_node_ID.end(), false);
	}

	paintMode = 0;
	highlightMode = false;
}

void SimulationDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_output_ctrl.Clear();
	
	Node* currentNode = findClickedNode(point);

	if (currentNode == nullptr) {
		wstring output_txt = L"No Output";
		m_output_ctrl.SetWindowTextW(output_txt.c_str());
		return;
	}

	highlighted_NodeID.insert(currentNode->SuperNodeID);
	m_output_ctrl.Clear();
	string output = "";
	if (currentNode->getType() == "Alpha") {
		AlphaNode* currentAlpha = dynamic_cast<AlphaNode*>(currentNode);

		output += "ID : " + to_string(currentNode->getID())+"\r\n\r\n";
		Node* prevNode = currentAlpha->getPrevNode().second;
		if (prevNode != NULL) {
			highlighted_NodeID.insert(prevNode->SuperNodeID);
			output += "Input : " + prevNode->justCondition + "\r\n\r\n";
		}
		else
			output += "Input : \r\n";

		output += "Content : " + currentNode->justCondition + "\r\n\r\n";


		output += "Next Pairs : \r\n";
		vector<Node*> nextPairs = currentNode->getNextPairs();
		output += "- " + nextPairs[0]->justCondition;

		for (int j = 1; j < nextPairs.size(); j++) {
			output += "\r\n- ";
			output += nextPairs[j]->justCondition;
		}

	}
	else {
		BetaNode* currentBeta = dynamic_cast<BetaNode*>(currentNode);

		output += "ID : " + to_string(currentNode->getID()) + "\r\n\r\n";
		
		if(currentBeta->leftSourcePair.second != NULL)
			output += "Left Input : "+ currentBeta->getLeftConnName() + "\r\n";
		if (currentBeta->rightSourcePair.second != NULL)
			output += "Right Input : "+ currentBeta->getRightConnName() + "\r\n\r\n";

		output += "Content : " + currentNode->justCondition + "\r\n\r\n";


		output += "Next Pairs : \r\n";
		vector<Node*> nextPairs = currentNode->getNextPairs();

		if (nextPairs.size() > 0) {
			output += "- " + nextPairs[0]->justCondition;
			for (int j = 1; j < nextPairs.size(); j++) {
				output += "\r\n- ";
				output += nextPairs[j]->justCondition;
			}
		}

		if(currentBeta->leftSourcePair.second != NULL)
			highlighted_NodeID.insert(currentBeta->leftSourcePair.second->SuperNodeID);
		if (currentBeta->rightSourcePair.second != NULL)
			highlighted_NodeID.insert(currentBeta->rightSourcePair.second->SuperNodeID);
	}
	wstring output_txt(output.begin(), output.end());
	m_output_ctrl.SetWindowTextW(output_txt.c_str());

	//repaint highlighted
	highlightMode = true;
	InvalidateRect(nodesRect);

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
	if (paintMode == 1) {

		getNodesPosition();
	}
	drawConnections(dc);
	drawNodes(dc);
	lastUpdateIndex = m_NodeList.size();
	nodeUpdate = false;
	node_first_draw = false;
	highlighted_NodeID.clear();

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
			m_beta_list_ctrl.InsertItem(0, condition.c_str());
	}
	lastTriggeredVect = triggered_vect.size();
}

void SimulationDlg::findSizeScaling(CClientDC &dc)
{
	dc.SelectObject(thickPen);
	int windWidth = windRect.Width();
	if (m_NodeList.size() < 70) {
		rad = 47;
		yDistance = 100;
		distance = 50;
		wmRad = rad + 20;
		xWM = (windWidth / 2) - 450;
		xCorrection = 25;
		yCorrection = 40;
	}
	else if (m_NodeList.size() < 100) {
		rad = 40;
		distance = 55;
		wmRad = rad + 10;
		xWM = (windWidth / 2) - 150;
		xCorrection = 15;
		yCorrection = 30;
	}
	else {
		rad = 35;
		distance = 50;
		wmRad = rad + 5;
		xWM = (windWidth / 2);
		xCorrection = 10;
		yCorrection = 20;
	}
}

void SimulationDlg::getNodesPosition()
{
	int yAlpha = distance;
	int xBeta, yBeta;

	Node* currNode = nullptr;
	Node* leftInput = nullptr;
	Node* rightInput = nullptr;
	BetaNode* currentBeta = nullptr;

	//nodePositions.clear(); //Reset node position
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

			if (leftInput != nullptr && rightInput != nullptr) {
				xBeta = (leftInput->visualPosition.first + rightInput->visualPosition.first) / 2;
				yBeta = max(leftInput->visualPosition.second, rightInput->visualPosition.second) + yDistance;
			}
			else {
				xBeta = leftInput != nullptr ? leftInput->visualPosition.first : rightInput->visualPosition.first;
				yBeta = leftInput != nullptr ? leftInput->visualPosition.second : rightInput->visualPosition.second; 
				yBeta += yDistance;
			}


			


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
	//oldPen = (CPen*)dc.SelectObject(&m_oPen);

	for (int i = 0; i < m_NodeList.size(); i++) {
		currNode = m_NodeList[i];

		if (highlightMode && highlighted_NodeID.find(currNode->SuperNodeID) == highlighted_NodeID.end())	//SKIP IF NOT HIGHLIGHTED in highlight mode
			continue;

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
				//if (highlightMode && highlighted_NodeID.find(nextNodes[j]->SuperNodeID) == highlighted_NodeID.end())
				//	continue;
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
				/*if (highlightMode && highlighted_NodeID.find(nextNodes[j]->SuperNodeID) == highlighted_NodeID.end())
					continue;*/
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
	Node* currNode = nullptr;
	CPoint currPosition;
	oldPen = (CPen*)dc.SelectObject(&m_oPen);
	for (int i = 0; i < m_NodeList.size(); i++) {
		currNode = m_NodeList[i];

		
		if(highlightMode && highlighted_NodeID.find(currNode->SuperNodeID) != highlighted_NodeID.end())		//PAINT FOR HIGHLIGHTED NODE
			dc.SelectObject(highlightedBrush);
		else if (ReteNet::triggered_node_ID[currNode->SuperNodeID])
			dc.SelectObject(greenBrush);
		else {
			if (currNode->getType() == "Alpha")
				dc.SelectObject(redBrush);
			else
				dc.SelectObject(blueBrush);
		}

		currPosition = CPoint(currNode->visualPosition.first, currNode->visualPosition.second);

		dc.Ellipse(currPosition.x, currPosition.y, currPosition.x + rad, currPosition.y + rad);

		visualizedNode.push_back(currNode);
		
		dc.SelectObject(oldPen);
	}
}

CPoint SimulationDlg::getPosition(int x, int y)
{
	//return CPoint(x, y);
	CPoint candidate1(x, y + (rad / 4)), candidate2(x + rad, y + (rad / 4));
	if (findClickedNode(candidate1) == nullptr && findClickedNode(candidate2) == nullptr)
		return CPoint(x, y);

	x += (distance / 3);
	y += (yDistance / 2.5);
	candidate1 = CPoint(x, y + (rad / 4));
	candidate2 = CPoint(x + rad, y + (rad / 4));

	while (findClickedNode(candidate1) != nullptr || findClickedNode(candidate2) != nullptr) {
		x += (distance / 2);
		y += (yDistance / 2);
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
	float windowH = windRect.Height(); float windowW = windRect.Width();
	float xDist = 2 * rad * (windowH / windowW);
	float yDist = 3 * rad * (windowW / (windowH * 5));

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

	//has_drawn = false;
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
	int drawn = false;
	m_beta_list_ctrl.DeleteAllItems();	//RESET OUTPUT

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
				Sleep(304);
				//Sleep(84);
			}


			global_itt++;
			counter++;
		}
		CString cs(_itoa(global_itt, buff, 10));
		dc.TextOutW(first_loc + 20, second_loc, cs);
		//has_drawn = true;
	}
}

void SimulationDlg::OnBnClickedButton1()
{
	paintMode = 3;
	InvalidateRect(nodesRect);
	// TODO: Add your control notification handler code here
}


BOOL SimulationDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;
	return CDialogEx::OnEraseBkgnd(pDC);
}
