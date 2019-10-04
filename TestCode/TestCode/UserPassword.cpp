// UserPassword.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TestCode.h"
#include "UserPassword.h"
#include "afxdialogex.h"
#include "StorageMgr.h"


// UserPassword ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(UserPassword, CDialogEx)

UserPassword::UserPassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PASSWORD, pParent)
{

}

UserPassword::~UserPassword()
{
}

void UserPassword::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_PW1, m_editPassword1);
    DDX_Control(pDX, IDC_EDIT_PW2, m_editPassword2);
}


BEGIN_MESSAGE_MAP(UserPassword, CDialogEx)
    ON_BN_CLICKED(IDOK, &UserPassword::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &UserPassword::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_PW2, &UserPassword::OnEnChangeEditPw2)
	ON_EN_CHANGE(IDC_EDIT_PW1, &UserPassword::OnEnChangeEditPw1)
	ON_EN_CHANGE(IDC_EDIT6, &UserPassword::OnEnChangeEdit6)
	ON_STN_CLICKED(IDC_STATIC1, &UserPassword::OnStnClickedStatic1)
END_MESSAGE_MAP()


// UserPassword �޽��� ó�����Դϴ�.
CString UserPassword::GetPassword()
{
    return m_strPassword;
}

void UserPassword::OnBnClickedOk()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    //CDialogEx::OnOK();
    ///< get password1
	
	m_editPassword1.GetWindowTextA(m_strPassword);
	//TODO: PW�� Arg ��ȿ�� ����(String Max ����,��ȿ���� ���� ���� ���� ����??)
	if (m_strPassword.GetLength() > 6 && m_strPassword.GetLength() < 11)
	{
		MessageBox(_T("Password ����"), _T("Password1 Valid"), MB_OK);
		ps1 = 1;
	}
	else {
			MessageBox(_T("Password ���̴� 7~10 �̾�� �մϴ�."), _T("Password1 Valid"), MB_ICONERROR);
			
	}
    ///< get password2
	m_editPassword2.GetWindowTextA(m_strPasswordcheck);
	if (m_strPasswordcheck.GetLength() > 6 && m_strPasswordcheck.GetLength() < 11)
	{
		MessageBox(_T("Password ����"), _T("Password2 Valid"), MB_OK);
		ps2 = 1;
	}
	else {
			MessageBox(_T("Password ���̴� 7~10 �̾�� �մϴ�."), _T("Password2 Valid"), MB_ICONERROR);
			
	}
    ///< compare password1 with password2
	if(ps1 == 1 && ps2 == 1) {
		CheckPW(m_strPassword, m_strPasswordcheck);
	}
	if (ps3 == 1) {
		CDialogEx::OnOK();
	}


	
}

int UserPassword::CheckPW(CString str1, CString str2)
{
	//TODO: PW1, PW2 �� 
	str1 = m_strPassword;
	str2 = m_strPasswordcheck;
	
	if (str1.Compare(str2) == 0) {
		//if �� �� �����ϸ� return 1, message popup ��� ������ PW
		MessageBox(_T("Password ���"), _T("Password Check"), MB_OK);
		m_stors.Setm_stor1(m_strPassword); //Setm_stor1:m_stors�� stor1���� password�� ����(store�� ����) 
		return ps3 = 1;
	}
	else {
		//else �� �� �ٸ��� return 0,message popup �Է��Ͻ� Password�� ��ġ���� �ʽ��ϴ�
		MessageBox(_T("�Է��Ͻ� Password�� ��ġ���� �ʽ��ϴ�"), _T("Password Check"), MB_ICONERROR);
		
	}
	
}
void UserPassword::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnCancel();
	//������Ʈ�޴��� �����մϴ�
	/// close file:f.close()
	///or out from GUI
}


void UserPassword::OnEnChangeEditPw2()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void UserPassword::OnEnChangeEditPw1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void UserPassword::OnEnChangeEdit6()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void UserPassword::OnStnClickedStatic1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
