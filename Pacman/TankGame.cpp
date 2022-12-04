#include "TankGame.h"
#include <sstream>

//Constructor
TankGame::TankGame(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.1f), _cPlayerFrameTime(100), _cAmmoFrameTime(500), _cDroneFrameTime(50),_cExplosionFrameTime(100)
{
	//Seed random
	srand(time(NULL));
	
	//Initialise important Game aspects
	_player = new Player();
	_player->isPlayerDead = false;
	
	for (int i = 0; i < AMMOPICKUPCOUNT; i++)
	{
		_ammoPickup[i] = new AmmoPickup();
		_ammoPickup[i]->_ammoFrameCount = rand() % 1;
		_ammoPickup[i]->_ammoCurrentFrameTime = 0;
		_ammoPickup[i]->_ammoFrame = rand() % 500 + 50;
	}

	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		_drones[i] = new MovingEnemy();
		_drones[i]->direction = 0;
		_drones[i]->speed = 0.2f;
		_drones[i]->_droneFrameCount = rand() % 1;
		_drones[i]->_droneCurrentFrame = 0;
		_drones[i]->_droneFrame = rand() % 500 + 50;
	}
	
	for (int i = 0; i < EXPLOSIONS; i++)
	{
		_explosions[i] = new Explosion();
		_explosions[i]->_boomFrameCount = 0;
		_explosions[i]->_boomCurrentFrame = 0;
		_explosions[i]->_boomFrame = rand() % 500 + 50;
		_explosions[i]->inUse = false;
	}
	
	
	Graphics::Initialise(argc, argv, this, 1280, 720, false, 25, 25, "Tank Game", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

//Destructor
TankGame::~TankGame() 
{
	delete _player->_playerTexture;
	delete _player->_playerTurretTexture;
	delete _player->_playerSourceRect;
	delete _player->_playerTurretSourceRect;
	delete _player->_playerTurretPosition;
	delete _player->_mousePosition;

	for (int i = 0; i < AMMOPICKUPCOUNT; i++)
	{
		delete _ammoPickup[i]->_ammoRect;
		delete _ammoPickup[i]->position;
		delete _ammoPickup[i];
	}
	
	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		delete _drones[i]->position;
		delete _drones[i]->sourceRect;
		delete _drones[i];
	}

	for (int i = 0; i < EXPLOSIONS; i++)
	{
		delete _explosions[i]->position;
		delete _explosions[i]->sourceRect;
		delete _explosions[i];
	}

	delete _ammoTexture;
	delete _boomTexture;
	delete[] _ammoPickup;
	delete _menuBackground;
	delete _player;
	delete[] _drones;
	delete _droneTexture;

}

void TankGame::LoadContent() 
{
	//Initilize Player
	_player->_playerTexture = new Texture2D();
	_player->_playerTurretTexture = new Texture2D();
	_player->_playerTexture->Load("Textures/TankBaseSheet.png", false);
	_player->_playerTurretTexture->Load("Textures/TankTurretSheet.png", false);
	_player->_playerPosition = new Vector2(350.0f, 350.0f);
	_player->_playerLastPosition = new Vector2(350.0f, 350.0f);
	_player->_mousePosition = new Vector2(0.0f, 0.0f);
	_player->_playerTurretPosition = new Vector2(0.0f, 0.0f);
	
	_player->_playerSourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_player->_playerTurretSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	_player->_playerDirection = 0;
	_player->_playerCurrentFrameTime = 0;
	_player->_playerFrame = 0;
	_player->_score = 0;
	_player->_ammo = 3;
	_player->_isPlayerMoving = false;
	_player->_turretRotation = 0.0f;

	//Initilize Ammo
	_ammoTexture->Load("Textures/Ammo.png", false);
	for (int i = 0; i < AMMOPICKUPCOUNT; i++)
	{	
		_ammoPickup[i]->_ammoTexture = _ammoTexture;
		_ammoPickup[i]->_ammoRect = new Rect(0.0f, 0.0f, 16, 16);
		_ammoPickup[i]->_ammoCurrentFrameTime = 0;
		_ammoPickup[i]->_ammoFrameCount = 0;
		_ammoPickup[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ammoPickup[i]->_isFollowingMouse = false;
	}

	//Initilize Drones
	_droneTexture->Load("Textures/Drone.png", false);
	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		_drones[i]->texture = _droneTexture;
		_drones[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_drones[i]->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	}

	//Initilize Explosions
	_boomTexture->Load("Textures/Explosion.png", false);
	for (int i = 0; i < EXPLOSIONS; i++)
	{
		_explosions[i]->texture = _boomTexture;
		_explosions[i]->position = new Vector2(-200,-200);
		_explosions[i]->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	}

	//Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
	_stringPosition2 = new Vector2(10.0f, 50.0f);

	//Set menu parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_startStringPosition = new Vector2(550.0f, Graphics::GetViewportHeight() / 2.0f);
	_paused = true;
	_startGameMenu = true;
	_escKeyDown = false;
}

bool CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1, left2 = x2, right1 = x1 + width1, right2 = x2 + width2, top1 = y1, top2 = y2, bottom1 = y1 + height1, bottom2 = y2 + height2;
	if (bottom1 < top2)
		return false;
	if (top1 > bottom2)
		return false;
	if (right1 < left2)
		return false;
	if (left1 > right2)
		return false;

	return true;
}

/*float GetRadians(Vector2* p1, Vector2* p2)
{
	float length1, length2;
	length1 = p1->X - p2->Y;
	length2 = p1->Y - p2->Y;
	return (atan(length2 / length1));
}*/

float getDegrees(float x, float y) {
	float angle = 0;
	if (x < 0) {
		angle = (float)(270 - (atan(y / -x) * 180 / 3.14159));
	}
	else {
		angle = (float)(90 + (atan(y / x) * 180 / 3.14159));
	}
	return int(angle);
}

void TankGame::Input(int elapsedTime, Input::KeyboardState* state,Input::MouseState* mouseState)
{
	//WASD player input and movement.
	if (state->IsKeyDown(Input::Keys::D))
	{
		_player->_playerPosition->X += _cPlayerSpeed * elapsedTime;
		_player->_playerDirection = 0;
	}
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_player->_playerPosition->X -= _cPlayerSpeed * elapsedTime;
		_player->_playerDirection = 2;
	}
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_player->_playerPosition->Y += _cPlayerSpeed * elapsedTime;
		_player->_playerDirection = 1;
	}
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_player->_playerPosition->Y -= _cPlayerSpeed * elapsedTime;
		_player->_playerDirection = 3;
	}

	_player->_mousePosition->X = mouseState->X;
	_player->_mousePosition->Y = mouseState->Y;
	
}

void TankGame::ShowExplosion(Vector2* position)
{
	bool foundBoom = false;

	for (int i = 0; i < EXPLOSIONS; i++)
	{
		if (_explosions[i]->inUse == false)
		{
			_explosions[i]->position->X = position->X;
			_explosions[i]->position->Y = position->Y;
			_explosions[i]->inUse = true;
			break;
		}


	}
}

void TankGame::UpdateDrone(MovingEnemy* drone, int elapsedTime, int i)
{
	if (drone->direction == 0)//Move Right
	{
		drone->position->X += drone->speed * elapsedTime;
	}
	else if (drone->direction == 1)//Move Left
	{
		drone->position->X -= drone->speed * elapsedTime;
	}

	if (drone->position->X + drone->sourceRect->Width >= Graphics::GetViewportWidth())//Change direction on right edge.
	{
		drone->direction = 1;
	}
	else if (drone->position->X <= 0)
	{
		drone->direction = 0;
	}
	
	//Tracking time since last drone animation frame change.
	_drones[i]->_droneCurrentFrame += elapsedTime;
	//Updating the drone frame.
	if (_drones[i]->_droneCurrentFrame > _cDroneFrameTime)
	{
		_drones[i]->sourceRect->X = _drones[i]->sourceRect->Width * _drones[i]->_droneFrameCount;
		_drones[i]->_droneFrameCount++;

		if (_drones[i]->_droneFrameCount >= 2)
		{
			_drones[i]->_droneFrameCount = 0;
		}

		_drones[i]->_droneCurrentFrame = 0;
	}
}

void TankGame::UpdateBoom(int elapsedTime, int i) 
{
	if (_explosions[i]->inUse == true)
	{
		//Tracking time since last drone animation frame change.
		_explosions[i]->_boomCurrentFrame += elapsedTime;
		//Updating the explosion frame.
		if (_explosions[i]->_boomCurrentFrame > _cExplosionFrameTime)
		{
			_explosions[i]->sourceRect->X = _explosions[i]->sourceRect->Width * _explosions[i]->_boomFrameCount;
			_explosions[i]->_boomFrameCount++;

			if (_explosions[i]->_boomFrameCount >= 5)
			{
				_explosions[i]->_boomFrameCount = 0;
				_explosions[i]->position->X = -200;
				_explosions[i]->position->Y = -200;
				_explosions[i]->inUse = false;

			}

			_explosions[i]->_boomCurrentFrame = 0;
		}
	}
	
}

void TankGame::CheckPaused(Input::KeyboardState* state)
{
	//Checks for pause input.
	if (state->IsKeyDown(Input::Keys::ESCAPE) && !_escKeyDown && !_startGameMenu)
	{
		_escKeyDown = true;
		_paused = !_paused;
	}

	if (state->IsKeyUp(Input::Keys::ESCAPE))
		_escKeyDown = false;

	if ((state->IsKeyDown(Input::Keys::SPACE) && _startGameMenu))
	{
		_startGameMenu = false;
		_paused = !_paused;
	}
}

void TankGame::CheckViewportCollision()
{
	//Colliding with the walls.
	if (_player->_playerPosition->X + _player->_playerSourceRect->Width > Graphics::GetViewportWidth())
	{
		_player->_playerPosition->X = Graphics::GetViewportWidth() - _player->_playerSourceRect->Width;
	}

	if (_player->_playerPosition->X < 0)
	{
		_player->_playerPosition->X = 0;
	}

	if (_player->_playerPosition->Y + _player->_playerSourceRect->Height > Graphics::GetViewportHeight())
	{
		_player->_playerPosition->Y = Graphics::GetViewportHeight() - _player->_playerSourceRect->Height;
	}

	if (_player->_playerPosition->Y < 0)
	{
		_player->_playerPosition->Y = 0;
	}
}

void TankGame::UpdatePlayer(int elapsedTime)
{
	//Checking if the player is moving.
	if (_player->_playerDirection == 0 || _player->_playerDirection == 2)
	{
		if (_player->_playerLastPosition->X > _player->_playerPosition->X || _player->_playerLastPosition->X < _player->_playerPosition->X)
		{
			_player->_isPlayerMoving = true;
		}
		else
		{
			_player->_isPlayerMoving = false;
		}
	}
	else
	{
		if (_player->_playerLastPosition->Y > _player->_playerPosition->Y || _player->_playerLastPosition->Y < _player->_playerPosition->Y)
		{
			_player->_isPlayerMoving = true;
		}
		else
		{
			_player->_isPlayerMoving = false;
		}
	}

	if (_player->_isPlayerMoving)
	{
		//Tracking time since last player animation frame change.
		_player->_playerCurrentFrameTime += elapsedTime;

		//Updating the x column of the animation.
		if (_player->_playerCurrentFrameTime > _cPlayerFrameTime)
		{
			_player->_playerFrame++;

			if (_player->_playerFrame >= 4)
			{
				_player->_playerFrame = 0;
			}

			_player->_playerCurrentFrameTime = 0;

			_player->_playerSourceRect->X = _player->_playerSourceRect->Width * _player->_playerFrame;
		}
	}

	//Changing player direction
	_player->_playerSourceRect->Y = _player->_playerSourceRect->Height * _player->_playerDirection;
}

void TankGame::UpdateAmmoPickups(int i,int elapsedTime)
{
	//Tracking time since last ammo animation frame change.
	_ammoPickup[i]->_ammoCurrentFrameTime += elapsedTime;

	//Updating the ammo frame.
	if (_ammoPickup[i]->_ammoCurrentFrameTime > _cAmmoFrameTime)
	{
		_ammoPickup[i]->_ammoRect->X = _ammoPickup[i]->_ammoRect->Width * _ammoPickup[i]->_ammoFrameCount;
		_ammoPickup[i]->_ammoFrameCount++;

		if (_ammoPickup[i]->_ammoFrameCount >= 2)
		{
			_ammoPickup[i]->_ammoFrameCount = 0;
		}

		_ammoPickup[i]->_ammoCurrentFrameTime = 0;
	}
	
}

void TankGame::Update(int elapsedTime)
{
	//Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	//Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	CheckPaused(keyboardState);

	//For things that need to stop when paused.
	if (!_paused)
	{
		//Determine players last position for isMoving.
		_player->_playerLastPosition->X = _player->_playerPosition->X;
		_player->_playerLastPosition->Y = _player->_playerPosition->Y;

		//Determine turret rotation.
		_player->_turretRotation = getDegrees(_player->_mousePosition->X - _player->_playerTurretPosition->X , _player->_mousePosition->Y - _player->_playerTurretPosition->Y);

		Input(elapsedTime, keyboardState, mouseState);
		UpdatePlayer(elapsedTime);

		for (int i = 0; i < AMMOPICKUPCOUNT; i++)
		{
			UpdateAmmoPickups(i, elapsedTime);
		}

		for (int i = 0; i < EXPLOSIONS; i++)
		{
			UpdateBoom(elapsedTime,i);
		}

		//Drone Collision
		for (int i = 0; i < ENEMYCOUNT; i++)
		{
			UpdateDrone(_drones[i], elapsedTime,i);
			if (CollisionCheck(_player->_playerPosition->X, _player->_playerPosition->Y, _player->_playerSourceRect->Width, _player->_playerSourceRect->Height, _drones[i]->position->X,
				_drones[i]->position->Y, _drones[i]->sourceRect->Width, _drones[i]->sourceRect->Height))
			{
				_player->isPlayerDead = true;
				ShowExplosion(_player->_playerPosition);
				i = ENEMYCOUNT;
			}
		}

		//Ammo Collision
		for (int i = 0; i < AMMOPICKUPCOUNT; i++)
		{
			if (CollisionCheck(_player->_playerPosition->X, _player->_playerPosition->Y, _player->_playerSourceRect->Width, _player->_playerSourceRect->Height, _ammoPickup[i]->position->X,
				_ammoPickup[i]->position->Y, _ammoPickup[i]->_ammoRect->Width, _ammoPickup[i]->_ammoRect->Height))
			{
				_player->_ammo += 3;
				_ammoPickup[i]->position->Y = -150;
				_ammoPickup[i]->position->X = -150;
				i = AMMOPICKUPCOUNT;
			}
		}

		CheckViewportCollision();
		
		//Set turret position.
		_player->_playerTurretPosition = _player->_playerPosition;

	}
}

void TankGame::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream score;
	score << "Score: " << _player->_score;
	std::stringstream ammo;
	ammo << "Ammo: " << _player->_ammo;
	
	SpriteBatch::BeginDraw(); // Starts Drawing
	
	if (!_player->isPlayerDead)
	{
		SpriteBatch::Draw(_player->_playerTexture, _player->_playerPosition, _player->_playerSourceRect); //Draws player.
		SpriteBatch::Draw(_player->_playerTurretTexture, _player->_playerPosition, _player->_playerTurretSourceRect, _player->_playerTurretPosition, 1, _player->_turretRotation, Color::White, SpriteEffect::NONE); //Draws player turret.
	}

	//draw ammo
	for (int i = 0; i < AMMOPICKUPCOUNT; i++)
	{
		SpriteBatch::Draw(_ammoPickup[i]->_ammoTexture, _ammoPickup[i]->position, _ammoPickup[i]->_ammoRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}

	//Draw drones
	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		SpriteBatch::Draw(_drones[i]->texture, _drones[i]->position, _drones[i]->sourceRect);
	}

	//Draw explosions
	for (int i = 0; i < EXPLOSIONS; i++)
	{
		SpriteBatch::Draw(_explosions[i]->texture, _explosions[i]->position, _explosions[i]->sourceRect);
	}

	// Draws Strings
	SpriteBatch::DrawString(score.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::DrawString(ammo.str().c_str(), _stringPosition2, Color::Green);
	
	if (_paused && !_startGameMenu)
	{
		std::stringstream menuStream; 
		menuStream << "Paused";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::White);
	}
	else if (_paused && _startGameMenu)
	{
		std::stringstream menuStream;
		menuStream << "Press Space to Start";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(),_startStringPosition, Color::Green);
	}
	
	SpriteBatch::EndDraw(); // Ends Drawing
}

