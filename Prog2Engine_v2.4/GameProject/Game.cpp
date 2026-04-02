#include "pch.h"
#include "Game.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }, m_P1(Vector2f{50, 150})
{
	Initialize();
}

Game::~Game( )
{
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
	Cleanup( );
}

void Game::Initialize()
{
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

	m_pEntityManager = new EntityManager();

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

		"Platform.png",
		"Level1.png"
	);
	
	
	m_pEntityManager->SetLevel(m_level1);

	//m_pEntityManager->AddZombie(Vector2f{50, 250}, false);
	//m_pEntityManager->AddZombie(Vector2f{ 50, 450 }, true);

	//m_pEntityManager->AddBird(Vector2f{ 250, 50 }, false);

	//m_pEntityManager->AddFlyingKnight(Vector2f{ 350, 50 }, false);

	//m_pEntityManager->AddGhost(Vector2f{ 400, 200 }, false);
	//m_pEntityManager->AddPlant(Vector2f{ 600, 50 });

	//m_pEntityManager->AddDemon(Vector2f{ 400, 20 });
	m_pEntityManager->AddTroll(Vector2f{ 400, 40 });

	m_pEntityManager->AddDrop(Vector2f{ 500, 50 }, PickupType::Lance);
	m_pEntityManager->AddDrop(Vector2f{ 550, 50 }, PickupType::Knife);
	m_pEntityManager->AddDrop(Vector2f{ 600, 50 }, PickupType::Torch);
	m_pEntityManager->AddDrop(Vector2f{ 650, 50 }, PickupType::Doll);
	m_pEntityManager->AddDrop(Vector2f{ 700, 50 }, PickupType::MoneyBag);


}

void Game::Cleanup()
{
	delete m_pEntityManager;
	m_pEntityManager = nullptr;

	delete m_level1;
	m_level1 = nullptr;
}

void Game::Update(float elapsedSec)
{
	m_level1->Update(elapsedSec);

	m_P1.Update(
		elapsedSec,
		m_level1->GetVertecies(),
		m_level1->GetLadders(),
		m_level1->GetPlatformTopEdges()
	);

	m_pEntityManager->Update(elapsedSec, m_P1);
}

void Game::Draw() const
{
	ClearBackground();

	glPushMatrix();

	glTranslatef(-m_P1.GetCenterPosition().x + 423.f, 0, 0);

	m_level1->Draw();
	m_pEntityManager->Draw();
	m_P1.Draw();

	glPopMatrix();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
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
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

