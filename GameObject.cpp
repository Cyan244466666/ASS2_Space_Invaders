#include <SDL.h>
#include "GameObject.h"

// Constructors:
GameObject::GameObject()
{
	m_Position = { 0, 0 };
	m_Velocity = { 0, 0 };
	m_Scale = { 0, 0 };
	m_Direction = 0;
	m_Status = Alive;
}
GameObject::GameObject(Vector2 _position, Vector2 _velocity, Vector2 _scale, int _direction, Status _status)
{
	m_Position = _position;
	m_Velocity = _velocity;
	m_Scale = _scale;
	m_Direction = _direction;
	m_Status = _status;
}

// Destructor:
GameObject::~GameObject(){}

// Getter Functions:
Vector2 GameObject::GetPosition() { return m_Position; }
Vector2 GameObject::GetVelocity() { return m_Velocity; }
Vector2 GameObject::GetScale() { return m_Scale; }
int GameObject::GetDirection() { return m_Direction; }
Status GameObject::GetStatus() { return m_Status; }

// Setter Functions:
void GameObject::SetPosition(Vector2 _position) { m_Position = _position; }
void GameObject::SetVelocity(Vector2 _velocity) { m_Velocity = _velocity; }
void GameObject::SetScale(Vector2 _scale) { m_Scale = _scale; }
void GameObject::SetDirection(int _direction) { m_Direction = _direction; }
void GameObject::SetStatus(Status _status) { m_Status = _status; }


// This function takes in the position and scale of a bullet. Using this,
// The function will check every frame of a game whether the game object
// has collided with a bullet. If it has, return true, otherwise false.
bool GameObject::BulletCollision(Vector2 _bulletPos, Vector2 _bulletScale)
{ 
	float leftA, leftB;
	float rightA, rightB;
	float topA, topB;
	float bottomA, bottomB;

	// Step 1: Calculate the sides of Rect 1:
	leftA = m_Position.x;
	rightA = m_Position.x + m_Scale.x; // x + width
	topA = m_Position.y;
	bottomA = m_Position.y + m_Scale.y; // y + heigh

	// Step 2: Calculate the sides of Rect 2:
	leftB = _bulletPos.x;
	rightB = _bulletPos.x + _bulletScale.x; // x + width
	topB = _bulletPos.y;
	bottomB = _bulletPos.y + _bulletScale.y; // y + heigh

	// Compare the sides of both Rects - if one of them is true, there is no intersection:
	if (bottomA <= topB) { return false; }
	if (topA >= bottomB) { return false; }
	if (rightA <= leftB) { return false; }
	if (leftA >= rightB) { return false; }

	// If none of there are true, then there is a collision.
	return true; 
}

