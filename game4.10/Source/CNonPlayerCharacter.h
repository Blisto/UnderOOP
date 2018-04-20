class CGameCharacter;
namespace game_framework {

	class CNonPlayerCharacter
	{
	public:
		CNonPlayerCharacter(string _name);
		void LoadBitmap();
		void SetMapInfo(int mapInfo[800][800]);
		void OnMove(bool mapMoveing, bool characterMoveing);	// ���ʨ���
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetXY(int nx, int ny);		// �]�w�}�⥪�W���y��
		int GetX();
		int GetY();
		bool MoveStepCheck(int mx, int my);
		bool isOnPortal();
		void OnShow(int mapNum);
		~CNonPlayerCharacter();
	private:
		string name;
		CAnimation animation;
		int map[800][800];          // ���⪺�a�Ϩt��
		int x, y, c_num;			// ���⥪�W���y��
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
	};



}
