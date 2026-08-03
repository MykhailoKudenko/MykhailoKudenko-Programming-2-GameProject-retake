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

	m_MainMenu = TextureManager::GetInstance().GetTexture("StartScreen.png");
	m_DeathMenu = TextureManager::GetInstance().GetTexture("DeathMenu.png");

	m_level1 = nullptr;

	m_PLayerLivesCurrent = m_PlayerLivesMax;
	m_MyState = GameState::MainMenu;

	m_SoundManager->SetMusicVolume(20);
	m_SoundManager->SetEffectVolume(20);
}

void Game::Cleanup()
{
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
				m_SoundManager->PlayEffect(SoundManager::SFX::Death);

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
	//	break;

}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	
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
	m_level1 = new Level("Level1Data.txt");
}

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