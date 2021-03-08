#pragma once



// SimulationThreadDlg

class SimulationThreadDlg : public CWinThread
{
	DECLARE_DYNCREATE(SimulationThreadDlg)

protected:
	SimulationThreadDlg();           // protected constructor used by dynamic creation
	virtual ~SimulationThreadDlg();

public:
	void Setup(CDialogEx* pDlg, UINT nIDTemplate, int nCmdShow);
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CDialogEx* m_pDlg;
	UINT     m_nIDTemplate;
	int      m_nCmdShow;
protected:
	DECLARE_MESSAGE_MAP()
};


