class CGameCharacter;
namespace game_framework {

	class CNonPlayerCharacter
	{
	public:
		CNonPlayerCharacter(string _name);
		void LoadBitmap();
		void SetMapInfo(int mapInfo[800][800]);
		void OnMove(bool mapMoveing, bool characterMoveing);	// 移動角色
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetXY(int nx, int ny);		// 設定腳色左上角座標
		int GetX();
		int GetY();
		bool MoveStepCheck(int mx, int my);
		bool isOnPortal();
		void OnShow(int mapNum);
		~CNonPlayerCharacter();
	private:
		string name;
		CAnimation animation;
		int map[800][800];          // 角色的地圖系統
		int x, y, c_num;			// 角色左上角座標
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
	};



}
