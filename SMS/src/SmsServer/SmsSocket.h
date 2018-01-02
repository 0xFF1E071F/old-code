#pragma  once 

class CSmsSocketCommunicate;
class CSMSMessage;
#include "SmsSocketProtocol.h"

class CSmsSocket
{
public:
	CSmsSocket(SOCKET socket,BOOL bClient);
	virtual ~CSmsSocket();
	//������Ϣ
	BOOL SocketSend(CSMSMessage* pMsg);
	//�ͻ��˹ر�
	VOID OnClose();
	//�յ��ͻ�������Ϣ
	VOID OnRecv(SMS_SOCKET_HEADER* header);

	static VOID SetSocketManager(CSmsSocketCommunicate* pManager);

protected:
	INT SendWrap(char* p,int nLen);
	INT ReadWrap(char* p,int nLen);

private:
	GUID	m_gId;					//�ͻ���ID	
	SOCKET  m_socket;	
	BOOL    m_bClient;				//�Ƿ��ǿͻ���
	BOOL    m_bLogoned;			    //�Ƿ��Ѿ���֤ͨ��
	BOOL    m_bDispatch;			//�Ƿ�����ȡ���ŵ�Ȩ��
	static  CSmsSocketCommunicate* m_pSocketManager;
	
	BOOL	m_bConnected;
	friend class  CSmsSocketCommunicate;
};