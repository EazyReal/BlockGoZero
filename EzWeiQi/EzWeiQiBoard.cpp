#include "stdafx.h"
#include "EzWeiQiBoard.h"
#include "DebugStream.h"

EzWeiQiBoard::EzWeiQiBoard( int iXMin, int iYMin, int iXMax, int iYMax, int nVerticalLines, int nHorizontalLines, const CPoint& nullMove )
:TransformableBoard( iXMin, iYMin, iXMax, iYMax, nVerticalLines, nHorizontalLines, nullMove )
{
	//intialize
	m_pBlackPiecese = new EzWeiQiPiece*[EZWEIQI_PIECE_NUM];
	m_pWhitePiecese = new EzWeiQiPiece*[EZWEIQI_PIECE_NUM];
	for(int i = 0; i<EZWEIQI_PIECE_NUM; ++i){
		m_pBlackPiecese[i] = EzWeiQiPiece::allocateNewPiece();
		m_pWhitePiecese[i] = EzWeiQiPiece::allocateNewPiece();
	}
	clearEzWeiQiBoard();
}

EzWeiQiBoard::~EzWeiQiBoard()
{
}

void EzWeiQiBoard::packCurrentNode(BaseTree::NodePointer node,MoveList* movelist)
{
	clearEzWeiQiBoard();
	for(int i = 0;i < EZWEIQI_EDGE; i++)
		for(int j = 0;j < EZWEIQI_EDGE; j++)
			setPointer(i, j, NULL);
	clear();
	TransformableBoard::packCurrentNode(node, movelist);
	packPieces( node );
	packVariants( node );
}
						
void EzWeiQiBoard::packPieces(BaseTree::NodePointer node)
{
	BaseTree::NodePointer trace = node;

	//setup the black/white piece to board
	while(trace.isRoot() == false){
		if(trace.isMoveNode()){
			CPoint pos = trace.getLocation();
			PlayerColor color = trace.getColor();
			int iPieceType = trace.getType();
			if(getType(pos) == EZWEIQIGRIDTYPE_EMPTY){

				//black piece, white piece
				if(trace.isBlack()){
					setSamePieceUsed(m_pBlackPiecese[iPieceType], true);
					m_pBlackPiecese[iPieceType]->isSelected = false;
					setPiecePosition(m_pBlackPiecese[iPieceType], pos);
					for(int i=0; i<4; ++i){
						addType(m_pBlackPiecese[iPieceType]->boardPoint[i], EZWEIQIGRIDTYPE_BLACK);
						if(trace.getType() >= 19) break;
					}
				}
				if(trace.isWhite()){
					setSamePieceUsed(m_pWhitePiecese[iPieceType], true);
					m_pWhitePiecese[iPieceType]->isSelected = false;
					setPiecePosition(m_pWhitePiecese[iPieceType], pos);
					for(int i=0; i<4; ++i){
						addType(m_pWhitePiecese[iPieceType]->boardPoint[i], EZWEIQIGRIDTYPE_WHITE);
						if(trace.getType() >=19 ) break;
					}
				}
				setPointer(pos, trace);
			}
		}
		trace.moveUp();
	}

	if(node.isBranch()) node.jumpToMoveNodeAbove();
	if(node.isNull()) return;
	if(node.isMoveNode()){
		addType(node.getLocation(), EZWEIQIGRIDTYPE_CURRENT);
	}
}

void EzWeiQiBoard::packVariants(BaseTree::NodePointer node)
{
	BaseTree::NodePointer trace = node;

	//if the selected move has at least one child, go to the immediate branch above its child.
	if(trace.hasDown()){
		if(trace.getDown().isBranch()){
			trace.moveDown();
		}
	}
	//if the selected move is a branch, then go to the first sibling branch.
	if(trace.isBranch()){
		trace.jumpToLeftMost();
	}
	//setup the black/white variants to board, so trace all the child moves
	bool isFirstTime = true;
	while(trace.isNotNull()){
		if(trace.getDown().isNotNull() && !trace.getDown().isBranch()){

			if(isFirstTime){
				this->setPieceSelected(getEzWeiQiPiecese(trace.getDown().getColor())[trace.getDown().getType()]);
				isFirstTime = false;
			}
			getEzWeiQiPiecese(trace.getDown().getColor())[trace.getDown().getType()]->isInSelecteList = true;
			if(trace.getDown().getType() == getSelectedType(trace.getDown().getColor())){
				setVariantsOfOneMove(trace.getDown());
			}
			else{
				setVariantsCandidateOfOneMove(trace.getDown());
			}
			
		}
		trace.moveRight();
	}
}

void EzWeiQiBoard::setVariantsOfOneMove(BaseTree::NodePointer child)
{
	if(child.isNull())return;
	if(child.isBranch())return;
	CPoint pos = child.getLocation();
	if(child.isBlack())
		addType(pos, EZWEIQIGRIDTYPE_BLACK | EZWEIQIGRIDTYPE_VARIANT);
	if(child.isWhite())
		addType(pos, EZWEIQIGRIDTYPE_WHITE | EZWEIQIGRIDTYPE_VARIANT);
	setPointer(pos, child);
}

void EzWeiQiBoard::setVariantsCandidateOfOneMove(BaseTree::NodePointer child)
{
	if(child.isNull())return;
	if(child.isBranch())return;
	CPoint pos = child.getLocation();
	if(child.isBlack())
		addType(pos, EZWEIQIGRIDTYPE_BLACK | EZWEIQIGRIDTYPE_VARIANT_CANDIDATE);
	if(child.isWhite())
		addType(pos, EZWEIQIGRIDTYPE_WHITE | EZWEIQIGRIDTYPE_VARIANT_CANDIDATE);
}

bool EzWeiQiBoard::isValid(int x, int y)
{
	return 0 <= x && x < getHorizontalLineNumber() && 0 <= y && y < getVerticalLineNumber() || CPoint(x, y) == getNullMove();

}

bool EzWeiQiBoard::isPieceValid(EzWeiQiPiece *pPiece)
{
	bool isAdjacent = false;
	bool isStartPoint = false;
	int iXShift[4] = {1, -1, 0, 0};
	int iYShift[4] = {0, 0, 1, -1};
	int x_axis;
	int y_axis;
	for(int i = 0; i<4; ++i){
		x_axis = pPiece->boardPoint[i].x;
		y_axis = pPiece->boardPoint[i].y;
		//check all the board point of piece is valid 
		if(!isValid(x_axis, y_axis) || (isPiece(x_axis, y_axis)))
			return false;
		
		if(isStartPoint == false){			//check is at start point
			if((x_axis == 3 && y_axis == 3) || (x_axis == 3 && y_axis == 9) || (x_axis == 9 && y_axis == 3) || (x_axis == 9 && y_axis == 9))
				isStartPoint = true;
			if(isStartPoint == false && isAdjacent == false){		//check is adjacent to other piece
				for(int j = 0; j<4; ++j)
					if( isValid(x_axis + iXShift[j], y_axis + iYShift[j]) == true)
						if( (isBlack(x_axis + iXShift[j], y_axis + iYShift[j]) && pPiece->getColor() == PLAYERCOLOR_BLACK) ||
							(isWhite(x_axis + iXShift[j], y_axis + iYShift[j]) && pPiece->getColor() == PLAYERCOLOR_WHITE) )
							isAdjacent = true;
			}
		}
		
		if(pPiece->getType() >= 19)
			break;
	}
	if(isStartPoint == false && isAdjacent == false)
		return false;
	return true;
}
EzWeiQiPiece** EzWeiQiBoard::getEzWeiQiPiecese(PlayerColor iColor)
{
	if(iColor == PLAYERCOLOR_BLACK)
		return m_pBlackPiecese;
	else if(iColor == PLAYERCOLOR_WHITE)
		return m_pWhitePiecese;
	return NULL;
}

void EzWeiQiBoard::setPieceInitialPosition(EzWeiQiPiece* pPiece)
{
	/*
		21 pieces divide into 3 * 7 to put on board
	*/
	int iOriginPointX = pPiece->getType()%3;
	int iOriginPointY = (pPiece->getType()/3)%7;
	pPiece->isUsed = false;
	if(pPiece->getColor() == PLAYERCOLOR_BLACK){
		pPiece->setLocations(
			CPoint( 
				(int)(iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.3 + dXInitialLocations[pPiece->getType()]),
				(int)(iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9 + dYInitialLocations[pPiece->getType()])
			),EZWEIQI_OUTBOARD_PIECE_SIZE);
	}
	else if(pPiece->getColor() == PLAYERCOLOR_WHITE){
		pPiece->setLocations(
			CPoint( 
				(int)(iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*20.5 + dXInitialLocations[pPiece->getType()]),
				(int)(iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9 + dYInitialLocations[pPiece->getType()])
			),EZWEIQI_OUTBOARD_PIECE_SIZE);
	}
}

void EzWeiQiBoard::setPiecePosition(EzWeiQiPiece* pPiece, CPoint pos)
{
	pPiece->setLocations(
		CPoint(EZWEIQI_BOARD_X_OFFSET + pos.x * EZWEIQI_PIECE_SIZE, EZWEIQI_BOARD_Y_OFFSET + (14 - pos.y - 2) * EZWEIQI_PIECE_SIZE ),
		EZWEIQI_PIECE_SIZE);
	pPiece->setBoardPoint(pos);
}
//////////////////////////////////////////////////////////////////////////
// Selected functions
void EzWeiQiBoard::setPieceSelected(EzWeiQiPiece* pPiece)
{
	for(int i = 0; i<EZWEIQI_PIECE_NUM; ++i){
		m_pBlackPiecese[i]->isSelected = false;
		m_pWhitePiecese[i]->isSelected = false;
	}
	pPiece->isSelected = true;
	
}

int EzWeiQiBoard::getSelectedType(PlayerColor iColor)
{
	for(int i = 0; i<EZWEIQI_PIECE_NUM; ++i)
		if(iColor == PLAYERCOLOR_BLACK && m_pBlackPiecese[i]->isSelected == true)
			return i;
		else if(iColor == PLAYERCOLOR_WHITE && m_pWhitePiecese[i]->isSelected == true)
			return i;
	return -1;
}

void EzWeiQiBoard::setSamePieceUsed(EzWeiQiPiece* pPiece, bool isUsed)
{
	EzWeiQiPiece **piecese;
	piecese = getEzWeiQiPiecese(pPiece->getColor());
	switch(pPiece->getType()){
	case 0:case 1:
		for(int i=0; i<=1; ++i)
			piecese[i]->isUsed = isUsed;
		break;
	case 2:case 3:case 4:case 5:
		for(int i=2; i<=5; ++i)
			piecese[i]->isUsed = isUsed;
		break;
	case 6:case 7:
		for(int i=6; i<=7; ++i)
			piecese[i]->isUsed = isUsed;
		break;
	case 8:case 9:case 10:case 11:
		for(int i=8; i<=11; ++i)
			piecese[i]->isUsed = isUsed;
		break;
	case 12:case 13:
		for(int i=12; i<=13; ++i)
			piecese[i]->isUsed = isUsed;
		break;
	case 14:case 15:case 16:case 17:
		for(int i=14; i<=17; ++i)
			piecese[i]->isUsed = isUsed;
		break;
	case 18:
		piecese[18]->isUsed = isUsed;
		break;
	case 19:
		piecese[19]->isUsed = isUsed;
		break;
	case 20:
		piecese[20]->isUsed = isUsed;
		break;
	}
}

void EzWeiQiBoard::clearEzWeiQiBoard()
{
	//intialize
	for(int i = 0; i<EZWEIQI_PIECE_NUM; ++i){
		m_pBlackPiecese[i]->setType(i);
		m_pBlackPiecese[i]->setColor(PLAYERCOLOR_BLACK);
		m_pBlackPiecese[i]->isInSelecteList = false;
		setPieceInitialPosition(m_pBlackPiecese[i]);

		m_pWhitePiecese[i]->setType(i);
		m_pWhitePiecese[i]->setColor(PLAYERCOLOR_WHITE);
		m_pWhitePiecese[i]->isInSelecteList = false;
		setPieceInitialPosition(m_pWhitePiecese[i]);
	}
}

void EzWeiQiBoard::updateVariants(BaseTree::NodePointer node)
{
	BaseTree::NodePointer trace = node;

	//if the selected move has at least one child, go to the immediate branch above its child.
	if(trace.hasDown()){
		if(trace.getDown().isBranch()){
			trace.moveDown();
		}
	}
	//if the selected move is a branch, then go to the first sibling branch.
	if(trace.isBranch()){
		trace.jumpToLeftMost();
	}
	//setup the black/white variants to board, so trace all the child moves
	clearVariants();
	while(trace.isNotNull()){
		if(trace.getDown().isNotNull() && !trace.getDown().isBranch()){
			if(getSelectedType(trace.getDown().getColor()) == -1){
				this->setPieceSelected(getEzWeiQiPiecese(trace.getDown().getColor())[trace.getDown().getType()]);
			}
			getEzWeiQiPiecese(trace.getDown().getColor())[trace.getDown().getType()]->isInSelecteList = true;
			if(trace.getDown().getType() == getSelectedType(trace.getDown().getColor()))
				setVariantsOfOneMove(trace.getDown());
			else{
				setVariantsCandidateOfOneMove(trace.getDown());
			}
		}
		trace.moveRight();
	}
}

void EzWeiQiBoard::clearVariants()
{
	for(int i = 0; i<EZWEIQI_PIECE_NUM; ++i){
		m_pBlackPiecese[i]->isInSelecteList = false;
		m_pWhitePiecese[i]->isInSelecteList = false;
	}
	for(int i = 0; i<14; ++i){
		for(int j = 0; j<14; ++j){
			if(getType(i, j) & EZWEIQIGRIDTYPE_VARIANT || getType(i, j) & EZWEIQIGRIDTYPE_VARIANT_CANDIDATE ){
				setType(CPoint(i, j), EZWEIQIGRIDTYPE_EMPTY);
				setPointer(CPoint(i, j), NULL);
			}
		}
	}
}


bool EzWeiQiBoard::isBlack(int x, int y)
{
	return (getType(x, y) & EZWEIQIGRIDTYPE_BLACK && !(getType(x, y) & EZWEIQIGRIDTYPE_VARIANT || getType(x, y) & EZWEIQIGRIDTYPE_VARIANT_CANDIDATE));
}

bool EzWeiQiBoard::isWhite(int x, int y)
{
	return (getType(x, y) & EZWEIQIGRIDTYPE_WHITE && !(getType(x, y) & EZWEIQIGRIDTYPE_VARIANT || getType(x, y) & EZWEIQIGRIDTYPE_VARIANT_CANDIDATE));
}

bool EzWeiQiBoard::isPiece(int x, int y)
{
	return (getType(x, y) & EZWEIQIGRIDTYPE_PIECE && !(getType(x, y) & EZWEIQIGRIDTYPE_VARIANT || getType(x, y) & EZWEIQIGRIDTYPE_VARIANT_CANDIDATE));
}

bool EzWeiQiBoard::isInSelectRegion(EzWeiQiPiece *pPiece, int x, int y)
{
	int iPieceType = pPiece->getType();
	int iOriginPointX = iPieceType%3;
	int iOriginPointY = (iPieceType/3)%7;
	if(pPiece->isUsed == true)		//if is used then simple return
		return false;
	if(pPiece->getColor() == PLAYERCOLOR_BLACK){
		if( x > iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.3 &&
			x < (iOriginPointX+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.3 &&
			y > iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9 &&
			y < (iOriginPointY+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9)
		{
			return true;
		}
	}
	else if(pPiece->getColor() == PLAYERCOLOR_WHITE){
		if( x > iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*20.5 &&
			x < (iOriginPointX+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*20.5 &&
			y > iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9 &&
			y < (iOriginPointY+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9 )
		{
			return true;
		}
	}
}
