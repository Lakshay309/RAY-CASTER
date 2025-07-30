#pragma once

#include<SFML/Graphics.hpp>

class Player {

private:
	// corrdinates px,py and current coordinates pdx,pdy ,pa -> current angle
	float px = 300, py = 300, pdx = 0, pdy = 0, pa = 0;
	
	// some constants (length length of face line)
	float moveSpeed = 100.f, length = 50.f;
	
	// our character
	sf::RectangleShape playerChar;

	// face line
	sf::Vertex* face;
	
	// disatance to wall
	float distance;
	
	// map tile size mapx and mapy size
	int mapS, mapX, mapY;
	
	// store does the ray hit horizontal line or vertical line
	int rayHit;

	// functions

	// give the distance line to the wall (used in draw function with a loop to run it for all the angle in our FOV)
	sf::Vertex* createPOV(const int* mapGrid,float angle);
	
	// draw3D create the 3d View of the map used inside the draw function
	void draw3D(sf::RenderWindow& window,int numRays,float angle,int numberOfRay,int WINDOW_HEIGHT, int WINDOW_WIDTH);
	
	// face line creation
	void createFace();

public:
	// constructor 
	Player(float px, float py,int mapS,int mapX,int mapY);

	// update position
	void update();
	
	// movement function
	void rotateLeft(float dt);
	void rotateRight(float dt);
	void moveBackwards(float dt,int* mapGrid);
	void moveForwards(float dt, int* mapGrid);
	
	// main draw method called in main function
	void draw(sf::RenderWindow& window, const int* mapGrid,int rays, int WINDOW_HEIGHT, int WINDOW_WIDTH);
	
};

