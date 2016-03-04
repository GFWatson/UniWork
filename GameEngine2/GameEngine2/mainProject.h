#pragma once

#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

#include "GLSLProgram.h"
#include "Camera.h"
#include "GLTexture.h"
#include "Sprite.h"
#include "imageLoader.h"
#include "dataArray.h"

class mainProject
{
public:
	mainProject();
	~mainProject();
	void run();
	void readFile(dataArray* dataOne, dataArray* dataTwo, dataArray* dataThree, dataArray* dataFour, dataArray* dataAll);
	void initSystems();
	void initShaders();
	void render(std::vector<Sprite*> &list);
	void makeTrajectory(std::string player, int x, int y);
	void createHeatmaps(std::vector<Sprite*> &vec, dataArray* data, int num);
	void input(bool* h, bool* t, bool* a, bool* b, bool* c, bool* d, bool* ch, dataArray* dataOne, dataArray* dataTwo, dataArray* dataThree, dataArray* dataFour, dataArray* dataAll);
	void changeScene(bool* h, bool* t, bool* a, bool* b, bool* c, bool* d, bool* ch);
	void deleteVector(std::vector<Sprite*> &vec);
	void eraseVector(std::vector<Sprite*> &vec);
	std::vector<Sprite*> sceneList, oneT, oneH, twoT, twoH, threeT, threeH, fourT, fourH, allH;

private:
	SDL_Window* window;
	GLSLProgram colourProgram;
	GLint pLocation, mLocation;
	Camera camera;
	glm::mat4 pMatrix, mMatrix;
	GLTexture dblue, lblue, green, yellow, orange, red, playerOne, playerTwo, playerThree, playerFour, heatmap, trajectory, heatmapAndTrajectory, onePlayer, twoPlayer, threePlayer, fourPlayer, allPlayer, texture;
	int first;
	int last;
	int currentFirst;
	int currentLast;
	int oneLastX;
	int oneLastY;
	int twoLastX;
	int twoLastY;
	int threeLastX;
	int threeLastY;
	int fourLastX;
	int fourLastY;
	dataArray dataOne, dataTwo, dataThree, dataFour, dataAll;
	Sprite *mode, *plyr;
};

