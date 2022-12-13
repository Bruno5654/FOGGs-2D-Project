#include "TankGame.h"


//Constructor
TankGame::TankGame(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.1f), _cPlayerFrameTime(100), _cAmmoFrameTime(500), _cExplosionFrameTime(75), _cEnemyFrameTime(100), _cLandMineFrameTime(1000)
{
	//Seed random
	srand(time(NULL));

	//Initialise important Game aspects
	_player = new Player();
	_player->isPlayerDead = false;


	for (int i = 0; i < BUILDINGS; i++)
	{
		_buildings[i] = new Building();
		_buildings[i]->_stage = 1;
		_buildings[i]->_isPassable = false;
	}

	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1280, 720, false, 25, 25, "Tank Game", 60);
	Input::Initialise();
	

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

//Destructor
TankGame::~TankGame()
{
	delete _player->_texture;
	delete _player->_turretTexture;
	delete _player->_sourceRect;
	delete _player->_turretSourceRect;
	delete _player->_turretPosition;
	delete _player->_mousePosition;

	for (int i = 0; i < BUILDINGS; i++)
	{
		delete _buildings[i]->_position;
		delete _buildings[i]->_sourceRect;
		delete _buildings[i];
	}

	delete _ammoTexture;
	delete _boomTexture;
	delete _menuBackground;
	delete _player;
	delete _droneTexture;
	delete[] * _buildings;
	delete _buildingTexture;
	delete _bulletTexture;
	delete _missleTexture;
	delete _landMineTexture;

	delete _stringPosition;
	delete _stringPosition2;

	delete _cannon;
	delete _boom;
	delete _damage;
	delete _blip;
	delete _gameOver;

}

void TankGame::LoadContent()
{
	//Initilize Player
	_player->_texture = new Texture2D();
	_player->_turretTexture = new Texture2D();
	_player->_texture->Load("Textures/TankBaseSheet.png", false);
	_player->_turretTexture->Load("Textures/TankTurretSheet.png", false);
	_player->_position = new Vector2(350.0f, 350.0f);
	_player->_lastPosition = new Vector2(350.0f, 350.0f);
	_player->_mousePosition = new Vector2(0.0f, 0.0f);
	_player->_turretPosition = new Vector2(0.0f, 0.0f);

	_player->_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_player->_turretSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	_player->_direction = 0;
	_player->_playerCurrentFrameTime = 0;
	_player->_playerFrame = 0;
	_player->_ammo = 3;
	_player->_turretRotation = 0.0f;

	//Initilize Buildings
	Vector2* randPos = new Vector2(0, 0);
	_buildingTexture->Load("Textures/Building.png", false);
	for (int i = 0; i < BUILDINGS; i++)
	{
		bool isOk = false;
		while (!isOk)
		{
			isOk = true;
			randPos = new Vector2((rand() % (Graphics::GetViewportWidth()-50)), (rand() % (Graphics::GetViewportHeight()-50)));
			for (int j = 0; j < i; j++)
			{
				isOk = !CollisionCheck(randPos->X, randPos->Y, 32, 32, _buildings[j]->_position->X, _buildings[j]->_position->Y, 32, 32);
				if (!isOk)
					break;
				isOk = !CollisionCheck(randPos->X, randPos->Y, 32, 32, _player->_position->X, _player->_position->Y, 32, 32);
				if (!isOk)
					break;
				
			}
		}
		_buildings[i]->_position = randPos;
		_buildings[i]->_texture = _buildingTexture;
		_buildings[i]->_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}

	//Initalize Ammo
	_ammoTexture->Load("Textures/Ammo.png", false);

	for (int i = 0; i < _initalAmmoCount; i++)
	{
		randPos = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		AmmoVector.push_back(*new AmmoPickup(_ammoTexture, randPos));
	}

	_bulletTexture->Load("Textures/Bullet.png", false);
	_missleTexture->Load("Textures/Missle.png", false);
	_landMineTexture->Load("Textures/LandMine.png", false);
	_droneTexture->Load("Textures/Drone.png", false);
	_boomTexture->Load("Textures/Explosion.png", false);

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

	//Set gameState
	gameState = 1;

	//High score
	ifstream inFile("HighScore.txt");
	getline(inFile, _highScore);
	inFile.close();

	//Load sound effects.
	_cannon = new SoundEffect();
	_cannon->Load("Sounds/Cannon.wav");
	_boom = new SoundEffect();
	_boom->Load("Sounds/Explosion.wav");
	_damage = new SoundEffect();
	_damage->Load("Sounds/Damage.wav");
	_blip = new SoundEffect();
	_blip->Load("Sounds/Blip.wav");
	_gameOver = new SoundEffect();
	_gameOver->Load("Sounds/GameOver.wav");
}

bool CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) //Returning true = collision
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

float getDegrees(float x, float y) //Calculate the inverse tangent (radians) and convert it to degrees for draw function.
{
	float angle = 0;

	if (x < 0) {
		angle = (float)(270 - (atan(y / -x) * 180 / 3.14159));
	}
	else {
		angle = (float)(90 + (atan(y / x) * 180 / 3.14159));
	}

	angle = (int(180 - angle) % 360) + 180;

	return int(angle);
}

int Lerp(int a, int b, float c) //Linear interpolation.
{
	return(a + c*(b - a));
}

void TankGame::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	Vector2 tempPosition = *_player->_position;
	bool isCol = false;

	if (gameState == 1)
	{
		//WASD player input and movement.
		if (state->IsKeyDown(Input::Keys::D))
		{
			tempPosition.X += _cPlayerSpeed * elapsedTime;
			_player->_direction = 0;
		}
		else if (state->IsKeyDown(Input::Keys::A))
		{
			tempPosition.X -= _cPlayerSpeed * elapsedTime;
			_player->_direction = 2;
		}
		else if (state->IsKeyDown(Input::Keys::S))
		{
			tempPosition.Y += _cPlayerSpeed * elapsedTime;
			_player->_direction = 1;
		}
		else if (state->IsKeyDown(Input::Keys::W))
		{
			tempPosition.Y -= _cPlayerSpeed * elapsedTime;
			_player->_direction = 3;
		}

		for (int i = 0; i < BUILDINGS; i++)
		{
			if (CollisionCheck(tempPosition.X, tempPosition.Y, _player->_sourceRect->Width, 10, _buildings[i]->_position->X, _buildings[i]->_position->Y, 18, _buildings[i]->_sourceRect->Height) && !_buildings[i]->_isPassable)
			{
				isCol = true;
			}
		}

		if (!isCol)
		{
			*_player->_position = tempPosition;
		}
	}

	//Mouse input.
	if (mouseState->LeftButton == Input::ButtonState::PRESSED && _player->_ammo >= 1 && !_leftMouseBeginDown)
	{
		_leftMouseBeginDown = true;
		FireBullet();
	}

	if (mouseState->LeftButton == Input::ButtonState::RELEASED)
	{
		_leftMouseBeginDown = false;
	}

	//Right mouse
	if (mouseState->RightButton == Input::ButtonState::PRESSED && !_rightMouseBeginDown)
	{
		_rightMouseBeginDown = true;
		FireMine();
	}

	if (mouseState->RightButton == Input::ButtonState::RELEASED)
	{
		_rightMouseBeginDown = false;
	}

	_player->_mousePosition->X = mouseState->X;
	_player->_mousePosition->Y = mouseState->Y;

}

void TankGame::FireBullet() //Create bullet with correct direction.
{
	Bullet* currentBullet = new Bullet(_bulletTexture);
	currentBullet->_position->X = _player->_turretPosition->X;
	currentBullet->_position->Y = _player->_turretPosition->Y;
	currentBullet->_orientation = _player->_turretRotation;
	Vector2 _dir = Vector2(_player->_mousePosition->X - _player->_turretPosition->X, _player->_mousePosition->Y - _player->_turretPosition->Y);
	_dir.Normalize();
	currentBullet->_direction = _dir;
	Audio::Play(_cannon);
	BulletVector.push_back(*currentBullet);
	_player->_ammo--;
}

void TankGame::FireMine()
{
	LandMine* currentMine = new LandMine(_landMineTexture);
	
	if (_player->_direction == 0)
	{
		currentMine->_position->X = _player->_turretPosition->X - 30;
		currentMine->_position->Y = _player->_turretPosition->Y;
	}
	else if (_player->_direction == 1)
	{
		currentMine->_position->X = _player->_turretPosition->X;
		currentMine->_position->Y = _player->_turretPosition->Y - 24;
	}
	else if (_player->_direction == 2)
	{
		currentMine->_position->X = _player->_turretPosition->X + 30;
		currentMine->_position->Y = _player->_turretPosition->Y;
	}
	else
	{
		currentMine->_position->X = _player->_turretPosition->X;
		currentMine->_position->Y = _player->_turretPosition->Y + 24;
	}
	MinesVector.push_back(*currentMine);
	
}

void TankGame::ShowExplosion(Vector2 * position) 
{
	Explosion currentExplosion = Explosion(_boomTexture);
	currentExplosion._position->X = position->X;
	currentExplosion._position->Y = position->Y;
	ExplosionVector.push_back(currentExplosion);
	Audio::Play(_boom);
	
}

void TankGame::UpdateDrone(int elapsedTime, int i)
{
	if (DroneVector[i]._direction == 0)//Move Right
	{
		DroneVector[i]._position->X += DroneVector[i]._speed * elapsedTime;
	}
	else if (DroneVector[i]._direction == 1)//Move Left
	{
		DroneVector[i]._position->X -= DroneVector[i]._speed * elapsedTime;
	}

	if (DroneVector[i]._position->X + DroneVector[i]._sourceRect->Width >= Graphics::GetViewportWidth())//Change direction on right edge.
	{
		DroneVector[i]._direction = 1;
	}
	else if (DroneVector[i]._position->X <= 0)
	{
		DroneVector[i]._direction = 0;
	}

	//Tracking time since last drone animation frame change.
	DroneVector[i]._droneCurrentFrame += elapsedTime;
	//Updating the drone frame.
	if (DroneVector[i]._droneCurrentFrame > _cEnemyFrameTime)
	{
		DroneVector[i]._sourceRect->X = DroneVector[i]._sourceRect->Width * DroneVector[i]._droneFrameCount;
		DroneVector[i]._droneFrameCount++;

		if (DroneVector[i]._droneFrameCount >= 2)
		{
			DroneVector[i]._droneFrameCount = 0;
		}
		DroneVector[i]._droneCurrentFrame = 0;
	}
}

void TankGame::UpdateMine(int elapsedTime, int i)
{
	//Tracking time since last drone animation frame change.
	MinesVector[i]._currentFrame += elapsedTime;
	//Updating the drone frame.
	if (MinesVector[i]._currentFrame > _cLandMineFrameTime)
	{
		MinesVector[i]._sourceRect->X = MinesVector[i]._sourceRect->Width * MinesVector[i]._frameCount;
		MinesVector[i]._frameCount++;

		if (MinesVector[i]._frameCount >= 2)
		{
			MinesVector[i]._frameCount = 0;
		}
		MinesVector[i]._currentFrame = 0;
	}
}

void TankGame::UpdateBoom(int elapsedTime, int i)
{
	//Tracking time since last drone animation frame change.
	ExplosionVector[i]._boomCurrentFrame += elapsedTime;
	//Updating the explosion frame.
	if (ExplosionVector[i]._boomCurrentFrame > _cExplosionFrameTime)
	{
		ExplosionVector[i]._sourceRect->X = ExplosionVector[i]._sourceRect->Width * ExplosionVector[i]._boomFrameCount;
		ExplosionVector[i]._boomFrameCount++;

		if (ExplosionVector[i]._boomFrameCount >= 6)
		{
			ExplosionVector[i]._boomFrameCount = 0;
			ExplosionVector[i]._position->X = -200;
			ExplosionVector[i]._position->Y = -200;
			if (_player->isPlayerDead == true) //Update the game state provided the explosion is for the player.
			{
				gameState = 2;
				_paused = true;
			}
		}
		ExplosionVector[i]._boomCurrentFrame = 0;
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
	if (_player->_position->X + _player->_sourceRect->Width > Graphics::GetViewportWidth())
	{
		_player->_position->X = Graphics::GetViewportWidth() - _player->_sourceRect->Width;
	}

	if (_player->_position->X < 0)
	{
		_player->_position->X = 0;
	}

	if (_player->_position->Y + _player->_sourceRect->Height > Graphics::GetViewportHeight())
	{
		_player->_position->Y = Graphics::GetViewportHeight() - _player->_sourceRect->Height;
	}

	if (_player->_position->Y < 0)
	{
		_player->_position->Y = 0;

	}
}

void TankGame::UpdatePlayer(int elapsedTime)
{
	//Checking if the player is moving.
	if (_player->_direction == 0 || _player->_direction == 2)
	{
		if (_player->_lastPosition->X > _player->_position->X || _player->_lastPosition->X < _player->_position->X)
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
		if (_player->_lastPosition->Y > _player->_position->Y || _player->_lastPosition->Y < _player->_position->Y)
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

			_player->_sourceRect->X = _player->_sourceRect->Width * _player->_playerFrame;
		}
	}

	//Changing player sprite direction
	_player->_sourceRect->Y = _player->_sourceRect->Height * _player->_direction;
}

void TankGame::UpdateAmmoPickups(int i, int elapsedTime)
{
	//Tracking time since last ammo animation frame change.
	AmmoVector[i]._ammoCurrentFrameTime += elapsedTime;

	//Updating the ammo frame.
	if (AmmoVector[i]._ammoCurrentFrameTime > _cAmmoFrameTime)
	{
		AmmoVector[i]._sourceRect->X = AmmoVector[i]._sourceRect->Width * AmmoVector[i]._ammoFrameCount;
		AmmoVector[i]._ammoFrameCount++;

		if (AmmoVector[i]._ammoFrameCount >= 2)
		{
			AmmoVector[i]._ammoFrameCount = 0;
		}

		AmmoVector[i]._ammoCurrentFrameTime = 0;
	}

}

void TankGame::UpdateBullet(int elapsedTime, int i)
{
	if (BulletVector[i]._moving)
	{
		Vector2 bulletVec = BulletVector[i]._direction;
		*BulletVector[i]._position += bulletVec * BulletVector[i]._speed;
	}
}

void TankGame::UpdateBuilding(int i) 
{
	
	if (_buildings[i]->_stage > 3)
		_buildings[i]->_stage = 3;

	switch (_buildings[i]->_stage)
	{
	case 1:
		_buildings[i]->_sourceRect->X = 0;
		break;
	case 2:
		_buildings[i]->_sourceRect->X = 32;
		break;
	case 3:
		_buildings[i]->_sourceRect->X = 64;
		_buildings[i]->_isPassable = true;
		break;
	default:
		_buildings[i]->_sourceRect->X = 0;
		break;
	}
}

void TankGame::UpdateMissle(int elapsedTime,int i)
{
	//Move missle towards player using Lerp.
	
	Vector2 _dir = Vector2(_player->_position->X - MissleVector[i]._position->X, _player->_position->Y - MissleVector[i]._position->Y);
	_dir.Normalize();
	MissleVector[i]._direction = _dir;

	if (MissleVector[i]._shouldMove)
	{
		*MissleVector[i]._position += MissleVector[i]._direction * MissleVector[i]._speed;
	}
	

	//Tracking time since last animation frame change.
	MissleVector[i]._currentFrame += elapsedTime;
	//Updating the frame.
	if (MissleVector[i]._currentFrame > _cEnemyFrameTime)
	{
		MissleVector[i]._sourceRect->X = MissleVector[i]._sourceRect->Width * MissleVector[i]._frameCount;
		MissleVector[i]._frameCount++;

		if (MissleVector[i]._frameCount >= 2)
		{
			MissleVector[i]._frameCount = 0;
		}
		MissleVector[i]._currentFrame = 0;
	}

	MissleVector[i]._orientation = getDegrees(_player->_position->X - MissleVector[i]._position->X, _player->_position->Y - MissleVector[i]._position->Y);
}

void TankGame::KillPlayer()
{
	_player->isPlayerDead = true;
	ShowExplosion(_player->_position);
	_player->_position->X = -200;
	_player->_position->Y = -200;

	//Check if new high score.
	if (_player->_score > stoi(_highScore))
	{
		fstream outFile("HighScore.txt");
		outFile << _player->_score << "\n";
		outFile.close();
	}
	
}

void TankGame::SpawnEnemy() 
{
	int enemyToSpawn;

	enemyToSpawn = rand() % 2;

	if (enemyToSpawn == 1)
	{
		LerpEnemy* currentEnemy = new LerpEnemy(_missleTexture);
		MissleVector.push_back(*currentEnemy);
	}
	else
	{
		MovingEnemy* currentEnemy = new MovingEnemy(_droneTexture);
		DroneVector.push_back(*currentEnemy);
	}
	
}

void TankGame::Update(int elapsedTime)
{
	int scoreBracket = 0;
	
	//Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	//Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	switch (gameState)
	{
	case 1:

		CheckPaused(keyboardState);

		//For things that need to stop when paused.
		if (!_paused)
		{
			totalTime += elapsedTime / 1000.0f;
			
			//Determine players last position for isMoving.
			_player->_lastPosition->X = _player->_position->X;
			_player->_lastPosition->Y = _player->_position->Y;

			//Determine turret rotation.
			_player->_turretRotation = getDegrees(_player->_mousePosition->X - _player->_turretPosition->X, _player->_mousePosition->Y - _player->_turretPosition->Y);

			Input(elapsedTime, keyboardState, mouseState);
			UpdatePlayer(elapsedTime);

			for (int i = 0; i < AmmoVector.size(); i++)
			{
				UpdateAmmoPickups(i, elapsedTime);
			}

			for (int i = 0; i < ExplosionVector.size(); i++)
			{
				UpdateBoom(elapsedTime, i);
			}
			
			for (int i = 0; i < BulletVector.size(); i++)
			{
				UpdateBullet(elapsedTime, i);
			}

			for (int i = 0; i < MinesVector.size(); i++)
			{
				UpdateMine(elapsedTime, i);
			}

			for (int i = 0; i < BUILDINGS; i++)
			{
				UpdateBuilding(i);
			}

			//Drone Collision
			for (int i = 0; i < DroneVector.size(); i++)
			{
				UpdateDrone(elapsedTime, i);
				if (CollisionCheck(_player->_position->X, _player->_position->Y, _player->_sourceRect->Width, _player->_sourceRect->Height, DroneVector[i]._position->X,
					DroneVector[i]._position->Y, DroneVector[i]._sourceRect->Width, DroneVector[i]._sourceRect->Height))
				{
					Audio::Play(_gameOver);
					KillPlayer();
				}
			}

			//Mine Collision
			for (int i = 0; i < MinesVector.size(); i++)
			{
				if (CollisionCheck(_player->_position->X, _player->_position->Y, 25, 25, MinesVector[i]._position->X,
					MinesVector[i]._position->Y, MinesVector[i]._sourceRect->Width, MinesVector[i]._sourceRect->Height))
				{
					MinesVector.clear();
					Audio::Play(_gameOver);
					KillPlayer();
				}
			}

			//Missle Collision
			for (int i = 0; i < MissleVector.size(); i++)
			{
				UpdateMissle(elapsedTime, i);
				if (CollisionCheck(_player->_position->X, _player->_position->Y, _player->_sourceRect->Width, _player->_sourceRect->Height, MissleVector[i]._position->X,
					MissleVector[i]._position->Y, 10, 10))
				{
					
					Audio::Play(_gameOver);
					KillPlayer();
				}
			}

			//Missle On Mine Collision
			for (int i = 0; i < MissleVector.size(); i++)
			{
				for (int j = 0; j < MinesVector.size(); j++)
				{
					if (CollisionCheck(MinesVector[j]._position->X, MinesVector[j]._position->Y, MinesVector[j]._sourceRect->Width, MinesVector[j]._sourceRect->Height, MissleVector[i]._position->X,
						MissleVector[i]._position->Y, MissleVector[i]._sourceRect->Width, MissleVector[i]._sourceRect->Height))
					{
						Vector2* missPosVec = new Vector2(MissleVector[i]._position->X, MissleVector[i]._position->Y);
						MinesVector.erase(MinesVector.begin() + j);
						AmmoVector.push_back(*new AmmoPickup(_ammoTexture, missPosVec));
						ShowExplosion(MissleVector[i]._position);
						MissleVector.erase(MissleVector.begin() + i);
						_player->_score += 3;
						Audio::Play(_damage);
					}
				}
			}

			//Drone On Mine Collision
			for (int i = 0; i < DroneVector.size(); i++)
			{
				for (int j = 0; j < MinesVector.size(); j++)
				{
					if (CollisionCheck(MinesVector[j]._position->X, MinesVector[j]._position->Y, MinesVector[j]._sourceRect->Width, MinesVector[j]._sourceRect->Height, DroneVector[i]._position->X,
						DroneVector[i]._position->Y, DroneVector[i]._sourceRect->Width, DroneVector[i]._sourceRect->Height))
					{
						Vector2* dronePosVec = new Vector2(DroneVector[i]._position->X, DroneVector[i]._position->Y);
						MinesVector.erase(MinesVector.begin() + j);
						AmmoVector.push_back(*new AmmoPickup(_ammoTexture, dronePosVec));
						ShowExplosion(DroneVector[i]._position);
						DroneVector.erase(DroneVector.begin() + i);
						_player->_score += 2;
						Audio::Play(_damage);
					}
				}
			}

			//Ammo Collision
			for (int i = 0; i < AmmoVector.size(); i++)
			{
				if (CollisionCheck(_player->_position->X, _player->_position->Y, _player->_sourceRect->Width, _player->_sourceRect->Height, AmmoVector[i]._position->X,
					AmmoVector[i]._position->Y, AmmoVector[i]._sourceRect->Width, AmmoVector[i]._sourceRect->Height))
				{
					_player->_ammo += 3;
					Audio::Play(_blip);
					AmmoVector.erase(AmmoVector.begin() + i);
				}
			}

			//Bullet on Building Collision
			for (int i = 0; i < BUILDINGS; i++)
			{
				for (int j = 0; j < BulletVector.size(); j++)
				{
					if (CollisionCheck(_buildings[i]->_position->X, _buildings[i]->_position->Y, _buildings[i]->_sourceRect->Width, _buildings[i]->_sourceRect->Height, BulletVector[j]._position->X,
						BulletVector[j]._position->Y, BulletVector[j]._sourceRect->Width, BulletVector[j]._sourceRect->Height))
					{
						if (_buildings[i]->_stage < 3)
						{
							BulletVector.erase(BulletVector.begin() + j);
							ShowExplosion(_buildings[i]->_position);
							_buildings[i]->_stage++;
						}
						
					}
				}
			}
			
			//Bullet on Drone Collision
			for (int i = 0; i < DroneVector.size(); i++)
			{
				for (int j = 0; j < BulletVector.size(); j++)
				{
					if (CollisionCheck(DroneVector[i]._position->X, DroneVector[i]._position->Y, DroneVector[i]._sourceRect->Width, DroneVector[i]._sourceRect->Height, BulletVector[j]._position->X,
						BulletVector[j]._position->Y, BulletVector[j]._sourceRect->Width, BulletVector[j]._sourceRect->Height))
					{
						Vector2* dronePosVec = new Vector2(DroneVector[i]._position->X, DroneVector[i]._position->Y);
						BulletVector.erase(BulletVector.begin() + j);
						ShowExplosion(DroneVector[i]._position);
						AmmoVector.push_back(*new AmmoPickup(_ammoTexture, dronePosVec));
						DroneVector.erase(DroneVector.begin() + i);
						_player->_score += 2;
						Audio::Play(_damage);

					}
				}
			}

			//Bullet on Missle Collision
			for (int i = 0; i < MissleVector.size(); i++)
			{
				for (int j = 0; j < BulletVector.size(); j++)
				{
					if (CollisionCheck(MissleVector[i]._position->X, MissleVector[i]._position->Y, MissleVector[i]._sourceRect->Width, MissleVector[i]._sourceRect->Height, BulletVector[j]._position->X,
						BulletVector[j]._position->Y, BulletVector[j]._sourceRect->Width, BulletVector[j]._sourceRect->Height))
					{
						Vector2* missPosVec = new Vector2(MissleVector[i]._position->X, MissleVector[i]._position->Y);
						BulletVector.erase(BulletVector.begin() + j);
						ShowExplosion(MissleVector[i]._position);
						AmmoVector.push_back(*new AmmoPickup(_ammoTexture, missPosVec));
						MissleVector.erase(MissleVector.begin()+i);
						_player->_score += 3;
						Audio::Play(_damage);

					}
				}
			}

			if(!_player->isPlayerDead)
				CheckViewportCollision();

			//Set turret position.
			_player->_turretPosition->X = _player->_position->X + 16;
			_player->_turretPosition->Y = _player->_position->Y + 16;
			
			
			//Score and Spawning Enemies
			if (_player->_score < 9)
			{
				scoreBracket = 0;
			}
			else if (_player->_score >= 9 && _player->_score < 20)
			{
				scoreBracket = 1;
			}
			else if (_player->_score >= 20 && _player->_score < 50)
			{
				scoreBracket = 2;
			}
			else 
			{
				scoreBracket = 3;
			}

			switch (scoreBracket)
			{
			case 0:
				if (totalTime > 4)
				{
					SpawnEnemy();
					totalTime = 0;
				}
				break;
			case 1:
				if (totalTime > 3)
				{
					SpawnEnemy();
					totalTime = 0;
				}
				break;
			case 2:
				if (totalTime > 2)
				{
					SpawnEnemy();
					totalTime = 0;
				}
				break;
			case 3:
				if (totalTime > 1)
				{
					SpawnEnemy();
					totalTime = 0;
				}
				break;
			default:
				if (totalTime > 4)
				{
					SpawnEnemy();
					totalTime = 0;
				}
				break;
			}
		}
		break;
	}

}

void TankGame::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream Score;
	//this will be the players current score eventually.
	Score << "Score: " << _player->_score;

	std::stringstream Ammo;
	Ammo << "Ammo: " << _player->_ammo;

	SpriteBatch::BeginDraw(); // Starts Drawing

	//Draw Mines
	for (const auto& mine : MinesVector)
	{
		SpriteBatch::Draw(mine._texture, mine._position, mine._sourceRect);
	}
	
	if (!_player->isPlayerDead)
	{
		Vector2* _turretRotOffset = new Vector2(16, 24);
		SpriteBatch::Draw(_player->_texture, _player->_position, _player->_sourceRect); //Draws player.
		SpriteBatch::Draw(_player->_turretTexture, _player->_turretPosition, _player->_turretSourceRect, _turretRotOffset, 1.0f, _player->_turretRotation, Color::White, SpriteEffect::NONE); //Draws player turret.
	}

	//Draw Buildings
	for (int i = 0; i < BUILDINGS; i++)
	{
		SpriteBatch::Draw(_buildings[i]->_texture, _buildings[i]->_position, _buildings[i]->_sourceRect);
	}

	//draw ammo
	for (const auto &ammo : AmmoVector)
	{
		SpriteBatch::Draw(ammo._texture, ammo._position, ammo._sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}

	//Draw drones
	for (const auto& drone : DroneVector)
	{
		SpriteBatch::Draw(drone._texture, drone._position, drone._sourceRect);
	}

	//Draw Missles
	Vector2* _missleRotOffset = new Vector2(16, 2);
	for (const auto& missle : MissleVector)
	{
		SpriteBatch::Draw(missle._texture, missle._position, missle._sourceRect, _missleRotOffset, 1.0f, missle._orientation, Color::White, SpriteEffect::NONE);
	}

	//Draw bullets
	for (const auto &bullet: BulletVector)
	{
		Vector2* _bulletRotOffset = new Vector2(4, 4);
		SpriteBatch::Draw(bullet._texture, bullet._position, bullet._sourceRect, _bulletRotOffset,1,bullet._orientation, Color::White, SpriteEffect::NONE);
	}
	
	// Draws String
	SpriteBatch::DrawString(Score.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::DrawString(Ammo.str().c_str(), _stringPosition2, Color::Green);

	//Draw explosions
	for (const auto& boom : ExplosionVector)
	{
		SpriteBatch::Draw(boom._texture, boom._position, boom._sourceRect);
	}

	// Draws Strings
	SpriteBatch::DrawString(Score.str().c_str(), _stringPosition, Color::White);
	SpriteBatch::DrawString(Ammo.str().c_str(), _stringPosition2, Color::White);

		if (_paused && !_startGameMenu && gameState != 2)
		{
			std::stringstream menuStream;
			menuStream << "Paused";
			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::White);
		}
		else if (_paused && _startGameMenu && gameState != 2)
		{
			std::stringstream menuStream;
			menuStream << "Current High Score is " << _highScore << "\nPress Space to start.";
			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _startStringPosition, Color::White);
		}
		else if (_paused && gameState == 2)
		{
			std::stringstream menuStream;
			menuStream << "Game Over! Final score: " << _player->_score;
			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _startStringPosition, Color::White);
		}

	

	SpriteBatch::EndDraw(); // Ends Drawing
}



