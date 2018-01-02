#pragma  once

#define PORT_WRITE_BUFFER_SIZE	1024

class CSmsComPortAgentWnd;
class CGPRSManager;

class CSerialPortEx
{
//function
public:
	CSerialPortEx(CSmsComPortAgentWnd* p, CGPRSManager* pManager);
	~CSerialPortEx(void);
	//�򿪴���
	bool initPort(UINT portnr=1,UINT baud=9600,char parity='N',UINT databits=8,UINT stopbits=1);
	//�رմ���
	void releasePort();

	//����д������
	void WriteToPort(BYTE* p,int nLen);
	//���������߳�
	bool StartMonitoring();
	//ֹͣ�����߳�
	void StopMonitoring();
	bool isPortOpen(){return m_bOpen;}
	bool isMonitorRunning(){return m_bRunning;}

public:	
	CSmsComPortAgentWnd* m_pAgentWnd;

protected:
	//���ڼ����߳�
	static UINT __stdcall CommThread(LPVOID pParam);
	//��ȡ����
	static void ReceiveChar(CSerialPortEx* port,COMSTAT& comstat);
	//д������
	static void	WriteChar(CSerialPortEx* port);
private:
	
	CSerialPortEx();		//��ֹȱʡ����
	unsigned long			m_hThread;
	CRITICAL_SECTION		m_csCommunicationSync;
	HANDLE			m_hShutdownEvent;
	HANDLE			m_hComm;
	HANDLE			m_hWriteEvent;
	HANDLE			m_hEventArray[3];
	OVERLAPPED			m_ov;
	COMMTIMEOUTS		m_CommTimeouts;

	BYTE				m_szWriteBuffer[PORT_WRITE_BUFFER_SIZE];//д����
	bool				m_bRunning;		    //�߳��Ƿ���������		
	bool				m_bOpen;		    //�����Ƿ��Ѿ���ʼ��
	DCB				m_dcb;
	int				m_nToSend;			//��Ҫ���͵���Ϣ�ĳ���
};
