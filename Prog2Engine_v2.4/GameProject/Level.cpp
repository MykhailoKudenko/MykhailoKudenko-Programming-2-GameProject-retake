#include "pch.h"
#include "Level.h"
#include "utils.h"
Level::Level(std::vector<std::vector<Vector2f>> vertices, std::vector<Rectf> ladders, const std::string& texturePath) : m_Vertices{ vertices }, m_Ladders{ ladders }, m_Texture { texturePath }
{

}

const std::vector<std::vector<Vector2f>>& Level::GetVertecies() const
{
	return(m_Vertices);
}

void Level::Draw() const
{
	

	m_Texture.Draw();

	for (const std::vector<Vector2f>& platform : m_Vertices)
	{
		for (size_t i = 0; i < platform.size() - 1; ++i)
		{
			utils::SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
			utils::DrawLine(platform[i], platform[i + 1]);
		}
	}

	for (Rectf ladder : m_Ladders)
	{
		
		utils::SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
		utils::DrawRect(ladder);
		
	}

}



const std::vector<Rectf>& Level::GetLadders() const
{
	return m_Ladders;
}