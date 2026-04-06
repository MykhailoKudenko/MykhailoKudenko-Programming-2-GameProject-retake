#include "pch.h"
#include "Game.h"

#include <iostream>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize()
{
	Enemy::InitializeSharedAssets();
	Zombie::InitializeAssets();
	Bird::InitializeAssets();
	FlyingKnight::InitializeAssets();
	Ghost::InitializeAssets();
	Plant::InitializeAssets();

	Demon::InitializeAssets();
	Troll::InitializeAssets();

	Lance::InitializeAssets();
	Knife::InitializeAssets();
	Torch::InitializeAssets();

	PlantProjectile::InitializeAssets();
	DemonProjectile::InitializeAssets();

	Drop::InitializeAssets();
	HUD::InitializeAssets();

	Effect::InitializeAssets();

	m_P1 = new Player(Vector2f{ 50, 38 });
	m_pEntityManager = new EntityManager();
	m_hud = new HUD();
	m_pCamera = new Camera(1024.f, 960.f);

	m_MainMenu = new Texture("StartScreen.png");
	m_DeathMenu = new Texture("DeathMenu.png");

	m_level1 = nullptr;

	PLayerLivesCurrent = PlayerLivesMax;
	m_MyState = GameState::MainMenu;
}

void Game::Cleanup()
{
	Enemy::FreeSharedAssets();
	Zombie::FreeAssets();
	Bird::FreeAssets();
	FlyingKnight::FreeAssets();
	Ghost::FreeAssets();
	Plant::FreeAssets();

	Demon::FreeAssets();
	Troll::FreeAssets();

	Lance::FreeAssets();
	Knife::FreeAssets();
	Torch::FreeAssets();

	PlantProjectile::FreeAssets();
	DemonProjectile::FreeAssets();

	Drop::FreeAssets();
	HUD::FreeAssets();

	Effect::FreeAssets();

	delete m_MainMenu;
	m_MainMenu = nullptr;

	delete m_DeathMenu;
	m_DeathMenu = nullptr;

	delete m_hud;
	m_hud = nullptr;

	delete m_pEntityManager;
	m_pEntityManager = nullptr;

	delete m_level1;
	m_level1 = nullptr;

	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_P1;
	m_P1 = nullptr;
}

void Game::Update(float elapsedSec)
{
	switch (m_MyState)
	{
	case GameState::MainMenu:
		break;

	case GameState::Playing:
		// Always update player, so death animation/physics can continue
		m_P1->Update(
			elapsedSec,
			m_level1->GetVertecies(),
			m_level1->GetLadders(),
			m_level1->GetPlatformTopEdges()
		);

		
		m_level1->Update(elapsedSec);
		m_pEntityManager->Update(elapsedSec);
		m_hud->Update(elapsedSec);
		

		if (m_P1->IsDeathAnimationFinished())
		{
			PLayerLivesCurrent -= 1;

			if (PLayerLivesCurrent > 0)
			{
				m_MyState = GameState::DeathMenu;
			}
			else
			{
				m_MyState = GameState::MainMenu;
			}
		}
		break;

	case GameState::DeathMenu:
		break;
	}
}
void Game::Draw() const
{
	ClearBackground();

	switch (m_MyState)
	{
	case GameState::MainMenu:
		if (m_MainMenu != nullptr)
		{
			m_MainMenu->Draw(Vector2f{ 0.f, 0.f });
		}
		break;

	case GameState::Playing:
		m_pCamera->Aim(
			m_level1->GetWidth(), m_level1->GetHeight(), 0, 20,
			Vector2f{ m_P1->GetCenterPosition().x, 0 }, m_CameraScale
		);

		m_level1->Draw(m_DebugShowColliders);
		m_pEntityManager->Draw(m_DebugShowColliders);
		m_P1->Draw();

		m_pCamera->Reset();

		m_hud->Draw(m_P1->GetPlayerScore(), m_P1->GetPlayerWeapon());
		break;

	case GameState::DeathMenu:
		if (m_DeathMenu != nullptr)
		{
			m_DeathMenu->Draw(Vector2f{ 0.f, 0.f });
		}
		break;
	}
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	if (e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_KP_ENTER)
	{
		switch (m_MyState)
		{
		case GameState::MainMenu:
			StartNewRun();
			break;

		case GameState::Playing:
			break;

		case GameState::DeathMenu:
			StartNextLife();
			break;

		}
	}
	//debug
	switch (e.keysym.sym)
	{
	case SDLK_F1:
		m_DebugShowColliders = !m_DebugShowColliders;
		if (m_DebugShowColliders)
			std::cout << "DEBUG: COLLIDERS SHOW ON" << std::endl;
		else
			std::cout << "DEBUG: COLLIDERS SHOW OFF" << std::endl;
		break;
	case SDLK_F2:
		
		m_P1->SetImmortal(!m_P1->IsImmortal());
		if (m_P1->IsImmortal())
			std::cout << "DEBUG: IMMORTALITY ON" << std::endl;
		else
			std::cout << "DEBUG: IMMORTALITY OFF" << std::endl;
		break;
	case SDLK_F3:

		m_P1->SetFlying(!m_P1->IsFlying());
		if (m_P1->IsFlying())
			std::cout << "DEBUG: FLYING ON" << std::endl;
		else
			std::cout << "DEBUG: FLYING OFF" << std::endl;
		break;
	case SDLK_F5:
		if (m_CameraScale < 10.f)
		{
			m_CameraScale += 1.f;
			if (m_CameraScale > 10.f)
				m_CameraScale = 10.f;
		}
		std::cout << "DEBUG: CAMERA SCALE = " << m_CameraScale << " (NORMAL SCALE IS 4)" << std::endl;
		break;

	case SDLK_F6:
		if (m_CameraScale > 1.f)
		{
			m_CameraScale -= 1.f;
			if (m_CameraScale < 1.f)
				m_CameraScale = 1.f;
		}
		std::cout << "DEBUG: CAMERA SCALE = " << m_CameraScale << " (NORMAL SCALE IS 4)" << std::endl;
		break;
	}
	
	
	
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::LoadLevel1()
{
	if (m_level1 != nullptr)
	{
		delete m_level1;
		m_level1 = nullptr;
	}

	m_level1 = new Level(
		std::vector<std::vector<Vector2f>>
	{
		{
			Vector2f{ 0, 37 },
				Vector2f{ 1246, 37 },
				Vector2f{ 1246, 0 }
		},
			{
				Vector2f{ 450, 95 },
				Vector2f{ 847, 95 }
			},
			{
				Vector2f{ 1343, 0 },
				Vector2f{ 1343, 37 },
				Vector2f{ 1463, 37 },
				Vector2f{ 1463, 0 }
			},
			{
				Vector2f{ 1487, 0 },
				Vector2f{ 1487, 37 },
				Vector2f{ 1511, 37 },
				Vector2f{ 1511, 0 }
			},
			{
				Vector2f{ 1535, 0 },
				Vector2f{ 1535, 37 },
				Vector2f{ 1835, 37 },
				Vector2f{ 1835, 0 }
			},
			{
				Vector2f{ 1859, 0 },
				Vector2f{ 1859, 37 },
				Vector2f{ 2026, 37 },
				Vector2f{ 2026, 0 }
			},
			{
				Vector2f{ 2051, 0 },
				Vector2f{ 2051, 37 },
				Vector2f{ 2686, 37 },
				Vector2f{ 2686, 0 }
			},
			{
				Vector2f{ 0, 0 },
				Vector2f{ 0, 179 }
			},
			{
				Vector2f{ 2686, 0 },
				Vector2f{ 2686, 179 }
			}
	},
		std::vector<Rectf>
	{
		Rectf{ 539.f, 42.f, 11.f, 53.f },
			Rectf{ 683.f, 42.f, 11.f, 53.f },
			Rectf{ 803.f, 42.f, 11.f, 53.f }
	},
		std::vector<Level::MovingPlatform>
	{
		Level::MovingPlatform
		{
			Rectf{ 1246.f, 24.f, 32.f, 13.f },
			20.f,
			1246.f,
			1343.f
		}
	},
		std::vector<Level::EnemySpawnPoint>
	{
		{ Level::EnemyType::Plant, Vector2f{ 620.f, 95.f } },
		{ Level::EnemyType::Plant, Vector2f{ 800.f, 37.f } }
	},
		std::vector<Level::EnemySpawnArea>
	{
		{ Level::EnemyType::Zombie, Rectf{ 0, 37, 900.f, 150.f } }
	},
		std::vector<Level::DropSpawnPoint>
	{
		{ PickupType::MoneyBag, Vector2f{ 300.f, 80.f } }
	},
		"Platform.png",
		"Level1.png"
	);
}

void Game::ResetLevel()
{
	int savedScore = m_P1->GetPlayerScore();

	delete m_pEntityManager;
	m_pEntityManager = new EntityManager();

	LoadLevel1();

	m_P1->Respawn(Vector2f{ 50.f, 38.f });
	m_P1->SetPlayerScore(savedScore);

	m_pEntityManager->SetLevel(m_level1);
	m_pEntityManager->SetPlayer(m_P1);
	m_pEntityManager->SpawnPointEnemies();

	m_hud->ResetTimer();
}
void Game::StartNewRun()
{
	PLayerLivesCurrent = PlayerLivesMax;
	m_P1->SetPlayerScore(0);
	ResetLevel();
	m_MyState = GameState::Playing;
}

void Game::StartNextLife()
{
	ResetLevel();
	m_MyState = GameState::Playing;
}