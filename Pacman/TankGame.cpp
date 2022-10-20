#include "TankGame.h"

#include <sstream>

TankGame::TankGame(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.1f)
{
	_frameCount = 0;
	_paused = true;
	_startGameMenu = true;
	_escKeyDown = false;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1280, 720, false, 25, 25, "Tank Game", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

TankGame::~TankGame()
{
	delete _playerTexture;
	delete _playerSourceRect;
	delete _ammoBlueTexture;
	delete _ammoInvertedTexture;
	delete _ammoRect;
}

void TankGame::LoadContent()
{
	// Load Player
	_playerTexture = new Texture2D();
	_playerTexture->Load("Textures/Pacman.tga", false);
	_playerPosition = new Vector2(350.0f, 350.0f);
	_playerSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Ammo
	_ammoBlueTexture = new Texture2D();
	_ammoBlueTexture->Load("Textures/Munchie.tga", true);
	_ammoInvertedTexture = new Texture2D();
	_ammoInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_ammoRect = new Rect(100.0f, 450.0f, 12, 12);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//Set menu parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_startStringPosition = new Vector2(550.0f, Graphics::GetViewportHeight() / 2.0f);
}

void TankGame::Update(int elapsedTime)
{
	//Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	
	//Checks for pause input.
	if (keyboardState->IsKeyDown(Input::Keys::ESCAPE) && !_escKeyDown && !_startGameMenu)
	{
		_escKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::ESCAPE))
		_escKeyDown = false;

	if (keyboardState->IsKeyDown(Input::Keys::SPACE) && _startGameMenu)
	{
		_startGameMenu = false;
		_paused = !_paused;
	}
	
	//For things that need to stop when paused.
	if (!_paused)
	{
		//Checks for WASD and moves player.
		if (keyboardState->IsKeyDown(Input::Keys::D))
		{
			_playerPosition->X += _cPlayerSpeed * elapsedTime;
		}	
		else if (keyboardState->IsKeyDown(Input::Keys::A))
		{
			_playerPosition->X -= _cPlayerSpeed * elapsedTime;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::S))
		{
			_playerPosition->Y += _cPlayerSpeed * elapsedTime;
		}	
		else if (keyboardState->IsKeyDown(Input::Keys::W))
		{
			_playerPosition->Y -= _cPlayerSpeed * elapsedTime;
		}
			

		//Colliding with the walls.
		if (_playerPosition->X + _playerSourceRect->Width > Graphics::GetViewportWidth())
		{
			_playerPosition->X = Graphics::GetViewportWidth() - _playerSourceRect->Width;
		}

		if (_playerPosition->X < 0)
		{
			_playerPosition->X = 0;
		}

		if (_playerPosition->Y + _playerSourceRect->Height > Graphics::GetViewportHeight())
		{
			_playerPosition->Y = Graphics::GetViewportHeight() - _playerSourceRect->Height;
		}

		if (_playerPosition->Y < 0)
		{
			_playerPosition->Y = 0;
		}

		//Frame count
		_frameCount++;
	}
}

void TankGame::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Player X: " << _playerPosition->X << " Y: " << _playerPosition->Y;
	
	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_playerTexture, _playerPosition, _playerSourceRect); // Draws player

	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_ammoInvertedTexture, _ammoRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_ammoBlueTexture, _ammoRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	
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