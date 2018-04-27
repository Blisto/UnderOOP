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

	void CObject::LoadBitmap(int ny,int nx)
	{
		x = nx;
		y = ny;
		texture.LoadBitmap(IDB_BITMAP3, RGB(255, 255, 255));
	}

	void CObject::OnShow(int mw, int mh,int my,int mx)
	{
		texture.SetTopLeft((SIZE_X/2)-(mw/2) + 5 * mx + 5 * x, (SIZE_Y / 2) - (mh / 2)+5*my+ 5 * y);
		texture.ShowBitmap();
	}
}