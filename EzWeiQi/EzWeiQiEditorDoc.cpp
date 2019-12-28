#include "stdafx.h"
#include "EzWeiQiEditorDoc.h"
#include "UpdateHint.h"

IMPLEMENT_DYNCREATE(EzWeiQiEditorDoc, BoardEditorDoc)

BEGIN_MESSAGE_MAP(EzWeiQiEditorDoc, BoardEditorDoc)  

END_MESSAGE_MAP()

EzWeiQiEditorDoc::EzWeiQiEditorDoc()
: TransformableBoardEditorDoc(m_board),
m_board(-1, -1, 30, 30, 13, 13, CPoint(-1, -1))
{
}

void EzWeiQiEditorDoc::addOnePiece(CPoint pos)
{
	if(!getBoard().isValid(pos.x, pos.y)) return;	//not in the legal board coordinate 

	if(getBoard().getType(pos) & EZWEIQIGRIDTYPE_VARIANT ){	//the position is a variant
		goToNextVariantWithMouse(pos);
		return;
	}
	
	if(!isEditMode()) return;						//if not edit mode, simply return

	//<! IMPORTANT!! DO NOT CHANGE THE ORDER OF UPDATES BELOW!!
	
	BaseTree::NodePointer parentNode = getCurrentNode();
	EzWeiQiPiece** pPieceses;
	PlayerColor iColor = PLAYERCOLOR_BLACK;
	int iSequence = 1;
	int iPieceType;

	//<! Insert the new move into BaseTree structure
	if(parentNode.isBranch() && parentNode.isRoot() == false) parentNode.jumpToMoveNodeAbove();
	if(parentNode.isNull()) return;
	if(parentNode.isRoot() == false){
		iSequence = parentNode.getSequence() + 1;
		iColor = parentNode.getAgainstColor();		//get anainst color
	}
	iPieceType = m_board.getSelectedType(iColor);	//get selected piece type
	if(iPieceType == -1) return;					//player does not select the piece, just return
		
	
	//<! Setup the piece locations and try is valid or not
	pPieceses =  m_board.getEzWeiQiPiecese(iColor);
	getBoard().setPiecePosition(pPieceses[iPieceType], pos);		//put the piece at the position first 
	if(getBoard().isPieceValid(pPieceses[iPieceType]) == false){	//check is valid or not
		getBoard().setPieceInitialPosition(pPieceses[iPieceType]);	//position is not valid so set to initial position
		return;
	}

	//<! Setup the node state according to the piece
	BaseTree::NodePointer newNode;
	newNode.allocateNewMoveNode();					//allocate a new node
	newNode.setColor(iColor);						//setup color of this node
	newNode.setSequence(iSequence);					//setup the sequence of this node
	newNode.setLocation(pos);						//setup the position of this node
	newNode.setType(iPieceType);					//setup the type of this node. 0~8 kinds of piecese
	parentNode.addMoveNodeToChild(newNode);			//insert into BaseTree
		
	getMoveList().insertMove(newNode);				//update the movelist

	//<! Update the TreeView
	UpdateHint hint = UpdateHint::makeTreeUpdateHint(parentNode);	
	updateViews(NULL, hint);
	changeCurrentNode(newNode);

	SetModifiedFlag();								//set modified flag
}

void EzWeiQiEditorDoc::goToNextVariantWithMouse(CPoint pos)
{
	if(!(getBoard().getType(pos) & EZWEIQIGRIDTYPE_VARIANT))return;
	BaseTree::NodePointer node = getBoard().getPointer(pos);

	//update movelist
	//it the user select a item in treeview, that probably make no next element in list,
	//because the list will be repacked in function OnTreeSelectChanged.
	MoveList* pList = &getMoveList();
	BaseTree::NodePointer lastNode = getCurrentNode();

	int iPosID = pList->isPosInList(pos);

	if(!pList->hasNext()){
		pList->insertMove(node);
	}else if(iPosID != -1 && iPosID == pList->getIndex() + 1){
		node = pList->getAt(iPosID);
		pList->moveToNext();
	}else{
		pList->clearEnd();
		pList->insertMove(node);
	}
	
	//update tree view
	changeCurrentNode(node);

	//auto point mouse cursor
	if(pList->hasNext()){
		CPoint transformedPos = pList->getNext().getLocation();
		m_board.transformFromRealToTransformed(transformedPos);
		UpdateHint hint = UpdateHint::makeCursorHint(transformedPos);
		updateViews(NULL, hint);
		getBoard().setPieceSelected(getBoard().getEzWeiQiPiecese(pList->getNext().getColor())[pList->getNext().getType()]);
		getBoard().updateVariants(getCurrentNode());

	}
	else{
		BaseTree::NodePointer firstChild = node;
		firstChild.jumpToMoveNodeBelow();
		if( firstChild.isNotNull() ){
			while(firstChild.isNotNull()){
				if(firstChild.getType() == getBoard().getSelectedType(firstChild.getColor()))
					break;
				firstChild.jumpToRightSibling();
			}
			if( firstChild.isNotNull() ){
				CPoint transformedPos = firstChild.getLocation();
				m_board.transformFromRealToTransformed(transformedPos);
				UpdateHint hint = UpdateHint::makeCursorHint(transformedPos);
				updateViews(NULL, hint);
			}
		}
	}
}

void EzWeiQiEditorDoc::goToPreviousPieceWithMouse(CPoint pos)
{

	MoveList* pList = &getMoveList();
	if(pList->isAtRoot()) return;

	//auto point the mouse to the current piece
	CPoint transformedPos = pList->getCurrent().getLocation();
	getBoard().transformFromRealToTransformed(transformedPos);
	UpdateHint hint = UpdateHint::makeCursorHint(transformedPos);
	updateViews(NULL, hint);
	//store current selected
	int iCurrentSelect = pList->getCurrent().getType();
	EzWeiQiPiece* pPiece = getBoard().getEzWeiQiPiecese(pList->getCurrent().getColor())[iCurrentSelect];
	//set selected and the variants will set according to the selected piece

	//update movelist, if it is root now, this function will do nothing.
	pList->moveToPrevious();

	//go to parent move node, if current node it the topmost move node, then go to root.
	if(pList->isAtRoot()) {
		changeCurrentNode(getTree().getRootNode());
		getBoard().setPieceSelected(pPiece);
		getBoard().updateVariants(getTree().getRootNode());
	}
	else {
		changeCurrentNode(pList->getCurrent());
		getBoard().setPieceSelected(pPiece);
		getBoard().updateVariants(pList->getCurrent());
	}
	
}

EzWeiQiBoard& EzWeiQiEditorDoc::getBoard() const
{
	return dynamic_cast<EzWeiQiBoard&>(TransformableBoardEditorDoc::getBoard());
}

bool EzWeiQiEditorDoc::checkSelect(CPoint pos)
{
	if(getCurrentNode().isBranch() && getCurrentNode().isRoot() == false)
		return false;
	//<! Check selecting by judge whether the position is on the unused piece 
	//get current color
	PlayerColor iColor = PLAYERCOLOR_BLACK;
	if(getCurrentNode().isRoot() == false)
		iColor = getCurrentNode().getAgainstColor();
	EzWeiQiPiece** pPieceses =  getBoard().getEzWeiQiPiecese(iColor);
	//find whether the click point is in the region of piecese
	for(int i = 0; i<EZWEIQI_PIECE_NUM; ++i){
		if(getBoard().isInSelectRegion(pPieceses[i], pos.x, pos.y) == true){
			getBoard().setPieceSelected(pPieceses[i]);
			return true;
		}
	}	
	return false;
}