#pragma  once 

#define CHAIN_MSG_MAP_MEMBER_PTR(pChainMember) \
	{ \
		if(pChainMember != NULL) \
		{ \
			if(pChainMember->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID)) \
				return TRUE; \
		} \
	}


//Button ID on Main Button Pane
#define TS_SINGLE_BTN_ID 100
#define TS_NET_BTN_ID 101
#define TS_REC_PLAY_BTN_ID 102
#define TS_SETUP_BTN_ID 103

#define TS_SETUP_DLG_CHANGED 110 //setup dialog do OK



#define WM_TS_DISPLAY_CONTROL WM_USER + 1000 //WPARAM(LPTsControlHeader), LPARAM(data pointer)
typedef struct tagTsControlHeader 
{
	int nControlCode;
	DWORD dwUserData;

	DWORD dwIDFrom;
	DWORD dwReserved;
}
TsControlHeader, *LPTsControlHeader;
#define TS_CONTROL_NONE 0
#define TS_CONTROL_INIT  1 //nControlCode
#define TS_CONTROL_START 2
#define TS_CONTROL_END	 3 
#define TS_CONTROL_SET_MOVING_ELE 4
#define TS_CONTROL_MOVE_LEFT 5
#define TS_CONTROL_MOVE_RIGHT 6
#define TS_CONTROL_MOVE_DOWN 7
#define TS_CONTROL_SWITCH_LEFT 8
#define TS_CONTROL_SWITCH_RIGHT 9
#define TS_CONTROL_DROP_DOWN 10
#define TS_CONTROL_PRODUCE_GARBAGE 11
#define TS_CONTROL_SET_RESULT 12
enum TS_DISPLAY_RESULT
{
	NONE_RESULT = 0,
	WIN_RESULT = 1,
	LOST_RESULT = 2
};

#define TS_CONTROL_REQUEST_ELEMENT 13
#define TS_CONTROL_REMOVED_LINE 14

#define TS_CONTROL_QUERY_STATUS 15
#define TS_CONTROL_ELE_GROUND_HIT 16

#define TS_CONTROL_GET_GROUND_DATA 17

#define WM_TS_DISPLAY_CONTROL_EVENT WM_USER + 1001

#define WM_TS_SET_DISPLAY_SINK WM_USER + 1002


#define WM_TS_SET_NEXT_ELEMENT WM_USER + 120 //WPARAM(nIndex), LPARAM(not used)



#define WM_TS_UI_DATA_CHANGED WM_USER + 2000 //WPARAM(UI change type) LPARAM(user Data)

#define TS_WPARAM_GAME_MODE_CHANGED  1
enum TS_GAME_MODE
{
	NONE_MODE = 0,

	SINGLE_PLAYER_MODE,
	TWO_PLAYER_MODE,
	COMPUTER_PLAYER_MODE,
	NET_SELECT_MODE,
	NET_PLAY_MODE,
	RECORD_SELECT_MODE,
	RECORD_PLAY_MODE
};

#define TS_WPARAM_DISPLAY_MODE_CHANGED 2
enum TS_DISPLAY_MODE
{
	DISPLAY_NONE = 0,
	DISPLAY_SINGLE,
	DISPLAY_MULTI
};

#define TS_WPARAM_HARD_LEVEL_CHANGED 3
enum TS_HARD_LEVEL
{
	LEVEL_EASY = 0,
	LEVEL_MIDDLE,
	LEVEL_HARD,
	LEVEL_CRAZY
};

#define TS_WPARAM_START_LINE_CHANGED 4
enum TS_START_LINE
{
	LINE_0 = 0,
	LINE_1,
	LINE_2,
	LINE_3,
	LINE_4,
	LINE_5,
	LINE_6,
	LINE_7,
	LINE_8,
	LINE_9,
	LINE_10,

	LINE_COUNT
};

#define TS_WPARAM_RECORD_OPTION_CHANGED 5

#define TS_WPARAM_GAME_STATUS_CHANGED 6
enum TS_GAME_STATUS
{
	IDLE_STATUS = 0,
	RUNNING_STATUS,
	PAUSE_STATUS
};

#define TS_WPARAM_PROCESS_CHANGED 10

#define TS_WPARAM_USER_CHANGED 11


#define WM_TS_SETTING_CHECK WM_USER + 3000
#define WM_TS_EDIT_LOST_FOCUS WM_USER + 3001
#define WM_TS_UPDATE_DISPLAY WM_USER + 3002
