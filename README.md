# Dog Biometrics Project

# [MFC]

## 마친 일
- UserPassword(완료)
	- 저장된 패스워드가 없을 때 패스워드 등록 창으로 넘어가서 패스워드 등록
- Login(완료) 
	- 저장된 패스워드가 있을 때 로그인 창으로 넘어가서 패스워드 비교후 로그인

- EditPassword(완료)
	- 패스워드 체크
	- 패스워드 변경 후, 변경 패스워드 저장

~~~
패스워드 파일의 저장경로는 my_define.h 파일에 명시되어 있다. 
~~~
- CamerMgr(완료)
	- 사진찍기

- DogRegister(부분완료)
	- local에서 이미지 불러오기
	- webcam에서 사진 찍어 띄우기(CameraMgr에서 찍은 사진 받아서 띄움)
	- db에 강아지 정보 등록(이미지 제외한 모든 정보)

- SearchDogInfo(부분완료)
	- local에서 이미지 불러오기
	- webcam에서 사진 찍어 띄우기(CameraMgr에서 찍은 사진 받아서 띄움)
	- 강아지정보 Search( 현재는 이미지검색이 안되므로 db의 제일 마지막에 등록되어있는 강아지 정보 불러옴)
	- 강아지정보 변경 & db에 저장
	- 강아지정보 삭제

## 해야할 일
- DogRegister
	- 이미지 분석 & 분석정보 받아오기
	- 분석한 이미지가 기존에 존재하는 강아지 정보인지 Search
	- 이미지 분석정보 저장

- SearchDogInfo
	- 이미지 검색해서 강아지 정보 받아오기

- 창닫기 버튼 비활성화

- memory leak 해결
