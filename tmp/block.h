#ifndef __EZWEIQIPIECE_H__
#define __EZWEIQIPIECE_H__


class POS
{
public:
  int x,y;
  POS(int _x, int _y) : x(_x), y(_y){}
  POS operator+(const POS&rhs)
  {
    return POS(x+rhs.x, y+rhs.y);
  }
};

typedef POS CPoint;

/*!
Piece of EzWeiQi
@author Ivan
@ingroup EzWeiQiGroup
*/
class EzWeiQiPiece :
{
public:
	int iPieceType;								//!< different kind of pieces
	PlayerColor m_color;						//!< PLAYERCOLOR_BLACK or PLAYERCOLOR_WHITE
	bool isUsed;								//!< is used on board or not
	bool isSelected;							//!< is select  or not
	bool isInSelecteList;						//!< is the candidate of selecting or not
	CPoint locations[4];						//!< location of each rectangular on board
	CPoint boardPoint[4];						//!< board poind of each rectangular on board


	/*!
	constructor
	@param clonePiece old piece
	@author Ivan
	*/
	EzWeiQiPiece();
	/*!
	destructor
	@author Ivan
	*/
	~EzWeiQiPiece();
	/*!
	allocate a new EzWeiQiPiece
	@author Ivan
	*/
	static EzWeiQiPiece* allocateNewPiece();
//////////////////////////////////////////////////////////////////////////
// Some location functions
	/*!
	set the location of the piece
	@param originPoint origin point of the piece
	@param iPieceSize piece size
	@author Ivan
	*/
	void setLocations(CPoint originPoint, int iPieceSize);
	/*!
	set the board point of the piece
	@param originPoint origin point of the piece
	@author Ivan
	*/
	void setBoardPoint(CPoint originPoint);
	/*!
	check whether the point is in the region of selecting piece
	@param x x_axis value
	@param y x_axis value
	@author Ivan
	*/
	bool isInSelectRegion(int x, int y);
//////////////////////////////////////////////////////////////////////////
// Conversiont functions
	/*!
	Function to caculate round(value) base on iPieceSize
	�Nvalue/iPieceSize���|�ˤ��J�A�b��iPieceSize
	@param value value that need be rounded
	@param iPieceSize round base
	@author Ivan
	*/
	static int getRoundValue(double value, int iPieceSize);
//////////////////////////////////////////////////////////////////////////
// some get & set functions
	/*!
	Function to set the piece type
	@param iPieceType which type want to set
	@author Ivan
	*/
	void setType(int iPieceType);
	/*!
	Function to get the piece type
	@author Ivan
	*/
	int getType();
	/*!
	Function to set the color type
	@param iColor which color want to set
	@author Ivan
	*/
	void setColor(int iColor);
	/*!
	Function to get the color type
	@author Ivan
	*/
	int getColor();
};

#endif
