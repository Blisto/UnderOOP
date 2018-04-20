#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CObject.h"


namespace game_framework {

	CObject::CObject() 
	{
	
	}

	void CObject::LoadBitmap()
	{
		test.LoadBitmap(IDB_BITMAP3, RGB(255, 255, 255));
	}


	void CObject::Shift(int cy, int cx, int my, int mx)
	{
		y = cy - my;
		x = cx - mx;
	}

	void CObject::OnShow() 
	{
		test.SetTopLeft((SIZE_Y/2)-(test.Height()/2)+5*y, (SIZE_X / 2) - (test.Height() / 2)+5*x);
		test.ShowBitmap();
		
	}
}