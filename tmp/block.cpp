#include "block.h"


using namespace std;

typedef POS CPoint;

EzWeiQiPiece::EzWeiQiPiece()
{
}

EzWeiQiPiece::~EzWeiQiPiece()
{
}

EzWeiQiPiece*  EzWeiQiPiece::allocateNewPiece()
{
	return new EzWeiQiPiece();
}

//////////////////////////////////////////////////////////////////////////
// Recalucate location/boardpoint functions
void EzWeiQiPiece::setLocations(CPoint originPoint, int iPieceSize)
{
	locations[0] = CPoint(originPoint.x, originPoint.y);
	switch(iPieceType){
		//|
	case 0:
		for(int k = 1; k<4; k++)
			locations[k] = CPoint(originPoint.x, originPoint.y + iPieceSize*k);
		break;
		//_
	case 1:
		for(int k = 1; k<4; k++)
			locations[k] = CPoint(originPoint.x + iPieceSize*k, originPoint.y);
		break;
	//___
	// |
	case 2:
		locations[1] = CPoint(originPoint.x + iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x - iPieceSize*1, originPoint.y);
		locations[3] = CPoint(originPoint.x, originPoint.y - iPieceSize*1);
		break;
	//|_
	//|
	case 3:
		locations[1] = CPoint(originPoint.x + iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y + iPieceSize*1);
		locations[3] = CPoint(originPoint.x, originPoint.y - iPieceSize*1);
		break;
	// |
	//___
	case 4:
		locations[1] = CPoint(originPoint.x + iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x + iPieceSize*-1, originPoint.y);
		locations[3] = CPoint(originPoint.x, originPoint.y + iPieceSize*1);
		break;
	//_|
	// |
	case 5:
		locations[1] = CPoint(originPoint.x - iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y + iPieceSize*1);
		locations[3] = CPoint(originPoint.x, originPoint.y - iPieceSize*1);
		break;
	//_
	// |_
	case 6:
		locations[1] = CPoint(originPoint.x - iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y - iPieceSize*1);
		locations[3] = CPoint(originPoint.x + iPieceSize*1, originPoint.y - iPieceSize*1);
		break;
	//
	case 7:
		locations[1] = CPoint(originPoint.x - iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y + iPieceSize*1);
		locations[3] = CPoint(originPoint.x - iPieceSize*1, originPoint.y - iPieceSize*1);
		break;
	case 8:
		locations[1] = CPoint(originPoint.x + iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y + iPieceSize*1);
		locations[3] = CPoint(originPoint.x + iPieceSize*2, originPoint.y);
		break;
	case 9:
		locations[1] = CPoint(originPoint.x + iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y - iPieceSize*1);
		locations[3] = CPoint(originPoint.x, originPoint.y - iPieceSize*2);
		break;
	case 10:
		locations[1] = CPoint(originPoint.x - iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y - iPieceSize*1);
		locations[3] = CPoint(originPoint.x - iPieceSize*2, originPoint.y);
		break;
	case 11:
		locations[1] = CPoint(originPoint.x - iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y + iPieceSize*1);
		locations[3] = CPoint(originPoint.x, originPoint.y + iPieceSize*2);
		break;
	case 12:
		locations[1] = CPoint(originPoint.x + iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y - iPieceSize*1);
		locations[3] = CPoint(originPoint.x - iPieceSize*1, originPoint.y - iPieceSize*1);
		break;
	case 13:
		locations[1] = CPoint(originPoint.x - iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y - iPieceSize*1);
		locations[3] = CPoint(originPoint.x - iPieceSize*1, originPoint.y + iPieceSize*1);
		break;
	case 14:
		locations[1] = CPoint(originPoint.x - iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y + iPieceSize*1);
		locations[3] = CPoint(originPoint.x - iPieceSize*2, originPoint.y);
		break;
	case 15:
		locations[1] = CPoint(originPoint.x - iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y - iPieceSize*1);
		locations[3] = CPoint(originPoint.x, originPoint.y - iPieceSize*2);
		break;
	case 16:
		locations[1] = CPoint(originPoint.x + iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y - iPieceSize*1);
		locations[3] = CPoint(originPoint.x + iPieceSize*2, originPoint.y);
		break;
	case 17:
		locations[1] = CPoint(originPoint.x + iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y + iPieceSize*1);
		locations[3] = CPoint(originPoint.x, originPoint.y + iPieceSize*2);
		break;
	case 18:
		locations[1] = CPoint(originPoint.x + iPieceSize*1, originPoint.y);
		locations[2] = CPoint(originPoint.x, originPoint.y + iPieceSize*1);
		locations[3] = CPoint(originPoint.x + iPieceSize*1, originPoint.y + iPieceSize*1);
		break;
	}
}

void EzWeiQiPiece::setBoardPoint(CPoint originPoint)
{
	boardPoint[0] = CPoint(originPoint.x, originPoint.y);
	switch(iPieceType){
	case 0:
		for(int k = 1; k<4; k++)
			boardPoint[k] = CPoint(originPoint.x, originPoint.y - k);
		break;
	case 1:
		for(int k = 1; k<4; k++)
			boardPoint[k] = CPoint(originPoint.x + k, originPoint.y);
		break;
	case 2:
		boardPoint[1] = CPoint(originPoint.x + 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x - 1, originPoint.y);
		boardPoint[3] = CPoint(originPoint.x, originPoint.y + 1);
		break;
	case 3:
		boardPoint[1] = CPoint(originPoint.x + 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y - 1);
		boardPoint[3] = CPoint(originPoint.x, originPoint.y + 1);
		break;
	case 4:
		boardPoint[1] = CPoint(originPoint.x + 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x - 1, originPoint.y);
		boardPoint[3] = CPoint(originPoint.x, originPoint.y - 1);
		break;
	case 5:
		boardPoint[1] = CPoint(originPoint.x - 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y - 1);
		boardPoint[3] = CPoint(originPoint.x, originPoint.y + 1);
		break;
	case 6:
		boardPoint[1] = CPoint(originPoint.x - 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y + 1);
		boardPoint[3] = CPoint(originPoint.x + 1, originPoint.y + 1);
		break;
	case 7:
		boardPoint[1] = CPoint(originPoint.x - 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y - 1);
		boardPoint[3] = CPoint(originPoint.x - 1, originPoint.y + 1);
		break;
	case 8:
		boardPoint[1] = CPoint(originPoint.x + 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y - 1);
		boardPoint[3] = CPoint(originPoint.x + 2, originPoint.y);
		break;
	case 9:
		boardPoint[1] = CPoint(originPoint.x + 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y + 1);
		boardPoint[3] = CPoint(originPoint.x, originPoint.y + 2);
		break;
	case 10:
		boardPoint[1] = CPoint(originPoint.x - 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y + 1);
		boardPoint[3] = CPoint(originPoint.x - 2, originPoint.y);
		break;
	case 11:
		boardPoint[1] = CPoint(originPoint.x - 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y - 1);
		boardPoint[3] = CPoint(originPoint.x, originPoint.y - 2);
		break;
	case 12:
		boardPoint[1] = CPoint(originPoint.x + 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y + 1);
		boardPoint[3] = CPoint(originPoint.x - 1, originPoint.y + 1);
		break;
	case 13:
		boardPoint[1] = CPoint(originPoint.x - 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y + 1);
		boardPoint[3] = CPoint(originPoint.x - 1, originPoint.y - 1);
		break;
	case 14:
		boardPoint[1] = CPoint(originPoint.x - 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y - 1);
		boardPoint[3] = CPoint(originPoint.x - 2, originPoint.y);
		break;
	case 15:
		boardPoint[1] = CPoint(originPoint.x - 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y + 1);
		boardPoint[3] = CPoint(originPoint.x, originPoint.y + 2);
		break;
	case 16:
		boardPoint[1] = CPoint(originPoint.x + 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y + 1);
		boardPoint[3] = CPoint(originPoint.x + 2, originPoint.y);
		break;
	case 17:
		boardPoint[1] = CPoint(originPoint.x + 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y - 1);
		boardPoint[3] = CPoint(originPoint.x, originPoint.y - 2);
		break;
	case 18:
		boardPoint[1] = CPoint(originPoint.x + 1, originPoint.y);
		boardPoint[2] = CPoint(originPoint.x, originPoint.y - 1);
		boardPoint[3] = CPoint(originPoint.x + 1, originPoint.y - 1);
		break;
	}
}
bool EzWeiQiPiece::isInSelectRegion(int x, int y)
{
	//only the piece out of board can be selected
	if(isUsed == true)
		return false;

	int x_axis;
	int y_axis;
	if(iPieceType < 19){
		for(int k = 0; k<4; k++){
			x_axis = locations[k].x;
			y_axis = locations[k].y;
			if(x >= x_axis && x <= x_axis + EZWEIQI_OUTBOARD_PIECE_SIZE  && y >= y_axis && y <= y_axis + EZWEIQI_OUTBOARD_PIECE_SIZE)
				return true;
		}
	}
	else if(iPieceType ==19 || iPieceType == 20){
		x_axis = locations[0].x;
		y_axis = locations[0].y;
		if(x >= x_axis && x <= x_axis + EZWEIQI_OUTBOARD_PIECE_SIZE  && y >= y_axis && y <= y_axis + EZWEIQI_OUTBOARD_PIECE_SIZE)
			return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
// some get & set functions
void EzWeiQiPiece::setType(int iPieceType)
{
	this->iPieceType = iPieceType;
}

int EzWeiQiPiece::getType()
{
	return iPieceType;
}

void EzWeiQiPiece::setColor(int iColor)
{
	this->m_color = iColor;
}

int EzWeiQiPiece::getColor()
{
	return m_color;
}
