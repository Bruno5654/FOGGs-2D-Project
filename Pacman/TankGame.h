#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

//Macro
#define BUILDINGS 30

// Just need to include main header file
#include "S2D/S2D.h"
#include <string>
#include <time.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <math.h>

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
	Texture2D* _texture;
	Vector2* _position;
	int _ammoFrame;
	int _ammoCurrentFrameTime;
	int _ammoFrameCount;

	AmmoPickup(Texture2D* texture, Vector2* position)
	{
		_texture = texture;
		_ammoFrameCount = rand() % 1;
		_ammoCurrentFrameTime = 0;
		_ammoFrame = rand() % 500 + 50;
		_sourceRect = new Rect(0.0f, 0.0f, 16, 16);
		_ammoCurrentFrameTime = 0;
		_ammoFrameCount = 0;
		_position = position;
	}
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
	MovingEnemy(Texture2D* texture, Vector2* pos)
	{
		_texture = texture;
		_direction = 0;
		_speed = 0.2f;
		_droneFrameCount = rand() % 1;
		_droneCurrentFrame = 0;
		_droneFrame = rand() % 500 + 50;
		_position = pos;
		_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}
};

struct LerpEnemy
{
	Vector2* _position;
	Vector2 _direction;
	Texture2D* _texture;
	Rect* _sourceRect;
	float _orientation;

	int _frame;
	int _currentFrame;
	int _frameCount;
	float _speed;
	bool _shouldMove;
	LerpEnemy(Texture2D* texture, Vector2* pos)
	{
		_texture = texture;
		_orientation = 0.0f;
		_speed = 2.0f;
		_frameCount = rand() % 1;
		_currentFrame = 0;
		_frame = rand() % 500 + 50;
		_position = pos;
		_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
		_shouldMove = true;
	}
};

struct Explosion
{
	Vector2* _position;
	Texture2D* _texture;
	Rect* _sourceRect;
	int _boomFrame;
	int _boomCurrentFrame;
	int _boomFrameCount;
	Explosion(Texture2D* texture)
	{
		_texture = texture;
		_position = new Vector2(-200,-200);
		_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
		_boomCurrentFrame = 0;
		_boomFrameCount = 0;
		_boomFrame = rand() % 500 + 50;
	}
};

struct Bullet
{
	Vector2* _position;
	Vector2 _direction;
	Texture2D* _texture;
	Rect* _sourceRect;
	float _speed;
	float _orientation;
	bool _moving;

	Bullet(Texture2D* texture)
	{
		_texture = texture;
		_position = new Vector2(0, 0);
		_sourceRect = new Rect(0.0f, 0.0f, 8, 8);
		_speed = 10.0f;
		_orientation = 0.0f;
		_moving = true;

	}	
};

struct LandMine
{
	Vector2* _position;
	Texture2D* _texture;
	Rect* _sourceRect;
	int _frame;
	int _currentFrame;
	int _frameCount;

	LandMine(Texture2D* texture)
	{
		_texture = texture;
		_position = new Vector2(0, 0);
		_sourceRect = new Rect(0.0f, 0.0f, 8, 8);
		_frameCount = rand() % 1;
		_currentFrame = 0;
		_frame = rand() % 500 + 50;
	}
};

struct Building
{
	Vector2* _position;
	Texture2D* _texture;
	Rect* _sourceRect;
	int _stage;
	bool _isPassable;
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
	void UpdateDrone(int elapsedTime, int i);
	void UpdateBoom(int elapsedTime, int i);
	void UpdateBullet(int elapsedTime, int i);
	void UpdateBuilding(int i);
	void UpdateMissle(int elapsedTime,int i);
	void UpdateMine(int elapsedTime, int i);
	void ShowExplosion(Vector2* position);
	void KillPlayer();
	void FireBullet();
	void FireMine();
	void SpawnEnemy();

	//Constant data for Game Variables.
	const float _cPlayerSpeed;
	const int _cPlayerFrameTime;
	const int _cAmmoFrameTime;
	const int _cExplosionFrameTime;
	const int _cEnemyFrameTime;
	const int _cLandMineFrameTime;

	Player* _player;
	Building* _buildings[BUILDINGS];
	vector < AmmoPickup > AmmoVector;
	vector < Bullet > BulletVector;
	vector < Explosion > ExplosionVector;
	vector < MovingEnemy > DroneVector;
	vector < LerpEnemy > MissleVector;
	vector < LandMine > MinesVector;
	Texture2D* _ammoTexture = new Texture2D();
	Texture2D* _droneTexture = new Texture2D();
	Texture2D* _boomTexture = new Texture2D();
	Texture2D* _buildingTexture = new Texture2D();
	Texture2D* _bulletTexture = new Texture2D();
	Texture2D* _missleTexture = new Texture2D();
	Texture2D* _landMineTexture = new Texture2D();

	// Position for String
	Vector2* _stringPosition;
	Vector2* _stringPosition2;

	//Data for Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	Vector2* _startStringPosition;

	string _highScore;

	bool _paused;
	bool _escKeyDown;
	bool _startGameMenu;
	bool _leftMouseBeginDown; //True on first tick of mouse pressed.
	bool _rightMouseBeginDown; //True on first tick of mouse pressed.

	//Other Game Variables
	int gameState;
	int _initalAmmoCount = 5;
	float totalTime;

	//Sounds
	SoundEffect* _cannon;
	SoundEffect* _boom;
	SoundEffect* _damage;
	SoundEffect* _blip;
	SoundEffect* _gameOver;
	

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
int Lerp(int a, int b, float c);



