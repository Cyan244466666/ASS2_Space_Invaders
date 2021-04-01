#pragma once
#include <SDL.h>


// This enum type will be used in most classes to determine whether
// an object is active (alive), or destroyed (dead):
enum Status
{
	Alive,
	JustKilled,
	Dead
};


//This struct will serve as a Vector2:
struct Vector2
{
	float x;
	float y;
};

// This class is the overall Base class for most elements in the game.
// Most game elements will derive from this class (e.g. Ship, Aliens, 
// and Bullets:
class GameObject
{
private:
	Vector2 m_Position; // The x and y Position of the object.
	Vector2 m_Velocity; // The velocity on the x and y axes.
	Vector2 m_Scale; // Width and Height of the object.
	int m_Direction; // Direction of the object (1) right, (-1) left.
	Status m_Status; // Alive (active), Dead (dead).
	
public:
	// Constructor:
	GameObject();
	GameObject(Vector2 _position, Vector2 _velocity, Vector2 _scale, int _direction, Status _status);
	// Destructor:
	virtual ~GameObject();

	// Getter Functions:
	Vector2 GetPosition();
	Vector2 GetVelocity();
	Vector2 GetScale();
	int GetDirection();
	Status GetStatus();

	// Setter Functions:
	void SetPosition(Vector2 _position);
	void SetVelocity(Vector2 _velocity);
	void SetScale(Vector2 _scale);
	void SetDirection(int _direction);
	void SetStatus(Status _status);

	// This function takes in the position and scale of a bullet. Using this,
	// The function will check every frame of a game whether the game object
	// has collided with a bullet. If it has, return true, otherwise false.
	bool BulletCollision(Vector2 _bulletPos, Vector2 _bulletScale);

	

};