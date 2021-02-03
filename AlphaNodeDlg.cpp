// AlphaNodeDlg.cpp : implementation file
//

#include "pch.h"
#include "RETE_multinode_app.h"
#include "AlphaNodeDlg.h"
#include "afxdialogex.h"


// AlphaNodeDlg dialog

IMPLEMENT_DYNAMIC(AlphaNodeDlg, CDialog)

AlphaNodeDlg::AlphaNodeDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_AlphaNodeDlg, pParent)
{

}

AlphaNodeDlg::~AlphaNodeDlg()
{
}

void AlphaNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_content_edit);
	DDX_Control(pDX, IDC_EDIT2, m_output_edit);
	DDX_Control(pDX, IDC_EDIT7, m_input_edit);
	DDX_Control(pDX, IDC_EDIT9, m_id_edit);
}


BEGIN_MESSAGE_MAP(AlphaNodeDlg, CDialog)
END_MESSAGE_MAP()


// AlphaNodeDlg message handlers


BOOL AlphaNodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int id = currNode->getID();
	wstring tempID = to_wstring(id);
	m_id_edit.SetWindowTextW(tempID.c_str());

	Node* prevNode = currNode->getPrevNode().second;
	if (prevNode != NULL) {
		wstring tempInp(prevNode->justCondition.begin(), prevNode->justCondition.end());
		m_input_edit.SetWindowTextW(tempInp.c_str());
	}

	wstring temp(currNode->justCondition.begin(), currNode->justCondition.end());
	m_content_edit.SetWindowTextW(temp.c_str());

	vector<Node*> nextPairs = currNode->getNextPairs();
	string pairsText = "- "+nextPairs[0]->justCondition;
	
	for (int j = 1; j < nextPairs.size(); j++) {
		pairsText += "\r\n- ";
		pairsText += nextPairs[j]->justCondition;
	}

	wstring pairsTemp(pairsText.begin(), pairsText.end());
	m_output_edit.SetWindowTextW(pairsTemp.c_str());
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
