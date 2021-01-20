#pragma once
#include "Node.h"
#include "BetaNode.h"
#include "AlphaNodeDlg.h"
#include "BetaNodeDlg.h"
#include <algorithm>
#ifndef RULESVISUAL_H
#define RULESVISUAL_H
#define NODE_SIZE 25
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
	void addNodePosition(Node* nodeInput);
	vector<Node*> m_NodeList;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg
	//void displayAlphaDetail(int x);
	//void displayBetaDetail(CPoint point);

	void OnLButtonDown(UINT nFlags, CPoint point);
	vector<pair<int, Node*>> nodePositions;
	Node* findClickedNode(CPoint point);
	int rad;
	int nodeWidth;
	int nodeHeight;
private:
	void showAlphaWindow(AlphaNode* nodeInput);
	void showBetaWindow(BetaNode* nodeInput);
};

#endif // !RULESVISUAL_H