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
END_MESSAGE_MAP()


// SimulationDlg message handlers


BOOL SimulationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	MoveWindow(50, 30, WIND_WIDTH, WIND_HEIGHT);
	m_alpha_list_ctrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 90);
	m_alpha_list_ctrl.InsertColumn(1, _T("Condition"), LVCFMT_LEFT, 380);
	m_beta_list_ctrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 90);
	m_beta_list_ctrl.InsertColumn(1, _T("Condition"), LVCFMT_LEFT, 400);

	Node* currNode;
	int aIndex = 0;
	int bIndex = 0;
	int nIndex;
	wstring ID, condition;
	for (int j = 0; j < m_NodeList.size(); j++)
	{
		currNode = m_NodeList[j];
		if (currNode->getType() == "Alpha") {
			ID = to_wstring(currNode->getID());
			condition = wstring(currNode->justCondition.begin(), currNode->justCondition.end());
			nIndex = m_alpha_list_ctrl.InsertItem(0, ID.c_str());
			m_alpha_list_ctrl.SetItemText(nIndex, 1, condition.c_str());
		}
		else {
			ID = to_wstring(currNode->getID());
			condition = wstring(currNode->justCondition.begin(), currNode->justCondition.end());
			nIndex = m_beta_list_ctrl.InsertItem(0, ID.c_str());
			m_beta_list_ctrl.SetItemText(nIndex, 1, condition.c_str());
		}
	}
	//Utilities::output_ctrl = m_output_ctrl;
	//MFC_FixedMultiThread::start(num_of_obj);
	SetTimer(IDT_TIMER_0, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void SimulationDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
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
		}
		return;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
