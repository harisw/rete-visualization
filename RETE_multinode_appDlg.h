
// RETE_multinode_appDlg.h : header file
//

#pragma once
#include <string>
#include <vector>
#include "ReteNet.h"
#include "MFC_MultiThread.h"
#include "MFC_FixedMultiThread.h"
#include "ObjectVisualization.h"
#include "GenerateData.h"
#include "SimulationDlg.h"

using namespace std;


// CRETEmultinodeappDlg dialog
class CRETEmultinodeappDlg : public CDialogEx
{
// Construction
public:
	CRETEmultinodeappDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RETE_MULTINODE_APP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	wstring vectToWstr(vector<string>* inp);
	int ruleCount = 0;
	void updateRuleList();
	map<CString, int> curveTypes = { {L"LINE", 0}, 
		{L"BEZIER", 1},
		{L"CURVE", 2 } };
public:

	vector<string> m_rulesString;
	bool is_processing;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	//CComboBox m_rule_combo;
	afx_msg void OnBnClickedButtonInsertRule();
	CEdit m_rule_content_edit;
	afx_msg void OnEnChangeEdit1();
	//afx_msg void OnBnClickedButton5();

	CEdit m_cq_content_edit;
	CEdit m_cep_content_edit;
	afx_msg void OnBnClickedButtonInsertCq();
	afx_msg void OnBnClickedButtonInsertCep();
	afx_msg void OnBnClickedRules50();
	CEdit m_edit_obj_num;
	afx_msg void OnBnClickedButtonStopSim();
	afx_msg void OnBnClickedButtonStartSim();
	afx_msg void OnBnClickedRules100();
	afx_msg void OnBnClickedRules70();
	CString m_search_event_capturing;
	CString m_search_cq;
	CString m_search_cep;
	afx_msg void OnBnClickedButtonFindCq();
	afx_msg void OnBnClickedButtonFindTop();
	afx_msg void OnBnClickedButtonFindCep();
	afx_msg void OnBnClickedButtonDelEc();
	afx_msg void OnBnClickedButtonDelCq();
	afx_msg void OnBnClickedButtonDelCep();
	afx_msg void OnBnClickedRules51();
	CString m_DupNum;
	WorkingMemory m_WMSet;
	afx_msg void OnBnClickedRules52();
	afx_msg void OnBnClickedButtonStopSim2();
	queue<EventPtr> generatedEvent;
	afx_msg void OnBnClickedButton7();
	CMFCEditBrowseCtrl dataFileCtrl;
	int fixed_data_num_of_obj = 0;
//	afx_msg void OnDropdownButtonVessel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton6();
	afx_msg void SetCoordinate_new();
	CString m_coordinate_0;
	CString m_coordinate_1;
	CString m_coordinate_2;
	CString m_coordinate_3;
	CString m_coordinate_4;
	CString m_coord_obj_num;
	CString m_coord_time;
	
	afx_msg void OnBnClickedSimu1();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnBnClickedButton12();
	CListCtrl m_rule_listctrl;
	afx_msg void OnEnChangeEditcoor1();
	CComboBox m_trajectoryCombo;
};
