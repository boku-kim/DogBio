// UserPassword.cpp: 구현 파일
//

#include "stdafx.h"
#include "DogIdentification.h"
#include "DogIdentificationDlg.h"
#include "UserPassword.h"
#include "afxdialogex.h"
#include "StorageMgr.h"
#include "util.h"

// UserPassword 대화 상자

IMPLEMENT_DYNAMIC(UserPassword, CDialogEx)

UserPassword::UserPassword(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PASSWORD, pParent)
    , m_nSucces(0)
{

}

UserPassword::~UserPassword()
{
}

void UserPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editPassword1);
	DDX_Control(pDX, IDC_EDIT2, m_editPassword2);
}


BEGIN_MESSAGE_MAP(UserPassword, CDialogEx)
	ON_BN_CLICKED(IDOK, &UserPassword::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &UserPassword::OnBnClickedCancel)
END_MESSAGE_MAP()


// UserPassword 메시지 처리기


void UserPassword::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
	///< get password1

	m_editPassword1.GetWindowTextA(m_strPassword);
	//TODO: PW의 Arg 유효성 점검(String Max 길이,유효하지 못한 문자 포함 여부??)
	if (m_strPassword.GetLength() > 6 && m_strPassword.GetLength() < 11)
	{
		///< get password2
		m_editPassword2.GetWindowTextA(m_strPasswordcheck);
		if (CheckPW(m_strPassword, m_strPasswordcheck)) {
			//MessageBox(_T("Password 등록"), _T("Password Check"), MB_OK);

            eModError ret = eMOD_ERROR_SUCCESS;
            char szFilePath[MAX_PATH] = { 0 };
            //CString strCurrentPath;
            //CString strDataPath;
			CString check;

            //CDogIdentificationDlg* pDlg = (CDogIdentificationDlg*)AfxGetMainWnd();
            //pDlg->GetCurrentModulePath(strCurrentPath);
            //strDataPath = MY_PASSWORD_FILE_PATH;
            sprintf_s(szFilePath, sizeof(szFilePath), "%s\\%s", MY_PASSWORD_FILE_PATH, MY_PASSWORD_FILE_NAME);
            StorageMgr* pStorage = StorageMgr::GetInstance();
	
            if (pStorage)
            {
				
				
                ret = pStorage->Store(eSECURITY_TYPE_IMPORTANT, szFilePath, (unsigned char*)(LPTSTR)(LPCTSTR)m_strPassword, m_strPassword.GetLength()*sizeof(char));
				//check.Format("%s----- %d----%d", (unsigned char*)(LPTSTR)(LPCTSTR)m_strPassword, m_strPassword.GetLength(),ret);
				MessageBox(_T(check), _T("Password Check"), MB_OK);
				if (ret == eMOD_ERROR_SUCCESS)
                {
                    //MY_CHECK_POINT("패스워드 파일이 성공적으로 저장되었습니다.");
					MessageBox(_T("Password 등록"), _T("Password Check"), MB_OK);
                    m_nSucces = 1;
                }

				else
				{
					//printf("%d", ret);
					
					//MY_CHECK_POINT("nonononononono");
				}
            }

			CDialogEx::OnOK();            

		}
		else {
			MessageBox(_T("입력하신 Password가 일치하지 않습니다"), _T("Password Check"), MB_ICONERROR);
		}
		//MessageBox(_T("Password 적합"), _T("Password1 Valid"), MB_OK);
		ps1 = 1;
	}
	else {
		MessageBox(_T("Password 길이는 7~10 이어야 합니다."), _T("Password1 Valid"), MB_ICONERROR);
	}

	
}


void UserPassword::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	PostQuitMessage(0);
	CDialogEx::OnCancel();
}

int UserPassword::CheckPW(CString str1, CString str2)
{
	//TODO: PW1, PW2 비교 
	str1 = m_strPassword;
	str2 = m_strPasswordcheck;

	if (str1.Compare(str2) == 0) {
		return 1;
	}
	else {
		return 0;
	}

}
CString UserPassword::GetPassword()
{
	return m_strPassword;
}

int UserPassword::GetPasswdSuccess()
{
    return m_nSucces;
}