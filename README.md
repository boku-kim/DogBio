# Dog Biometrics Project
## test update

- 2019.10.24������ ������Ʈ ��Ȳ

1. DogRegister::Checkarg() �߰�
	�̸�,��,�ּ�,���� ������ ����
	���� ���� ����� ����(���ڸ��� �ٸ��� �ĵ� ����)

2. CameraMgr.cpp �� �ڵ��߰�
	CameraMgr::OnBnClickedOk() �� CameraMgr::OnBnClickedCancel()�� �ڵ��߰�
	

	�߰��� �ڵ�:

		if (capture)
		{
			delete capture;
		}
	�߰��� ����:
		���ϸ� ī�޶� �Ȳ��� -> �Ȳ����� ������ �ٽ� ����Ϸ��� ������ MFC�� ����

3-1. stdafx.h�� #define���� SAVE_PATH�� IMAGE_PATH �߰�
	������� ��ǻ�� C:\DogIdentificationdn�� ������ ����� �̹����� C:\DogIdentification\Image �� �����Ϸ���
	IMAGE_PATH�� ����� "Image"�� �ߴµ� �׳� " C:\DogIdentification\Image"�� �ٲܱ� �����(3-3����)


3-2. DogIdentificationDlg.cpp�� SAVE_PATH�� ������ ������ ����� �ڵ� ����
	������ �����ϸ�, ���� ������ ���� �Ŀ� ������ ������ ���� �� ����

3-3. DogResister.cpp�� Image ���� ������ ����� CImage�̿��ؼ� ���� ���� �����ϴ� �ڵ� ����(4�� ����)
	�̶� ��� ���� �Ҷ�  SAVE_PATH�� IMAGE_PATH �� ���ļ� �����ϴµ� �׳� IMAGE_PATH �ϳ��� �� �� �ְ� IMAGE_PATH ��ü�� �ٲܱ� �����
	���� �����Ҷ� ���� �̸��� ������ �̸�,
		�̶� ���� �̸��� �������� �̹� �ִٸ� �̸�_����, �� ����
		���ڴ� �����̸��� �������� �Ѹ��� ���������� �þ.

4. CameraMgr.h�� CStatic* m_picDog �߰��ϰ� DogResister.cpp ���� open camera��ư ������ m_bmpBitmap�̶� ������Ŵ (�̹����� �����ϱ� ����)
	DogRegister::OnBnClickedBtnOpencamera() ���� pCameraInst->save_img = &m_bmpBitmap; �� ����
	CameraMgr::OnBnClickedOk() �κп��� save_img�� ���� ����

5. CameraMgr::OnBnClickedOk()���� StretchDIBits�Լ� ����
	������ Ʋ������ ���� �Ǳ淡 ���� �ʺ� ������, ���� �ڵ尡 ����� ���� ������
	StretchDIBits(cimage_mfc.GetDC(), 0, 0, rect.Width(), rect.Height(), 0, 0, imgWidth, imgHeight, mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY); ����
	imgWidth, imgHeight �κи� �ٲ�






- 2019.10.24 ���ı����� ������Ʈ ��Ȳ

1. CateraMgr.cpp �� void CameraMgr::OnBnClickedOk()���� �߸��� DC�� Release �ǰ��ֱ⿡ ������.
	::ReleaseDC(m_camerapic->m_hWnd, dc);  ���� ::ReleaseDC(m_picDog->m_hWnd, dc); �� ����

2. DbAccess�� Image_path�� ���õ� �κе� �߰�

3. SearchDogInfo�� search/edit/delete ��� �߰�
	
	search -> ������ �ϴ��� ���� �������� ����� ������ ã�´�.
		   -> �̹��� ������ �̹����� ���ٴ� �˸�â�� ���.

	edit   -> ������ ������ �ϴ� ����Ǿ��ִ� ���� ������ �����, ���� ������ �̸����� �ٽ� �����Ѵ�. (����� dogRegister�� ����)
		   -> DbAccess::DbSetting(m_dogName, m_dogAge, m_gender, m_dogSpecies, m_dogAddr, file_name); ���� �������� file_name�� ������ ���ο� ��� �����Ϸ���.
		   -> edit �Ҷ� �̻��� ���� �����°� ���� ���� DogRegister�� �ִ� Checkarg()�Լ� ������ -> �ٵ� '���� ���� �ִ°��� ���ƾ� �Ѵ�,,,, db�� ���� �� ������...

	delete -> ���� ������ ������ ������ �����, db_table���� ������ �� �����.
		   -> ���� �Ŀ��� ȭ�� �ʱ�ȭ (Invalidate(true); ���)


