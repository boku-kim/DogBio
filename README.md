# Dog Biometrics Project
## test update

-2019.10.24������ ������Ʈ ��Ȳ

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


## [MFC]
## ��ģ ��
- UserPassword(�Ϸ�)

	- ����� �н����尡 ���� �� �н����� ��� â���� �Ѿ�� �н����� ���
- Login(�Ϸ�)

	- ����� �н����尡 ���� �� �α��� â���� �Ѿ�� �н����� ���� �α���
- EditPassword(�Ϸ�)

	- �н����� üũ
	- �н����� ���� ��, ���� �н����� ����
~~~ 
�н����� ������ �����δ� my_define.h ���Ͽ� ��õǾ� �ִ�. 
~~~
- CamerMgr(�Ϸ�)

	- �������
- DogRegister(�κпϷ�)

	- local���� �̹��� �ҷ�����
	- webcam���� ���� ��� ����(CameraMgr���� ���� ���� �޾Ƽ� ���)
	- db�� ������ ���� ���(�̹��� ������ ��� ����)

- SearchDogInfo(�κпϷ�)
	- local���� �̹��� �ҷ�����
	- webcam���� ���� ��� ����(CameraMgr���� ���� ���� �޾Ƽ� ���)
	- ���������� Search( ����� �̹����˻��� �ȵǹǷ� db�� ���� �������� ��ϵǾ��ִ� ������ ���� �ҷ���)
	- ���������� ���� & db�� ����
	- ���������� ����
## �ؾ��� ��
- DogRegister

	- �̹��� �м� & �м����� �޾ƿ���
	- �м��� �̹����� ������ �����ϴ� ������ �������� Search
	- �̹��� �м����� ����
- SearchDogInfo

	- �̹��� �˻��ؼ� ������ ���� �޾ƿ���
- â�ݱ� ��ư ��Ȱ��ȭ

- memory leak �ذ�

[RaceCar]