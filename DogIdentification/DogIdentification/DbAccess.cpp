#include "stdafx.h"
#include "DbAccess.h"
#include "SearchDogInfo.h"


Dogdata DbAccess::_dog_info;


void DbAccess::DbSetting(CString _name, CString _age, CString _gender, CString _species, CString _address, CString _dogimg)
{
	_dog_info.name = _name;
	_dog_info.age = _age;
	_dog_info.gender = _gender;
	_dog_info.species = _species;
	_dog_info.address = _address;
	_dog_info.dogimg = _dogimg;
}


void DbAccess::DbDelete()
{
	BOOL bSuccess = FALSE;
	CString strDsnText;
	CDatabase db;

	strDsnText.Format("DSN=dbdog;UID=test");
	bSuccess = db.OpenEx(strDsnText);

	if (bSuccess)
	{
		if (db.IsOpen())
		{
			BOOL bRet = FALSE;
			int count = 0;
			int field_count = 0;
			int nNo = 0;
			CString strValue;
			char szMessage[128] = { 0 };
			CRecordset rs(&db);

			try
			{
				CString str;
				str.Format("DELETE FROM so_test_191008_1 WHERE ID= %s", _dog_info.id);
				OutputDebugString(str);
				db.ExecuteSQL(str);


			}
			catch (CDBException * e)
			{
				e->ReportError();
			}

			db.Close();
		}
	}

}

void DbAccess::DbUpdate()
{
	BOOL bSuccess = FALSE;
	CString strDsnText;
	CDatabase db;

	strDsnText.Format("DSN=dbdog;UID=test");
	bSuccess = db.OpenEx(strDsnText);

	if (bSuccess)
	{
		if (db.IsOpen())
		{
			BOOL bRet = FALSE;
			int count = 0;
			int field_count = 0;
			int nNo = 0;
			CString strValue;
			char szMessage[128] = { 0 };
			CRecordset rs(&db);

			try
			{
				CString str;
				str.Format("UPDATE so_test_191008_1 SET _NAME ='%s', GENDER='%s', AGE='%s' ,SPECIES='%s', ADDRESS='%s',IMAGE='%s' WHERE ID = %s", _dog_info.name, _dog_info.gender, _dog_info.age, _dog_info.species, _dog_info.address,_dog_info.dogimg, _dog_info.id);

				OutputDebugString(str);
				db.ExecuteSQL(str);


			}
			catch (CDBException * e)
			{
				e->ReportError();
			}

			db.Close();
		}
	}

}

void DbAccess::DbOutting()
{
	char szMessage[128] = { 0 };

	SearchDogInfo::m_dogName = _dog_info.name;
	SearchDogInfo::m_dogAge = _dog_info.age;
	SearchDogInfo::m_gender = _dog_info.gender;
	SearchDogInfo::m_dogSpecies = _dog_info.species;
	SearchDogInfo::m_dogAddr = _dog_info.address;
	SearchDogInfo::m_dogimg = _dog_info.dogimg;

	sprintf_s(szMessage, 128, "YOYOYOYOYOYOYYO %s  %s.\n", SearchDogInfo::m_dogName, _dog_info.id);
	OutputDebugString(szMessage);
}


void DbAccess::DbTest()
{

}



/*
** 일단은 제일 마지막 애 검색
*/

void DbAccess::DbSearch()
{
	BOOL bSuccess = FALSE;
	CString strDsnText;
	CDatabase db;

	strDsnText.Format("DSN=dbdog;UID=test");
	bSuccess = db.OpenEx(strDsnText);

	if (bSuccess)
	{
		if (db.IsOpen())
		{
			BOOL bRet = FALSE;
			int count = 0;
			int field_count = 0;
			int nNo = 0;
			CString strValue;
			char szMessage[128] = { 0 };
			CRecordset rs(&db);

			try
			{
				bRet = rs.Open(CRecordset::dynaset, "SELECT * FROM so_test_191008_1");
				if (bRet == TRUE)
				{
					int num = 0;
					OutputDebugString("db opening was succeeded.\n");
					count = (int)rs.GetRecordCount();
					sprintf_s(szMessage, 128, "total record count : %d.\n", count);
					OutputDebugString(szMessage);
					field_count = rs.GetODBCFieldCount();
					sprintf_s(szMessage, 128, "total record field count : %d.\n", field_count);
					OutputDebugString(szMessage);



					while (!rs.IsEOF())
					{
						num++;
						sprintf_s(szMessage, 128, "----------- record no : %d ---------------\n", ++nNo);
						OutputDebugString(szMessage);

						//	rs.GetFieldValue("ID", _dog_id);
						rs.GetFieldValue("ID", _dog_info.id);
						rs.GetFieldValue("_NAME", _dog_info.name);
						sprintf_s(szMessage, 128, "ID : %s \n", _dog_info.name);
						OutputDebugString(szMessage);
						rs.GetFieldValue("GENDER", _dog_info.gender);
						sprintf_s(szMessage, 128, "GENDER : %s \n", _dog_info.gender);
						OutputDebugString(szMessage);
						rs.GetFieldValue("AGE", _dog_info.age);
						rs.GetFieldValue("SPECIES", _dog_info.species);
						rs.GetFieldValue("ADDRESS", _dog_info.address);
						rs.GetFieldValue("IMAGE", _dog_info.dogimg);

						rs.MoveNext();
					}

					/*try
					{

						CString str;
						str.Format("SELECT * FROM so_test_191008_1 WHERE ID= %d",num-1);

						db.ExecuteSQL(str);

					}
					catch (CDBException * e)
					{
						int errCode = e->ReportError();       //에러나면자동으로메시지박스뜬다
					}*/

					//반듯이다시쿼리를실행시켜줘야한다.
					//이때쿼리는rs의Select문이다
					//rs.Requery();


					rs.Close();
				}
				else
				{
					OutputDebugString("db opening was failed.\n");
				}
			}
			catch (CDBException * e)
			{
				e->ReportError();
			}

			db.Close();
		}
	}
}

void DbAccess::DbInsert()
{
	BOOL bSuccess = FALSE;
	CString strDsnText;
	CDatabase db;

	strDsnText.Format("DSN=dbdog;UID=test");
	bSuccess = db.OpenEx(strDsnText);

	if (bSuccess)
	{
		if (db.IsOpen())
		{
			BOOL bRet = FALSE;
			int count = 0;
			int field_count = 0;
			int nNo = 0;
			CString strValue;
			char szMessage[128] = { 0 };

			///<쿼리를 날리면 결과는 레코드가 되므로 CRecordset 클레스를 CDatabase와 연동하면 편하다.
			CRecordset rs(&db);

			try
			{
				bRet = rs.Open(CRecordset::dynaset, "SELECT * FROM so_test_191008_1");
				if (bRet == TRUE)
				{
					int num = 0;
					OutputDebugString("db opening was succeeded.\n");
					count = (int)rs.GetRecordCount();
					sprintf_s(szMessage, 128, "total record count : %d.\n", count);
					OutputDebugString(szMessage);
					field_count = rs.GetODBCFieldCount();
					sprintf_s(szMessage, 128, "total record field count : %d.\n", field_count);
					OutputDebugString(szMessage);
					while (!rs.IsEOF())
					{
						num++;
						/*sprintf_s(szMessage, 128, "----------- record no : %d ---------------\n", ++nNo);
						OutputDebugString(szMessage);
						rs.GetFieldValue("ID", strValue);
						sprintf_s(szMessage, 128, "ID : %s \n", strValue);
						OutputDebugString(szMessage);
						rs.GetFieldValue("field1", strValue);
						sprintf_s(szMessage, 128, "field1 : %s\n", strValue);
						OutputDebugString(szMessage);
						rs.GetFieldValue("field2", strValue);
						sprintf_s(szMessage, 128, "field2 : %s\n", strValue);
						OutputDebugString(szMessage);*/
						rs.MoveNext();
					}

					try
					{

						CString str;
						str.Format("INSERT INTO so_test_191008_1 (_NAME, GENDER, AGE ,SPECIES, ADDRESS, IMAGE) VALUES('%s','%s','%s','%s','%s','%s')", _dog_info.name, _dog_info.gender, _dog_info.age, _dog_info.species, _dog_info.address, _dog_info.dogimg);
						//db.ExecuteSQL("INSERT INTO so_test_191008_1 (_NAME, GENDER ,AGE ,SPECIES, ADDRESS) VALUES('%s','%s','%s','%s')", _edit_name, _edit_age, _edit_species, _edit_address);
						db.ExecuteSQL(str);
						sprintf_s(szMessage, 128, "삽입!!\n", field_count);
						OutputDebugString(szMessage);
						//cout << "\n\n하나의필드를추가하였습니다." << endl;
					}
					catch (CDBException * e)
					{
						int errCode = e->ReportError();       //에러나면자동으로메시지박스뜬다
					}

					//반듯이다시쿼리를실행시켜줘야한다.
					//이때쿼리는rs의Select문이다
					rs.Requery();


					rs.Close();
				}
				else
				{
					OutputDebugString("db opening was failed.\n");
				}
			}
			catch (CDBException * e)
			{
				e->ReportError();
			}

			db.Close();
		}
	}
}

