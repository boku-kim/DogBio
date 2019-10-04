
// TestCodeDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

#include "my_system.h"

#include "Util.h"


// CTestCodeDlg ��ȭ ����
class CTestCodeDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CTestCodeDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTCODE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    // Run testing code
    CButton m_btnRunTestCode;
    afx_msg void OnBnClickedButtonTest();

    /*
     * @brief check whether pasword file is eixst.
     * @param N/A
     * @retrn If exist return eMOD_ERROR_SUCCESS, else return eMOD_ERROR_FAIL
     * @Note 
    **/
    eModError TestFunction01();
    CEdit m_editPassword;

    Util* m_instUtil;
    afx_msg void OnDestroy();

private:
    bool m_bAlreadySetPasswd;
};
