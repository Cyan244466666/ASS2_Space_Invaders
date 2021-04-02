#pragma once
#include <SDL.h>
#include "GameObject.h"
#include "Game.h"
// This class contains the bullets of which can
// be shot by both aliens and the player.
enum bulletType
{
	SquidBullet,
	OctopusBullet,
	CrabBullet,
	
};

class Bullet : public GameObject
{
private:
	float m_Speed; // How fast the bullet will travel on the y-axis.
	int m_HurtBoxWidth; // Width of the hurtbox.
	int m_HurtBoxHeight; // Height of hurtbox.
	bool m_HasHit;

public:
	// Constructor:
	Bullet()
	{
		Vector2 scale{ 4, 12 };
		SetScale(scale);

		
		m_HurtBoxWidth = 6;
		m_HurtBoxHeight = 4;
		m_Speed = 10.0f;
		m_HasHit = false;
	}

	// Destructor:
	~Bullet() override = default;

	// Getters:
	int GetHurtBoxW() { return m_HurtBoxWidth; }
	int GetHurtBoxH() { return m_HurtBoxHeight; }
	float GetSpeed() { return m_Speed; }
	bool GetHasHit() { return m_HasHit; }

	//Setters:
	void SetHurtBoxW(int hurtBoxW) { m_HurtBoxWidth = hurtBoxW; }
	void SetHurtBoxH(int hurtBoxH) { m_HurtBoxHeight = hurtBoxH; }
	void SetSpeed(float speed) { m_Speed = speed; }
	void SetHasHit(bool hasHit) { m_HasHit = hasHit; }


	// Move Bullet:
	void MoveBullet()
	{
		Vector2 updatePos = this->GetPosition();
		updatePos.y -= m_Speed;
		this->SetPosition(updatePos);
	}

	void MoveAlienBullet()
	{
		Vector2 updatePos = this->GetPosition();
		updatePos.y += m_Speed;
		this->SetPosition(updatePos);
	}
};