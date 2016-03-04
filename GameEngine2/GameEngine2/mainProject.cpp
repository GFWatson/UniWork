#include "mainProject.h"

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

mainProject::mainProject()
{
	dataOne.init();
	dataTwo.init();
	dataThree.init();
	dataFour.init();
	dataAll.init();
	first = 1;
	last = 15000;
	currentFirst = first;
	currentLast = last;
	camera.init(500, 500);
}

mainProject::~mainProject()
{
	deleteVector(sceneList);
	deleteVector(oneH);
	deleteVector(oneT);
	deleteVector(twoH);
	deleteVector(twoT);
	deleteVector(threeH);
	deleteVector(threeT);
	deleteVector(fourH);
	deleteVector(fourT);
	deleteVector(allH);
}

void mainProject::run() {

	initSystems();

	//load in textures
	dblue = imageLoader::loadPNG("Textures/dblue.png");
	lblue = imageLoader::loadPNG("Textures/lblue.png");
	green = imageLoader::loadPNG("Textures/green.png");
	yellow = imageLoader::loadPNG("Textures/yellow.png");
	orange = imageLoader::loadPNG("Textures/orange.png");
	red = imageLoader::loadPNG("Textures/red.png");
	playerOne = imageLoader::loadPNG("Textures/playerOne.png");
	playerTwo = imageLoader::loadPNG("Textures/playerTwo.png");
	playerThree = imageLoader::loadPNG("Textures/playerThree.png");
	playerFour = imageLoader::loadPNG("Textures/playerFour.png");
	heatmap = imageLoader::loadPNG("Textures/Heatmap.png");
	trajectory = imageLoader::loadPNG("Textures/Trajectory.png");
	heatmapAndTrajectory = imageLoader::loadPNG("Textures/H&T.png");
	onePlayer = imageLoader::loadPNG("Textures/One.png");
	twoPlayer = imageLoader::loadPNG("Textures/Two.png");
	threePlayer = imageLoader::loadPNG("Textures/Three.png");
	fourPlayer = imageLoader::loadPNG("Textures/Four.png");
	allPlayer = imageLoader::loadPNG("Textures/All.png");

	//read file
	//creates trajectory maps straight from reading file
	
	readFile(&dataOne, &dataTwo, &dataThree, &dataFour, &dataAll);
	
	//create sprite batch for heatmaps
	createHeatmaps(oneH, &dataOne, (currentLast - currentFirst));
	createHeatmaps(twoH, &dataTwo, (currentLast - currentFirst));
	createHeatmaps(threeH, &dataThree, (currentLast - currentFirst));
	createHeatmaps(fourH, &dataFour, (currentLast - currentFirst));
	createHeatmaps(allH, &dataAll, ((currentLast - currentFirst) * 4));

	bool h = true; bool t = false; bool a = true; bool b = false; bool c = false; bool d = false; bool ch = false;
	
	//initially show heatmap for player one
	for each(Sprite* s in oneH) {
		sceneList.push_back(s);
	}
	//create sprites to show mode
	mode = new Sprite;
	plyr = new Sprite;
	mode->init(20.0, 400.0, 200.0, 100.0, heatmap);
	plyr->init(20.0, 300.0, 200.0, 100.0, onePlayer);
	sceneList.push_back(mode);
	sceneList.push_back(plyr);
	std::cout << "Heatmap Player 1" << std::endl << "Time: " << first << " - " << last << std::endl;

	//loop for input and draw
	while (true) {
		input(&h, &t, &a, &b, &c, &d, &ch, &dataOne, &dataTwo, &dataThree, &dataFour, &dataAll);
		changeScene(&h, &t, &a, &b, &c, &d, &ch);
		render(sceneList);
	}

}


void mainProject::initSystems()
{
	// initialise SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// create window
	window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		std::cout << "SDL Window could not be created!" << std::endl;
	}


	// create context
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) {
		std::cout << "SDL_GL context could not be created!" << std::endl;
	}


	// initialise glew
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "Could not initialise glew!" << std::endl;
	}

	//glGetString(GL_VERSION);

	// create two windows for swapping between
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// set background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//transparency for mode text
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initShaders();
}

void mainProject::initShaders() {
	colourProgram.compileShaders("Shaders/vertexShading.vert.txt", "Shaders/colourShading.frag.txt");
	colourProgram.addAttribute("vertexPosition");
	colourProgram.addAttribute("vertexColour");
	colourProgram.addAttribute("vertexUV");
	colourProgram.linkShaders();

	pLocation = colourProgram.getUniformLocation("P");
	mLocation = colourProgram.getUniformLocation("M");
}

void mainProject::render(std::vector<Sprite*> &list) {

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	colourProgram.use();
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = colourProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	for (int i = 0; i < list.size(); i++) {
		mMatrix = list[i]->getModel();
		glUniformMatrix4fv(mLocation, 1, GL_FALSE, &(mMatrix[0][0]));
		texture = list[i]->getTexture();
		glBindTexture(GL_TEXTURE_2D, texture.id);
		list[i]->draw();
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);

	colourProgram.unuse();

	SDL_GL_SwapWindow(window);
}

void mainProject::readFile(dataArray* dataOne, dataArray* dataTwo, dataArray* dataThree, dataArray* dataFour, dataArray* dataAll) {
	//read file
	std::string currentPlayer = "";
	bool setPlayer = false;
	int currentTime = 0;
	bool setTime = false;
	int currentX = 0;
	bool setX = false;
	int currentY = 0;
	bool setY = false;
	std::string currentLine = "";
	std::string currentWord = "";
	int numDelimeter = 0;
	int lastFound = 0;
	int currentNum = 0;

	std::ifstream myFile("..\\altlog.txt");
	if (myFile.is_open())
	{
		//loop through lines in file
		while (!myFile.eof())
		{
			myFile >> currentLine;
			//loop through words in line
			for each(char c in currentLine) {
				//get current word
				if (c != ':') {
					currentWord += c;
				}
				else {
					numDelimeter++;
				}
				//fill in details accordingly
				if (numDelimeter == 1 && !setPlayer)
				{
					currentPlayer = currentWord;
					currentWord = "";
					setPlayer = true;
				}
				else if (numDelimeter == 2 && !setTime) {
					currentTime = std::stoi(currentWord);
					if (currentTime > lastFound) {
						lastFound = currentTime;
					}
					currentWord = "";
					setTime = true;
				}
				else if (numDelimeter == 3 && !setX) {
					currentX = (std::stoi(currentWord) + 2000) / 4;
					currentWord = "";
					setX = true;
				}
				else if (numDelimeter == 4 && !setY) {
					currentY = (std::stoi(currentWord) + 2000) / 4;
					currentWord = "";
					setY = true;
				}
			} //end of for each
			//reset vars
			currentWord = "";
			numDelimeter = 0;
			setPlayer = false;
			setTime = false;
			setX = false;
			setY = false;
			//deal with data
			makeTrajectory(currentPlayer, currentX, currentY);

			if (currentTime >= currentFirst && currentTime <= currentLast) {
				currentX /= 10;
				currentY /= 10;

				if (currentPlayer == "P1") {
					currentNum = dataOne->getArray(currentX, currentY);
					currentNum++;
					dataOne->setArray(currentX, currentY, currentNum);
					currentNum = dataAll->getArray(currentX, currentY);
					currentNum++;
					dataAll->setArray(currentX, currentY, currentNum);
				}
				else if (currentPlayer == "P2") {
					currentNum = dataTwo->getArray(currentX, currentY);
					currentNum++;
					dataTwo->setArray(currentX, currentY, currentNum);
					currentNum = dataAll->getArray(currentX, currentY);
					currentNum++;
					dataAll->setArray(currentX, currentY, currentNum);
				}
				else if (currentPlayer == "P3") {
					currentNum = dataThree->getArray(currentX, currentY);
					currentNum++;
					dataThree->setArray(currentX, currentY, currentNum);
					currentNum = dataAll->getArray(currentX, currentY);
					currentNum++;
					dataAll->setArray(currentX, currentY, currentNum);
				}
				else if (currentPlayer == "P4") {
					currentNum = dataFour->getArray(currentX, currentY);
					currentNum++;
					dataFour->setArray(currentX, currentY, currentNum);
					currentNum = dataAll->getArray(currentX, currentY);
					currentNum++;
					dataAll->setArray(currentX, currentY, currentNum);
				}
			}
		} //end of while
		myFile.close();
		if (lastFound < last) {
			last = lastFound;
		}
		if (currentLast > last) {
			currentLast = last;
		}
	}
	else
	{
		std::cout << "Could not find file!" << std::endl;
	}
}

void mainProject::makeTrajectory(std::string player, int x, int y) {
	int curX = x / 2;
	int curY = y / 2;
	
	if (player == "P1") {
		if (oneLastX != 0 && oneLastY != 0 && (oneLastX != curX && oneLastY != curY) && ((curX - oneLastX) < 25 && (curX - oneLastX) > -25) && ((curY - oneLastY) < 25 && (curY - oneLastY) > -25)) {
			Sprite* s = new Sprite;
			s->init(oneLastX, oneLastY, (curX - oneLastX) + 2, (curY - oneLastY) + 2, playerOne);
			oneT.emplace_back(s);
		}
		oneLastX = curX;
		oneLastY = curY;
	}
	else if (player == "P2") {
		if (twoLastX != 0 && twoLastY != 0 && (twoLastX != curX && twoLastY != curY) && ((curX - twoLastX) < 25 && (curX - twoLastX) > -25) && ((curY - twoLastY) < 25 && (curY - twoLastY) > -25)) {
			Sprite* s = new Sprite;
			s->init(twoLastX, twoLastY, (curX - twoLastX) + 2, (curY - twoLastY) + 2, playerTwo);
			twoT.emplace_back(s);
		}
		twoLastX = curX;
		twoLastY = curY;
	}
	else if (player == "P3") {
		if (threeLastX != 0 && threeLastY != 0 && (threeLastX != curX && threeLastY != curY) && ((curX - threeLastX) < 25 && (curX - threeLastX) > -25) && ((curY - threeLastY) < 25 && (curY - threeLastY) > -25)) {
			Sprite* s = new Sprite;
			s->init(threeLastX, threeLastY, (curX - threeLastX) + 2, (curY - threeLastY) + 2, playerThree);
			threeT.emplace_back(s);
		}
		threeLastX = curX;
		threeLastY = curY;
	}
	else if (player == "P4") {
		if (fourLastX != 0 && fourLastY != 0 && (fourLastX != curX && fourLastY != curY) && ((curX - fourLastX) < 25 && (curX - fourLastX) > -25) && ((curY - fourLastY) < 25 && (curY - fourLastY) > -25)) {
			Sprite* s = new Sprite;
			s->init(fourLastX, fourLastY, (curX - fourLastX) + 2, (curY - fourLastY) + 2, playerFour);
			fourT.emplace_back(s);
		}
		fourLastX = curX;
		fourLastY = curY;
	}
}

void mainProject::createHeatmaps(std::vector<Sprite*> &vec, dataArray* data, int num) {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {

			float num2 = num;

			float a = (data->getArray(i, j)*100)/(num2);

			Sprite* s = new Sprite;

			if (a == 0) {
				s->init((i * 5), (j * 5), 5.0, 5.0, dblue);
			}
			else if (a > 0 && a <= 0.05) {
				s->init((i * 5), (j * 5), 5.0, 5.0, lblue);
			}
			else if (a >= 0.05 && a < 0.1) {
				s->init((i * 5), (j * 5), 5.0, 5.0, green);
			}
			else if (a >= 0.1 && a < 0.15) {
				s->init((i * 5), (j * 5), 5.0, 5.0, yellow);
			}
			else if (a >= 0.15 && a < 0.2) {
				s->init((i * 5), (j * 5), 5.0, 5.0, orange);
			}
			else{
				s->init((i * 5), (j * 5), 5.0, 5.0, red);
			}//end of else if statements

			//add to vector
			vec.emplace_back(s);
			//reset array for future use
			data->setArray(i, j, 0);

		}//end of j for loop
	}//end of i for loop
}

void mainProject::input(bool* h, bool* t, bool* a, bool* b, bool* c, bool* d, bool* ch, dataArray* dataOne, dataArray* dataTwo, dataArray* dataThree, dataArray* dataFour, dataArray* dataAll) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			//switch between heatmap and trajectory
			case SDLK_a:
				if (*h && !*t) {
					*h = false;
					*t = true;
				}
				else if (*t && !*h) {
					*h = true;
				}
				else if (*t && *h) {
					*t = false;
				}
				*ch = true;
				break;
			//switch between players
			case SDLK_d:
				if (*a && !*b && !*c && !*d) {
					*a = false;
					*b = true;
				}
				else if (!*a && *b && !*c && !*d) {
					*b = false;
					*c = true;
				}
				else if (!*a && !*b && *c && !*d) {
					*c = false;
					*d = true;
				}
				else if (!*a && !*b && !*c && *d) {
					*a = true;
					*b = true;
					*c = true;
				}
				else if (*a && *b && *c && *d) {
					*b = false;
					*c = false;
					*d = false;
				}
				*ch = true;
				break;
			//change time
			case SDLK_j:
				currentFirst -= 10;
				if (currentFirst < first) {
					currentFirst = first;
				}
				deleteVector(oneH);eraseVector(oneH);
				deleteVector(oneT);eraseVector(oneT);
				deleteVector(twoH);eraseVector(twoH);
				deleteVector(twoT);eraseVector(twoT);
				deleteVector(threeH);eraseVector(threeH);
				deleteVector(threeT); eraseVector(threeT);
				deleteVector(fourH); eraseVector(fourH);
				deleteVector(fourT); eraseVector(fourT);
				deleteVector(allH); eraseVector(allH);
				readFile(dataOne, dataTwo, dataThree, dataFour, dataAll);
				createHeatmaps(oneH, dataOne, (currentLast - currentFirst));
				createHeatmaps(twoH, dataTwo, (currentLast - currentFirst));
				createHeatmaps(threeH, dataThree, (currentLast - currentFirst));
				createHeatmaps(fourH, dataFour, (currentLast - currentFirst));
				createHeatmaps(allH, dataAll, (currentLast - currentFirst));
				*ch = true;
				break;
			case SDLK_k:
				currentFirst += 10;
				if (currentFirst > currentLast) {
					currentFirst = currentLast;
				}
				deleteVector(oneH); eraseVector(oneH);
				deleteVector(oneT); eraseVector(oneT);
				deleteVector(twoH); eraseVector(twoH);
				deleteVector(twoT); eraseVector(twoT);
				deleteVector(threeH); eraseVector(threeH);
				deleteVector(threeT); eraseVector(threeT);
				deleteVector(fourH); eraseVector(fourH);
				deleteVector(fourT); eraseVector(fourT);
				deleteVector(allH); eraseVector(allH);
				readFile(dataOne, dataTwo, dataThree, dataFour, dataAll);
				createHeatmaps(oneH, dataOne, (currentLast - currentFirst));
				createHeatmaps(twoH, dataTwo, (currentLast - currentFirst));
				createHeatmaps(threeH, dataThree, (currentLast - currentFirst));
				createHeatmaps(fourH, dataFour, (currentLast - currentFirst));
				createHeatmaps(allH, dataAll, (currentLast - currentFirst));
				*ch = true;
				break;
			case SDLK_n:
				currentLast -= 10;
				if (currentFirst < currentFirst) {
					currentFirst = currentFirst;
				}
				deleteVector(oneH); eraseVector(oneH);
				deleteVector(oneT); eraseVector(oneT);
				deleteVector(twoH); eraseVector(twoH);
				deleteVector(twoT); eraseVector(twoT);
				deleteVector(threeH); eraseVector(threeH);
				deleteVector(threeT); eraseVector(threeT);
				deleteVector(fourH); eraseVector(fourH);
				deleteVector(fourT); eraseVector(fourT);
				deleteVector(allH); eraseVector(allH);
				readFile(dataOne, dataTwo, dataThree, dataFour, dataAll);
				createHeatmaps(oneH, dataOne, (currentLast - currentFirst));
				createHeatmaps(twoH, dataTwo, (currentLast - currentFirst));
				createHeatmaps(threeH, dataThree, (currentLast - currentFirst));
				createHeatmaps(fourH, dataFour, (currentLast - currentFirst));
				createHeatmaps(allH, dataAll, (currentLast - currentFirst));
				*ch = true;
				break;
			case SDLK_m:
				currentLast += 10;
				if (currentFirst > last) {
					currentFirst = last;
				}
				deleteVector(oneH); eraseVector(oneH);
				deleteVector(oneT); eraseVector(oneT);
				deleteVector(twoH); eraseVector(twoH);
				deleteVector(twoT); eraseVector(twoT);
				deleteVector(threeH); eraseVector(threeH);
				deleteVector(threeT); eraseVector(threeT);
				deleteVector(fourH); eraseVector(fourH);
				deleteVector(fourT); eraseVector(fourT);
				deleteVector(allH); eraseVector(allH);
				readFile(dataOne, dataTwo, dataThree, dataFour, dataAll);
				createHeatmaps(oneH, dataOne, (currentLast - currentFirst));
				createHeatmaps(twoH, dataTwo, (currentLast - currentFirst));
				createHeatmaps(threeH, dataThree, (currentLast - currentFirst));
				createHeatmaps(fourH, dataFour, (currentLast - currentFirst));
				createHeatmaps(allH, dataAll, (currentLast - currentFirst));
				*ch = true;
				break;
			//camera movement
			case SDLK_w:
				camera.zoom(10.0f);
				break;
			case SDLK_s:
				camera.zoom(-10.0f);
				break;
			case SDLK_UP:
				camera.pan(10.0f, 0.0f);
				break;
			case SDLK_DOWN:
				camera.pan(-10.0f, 0.0f);
				break;
			case SDLK_RIGHT:
				camera.pan(0.0f, 10.0f);
				break;
			case SDLK_LEFT:
				camera.pan(0.0f, -10.0f);
				break;
			}//end of keydown switch
			break;
		case SDL_KEYUP:
			//not sure if i need keyup yet
			break;
		default:
			break;
		}//end of switch for keydown/up
	}//end of while
}

void mainProject::changeScene(bool* h, bool* t, bool* a, bool* b, bool* c, bool* d, bool* ch) {
	//output current mode
	std::string curMode = "";
	//only if changes have been made
	if (*ch) {
		//delete previous scene
		sceneList.erase(sceneList.begin(), sceneList.end());
		//remake scene
		if (*a && !*b && !*c && !*d) {
			if (*h && !*t) {
				for each(Sprite* s in oneH) {
					sceneList.push_back(s);
				}
				mode->changeTexture(heatmap);
				plyr->changeTexture(onePlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Heatmap Player 1";
			}
			else if (*t && !*h) {
				for each(Sprite* s in oneT) {
					sceneList.push_back(s);
				}
				mode->changeTexture(trajectory);
				plyr->changeTexture(onePlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Trajectory Player 1";
			}
			else if (*t && *h) {
				for each(Sprite* s in oneH) {
					sceneList.push_back(s);
				}
				for each(Sprite* s in oneT) {
					sceneList.push_back(s);
				}
				mode->changeTexture(heatmapAndTrajectory);
				plyr->changeTexture(onePlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Heatmap & Trajectory Player 1";
			}
		}
		else if (!*a && *b && !*c && !*d) {
			if (*h && !*t) {
				for each(Sprite* s in twoH) {
					sceneList.push_back(s);
				}
				mode->changeTexture(heatmap);
				plyr->changeTexture(twoPlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Heatmap Player 2";
			}
			else if (*t && !*h) {
				for each(Sprite* s in twoT) {
					sceneList.push_back(s);
				}
				mode->changeTexture(trajectory);
				plyr->changeTexture(twoPlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Trajectory Player 2";
			}
			else if (*t && *h) {
				for each(Sprite* s in twoH) {
					sceneList.push_back(s);
				}
				for each(Sprite* s in twoT) {
					sceneList.push_back(s);
				}
				mode->changeTexture(heatmapAndTrajectory);
				plyr->changeTexture(twoPlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Heatmap & Trajectory Player 2";
			}
		}
		else if (!*a && !*b && *c && !*d) {
			if (*h && !*t) {
				for each(Sprite* s in threeH) {
					sceneList.push_back(s);
				}
				mode->changeTexture(heatmap);
				plyr->changeTexture(threePlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Heatmap Player 3";
			}
			else if (*t && !*h) {
				for each(Sprite* s in threeT) {
					sceneList.push_back(s);
				}
				mode->changeTexture(trajectory);
				plyr->changeTexture(threePlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Trajectory Player 3";
			}
			else if (*t && *h) {
				for each(Sprite* s in threeH) {
					sceneList.push_back(s);
				}
				for each(Sprite* s in threeT) {
					sceneList.push_back(s);
				}
				mode->changeTexture(heatmapAndTrajectory);
				plyr->changeTexture(threePlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Heatmap & Trajectory Player 3";
			}
		}
		else if (!*a && !*b && !*c && *d) {
			if (*h && !*t) {
				for each(Sprite* s in fourH) {
					sceneList.push_back(s);
				}
				mode->changeTexture(heatmap);
				plyr->changeTexture(fourPlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Heatmap Player 4";
			}
			else if (*t && !*h) {
				for each(Sprite* s in fourT) {
					sceneList.push_back(s);
				}
				mode->changeTexture(trajectory);
				plyr->changeTexture(fourPlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Trajectory Player 4";
			}
			else if (*t && *h) {
				for each(Sprite* s in fourH) {
					sceneList.push_back(s);
				}
				for each(Sprite* s in fourT) {
					sceneList.push_back(s);
				}
				mode->changeTexture(heatmapAndTrajectory);
				plyr->changeTexture(fourPlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Heatmap & Trajectory Player 4";
			}
		}
		else if (*a && *b && *c && *d) {
			if (*h && !*t) {
				for each(Sprite* s in allH) {
					sceneList.push_back(s);
				}
				mode->changeTexture(heatmap);
				plyr->changeTexture(allPlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Heatmap All Players";
			}
			else if (*t && !*h) {
				for each(Sprite* s in oneT) {
					sceneList.push_back(s);
				}
				for each(Sprite* s in twoT) {
					sceneList.push_back(s);
				}
				for each(Sprite* s in threeT) {
					sceneList.push_back(s);
				}
				for each(Sprite* s in fourT) {
					sceneList.push_back(s);
				}
				mode->changeTexture(trajectory);
				plyr->changeTexture(allPlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Heatmap & Trajectory All Players";
			}
			else if (*t && *h) {
				for each(Sprite* s in allH) {
					sceneList.push_back(s);
				}
				for each(Sprite* s in oneT) {
					sceneList.push_back(s);
				}
				for each(Sprite* s in twoT) {
					sceneList.push_back(s);
				}
				for each(Sprite* s in threeT) {
					sceneList.push_back(s);
				}
				for each(Sprite* s in fourT) {
					sceneList.push_back(s);
				}
				mode->changeTexture(heatmapAndTrajectory);
				plyr->changeTexture(allPlayer);
				sceneList.push_back(mode);
				sceneList.push_back(plyr);
				curMode = "Heatmap & Trajectory All Players";
			}
		}
		*ch = false;
		std::cout << curMode << std::endl;
		std::cout << "Time: " << currentFirst << " - " << currentLast << std::endl;
	}
}

void mainProject::deleteVector(std::vector<Sprite*> &vec) {
	for (int i = 0; i < vec.size(); i++) {
		delete vec[i];
	}
}

void mainProject::eraseVector(std::vector<Sprite*> &vec) {
	for (int i = 0; i < vec.size(); i++) {
		vec.erase(vec.begin(), vec.end());
	}
}