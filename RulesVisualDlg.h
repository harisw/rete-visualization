#pragma once
#include "Node.h"
#include "BetaNode.h"
#include "AlphaNodeDlg.h"
#include <algorithm>
#ifndef RULESVISUAL_H
#define RULESVISUAL_H

// RulesVisualDlg dialog

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
	vector<Node*> m_NodeList;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg
	void displayAlphaDetail(int x);
	void displayBetaDetail(CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	vector<pair<int, AlphaNode*>> alphaPositions;
	vector<pair<int, BetaNode*>> betaPositions;
	int rad;
	int nodeWidth;
	int nodeHeight;

private:
	void showAlphaWindow(AlphaNode* nodeInput);
};

#endif // !RULESVISUAL_H