#include <iostream>
#include <string>
#include "SFML/Graphics.hpp"
#include "conio.h"
#include <vector>
#include <cmath>
#include "Piece.h"

using namespace std;

bool playerTurn = 0; //red = 0, black = 1
bool endTurn = false;

//vector<Piece*> pieces;
void loadBoard(sf::Sprite& red, sf::Sprite& black, sf::Sprite& kingSymbol, sf::RenderWindow& window, vector<int>& boardData, vector<int>& kingIndices);
void drawCheckerinHand(sf::Sprite& red, sf::Sprite& black, sf::Sprite& KingSymbol, sf::RenderWindow& window, int handChecker, bool handKing);
void putCheckerinHand(vector<int>& boardData, vector<int>& kingIndicies, int& handChecker, bool& handKing, int& originalIndex, bool);
void placeChecker(vector<int>& boardData, int& handChecker,bool& handKing, int& originalIndex, bool&, vector<int>&);
//vector<int> GetAvailableMoves(vector<int>& boardData, int& originalIndex, bool redTurn);
void GetAvailableMoves(vector<int>& boardData, vector<int>& moves, int originalIndex, bool redTurn, int step, int dir, bool handKing);
bool additionalMove(int index, vector<int>& moves, int originalIndex, bool redTurn);

void checkKing(int index, bool redTurn, vector<int>& kingIndices);

bool winScenario(vector<int>& boardData, bool& redWin);
void drawWinScreen(sf::Sprite& redText, sf::Sprite& blackText, sf::Sprite& winText, sf::RenderWindow& window, bool redWin);

int main()
{

	int handChecker = 0;
	bool handKing = false;
	int originalIndex = -1;
	bool redTurn = true;
	bool redWin = false;
	bool endGame = false;
	vector<int> boardData = { 0,1,0,1,0,1,0,1, //1 black, 2 red
						      1,0,1,0,1,0,1,0,
							  0,1,0,1,0,1,0,1,
							  0,0,0,0,0,0,0,0,
							  0,0,0,0,0,0,0,0,
							  2,0,2,0,2,0,2,0,
							  0,2,0,1,0,1,0,2,
							  2,0,2,0,0,0,0,0 };
	/*					      0,1,0,1,0,1,0,1, //test board for reference
						      1,0,0,0,1,0,1,0,
							  0,1,0,1,0,1,0,1,
							  2,0,0,0,0,0,0,0,
							  0,0,0,1,0,0,0,0,
							  2,0,2,0,2,0,2,0,
							  0,2,0,2,0,2,0,2,
							  2,0,2,0,2,0,2,0*/
	vector<int> kingIndices(64);

	sf::RenderWindow window(sf::VideoMode(1024, 1024), "Checkers"); //Window object

	sf::Texture boardTexture;
	boardTexture.loadFromFile("CheckerBoard.png", sf::IntRect(0, 0, 16, 16)); //Gets the board Texture

	sf::Sprite board; //Board is Sprite
	board.setTexture(boardTexture);
	board.setScale(64, 64);

	/////////////////////INITIALIZE RED PIECE SPRITE AND TEXTURE///////////////////
	sf::Texture redPiece;
	redPiece.loadFromFile("CheckerPieceRed.png", sf::IntRect(0, 0, 16, 16));
	sf::Sprite red;
	red.setTexture(redPiece);
	red.setScale(8,8);

	/////////////////////INITIALIZE BLACK PIECE SPRITE AND TEXTURE///////////////////
	sf::Texture blackPiece;
	blackPiece.loadFromFile("CheckerPiece.png", sf::IntRect(0, 0, 16, 16));
	sf::Sprite black;
	black.setTexture(blackPiece);
	black.setScale(8, 8);
	/////////////////////INITIALIZE KING SYMBOL SPRITE AND TEXTURE///////////////////
	sf::Texture kingSymbolTexture;
	kingSymbolTexture.loadFromFile("KingSymbol.png", sf::IntRect(0, 0, 16, 16));
	sf::Sprite kingSymbol;
	kingSymbol.setTexture(kingSymbolTexture);
	kingSymbol.setScale(8, 8);

	/////////////////////INITIALIZE NECESSARY WIN TEXTURES AND SPRITES////////////////////

	sf::Texture blackwinText;
	blackwinText.loadFromFile("blackText.png", sf::IntRect(0, 0, 150, 100));
	sf::Sprite black_winText;
	black_winText.setTexture(blackwinText);
	black_winText.setScale(4, 4);
	black_winText.setPosition(225, 212);

	sf::Texture redwinText;
	redwinText.loadFromFile("redText.png", sf::IntRect(0, 0, 100, 100));
	sf::Sprite red_winText;
	red_winText.setTexture(redwinText);
	red_winText.setScale(4, 4);
	red_winText.setPosition(300, 212);

	sf::Texture winText;
	winText.loadFromFile("winText.png", sf::IntRect(0, 0, 150, 100));
	sf::Sprite win_Text;
	win_Text.setTexture(winText);
	win_Text.setScale(4, 4);
	win_Text.setPosition(250, 412);
	///////////////////////////////////////////////////////////////////////////////////////

	while(window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && endGame == false) {
				if (handChecker == 0) {
					
					putCheckerinHand(boardData, kingIndices, handChecker, handKing, originalIndex, redTurn);
				}
				else {
					placeChecker(boardData, handChecker,handKing, originalIndex, redTurn, kingIndices);

					if (winScenario(boardData, redWin)) endGame = true; //if one player has no checkers left, end the game.
				}
			}
		}
		window.clear();
		window.draw(board);
		loadBoard(red, black,kingSymbol, window,boardData, kingIndices);
		if (handChecker != 0) drawCheckerinHand(red,black,kingSymbol ,window,handChecker, handKing);
		if (endGame) drawWinScreen(red_winText, black_winText, win_Text, window, redWin);
		window.display();
	}
	return 0;
}



void drawWinScreen(sf::Sprite& redText, sf::Sprite& blackText, sf::Sprite& winText, sf::RenderWindow& window, bool redWin) {
	window.draw(winText);
	if (redWin) {
		window.draw(redText);
	}
	else {
		window.draw(blackText);
	}
}

bool winScenario(vector<int>& boardData, bool& redWin) {
	bool red = false;
	bool black = false;
	for (int i = 0; i < boardData.size(); ++i) { //checks for existence of red and black checkers, flagging accordingly.
		if (boardData.at(i) == 1) black = true;
		if (boardData.at(i) == 2) red = true;
		if (black && red) break;
	}
	if (black ^ red) redWin = red;
	return black ^ red;
}

void drawCheckerinHand(sf::Sprite& red, sf::Sprite& black,sf::Sprite& kingSymbol, sf::RenderWindow& window, int handChecker, bool handKing) {
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	if (handChecker == 1) {
		black.setPosition(mousePos.x-512,mousePos.y-128); //why is the checker so offset? BECAUSE IT MEASURES SCREEN PIXELS NOT WINDOW PIXELS
		window.draw(black);
		
	}
	else {
		red.setPosition(mousePos.x-512,mousePos.y-128);
		window.draw(red);
	}
	if (handKing) {
		kingSymbol.setPosition(mousePos.x - 512, mousePos.y - 128);
		window.draw(kingSymbol);
	}
}

void loadBoard(sf::Sprite& red, sf::Sprite& black, sf::Sprite& kingSymbol, sf::RenderWindow& window, vector<int>& boardData, vector<int>& kingIndices) {
	unsigned multiplier = 128;
	for (int i = 0; i < boardData.size(); ++i) {
		if (boardData.at(i) == 0) {
			continue;
		}
		else if (boardData.at(i) == 1) {
			black.setPosition(((i % 8) * multiplier), (i / 8) * multiplier);
			window.draw(black);
		}
		else {
			red.setPosition(((i % 8) * multiplier), (i / 8) * multiplier);
			window.draw(red);
		}
		if (kingIndices.at(i) == 1) {
			kingSymbol.setPosition(((i % 8) * multiplier), (i / 8) * multiplier);
			window.draw(kingSymbol);
		}
	}
}

void putCheckerinHand(vector<int>& boardData, vector<int>& kingData, int& handChecker,bool& handKing, int& originalIndex, bool redTurn) {
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	int index = ((mousePos.x-448) / 128) + ((mousePos.y-64) / 128) * 8; 
	
	if (boardData.at(index) == 0) {
		originalIndex = -1;
	}
	else {
		if (!( (boardData.at(index) == 2) ^redTurn)) {
			originalIndex = index;
		}
		else {
			return;
		}
	}
	
	//Get available moves from index and boardData

	//If within available moves THEN
	int temp = handChecker;
	handChecker = boardData.at(index);
	boardData.at(index) = temp;

	temp = handKing;
	handKing = kingData.at(index);
	kingData.at(index) = temp;
	/////////////////////////////////
}

void placeChecker(vector<int>& boardData, int& handChecker,bool& handKing, int& originalIndex, bool& redTurn, vector<int>& kingIndices) {
	vector<int> availableIndices = {};
	availableIndices.push_back(originalIndex);
	GetAvailableMoves(boardData, availableIndices, originalIndex, redTurn, 0, 1, false); //gathers all moves for the player

	if(handKing) GetAvailableMoves(boardData, availableIndices, originalIndex, redTurn, 0, 1, true);

	sf::Vector2i mousePos = sf::Mouse::getPosition();
	int index = ((mousePos.x - 448) / 128) + ((mousePos.y - 64) / 128) * 8;


	for (int i = 0; i < availableIndices.size(); ++i) {
		if (index == availableIndices.at(i)) {
			int temp = handChecker;
			handChecker = boardData.at(index);
			boardData.at(index) = temp; //swap checker value with board value. IE you place it down.

			temp = handKing;
			handKing = kingIndices.at(index);
			kingIndices.at(index) = temp;



			if((redTurn && index / 8 == 0) || (!redTurn && index / 8 == 7)) checkKing(index, redTurn, kingIndices); //check king list and add accordingly.




			//take away enemy checker if possible.
			int indexOffset = availableIndices.at(i) - originalIndex;

			if (abs(indexOffset) > 9) {
				int enemyPiece = 1 + !redTurn;
				if (boardData.at(originalIndex + indexOffset / 2) == enemyPiece) {
					boardData.at(originalIndex + indexOffset / 2) = 0; //if enemy piece is present, delete.
					kingIndices.at(originalIndex + indexOffset / 2) = 0;
				}
			}

			

			if (originalIndex != availableIndices.at(i) && !additionalMove(i, availableIndices, index, redTurn) ) redTurn = !redTurn; //dont switch the turns if the player puts the piece in its original pos.
			break;
		}
	}
}

void checkKing(int index, bool redTurn, vector<int>& kingIndices) {

		if (kingIndices.at(index) == 0) kingIndices.at(index) = 1;
}


bool additionalMove(int index, vector<int>& moves, int originalIndex, bool redTurn) {
	for (int i = index+1; i < moves.size(); ++i) {
		if (redTurn) {
			
			if ( ( (originalIndex - moves.at(i)) % 18 == 0) || ( (originalIndex - moves.at(i)) % 14 == 0) ) {
				return true;
			}
		}
		else {
			if (((moves.at(i)-originalIndex) % 18 == 0) || ((moves.at(i)-originalIndex) % 14 == 0)) {
				return true;
			}
		}
	}
	return false;
}

void GetAvailableMoves(vector<int>& boardData, vector<int>& moves, int originalIndex, bool redTurn, int step, int dir, bool handKing) {
	int enemyPiece = 1 + !redTurn;
	int friendlyPiece = 1 + redTurn;
	if (originalIndex < 0 || originalIndex >= boardData.size()) return;

	if (step % 2 == 0 && boardData.at(originalIndex) == enemyPiece) return;
	if (boardData.at(originalIndex) == friendlyPiece) return;

	if (step == 1 && boardData.at(originalIndex) == 0) { //immediate adjacent move
		moves.push_back(originalIndex);
		return;
	}
	if (step % 2 != 0 && boardData.at(originalIndex) == 0) return; //all other odd-step moves are unavailable


	if (step % 2 == 0 && boardData.at(originalIndex) == 0 && moves.at(0) != originalIndex) { //multiple adjacent move
		moves.push_back(originalIndex);
	}
	
	int offset = redTurn^handKing ? -8 : 8;

	GetAvailableMoves(boardData, moves, originalIndex + offset + dir, redTurn, step+1, dir, handKing);
	//if(handKing) GetAvailableMoves(boardData, moves, originalIndex - offset - dir, redTurn, step + 1, dir, handKing);

	if (step % 2 == 0) {
		dir = -dir;
		GetAvailableMoves(boardData, moves, originalIndex + offset + dir, redTurn, step+1, dir, handKing);
		//if(handKing) GetAvailableMoves(boardData, moves, originalIndex - offset - dir, redTurn, step + 1, dir, handKing);
	}
	return;

}