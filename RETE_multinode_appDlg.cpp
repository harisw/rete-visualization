
// RETE_multinode_appDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "RETE_multinode_app.h"
#include "RETE_multinode_appDlg.h"
#include "afxdialogex.h"
#include <fstream>

#define SHOW_RULE_EXAMPLE
#define MINLATITUDE 20
#define MAXLATITUDE 51
#define MINLONGITUDE 110
#define MAXLONGITUDE 141
#define RANGE_FROM_CENTRAL 1

#ifdef _DEBUG
#define new DEBUG_NEW

#define VISUAL_W 800
#define VISUAL_H 300
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRETEmultinodeappDlg dialog



CRETEmultinodeappDlg::CRETEmultinodeappDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RETE_MULTINODE_APP_DIALOG, pParent)
	, m_search_event_capturing(_T(""))
	, m_search_cq(_T(""))
	, m_search_cep(_T(""))
	, m_DupNum(_T(""))
	, m_coordinate_0(_T(""))
	, m_coordinate_1(_T(""))
	, m_coordinate_2(_T(""))
	, m_coordinate_4(_T(""))
	, m_coord_obj_num(_T(""))
	, m_coord_time(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRETEmultinodeappDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_rule_content_edit);
	DDX_Control(pDX, IDC_EDIT3, m_cq_content_edit);
	DDX_Control(pDX, IDC_EDIT5, m_cep_content_edit);
	//DDX_Control(pDX, IDC_EDIT_OBJ_NUM, m_edit_obj_num);
	DDX_Text(pDX, IDC_EDIT2, m_search_event_capturing);
	DDX_Text(pDX, IDC_EDIT4, m_search_cq);
	DDX_Text(pDX, IDC_EDIT6, m_search_cep);
	//DDX_Text(pDX, IDC_EDIT_DUP_RULE, m_DupNum);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, dataFileCtrl);
	DDX_Text(pDX, IDC_EDITCOOR_0, m_coordinate_0);
	DDX_Text(pDX, IDC_EDITCOOR_1, m_coordinate_1);
	DDX_Text(pDX, IDC_EDITCOOR_2, m_coordinate_2);
	//DDX_Text(pDX, IDC_EDITCOOR_3, m_coordinate_4);
	//DDX_Text(pDX, IDC_EDITCOOR_4, m_coordinate_4);
	DDX_Text(pDX, IDC_EDITCOOR_5, m_coord_obj_num);
	DDX_Text(pDX, IDC_EDITCOOR_6, m_coord_time);
	DDX_Control(pDX, IDC_LIST1, m_rule_listctrl);
	DDX_Control(pDX, IDC_COMBO1, m_trajectoryCombo);

}

BEGIN_MESSAGE_MAP(CRETEmultinodeappDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CRETEmultinodeappDlg::OnBnClickedOk)
	//ON_BN_CLICKED(IDC_BUTTON1, &CRETEmultinodeappDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRETEmultinodeappDlg::OnBnClickedButtonInsertRule)
	//ON_EN_CHANGE(IDC_EDIT1, &CRETEmultinodeappDlg::OnEnChangeEdit1)
	//ON_BN_CLICKED(IDC_BUTTON5, &CRETEmultinodeappDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_CQ, &CRETEmultinodeappDlg::OnBnClickedButtonInsertCq)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_CEP, &CRETEmultinodeappDlg::OnBnClickedButtonInsertCep)
	ON_BN_CLICKED(IDC_RULES_50, &CRETEmultinodeappDlg::OnBnClickedRules50)
	ON_BN_CLICKED(IDC_BUTTON_STOP_SIM, &CRETEmultinodeappDlg::OnBnClickedButtonStopSim)
	ON_BN_CLICKED(IDC_BUTTON_START_SIM, &CRETEmultinodeappDlg::OnBnClickedButtonStartSim)
	ON_BN_CLICKED(IDC_RULES_100, &CRETEmultinodeappDlg::OnBnClickedRules100)
	ON_BN_CLICKED(IDC_RULES_70, &CRETEmultinodeappDlg::OnBnClickedRules70)
	ON_BN_CLICKED(IDC_BUTTON_FIND_CQ, &CRETEmultinodeappDlg::OnBnClickedButtonFindCq)
	ON_BN_CLICKED(IDC_BUTTON_FIND_TOP, &CRETEmultinodeappDlg::OnBnClickedButtonFindTop)
	ON_BN_CLICKED(IDC_BUTTON_FIND_CEP, &CRETEmultinodeappDlg::OnBnClickedButtonFindCep)
	ON_BN_CLICKED(IDC_BUTTON_DEL_EC, &CRETEmultinodeappDlg::OnBnClickedButtonDelEc)
	ON_BN_CLICKED(IDC_BUTTON_DEL_CQ, &CRETEmultinodeappDlg::OnBnClickedButtonDelCq)
	ON_BN_CLICKED(IDC_BUTTON_DEL_CEP, &CRETEmultinodeappDlg::OnBnClickedButtonDelCep)
	ON_BN_CLICKED(IDC_RULES_51, &CRETEmultinodeappDlg::OnBnClickedRules51)
	ON_BN_CLICKED(IDC_RULES_52, &CRETEmultinodeappDlg::OnBnClickedRules52)
	ON_BN_CLICKED(IDC_BUTTON_STOP_SIM2, &CRETEmultinodeappDlg::OnBnClickedButtonStopSim2)
	ON_BN_CLICKED(IDC_BUTTON7, &CRETEmultinodeappDlg::OnBnClickedButton7)
//	ON_NOTIFY(BCN_DROPDOWN, IDC_BUTTON_VESSEL, &CRETEmultinodeappDlg::OnDropdownButtonVessel)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_COOR, &CRETEmultinodeappDlg::SetCoordinate_new)

ON_BN_CLICKED(IDC_BUTTON9, &CRETEmultinodeappDlg::OnBnClickedButton9)
ON_BN_CLICKED(IDC_BUTTON10, &CRETEmultinodeappDlg::OnBnClickedButton10)
ON_BN_CLICKED(IDC_SIMU1, &CRETEmultinodeappDlg::OnBnClickedSimu1)
ON_EN_CHANGE(IDC_EDIT6, &CRETEmultinodeappDlg::OnEnChangeEdit6)
ON_BN_CLICKED(IDC_BUTTON12, &CRETEmultinodeappDlg::OnBnClickedButton12)
ON_EN_CHANGE(IDC_EDITCOOR_1, &CRETEmultinodeappDlg::OnEnChangeEditcoor1)
END_MESSAGE_MAP()


// CRETEmultinodeappDlg message handlers

BOOL CRETEmultinodeappDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	// TODO: Add extra initialization here
	//m_rule_combo.SetDroppedWidth(120);
	//m_rule_combo.AddString(L"E. Capturing");
	//m_rule_combo.AddString(L"C. Query");
	//m_rule_combo.AddString(L"C. Event Processing");

	/*m_alpha_tree.InsertColumn(0, _T("Rule"), LVCFMT_LEFT, 400);
	m_beta_tree.InsertColumn(0, _T("Rule"), LVCFMT_LEFT, 400);*/

	is_processing = false;

	m_rule_listctrl.InsertColumn(0, _T("Rules List"), LVCFMT_LEFT, 1000);

	DWORD dwStyle;
	dwStyle = GetDlgItem(IDC_LIST1)->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_REPORT | LVS_EX_GRIDLINES;
	GetDlgItem(IDC_LIST1)->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);

#ifdef SHOW_RULE_EXAMPLE
	//m_rule_combo.SetCurSel(2);
	m_rule_content_edit.SetWindowTextW(L"IF speed > 3 & elevation < 10 & iff = false\r\nTHEN enemyvessel");
	m_cq_content_edit.SetWindowTextW(L"IF plot_dist(rect(123,33,127,35), enemyvessel) & enemyvessel.type=recon\r\nWINDOW range=3, trigger=2\r\nTHEN vesselisinside");
	m_cep_content_edit.SetWindowTextW(L"IF exist(vesselisinside)\r\nWINDOW range=5, trigger=3\r\nTHEN navalresponse");
#endif // SHOW_RULE_EXAMPLE
	
	m_trajectoryCombo.AddString(L"LINE");
	m_trajectoryCombo.AddString(L"BEZIER");
	m_trajectoryCombo.AddString(L"CURVE");

	m_trajectoryCombo.SetCurSel(0);

	SetDlgItemText(IDC_EDITCOOR_0, L"50,50");
	SetDlgItemText(IDC_EDITCOOR_1, L"600,150");
	SetDlgItemText(IDC_EDITCOOR_2, L"100,350");

	SetDlgItemText(IDC_EDITCOOR_5, L"3");
	SetDlgItemText(IDC_EDITCOOR_6, L"50");

	////SET UP VESSEL INPUT
	SetDlgItemText(IDC_EDIT_VESSEL_X, L"300");
	SetDlgItemText(IDC_EDIT_VESSEL_Y, L"150");
	//CRETEmultinodeappDlg::OnBnClickedButton9();

	//SetDlgItemText(IDC_EDIT_VESSEL_X, L"450");
	//SetDlgItemText(IDC_EDIT_VESSEL_Y, L"450");
	//CRETEmultinodeappDlg::OnBnClickedButton9();

	//CRETEmultinodeappDlg::SetCoordinate_new();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRETEmultinodeappDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRETEmultinodeappDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		

		CDialog::OnPaint();
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRETEmultinodeappDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

wstring CRETEmultinodeappDlg::vectToWstr(vector<string>* inp)
{
	string result = "";

	for (int j = 0; j < inp->size(); j++)
		result += inp->at(j);

	return wstring(result.begin(), result.end());
}

void CRETEmultinodeappDlg::updateRuleList()
{
	//queue<string> rulesQueue = ReteNet::rulesQueue;
	string topQ;
	while (!ReteNet::rulesQueue.empty()) {
		topQ = ReteNet::rulesQueue.front();
		m_rule_listctrl.InsertItem(ruleCount, wstring(topQ.begin(), topQ.end()).c_str());
		ruleCount++;
		ReteNet::rulesQueue.pop();
	}
	
	return;
}



void CRETEmultinodeappDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CRETEmultinodeappDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	vector<string> made;
	vector<vector<pair<string, string>>> colMade;

	//vessels
	made = {};
	made.push_back("IF speed>3 & elevation<10 & iff=false");
	made.push_back("THEN enemyvessel");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>3 & elevation<10 & iff=true");
	made.push_back("THEN allyvessel");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//submarines
	made = {};
	made.push_back("IF speed>3 & elevation<0 & iff=false");
	made.push_back("THEN enemysubmarine");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>3 & elevation<0 & iff=true");
	made.push_back("THEN allysubmarine");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//aircraft
	made = {};
	made.push_back("IF speed>10 & elevation>5 & iff=false");
	made.push_back("THEN enemyaircraft");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>10 & elevation>5 & iff=true");
	made.push_back("THEN allyaircraft");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>15 & elevation>5 & iff=false");
	made.push_back("THEN enemymissiles");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>10 & elevation<0 & iff=false");
	made.push_back("THEN enemytorpedoes");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);
}


void CRETEmultinodeappDlg::OnBnClickedButtonInsertRule()
{
	system("cls");
	string then_detect;
	vector<string> master_str;

	int len = m_rule_content_edit.LineLength(m_rule_content_edit.LineIndex(0));
	CString path;
	string temp;
	LPTSTR str_line = path.GetBuffer(len);
	m_rule_content_edit.GetLine(0, str_line, len);
	path.ReleaseBuffer();
	
	int line_count = m_rule_content_edit.GetLineCount();
	
	if(line_count == 0)
		AfxMessageBox(_T("Rule is empty"), MB_OK | MB_ICONSTOP);
	
	temp = CT2A(str_line);
	if (temp.substr(0, 2) != "IF") {
		AfxMessageBox(_T("Invalid rule"), MB_OK | MB_ICONSTOP);
		return;
	}
	master_str.push_back(temp);
	path = L"";
	for (int j = 1; j < line_count; j++) {
		len = m_rule_content_edit.LineLength(m_rule_content_edit.LineIndex(j));
		str_line = path.GetBuffer(len);
		m_rule_content_edit.GetLine(j, str_line, len);
		path.ReleaseBuffer();
		temp = CT2A(str_line);

		master_str.push_back(temp);
		if (temp.substr(0, 4) == "THEN") {
			break;
		}
		path = L"";
	}

	vector<vector<pair<string, string>>> colMade;
	colMade = ReteNet::parseConditionOriginal(master_str);

	ReteNet::growTheNodes(colMade);

	MessageBox(L"Event Capture Rule is inserted", L"Insert Rule", MB_OK);
	m_rule_content_edit.SetWindowTextW(L"");

	if (is_processing) {
		MFC_MultiThread::pause_sim();
		ReteNet::buildNetNode();
		MFC_MultiThread::continue_sim();
	}
	updateRuleList();

	return;

}

void CRETEmultinodeappDlg::SetCoordinate_new() {
	UpdateData(TRUE);

	vector<pair<int, int>> coordList;
	if (m_coordinate_0 != L"") {
		CT2CA pszConvertedAnsiString(m_coordinate_0);
		std::string strStd(pszConvertedAnsiString);
		size_t comma = strStd.find(',');
		coordList.push_back({atoi(strStd.substr(0, comma).c_str()), atoi(strStd.substr(comma+1, strStd.length() - comma).c_str()) });
	}

	if (m_coordinate_1 != L"") {
		CT2CA pszConvertedAnsiString(m_coordinate_1);
		std::string strStd(pszConvertedAnsiString);
		size_t comma = strStd.find(',');
		coordList.push_back({ atoi(strStd.substr(0, comma).c_str()), atoi(strStd.substr(comma + 1, strStd.length() - comma).c_str()) });
	}

	if (m_coordinate_2 != L"") {
		CT2CA pszConvertedAnsiString(m_coordinate_2);
		std::string strStd(pszConvertedAnsiString);
		size_t comma = strStd.find(',');
		coordList.push_back({ atoi(strStd.substr(0, comma).c_str()), atoi(strStd.substr(comma + 1, strStd.length() - comma).c_str()) });
	}

	if (m_coordinate_3 != L"") {
		CT2CA pszConvertedAnsiString(m_coordinate_3);
		std::string strStd(pszConvertedAnsiString);
		size_t comma = strStd.find(',');
		coordList.push_back({ atoi(strStd.substr(0, comma).c_str()), atoi(strStd.substr(comma + 1, strStd.length() - comma).c_str()) });
	}

	if (m_coordinate_4 != L"") {
		CT2CA pszConvertedAnsiString(m_coordinate_4);
		std::string strStd(pszConvertedAnsiString);
		size_t comma = strStd.find(',');
		coordList.push_back({ atoi(strStd.substr(0, comma).c_str()), atoi(strStd.substr(comma + 1, strStd.length() - comma).c_str()) });
	}

	//num of object
	int obj_num = -1;
	if (m_coord_obj_num != L"") {
		CT2CA pszConvertedAnsiString(m_coord_obj_num);
		std::string strStd(pszConvertedAnsiString);
		obj_num = atoi(strStd.c_str());
	}
	else {
		MessageBox(L"PLEASE insert the number of object", 0, MB_OK);
		return;
	}

	//length of time
	int time_len = -1;
	if (m_coord_time != L"") {
		CT2CA pszConvertedAnsiString(m_coord_time);
		std::string strStd(pszConvertedAnsiString);
		time_len = atoi(strStd.c_str());
	}
	else {
		MessageBox(L"PLEASE insert the time length", 0, MB_OK);
		return;
	}

	CString curveSel;
	m_trajectoryCombo.GetLBText(m_trajectoryCombo.GetCurSel(), curveSel);
	GenerateData::Generate(coordList, obj_num, time_len, curveTypes[curveSel]);
	//int a = 10;
	fixed_data_num_of_obj = obj_num;

	MessageBox(L"Data generated sucessfully", L"Congrats", MB_OK);

	UpdateData(FALSE);
}

//void CRETEmultinodeappDlg::OnBnClickedButton5()
//{
//	m_alpha_nodes.clear();
//	m_beta_nodes.clear();
//	ReteNet::getAllNodes(m_alpha_nodes, m_beta_nodes);
//	wstring rule_content = L"";
//
//	m_alpha_tree.DeleteAllItems();
//	for (int index = 0; index < m_alpha_nodes.size(); index++) {
//		rule_content = wstring(m_alpha_nodes[index].begin(), m_alpha_nodes[index].end());
//		m_alpha_tree.InsertItem(index, rule_content.c_str());
//	}
//	
//	m_beta_tree.DeleteAllItems();
//	for (int index = 0; index < m_beta_nodes.size(); index++) {
//		rule_content = wstring(m_beta_nodes[index].begin(), m_beta_nodes[index].end());
//		m_beta_tree.InsertItem(index, rule_content.c_str());
//	}
//}


void CRETEmultinodeappDlg::OnBnClickedButtonInsertCq()
{
	system("cls");
	string then_detect;
	vector<string> master_str;

	int len = m_cq_content_edit.LineLength(m_cq_content_edit.LineIndex(0));
	CString path;
	string temp;
	LPTSTR str_line = path.GetBuffer(len);
	m_cq_content_edit.GetLine(0, str_line, len);
	path.ReleaseBuffer();

	int line_count = m_cq_content_edit.GetLineCount();

	if (line_count == 0)
		AfxMessageBox(_T("Rule is empty"), MB_OK | MB_ICONSTOP);

	temp = CT2A(str_line);
	if (temp.substr(0, 2) != "IF") {
		AfxMessageBox(_T("Invalid rule"), MB_OK | MB_ICONSTOP);
		return;
	}
	master_str.push_back(temp);
	path = L"";
	for (int j = 1; j < line_count; j++) {
		len = m_cq_content_edit.LineLength(m_cq_content_edit.LineIndex(j));
		str_line = path.GetBuffer(len);
		m_cq_content_edit.GetLine(j, str_line, len);
		path.ReleaseBuffer();
		temp = CT2A(str_line);

		master_str.push_back(temp);
		if (temp.substr(0, 4) == "THEN") {
			break;
		}
		path = L"";
	}

	vector<vector<pair<string, string>>> colMade;
	colMade = ReteNet::parseConditionOriginal(master_str);

	ReteNet::growTheNodes(colMade);

	updateRuleList();

	MessageBox(L"Continuous Query Rule is inserted", L"Insert Rule", MB_OK);
	m_cq_content_edit.SetWindowTextW(L"");
	return;
}


void CRETEmultinodeappDlg::OnBnClickedButtonInsertCep()
{
	system("cls");
	string then_detect;
	vector<string> master_str;

	int len = m_cep_content_edit.LineLength(m_cep_content_edit.LineIndex(0));
	CString path;
	string temp;
	LPTSTR str_line = path.GetBuffer(len);
	m_cep_content_edit.GetLine(0, str_line, len);
	path.ReleaseBuffer();

	int line_count = m_cep_content_edit.GetLineCount();

	if (line_count == 0)
		AfxMessageBox(_T("Rule is empty"), MB_OK | MB_ICONSTOP);

	temp = CT2A(str_line);
	if (temp.substr(0, 2) != "IF") {
		AfxMessageBox(_T("Invalid rule"), MB_OK | MB_ICONSTOP);
		return;
	}
	master_str.push_back(temp);

	path = L"";
	for (int j = 1; j < line_count; j++) {
		len = m_cep_content_edit.LineLength(m_cep_content_edit.LineIndex(j));
		str_line = path.GetBuffer(len);
		m_cep_content_edit.GetLine(j, str_line, len);
		path.ReleaseBuffer();
		temp = CT2A(str_line);

		master_str.push_back(temp);
		if (temp.substr(0, 4) == "THEN") {
			break;
		}
		path = L"";
	}

	vector<vector<pair<string, string>>> colMade;
	colMade = ReteNet::parseConditionOriginal(master_str);

	ReteNet::growTheNodes(colMade);

	updateRuleList();
	MessageBox(L"Rule is inserted", L"Insert Rule", MB_OK);
	m_cep_content_edit.SetWindowTextW(L"");
	return;
}


void CRETEmultinodeappDlg::OnBnClickedRules50()
{
	// TODO: Add your control notification handler code here
	system("cls");
	cout << "Rules for checking Aircraft, Vessels, submarines" << endl;

	//BASIC IF ELSE
	vector<string> made;
	vector<vector<pair<string, string>>> colMade;

	//vessels
	made = {};
	made.push_back("IF speed>3 & elevation<10 & iff=false");
	made.push_back("THEN enemyvessel");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>3 & elevation<10 & iff=true");
	made.push_back("THEN allyvessel");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//submarines
	made = {};
	made.push_back("IF speed>3 & elevation<0 & iff=false");
	made.push_back("THEN enemysubmarine");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>3 & elevation<0 & iff=true");
	made.push_back("THEN allysubmarine");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//aircraft
	made = {};
	made.push_back("IF speed>10 & elevation>5 & iff=false");
	made.push_back("THEN enemyaircraft");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>10 & elevation>5 & iff=true");
	made.push_back("THEN allyaircraft");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//SPATIAL IF ELSE --------------------------------------------------------------------------------
	//vessels
	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemyvessel) & enemyvessel.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN navalthreat");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), allyvessel) & allyvessel.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN farnavalally");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), enemyvessel) & enemyvessel.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN navalthreatsevere");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), allyvessel) & allyvessel.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN closeally");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//aircraft
	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemyaircraft) & enemyaircraft.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN airthreat");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), enemyaircraft) & enemyaircraft.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN airthreatsevere");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//submarines
	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemysubmarine) & enemysubmarine.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN underwaterthreat");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), enemysubmarine) & enemysubmarine.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN underwaterthreatsevere");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//Now Exist
	//vessel
	made = {};
	made.push_back("IF exist(navalthreat)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN navalresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(navalthreatsevere)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN urgentnavalresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(airthreat)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN airresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(airthreatsevere)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN urgentairresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(underwaterthreat)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN underwaterresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(underwaterthreatsevere)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN urgentunderwaterresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	MessageBox(L"50 defined rules inserted", L"Insert Pre-Defined Rule", MB_OK);
	return;
}


void CRETEmultinodeappDlg::OnBnClickedButtonStopSim()
{
	// TODO: Add your control notification handler code here
	MFC_MultiThread::stop_sim();

	is_processing = false;
}


void CRETEmultinodeappDlg::OnBnClickedButtonStartSim()
{
	system("cls");

	CString received_num;
	m_edit_obj_num.GetWindowTextW(received_num);

	cout << "start processing" << endl;

	MFC_MultiThread::start_sim(_wtoi(received_num), this->generatedEvent);

	is_processing = true;
	// TODO: Add your control notification handler code here
}


void CRETEmultinodeappDlg::OnBnClickedRules100()
{
	// TODO: Add your control notification handler code here
	system("cls");
	cout << "Rules for checking Aircraft, Vessels, submarines, torpedoes, missiles" << endl;

	//BASIC IF ELSE
	vector<string> made;
	vector<vector<pair<string, string>>> colMade;

	//vessels
	made = {};
	made.push_back("IF speed>3 & elevation<10 & iff=false");
	made.push_back("THEN enemyvessel");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>3 & elevation<10 & iff=true");
	made.push_back("THEN allyvessel");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//submarines
	made = {};
	made.push_back("IF speed>3 & elevation<0 & iff=false");
	made.push_back("THEN enemysubmarine");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>3 & elevation<0 & iff=true");
	made.push_back("THEN allysubmarine");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//aircraft
	made = {};
	made.push_back("IF speed>10 & elevation>5 & iff=false");
	made.push_back("THEN enemyaircraft");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>10 & elevation>5 & iff=true");
	made.push_back("THEN allyaircraft");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>15 & elevation>5 & iff=false");
	made.push_back("THEN enemymissiles");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>10 & elevation<0 & iff=false");
	made.push_back("THEN enemytorpedoes");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//SPATIAL IF ELSE --------------------------------------------------------------------------------
	//vessels
	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemyvessel) & enemyvessel.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN navalthreat");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), allyvessel) & allyvessel.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN farallyvessel");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), enemyvessel) & enemyvessel.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN navalthreatsevere");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), allyvessel) & allyvessel.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN closeallyvessel");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//aircraft
	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemyaircraft) & enemyaircraft.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN airthreat");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), allyaircraft) & allyaircraft.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN farallyaircraft");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), enemyaircraft) & enemyaircraft.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN airthreatsevere");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), allyaircraft) & allyaircraft.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN closeallyaircraft");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//submarines
	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemysubmarine) & enemysubmarine.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN underwaterthreat");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), allysubmarine) & allysubmarine.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN farallysubmarine");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), enemysubmarine) & enemysubmarine.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN underwaterthreatsevere");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), allysubmarine) & allysubmarine.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN closeallysubmarine");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), enemymissiles) & enemymissiles.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN farenemymissiles");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemymissiles) & enemymissiles.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN closeenemymissiles");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(121,30,130,40), enemymissiles) & enemymissiles.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN severeenemymissiles");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemytorpedoes) & enemytorpedoes.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN closeenemytorpedoes");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,43), enemytorpedoes) & enemytorpedoes.type=recon");
	made.push_back("WINDOW range=3, trigger=2");
	made.push_back("THEN farenemytorpedoes");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//Now Exist
	//vessel
	made = {};
	made.push_back("IF exist(navalthreat)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN navalresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(farallyvessel)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN farallynavalresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(navalthreatsevere)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN urgentnavalresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(closeallyvessel)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN closeallynavalresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//aircraft
	made = {};
	made.push_back("IF exist(airthreat)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN airresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(farallyaircraft)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN farairallyresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(airthreatsevere)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN urgentairresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(closeallyaircraft)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN closeairallyresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//submarines
	made = {};
	made.push_back("IF exist(underwaterthreat)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN underwaterresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(farallysubmarine)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN farallyunderwaterresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(underwaterthreatsevere)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN urgentunderwaterresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(closeallysubmarine)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN allysubmarineclosing");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(farenemymissiles)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN farmissilesresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(closeenemymissiles)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN closemissileresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(severeenemymissiles)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN urgenmissileresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(closeenemytorpedoes)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN enemytorpedoesresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(farenemytorpedoes)");
	made.push_back("WINDOW range=6, trigger=5");
	made.push_back("THEN urgenttorpedoesresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	updateRuleList();
	MessageBox(L"100 defined rules inserted", L"Insert Pre-Defined Rule", MB_OK);
	return;
}


void CRETEmultinodeappDlg::OnBnClickedRules70()
{
	// TODO: Add your control notification handler code here
	system("cls");
	cout << "Rules for checking Aircraft, Vessels, submarines" << endl;

	//BASIC IF ELSE
	vector<string> made;
	vector<vector<pair<string, string>>> colMade;

	//vessels
	made = {};
	made.push_back("IF speed>3 & elevation<10 & iff=false");
	made.push_back("THEN enemyvessel");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>3 & elevation<10 & iff=true");
	made.push_back("THEN allyvessel");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//submarines
	made = {};
	made.push_back("IF speed>3 & elevation<0 & iff=false");
	made.push_back("THEN enemysubmarine");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>3 & elevation<0 & iff=true");
	made.push_back("THEN allysubmarine");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//aircraft
	made = {};
	made.push_back("IF speed>10 & elevation>5 & iff=false");
	made.push_back("THEN enemyaircraft");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF speed>10 & elevation>5 & iff=true");
	made.push_back("THEN allyaircraft");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//SPATIAL IF ELSE --------------------------------------------------------------------------------
	//vessels
	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemyvessel) & enemyvessel.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN navalthreat");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), allyvessel) & allyvessel.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN farallyvessel");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), enemyvessel) & enemyvessel.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN navalthreatsevere");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), allyvessel) & allyvessel.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN closeallyvessel");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//aircraft
	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemyaircraft) & enemyaircraft.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN airthreat");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), allyaircraft) & allyaircraft.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN farallyaircraft");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), enemyaircraft) & enemyaircraft.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN airthreatsevere");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), allyaircraft) & allyaircraft.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN closeallyaircraft");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//submarines
	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemysubmarine) & enemysubmarine.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN underwaterthreat");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), allysubmarine) & allysubmarine.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN farallysubmarine");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), enemysubmarine) & enemysubmarine.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN underwaterthreatsevere");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF plot_dist(rect(123,33,127,35), allysubmarine) & allysubmarine.type=recon");
	made.push_back("WINDOW range=5, trigger=5");
	made.push_back("THEN closeallysubmarine");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//Now Exist
	//vessel
	made = {};
	made.push_back("IF exist(navalthreat)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN navalresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(farallyvessel)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN farallynavalresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(navalthreatsevere)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN urgentnavalresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(closeallyvessel)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN closeallynavalresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//aircraft
	made = {};
	made.push_back("IF exist(airthreat)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN airresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(farallyaircraft)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN farairallyresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(airthreatsevere)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN urgentairresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(closeallyaircraft)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN closeairallyresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//submarines
	made = {};
	made.push_back("IF exist(underwaterthreat)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN underwaterresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(farallysubmarine)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN farallyunderwaterresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(underwaterthreatsevere)");
	made.push_back("WINDOW range=10, trigger=10");
	made.push_back("THEN urgentunderwaterresponse");

	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	updateRuleList();
	MessageBox(L"70 defined rules inserted", L"Insert Pre-Defined Rule", MB_OK);
	return;
}


void CRETEmultinodeappDlg::OnBnClickedButtonFindCq()
{
	//MessageBox(L"50 defined rules inserted", L"Insert Pre-Defined Rule", MB_OK);
	UpdateData(true);
	string expression = CT2A(m_search_cq);

	bool isAlpha = false;

	Node* temp = ReteNet::findNode(expression, 0);
	if (temp == NULL) {//find using beta node
		temp = ReteNet::findNode(expression, 1);
		isAlpha = false;
	}
	else {
		isAlpha = true;
	}

	CString shown_expression;

	if (temp == NULL) {//maybe it is wrong
		m_cq_content_edit.SetWindowTextW(L"Invalid expression");
	}
	else {
		shown_expression = temp->justCondition.c_str();
		m_cq_content_edit.SetWindowTextW(shown_expression);
	}
	;
	UpdateData(false);
}


void CRETEmultinodeappDlg::OnBnClickedButtonFindTop()
{
	//MessageBox(L"50 defined rules inserted", L"Insert Pre-Defined Rule", MB_OK);
	UpdateData(true);
	string expression = CT2A(m_search_event_capturing);

	bool isAlpha = false;

	Node* temp = ReteNet::findNode(expression, 0);
	if (temp == NULL) {//find using beta node
		temp = ReteNet::findNode(expression, 1);
		isAlpha = false;
	}
	else {
		isAlpha = true;
	}

	CString shown_expression;

	if (temp == NULL) {//maybe it is wrong
		m_rule_content_edit.SetWindowTextW(L"Invalid expression");
	}
	else {
		if (dynamic_cast<AlphaNode*>(temp)) {
			shown_expression = temp->justCondition.c_str();
			m_rule_content_edit.SetWindowTextW(shown_expression);
		}
		else {
			shown_expression = temp->justCondition.c_str();
			m_rule_content_edit.SetWindowTextW(shown_expression);
		}
	}
	UpdateData(false);
}


void CRETEmultinodeappDlg::OnBnClickedButtonFindCep()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	string expression = CT2A(m_search_cep);

	Node* temp = ReteNet::findNode(expression, 0);
	if (temp == NULL) {//find using beta node
		temp = ReteNet::findNode(expression, 1);
	}

	CString shown_expression;

	if (temp == NULL) {//maybe it is wrong
		m_cep_content_edit.SetWindowTextW(L"Invalid expression");
	}
	else {
		shown_expression = temp->justCondition.c_str();
		m_cep_content_edit.SetWindowTextW(shown_expression);
	}
	UpdateData(false);
}


void CRETEmultinodeappDlg::OnBnClickedButtonDelEc()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	string expression = CT2A(m_search_event_capturing);

	int mode = 0;

	Node* temp = ReteNet::findNode(expression, 0);
	if (temp == NULL) {//find using beta node
		temp = ReteNet::findNode(expression, 1);
		mode = 1;
	}

	CString shown_expression;

	if (temp == NULL) {//maybe it is wrong
		m_rule_content_edit.SetWindowTextW(L"Invalid expression");
	}
	else {
		MFC_MultiThread::pause_sim();
		ReteNet::deleteNode(expression, mode);
		MFC_MultiThread::continue_sim();
		MessageBox(L"Node Deleted", L"Success", MB_OK);
	}
	UpdateData(false);
}


void CRETEmultinodeappDlg::OnBnClickedButtonDelCq()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	string expression = CT2A(m_search_cq);

	int mode = 0;

	Node* temp = ReteNet::findNode(expression, 0);
	if (temp == NULL) {//find using beta node
		temp = ReteNet::findNode(expression, 1);
		mode = 1;
	}

	CString shown_expression;

	if (temp == NULL) {//maybe it is wrong
		m_cq_content_edit.SetWindowTextW(L"Invalid expression");
	}
	else {
		MFC_MultiThread::pause_sim();
		ReteNet::deleteNode(expression, mode);
		MFC_MultiThread::continue_sim();
		MessageBox(L"Node Deleted", L"Success", MB_OK);
	}
	UpdateData(false);
}


void CRETEmultinodeappDlg::OnBnClickedButtonDelCep()
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	UpdateData(true);
	string expression = CT2A(m_search_cep);

	int mode = 0;

	Node* temp = ReteNet::findNode(expression, 0);
	if (temp == NULL) {//find using beta node
		temp = ReteNet::findNode(expression, 1);
		mode = 1;
	}

	CString shown_expression;

	if (temp == NULL) {//maybe it is wrong
		m_cep_content_edit.SetWindowTextW(L"Invalid expression");
	}
	else {
		MFC_MultiThread::pause_sim();
		ReteNet::deleteNode(expression, mode);
		MFC_MultiThread::continue_sim();
		MessageBox(L"Node Deleted", L"Success", MB_OK);
	}
	UpdateData(false);
}


void CRETEmultinodeappDlg::OnBnClickedRules51()
{
	// TODO: Add your control notification handler code here
	cout << "Basic CQ rules" << endl;

	//BASIC IF ELSE
	vector<string> made;
	vector<vector<pair<string, string>>> colMade;

	//vessels
	made = {};
	made.push_back("IF speed>3 & elevation<10 & iff=false");
	made.push_back("THEN enemyvessel");
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);

	//SPATIAL IF ELSE --------------------------------------------------------------------------------
	made = {};
	made.push_back("IF plot_dist(rect(121,31,130,41), enemyvessel) & enemyvessel.type=recon");
	made.push_back("WINDOW range=3, trigger=1");
	made.push_back("THEN navalthreat");
	colMade = ReteNet::parseConditionOriginal(made);

	ReteNet::growTheNodes(colMade);

	//sonar
	made = {};
	made.push_back("IF sonar_dist(config((70,100),10,(125,35)), enemyvessel) &  enemyvessel.type=recon");
	made.push_back("WINDOW range=3, trigger=1");
	made.push_back("THEN navalsonar");
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);

	//radar
	made = {};
	made.push_back("IF radar_dist(point(125, 35), 10, enemyvessel) &  enemyvessel.type=recon");
	made.push_back("WINDOW range=3, trigger=1");
	made.push_back("THEN navalradar");
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);

	//Now Exist
	made = {};
	made.push_back("IF exist(navalthreat)");
	made.push_back("WINDOW range=5, trigger=1");
	made.push_back("THEN navalresponse");

	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(navalsonar)");
	made.push_back("WINDOW range=5, trigger=1");
	made.push_back("THEN navalsonarresponse");

	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);

	made = {};
	made.push_back("IF exist(navalradar)");
	made.push_back("WINDOW range=5, trigger=1");
	made.push_back("THEN navalradarresponse");

	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);

	updateRuleList();
	MessageBox(L"Basic CQ rules inserted", L"Insert Pre-Defined Rule", MB_OK);
	return;
}


void CRETEmultinodeappDlg::OnBnClickedRules52()
{
	UpdateData(true);
	cout << "Rules for checking Aircraft, Vessels, submarines" << endl;

	//BASIC IF ELSE
	vector<string> made;
	vector<vector<pair<string, string>>> colMade;

	//vessels
	made = {};
	made.push_back("IF speed>3 & elevation<10 & iff=false");
	made.push_back("THEN enemyvessel");
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);

	//made = {};
	//made.push_back("IF speed>3 & elevation<10 & iff=true");
	//made.push_back("THEN allyvessel");
	//colMade = ReteNet::parseConditionOriginal(made);

	//ReteNet::growTheNodes(colMade);

	////submarines
	//made = {};
	//made.push_back("IF speed>3 & elevation<0 & iff=false");
	//made.push_back("THEN enemysubmarine");
	//colMade = ReteNet::parseConditionOriginal(made);

	//ReteNet::growTheNodes(colMade);

	//made = {};
	//made.push_back("IF speed>3 & elevation<0 & iff=true");
	//made.push_back("THEN allysubmarine");
	//colMade = ReteNet::parseConditionOriginal(made);

	//ReteNet::growTheNodes(colMade);

	////aircraft
	//made = {};
	//made.push_back("IF speed>10 & elevation>5 & iff=false");
	//made.push_back("THEN enemyaircraft");
	//colMade = ReteNet::parseConditionOriginal(made);

	//ReteNet::growTheNodes(colMade);

	//made = {};
	//made.push_back("IF speed>10 & elevation>5 & iff=true");
	//made.push_back("THEN allyaircraft");
	//colMade = ReteNet::parseConditionOriginal(made);

	//ReteNet::growTheNodes(colMade);


	//duplication time
	int dup = _wtoi(m_DupNum);
	
	
	char buff[10];
	int x_gap = abs(MAXLONGITUDE - MINLONGITUDE) / (float)dup;
	int y_gap = abs(MAXLONGITUDE - MINLONGITUDE) / (float)dup;
	//radar dist
	for (int i = 0; i < dup; i++) {
		made = {};
		//float now_x = MINLONGITUDE + (i * x_gap)) % MAXLONGITUDE;
		//string x = to_string(((MINLONGITUDE + (i * x_gap)) % MAXLONGITUDE));
		//string x = _itoa(((MINLONGITUDE + (i*x_gap)) % MAXLONGITUDE) , buff, 10);
		//string y = _itoa(((MINLATITUDE + (i*y_gap)) % MAXLATITUDE), buff, 10);

		int x_val = (int)Utilities::randomFloat(MINLONGITUDE + RANGE_FROM_CENTRAL, MAXLONGITUDE - RANGE_FROM_CENTRAL);
		int y_val = (int)Utilities::randomFloat(MINLATITUDE + RANGE_FROM_CENTRAL, MAXLATITUDE - RANGE_FROM_CENTRAL);
		string x = _itoa(x_val, buff, 10);
		string y = _itoa(y_val, buff, 10);

		string id = _itoa(i, buff, 10);
		string temp = "IF radar_dist(point("+ x +", "+y+"), 10, enemyvessel) & enemyvessel.type = recon";
		made.push_back(temp);
		made.push_back("WINDOW range=2, trigger=1");
		made.push_back("THEN radarenemyvessel" + id);
		colMade = ReteNet::parseConditionOriginal(made);
		ReteNet::growTheNodes(colMade);
	}
	for (int i = 0; i < dup; i++) {
		made = {};
		string id = _itoa(i, buff, 10);
		made.push_back("IF exist(radarenemyvessel"+ id +")");
		made.push_back("WINDOW range=2, trigger=1");
		made.push_back("THEN radarenemyvesselresp" + id);
		colMade = ReteNet::parseConditionOriginal(made);
		ReteNet::growTheNodes(colMade);
	}

	//sonar dist
	for (int i = 0; i < dup; i++) {
		made = {};
		//float now_x = MINLONGITUDE + (i * x_gap)) % MAXLONGITUDE;
		//string x = to_string(((MINLONGITUDE + (i * x_gap)) % MAXLONGITUDE));
		/*string x = _itoa(((MINLONGITUDE + (i * x_gap)) % MAXLONGITUDE), buff, 10);
		string y = _itoa(((MINLATITUDE + (i * y_gap)) % MAXLATITUDE), buff, 10);*/

		int x_val = (int)Utilities::randomFloat(MINLONGITUDE + RANGE_FROM_CENTRAL, MAXLONGITUDE - RANGE_FROM_CENTRAL);
		int y_val = (int)Utilities::randomFloat(MINLATITUDE + RANGE_FROM_CENTRAL, MAXLATITUDE - RANGE_FROM_CENTRAL);
		string x = _itoa(x_val, buff, 10);
		string y = _itoa(y_val, buff, 10);

		string id = _itoa(i, buff, 10);
		string temp = "IF sonar_dist(config((70,100),10,("+x+","+y+")), enemyvessel) & enemyvessel.type = recon";
		made.push_back(temp);
		made.push_back("WINDOW range=2, trigger=1");
		made.push_back("THEN sonarenemyvessel" + id);
		colMade = ReteNet::parseConditionOriginal(made);
		ReteNet::growTheNodes(colMade);
	}
	for (int i = 0; i < dup; i++) {
		made = {};
		string id = _itoa(i, buff, 10);
		made.push_back("IF exist(sonarenemyvessel" + id + ")");
		made.push_back("WINDOW range=2, trigger=1");
		made.push_back("THEN sonarenemyvesselresp" + id);
		colMade = ReteNet::parseConditionOriginal(made);
		ReteNet::growTheNodes(colMade);
	}

	//plot dist
	for (int i = 0; i < dup; i++) {
		made = {};
		//float now_x = MINLONGITUDE + (i * x_gap)) % MAXLONGITUDE;
		//string x = to_string(((MINLONGITUDE + (i * x_gap)) % MAXLONGITUDE));
		//string xmin = _itoa((((MINLONGITUDE + (i * x_gap)) % MAXLONGITUDE) - range ), buff, 10);
		//string xmax = _itoa((((MINLONGITUDE + (i * x_gap)) % MAXLONGITUDE) + range), buff, 10);
		//string ymin = _itoa((((MINLATITUDE + (i * y_gap)) % MAXLATITUDE) - range), buff, 10);
		//string ymax = _itoa((((MINLATITUDE + (i * y_gap)) % MAXLATITUDE) + range), buff, 10);

		//int x_min = (int)Utilities::randomFloat(MINLONGITUDE + RANGE_FROM_CENTRAL, MAXLONGITUDE - RANGE_FROM_CENTRAL) - RANGE_FROM_CENTRAL;
		//int x_max = (int)Utilities::randomFloat(MINLONGITUDE + RANGE_FROM_CENTRAL, MAXLONGITUDE - RANGE_FROM_CENTRAL) + RANGE_FROM_CENTRAL;
		//int y_min = (int)Utilities::randomFloat(MINLATITUDE + RANGE_FROM_CENTRAL, MAXLATITUDE - RANGE_FROM_CENTRAL) - RANGE_FROM_CENTRAL;
		//int y_max = (int)Utilities::randomFloat(MINLATITUDE + RANGE_FROM_CENTRAL, MAXLATITUDE - RANGE_FROM_CENTRAL) + RANGE_FROM_CENTRAL;

		int x_loc = (int)Utilities::randomFloat(MINLONGITUDE + RANGE_FROM_CENTRAL, MAXLONGITUDE - RANGE_FROM_CENTRAL);
		int y_loc = (int)Utilities::randomFloat(MINLATITUDE + RANGE_FROM_CENTRAL, MAXLATITUDE - RANGE_FROM_CENTRAL);

		string xmin = _itoa(x_loc - RANGE_FROM_CENTRAL, buff, 10);
		string xmax = _itoa(x_loc + RANGE_FROM_CENTRAL , buff, 10);
		string ymin = _itoa(y_loc - RANGE_FROM_CENTRAL, buff, 10);
		string ymax = _itoa(y_loc + RANGE_FROM_CENTRAL, buff, 10);

		string id = _itoa(i, buff, 10);
		string temp = "IF plot_dist(rect("+xmin+","+ymin+","+xmax+","+ymax+"), enemyvessel) & enemyvessel.type=recon";
		made.push_back(temp);
		made.push_back("WINDOW range=2, trigger=1");
		made.push_back("THEN plotenemyvessel" + id);
		colMade = ReteNet::parseConditionOriginal(made);
		ReteNet::growTheNodes(colMade);
	}
	for (int i = 0; i < dup; i++) {
		made = {};
		string id = _itoa(i, buff, 10);
		made.push_back("IF exist(plotenemyvessel" + id + ")");
		made.push_back("WINDOW range=2, trigger=1");
		made.push_back("THEN plotenemyvesselresp" + id);
		colMade = ReteNet::parseConditionOriginal(made);
		ReteNet::growTheNodes(colMade);
	}

	UpdateData(false);
	string s = "Random " + to_string(dup * 3 * 2) +" rules inserted";
	CString CStr(s.c_str());
	MessageBox(CStr, L"Insert Pre-Defined Rule", MB_OK);
	return;
}


void CRETEmultinodeappDlg::OnBnClickedButtonStopSim2()
{
	//this->generatedEvent = this->m_WMSet.getWMInputQueue();
	cout << "DONE" << endl;
}


void CRETEmultinodeappDlg::OnBnClickedButton7()
{
	ReteNet::buildNetNode();
	
	// TODO: Add your control notification handler code here
}


//VESSEL RULE INSERTION
void CRETEmultinodeappDlg::OnBnClickedButton9()
{
	UpdateData(true);
	cout << "Rules for checking Aircraft, Vessels, submarines" << endl;

	//BASIC IF ELSE
	vector<string> made;
	vector<vector<pair<string, string>>> colMade;

	if (first_vessel) {
		//vessels
		made = {};
		made.push_back("IF elevation<10 & iff=false");
		made.push_back("THEN enemyvessel");
		colMade = ReteNet::parseConditionOriginal(made);
		ReteNet::growTheNodes(colMade);

		//made = {};
		//made.push_back("IF speed>3 & elevation<10 & iff=true");
		//made.push_back("THEN allyvessel");
		//colMade = ReteNet::parseConditionOriginal(made);
		//ReteNet::growTheNodes(colMade);

		//submarines
		made = {};
		made.push_back("IF elevation<0 & iff=false");
		made.push_back("THEN enemysubmarine");
		colMade = ReteNet::parseConditionOriginal(made);
		ReteNet::growTheNodes(colMade);
		first_vessel = false;

		//made = {};
		//made.push_back("IF speed>3 & elevation<0 & iff=true");
		//made.push_back("THEN allysubmarine");
		//colMade = ReteNet::parseConditionOriginal(made);
		//ReteNet::growTheNodes(colMade);

		//aircraft
		made = {};
		made.push_back("IF elevation>5 & iff=false");
		made.push_back("THEN enemyaircraft");
		colMade = ReteNet::parseConditionOriginal(made);
		ReteNet::growTheNodes(colMade);
	
		//made = {};
		//made.push_back("IF speed>10 & elevation>5 & iff=true");
		//made.push_back("THEN allyaircraft");
		//colMade = ReteNet::parseConditionOriginal(made);

		//ReteNet::growTheNodes(colMade);
	}

	CString xStr, yStr;
	GetDlgItemText(IDC_EDIT_VESSEL_X, xStr);
	GetDlgItemText(IDC_EDIT_VESSEL_Y, yStr);
	int x_int = _wtoi(xStr);
	int y_int = _wtoi(yStr);
	char buff[10];
	string x, y, temp;

	int radarCounter = 0;
	int sonarCounter = 0;

	//RADAR -------------------------------------
	made = {};
	x_int += 50;
	x = _itoa(x_int, buff, 10);
	y = _itoa(y_int, buff, 10);
	temp = "IF radar_dist(point(" + y + ", " + x + "), 100, enemyaircraft) & enemyaircraft.type = recon";
	made.push_back(temp);
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN radarenemyaircraft"+to_string(vesselNum)+to_string(radarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);
	made = {};
	made.push_back("IF exist(radarenemyaircraft" + to_string(vesselNum) + to_string(radarCounter)+")");
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN radarenemyaircraftresp"+ to_string(vesselNum)+ to_string(radarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);
	radarCounter++;


	x_int -= 100;
	x = _itoa(x_int, buff, 10);
	made = {};
	temp = "IF radar_dist(point(" + y + ", " + x + "), 100, enemyaircraft) & enemyaircraft.type = recon";
	made.push_back(temp);
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN radarenemyaircraft" + to_string(vesselNum) + to_string(radarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);
	made = {};
	made.push_back("IF exist(radarenemyaircraft" + to_string(vesselNum) + to_string(radarCounter) + ")");
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN radarenemyaircraftresp" + to_string(vesselNum) + to_string(radarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);


	//SONAR -------------------------------------
	x_int = _wtoi(xStr);
	y_int = _wtoi(yStr);
	x_int += 50;
	x = _itoa(x_int, buff, 10);
	y = _itoa(y_int, buff, 10);

	made = {};
	temp = "IF sonar_dist(config((0,40),80,(" + y + "," + x + ")), enemyvessel) & enemyvessel.type = recon";
	made.push_back(temp);
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN sonarenemyvessel" + to_string(vesselNum) + to_string(sonarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);
	made = {};
	made.push_back("IF exist(sonarenemyvessel" + to_string(vesselNum) + to_string(sonarCounter)+")");
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN sonarenemyvesselresp" + to_string(vesselNum) + to_string(sonarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);
	sonarCounter++;


	x_int -= 100;
	x = _itoa(x_int, buff, 10);
	y = _itoa(y_int, buff, 10);
	made = {};
	temp = "IF sonar_dist(config((160,200),80,(" + y + "," + x + ")), enemyvessel) & enemyvessel.type = recon";
	made.push_back(temp);
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN sonarenemyvessel" + to_string(vesselNum) + to_string(sonarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);
	made = {};
	made.push_back("IF exist(sonarenemyvessel" + to_string(vesselNum) + to_string(sonarCounter) + ")");
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN sonarenemyvesselresp" + to_string(vesselNum) + to_string(sonarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);
	sonarCounter++;


	x_int += 50;
	x = _itoa(x_int, buff, 10);
	y = _itoa(y_int, buff, 10);
	made = {};
	temp = "IF sonar_dist(config((60,110),80,(" + y + "," + x + ")), enemyvessel) & enemyvessel.type = recon";
	made.push_back(temp);
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN sonarenemyvessel" + to_string(vesselNum) + to_string(sonarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);
	made = {};
	made.push_back("IF exist(sonarenemyvessel" + to_string(vesselNum) + to_string(sonarCounter) + ")");
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN sonarenemyvesselresp" + to_string(vesselNum) + to_string(sonarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);
	sonarCounter++;

	x = _itoa(x_int, buff, 10);
	y = _itoa(y_int, buff, 10);
	made = {};
	temp = "IF sonar_dist(config((250,290),80,(" + y + "," + x + ")), enemyvessel) & enemyvessel.type = recon";
	made.push_back(temp);
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN sonarenemyvessel" + to_string(vesselNum) + to_string(sonarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);
	made = {};
	made.push_back("IF exist(sonarenemyvessel" + to_string(vesselNum) + to_string(sonarCounter) + ")");
	made.push_back("WINDOW range=2, trigger=1");
	made.push_back("THEN sonarenemyvesselresp" + to_string(vesselNum) + to_string(sonarCounter));
	colMade = ReteNet::parseConditionOriginal(made);
	ReteNet::growTheNodes(colMade);
	
	vesselNum++;
	updateRuleList();
	//MessageBox(L"Rules for checking Aircraft, Vessels, and Submarines have been inserted", L"Rules Inserted", MB_OK);
}


void CRETEmultinodeappDlg::OnBnClickedButton10()
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	try
	{
		/*CT2CA pszStr(input_file_addr);
		string filename(pszStr);*/
		CString filename;
		dataFileCtrl.GetWindowText(filename);
		if (filename == L"") {
			MessageBox(L"Please Insert Data File source firstly",
				L"Rules Insertion Error", MB_OK | MB_ICONERROR);
			return;
		}

		CT2CA pszStr(filename);
		string filename_str(pszStr);
		
		fixed_data_num_of_obj = GenerateData::ReadFromFile(filename_str);
	}
	catch (const std::string& e)
	{
		CString cs(e.c_str());
		MessageBox(cs, MB_OK);
	}

	MessageBox(L"Fixed Data included", MB_OK);

	UpdateData(FALSE);
}

void CRETEmultinodeappDlg::OnBnClickedSimu1()
{
	SimulationDlg dlg;
	//dlg.m_NodeList = ReteNet::getCopyNodes();
	ReteNet::triggered_node_ID.resize(ReteNet::GetNumberOfNodes()+1);
	fill(ReteNet::triggered_node_ID.begin(), ReteNet::triggered_node_ID.end(), false);
	MFC_FixedMultiThread::start(fixed_data_num_of_obj);
	dlg.num_of_obj = fixed_data_num_of_obj;
	dlg.DoModal();
}


void CRETEmultinodeappDlg::OnEnChangeEdit6()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CRETEmultinodeappDlg::OnBnClickedButton12()
{
	// TODO: Add your control notification handler code here
}


void CRETEmultinodeappDlg::OnEnChangeEditcoor1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
