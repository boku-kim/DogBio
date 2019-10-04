#pragma once
class Util
{
private:
    Util();
    ~Util();

public:
    static Util* GetInstance();
    static void ReleaseInstance();

    ///< method
    bool ExistFile(char* pszFilePath);

private:
    static Util* m_Inst;
};

