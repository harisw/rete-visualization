#pragma once
#include "Node.h"
#include "ReteNet.h"
//#include "BetaNode.h"
//#include "AlphaNodeDlg.h"
//#include "BetaNodeDlg.h"
//#include "RulesVisualDlg.h"
//#include "MFC_FixedMultiThread.h"
#include <algorithm>
//#ifndef SIMUDLG_H
//#define SIMUDLG_H

#define NODE_SIZE 25
#define WIND_WIDTH 1800
#define WIND_HEIGHT 1000
#define RAD_CONST 1.25
#define DIST_CONST 0.8
#define IDT_TIMER_0 WM_USER + 200
// SimulationDlg dialog

class SimulationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SimulationDlg)

public:
	SimulationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SimulationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SimulationDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void appendTextToEditCtrl(string pszText);
	int num_of_obj;
	CListCtrl m_alpha_list_ctrl;
	CListCtrl m_beta_list_ctrl;
	vector<Node*> m_NodeList;

	CEdit m_output_ctrl;
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//#endif // !SIMUDLG_H