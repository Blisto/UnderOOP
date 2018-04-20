/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////
	//練習 5：利用陣列建立一個遊戲地圖的類別
	/////////////////////////////////////////////////////////////////////////////
	
	void CBouncingBall::SetXY(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	void CBouncingBall::SetFloor(int floor)
	{
		this->floor = floor;
	}
	void CBouncingBall::SetVelocity(int velociity)
	{
		this->velocity = velociity;
		this->initial_velocity = velociity;
	}
	/////////////////////////////////////////////////////////////////////////////
	// ________________________UnderOOP__________________________________________
	/////////////////////////////////////////////////////////////////////////////

	CGameMap::CGameMap()
	{
		xyMode = 0;
		nowMap_num = 1;
		isMovingDown = isMovingUp = isMovingLeft = isMovingRight = false;
		x = 39;
		y = 0;
	
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 800; j++) //初始化x軸
			{
				for (int k = 0; k < 800; k++)map[i][j][k] = 0; // 初始化y軸 0為不可通過
			}
		}
		
	}
	void CGameMap::MapInit()//所有地圖的障礙物資訊 0:不可通過 1:可通過
	{
		for (int i = 0; i <= NowMap[1].Height() / 5; i++)
		{
			for (int j = 0; j <= NowMap[1].Width() / 5; j++)
			{
				if (i >= 13 && i <= 47 && j >= 4 && j <= 54)map[1][i][j] = 1;//左空地
				if (i >= 32 && i <= 40 && j >= 55 && j <= 130)map[1][i][j] = 1;//右走道
				if (i >= 21 && i <= 32 && j >= 119 && j <= 126)map[1][i][j] = 1;//門

				if (i >= 13 && i <= 16 && j >= 4 && j <= 11)map[1][i][j] = 0;//空地四角落
				if (i >= 13 && i <= 16 && j >= 47 && j <= 54)map[1][i][j] = 0;
				if (i >= 36 && i <= 47 && j >= 4 && j <= 7)map[1][i][j] = 0;
				if (i >= 40 && i <= 47 && j >= 8 && j <= 11)map[1][i][j] = 0;
				if (i >= 44 && i <= 47 && j >= 12 && j <= 15)map[1][i][j] = 0;
				if (i >= 44 && i <= 47 && j >= 43 && j <= 46)map[1][i][j] = 0;
				if (i >= 40 && i <= 47 && j >= 47 && j <= 54)map[1][i][j] = 0;
			}
		}

		map[1][22][19] = 2;
		map[1][22][20] = 2;
		map[1][23][19] = 2;
		map[1][23][20] = 2;

		for (int i = 0; i <= NowMap[2].Height() / 5; i++)
		{
			for (int j = 0; j <= NowMap[2].Width() / 5; j++)
			{
				if (i >= 33 && i <= 99 && j >= 0 && j <= 63)map[2][i][j] = 1;
				if (i >= 22 && i <= 32 && j >= 28 && j <= 36)map[2][i][j] = 1;
			}
		}

		for (int i = 0; i <= NowMap[3].Height() / 5; i++)
		{
			for (int j = 0; j <= NowMap[3].Width() / 5; j++)
			{
				map[3][i][j] = 1;
			}
		}
		for (int i = 0; i <= NowMap[3].Height() / 5; i++)
		{
			for (int j = 0; j <= NowMap[3].Width() / 5; j++)
			{
				if (i >= 26 && i <= 37 && j >= 17 && j <= 79)map[3][i][j] = 0;
				if (i >= 74 && i <= 85 && j >= 0 && j <= 62)map[3][i][j] = 0;
				if (i >= 105 && i <= 116 && j >= 0 && j <= 37)map[3][i][j] = 0;
				if (i >= 105 && i <= 116 && j >= 43 && j <= 79)map[3][i][j] = 0;
			}
		}




	}
	void CGameMap::LoadBitmap()
	{
		NowMap[0].LoadBitmap(IDB_MAP000);
		NowMap[1].LoadBitmap(IDB_MAP001, RGB(255, 255, 255));
		NowMap[2].LoadBitmap(IDB_MAP002, RGB(255, 255, 255));
		NowMap[3].LoadBitmap(IDB_MAP003, RGB(255, 255, 255));
		item.LoadBitmap();

		this->MapInit();
	}
	void CGameMap::SetCharacterMap(CGameCharacter*character) 
	{
		character->SetMapInfo(map[nowMap_num]);
	}
	void CGameMap::OnMove(CGameCharacter*character)
	{	
		if (this->MoveStepCheck(character)==false)
		{
			return;
		}

		const int STEP_SIZE = 1;
		if (isMovingLeft)
			x -= STEP_SIZE;
		if (isMovingRight)
			x += STEP_SIZE;
		if (isMovingUp)
			y -= STEP_SIZE;
		if (isMovingDown)
			y += STEP_SIZE;

		item.Shift(character->GetY(), character->GetX(),this->y,this->x);

	}
	bool CGameMap::MoveStepCheck(CGameCharacter*character) 
	{
		int mx = this->GetX(), my = this->GetY();
		int cx = character->GetX(), cy = character->GetY();
		int lr=0,ud=0;
		if (isMovingRight == true)lr = -1;
		if (isMovingLeft == true)lr = 1;
		if (isMovingUp == true)ud = 1;
		if (isMovingDown == true)ud = -1;
		if (ud == 0 && cy - my>=0 && cx - mx + lr>=0 && map[nowMap_num][cy - my][cx - mx+lr] == 1&& map[nowMap_num][cy - my+6][cx - mx + lr+4] == 1)return true;
		if (lr == 0 && cx - mx>=0 && cy - my + ud>=0 && map[nowMap_num][cy - my + ud][cx - mx] == 1 && map[nowMap_num][cy - my + ud + 6][cx - mx + 4] == 1)return true;
		return false;
	}
	void CGameMap::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}
	void CGameMap::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}
	void CGameMap::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}
	void CGameMap::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}
	void CGameMap::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}
	int CGameMap::GetX()
	{
		return ((SIZE_X / 2) - (NowMap[nowMap_num].Width() / 2) + (this->x * 5))/5;
	}
	int CGameMap::GetY()
	{
		return ((SIZE_Y / 2) - (NowMap[nowMap_num].Height() / 2) + (this->y * 5)) / 5;
	}
	int CGameMap::GetNowMapNum() 
	{
		return nowMap_num;
	}
	void CGameMap::OnShow()
	{
		NowMap[nowMap_num].SetTopLeft((SIZE_X / 2) - (NowMap[nowMap_num].Width() / 2) + (x * 5), (SIZE_Y / 2) - (NowMap[nowMap_num].Height() / 2)+(y * 5));
		//強制地圖置中 **必須依靠改變地圖左上座標進行初始化地圖位置的動作
		NowMap[nowMap_num].ShowBitmap();
		//item.OnShow();
	}
	void CGameMap::Mesg(CGameCharacter*character)
	{
		TRACE("c.x=%d,c.y=%d; m.x=%d,m.y=%d    TF=%d    Portal:cx=%d,cy=%d  mx=%d,my=%d", character->GetX(), character->GetY(), this->GetX(), this->GetY(), character->MoveStepCheck(this->GetX(), this->GetY()), character->GetX() * 5 + 10 - SIZE_X / 2, character->GetY() * 5 + 15 - SIZE_Y / 2, this->GetX() * 5 + NowMap[nowMap_num].Width() / 2 - SIZE_X / 2, this->GetY() * 5 + NowMap[nowMap_num].Height() / 2 - SIZE_Y / 2);

	}
	void CGameMap::Portal(CGameCharacter*character)
	{//需對應地圖捲動 設置位移量 將傳送點設置在正確位置or使用Map座標判斷
		int yShift,xShift;
		switch (nowMap_num)
		{
		case 1:
			if (character->GetY() >= 43 && character->GetY() <= 46 && this->GetX() >= -60 && this->GetX() <= -57)
			{
				nowMap_num = 2;
				this->x = 0;
				this->y = 0;
				character->SetXY(0, 37);
				character->SetMapInfo(map[nowMap_num]);
			}
			break;
		case 2:
			yShift = this->y;
			if (character->GetY() >= 20 + yShift && character->GetY() <= 24 + yShift && this->GetX() >= 30 && this->GetX() <= 34)
			{
				nowMap_num = 3;
				this->x = 0;
				this->y = -60;
				character->SetXY(0, 12);
				character->SetMapInfo(map[nowMap_num]);
			}
			if (character->GetY() >= 82 + yShift && character->GetY() <= 87 + yShift && this->GetX() >= 29 && this->GetX() <= 34)
			{
				nowMap_num = 1;
				this->x = -55;
				this->y = 0;
				character->SetXY(0, 4);
				character->SetMapInfo(map[nowMap_num]);
			}
			break;
		case 3:
			yShift = this->y - (-60);
			if (character->GetY() >= 60+ yShift && character->GetY() <= 63+ yShift && this->GetX() >= 22 && this->GetX() <= 25)
			{
				nowMap_num = 2;
				this->x = 0;
				this->y = 0;
				character->SetXY(0, -17);
				character->SetMapInfo(map[nowMap_num]);
			}
			break;

		default:
			break;
		}


	}





	CGameCharacter::CGameCharacter(string _name)
	{
		this->name =_name;
		this->c_num = 0;
		isMovingDown = isMovingUp = isMovingLeft = isMovingRight = false;
		this->SetXY(0, 0);
		
			for (int j = 0; j < 800; j++) //初始化x軸
			{
				for (int k = 0; k < 800; k++)map[j][k] = 0; // 初始化y軸 0為不可通過
			}

	}
	void CGameCharacter::SetMapInfo(int mapInfo[800][800])
	{
		for (int i = 0; i < 800; i++)
		{
			for (int j = 0; j < 800; j++)
			{
				map[i][j] = mapInfo[i][j];
			}
		}
	}
	void CGameCharacter::LoadBitmap() 
	{
		characterBMP[1].LoadBitmap(IDB_CHARACTER001, RGB(255, 255, 255));
		characterBMP[2].LoadBitmap(IDB_CHARACTER002, RGB(255, 255, 255));
		characterBMP[3].LoadBitmap(IDB_CHARACTER003, RGB(255, 255, 255));
		characterBMP[4].LoadBitmap(IDB_CHARACTER004, RGB(255, 255, 255));

		animation[0] = new CAnimation;
		animation[0]->AddBitmap(IDB_CHARACTER001, RGB(255, 255, 255));
		animation[1] = new CAnimation;
		animation[1]->AddBitmap(IDB_CHARACTER002, RGB(255, 255, 255));
		animation[2] = new CAnimation;
		animation[2]->AddBitmap(IDB_CHARACTER003, RGB(255, 255, 255));
		animation[3] = new CAnimation;
		animation[3]->AddBitmap(IDB_CHARACTER004, RGB(255, 255, 255));
		animation[4] = new CAnimation;
		animation[4]->AddBitmap(IDB_CHARACTER001_M1, RGB(255, 255, 255));
		animation[4]->AddBitmap(IDB_CHARACTER001_M2, RGB(255, 255, 255));
		animation[5] = new CAnimation;
		animation[5]->AddBitmap(IDB_CHARACTER002, RGB(255, 255, 255));
		animation[5]->AddBitmap(IDB_CHARACTER002_M, RGB(255, 255, 255));
		animation[6] = new CAnimation;
		animation[6]->AddBitmap(IDB_CHARACTER003_M1, RGB(255, 255, 255));
		animation[6]->AddBitmap(IDB_CHARACTER003_M2, RGB(255, 255, 255));
		animation[7] = new CAnimation;
		animation[7]->AddBitmap(IDB_CHARACTER004, RGB(255, 255, 255));
		animation[7]->AddBitmap(IDB_CHARACTER004_M, RGB(255, 255, 255));
		animation[4]->SetDelayCount(5);
		animation[5]->SetDelayCount(5);
		animation[6]->SetDelayCount(5);
		animation[7]->SetDelayCount(5);
	}
	void CGameCharacter::OnMove(int mx, int my)
	{
		if (this->MoveStepCheck(mx,my) == false)
		{
			return;
		}
		const int STEP_SIZE = 1;
		animation[c_num]->OnMove();
		if (isMovingLeft)
			x -= STEP_SIZE;
		if (isMovingRight)
			x += STEP_SIZE;
		if (isMovingUp && this->GetY() > 5)
		{
			y -= STEP_SIZE;
		}else
		{
			if(this->GetY() < 5)y = 5;
		}
		if (isMovingDown && this->GetY() < 85)
		{
			y += STEP_SIZE;
		}
		else 
		{
			if (this->GetY() > 85)y = 84;
		}
	}

	void CGameCharacter::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}
	void CGameCharacter::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}
	void CGameCharacter::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}
	void CGameCharacter::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}
	void CGameCharacter::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}
	void CGameCharacter::SetToword(int key) 
	{
		c_num = key;
	}
	int CGameCharacter::GetX()
	{
		return ((SIZE_X / 2) - (animation[c_num]->Width() / 2) + (x * 5))/5;
		//return x;
	}
	int CGameCharacter::GetY()
	{
		return ((SIZE_Y / 2) - (animation[c_num]->Height() / 2) + (y * 5))/5;
		//return y;
	}
	bool CGameCharacter::MoveStepCheck(int mx,int my)
	{
		int cx=this->GetX(), cy = this->GetY();
		int lr=0,ud = 0;
		if (isMovingRight == true)lr = 0;
		if (isMovingLeft == true)lr = 0;
		if (isMovingUp == true)ud = -1;
		if (isMovingDown == true)ud = 1;

		if (lr == 0 && cy - my + ud >= 0 && cx - mx>=0 && map[cy - my + ud][cx - mx  ] == 1 && map[cy - my + ud + 6][cx - mx + 4] == 1)return true;
		//if (ud == 0 && cy - my >= 0 && cx - mx + lr >= 0 && map[cy - my][cx - mx + lr] == 1 && map[cy - my + 6][cx - mx + lr + 4] == 1)return true;

		return false;
	}
	bool CGameCharacter::isOnPortal() 
	{
		return false;
	}
	void CGameCharacter::OnShow() 
	{
		//characterBMP[c_num].SetTopLeft((SIZE_X/2)-(characterBMP[c_num].Width()/2)+(x*5), (SIZE_Y/2)-(characterBMP[c_num].Height()/2)+(y * 5));
		//characterBMP[c_num].ShowBitmap();
		animation[c_num]->SetTopLeft((SIZE_X / 2) - (animation[c_num]->Width() / 2) + (x * 5), (SIZE_Y / 2) - (animation[c_num]->Height() / 2) + (y * 5));
		animation[c_num]->OnShow();

	}
	CGameCharacter::~CGameCharacter()
	{
		
	}


/////////////////////////////////////////////////////////////////////////////
// ________________________UnderOOP__________________________________________
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
}

void CGameStateInit::OnInit()
{

	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0);	// 一開始的loading進度為0%
	//
	// 開始載入資料
	//
	//logo.LoadBitmap(IDB_BACKGROUND);
	Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ESC = 27;
	const char KEY_SPACE = ' ';
	if (nChar == KEY_SPACE)
		GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
	else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// 關閉遊戲
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
	//
	// 貼上logo
	//
	//logo.SetTopLeft((SIZE_X - logo.Width())/2, SIZE_Y/8);
	//logo.ShowBitmap();
	//
	// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
	//
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(110,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	pDC->TextOut(120,220,"Please click mouse or press SPACE to begin.");
	pDC->TextOut(5,395,"Press Ctrl-F to switch in between window mode and full screen mode.");
	if (ENABLE_GAME_PAUSE)
		pDC->TextOut(5,425,"Press Ctrl-Q to pause the Game.");
	pDC->TextOut(5,455,"Press Alt-F4 or ESC to Quit.");
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}								

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{
}

void CGameStateOver::OnMove()
{
	counter--;
	if (counter < 0)
		GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
	counter = 30 * 5; // 5 seconds
}

void CGameStateOver::OnInit()
{
	
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
	//
	// 開始載入資料
	//
	Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 最終進度為100%
	//
	ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[80];								// Demo 數字對字串的轉換
	sprintf(str, "Game Over ! (%d)", counter / 30);
	pDC->TextOut(240,210,str);
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g), NUMBALLS(28)
{
	ball = new CBall [NUMBALLS];
	frisk = new CGameCharacter("Frisk");
	flowey = new CNonPlayerCharacter("Flowey");//NPC:Flowey
	map = new CGameMap();
}

CGameStateRun::~CGameStateRun()
{
	delete [] ball;
	delete frisk;
	delete map;
	delete flowey;
}

void CGameStateRun::OnBeginState()
{
	const int BALL_GAP = 90;
	const int BALL_XY_OFFSET = 45;
	const int BALL_PER_ROW = 7;
	const int HITS_LEFT = 10;
	const int HITS_LEFT_X = 590;
	const int HITS_LEFT_Y = 0;
	const int BACKGROUND_X = 60;
	const int ANIMATION_SPEED = 15;
	for (int i = 0; i < NUMBALLS; i++) {				// 設定球的起始座標
		int x_pos = i % BALL_PER_ROW;
		int y_pos = i / BALL_PER_ROW;
		ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
		ball[i].SetDelay(x_pos);
		ball[i].SetIsAlive(true);
	}
	eraser.Initialize();
	background.SetTopLeft(BACKGROUND_X,0);				// 設定背景的起始座標
	help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
	hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
	hits_left.SetTopLeft(HITS_LEFT_X,HITS_LEFT_Y);		// 指定剩下撞擊數的座標
	CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
	CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
	CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{

	//
	// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
	//
	if (map->GetNowMapNum() != 2)flowey->SetXY(0, 10);
	map->OnMove(frisk);
	flowey->OnMove(map->MoveStepCheck(frisk),frisk->MoveStepCheck(map->GetX(), map->GetY()));
	frisk->OnMove(map->GetX(),map->GetY());
	
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	map->LoadBitmap();
	frisk->LoadBitmap();
	map->SetCharacterMap(frisk);
	flowey->LoadBitmap();
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
	//
	// 開始載入資料
	//
	int i;
	for (i = 0; i < NUMBALLS; i++)	
		ball[i].LoadBitmap();								// 載入第i個球的圖形
	eraser.LoadBitmap();
	background.LoadBitmap(IDB_BACKGROUND);					// 載入背景的圖形
	//
	// 完成部分Loading動作，提高進度
	//
	ShowInitProgress(50);
	Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 繼續載入其他資料
	//
	help.LoadBitmap(IDB_HELP,RGB(255,255,255));				// 載入說明的圖形
	corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
	corner.ShowBitmap(background);							// 將corner貼到background
	bball.LoadBitmap();										// 載入圖形
	hits_left.LoadBitmap();									
	CAudio::Instance()->Load(AUDIO_DING,  "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
	CAudio::Instance()->Load(AUDIO_LAKE,  "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
	CAudio::Instance()->Load(AUDIO_NTUT,  "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
	//
	// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
	//
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	
	if (nChar == KEY_LEFT)
	{
		//frisk->SetMovingLeft(true);
		map->SetMovingRight(true);//人物Y軸
		frisk->SetToword(7);
		if (map->GetNowMapNum() == 2)flowey->SetMovingRight(true);
	}
	if (nChar == KEY_RIGHT)
	{
		//frisk->SetMovingRight(true);
		map->SetMovingLeft(true);//人物Y軸
		frisk->SetToword(5);
		if (map->GetNowMapNum() == 2)flowey->SetMovingLeft(true);
	}
	if (nChar == KEY_UP)
	{
		frisk->SetToword(6);
		//frisk->SetMovingUp(true);
		if (frisk->GetY() > 6)
		{
			frisk->SetMovingUp(true);
		}
		else
		{
			map->SetMovingDown(true);
			if (map->GetNowMapNum() == 2)flowey->SetMovingDown(true);
		}
	}
	if (nChar == KEY_DOWN)
	{
		//frisk->SetMovingDown(true);
		frisk->SetToword(4);
		if (frisk->GetY() < 84)
		{
			frisk->SetMovingDown(true);
		}
		else 
		{
			map->SetMovingUp(true);
			if (map->GetNowMapNum() == 2)flowey->SetMovingUp(true);
		}
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x25; // keyboard左箭頭
	const char KEY_UP = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN = 0x28; // keyboard下箭頭
	if (nChar == KEY_LEFT)
	{
		//frisk->SetMovingLeft(false);
		map->SetMovingRight(false);
		flowey->SetMovingRight(false);
		frisk->SetToword(3);
		map->Portal(frisk);
	}
	if (nChar == KEY_RIGHT)
	{
		//frisk->SetMovingRight(false);
		map->SetMovingLeft(false);
		flowey->SetMovingLeft(false);
		frisk->SetToword(1);
		map->Portal(frisk);
	}
	if (nChar == KEY_UP)
	{
		frisk->SetMovingUp(false);
		map->SetMovingDown(false);
		flowey->SetMovingDown(false);
		frisk->SetToword(2);
		map->Portal(frisk);
	}
	if (nChar == KEY_DOWN)
	{
		frisk->SetMovingDown(false);
		map->SetMovingUp(false);
		flowey->SetMovingUp(false);
		frisk->SetToword(0);
		map->Portal(frisk);
	}
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	//eraser.SetMovingLeft(true);
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	//eraser.SetMovingLeft(false);
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	//eraser.SetMovingRight(true);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	//eraser.SetMovingRight(false);
}

void CGameStateRun::OnShow()
{
	map->OnShow();
	//
	//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
	//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
	//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
	//
	//
	//  貼上背景圖、撞擊數、球、擦子、彈跳的球
	//


	//background.ShowBitmap();			// 貼上背景圖
	//help.ShowBitmap();					// 貼上說明圖
	//hits_left.ShowBitmap();
	

/////////////////////////////////////////////////////////////////////////////
// ________________________UnderOOP__________________________________________
/////////////////////////////////////////////////////////////////////////////

	

	map->Mesg(frisk);
	frisk->OnShow();
	flowey->OnShow((map->GetNowMapNum()));



/////////////////////////////////////////////////////////////////////////////
// ________________________UnderOOP__________________________________________
/////////////////////////////////////////////////////////////////////////////


}
}