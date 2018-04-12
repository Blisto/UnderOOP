#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CNonPlayerCharacter.h"

namespace game_framework {

	CNonPlayerCharacter::CNonPlayerCharacter(string _name)
	{
		this->name = _name;
		//this->c_num = C_NUM;
		isMovingDown = isMovingUp = isMovingLeft = isMovingRight = false;
		this->SetXY(0, 10);

		for (int j = 0; j < 800; j++) //初始化x軸
		{
			for (int k = 0; k < 800; k++)map[j][k] = 0; // 初始化y軸 0為不可通過
		}

	}
	void CNonPlayerCharacter::SetMapInfo(int mapInfo[800][800])
	{
		for (int i = 0; i < 800; i++)
		{
			for (int j = 0; j < 800; j++)
			{
				map[i][j] = mapInfo[i][j];
			}
		}
	}
	void CNonPlayerCharacter::LoadBitmap()
	{
		//characterBMP[1].LoadBitmap(IDB_CHARACTER001, RGB(255, 255, 255));
		animation.AddBitmap(IDB_FLOWEY001, RGB(255, 255, 255));
		animation.AddBitmap(IDB_FLOWEY002, RGB(255, 255, 255));
	}
	void CNonPlayerCharacter::OnMove(bool mapMoveing,bool characterMoveing)
	{
		if (mapMoveing == false || characterMoveing == false)return;

		const int STEP_SIZE = 1;

		if (isMovingLeft)
			x -= STEP_SIZE;
		if (isMovingRight)
			x += STEP_SIZE;
		if (isMovingUp)
			y -= STEP_SIZE;
		if (isMovingDown)
			y += STEP_SIZE;
	}

	void CNonPlayerCharacter::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}
	void CNonPlayerCharacter::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}
	void CNonPlayerCharacter::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}
	void CNonPlayerCharacter::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}
	void CNonPlayerCharacter::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}
	int CNonPlayerCharacter::GetX()
	{
		return ((SIZE_X / 2) - (characterBMP[c_num].Width() / 2) + (x * 5)) / 5;
		//return x;
	}
	int CNonPlayerCharacter::GetY()
	{
		return ((SIZE_Y / 2) - (characterBMP[c_num].Height() / 2) + (y * 5)) / 5;
		//return y;
	}
	bool CNonPlayerCharacter::MoveStepCheck(int mx, int my)
	{
		int cx = this->GetX(), cy = this->GetY();
		int lr = 0, ud = 0;
		if (isMovingRight == true)lr = 0;
		if (isMovingLeft == true)lr = 0;
		if (isMovingUp == true)ud = -1;
		if (isMovingDown == true)ud = 1;

		if (lr == 0 && cy - my + ud >= 0 && cx - mx >= 0 && map[cy - my + ud][cx - mx] == 1 && map[cy - my + ud + 6][cx - mx + 4] == 1)return true;
		//if (ud == 0 && cy - my >= 0 && cx - mx + lr >= 0 && map[cy - my][cx - mx + lr] == 1 && map[cy - my + 6][cx - mx + lr + 4] == 1)return true;

		return false;
	}
	bool CNonPlayerCharacter::isOnPortal()
	{
		return false;
	}
	void CNonPlayerCharacter::OnShow(int mapNum)
	{
		if (mapNum != 2)return;
		//characterBMP[c_num].SetTopLeft((SIZE_X / 2) - (characterBMP[c_num].Width() / 2) + (x * 5), (SIZE_Y / 2) - (characterBMP[c_num].Height() / 2) + (y * 5));
		//characterBMP[c_num].ShowBitmap();
		animation.SetTopLeft((SIZE_X / 2) - (animation.Width() / 2) + (x * 5), (SIZE_Y / 2) - (animation.Height() / 2) + (y * 5));
		animation.OnMove();
		animation.OnShow();
	}

	CNonPlayerCharacter::~CNonPlayerCharacter()
	{
	
	}

}

