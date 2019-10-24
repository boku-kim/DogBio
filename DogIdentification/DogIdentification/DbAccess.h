#pragma once

typedef struct _Dogdata {
	CString id;
	CString name;
	CString age;
	CString gender;
	CString species;
	CString address;
	CString dogimg;
}Dogdata;

class DbAccess
{
public:
	void DbTest();
	static void DbInsert();
	static void DbSearch();
	static void DbSetting(CString, CString, CString, CString, CString, CString);
	static void DbOutting();
	static void DbDelete();
	static void DbUpdate();

protected:
	static Dogdata _dog_info;
	static int _dog_id;

};



