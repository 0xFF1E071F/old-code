#pragma  once 

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlmisc.h>

//class CString;

//��⵱ǰ�Ļ����Ƿ��Ѿ�ע��
BOOL CheckIsRegistered();

//��ȡ��ǰ�����Ļ�����
BOOL GetMachineCode(CString& strMachineCode);

//���ݻ������ȡע����
BOOL GetRegisterCodeByMachineCode(const CString& strMachineCode, CString& strRegisterCode);

//ע���˺�
BOOL RegisterAccount(const CString& strMachineCode, CString& strRegisterCode);