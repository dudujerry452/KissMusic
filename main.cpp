//-------------head------------
#include <windows.h>
#include <d2d1.h>
#include <cstdlib>
#include <ctime>
#include <D2d1helper.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <cmath>
#include <thread>
#include <mutex>
//#include "mpg123.h"
#include <string>
#include "D2DDraw.h"
#include "CDirectSound.h"
#include<cstdio>
//#include "stdafx.h"

//#pragma comment (lib,"./D2DDraw.lib")
#pragma warning(disable:4996)
//#pragma comment(lib,"./libmpg123.lib")
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"oleaut32.lib")
#pragma comment(lib,"ole32.lib")
//------------------------------


//------------version------------------
const LPCWSTR version = L"v0.3.7.1124_beta";
//-------------------------------------

//---------------Window--------------
int ScrWid, ScrHei;
int WinWid = 960, WinHei = 560;//1120,740
//-----------------------------------

//------------D2DDraw------------------
D2DDraw D2D;

D2DBitMap *start_1,*logo[1],*Option,*title;

D2DBitMap *blk,*line,*block,*lfbu,*ribu,*bgi,*spk_blk;

D2DBitMap *whiteStr,*line1,*line2,*line3;

//D2DBitMap *BG, *Bird, *Bird2, *XG;
const int TxNum=5;

TextFormat* TxFormat[5];
//RECT_F RC(L, T, R, B);

RECT_F RC(0.f, 20.f, 120.f, 20.f);
//------------------------------------

//---------------DSound----------------
CDirectSound bgm,cpy;
//-------------------------------------

//------------rect--------------------
RECT_F myinf_rect[1] = { {0,0,0,0} };
//------------------------------------

//--------------Game-------------------
int state;
int deviation,dev_dis;
POINT p;
float Salp[6];
//-------------------------------------

//--------------inf-------------------
LPCWSTR myinf[2] = { L"By dudujerry----Code",L"By 月霜---靠谱美工" };
//------------------------------------

//------------start--------------
float alp = 0.1;
int logon = 1, nowlogo = 0;
bool updown=false;
//---------------------------------

//-----------state:0--------------
int nowOption;
std::vector<std::string> bkspk;
//--------------------------------

//------------state:0.1-------------
int blkstate=-1;
float alk=0;
//--------------------------------

//-------------state:1-------------
struct BLOCK {
	int POS;
	int KEY;
};
struct nowBLOCK {
	POINT PI;
	int LONG;
	int KEY;
	float alf;
};
struct nowTxt {
	bool tpe;
	std::string txt;
	int LONG;
	int plr;
	int displyTime;
	int displyPicTime;
	bool disply_w;
};
struct nowTxt2 {
	std::string txt;
	int start;
	int plr;
	int displyTime;
	int displyPicTime;
	bool disply_w;
};
std::queue<BLOCK> msQue;
std::queue<nowTxt> msQue_txt;
void clean(std::queue<BLOCK>& q) {
	std::queue<BLOCK> empty_;
	q = empty_;
}

std::vector<nowTxt> spk;
std::vector<nowTxt2> spk2;
std::vector<nowBLOCK> nowblk;
int st,playTime;
bool keybor[6];
bool chkkeybor[6];
D2DBitMap* plrPic[20];
int plrN;
std::string nme[20];
int nowABC[3];
int score;
int erase_;
int speed = 5;
int keynum=3;
std::vector<std::string> saveList;
std::vector<std::string> nameList;
LPCWSTR lpw;

void LoadStand() {
	
	
	
	if (saveList.size() == 0) {

		int num = 0;

		
		std::string tmP;
		while (1) {
			tmP="res\\Save\\SurSave\\Save";
			
			tmP += (char)(num + 48);
			tmP += ".mcs";
			
			std::ifstream is;
			is.open(tmP.c_str(), std::ios::in);
			

			if (!is) {
				
				break;
			}

			getline(is, tmP);
			
			saveList.push_back(tmP);

			getline(is, tmP);
			nameList.push_back(tmP);

			num++;
		}
	}

	

}

inline std::string toSTR(int NUM) {
	std::string str;
	int tmp = NUM;
	while (tmp) {
		str =str+ char(tmp%10+48);
		tmp /= 10;
	}
	reverse(str.begin(), str.end());
	if (!NUM) str = '0';
	return str;
}

//--------------------------------------------------

//-----------------------state:2---------------------
int nowSave;
//---------------------------------------------------

//-----------------------state:3---------------------
int lastState;
//---------------------------------------------------

//---------------state:4-----------
int FullScore;
bool flg = 0;

nowTxt tmx;
//---------------------------------

void LoadSave(int num,HWND hwnd) {

	nowblk.clear();
	clean(msQue);
	score = 0;
	
	std::string name_;

	name_ = "res\\music\\SaveMusic\\";
	name_ += saveList[num] + ".mp3";

	//MessageBox(NULL, name_.c_str(), "debug", MB_OK);


	bgm=cpy;
	bgm.Create(name_.c_str(),true,hwnd);


	std::string tmP;
	if (keynum == 3) {
		tmP = "res\\Save\\SurSave\\Save";

		tmP += (char)(num + 48);
		tmP += ".mcs";
	}
	else {
		tmP = "res\\Save\\SurSave\\Save";

		tmP += (char)(num + 48);
		tmP += "_.mcs";

	}

	std::ifstream is;
	is.open(tmP.c_str(), std::ios::in);
	


	getline(is, tmP);
	getline(is, tmP);

	int N;
	is >> N;

	BLOCK bl;
	srand(time(0));
	for (int i = 0; i < N; i++) {

		is >> bl.POS;
		is >> bl.KEY;
		if (bl.KEY == 0) {
			
			bl.KEY = rand() % keynum+1 ;
		}
		msQue.push(bl);
	}

	is.close();


	tmP = "res\\Save\\ScSave\\Save";

	tmP += (char)(num + 48);
	tmP += ".mcs";

	std::ifstream is2;
	is2.open(tmP.c_str(), std::ios::in);
	is2 >> FullScore;
	is2.close();

	tmP = "res\\Picture\\BGI\\bgi";

	tmP += (char)(num + 48);
	tmP += ".png";

	LPCWSTR wstr;
	WCHAR wcr[60];
	MultiByteToWideChar(0, 0, tmP.c_str(), -1, wcr, 64);
	wstr = wcr;

	bgi = D2D.LoadBMP(wstr, WinWid, WinHei);
	if (bgi == NULL) {
		bgi = D2D.LoadBMP(L"res\\Picture\\BGI\\bgi.png", WinWid, WinHei);
	}


	tmP = "res\\Save\\SpkSave\\Save";
	tmP += (char)(num+48);
	tmP += ".mcs";
		
	std::ifstream is3;
	is3.open(tmP, std::ios::in);

	int txtnum;

	is3 >> plrN;
	for (int i = 0; i < plrN; i++) {
		is3 >> tmP;
		nme[i] = tmP;
		tmP = "res\\Picture\\plrPic\\" + tmP + ".png";//

		LPCWSTR wstr;
		WCHAR wcr[60];
		MultiByteToWideChar(0, 0, tmP.c_str(), -1, wcr, 64);
		wstr = wcr;
		plrPic[i]=D2D.LoadBMP(wstr, 70, 70);
	}
	is3 >> txtnum;
	for (int i = 0; i < 3; i++) {
		nowABC[i] = i;
	}
	char a;
	for (int i = 0; i < txtnum; i++) {
		//nowTxt tmx;
		is3>>tmx.LONG>> a>>tmP;
		if (a >= 65) {
			tmx.plr = a - 65;
		}
		else {
			tmx.tpe = true;
			tmx.plr = a-48;
		}
		
		tmx.txt = tmP;

		tmx.displyTime = tmP.length() * 50;

		tmx.displyPicTime = -70;

		msQue_txt.push(tmx);
	}
	is3.close();
	
}
//---------------------------------

void addTxt(std::string txt,char plr) {

	nowTxt2 tmx;
	tmx.txt = txt;
	tmx.displyTime = txt.length() * 50;
	tmx.displyPicTime = -70;
	tmx.plr = plr-65;
	tmx.disply_w = false;
	tmx.start = clock();

	spk2.push_back(tmx);
	/*if (spk.size() == 1)
		MessageBox(NULL, "OK", "OK", MB_OK);*/
}


void PaintStart();
void Paint(HWND hwnd);
void Loading(HWND hwnd);
void GameRelease();
void Play(HWND hwnd);

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	p.x = LOWORD(lParam);  p.y = HIWORD(lParam);
	
	switch (Message) {

		/* Upon destruction, tell the main thread to stop */
	case WM_DESTROY: {
		GameRelease();
		PostQuitMessage(0);
		break;
	}

	case WM_KEYUP: {
		switch (wParam) {
		case 65: {//A
			if (state == 1) {
				chkkeybor[0] = 0;
				keybor[0] = 0;
				//MessageBox(NULL, "OK", "ok", MB_OK);//debug
			}
				
			break;
		}

		case 83: {//S
			if (state == 1) {
				chkkeybor[1] = 0;
				keybor[1] = 0;
			}
				
			break;
		}

		case 68: {//D
			if (state == 1) {
				chkkeybor[2] = 0;
				keybor[2] = 0;
			}
				
			break;
		}

		case 74: {//J
			if (state == 1) {
				chkkeybor[3] = 0;
				keybor[3] = 0;
			}
				
			break;
		}

		case 75: {//K
			if (state == 1) {
				chkkeybor[4] = 0;
				keybor[4] = 0;
			}
				
			break;
		}

		case 76: {//L
			if (state == 1) {
				chkkeybor[5] = 0;
				keybor[5] = 0;
			}
				
			break;
		}

		}

		break;
	}

	case WM_PLAYOVER: {
		if (state == 0) {
			bgm.Play(0, -1, 1);
		}
		break;
	}


	case WM_KEYDOWN: {

		switch (wParam) {
		
		case 65:{//A
			if (state == 1) {
				chkkeybor[0] = 1;
				keybor[0] = 1;
				//MessageBox(NULL, "OK", "ok", MB_OK);//debug
			}
				
			break;
		}
		
		case 83: {//S
			if (state == 1) {
				chkkeybor[1] = 1;
				keybor[1] = 1;
			}
			break;
		}

		case 68: {//D
			if (state == 1) {
				chkkeybor[2] = 1;
				keybor[2] = 1;
			}
				
			break;
		}

		case 74: {//J
			if (state == 1) {
				chkkeybor[3] = 1;
				keybor[3] = 1;
			}
				
			break;
		}

		case 75: {//K
			if (state == 1) {
				chkkeybor[4] = 1;
				keybor[4] = 1;
			}
				
			break;
		}

		case 76: {//L
			if (state == 1) {
				chkkeybor[5] = 1;
				keybor[5] = 1;
			}
				
			break;
		}

		case 88: {//X
			if (state == 0) {
				int bAck=MessageBox(hwnd, "确认删除所有存档吗？", "删除存档", MB_YESNO| MB_ICONWARNING);
				/*std::string k;
				k = (char)(bAck + 48);
				MessageBox(NULL,k.c_str(), "Debug", MB_OK);*/
				if (bAck == 6) {
					std::ofstream os;
					for (int i = 0; i < saveList.size(); i++) {
						std::string tmP = "res\\Save\\ScSave\\Save";
						tmP += (char)(i + 48);
						tmP += ".mcs";
						
						os.open(tmP.c_str(), std::ios::out);
						os << 0;
						os.close();
					}
				}
				
			}
				
			break;
		}

		case 77: {  //M
			if (state == 2) {
				if (keynum == 6)
					keynum = 3;
				else
					keynum = 6;
			}
			break;
		}

		case VK_BACK: {
			if (state == 2) {
				state = 0;
				bgm.Stop();
				bgm = cpy;
				bgm.Create("res\\Music\\Bg_Music\\Background_music.mp3", 1, hwnd);
				bgm.Play(0, -1, 1);
			}
			break;
		}
		

		case VK_ESCAPE: {
			
			break;
		}

		case VK_UP: {
			if (state == 0 || state == 3) {
				if (nowOption == 0) break;
				nowOption--;

			}
			

			break;
		}

		case VK_DOWN: {
			if (state == 0 || state == 3) {
				if (nowOption == 2) break;
				nowOption++;
			}
			break;
		}

		case VK_LEFT: {
			if (state == 2) {
				if (nowSave == 0) {
					break;
				}
				nowSave--;
			}
			break;
		}

		case VK_RIGHT: {
			if (state == 2) {
				if (nowSave == saveList.size() - 1)
					break;
				nowSave++;

			}
			break;
		}

		case VK_RETURN: {
			if (state == 0) {
				if (nowOption == 0) {
					blkstate = 0;
				}
				else if (nowOption == 1) {
					lastState = state;
					state = 3;
				}
				else if (nowOption == 2) {
					GameRelease();
					PostQuitMessage(0);
				}
			}
			else if (state == 2) {
				state = 1;
				LoadSave(nowSave,hwnd);
				bgm.Play(0, -1, 0);
			}
			else if (state == 3) {
				if (nowOption == 1) {
					state = lastState;
				}
				else if (nowOption == 0) {
					state = 0;
					blkstate = -1;
					alk = 0;
					nowSave = 0;
					bgm.Stop();
					bgm = cpy;
					bgm.Create("res\\Music\\Bg_Music\\Background_music.mp3",1, hwnd);
					bgm.Play(0, -1, 1);
				}
				else {
					GameRelease();
					PostQuitMessage(0);
				}
			}
			else if (state == 4) {
				bgm.Stop();
				blkstate = 0;
				alk = 0;
			}


		}


		}

		break;

	}

	 

					 /* All other messages (a lot of them) are processed using default procedures */
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG Msg; /* A temporary location for all messages */

	memset(&Msg, 0, sizeof(Msg));

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon = (HICON)LoadImage(GetModuleHandle(0), "res\\logo.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); /* Load a standard icon */
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(0), "res\\picture\\logo.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); /* use the name "A" to use the project icon */

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ScrWid = GetSystemMetrics(SM_CXSCREEN);
	ScrHei = GetSystemMetrics(SM_CYSCREEN);

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "WindowClass","KissMusic", WS_VISIBLE | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME^WS_MAXIMIZEBOX,
		ScrWid/2-(WinWid/2), /* x */
		ScrHei/2-(WinHei/2), /* y */
		WinWid, /* width */
		WinHei, /* height */
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	
	
	


	D2D.LoadD2D(hwnd);

	//MessageBox(NULL, (LPCWSTR)(MAKEINTRESOURCE(IDR_WAVE1)), L"debug", MB_OK);

	
	
	//MessageBox(NULL, "Yes", "Debug", MB_OK);  //Here is RE
	
	
     


	int SClock = clock();

	
	/*
	This is the heart of our program where all input is processed and
	sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
	this loop will not produce unreasonably high CPU usage
	*/
	
	bool flg = 0;

	bgm.Create("res\\music\\Bg_Music\\Background_music.mp3",true,hwnd);

	bgm.Play(0,-1,1);

	

	while (1) {
		if(!flg)
			Loading(hwnd),flg=1;
		SClock = clock();
		if (!updown) {
			alp += 0.1f;
		}
		else {
			alp -= 0.1f;
		}
		PaintStart();
		if (alp >= 1) {
			alp = 1;
			PaintStart();
			Sleep(200);
			updown = true;
		}
		else if (alp <= 0) {
			alp = 0;
			PaintStart();
			updown = false;
			nowlogo++;
			D2D.BeginDr();
			D2D.ClearScr(D2D1::ColorF(D2D1::ColorF::Black));
			D2D.EndDr();
			Sleep(100);
			if (nowlogo >= logon) {
				break;
			}
		}
		while (clock() - SClock<30);
	}
	
	srand(time(0));
	int r = rand() % (bkspk.size());
		
	/*
	*这里注意，编写Background_Speak.mcs的时候
	*第一行数字填写的是以下有多少行
	*在void Loading加载的时候
	*读取的第一行一定要加1
	*这里随机的时候
	*只需要随机bkspk的size就可以了
	*addTxt的最后一个参数指定“角色”,不同的角色指定了不同的头像图片
	*/
		
	addTxt(bkspk[r], 'A');

	while (Msg.message != WM_QUIT) {
		if (PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else
		{
			SClock = clock();
			
			
			Play(hwnd);

			Paint(hwnd);

			while (clock() - SClock<15);
		}
	}
	return Msg.wParam;
}

void PaintStart() {
	D2D.BeginDr();
	D2D.DrawBMP(logo[nowlogo], WinWid/2-100,WinHei/2-100,alp);
	D2D.D2DDrawText(D2D1::ColorF::White, myinf_rect[nowlogo], (LPCWSTR)myinf[nowlogo], TxFormat[0]);
	D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(WinWid / 2 - 100, 420, WinWid / 2 +100, 450), myinf[nowlogo + 1], TxFormat[0]);
	D2D.EndDr();
}

void Play(HWND hwnd)
{
	if (state == 1) {

		int now_c = clock() - st;
		if (keynum == 6) {
			while (!msQue.empty()) {

				BLOCK fr = msQue.front();
				if (fr.POS <= now_c) {
					msQue.pop();
					nowBLOCK tmp;
					tmp.KEY = fr.KEY;
				
					tmp.PI.x = 50 + 160 * (fr.KEY - 1);
					
					tmp.PI.y = -50;
					tmp.alf = 1;


					nowblk.push_back(tmp);

				}
				else {
					break;
				}
			}
		}
		else {
			while (!msQue.empty()) {

				BLOCK fr = msQue.front();
				if (fr.POS <= now_c) {
					msQue.pop();
					nowBLOCK tmp;
					tmp.KEY = fr.KEY;

					if (tmp.KEY == 1)
						tmp.PI.x = 50 + 85;
					else if (tmp.KEY == 2)
						tmp.PI.x = 330 + 85;
					else
						tmp.PI.x = 610 + 85;

					tmp.PI.y = -50;
					tmp.alf = 1;


					nowblk.push_back(tmp);

				}
				else {
					break;
				}
			}
			
		}
		/*std::string tm;
		int num = nowblk.size();
		tm = char(num+48);
		MessageBox(NULL, tm.c_str(), "ok", MB_OK);*/
		for (int i = 0; i < keynum; i++) {
			if (keybor[i] == 1) {
				Salp[i] = 1;
			}
				
		}

		//bool add_[6] = { 0 };
		//bool chk[6] = { 0 };//这是个废物，见到删了就好啦/或者改一改，因为一帧来判断
		//实在是太短啦
		for (int i=0; i<nowblk.size();i++) {
			
			if (nowblk[i].PI.y >= WinHei ){
				nowblk.erase(nowblk.begin()+i);
				if (nowblk.size() == 0 ) {
					break;
				}
			}
			else if (chkkeybor[nowblk[i].KEY - 1]==1&&nowblk[i].PI.y < WinHei - 150&&nowblk[i].PI.y+50>=WinHei-150 && keybor[nowblk[i].KEY - 1] == 1&&nowblk[i].alf==1.f) {
				score += 25;
				nowblk[i].alf -= 0.1f;
				chkkeybor[nowblk[i].KEY - 1] = 0;
			}
			else if (chkkeybor[nowblk[i].KEY - 1] == 1 && nowblk[i].PI.y > WinHei - 150 && keybor[nowblk[i].KEY - 1] == 1 && nowblk[i].alf == 1.f) {
				score += 50;
				nowblk[i].alf -= 0.1f;
				chkkeybor[nowblk[i].KEY - 1] = 0;
			}
			else if(erase_==0){
				
				if (keybor[nowblk[i].KEY] == 1 &&  nowblk[i].alf == 1.f) {
					if (score >= 25) {
						score -= 25;
					}
					else if (score > 0 && score < 25) {
						score = 0;
					}
				}
			}
			erase_++;
			if (erase_ >= 10)
				erase_ = 0;
			nowblk[i].PI.y += speed;
			if (nowblk[i].alf < 1 && nowblk[i].alf != 0) {
				nowblk[i].alf -= 0.1f;
			}
			else if (nowblk[i].alf == 0 ) {
				nowblk.erase(nowblk.begin() + i);
				if (nowblk.size() == 0 ) {
					break;
				}
			}
		}
		if (nowblk.size() == 0 && msQue.empty()) {
			Sleep(1000);
			state = 4;
			
			flg = 0;
			if (score > FullScore) {
				FullScore = score;
				flg = 1;
			}
			std::string st;
			st = "res\\Save\\ScSave\\Save";
			st = st + char(nowSave + 48) + ".mcs";
			std::ofstream os;
			os.open(st.c_str(), std::ios::out);
			os << FullScore;
			os.close();
		}

		while (!msQue_txt.empty()) {
			nowTxt fr=msQue_txt.front();
			
			if (fr.LONG<= now_c) {
				spk.push_back(fr);
				/*LPCWSTR wstr;
				WCHAR wcr[60];
				MultiByteToWideChar(0, 0, fr.txt.c_str(), -1, wcr, 64);
				wstr = wcr;
				MessageBox(NULL, fr.txt.c_str(), "OK", MB_OK);
				D2D.BeginDr();
				D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(0, 0, 300, 300), wstr, TxFormat);
				D2D.EndDr();
				Sleep(5000);*/
				msQue_txt.pop();
				
			}
			else {
				break;
			}
		}

		
		

		for (int i = 0; i < keynum; i++) {
			if (Salp[i] > 0) {
				Salp[i] -= 0.05;
				//MessageBox(hwnd, "OK", "ok", MB_OK);
			}
				
		}

		for (int i = 0; i < spk.size(); i++) {



			if (spk[i].displyPicTime <= -70 && spk[i].disply_w) {
				//MessageBox(NULL, "ok", "ok", MB_OK);//
				spk.erase(spk.begin() + i);
			}

		}
	}

	for (int i = 0; i < spk2.size(); i++) {

		

		if (spk2[i].displyPicTime <= -70 && spk2[i].disply_w) {
			//MessageBox(NULL, "ok", "ok", MB_OK);//
			spk2.erase(spk2.begin() + i);
		}

	}

}

void Paint(HWND hwnd)
{
	

	D2D.BeginDr();
	//-----------state:0----------
	if (state == 0) {
		D2D.DrawBMP(title, 320, -100, 1.f);

		D2D.DrawBMP(Option, 0, 50, 1.f);
		D2D.DrawBMP(Option, 0, 200, 1.f);
		D2D.DrawBMP(Option, 0, 350,1.f);

		D2D.D2DDrawText(D2D1::ColorF::Black, RECT_F(50,
			70+(nowOption!=0?20:0),
			200,
			100 + (nowOption != 0 ? 20 : 0)
		), L"开始", nowOption==0?TxFormat[1]: TxFormat[2]);

		D2D.D2DDrawText(D2D1::ColorF::Black, RECT_F(50,
			220 + (nowOption != 1 ? 20 : 0),
			200,
			250 + (nowOption != 0 ? 20 : 0)
		), L"选项", nowOption == 1 ? TxFormat[1] : TxFormat[2]);

		D2D.D2DDrawText(D2D1::ColorF::Black, RECT_F(50,
			370 + (nowOption != 2 ? 20 : 0),
			200,
			400 + (nowOption != 0 ? 20 : 0)
		), L"退出", nowOption == 2 ? TxFormat[1] : TxFormat[2]);
		if (blkstate != -1) {
			if (blkstate == 0) {
				D2D.DrawBMP(blk, 0, 0, alk);
				alk += 0.1f;
				if (alk >= 1) {
					blkstate++;
				}
			}
			else if (blkstate == 1) {
				D2D.DrawBMP(blk, 0, 0, 1);
				state = 2;
				bgm.Stop();
				
				st = clock();
				blkstate = -1;
			}
		}
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(0, WinHei - 70, 200, WinHei), version, TxFormat[0]);//viss
	}
	//------------------------------------
	

	//--------------state:1---------------
	else if (state == 1) {

		int now_c = clock() - st;
		

		D2D.DrawBMP(bgi, 0, 0, 1);

		
		

		if (keynum == 6) {

			for (int i = 0; i < 6; i++) {
				if (Salp[i] >0) {
					D2D.DrawBMP(whiteStr, i * 158, 0, Salp[i]);
					//MessageBox(hwnd, "OK", "ok", MB_OK);
				}

			}

			for (int i = 0; i < nowblk.size(); i++) {
				D2D.DrawBMP(block, nowblk[i].PI.x, nowblk[i].PI.y, nowblk[i].alf);
			}
			D2D.DrawBMP(line, -10, WinHei - 160, 1);
		}
		else {

			for (int i = 0; i < 3; i++) {
				if (Salp[i] >0) {
					if(i==0)
						D2D.DrawBMP(whiteStr, 50+10, 0,220,WinHei, Salp[i],false);
					else if (i == 1)
						D2D.DrawBMP(whiteStr, 330+10, 0, 220, WinHei, Salp[i], false);
					else
						D2D.DrawBMP(whiteStr, 610+10, 0, 220, WinHei, Salp[i], false);
					//MessageBox(hwnd, "OK", "ok", MB_OK);
				}

			}

			for (int i = 0; i < nowblk.size(); i++) {
				D2D.DrawBMP(block, nowblk[i].PI.x, nowblk[i].PI.y,70,70, nowblk[i].alf,false);
			}

			D2D.DrawBMP(line1, 50, WinHei-170, 1);
			D2D.DrawBMP(line2, 330, WinHei-170, 1);
			D2D.DrawBMP(line3, 610, WinHei - 170, 1);
		}

		std::string str;
		str = toSTR(score);
		LPCWSTR wstr;
		WCHAR wcr[60];
		MultiByteToWideChar(0, 0, str.c_str(), -1, wcr, 64);
		wstr = wcr;
		
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(20, 10, 200, 50),wstr , TxFormat[0]);
		
		for (int i = 0; i < spk.size(); i++) {

			LPCWSTR wstr;
			WCHAR wcr[60];
			MultiByteToWideChar(0, 0, spk[i].txt.c_str(), -1, wcr, 64);
			wstr = wcr;

			D2D.DrawBMP(plrPic[spk[i].plr], spk[i].displyPicTime, 250, 1.f);

			if (!spk[i].disply_w&&spk[i].displyPicTime <= 15)
				spk[i].displyPicTime += 3;
			else if (spk[i].disply_w)
				spk[i].displyPicTime -= 3;

			if (spk[i].displyPicTime >= 15 && spk[i].LONG + spk[i].displyTime < now_c)
				spk[i].disply_w = true;

			if (spk[i].LONG + spk[i].displyTime >= now_c) {
				D2D.DrawBMP(spk_blk, spk[i].displyPicTime + 70, 200, 1.f);
				D2D.D2DDrawText(D2D1::ColorF::Black, RECT_F(spk[i].displyPicTime + 80, 225, spk[i].displyPicTime + 260, 300), wstr, TxFormat[0]);
			}
		}
		
		//debug:
		/*std::string tmp;
		for (int i = 0; i < 6; i++) {
			tmp += char(int(chkkeybor[i]) + 48);
			tmp += "   ";
		}

		LPCWSTR wstrr;
		WCHAR wcrr[60];
		MultiByteToWideChar(0, 0, tmp.c_str(), -1, wcrr, 64);
		wstrr = wcrr;

		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(100, 0, 300, 100), wstrr, TxFormat);
		//*/

		if (now_c <= 3000) {
			LPCWSTR wstrr;
			WCHAR wcrr[60];
			MultiByteToWideChar(0, 0, nameList[nowSave].c_str(), -1, wcrr, 64);
			wstrr = wcrr;

			D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(50, 200, 960, 500), wstrr, TxFormat[2]);
		}

	}
	
	//------------------state:2---------------------------
	else if (state == 2) {
		D2D.DrawBMP(lfbu, 120, 150, 1.f);
		D2D.DrawBMP(ribu, 650, 150, 1.f);

		WCHAR wchr[60];
		MultiByteToWideChar(0, 0, nameList[nowSave].c_str(), -1, wchr, 64);
		LPCWSTR wt = wchr;

		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(300, 330, 600, 230), wt, TxFormat[3]);
	
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(100, 450, 200, 600), L"返回", TxFormat[3]);

		
		if (keynum == 3)
			D2D.D2DDrawText(D2D1::ColorF::Red, RECT_F(300, 400, 800, 600), L"按M选择困难模式(六轨)", TxFormat[4]);
		else
			D2D.D2DDrawText(D2D1::ColorF::Green, RECT_F(300, 400, 800, 600), L"按M选择普通模式(三轨)", TxFormat[4]);
	}
	//----------------------------------------------------

	//--------------------state:3-------------------------
	else if (state == 3) {
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(200, 100, 400, 200), L"主菜单", TxFormat[3]);
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(200, 200, 400, 300), L"继续", TxFormat[3]);
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(200, 300, 400, 400), L"离开", TxFormat[3]);
		if (nowOption == 0)
			D2D.D2DDrawText(D2D1::ColorF::Blue, RECT_F(200, 100, 400, 200), L"主菜单", TxFormat[3]);
		else if (nowOption == 1)
			D2D.D2DDrawText(D2D1::ColorF::Blue, RECT_F(200, 200, 400, 300), L"继续", TxFormat[3]);
		else if (nowOption == 2)
			D2D.D2DDrawText(D2D1::ColorF::Blue, RECT_F(200, 300, 400, 400), L"离开", TxFormat[3]);
	
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(0, 430, 600, 600), L"按键说明:", TxFormat[0]);
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(0, 450, 600, 620), L"选择:  Enter", TxFormat[0]);
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(0, 470, 600, 640), L"上下,左右选择:  方向键", TxFormat[0]);
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(0, 490, 600, 660), L"游戏中弹奏:  ASD JKL", TxFormat[0]);



	}
	//----------------------------------------------------

	//-------------------state:4--------------------------
	else if (state == 4) {
		std::string str;
		str = toSTR(score);
		LPCWSTR wstr;
		WCHAR wcr[60];
		MultiByteToWideChar(0, 0, str.c_str(), -1, wcr, 64);
		wstr = wcr;

		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(100, 100, 400, 400), L"分数:", TxFormat[3]);
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(300, 100, 700, 400), wstr, TxFormat[3]);
		

		


		str = toSTR(FullScore);
		MultiByteToWideChar(0, 0, str.c_str(), -1, wcr, 64);
		wstr = wcr;

		
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(100, 200, 300, 400), L"最高分:", TxFormat[3]);
		D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(300, 200, 600, 400),wstr, TxFormat[3]);
		
		if (flg) {
			D2D.D2DDrawText(D2D1::ColorF::White, RECT_F(100, 270, 800, 560), L"新纪录！", TxFormat[1]);
			
		}

		D2D.D2DDrawText(D2D1::ColorF::Yellow, RECT_F(100, 400, 300, 500),L"继续", TxFormat[3]);
		
		if (blkstate != -1) {
			if (blkstate == 0) {
				D2D.DrawBMP(blk, 0, 0, alk);
				alk += 0.1f;
				if (alk >= 1) {
					blkstate++;
				}
			}
			else if (blkstate == 1) {
				D2D.DrawBMP(blk, 0, 0, 1);
				state = 0;


				st = clock();
				blkstate = -1;

				bgm.Create("res\\music\\Bg_Music\\Background_music.mp3", true, hwnd);

				bgm.Play(0, -1, 1);
			}
		}

	}
	//----------------------------------------------------

	//------------------------------------

	/*
	*模板
	D2D.BeginDr();

	//D2D.SetTrans(1.0);

	D2D.ClearScr(D2D1::ColorF(D2D1::ColorF::Black));

	D2D.DrawBMP(BG, 0, 0, 1.f);
	D2D.DrawBMP(Bird, 0, 0, 400, 400, 1.f);
	//D2D.DrawBMP(Bird2, 500, 500, 1.f);
	D2D.DrawBMP(XG, 500, 500, 1.f);

	D2D.D2DDrawText(D2D1::ColorF::Black, RC, version, TxFormat);

	D2D.EndDr();
	*/

	for (int i = 0; i < spk2.size(); i++) {

		LPCWSTR wstr;
		WCHAR wcr[60];
		MultiByteToWideChar(0, 0, spk2[i].txt.c_str(), -1, wcr, 64);
		wstr = wcr;

		D2D.DrawBMP(plrPic[spk2[i].plr], spk2[i].displyPicTime, 250, 1.f);

		if (!spk2[i].disply_w&&spk2[i].displyPicTime <= 15)
			spk2[i].displyPicTime += 3;
		else if (spk2[i].disply_w)
			spk2[i].displyPicTime -= 3;

		if (spk2[i].displyPicTime >= 15 &&clock()-spk2[i].start>=spk2[i].displyTime)
			spk2[i].disply_w = true;

		D2D.DrawBMP(spk_blk, spk2[i].displyPicTime + 70, 200, 1.f);
		D2D.D2DDrawText(D2D1::ColorF::Black, RECT_F(spk2[i].displyPicTime + 80, 225, spk2[i].displyPicTime + 250, 300), wstr, TxFormat[0]);
		break;
	}

	D2D.EndDr();
}

void Loading(HWND hwnd)
{
	
	start_1=D2D.LoadBMP(L"res\\picture\\start_1.png", 1000, 500);
	
	int Time_ = clock();
	//-----------"loading"----------------
	D2D.BeginDr();
	D2D.DrawBMP(start_1, -30,-50, 1.f);
	D2D.EndDr();
	//------------------------------------

	LoadStand();

	//------------logo--------------------
	logo[0] = D2D.LoadBMP(L"res\\picture\\logo0.png", 200, 200);
	//------------------------------------

	//---------------text-----------------
	TxFormat[0] = D2D.LoadText(L"华文新魏", 20.0f, L"en-us");
	TxFormat[1] = D2D.LoadText(L"微软雅黑", 70.0f, L"en-us");
	TxFormat[2] = D2D.LoadText(L"华文新魏", 60.0f, L"en-us");
	TxFormat[3] = D2D.LoadText(L"华文新魏", 50.0f, L"en-us");
	TxFormat[4] = D2D.LoadText(L"微软雅黑", 30.0f, L"en-us");
	
	RECT_F tmpRc(WinWid/2-100,400, WinWid / 2+ 100, 430);
	myinf_rect[0]=tmpRc;
	
	//------------------------------------

	//-------------load picture-----------
	Option = D2D.LoadBMP(L"res\\picture\\Option.png", 300, 150);
	title = D2D.LoadBMP(L"res\\picture\\title.png", 500, 300);
	blk = D2D.LoadBMP(L"res\\picture\\blk.png", WinWid, WinHei);
	line = D2D.LoadBMP(L"res\\picture\\line.png", WinWid,150);
	block = D2D.LoadBMP(L"res\\picture\\block.png", 50, 50);
	lfbu = D2D.LoadBMP(L"res\\picture\\lfbu.png", 200, 200);
	ribu = D2D.LoadBMP(L"res\\picture\\ribu.png", 200, 200);
	spk_blk = D2D.LoadBMP(L"res\\picture\\spk_block.png", 200, 105);
	whiteStr = D2D.LoadBMP(L"res\\picture\\whiteStr.png", (WinWid - 30) / 6, WinHei);
	plrPic[0] = D2D.LoadBMP(L"res\\picture\\logo0.png", 70, 70);
	line1 = D2D.LoadBMP(L"res\\picture\\line1.png", WinWid / 4 , 130);
	line2 = D2D.LoadBMP(L"res\\picture\\line2.png", WinWid/4, 130);
	line3 = D2D.LoadBMP(L"res\\picture\\line3.png", WinWid / 4 , 130);

	std::string tmP;
	tmP = "res\\Save\\Bg_Speak\\Background_Speak.mcs";
;

	std::ifstream is;
	is.open(tmP.c_str(), std::ios::in);

	int spn;
	is >> spn;
	spn++;
	
	for (int i = 0; i < spn; i++) {
		getline(is, tmP);
		//MessageBox(NULL, tmP.c_str(), "", MB_OK);
		bkspk.push_back(tmP);
	}
	
	std::vector<std::string>::iterator it = bkspk.begin();
	bkspk.erase(it);
	is.close();
	//------------------------------------
	/*
	*模板
	RGBA_F Col(1.f, 0.f, 0.f, 0.f);
	//RGBA_F Col2(1.f, 0.f, 0.f, 1.f);

	BG = D2D.LoadBMP(L"Res\\day.bmp", ScrWid, ScrHei);
	Bird = D2D.LoadBMP(L"Res\\bird.bmp", 500, 500);
	//Bird2 = D2D.LoadAlphaBMP(L"Res\\bird.bmp", 200, 200, Col2);
	XG = D2D.LoadBMP(L"Res\\Gua.png", 200, 200);

	RC.top = float(ScrHei - 30);
	RC.bottom = float(ScrHei);

	TxFormat = D2D.LoadText(L"华文新魏", 20.0f, L"en-us");*/

	//---------load picture--------------

	//----------load music--------------------
	
	//-----------------------------------------

	//-----------------------------------
	//while (clock() - Time_ < 700);
	
}

void GameRelease()
{
	/*
	*模板
	SafeRelease(BG);
	SafeRelease(Bird);
	SafeRelease(Bird2);

	SafeRelease(TxFormat);
	SafeRelease(XG);

	*/
	SafeRelease(start_1);
	SafeRelease(Option);
	SafeRelease(title);
	SafeRelease(blk);
	SafeRelease(line);
	SafeRelease(block);
	SafeRelease(lfbu);
	SafeRelease(ribu);
	SafeRelease(bgi);
	SafeRelease(spk_blk);
	SafeRelease(whiteStr);
	SafeRelease(line1);
	SafeRelease(line2);
	SafeRelease(line3);

	for (int i = 0; i < TxNum; i++) {
		SafeRelease(TxFormat[i]);
	}

	for (int i = 0; i < logon; i++) {
		SafeRelease(logo[i]);
	}
	for (int i = 0; i < plrN; i++) {
		SafeRelease(plrPic[i]);
	}

	bgm.Stop();
}

