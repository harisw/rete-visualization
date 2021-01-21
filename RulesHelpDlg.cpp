// RulesHelpDlg.cpp : implementation file
//

#include "pch.h"
#include "RETE_multinode_app.h"
#include "RulesHelpDlg.h"
#include "afxdialogex.h"


// RulesHelpDlg dialog

IMPLEMENT_DYNAMIC(RulesHelpDlg, CDialog)

RulesHelpDlg::RulesHelpDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_RulesHelpDlg, pParent)
{

}

RulesHelpDlg::~RulesHelpDlg()
{
}

void RulesHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RulesHelpDlg, CDialog)
END_MESSAGE_MAP()


// RulesHelpDlg message handlers


BOOL RulesHelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages
	CWnd* pImage = GetDlgItem(IDC_STATIC);
	CRect rc;
	pImage->GetWindowRect(rc);

	int xStart = 0;
	int yAlpha = 0;
	int rad = 80;
	HBRUSH redBrush = CreateSolidBrush(0x000000FF);
	HBRUSH blueBrush = CreateSolidBrush(0x00FF0000);
	HRGN hRgn;

	hRgn = CreateRoundRectRgn(xStart, yAlpha, xStart + rad, yAlpha + rad, rad, rad);
	HINSTANCE hIns = AfxGetInstanceHandle();
	DeleteObject(hIns);
	FillRgn(pImage->GetDC()->GetSafeHdc(), hRgn, redBrush);

	yAlpha += 100;
	hRgn = CreateRoundRectRgn(xStart, yAlpha, xStart + rad, yAlpha + rad, rad, rad);
	hIns = AfxGetInstanceHandle();
	DeleteObject(hIns);
	FillRgn(pImage->GetDC()->GetSafeHdc(), hRgn, blueBrush);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
