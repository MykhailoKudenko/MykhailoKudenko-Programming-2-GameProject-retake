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

	m_P1 = new Player(Vector2f{ 50.f, 38.f });
	m_pEntityManager = new EntityManager();
	m_SoundManager = new SoundManager();
	m_pEntityManager->SetSoundManager(m_SoundManager);
	m_hud = new HUD();
	m_pCamera = new Camera(1024.f, 960.f);

	m_MainMenu = new Texture("StartScreen.png");
	m_DeathMenu = new Texture("DeathMenu.png");

	m_level1 = nullptr;

	m_PLayerLivesCurrent = m_PlayerLivesMax;
	m_MyState = GameState::MainMenu;

	m_SoundManager->SetMusicVolume(20);
	m_SoundManager->SetEffectVolume(20);
}

void Game::Cleanup()
{

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

	m_SoundManager->StopMusic();

	delete m_SoundManager;
	m_SoundManager = nullptr;


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
			m_level1->GetVertices(),
			m_level1->GetPlayerOnlyVertices(),
			m_level1->GetLadders(),
			m_level1->GetPlatformTopEdges()
		);

		
		m_level1->Update(elapsedSec);
		m_pEntityManager->Update(elapsedSec);
		m_hud->Update(elapsedSec);
		
		if (m_hud->DidTimerFinish())
		{
			m_SoundManager->StopMusic();
			m_MyState = GameState::MainMenu;
			m_hud->ResetTimer();
		}

		if (m_P1->IsDeathAnimationFinished() || m_P1->GetCenterPosition().y < 0)
		{
			m_PLayerLivesCurrent -= 1;

			m_SoundManager->StopMusic();

			if (m_PLayerLivesCurrent > 0)
			{
				m_MyState = GameState::DeathMenu;
				m_SoundManager->PlayEffect(SFX::Death);

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
			glPushMatrix();
			glScalef(4, 4, 1.0f);
			m_MainMenu->Draw(Vector2f{ 0.f, 0.f });
			glPopMatrix();

		}
		break;

	case GameState::Playing:
		m_pCamera->Aim(
			m_level1->GetWidth(), m_level1->GetHeight(), 0, 20,
			Vector2f{ m_P1->GetCenterPosition().x, 0 }, (float)m_CameraScale
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
			glPushMatrix();
			glScalef(4, 4, 1.0f);
			m_DeathMenu->Draw(Vector2f{ 0.f, 0.f });
			glPopMatrix();
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
			m_SoundManager->PlayMusic();
			break;

		case GameState::Playing:
			break;

		case GameState::DeathMenu:
			m_SoundManager->PlayMusic();
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
		{
			std::cout << "DEBUG: COLLIDERS SHOW ON" << std::endl;

		}
		else
		{
			std::cout << "DEBUG: COLLIDERS SHOW OFF" << std::endl;

		}
		break;
	case SDLK_F2:
		
		m_P1->SetImmortal(!m_P1->IsImmortal());
		if (m_P1->IsImmortal())
		{
			std::cout << "DEBUG: IMMORTALITY ON" << std::endl;
		}
		else
		{
			std::cout << "DEBUG: IMMORTALITY OFF" << std::endl;
		}
		break;
	case SDLK_F3:

		m_P1->SetFlying(!m_P1->IsFlying());
		if (m_P1->IsFlying())
		{
			std::cout << "DEBUG: FLYING ON" << std::endl;

		}
		else
		{
			std::cout << "DEBUG: FLYING OFF" << std::endl;

		}
		break;
	case SDLK_F5:
		if (m_CameraScale < 10)
		{
			m_CameraScale += 1;
			if (m_CameraScale > 10)
				m_CameraScale = 10;
		}
		std::cout << "DEBUG: CAMERA SCALE = " << m_CameraScale << " (NORMAL SCALE IS 4)" << std::endl;
		break;

	case SDLK_F6:
		if (m_CameraScale > 1)
		{
			m_CameraScale -= 1;
			if (m_CameraScale < 1)
				m_CameraScale = 1;
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
	// make it readabl for a file svg (game technique)
	m_level1 = new Level(
		"Level1.svg"
	, std::vector<std::vector<Vector2f>>
	{
	{
		Vector2f{ 0 + 46, 0 + 49 },
		Vector2f{ 0 + 46, 49 + 15 },
		Vector2f{ 46 + 15, 49 + 15 },
		Vector2f{ 46 + 15, 0 + 49 },
	},
	{
		Vector2f{ 0 + 238, 0 + 49 },
		Vector2f{ 0 + 238, 49 + 15 },
		Vector2f{ 238 + 15, 49 + 15 },
		Vector2f{ 238 + 15, 0 + 49 },
	},
	{
		Vector2f{ 0 + 414, 0 + 49 },
		Vector2f{ 0 + 414, 49 + 15 },
		Vector2f{ 414 + 15, 49 + 15 },
		Vector2f{ 414 + 15, 0 + 49 },
	},
	{
		Vector2f{ 0 + 526, 0 + 49 },
		Vector2f{ 0 + 526, 49 + 15 },
		Vector2f{ 526 + 15, 49 + 15 },
		Vector2f{ 526 + 15, 0 + 49 },
	},
	{
		Vector2f{ 0 + 750, 0 + 49 },
		Vector2f{ 0 + 750, 49 + 15 },
		Vector2f{ 750 + 15, 49 + 15 },
		Vector2f{ 750 + 15, 0 + 49 },
	},
	{
		Vector2f{ 0 + 958, 0 + 49 },
		Vector2f{ 0 + 958, 49 + 15 },
		Vector2f{ 958 + 15, 49 + 15 },
		Vector2f{ 958 + 15, 0 + 49 },
	},
	{
		Vector2f{ 0 + 1102, 0 + 49 },
		Vector2f{ 0 + 1102, 49 + 15 },
		Vector2f{ 1102 + 15, 49 + 15 },
		Vector2f{ 1102 + 15, 0 + 49 },
	},
	{
		Vector2f{ 0 + 1262, 0 + 49 },
		Vector2f{ 0 + 1262, 49 + 15 },
		Vector2f{ 1262 + 15, 49 + 15 },
		Vector2f{ 1262 + 15, 0 + 49 },
	},
	{
		Vector2f{ 0 + 1518, 0 + 49 },
		Vector2f{ 0 + 1518, 49 + 15 },
		Vector2f{ 1518 + 15, 49 + 15 },
		Vector2f{ 1518 + 15, 0 + 49 },
	},

	{
		Vector2f{ 0 + 766, 0 + 127 },
		Vector2f{ 0 + 766, 127 + 15 },
		Vector2f{ 766 + 15, 127 + 15 },
		Vector2f{ 766 + 15, 0 + 127 },
	},
	{
		Vector2f{ 0 + 862, 0 + 127 },
		Vector2f{ 0 + 862, 127 + 15 },
		Vector2f{ 862 + 15, 127 + 15 },
		Vector2f{ 862 + 15, 0 + 127 },
	},
	{
		Vector2f{ 0 + 958, 0 + 127 },
		Vector2f{ 0 + 958, 127 + 15 },
		Vector2f{ 958 + 15, 127 + 15 },
		Vector2f{ 958 + 15, 0 + 127 },
	}
	},
		std::vector<Rectf>
	{
		Rectf{ 719.f, 50.f, 16.f, 80.f },
		Rectf{ 911.f, 50.f, 16.f, 80.f },
		Rectf{ 1066.f, 50.f, 16.f, 80.f }
	},
		std::vector<Level::MovingPlatform>
	{
		Level::MovingPlatform
		{
			Rectf{ 1664, 24.f, 32.f, 13.f },
			20.f,
			1664.f,
			1789.f
		}
	},
		std::vector<Level::EnemySpawnPoint>
	{
		{ Level::EnemyType::Plant, Vector2f{ 798.f, 128.f } },
		{ Level::EnemyType::Plant, Vector2f{ 1102.f, 128.f } },
		{ Level::EnemyType::Plant, Vector2f{ 2351.f, 49.f } },
		{ Level::EnemyType::Plant, Vector2f{ 2777.f, 49.f } },

		{ Level::EnemyType::Bird, Vector2f{ 752.f, 64.f } },
		{ Level::EnemyType::Bird, Vector2f{ 1103.f, 64.f } },
		{ Level::EnemyType::Bird, Vector2f{ 1268.f, 62.f } },
		{ Level::EnemyType::Bird, Vector2f{ 1520.f, 64.f } },

		{ Level::EnemyType::Demon, Vector2f{ 1500, 49.f } },

		{ Level::EnemyType::FlyingKnight, Vector2f{ 2050.f, 81.f } },
		{ Level::EnemyType::FlyingKnight, Vector2f{ 2080.f, 111.f } },
		{ Level::EnemyType::FlyingKnight, Vector2f{ 2110.f, 81.f } },

		{ Level::EnemyType::FlyingKnight, Vector2f{ 2200.f, 81.f } },
		{ Level::EnemyType::FlyingKnight, Vector2f{ 2230.f, 111.f } },
		{ Level::EnemyType::FlyingKnight, Vector2f{ 2260.f, 81.f } },

		{ Level::EnemyType::Troll, Vector2f{ 3424.f, 49.f } }
	},
		std::vector<Level::EnemySpawnArea>
	{
		{ Level::EnemyType::Zombie, Rectf{ 0, 49.f, 1200.f, 150.f }, true, 2.f },
		{ Level::EnemyType::Ghost, Rectf{ 2670, 49.f+5.f, 300.f, 80.f }, false , 4.f}
	},
		std::vector<Level::DropSpawnPoint>
	{
		{ PickupType::MoneyBag, Vector2f{ 304.f, 49.f } },
		{ PickupType::MoneyBag, Vector2f{ 609.f, 49.f } },
		{ PickupType::MoneyBag, Vector2f{ 921.f, 128.f } },
		{ PickupType::MoneyBag, Vector2f{ 1466.f, 49.f } },
		{ PickupType::MoneyBag, Vector2f{ 2382.f, 49.f } },
		{ PickupType::MoneyBag, Vector2f{ 2436.f, 49.f } },
		{ PickupType::MoneyBag, Vector2f{ 2940.f, 49.f } },

	},
		"Platform.png",
		"Level1.png"
	);
}


//void Game::LoadLevel1()
//{
//	if (m_level1 != nullptr)
//	{
//		delete m_level1;
//		m_level1 = nullptr;
//	}
//	// make it readabl for a file svg (game technique)
//	m_level1 = new Level(
//		std::vector<std::vector<Vector2f>>
//	{
//		{
//			Vector2f{ 0, 49 },
//				Vector2f{ 1661, 49 },
//				Vector2f{ 1661, 0 }
//		},
//			{
//				Vector2f{ 600, 127 },
//				Vector2f{ 1120, 127 }
//			},
//			{
//				Vector2f{ 1790, 0 },
//				Vector2f{ 1790, 49},
//				Vector2f{ 1949, 49},
//				Vector2f{ 1949, 0 }
//			},
//			{
//				Vector2f{ 1982, 0 },
//				Vector2f{ 1982, 49},
//				Vector2f{ 2013, 49},
//				Vector2f{ 2013, 0 }
//			},
//			{
//				Vector2f{ 2046, 0 },
//				Vector2f{ 2046, 49},
//				Vector2f{ 2445, 49},
//				Vector2f{ 2445, 0 }
//			},
//			{
//				Vector2f{ 2478, 0 },
//				Vector2f{ 2478, 49},
//				Vector2f{ 2701, 49},
//				Vector2f{ 2701, 0 }
//			},
//			{
//				Vector2f{ 2734, 0 },
//				Vector2f{ 2734, 49},
//				Vector2f{ 3582, 49},
//				Vector2f{ 3582, 0 }
//			},
//			{
//				Vector2f{ 0, 0 },
//				Vector2f{ 0, 240 }
//			},
//			{
//				Vector2f{ 3582, 0 },
//				Vector2f{ 3582, 240 }
//			}
//	}, std::vector<std::vector<Vector2f>>
//	{
//	{
//		Vector2f{ 0 + 46, 0 + 49 },
//		Vector2f{ 0 + 46, 49 + 15 },
//		Vector2f{ 46 + 15, 49 + 15 },
//		Vector2f{ 46 + 15, 0 + 49 },
//	},
//	{
//		Vector2f{ 0 + 238, 0 + 49 },
//		Vector2f{ 0 + 238, 49 + 15 },
//		Vector2f{ 238 + 15, 49 + 15 },
//		Vector2f{ 238 + 15, 0 + 49 },
//	},
//	{
//		Vector2f{ 0 + 414, 0 + 49 },
//		Vector2f{ 0 + 414, 49 + 15 },
//		Vector2f{ 414 + 15, 49 + 15 },
//		Vector2f{ 414 + 15, 0 + 49 },
//	},
//	{
//		Vector2f{ 0 + 526, 0 + 49 },
//		Vector2f{ 0 + 526, 49 + 15 },
//		Vector2f{ 526 + 15, 49 + 15 },
//		Vector2f{ 526 + 15, 0 + 49 },
//	},
//	{
//		Vector2f{ 0 + 750, 0 + 49 },
//		Vector2f{ 0 + 750, 49 + 15 },
//		Vector2f{ 750 + 15, 49 + 15 },
//		Vector2f{ 750 + 15, 0 + 49 },
//	},
//	{
//		Vector2f{ 0 + 958, 0 + 49 },
//		Vector2f{ 0 + 958, 49 + 15 },
//		Vector2f{ 958 + 15, 49 + 15 },
//		Vector2f{ 958 + 15, 0 + 49 },
//	},
//	{
//		Vector2f{ 0 + 1102, 0 + 49 },
//		Vector2f{ 0 + 1102, 49 + 15 },
//		Vector2f{ 1102 + 15, 49 + 15 },
//		Vector2f{ 1102 + 15, 0 + 49 },
//	},
//	{
//		Vector2f{ 0 + 1262, 0 + 49 },
//		Vector2f{ 0 + 1262, 49 + 15 },
//		Vector2f{ 1262 + 15, 49 + 15 },
//		Vector2f{ 1262 + 15, 0 + 49 },
//	},
//	{
//		Vector2f{ 0 + 1518, 0 + 49 },
//		Vector2f{ 0 + 1518, 49 + 15 },
//		Vector2f{ 1518 + 15, 49 + 15 },
//		Vector2f{ 1518 + 15, 0 + 49 },
//	},
//
//	{
//		Vector2f{ 0 + 766, 0 + 127 },
//		Vector2f{ 0 + 766, 127 + 15 },
//		Vector2f{ 766 + 15, 127 + 15 },
//		Vector2f{ 766 + 15, 0 + 127 },
//	},
//	{
//		Vector2f{ 0 + 862, 0 + 127 },
//		Vector2f{ 0 + 862, 127 + 15 },
//		Vector2f{ 862 + 15, 127 + 15 },
//		Vector2f{ 862 + 15, 0 + 127 },
//	},
//	{
//		Vector2f{ 0 + 958, 0 + 127 },
//		Vector2f{ 0 + 958, 127 + 15 },
//		Vector2f{ 958 + 15, 127 + 15 },
//		Vector2f{ 958 + 15, 0 + 127 },
//	}
//	},
//		std::vector<Rectf>
//	{
//		Rectf{ 719.f, 50.f, 16.f, 80.f },
//		Rectf{ 911.f, 50.f, 16.f, 80.f },
//		Rectf{ 1071.f, 50.f, 16.f, 80.f }
//	},
//		std::vector<Level::MovingPlatform>
//	{
//		Level::MovingPlatform
//		{
//			Rectf{ 1664, 24.f, 32.f, 13.f },
//			20.f,
//			1664.f,
//			1789.f
//		}
//	},
//		std::vector<Level::EnemySpawnPoint>
//	{
//		{ Level::EnemyType::Plant, Vector2f{ 798.f, 128.f } },
//		{ Level::EnemyType::Plant, Vector2f{ 1102.f, 128.f } },
//		{ Level::EnemyType::Plant, Vector2f{ 2351.f, 49.f } },
//		{ Level::EnemyType::Plant, Vector2f{ 2777.f, 49.f } },
//
//		{ Level::EnemyType::Bird, Vector2f{ 752.f, 64.f } },
//		{ Level::EnemyType::Bird, Vector2f{ 1103.f, 64.f } },
//		{ Level::EnemyType::Bird, Vector2f{ 1268.f, 62.f } },
//		{ Level::EnemyType::Bird, Vector2f{ 1520.f, 64.f } },
//
//		{ Level::EnemyType::Demon, Vector2f{ 1500, 49.f } },
//
//		{ Level::EnemyType::FlyingKnight, Vector2f{ 2050.f, 81.f } },
//		{ Level::EnemyType::FlyingKnight, Vector2f{ 2080.f, 111.f } },
//		{ Level::EnemyType::FlyingKnight, Vector2f{ 2110.f, 81.f } },
//
//		{ Level::EnemyType::FlyingKnight, Vector2f{ 2200.f, 81.f } },
//		{ Level::EnemyType::FlyingKnight, Vector2f{ 2230.f, 111.f } },
//		{ Level::EnemyType::FlyingKnight, Vector2f{ 2260.f, 81.f } },
//
//		{ Level::EnemyType::Troll, Vector2f{ 3424.f, 49.f } }
//	},
//		std::vector<Level::EnemySpawnArea>
//	{
//		{ Level::EnemyType::Zombie, Rectf{ 0, 49.f, 1200.f, 150.f } },
//		{ Level::EnemyType::Ghost, Rectf{ 2670, 49.f, 480.f, 150.f }, false }
//	},
//		std::vector<Level::DropSpawnPoint>
//	{
//		{ PickupType::MoneyBag, Vector2f{ 304.f, 49.f } },
//		{ PickupType::MoneyBag, Vector2f{ 609.f, 49.f } },
//		{ PickupType::MoneyBag, Vector2f{ 921.f, 128.f } },
//		{ PickupType::MoneyBag, Vector2f{ 1466.f, 49.f } },
//		{ PickupType::MoneyBag, Vector2f{ 2382.f, 49.f } },
//		{ PickupType::MoneyBag, Vector2f{ 2436.f, 49.f } },
//		{ PickupType::MoneyBag, Vector2f{ 2940.f, 49.f } },
//
//	},
//		"Platform.png",
//		"Level1.png"
//	);
//}

void Game::ResetLevel()
{
	int savedScore = m_P1->GetPlayerScore();

	delete m_pEntityManager;
	m_pEntityManager = new EntityManager();

	LoadLevel1();

	m_P1->Respawn(Vector2f{ 150.f, 38.f });
	m_P1->SetPlayerScore(savedScore);

	m_hud->ResetTimer();

	m_pEntityManager->SetLevel(m_level1);
	m_pEntityManager->SetPlayer(m_P1);
	m_pEntityManager->SpawnPointEnemies();
	m_pEntityManager->SetSoundManager(m_SoundManager);

	
}
void Game::StartNewRun()
{
	m_PLayerLivesCurrent = m_PlayerLivesMax;
	m_P1->SetPlayerScore(0);
	ResetLevel();
	m_MyState = GameState::Playing;
	m_hud->ResetTimer();
}

void Game::StartNextLife()
{
	ResetLevel();
	m_MyState = GameState::Playing;
}