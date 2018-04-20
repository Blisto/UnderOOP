#pragma once
namespace game_framework {

	class CObject 
	{
	public:
		CObject();
		void LoadBitmap();
		void Shift(int cy,int cx ,int my, int mx);
		void OnShow();
	private:
		CMovingBitmap test;
		int x, y;
	};

}