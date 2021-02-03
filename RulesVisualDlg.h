#pragma once
#include "Node.h"
#include "BetaNode.h"
#include "AlphaNodeDlg.h"
#include "BetaNodeDlg.h"
#include "RulesVisualDlg.h"
#include <algorithm>
#ifndef RULESVISUAL_H
#define RULESVISUAL_H
#define NODE_SIZE 25
#define WIND_WIDTH 1700
#define WIND_HEIGHT 800
#define RAD_CONST 1.25
#define DIST_CONST 0.8
// RulesVisualDlg dialog

struct LinkedNode {
	Node* node;
	LinkedNode* next = nullptr;
};

class RulesVisualDlg : public CDialog
{
	DECLARE_DYNAMIC(RulesVisualDlg)

public:
	RulesVisualDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RulesVisualDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RulesVisualDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	void connectNodes(Node*& currNode, vector<Node*>& unconnectedNodes, CPaintDC& dc);
	void addNodePosition(Node* nodeInput);
	vector<Node*> m_NodeList;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg
	//void displayAlphaDetail(int x);
	//void displayBetaDetail(CPoint point);

	void OnLButtonDown(UINT nFlags, CPoint point);
	vector<pair<int, Node*>> nodePositions;
	Node* findClickedNode(CPoint point);
	
	//SIZE CONSTANTS
	int rad;
	int distance;

	//TRYING SCROLLBAR
	SCROLLINFO horz, vert;
	int sourcex, sourcey, offsetx, offsety;
	CDC m_dcMem;

private:
	CPoint wmPos;
	vector<Node*> visualizedNode;
	void showAlphaWindow(AlphaNode* nodeInput);
	void showBetaWindow(BetaNode* nodeInput);
public:
	CScrollBar m_hbar;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};

#endif // !RULESVISUAL_H