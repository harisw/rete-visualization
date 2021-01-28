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
	ON_BN_CLICKED(IDC_BUTTON1, &RulesVisualDlg::OnBnClickedButton1)
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


	int wmRad;
	int xWM;
	//rad = WIND_WIDTH / (m_NodeList.size() * RAD_CONST);
	//distance = (WIND_WIDTH / m_NodeList.size()) * DIST_CONST;
	if (m_NodeList.size() < 70) {
		rad = 80;
		distance = 120;
		wmRad = rad + 20;
		xWM = (WIND_WIDTH / 2) - 450;
	}
	else if (m_NodeList.size() < 100) {
		rad = 55;
		distance = 68;
		wmRad = rad + 10;
		xWM = (WIND_WIDTH / 2) - 150;
	}
	else {
		rad = 45;
		distance = 50;
		wmRad = rad + 5;
		xWM = (WIND_WIDTH / 2);
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

	
	hRgn = CreateRoundRectRgn(xWM, yAlpha, xWM + wmRad, yAlpha + wmRad, wmRad, wmRad);

	wmPos = CPoint(xWM + wmRad, yAlpha+wmRad);
	HINSTANCE hIns = AfxGetInstanceHandle();
	DeleteObject(hIns);
	FillRgn(pImage->GetDC()->GetSafeHdc(), hRgn, blackBrush);

	yAlpha += distance;

	for (int i = 0; i < m_NodeList.size(); i++) {
		currNode = m_NodeList[i];

		if (currNode->getType() == "Alpha") {
			hRgn = CreateRoundRectRgn(xStart, yAlpha, xStart + rad, yAlpha + rad, rad, rad);
			
			//STORING POSITION
			currNode->visualPosition = make_pair(xStart, yAlpha);
			nodePositions.push_back(make_pair(xStart, currNode));


			unconnectedNodes.push_back(currNode);
			xStart += distance;
		}
		else {
			vector<int> inputIndexes;
			vector<Node*> nextPairs;
			//DRAWING BETA
			for (int index = 0; index < unconnectedNodes.size(); index++) {		//CHECK FOR CONNECTED ALPHA NODES
				nextPairs = unconnectedNodes[index]->getNextPairs();
				if ((nextPairs.size() == 1 && nextPairs[0] == currNode) ||
					find(nextPairs.begin(), nextPairs.end(), currNode) != nextPairs.end()) {
					inputIndexes.push_back(index);
				}
			}
			

			//GET x position for BetaNodes FROM connected node's X
			if (inputIndexes.size() == 2)
				xBeta = (unconnectedNodes[inputIndexes.front()]->visualPosition.first + unconnectedNodes[inputIndexes.back()]->visualPosition.first) / 2;
			else
				xBeta = unconnectedNodes[inputIndexes.front()]->visualPosition.first;

			yBeta = max(unconnectedNodes[inputIndexes.front()]->visualPosition.second, unconnectedNodes[inputIndexes.back()]->visualPosition.second) + distance;

			/*CPoint pointCandidate(xBeta, yBeta);

			while (findClickedNode(pointCandidate) != nullptr) {
				yBeta += distance;
				pointCandidate =  CPoint(xBeta, yBeta);
			}*/


			hRgn = CreateRoundRectRgn(xBeta, yBeta, xBeta + rad, yBeta + rad, rad, rad);
			///// END OF DRAWING
		
			currNode->visualPosition = make_pair(xBeta, yBeta);
			nodePositions.push_back(make_pair(xBeta, currNode));

			connectNodes(currNode, unconnectedNodes, dc);
		}


		HINSTANCE hIns = AfxGetInstanceHandle();
		DeleteObject(hIns);
		if (currNode->getType() == "Alpha")
			FillRgn(pImage->GetDC()->GetSafeHdc(), hRgn, redBrush);
		else
			FillRgn(pImage->GetDC()->GetSafeHdc(), hRgn, blueBrush);

	}

	CDialog::OnPaint();
}

void RulesVisualDlg::connectNodes(Node* &currNode, vector<Node*> &unconnectedNodes, CPaintDC &dc)
{
	///// CONNECTING NODES
	if (currNode->getNextPairs().size() > 0)
		unconnectedNodes.push_back(currNode);
	else {
		for (int j = 0; j < unconnectedNodes.size(); j++) {
			Node* unconnNode = unconnectedNodes[j];
			vector<Node*> targetNodes = unconnNode->getNextPairs();
			for (int k = 0; k < targetNodes.size(); k++) {	//CONNECTING WITH NEXT PAIRS
				dc.MoveTo(unconnNode->visualPosition.first + 25, unconnNode->visualPosition.second + 40);		//DRAWING LINE
				dc.LineTo(targetNodes[k]->visualPosition.first + 40, targetNodes[k]->visualPosition.second + 40);
			}
		}
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
	float yDist = 2.5 * rad * (windowW / (windowH * 2));


	while (l <= r) {
		int m = l + (r - l) / 2;
		xMin = nodePositions[m].first;
		xMax = xMin + xDist;
		cout << "min : " << xMin << " max : " << xMax << endl;
		if (x >= xMin && x <= xMax) {
			
			while (1) {		//GOES TO LEFTMOST NODE

				if (m == 0 || (x >= nodePositions[m-1].first + xDist ) )
					break;
				
				m--;
			}

			while (1) {		//FIND RIGHT Y position
				xMin = nodePositions[m].first;
				xMax = xMin + xDist;
				yMin = nodePositions[m].second->visualPosition.second + 10;
				yMax = yMin + yDist;

				if (x >= xMin && x <= xMax
					&& y >= (yMin) && y <= yMax) {
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

BOOL RulesVisualDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CClientDC dc(this);
	m_dcMem.CreateCompatibleDC(&dc);
	//m_vbar.ShowWindow(false);  //Hide Vertical Scroll Bar
	m_hbar.ShowWindow(false);  //Hide Horizontal Scroll Bar
	MoveWindow(100, 100, WIND_WIDTH, WIND_HEIGHT);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void RulesVisualDlg::OnBnClickedButton1()
{
	/*RulesVisualDlg* modeless_dlg = new RulesVisualDlg(this);
	modeless_dlg->Create();
	modeless_dlg->ShowWindow(SW_SHOW);*/
	// TODO: Add your control notification handler code here
}
