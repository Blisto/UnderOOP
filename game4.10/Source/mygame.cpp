/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
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
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//�m�� 4�G�N�Ϥ����K�ϩM���ʵ��ʧ@�A�s�@���@�ӡu����v
/////////////////////////////////////////////////////////////////////////////
	Cpratice::Cpratice() 
	{
		x = y = 0;
	}
	void Cpratice::OnMove()
	{
		if (y <= SIZE_Y) {
			x += 3;
			y += 3;
		}
		else {
			x = y = 0;
		}
	}
	void Cpratice::OnShow()
	{
		pic.SetTopLeft(x, y);
		pic.ShowBitmap();
	}
	void Cpratice::LoadBitmap()
	{
		pic.LoadBitmap(IDB_map);
	}
/////////////////////////////////////////////////////////////////////////////
//�m�� 5�G�Q�ΰ}�C�إߤ@�ӹC���a�Ϫ����O
/////////////////////////////////////////////////////////////////////////////
	CGameMapp::CGameMapp():X(30),Y(30),MW(130),MH(130)
	{
		int map_init[4][5] = {
			{ 1,2,1,2,1 },
			{ 2,1,2,1,2 },
			{ 1,2,1,2,1 },
			{ 2,1,2,1,2 }
		};

		for (int i = 0; i < 4; i++) 
		{
			for (int j = 0; j < 5; j++)
			{
				map[i][j] = map_init[i][j];
			}
		}
		random_num = 0;
		bballs = NULL;
	}
	void CGameMapp::LoadBitmap() 
	{
		blue.LoadBitmap(ID_BLUE);
		green.LoadBitmap(ID_GREEN);
	}
	void CGameMapp::OnShow()
	{
		for (int i = 0; i < 5; i++) 
			for (int j = 0; j < 4; j++) 
			{
				if (map[j][i] != 0) {
					switch (map[j][i]) 
					{
					case 1:
						blue.SetTopLeft(X+(MW*i),Y+(MH*j) );
						blue.ShowBitmap();
						break;
					case 2:
						green.SetTopLeft(X + (MW*i), Y + (MH*j));
						green.ShowBitmap();
						break;
					default:
						ASSERT(0);
					}
				}
			}
		for (int i = 0; i < random_num; i++) 
		{
			bballs[i].OnShow();
		}
	}
/////////////////////////////////////////////////////////////////////////////
//�m��6
/////////////////////////////////////////////////////////////////////////////
	void CBouncingBall::SetXY(int x, int y) 
	{
		this->x=x;
		this->y=y;
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

	void CGameMapp::InitialzeBouncingBall(int ini_index,int row,int col)
	{
		const int VELOCITY = 10;
		const int BALL_PIC_HEIGHT = 15;
		int floor = Y + (row + 1)*MH - BALL_PIC_HEIGHT;

		bballs[ini_index].LoadBitmap();
		bballs[ini_index].SetFloor(floor);
		bballs[ini_index].SetVelocity(VELOCITY + col);
		bballs[ini_index].SetXY(X + col*MH + MW / 2, floor);
	}
	void CGameMapp::RandomBouncingBall()
	{
		const int MAX_RAND_NUM = 10;
		random_num = (rand() % MAX_RAND_NUM) + 1;
		
		delete[]bballs;
		bballs = new CBouncingBall[random_num];
		int ini_index=0;
		for (int row = 0; row < 4; row++) 
		{
			for (int col = 0; col < 5; col++) 
			{
				if (map[row][col] != 0 && ini_index < random_num) 
				{
					InitialzeBouncingBall(ini_index, row, col);
					ini_index++;
				}
			}
		}
	}
	void CGameMapp::OnKeyDown(UINT nChar) 
	{
		const int KEY_SPACE = 0x20;
		if (nChar == KEY_SPACE) 
		{
			RandomBouncingBall();
		}
	}
	void CGameMapp::OnMove()
	{
		for (int i = 0; i < random_num; i++)
		{
			bballs[i].OnMove();
		}
	}
	CGameMapp::~CGameMapp() 
	{
		delete []bballs;
	}
/////////////////////////////////////////////////////////////////////////////
/////////////////  Tutorial���e
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ________________________UnderOOP__________________________________________
/////////////////////////////////////////////////////////////////////////////

	CGameMap::CGameMap() 
	{
		nowMap_num = 1;
		isMovingDown = isMovingUp = isMovingLeft = isMovingRight = false;
		x = 140;
		y = 64;
	}
	
	void CGameMap::LoadBitmap() 
	{
		NowMap[0].LoadBitmap(IDB_map);
		NowMap[1].LoadBitmap(IDB_MAP001);
		NowMap[2].LoadBitmap(IDB_MAP002);
	}

	void CGameMap::OnMove() 
	{
		const int STEP_SIZE = 5;

		if (isMovingLeft)
			x -= STEP_SIZE;
		if (isMovingRight)
			x += STEP_SIZE;
		if (isMovingUp)
			y -= STEP_SIZE;
		if (isMovingDown)
			y += STEP_SIZE;
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
	void CGameMap::OnShow() 
	{
		NowMap[nowMap_num].SetTopLeft(x, y);
		NowMap[nowMap_num].ShowBitmap();
	}




	CGameCharacter::CGameCharacter(string _name,int C_NUM)
	{
		this->name =_name;
		this->c_num = C_NUM;
		isMovingDown = isMovingUp = isMovingLeft = isMovingRight = false;
		x = y = 0;
	}
	void CGameCharacter::LoadBitmap() 
	{
		characterBMP[1].LoadBitmap(IDB_CHARACTER001,RGB(255,255,255));
	}
	void CGameCharacter::OnMove() 
	{
		const int STEP_SIZE = 5;

		if (isMovingLeft)
			x -= STEP_SIZE;
		if (isMovingRight)
			x += STEP_SIZE;
		if (isMovingUp)
			y -= STEP_SIZE;
		if (isMovingDown)
			y += STEP_SIZE;
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
	void CGameCharacter::OnShow() 
	{
		characterBMP[c_num].SetTopLeft((SIZE_X/2)-(characterBMP[c_num].Width()/2)+x, (SIZE_Y/2)-(characterBMP[c_num].Height()/2)+y);
		characterBMP[c_num].ShowBitmap();
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
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
	//
	// �}�l���J���
	//
	logo.LoadBitmap(IDB_BACKGROUND);
	Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
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
		GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
	else if (nChar == KEY_ESC)								// Demo �����C������k
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// �����C��
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
	//
	// �K�Wlogo
	//
	logo.SetTopLeft((SIZE_X - logo.Width())/2, SIZE_Y/8);
	logo.ShowBitmap();
	//
	// Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
	//
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// ���� font f; 160���16 point���r
	fp=pDC->SelectObject(&f);					// ��� font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	pDC->TextOut(120,220,"Please click mouse or press SPACE to begin.");
	pDC->TextOut(5,395,"Press Ctrl-F to switch in between window mode and full screen mode.");
	if (ENABLE_GAME_PAUSE)
		pDC->TextOut(5,425,"Press Ctrl-Q to pause the Game.");
	pDC->TextOut(5,455,"Press Alt-F4 or ESC to Quit.");
	pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}								

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
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
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
	//
	// �}�l���J���
	//
	Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// �̲׶i�׬�100%
	//
	ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// ���� font f; 160���16 point���r
	fp=pDC->SelectObject(&f);					// ��� font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
	sprintf(str, "Game Over ! (%d)", counter / 30);
	pDC->TextOut(240,210,str);
	pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g), NUMBALLS(28)
{
	ball = new CBall [NUMBALLS];
	frisk = new CGameCharacter("frisk",1);
}

CGameStateRun::~CGameStateRun()
{
	delete [] ball;
	delete frisk;
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
	for (int i = 0; i < NUMBALLS; i++) {				// �]�w�y���_�l�y��
		int x_pos = i % BALL_PER_ROW;
		int y_pos = i / BALL_PER_ROW;
		ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
		ball[i].SetDelay(x_pos);
		ball[i].SetIsAlive(true);
	}
	eraser.Initialize();
	background.SetTopLeft(BACKGROUND_X,0);				// �]�w�I�����_�l�y��
	help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
	hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
	hits_left.SetTopLeft(HITS_LEFT_X,HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
	CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
	CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
	CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI
}

void CGameStateRun::OnMove()							// ���ʹC������
{

	//
	// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
	//
	// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
	//
	// ���ʭI���Ϫ��y��
	//
	/*
	if (background.Top() > SIZE_Y)
		background.SetTopLeft(60 ,-background.Height());
	background.SetTopLeft(background.Left(),background.Top()+1);
	//
	// ���ʲy
	//
	int i;
	for (i=0; i < NUMBALLS; i++)
		ball[i].OnMove();
	//
	// �������l
	//
	eraser.OnMove();
	//
	// �P�_���l�O�_�I��y
	//
	for (i=0; i < NUMBALLS; i++)
		if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
			ball[i].SetIsAlive(false);
			CAudio::Instance()->Play(AUDIO_DING);
			hits_left.Add(-1);
			//
			// �Y�Ѿl�I�����Ƭ�0�A�h����Game Over���A
			//
			if (hits_left.GetInteger() <= 0) {
				CAudio::Instance()->Stop(AUDIO_LAKE);	// ���� WAVE
				CAudio::Instance()->Stop(AUDIO_NTUT);	// ���� MIDI
				GotoGameState(GAME_STATE_OVER);
			}
		}
	//
	// ���ʼu�����y
	//
	bball.OnMove();
	*/
	map.OnMove();
	frisk->OnMove();
}

void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
	map.LoadBitmap();
	frisk->LoadBitmap();
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
	//
	// �}�l���J���
	//
	int i;
	for (i = 0; i < NUMBALLS; i++)	
		ball[i].LoadBitmap();								// ���J��i�Ӳy���ϧ�
	eraser.LoadBitmap();
	background.LoadBitmap(IDB_BACKGROUND);					// ���J�I�����ϧ�
	//
	// ��������Loading�ʧ@�A�����i��
	//
	ShowInitProgress(50);
	Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// �~����J��L���
	//
	help.LoadBitmap(IDB_HELP,RGB(255,255,255));				// ���J�������ϧ�
	corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
	corner.ShowBitmap(background);							// �Ncorner�K��background
	bball.LoadBitmap();										// ���J�ϧ�
	hits_left.LoadBitmap();									
	CAudio::Instance()->Load(AUDIO_DING,  "sounds\\ding.wav");	// ���J�s��0���n��ding.wav
	CAudio::Instance()->Load(AUDIO_LAKE,  "sounds\\lake.mp3");	// ���J�s��1���n��lake.mp3
	CAudio::Instance()->Load(AUDIO_NTUT,  "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
	//
	// ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
	//
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard���b�Y
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
	if (nChar == KEY_LEFT)
		//frisk->SetMovingLeft(true);
		//eraser.SetMovingLeft(true);
		map.SetMovingRight(true);
	if (nChar == KEY_RIGHT)
		//frisk->SetMovingRight(true);
		//eraser.SetMovingRight(true);
		map.SetMovingLeft(true);
	if (nChar == KEY_UP)
		//frisk->SetMovingUp(true);
		//eraser.SetMovingUp(true);
		map.SetMovingDown(true);
	if (nChar == KEY_DOWN)
		//frisk->SetMovingDown(true);
		//eraser.SetMovingDown(true);
		map.SetMovingUp(true);
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard���b�Y
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
	if (nChar == KEY_LEFT)
		//frisk->SetMovingLeft(false);
		//eraser.SetMovingLeft(false);
		map.SetMovingRight(false);
	if (nChar == KEY_RIGHT)
		//frisk->SetMovingRight(false);
		//eraser.SetMovingRight(false);
		map.SetMovingLeft(false);
	if (nChar == KEY_UP)
		//frisk->SetMovingUp(false);
		//eraser.SetMovingUp(false);
		map.SetMovingDown(false);
	if (nChar == KEY_DOWN)
		//frisk->SetMovingDown(false);
		//eraser.SetMovingDown(false);
		map.SetMovingUp(false);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
	eraser.SetMovingLeft(true);
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
	eraser.SetMovingLeft(false);
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
	// �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
	eraser.SetMovingRight(true);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
	eraser.SetMovingRight(false);
}

void CGameStateRun::OnShow()
{
	map.OnShow();
	//
	//  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
	//        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
	//        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
	//
	//
	//  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
	//


	//background.ShowBitmap();			// �K�W�I����
	help.ShowBitmap();					// �K�W������
	//hits_left.ShowBitmap();
	

	/*
	for (int i=0; i < NUMBALLS; i++)
		ball[i].OnShow();				// �K�W��i���y
	bball.OnShow();						// �K�W�u�����y
	eraser.OnShow();					// �K�W���l
	*/


	//
	//  �K�W���W�Υk�U��������
	//
	//corner.SetTopLeft(0,0);
	//corner.ShowBitmap();
	//corner.SetTopLeft(SIZE_X-corner.Width(), SIZE_Y-corner.Height());
	//corner.ShowBitmap();
	
	
	//c_pratice.OnShow();// �m��4
	//�m��5or6
	//gamemap.OnShow();


/////////////////////////////////////////////////////////////////////////////
// ________________________UnderOOP__________________________________________
/////////////////////////////////////////////////////////////////////////////

	

	frisk->OnShow();




/////////////////////////////////////////////////////////////////////////////
// ________________________UnderOOP__________________________________________
/////////////////////////////////////////////////////////////////////////////


}
}