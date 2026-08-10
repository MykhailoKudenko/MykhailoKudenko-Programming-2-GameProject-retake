#pragma once
#include <vector>
#include "utils.h"

#include "Animation.h"




class Player final
{
public:

	enum class PlayerWeapon
	{
		Lance,
		Knife,
		Torch
	};

	explicit Player(Vector2f startPos);
	~Player() = default;
	//rule of 5
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	Player(Player&&) = delete;
	Player& operator=(Player&&) = delete;

	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, 
		const std::vector<std::vector<Vector2f>>& playerOnlyVertices,
		const std::vector<Rectf>& ladders, 
		const std::vector<std::vector<Vector2f>>& platfroms);

	Vector2f GetCenterPosition() const;
	Rectf GetHitbox() const;
	void TakeDamage();

	bool DoesWantToThrow() const;
	bool IsFacingRight() const;

	Player::PlayerWeapon GetPlayerWeapon() const;
	void SetPlayerWeapon(Player::PlayerWeapon weapon);

	int GetPlayerScore() const;
	void AddToPLayerScore(int score);
	void SetPlayerScore(int newScore);

	bool IsDeathAnimationFinished() const;

	void Respawn(const Vector2f& pos);

	void SetImmortal(bool isImmortal);

	bool IsImmortal() const;

	void SetFlying(bool isFlying);

	bool IsFlying() const;

	Vector2f GetThrowPosition() const;


private:

	enum class PlayerState
	{
		Standing,
		Walking,
		Jumping,
		Climbing,
		ClimbingStill,
		Knockback,
		Ducking,
		Throwing,
		DuckingThrow,
		KnockbackDead,
		Dead
	};

	void UpdateStates(const std::vector<Rectf>& ladders, float elapsedSec);
	void UpdateInput();
	void UpdateTimers(float elapsedSec);

	void UpdateMovementHorizontal(const std::vector<std::vector<Vector2f>>& vertices,const std::vector<std::vector<Vector2f>>& playerOnlyVertices,float elapsedSec); 
	void UpdateMovementVertical(const std::vector<std::vector<Vector2f>>& vertices,
		const std::vector<std::vector<Vector2f>>& playerOnlyVertices,
		const std::vector<std::vector<Vector2f>>& platforms,
		float elapsedSec);

	bool TryClimb(const std::vector<Rectf>& ladders, bool isGoingUp);
	bool IsStillOnLadder() const;
	void SnapToCurrentLadderCenter();
	//movement(speeds)
	float m_MovementSpeed;
	float m_JumpSpeed;
	float m_KnockBackSpeed;
	float m_ClimbSpeed;

	//input
	int m_InputDirectionX;
	int m_InputDirectionY;
	bool m_IsShootButtonPressed;
	//POSITION
	bool m_IsFacingRight;
	bool m_IsOnTheGround;
	Rectf m_Collider;

	//jumping
	float m_JumpDirectionX;
	const float m_JumpTimeUpMax;
	float m_JumpTimeUpCurrent;
	//KonkBack
	const float m_InvulnerableTimeMax;
	float m_InvulnerableTimeCurrent;
	const float	m_KnockbackTimeMax;
	float m_KnockbackTimeCurrent;
	float m_KnockBackDirectionX;
	//climbing 
	const Rectf* m_pCurrentLadder;
	bool m_BlockVerticalActionsUntilReleased;
	//Animations
	Animation m_WalkingArmour;
	Animation m_ClimbingArmour;
	Animation m_ThrowArmour;
	Animation m_DuckThrowArmour;

	const Texture* m_JumpingArmour;
	const Texture* m_DuckArmour;

	Animation m_WalkingNaked;
	Animation m_ClimbingNaked;
	Animation m_ThrowNaked;
	Animation m_DuckThrowNaked;

	const Texture* m_JumpingNaked;
	const Texture* m_DuckNaked;

	const Texture* m_Hit;

	const Texture* m_DeathKnockBack;
	const Texture* m_Death;

	//State machine
	bool m_IsWearingArmour;
	PlayerState m_Mystate;

	//Throwing
	bool m_PreviousShootPressed;
	bool m_DoesWantToThrow;

	float m_ThrowCooldownMax;
	float m_ThrowCooldownCurrent;

	//wepon
	PlayerWeapon m_MyWeapon;

	int m_Score;
	//Death
	bool m_IsImmortal;
	const float m_DeathTimerMax;
	float m_DeathTimeCurrent;
	//DEBUG fly
	bool m_IsFlying;
	float m_FlySpeed;
};

