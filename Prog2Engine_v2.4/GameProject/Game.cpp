#include "pch.h"
#include "Game.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }, m_P1(Rectf{50, 150, 20, 20})
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{


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
		"Level1.png"
	);
	//2026 2051

	m_Zombies.emplace_back(Rectf{ 50, 250, 20, 30 }, false);
	m_Zombies.emplace_back(Rectf{ 50, 450, 20, 30 }, true);

	for (Zombie& zomb : m_Zombies)
	{
		zomb.SetWorld(&m_level1->GetVertecies());
	}
}

void Game::Cleanup( )
{
	delete m_level1;
}

void Game::Update( float elapsedSec )
{
	m_P1.Update(elapsedSec, m_level1->GetVertecies(), m_level1->GetLadders());


	for (Projectile& Proj : m_Projectiles)
	{
		Proj.Update(elapsedSec);
	}

	for (Zombie& zomb : m_Zombies)
	{
		zomb.Update(elapsedSec);
		if (utils::IsOverlapping(zomb.GetHitbox(), m_P1.GetHitbox()))
			m_P1.TakeDamage();

		for (Projectile& Proj : m_Projectiles)
		{
			if (utils::IsOverlapping(zomb.GetHitbox(), Proj.GetHitbox()))
			{
				zomb.Kill();
				Proj.Kill();
			}
		}
	}
	if (m_P1.DoesWantToThrow())
	{
		m_Projectiles.emplace_back(Lance(m_P1.GetCenterPosition(), m_P1.IsFacingRight()));
	}
	//clean up
	for (int i = 0; i < m_Zombies.size(); ++i)
	{
		if (m_Zombies[i].isDead())
		{
			m_Zombies.erase(m_Zombies.begin() + i);
			--i;
		}
	}

	for (int i = 0; i < m_Projectiles.size(); ++i)
	{
		if (m_Projectiles[i].isDead())
		{
			m_Projectiles.erase(m_Projectiles.begin() + i);
			--i;
		}
	}

}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();

	//glTranslatef(-m_P1.GetCenterPosition().x, -m_P1.GetCenterPosition().y, 0);
	glTranslatef(-m_P1.GetCenterPosition().x + 423.f, 0, 0);
	//glScalef(4, 4, 1);
	m_level1->Draw();

	for (const Zombie& zomb : m_Zombies)
	{
		zomb.Draw();
	}
	for (const Projectile& Proj : m_Projectiles)
	{
		Proj.Draw();
	}



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

