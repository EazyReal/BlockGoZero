#ifndef __EZWEIQIBOARD_H__
#define __EZWEIQIBOARD_H__

#include "TransformableBoard.h"
#include "EzWeiQiPiece.h"
#include <list>

using namespace std;

const int EZWEIQI_FONT_SIZE = 12;
const int EZWEIQI_EDGE = 13;
const int EZWEIQI_LINES = EZWEIQI_EDGE + 1;
const int EZWEIQI_BOARD_X_OFFSET = EZWEIQI_PIECE_SIZE*7;
const int EZWEIQI_BOARD_Y_OFFSET = EZWEIQI_PIECE_SIZE;
const int EZWEIQI_PIECE_NUM = 21;


const double dXInitialLocations[21] = {24.5, 3.5, 24.5, 24.5, 24.5, 24.5, 24.5, 31.5, 10.5, 17.5, 38.5, 31.5, 24.5, 31.5, 38.5, 31.5, 10.5, 17.5, 17.5, 24.5, 24.5};
const double dYInitialLocations[21] = {3.5, 24.5, 24.5, 24.5, 24.5, 24.5, 31.5, 24.5, 17.5, 38.5, 31.5, 10.5, 31.5, 24.5, 17.5, 38.5, 31.5, 10.5, 17.5, 24.5, 24.5};
const double dArrowLocations[9] = {0.0, 2.0, 4.4, 6.8, 9.2, 11.6, 14.0, 15.9, 17.3};

enum EzWeiQiGridType{
	EZWEIQIGRIDTYPE_EMPTY				= 0x00,
	EZWEIQIGRIDTYPE_BLACK				= 0x01,
	EZWEIQIGRIDTYPE_WHITE				= 0x02,
	EZWEIQIGRIDTYPE_PIECE				= 0x03,		//black or white piece
	EZWEIQIGRIDTYPE_VARIANT				= 0x04,
	EZWEIQIGRIDTYPE_CURRENT				= 0x08,
	EZWEIQIGRIDTYPE_VARIANT_CANDIDATE	= 0x10,
	EZWEIQIGRIDTYPE_NULL				= 0x20
};


/*!
Board of EzWeiQi
@author Ivan
@ingroup EzWeiQiGroup
*/
class EzWeiQiBoard : public TransformableBoard  
{
private:
	EzWeiQiPiece **m_pBlackPiecese;				//!< black piecese pointer
	EzWeiQiPiece **m_pWhitePiecese;				//!< white piecese pointer


protected:
	/*!
	pack all pieces from the new selected node to the root of tree.
	@param node The new selected node of tree item
	@author kk
	*/
	void packPieces(BaseTree::NodePointer node);

public:
	/*!
	Constructor
	@param iXMin the minimum X index you need for the game, and it can be negative.
	@param iYMin the minimum Y index you need for the game, and it can be negative.
	@param iXMax the maximum X index you need for the game.
	@param iYMax the maximum Y index you need for the game.
	@author Ivan
	*/
	EzWeiQiBoard( int iXMin, int iYMin, int iXMax, int iYMax, int nVerticalLines, int nHorizontalLines, const CPoint& nullMove );
	/*!
	Destructor
	@author Ivan
	*/
	~EzWeiQiBoard();
	/*!
	pack the board status when the tree item selected changed.
	pack all pieces from the new selected node to the root of tree.
	pack all variants from all children.
	pack all special mark from comments.
	@param node The new selected node of tree item
	@author Ivan
	*/
	void packCurrentNode(BaseTree::NodePointer node,MoveList* movelist = NULL);
	virtual bool isValid(int x, int y);
	bool isPieceValid(EzWeiQiPiece *pPiece);
	/*!
	get m_bBlackRemainPiecese or m_bWhiteRemainPiecese
	@param iColor PLAYERCOLOR_BLACK or PLAYERCOLOR_WHITE
	@author Ivan
	*/
	EzWeiQiPiece** getEzWeiQiPiecese(PlayerColor iColor);
	/*!
	set the piece to the initial location
	@param pPiece EzWeiQiPiece
	@author Ivan
	*/
	void setPieceInitialPosition(EzWeiQiPiece* pPiece);
	/*!
	set the piece to the location
	@param pPiece EzWeiQiPiece
	@param pos target position
	@author Ivan
	*/
	void setPiecePosition(EzWeiQiPiece* pPiece, CPoint pos);
	/*!
	set the piece state to selected
	@param pPiece EzWeiQiPiece
	@author Ivan
	*/
	void setPieceSelected(EzWeiQiPiece* pPiece);
	/*!
	get the piece type of the selected piece
	@param iColor the type needed is which color
	@author Ivan
	*/
	int getSelectedType(PlayerColor iColor);
	/*!
	set all same kind of piecese as used
	@param pPiece EzWeiQiPiece
	@param isUsed set value
	@author Ivan
	*/
	void setSamePieceUsed(EzWeiQiPiece* pPiece, bool isUsed);
	/*!
	reset the board value
	@author Ivan
	*/
	void clearEzWeiQiBoard();
	/*!
	update the variant according current board's selected type
	@param node current node
	@author Ivan
	*/
	void updateVariants(BaseTree::NodePointer node);
	/*!
	clear all variants
	@author Ivan
	*/
	void clearVariants();
	/*!
	traverse down and collect all the pieces to variants
	with the same move with param child.
	@param child The new selected node of tree item
	@author Ivan
	*/
	void setVariantsOfOneMove(BaseTree::NodePointer child);
	/*!
	traverse down and collect all the pieces which is variant candidate
	with the same move with param child.
	@param child The new selected node of tree item
	@author Ivan
	*/
	void setVariantsCandidateOfOneMove(BaseTree::NodePointer child);
	/*!
	pack all variants from all children and its sibling moves that 
	share at least one piece to param node.
	@param node The new selected node of tree item
	@author Ivan
	*/
	void packVariants(BaseTree::NodePointer node);
	/*!
	check if the piece at (x, y) is black or not (not variant)
	@param x x_axis
	@param y y_axis
	@author Ivan
	*/
	bool isBlack(int x, int y);
	/*!
	check if the piece at (x, y) is white or not (not variant)
	@param x x_axis
	@param y y_axis
	@author Ivan
	*/
	bool isWhite(int x, int y);
	/*!
	check if the piece at (x, y) is piece or not (not variant)
	@param x x_axis
	@param y y_axis
	@author Ivan
	*/
	bool isPiece(int x, int y);
	/*!
	check whether the point is in the region of selecting piece
	@param pPiece EzWeiQiPiece
	@param x x_axis value
	@param y x_axis value
	@author Ivan
	*/
	bool isInSelectRegion(EzWeiQiPiece *pPiece, int x, int y);
	list<BaseTree::NodePointer> getMovePath();
};

#endif