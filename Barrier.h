#pragma once
// This class is used to initialise the barriers that will prevent the
// player and the aliens from going out of bounds.

class Barrier
{
private:
	int m_PosX;
	int m_PosY;
	int m_ScaleX;
	int m_ScaleY;
public:
	
	// Constructor:
	Barrier() 
	{
		m_PosX = NULL;
		m_PosY = NULL;
		m_ScaleX = NULL;
		m_ScaleY = NULL;
	}
	Barrier(int posX, int posY, int scaleX, int scaleY)
	{
		m_PosX = posX;
		m_PosY = posY;
		m_ScaleX = scaleX;
		m_ScaleY = scaleY;
	}
	// Destructor:
	~Barrier() = default;

	// Getter Functions:
	int GetPosX() { return m_PosX; }
	int GetPosY() { return m_PosY; }
	int GetScaleX() { return m_ScaleX; }
	int GetScaleY() { return m_ScaleY; }

};