// SimulationThreadDlg.cpp : implementation file
//

#include "pch.h"
#include "RETE_multinode_app.h"
#include "SimulationThreadDlg.h"


// SimulationThreadDlg

IMPLEMENT_DYNCREATE(SimulationThreadDlg, CWinThread)

SimulationThreadDlg::SimulationThreadDlg()
{
}

SimulationThreadDlg::~SimulationThreadDlg()
{
}

void SimulationThreadDlg::Setup(CDialogEx* pDlg, UINT nIDTemplate, int nCmdShow /*=SW_SHOW*/) {
    m_pDlg = pDlg;
    m_nIDTemplate = nIDTemplate;
    m_nCmdShow = nCmdShow;
};
BOOL SimulationThreadDlg::InitInstance() {
    m_pActiveWnd = m_pDlg;
    m_pDlg->Create(m_nIDTemplate, 0); //parent is the desktop
    m_pDlg->ShowWindow(m_nCmdShow);
    return TRUE;
}
int SimulationThreadDlg::ExitInstance() {
    return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(SimulationThreadDlg, CWinThread)
END_MESSAGE_MAP()


// SimulationThreadDlg message handlers
