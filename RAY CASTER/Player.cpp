#include "Player.h"
#include<SFML/Graphics.hpp>
#include<math.h>
#include<iostream>

#define PI 3.1415926535

// constructor defination
Player::Player(float px, float py, int mapS, int mapX, int mapY)
	:px(px), py(py), playerChar(sf::Vector2f(10.f, 10.f)), mapS(mapS), mapX(mapX), mapY(mapY)
{
	playerChar.setFillColor(sf::Color::Yellow);
	playerChar.setPosition(sf::Vector2f(px, py));
	createFace();
}

// rotate left
void Player::rotateLeft(float dt) {
	pa -= 2 * dt;
	if (pa < 0) {
		pa += 2 * PI;
	}
	pdx = cos(pa) * moveSpeed * dt;
	pdy = sin(pa) * moveSpeed * dt;
}

// rotate right
void Player::rotateRight(float dt) {
	pa += 2 * dt;
	if (pa > 2 * PI) {
		pa -= 2 * PI;
	}
	pdx = cos(pa) * moveSpeed * dt;
	pdy = sin(pa) * moveSpeed * dt;
}

// move forward
void Player::moveForwards(float dt, int* mapGrid) {
	float nextPx = px + pdx;
	float nextPy = py + pdy;

	float halfSize = 5.f;

	// Check X 
	int leftX = int((nextPx) / mapS);
	int rightX = int((nextPx + 2 * halfSize) / mapS);
	int topX = int((py) / mapS);
	int bottomX = int((py + 2 * halfSize) / mapS);

	if (mapGrid[topX * mapX + leftX] == 0 &&
		mapGrid[topX * mapX + rightX] == 0 &&
		mapGrid[bottomX * mapX + leftX] == 0 &&
		mapGrid[bottomX * mapX + rightX] == 0) {
		px = nextPx;
	}

	// Check Y 
	int leftY = int((px) / mapS);
	int rightY = int((px + 2 * halfSize) / mapS);
	int topY = int((nextPy) / mapS);
	int bottomY = int((nextPy + 2 * halfSize) / mapS);

	if (mapGrid[topY * mapX + leftY] == 0 &&
		mapGrid[topY * mapX + rightY] == 0 &&
		mapGrid[bottomY * mapX + leftY] == 0 &&
		mapGrid[bottomY * mapX + rightY] == 0) {
		py = nextPy;
	}
}

// Move Backward
void Player::moveBackwards(float dt, int* mapGrid) {
	float nextPx = px - pdx;
	float nextPy = py - pdy;

	float halfSize = 5.f;

	// Check X 
	int leftX = int((nextPx) / mapS);
	int rightX = int((nextPx + 2 * halfSize) / mapS);
	int topX = int((py) / mapS);
	int bottomX = int((py + 2 * halfSize) / mapS);

	if (mapGrid[topX * mapX + leftX] == 0 &&
		mapGrid[topX * mapX + rightX] == 0 &&
		mapGrid[bottomX * mapX + leftX] == 0 &&
		mapGrid[bottomX * mapX + rightX] == 0) {
		px = nextPx;
	}

	// Check Y 
	int leftY = int((px) / mapS);
	int rightY = int((px + 2 * halfSize) / mapS);
	int topY = int((nextPy) / mapS);
	int bottomY = int((nextPy + 2 * halfSize) / mapS);

	if (mapGrid[topY * mapX + leftY] == 0 &&
		mapGrid[topY * mapX + rightY] == 0 &&
		mapGrid[bottomY * mapX + leftY] == 0 &&
		mapGrid[bottomY * mapX + rightY] == 0) {
		py = nextPy;
	}
}

// update the position
void Player::update() {
	playerChar.setPosition(sf::Vector2f(px, py));
}


// call this before draw function to get face
// create the line (face) (tell the direction in which we are facing)
void Player::createFace() {
	sf::Vector2f center = playerChar.getPosition() + sf::Vector2f(5.f, 5.f);
	sf::Vector2f end = center + sf::Vector2f(cos(pa) * length, sin(pa) * length);
	face = new sf::Vertex[2]{
		sf::Vertex(center,sf::Color::Yellow),
		sf::Vertex(end,sf::Color::Yellow)
	};
}

// find out the smallest distance to the wall for a particular angle in out fov
sf::Vertex* Player::createPOV(const int* mapGrid,float angle) {
	
	sf::Vector2f center = playerChar.getPosition() + sf::Vector2f(5.f, 5.f);

	float rayAngle = fmod(angle, 2 * PI);
	if (rayAngle < 0) rayAngle += 2 * PI;

	float sinA = -sin(rayAngle);
	float cosA = cos(rayAngle);
	float tanA = -tan(rayAngle);

	float horizDist = INFINITY, vertDist = INFINITY;
	sf::Vector2f horizHit, vertHit;

	//--- HORIZONTAL INTERSECTION ---

	{
		float ry, rx, yo, xo;
		if (sinA > 0) {  // player is looking up so the angle is positive but the distance decrease if we go up  
			ry = floor(center.y / mapS) * mapS - 0.0001f; // floor(find tile)* (size of 1 tile) - some distance   so,it check the above tile not current tile  
			yo = -mapS; //height of 1 tile 
		}
		else { // player is looking down so angle is negative but the distance increase if we go down
			ry = floor(center.y / mapS) * mapS + mapS; // floor(find tile)* (size of 1 tile) + tile size so,it check the below tile not current tile 
			yo = mapS;   //height of 1 tile 
		}
		rx = center.x + (center.y - ry) / tanA; // calculating the x coordinates as we know that tanA=(center.y-ry)/(center.x-rx);
		xo = -yo / tanA;  // will find the x offset how much x changes in 1 tile as yo=maps/-maps so it will give how much x change and we know that next intersection 
		// point after rx,ry will be rx+xo , ry+yo  


		// checking if the line intersects with horizontal line or not if not thenn add xo and yo and if yes it interset with wall then store values in horizHit and horizDist
		for (int i = 0; i < 100; ++i) {
			int gridX = int(rx) / mapS;
			int gridY = int(ry) / mapS;
			if (gridX < 0 || gridX >= mapX || gridY < 0 || gridY >= mapY) break;
			if (mapGrid[gridY * mapX + gridX] == 1) {
				horizHit = { rx, ry };
				horizDist = hypot(rx - center.x, ry - center.y);
				break;
			}
			rx += xo;
			ry += yo;
		}
	}
	// same thing for vertical intersection just change x with y and y with x
	// --- VERTICAL INTERSECTION ---
	{
		float rx, ry, xo, yo;

		if (cosA > 0) {
			rx = floor(center.x / mapS) * mapS + mapS; // looking right
			xo = mapS;
		}
		else {
			rx = floor(center.x / mapS) * mapS - 0.0001f; // looking left
			xo = -mapS;
		}

		ry = center.y + (center.x - rx) * tanA;
		yo = -xo * tanA;

		for (int i = 0; i < 100; ++i) {
			int gridX = int(rx) / mapS;
			int gridY = int(ry) / mapS;
			if (gridX < 0 || gridX >= mapX || gridY < 0 || gridY >= mapY) break;
			if (mapGrid[gridY * mapX + gridX] == 1) {
				vertHit = { rx, ry };
				vertDist = hypot(rx - center.x, ry - center.y);
				break;
			}
			rx += xo;
			ry += yo;
		}
	}
	// finding which distance is smaller and returning line to which is smaller
	sf::Vector2f hitPoint = (horizDist < vertDist) ? horizHit : vertHit;
	sf::Vertex* line;
	if (horizDist < vertDist) {
		line = new sf::Vertex[2]{
			sf::Vertex(center, sf::Color::Red),
			sf::Vertex(hitPoint, sf::Color::Red)
		};
	}
	else {
		line = new sf::Vertex[2]{
			sf::Vertex(center, sf::Color::Blue),
			sf::Vertex(hitPoint, sf::Color::Blue)
		};
	}
	distance = (horizDist < vertDist) ? horizDist : vertDist;
	rayHit= (horizDist < vertDist) ? 1 : 0;
	return line;
}

// convert 2D grid into 3D grid
void Player::draw3D(sf::RenderWindow& window,int numRays,float angle,int numberOfRay, int WINDOW_HEIGHT, int WINDOW_WIDTH) {
	float correctDistance = distance * cos(pa - angle);

	float lineH = (mapS * 320) / (correctDistance + 0.0001);
	if (lineH > WINDOW_HEIGHT) lineH = WINDOW_HEIGHT;

	float lineOffset = ((float)WINDOW_HEIGHT / 2) - (lineH / 2);

	sf::RectangleShape wall(sf::Vector2f(WINDOW_WIDTH / float(numRays), lineH));
	wall.setPosition(sf::Vector2f(((float)WINDOW_WIDTH / 2.0) + numberOfRay * (WINDOW_WIDTH / float(numRays)), lineOffset));
	
	if(rayHit==1)wall.setFillColor(sf::Color(255 - distance, 0, 0));
	else wall.setFillColor(sf::Color(255 - distance, 15, 0));
	if(distance>=255)wall.setFillColor(sf::Color(255 - 251, 0, 0));
	window.draw(wall);
}

// main Drawing function used outside the class (encapsulates all the other function)
void Player::draw(sf::RenderWindow& window, const int* mapGrid,int rays,int WINDOW_HEIGHT, int WINDOW_WIDTH) {
	createFace();
	float increment = 1.0 / (float)rays;
	int numRays = rays * 60;
	window.draw(playerChar);
	int numberOfRay = 0;
	window.draw(face, 2, sf::PrimitiveType::Lines);
	for (float i = pa - 30; i <= pa + 30; i += increment) {
		float angle = pa + i * PI / 180.f;
		window.draw(createPOV(mapGrid,angle), 2, sf::PrimitiveType::Lines);
		draw3D(window,numRays,angle, numberOfRay,WINDOW_HEIGHT, WINDOW_WIDTH);
		numberOfRay++;
	}
}
