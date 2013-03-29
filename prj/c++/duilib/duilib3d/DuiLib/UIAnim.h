#if !defined(AFX_UIANIM_H__20050522_5560_2E48_0B2D_0080AD509054__INCLUDED_)
#define AFX_UIANIM_H__20050522_5560_2E48_0B2D_0080AD509054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3d9.h>
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////
//
namespace DuiLib{

/////////////////////////////////////////////////////////////////////////////////////
//
	typedef enum __ANIM_TEXTURE_TYPE	//纹理来源方式
	{
		TEXTURE_CAPTURE,				//纹理来自截图
		TEXTURE_FILE					//纹理来自文件
	}ANIM_TEXTURE_TYPE;

// 	typedef enum __INTERPOLATE_TYPE 	//插值计算方式
// 	{
// 		INTERPOLATE_LINEAR, 			//线性插值
// 		INTERPOLATE_COS,				//cos插值
// 		INTERPOLATE_SIN					//sin插值
// 	}INTERPOLATE_TYPE;

	typedef enum __ANIM_SPEED_TYPE  	// 减速方式
	{
		SPEED_LINEAR,					// 直线,匀速
		SPEED_PARABOLA,					// 抛物线, 放缓减速(放缓效果不太明显)
		SPEED_HYPERBOLA					// 双曲线, 放缓减速(放缓效果比较明显)
	}ANIM_SPEED_TYPE;

class UILIB_API CAnimJobUI
{
	friend class CAnimationSpooler;
private:
	static const RECT s_cEmptyRect;

	DWORD m_dwStartTick;     			//动画开始时间
	DWORD m_dwDuration;		 			//持续时间
	
	POINT m_ptStartCenter;				//动画起始中心点
	POINT m_ptXYtrans;					//动画从起始点偏移量
	float m_fStartZtrans;				//z轴偏移量,相当于放大缩小倍数,会被求绝对值,起始点
	float m_fEndZtrans;					//z轴偏移量,相当于放大缩小倍数,会被求绝对值,终止点

	COLORREF m_clrBack;					//做动画时源区域的背景颜色
	int m_iAlpha;						//透明度
	
	float m_fRotateVecX;				//旋转轴X坐标
	float m_fRotateVecY;				//旋转轴Y坐标
	float m_fRotateVecZ;				//旋转轴Z坐标
	float m_fRotateAngle;   			//旋转角度,以度为单位

	ANIM_TEXTURE_TYPE m_eTexFormType;	//纹理来源方式
	RECT m_rcCaptureSrc;				//截图时纹理来源矩形,TEXTURE_CAPTURE时有效
	CStdString m_strImgPath;			//文件提供纹理是文件路径,TEXTURE_FILE时有效
	LPDIRECT3DTEXTURE9 m_pTexture;		//纹理
	HDC m_hMaskDC;
	HBITMAP m_hMaskBmp;
	HDC m_hDestDC;
	HBITMAP m_hDestBmp;
	bool m_bUseMaskBk;

	ANIM_SPEED_TYPE m_eSpeedType;		//减速方式
	bool m_bDrawBack;					//在背面绘制
	bool m_bCullFace;					//是否剔除背面,对应这个属性D3DRS_CULLMODE

	bool m_bBeforePaint;	 			//是否在重绘之前截取纹理
	bool m_bClip;			 			//是否对超出范围的动画进行裁剪
	RECT m_rcClip;			 			//裁剪区域,动画只能在这个区域播放,只有m_bClip为true时有效

	bool m_bIsCompleted;	 			//该动画是执行完了还是取消了
	UINT m_uCallBackMsg;	 			//动画执行完后发送此消息,只有大于WM_USER的消息才会被发送,lParam返回初始化是传入的值,wParam 表示是否执行完成
	LPVOID m_pMsgCallBackData;			//初始化时传入的指针
public:
	CAnimJobUI(const CAnimJobUI& src);
	CAnimJobUI(
		DWORD dwStartTick,
		DWORD dwDuration,
		POINT ptStartCenter,
		POINT ptXYtrans,
		float fStartZtrans = 1.0f,
		float fEndZtrans = 1.0f,
		bool bBeforePaint = false,
		bool bClip = false,
		RECT rcClip = s_cEmptyRect,
		RECT rcCaptureSrc = s_cEmptyRect,
 		UINT uCallBackMsg = WM_USER,
		LPVOID pMsgCallBackData = NULL);
	~CAnimJobUI();
public:
	void SetRotateInfo(float fRotateAngle,float fRotateVecX,float fRotateVecY,float fRotateVecZ);
	void SetBkColor(COLORREF clrBack);
	void SetAlpha(int iAlpha);
	void SetSpeedType(ANIM_SPEED_TYPE eSpeedType);
	void SetTextureFromFile(CStdString strImgPath);
	void SetDrawMode(bool bDrawBack,bool bCullFace);
	void SetUseMaskBk();
private:
	bool InitBkInfo(HWND hWnd);
	bool InitDestInfo(HDC hDC);
	bool AdjustTargetTexture(HWND hWnd);
};

class UILIB_API CAnimationSpooler
{
protected:
	struct CUSTOMVERTEX								   //自定义顶点缓冲区格式
	{
		FLOAT x, y, z;
		FLOAT rhw;
		DWORD color;
		FLOAT tu, tv;
	};
	typedef CUSTOMVERTEX CUSTOMVERTEXRECT[4];

	HWND m_hWnd;										//目标窗体
	bool m_bIsAnimating;								//是否正在画动画
	bool m_bIsInitialized;								//是否初始化
	CStdPtrArray m_aJobs;								//保存动画队列
	D3DFORMAT m_clrFormat;
	LPDIRECT3D9 m_pD3D;									//D3D
	LPDIRECT3DDEVICE9 m_p3DDevice;						//D3D设备
	LPDIRECT3DSURFACE9 m_p3DSurface;					//D3D缓冲区表面
public:
	CAnimationSpooler();
	~CAnimationSpooler();

	bool Init(HWND hWnd);
	bool PrepareAnimation();
	bool Render();

	bool IsInitialized();
	bool IsAnimating() const;
	bool IsJobsEmpty() const;
	bool AddJob(CAnimJobUI* pJob);
	void RemoveJobs();
protected:

	COLORREF TranslateColor(LPDIRECT3DSURFACE9 pSurface, COLORREF clr) const;
	bool SetColorKey(LPDIRECT3DTEXTURE9 pTexture, LPDIRECT3DSURFACE9 pSurface, int iTexX,int iTexY,COLORREF clrColorKey);

	bool PrepareJob_Flat(CAnimJobUI* pJob);
	bool RenderJob_Flat(CAnimJobUI* pJob, LPDIRECT3DSURFACE9 pSurface, DWORD dwTick);

	bool MakeTexture(CAnimJobUI* pJob);
	bool CaptureSurface();

// 	double CAnimationSpooler::LinearInterpolate(double y1, double y2, double fPrecent);
// 	double CAnimationSpooler::CosineInterpolate(double y1, double y2, double fPrecent);
};
}
#endif // !defined(AFX_UIANIM_H__20050522_5560_2E48_0B2D_0080AD509054__INCLUDED_)

