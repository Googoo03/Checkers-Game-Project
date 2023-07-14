#ifndef PIECE_H
#define PIECE_H

#include "SFML/Graphics.hpp"

class Piece {

public:
	Piece(bool red, sf::Vector2u pos): textureSize(8), position(pos){ //perhaps change texture size later?
		sf::Texture texture;
		if (red) {
			texture.loadFromFile("CheckerPieceRed.png", sf::IntRect(0, 0, 16, 16));
		}
		else {
			texture.loadFromFile("CheckerPiece.png", sf::IntRect(0, 0, 16, 16));
		}

		sprite.setTexture(texture);
	}
private:
	uint16_t textureSize;
	sf::Vector2u position;
	sf::Sprite sprite;
};

#endif
