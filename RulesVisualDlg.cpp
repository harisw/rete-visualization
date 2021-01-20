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
	rad = 50;
	nodeWidth = 60;
	nodeHeight = 60;

	
	int xStart = 0;
	int yAlpha = 0;
	int yBeta = 100;
	int xBeta = 0;
	int nAlpha = 0;


	int lastXBeta;
	int lastYBeta;
	int xSum = 0;
	HBRUSH redBrush = CreateSolidBrush(0x000000FF);
	HBRUSH blueBrush = CreateSolidBrush(0x00FF0000);
	vector<int> alphaXs;
	HRGN hRgn;

	vector<Node*> unconnectedNode;
	Node* currNode;
	for (int i = 0; i < m_NodeList.size(); i++) {
		currNode = m_NodeList[i];

		if (currNode->getType() == "Alpha") {
			alphaXs.push_back(xStart+20);
			hRgn = CreateRoundRectRgn(xStart, yAlpha, xStart + rad, yAlpha + rad, nodeWidth, nodeHeight);
			
			//STORING POSITION
			currNode->visualPosition = make_pair(xStart, yAlpha);
			nodePositions.push_back(make_pair(xStart, currNode));


			unconnectedNode.push_back(currNode);

			if (nAlpha == 0) {
				xBeta = xStart;
			}
			xSum += xStart;
			nAlpha++;
			xStart += 80;
			lastYBeta = yBeta;
		}
		else {
			vector<int> inputIndexes;
			vector<Node*> nextPairs;
			//DRAWING BETA
			for (int index = 0; index < unconnectedNode.size(); index++) {		//CHECK FOR CONNECTED ALPHA NODES
				nextPairs = unconnectedNode[index]->getNextPairs();
				if ((nextPairs.size() == 1 && nextPairs[0] == currNode) ||
					find(nextPairs.begin(), nextPairs.end(), currNode) != nextPairs.end()) {
					inputIndexes.push_back(index);
				}
			}
			

			//GET x position for BetaNodes FROM connected node's X
			if (inputIndexes.size() == 2)
				xBeta = (unconnectedNode[inputIndexes.front()]->visualPosition.first + unconnectedNode[inputIndexes.back()]->visualPosition.first) / 2;
			else
				xBeta = unconnectedNode[inputIndexes.front()]->visualPosition.first;

			yBeta = max(unconnectedNode[inputIndexes.front()]->visualPosition.second, unconnectedNode[inputIndexes.back()]->visualPosition.second) + 80;

			hRgn = CreateRoundRectRgn(xBeta, yBeta, xBeta + rad, yBeta + rad, nodeWidth, nodeHeight);
			///// END OF DRAWING
		
			currNode->visualPosition = make_pair(xBeta, yBeta);
			nodePositions.push_back(make_pair(xBeta, currNode));


			///// CONNECTING NODES
			if (currNode->getNextPairs().size() > 0)
				unconnectedNode.push_back(currNode);
			else {
				for (int j = 0; j < unconnectedNode.size(); j++) {
					Node* unconnNode = unconnectedNode[j];
					vector<Node*> targetNodes = unconnNode->getNextPairs();
					for (int k = 0; k < targetNodes.size(); k++) {	//CONNECTING WITH NEXT PAIRS
						dc.MoveTo(unconnNode->visualPosition.first+25, unconnNode->visualPosition.second + 40);		//DRAWING LINE
						dc.LineTo(targetNodes[k]->visualPosition.first + 40, targetNodes[k]->visualPosition.second + 40);
					}
				}
				unconnectedNode.clear();
			}

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

BOOL RulesVisualDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN && GetDlgItem(IDC_STATIC)->GetSafeHwnd() == pMsg->hwnd)
		OnLButtonDown(MK_LBUTTON, pMsg->pt);
	return CDialog::PreTranslateMessage(pMsg);
}

//void RulesVisualDlg::displayAlphaDetail(int x)
//{
//	int l = 0;
//	int r = alphaPositions.size() - 1;
//	while (l <= r) {
//		int m = l + (r - l) / 2;
//		if (x >= alphaPositions[m].first && x <= alphaPositions[m].first + (rad*2)) {
//			showAlphaWindow(alphaPositions[m].second);
//			/*wstring nodeInfo(alphaPositions[m].second->justCondition.begin(), alphaPositions[m].second->justCondition.end());
//			MessageBox(nodeInfo.c_str(), L"Judulll", MB_OK);*/
//			return;
//		}
//		else if (x > alphaPositions[m].first)
//			l = m + 1;
//		else
//			r = m - 1;
//	}
//}


void RulesVisualDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	Node* currentNode = findClickedNode(point);
	
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

	while (l <= r) {
		int m = l + (r - l) / 2;
		if (x >= nodePositions[m].first && x <= nodePositions[m].first + (rad*2)) {
			
			while (1) {		//GOES TO LEFTMOST NODE
				if (m == 0 || (x >= nodePositions[m-1].first + (rad * 2) ))
					break;
				
				m--;
			}

			while (1) {		//FIND RIGHT Y position
				if (y >= nodePositions[m].second->visualPosition.second && y <= nodePositions[m].second->visualPosition.second + (rad * 2)) {
					return nodePositions[m].second;
				}
				m++;
				
			}
			
		}
		else if (x > nodePositions[m].first)
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