#pragma once
namespace game_framework {

	class CObject 
	{
	public:
		CObject();
		void LoadBitmap(int ny, int nx);
		void OnShow(int mw,int mh, int my, int mx);
	private:
		CMovingBitmap texture;
		int x, y;
	};

}