#pragma once
#include "afxwin.h"
//#include "CameraMgr.h"

// DogRegister 대화 상자

class DogRegister : public CDialogEx
{
	DECLARE_DYNAMIC(DogRegister)

public:
	DogRegister(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DogRegister();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DOGREGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()


private:
	//CameraMgr cameramgr;
	
	CEdit m_editDogname;
	CEdit m_editDogAge;
	CEdit m_editSpecies;
	CEdit m_editAddress;
	CStatic m_picDog;
	UINT m_radio;

	CString m_dogName;
	CString m_dogAge;
	CString m_dogSpecies;
	CString m_dogAddr;
	CString m_gender;
	CImage m_bmpBitmap;

public:
	int Checkarg();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnloadimg();
	afx_msg void OnBnClickedBtnopencamera();
	afx_msg void RadioCtrl(UINT ID);
	afx_msg void OnBnClickedBtnanalysis();

	afx_msg void OnStnClickedStaticname();
};
