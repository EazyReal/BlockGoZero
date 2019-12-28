#ifndef __EZWEIQIEDITORDOC_H__
#define __EZWEIQIEDITORDOC_H__

#include "TransformableBoardEditorDoc.h"
#include "BaseNode.h"
#include "EzWeiQiSgfParser.h"
#include "EzWeiQiSgfSerializer.h"
#include "EzWeiQiBoard.h"

class EzWeiQiEditorView;

/*!
doc of EzWeiQi
@author Ivan
@ingroup EzWeiQiGroup
*/
class EzWeiQiEditorDoc : public TransformableBoardEditorDoc
{
	DECLARE_DYNCREATE(EzWeiQiEditorDoc)
	DECLARE_MESSAGE_MAP()
protected:
	EzWeiQiBoard m_board;					//!< board to present current position

	EzWeiQiEditorDoc();
public:
	
	/*!
	add one piece on the board.
	@param pos position
	@author Ivan
	*/
	void addOnePiece(CPoint pos);
	void goToNextVariantWithMouse(CPoint pos);
	void goToPreviousPieceWithMouse(CPoint pos);
	bool checkSelect(CPoint pos);
	EzWeiQiBoard& getBoard() const;
};

#endif