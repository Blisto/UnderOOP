/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
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
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#include "CEraser.h"
#include "CBall.h"
#include "CBouncingBall.h"
#include<string.h>

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};


	//練習 4：將圖片的貼圖和移動等動作，製作成一個「物件」
	class Cpratice
	{
	public:
		Cpratice();
		void LoadBitmap();
		void OnMove();
		void OnShow();
	private:
		CMovingBitmap pic;
		int x, y;
	};
	//

	//
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap logo;								// csie的logo
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	//練習 5：利用陣列建立一個遊戲地圖的類別
	class CGameMapp
	{
	public:
		CGameMapp();
		void LoadBitmap();
		void OnShow();
		void OnMove();  //練習6
		void OnKeyDown(UINT);
		void RandomBouncingBall();
		void InitialzeBouncingBall(int, int, int);
		~CGameMapp();
	protected:
		CMovingBitmap blue, green;
		int map[4][5];
		const int X, Y;
		const int MW, MH;
		CBouncingBall *bballs;//練習6
		int random_num;
	};
	//

	//
	class CGameCharacter 
	{
	public:
		CGameCharacter(string _name,int C_NUM);
		void LoadBitmap();
		void OnMove(int mx, int my);					// 移動角色
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetXY(int nx, int ny);		// 設定擦子左上角座標
		int GetX();
		int GetY();
		bool MoveStepCheck(int mx,int my);
		void OnShow();
		~CGameCharacter();
	private:
		string name;
		CMovingBitmap characterBMP[10];
		int map[800][800];          // 角色的地圖系統
		void MapInit();
		int x, y, c_num;			// 角色左上角座標
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
	};
	//

	//負責處理遊戲地圖的class
	class CGameMap
	{
	public:
		CGameMap();
		void LoadBitmap();
		void OnMove(CGameCharacter*character);
		bool MoveStepCheck(CGameCharacter*character);
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetXY(int nx, int ny);		// 設定擦子左上角座標
		int GetX();
		int GetY();
		void OnShow();
		int GetMoveType();
		void Portal(CGameCharacter*character);
	private:
		CMovingBitmap NowMap[10];
		int map[10][800][800];		// 包含10張地圖的座標資訊 座標系統可容納4000x4000像素地圖 1格=5x5像素
		void MapInit();             // 初始化地圖資訊
		int x, y, nowMap_num, xyMode;		// 地圖左上角座標,當前地圖,地圖XY軸模式
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
	};
	//


	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		///////////////////////////
		Cpratice c_pratice;
		CGameMapp gamemap;
		///////////////////////////

		CGameMap *map;
		CGameCharacter *frisk;

		const int		NUMBALLS;	// 球的總數
		CMovingBitmap	background;	// 背景圖
		CMovingBitmap	help;		// 說明圖
		CBall			*ball;		// 球的陣列
		CMovingBitmap	corner;		// 角落圖
		CEraser			eraser;		// 拍子
		CInteger		hits_left;	// 剩下的撞擊數
		CBouncingBall   bball;		// 反覆彈跳的球

	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;	// 倒數之計數器
	};

}