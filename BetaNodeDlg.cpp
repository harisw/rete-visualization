// BetaNodeDlg.cpp : implementation file
//

#include "pch.h"
#include "RETE_multinode_app.h"
#include "BetaNodeDlg.h"
#include "afxdialogex.h"


// BetaNodeDlg dialog

IMPLEMENT_DYNAMIC(BetaNodeDlg, CDialog)

BetaNodeDlg::BetaNodeDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_BetaNodeDlg, pParent)
{

}

BetaNodeDlg::~BetaNodeDlg()
{
}

void BetaNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_left_edit);
	DDX_Control(pDX, IDC_EDIT2, m_right_edit);
	DDX_Control(pDX, IDC_EDIT7, m_content_edit);
	DDX_Control(pDX, IDC_EDIT8, m_output_edit);
	DDX_Control(pDX, IDC_EDIT9, m_id_edit);
}


BEGIN_MESSAGE_MAP(BetaNodeDlg, CDialog)
END_MESSAGE_MAP()


// BetaNodeDlg message handlers


BOOL BetaNodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int id = currNode->getID();
	wstring tempID = to_wstring(id);
	m_id_edit.SetWindowTextW(tempID.c_str());

	string lConn = currNode->getLeftConnName();
	wstring tempL(lConn.begin(), lConn.end());
	m_left_edit.SetWindowTextW(tempL.c_str());

	string rConn = currNode->getRightConnName();
	wstring tempR(rConn.begin(), rConn.end());
	m_right_edit.SetWindowTextW(tempR.c_str());

	wstring temp(currNode->justCondition.begin(), currNode->justCondition.end());
	m_content_edit.SetWindowTextW(temp.c_str());

	vector<Node*> nextPairs = currNode->getNextPairs();
	if (nextPairs.size() > 0) {
		string pairsText = "- " + nextPairs[0]->justCondition;

		for (int j = 1; j < nextPairs.size(); j++) {
			pairsText += "\r\n- ";
			pairsText += nextPairs[j]->justCondition;
		}

		wstring tempOut(pairsText.begin(), pairsText.end());
		m_output_edit.SetWindowTextW(tempOut.c_str());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
