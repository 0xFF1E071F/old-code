#if !defined(WAVE_OBJECT_INCLUDED_)
#define WAVE_OBJECT_INCLUDED_
typedef struct _WAVE_OBJECT
{
	HWND hWnd;
	DWORD dwFlag;
	DWORD hDcRender;
	DWORD hBmpRender;
	DWORD lpDIBitsSource;
	DWORD   lpDIBitsRender;// ������ʾ����Ļ����������
	DWORD lpWave1;       // ˮ���������ݻ���1
	DWORD lpWave2;       // ˮ���������ݻ���2
	//********************************************************************
	DWORD dwBmpWidth;
	DWORD dwBmpHeight;
	DWORD dwDIByteWidth;// = (dwBmpWidth * 3 + 3) and ~3
	DWORD dwWaveByteWidth;// = dwBmpWidth * 4
	DWORD dwRandom;
	//********************************************************************
	// ��Ч����
	//********************************************************************
	DWORD dwEffectType;
	DWORD dwEffectParam1;
	DWORD dwEffectParam2;
	DWORD dwEffectParam3;
	//********************************************************************
	// �����д���Ч
	//********************************************************************
	DWORD dwEff2X;
	DWORD dwEff2Y;
	DWORD dwEff2XAdd;
	DWORD dwEff2YAdd;
	DWORD dwEff2Flip;
	//********************************************************************
	BITMAPINFO stBmpInfo;// BITMAPINFO <>
} WAVE_OBJECT,*LPWAVE_OBJECT;


//===================================================================================================================

extern "C" void __stdcall _WaveInit(LPWAVE_OBJECT lpWaveObject,HWND hWnd,HBITMAP hBmp,DWORD dwSpeed,DWORD dwType);	//��ʼ������; ������_lpWaveObject �� ָ�� WAVE_OBJECT��ָ��; ����ֵ eax = 0 �ɹ���= 1 ʧ��
extern "C" void __stdcall _WaveDropStone(LPWAVE_OBJECT lpWaveObject,DWORD dwPosX,DWORD dwPosY,DWORD dwStoneSize,DWORD dwStoneWeight);	//; ��һ��ʯͷ
extern "C" void __stdcall _WaveUpdateFrame(LPWAVE_OBJECT lpWaveObject,HDC hDc,DWORD bIfForce);
extern "C" void __stdcall _WaveFree(LPWAVE_OBJECT lpWaveObject);
extern "C" void __stdcall _WaveEffect(LPWAVE_OBJECT lpWaveObject,DWORD dwType,DWORD dwParam1,DWORD dwParam2,DWORD dwParam3);	// ������Ч

/***********************************************�÷�����*******************************************************************
;********************************************************************
; ����������ˮ��
;********************************************************************
		.elseif	eax ==	WM_LBUTTONDOWN
			mov	eax,lParam
			movzx	ecx,ax		; x
			shr	eax,16		; y

			invoke	_WaveDropStone,addr stWaveObj,ecx,eax,2,256
;********************************************************************
; ����Ҽ��л���Ч
;********************************************************************
		.elseif	eax ==	WM_RBUTTONDOWN
			mov	eax,stWaveObj.dwEffectType
			.if	eax ==	0
				invoke	_WaveEffect,addr stWaveObj,1,5,4,250
			.elseif	eax ==	1
				invoke	_WaveEffect,addr stWaveObj,3,250,4,8
			.elseif	eax ==	3
				invoke	_WaveEffect,addr stWaveObj,2,4,2,180
			.else
				invoke	_WaveEffect,addr stWaveObj,0,0,0,0
			.endif
;********************************************************************
*******************************************************************************************************************************/
#endif