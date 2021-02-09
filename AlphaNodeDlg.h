#pragma once
#include "AlphaNode.h"

#define A_WIND_WIDTH 1200
#define A_WIND_HEIGHT 650
// AlphaNodeDlg dialog

class AlphaNodeDlg : public CDialog
{
	DECLARE_DYNAMIC(AlphaNodeDlg)

public:
	AlphaNodeDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AlphaNodeDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AlphaNodeDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CPoint wmPos;
	vector<Node*> visualizedNode;
	CPen m_oPen;
	CPen* oldPen;
	int xCorrection;
	int yCorrection;
	int windWidth;
	int windHeight;
	CPoint getPosition(int x, int y);
	void fillNodes(Node* currentNode);
	void iteratePrevNode(Node* currentNode);
	void iterateNextNode(Node* currentNode);
public:
	AlphaNode* currNode;
	CEdit m_content_edit;
	CEdit m_output_edit;
	virtual BOOL OnInitDialog();
	CEdit m_input_edit;
	CEdit m_id_edit;

	vector<pair<int, Node*>> nodePositions;
	Node* findClickedNode(CPoint point);
	void connectNodes(Node*& currNode, vector<Node*>& unconnectedNodes, CClientDC& dc);
	void addNodePosition(Node* nodeInput);
	vector<Node*> m_NodeList;
	vector<BetaNode*> visualizedBeta;

	//SIZE CONSTANTS
	int rad;
	int distance;
	afx_msg void OnPaint();
};
