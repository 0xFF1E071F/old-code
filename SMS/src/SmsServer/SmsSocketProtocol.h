#pragma  once 

#include "SmsMessage.h"

#define SMS_SOCKET_SEND_TAG		"SMS_SEND_TAG"
#define SMS_SOCKET_DISPATCH_TAG  "SMS_DISPATCH_TAG"
#define SMS_SOCKET_CLOSE_TAG	"SMS_CLOSE_TAG"
#define SMS_SOCKET_LOGON_TAG	"SMS_LOGON_TAG"
#define SMS_SOCKET_SENDBACK_TAG "SMS_SENDBACK_TAG"
#define SMS_SOCKET_STOP_TAG		"SMS_STOP_TAG"
#define SMS_SOCKET_HEART_BEAT_TAG "SMS_HEART_BEAT_TAG"

typedef enum			
{
	SMSSEND = 0,		//���Ͷ���
	SMSRECEIVE = 1,		//�յ�����
	SMSCLOSE = 2,		//�ͻ�������Ͽ�
	SMSLOGON = 3,		//�ͻ��˷����ʺŵ�½	
	SMSSENDBACK = 4,	//֪ͨ�ͻ��˶����Ƿ��ͳɹ�
	SMSSTOP = 5	,		//����ֹͣ
	SMSHEARTBEAT = 6
} ENUM_SMS_SOCKET_PACKAGE_TYPE;

struct SMS_SOCKET_HEADER
{
	char sHeadTag[20];		//�����ͱ��
	int	 nP1Len;			//��������ʺų���
	int	 nP2Len;			//�������ݻ������볤��
	int	 nP3Len;			//��ϢID����
	int	 bOK;			    //�Ƿ�ɹ����(0 ʧ��, 1 �ɹ� ,3��½ʱ�ʺ�����ʹ��)
} ;
