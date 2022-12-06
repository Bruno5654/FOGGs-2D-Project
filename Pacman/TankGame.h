#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

//Macro
#define ENEMYCOUNT 3
#define EXPLOSIONS 5

// Just need to include main header file
#include "S2D/S2D.h"
#include<time.h>

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Data to represent Player
struct Player
{
	Vector2* _position;
	Vector2* _lastPosition;
	Vector2* _turretPosition;
	Vector2* _mousePosition;
	Rect* _sourceRect;
	Rect* _turretSourceRect;
	Texture2D* _texture;
	Texture2D* _turretTexture;

	int _direction;
	int _playerFrame;
	int _playerCurrentFrameTime;
	int _ammo;
	int _score;
	float _turretRotation;

	bool _isPlayerMoving;
	bool isPlayerDead;
};

// Data to represent Ammo
struct AmmoPickup
{
	Rect* _sourceRect;
	static Texture2D _texture;
	Vector2* _position;
	int _ammoFrame;
	int _ammoCurrentFrameTime;
	int _ammoFrameCount;

	AmmoPickup()
	{
		_ammoFrameCount = rand() % 1;
		_ammoCurrentFrameTime = 0;
		_ammoFrame = rand() % 500 + 50;
		_sourceRect = new Rect(0.0f, 0.0f, 16, 16);
		_ammoCurrentFrameTime = 0;
		_ammoFrameCount = 0;
		_position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}

	//~AmmoPickup()
	//{
			//delete _sourceRect;
			//delete _position;
	//}
};

struct MovingEnemy
{
	Vector2* _position;
	Texture2D* _texture;
	Rect* _sourceRect;
	int _direction;
	int _droneFrame;
	int _droneCurrentFrame;
	int _droneFrameCount;
	float _speed;
};

struct Explosion
{
	Vector2* _position;
	Texture2D* _texture;
	Rect* _sourceRect;
	int _boomFrame;
	int _boomCurrentFrame;
	int _boomFrameCount;
	bool _inUse;
};

struct Bullet
{
	Vector2* _position;
	Texture2D* _texture;
	Rect* _sourceRect;
	float _speed;
};


class TankGame : public Game
{
private:

	//Input
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	//Check methods
	void CheckPaused(Input::KeyboardState* state);
	void CheckViewportCollision();

	//Update methods
	void UpdatePlayer(int elapsedTime);
	void UpdateAmmoPickups(int i, int elapsedTime);
	void UpdateDrone(MovingEnemy * drone, int elapsedTime, int i);
	void UpdateBoom(int elapsedTime, int i);
	void ShowExplosion(Vector2* position);
	void KillPlayer();

	//Constant data for Game Variables.
	const float _cPlayerSpeed;
	const int _cPlayerFrameTime;
	const int _cAmmoFrameTime;
	const int _cExplosionFrameTime;
	const int _cDroneFrameTime;

	Player* _player;
	MovingEnemy* _drones[ENEMYCOUNT];
	Explosion* _explosions[EXPLOSIONS];
	vector < AmmoPickup > AmmoVector;
	Texture2D* _ammoTexture = new Texture2D();
	Texture2D* _droneTexture = new Texture2D();
	Texture2D* _boomTexture = new Texture2D();

	// Position for String
	Vector2* _stringPosition;
	Vector2* _stringPosition2;

	//Data for Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	Vector2* _startStringPosition;
	bool _paused;
	bool _escKeyDown;
	bool _startGameMenu;

	int gameState;
	int _initalAmmoCount = 20;


public:
	/// <summary> Constructs the Pacman class. </summary>
	TankGame(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Player class. </summary>
	virtual ~TankGame();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};

bool CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
static float getDegrees(float x, float y);

