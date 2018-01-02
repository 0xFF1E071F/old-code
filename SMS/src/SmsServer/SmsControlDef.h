#pragma  once 

//controller to smsserver


//smsserver to controller
#define  WM_SMS_ON_EVENT_BASE WM_USER+600
#define  WM_SMS_ON_START		     WM_SMS_ON_EVENT_BASE+1
#define  WM_SMS_ON_STOP			     WM_SMS_ON_EVENT_BASE+2
#define  WM_SMS_ON_ERROR		     WM_SMS_ON_EVENT_BASE+3
#define  WM_SMS_ON_CONNECT			 WM_SMS_ON_EVENT_BASE+4
#define  WM_SMS_ON_DISCONNECT		 WM_SMS_ON_EVENT_BASE+5
#define  WM_SMS_ON_RECEIVE_MESSAGE	 WM_SMS_ON_EVENT_BASE+6
#define  WM_SMS_ON_SEND_MESSAGE		 WM_SMS_ON_EVENT_BASE+7
#define  WM_SMS_ON_STATUS_INDICATION WM_SMS_ON_EVENT_BASE+8
#define  WM_SMS_ON_DEVICE_SIGNAL     WM_SMS_ON_EVENT_BASE+9
#define  WM_SMS_ON_CONNECT_COUNT	 WM_SMS_ON_EVENT_BASE+10		

//message from where
#define  SMS_TYPE_DEVICE 1
#define  SMS_TYPE_COMMUNICATE 0
#define  SMS_TYPE_SERVICE 3

//
#define  SMS_OK 0
#define  SMS_FAIL 1
