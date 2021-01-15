#pragma once
#include "../RETEwithDoubleNodeIndexing/Node.h"

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
	void OnLButtonDown(UINT nFlags, CPoint point);
	vector<pair<int, string>> alphaPositions;
	int rad;
};

#endif // !RULESVISUAL_H