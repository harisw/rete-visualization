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
END_MESSAGE_MAP()


// SimulationDlg message handlers


BOOL SimulationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	MoveWindow(50, 30, WIND_WIDTH, WIND_HEIGHT);
	//m_alpha_list_ctrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 90);
	m_alpha_list_ctrl.InsertColumn(0, _T("AlphaNode Rules"), LVCFMT_LEFT, 600);
	//m_beta_list_ctrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 90);
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
	//SetTimer(IDT_TIMER_OBJ_SIMU, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
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
			paintMode = 1;
		}
		if (paintMode == 1) {
			populateNodes();
			updateListCtrl();
			InvalidateRect(nodesRect);
		}

		return;
	//case IDT_TIMER_OBJ_SIMU:
	//	if (global_itt >= m_object_location[0].size()) {
	//		//Invalidate(false);
	//		KillTimer(IDT_TIMER_OBJ_SIMU);
	//		return;
	//	}

	//	//if (has_drawn)
	//	paintMode = 2;
	//	InvalidateRect(objRect);

	//	return;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void SimulationDlg::OnPaint()
{
	CClientDC nodesDC(GetDlgItem(IDC_STATIC_NODE));
	if (paintMode == 1) {
		findSizeScaling(nodesDC);

		paintWMNode(nodesDC);

		paintNodeVisual(nodesDC);
	}
	/*else if(paintMode == 2)
		paintObjectVisual();*/

	else if (paintMode == 3) {
		findSizeScaling(nodesDC);

		paintWMNode(nodesDC);

		//paintNodeVisual(nodesDC);
		//paintObjectVisual();
		m_objVisualDlg = new ObjVisualDlg();
		m_objVisualDlg->Create(IDD_ObjVisualDlg);
		m_objVisualDlg->ShowWindow(SW_SHOW);
	}
	paintMode = 0;
	CDialog::OnPaint();
	// TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	
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
	HRGN hRgn;

	vector<Node*> unconnectedNodes;
	Node* currNode;


	dc.SelectObject(blackBrush);
	dc.Ellipse(xWM, yAlpha, xWM + wmRad, yAlpha + wmRad);

	wmPos = CPoint(xWM + wmRad, yAlpha + wmRad);

}

void SimulationDlg::paintNodeVisual(CClientDC &dc)
{
	findSizeScaling(dc);
	int xStart = 0;
	int yAlpha = 0;
	int yBeta = 100;
	int xBeta = 0;
	HBRUSH redBrush = CreateSolidBrush(0x000000FF);
	HBRUSH blueBrush = CreateSolidBrush(0x00FF0000);
	HRGN hRgn;

	vector<Node*> unconnectedNodes;
	Node* currNode;


	yAlpha += distance;

	for (int i = 0; i < m_NodeList.size(); i++) {
		currNode = m_NodeList[i];

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

			currNode->visualPosition = make_pair(xBeta, yBeta);
			nodePositions.push_back(make_pair(xBeta, currNode));
			connectNodes(currNode, unconnectedNodes, dc);
		}

		visualizedNode.push_back(currNode);
	}
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
			//ID = to_wstring(currNode->getID());
			condition = wstring(currNode->justCondition.begin(), currNode->justCondition.end());
			/*nIndex = m_alpha_list_ctrl.InsertItem(0, ID.c_str());
			m_alpha_list_ctrl.SetItemText(nIndex, 1, condition.c_str());*/
			m_alpha_list_ctrl.InsertItem(0, condition.c_str());
		}
		else {
			//ID = to_wstring(currNode->getID());
			condition = wstring(currNode->justCondition.begin(), currNode->justCondition.end());
			/*nIndex = m_beta_list_ctrl.InsertItem(0, ID.c_str());
			m_beta_list_ctrl.SetItemText(nIndex, 1, condition.c_str());*/
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

//void SimulationDlg::fillNodes(Node* currentNode)
//{
//	this->m_NodeList.clear();
//	if (currentNode->getType() == "Alpha") {
//		Node* prevNode = dynamic_cast<AlphaNode*>(currentNode)->getPrevNode().second;
//		iteratePrevNode(prevNode);
//	}
//	else {
//		Node* leftSource = dynamic_cast<BetaNode*>(currentNode)->getLeftConnNode();
//		Node* rightSource = dynamic_cast<BetaNode*>(currentNode)->getRightConnNode();
//		iteratePrevNode(leftSource);
//		iteratePrevNode(rightSource);
//	}
//	cout << currentNode->justCondition << endl;
//	m_NodeList.push_back(currentNode);
//	vector<Node*> nextPairs = currentNode->getNextPairs();
//	for (int j = 0; j < nextPairs.size(); j++) {
//		iterateNextNode(nextPairs[j]);
//	}
//}
//
//void SimulationDlg::iteratePrevNode(Node* currentNode)
//{
//	if (currentNode == NULL)
//		return;
//	if (currentNode->getType() == "Alpha") {
//		Node* prevNode = dynamic_cast<AlphaNode*>(currentNode)->getPrevNode().second;
//		iteratePrevNode(prevNode);
//	}
//	else {
//		Node* leftSource = dynamic_cast<BetaNode*>(currentNode)->getLeftConnNode();
//		Node* rightSource = dynamic_cast<BetaNode*>(currentNode)->getRightConnNode();
//		iteratePrevNode(leftSource);
//		iteratePrevNode(rightSource);
//	}
//	cout << currentNode->justCondition << endl;
//	m_NodeList.push_back(currentNode);
//}

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

void SimulationDlg::paintObjectVisual()
{
	CPaintDC dc(GetDlgItem(IDC_STATIC_OBJECT));
	
	drawObjects(dc);
}

void SimulationDlg::drawCQVessel(CPaintDC& dc)
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

	ReteNet::buildNetNode();

	spatialNodePolygon = SpatialNodeIndexing::getExistingPolygons();

	has_drawn = false;
	return;
}

void SimulationDlg::drawObjects(CPaintDC& dc)
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
					InvalidateRect(objRect);
					Sleep(5);
				}

				global_itt++;
				counter++;
			}

			has_drawn = true;
		}
	//}
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