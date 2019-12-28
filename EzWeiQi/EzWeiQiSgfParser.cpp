#include "stdafx.h"
#include "EzWeiQiSgfParser.h"
#include "EzWeiQiBoard.h"

EzWeiQiSgfParser::EzWeiQiSgfParser() 
{
	setGameType( CString("1234") );
	addMoveId( CString("B") );
	addMoveId( CString("W") );
}
void EzWeiQiSgfParser::parseMove(const CString & sPropertyId, CString& sPropertyValue, NodePointer pCurrent)
{
	ASSERT ( pCurrent.isMoveNode() ) ;
	if ( sPropertyValue.GetLength() != 3 ) {
		reportError( ERRORTYPE_FORMAT, "length of piece should be 3.");
	} else {
		CPoint pos = CPoint( translateCharToInt(sPropertyValue.GetAt(0)), 
			translateCharToInt(sPropertyValue.GetAt(1)) );
		if ( pos.x == -1 || pos.y == -1 ) {
			reportError( ERRORTYPE_FORMAT, "Incorrect stone" + sPropertyValue + ".");
		}
		pCurrent.setLocation(pos);
		pCurrent.setType(translateCharToInt(sPropertyValue.GetAt(2)));
		if( sPropertyId == CString( "W" ) ) {
			pCurrent.setColor( PLAYERCOLOR_WHITE );
		} else if( sPropertyId == CString( "B" ) ) {
			pCurrent.setColor( PLAYERCOLOR_BLACK );
		}
	}
}

int EzWeiQiSgfParser::translateCharToInt( TCHAR c ) 
{
	if(isalpha(c)){
		if( isupper(c) ) {
			return c-'A';
		} else if( islower(c) ){
			return c-'a';
		}
	}else if(isdigit(c)){
		return c - '1' + 26;
	}
	return -1;
}

void EzWeiQiSgfParser::setSequence(const NodePointer& pParent, NodePointer& pCurrent ) 
{
	if( pParent.isRoot() ) {
		pCurrent.setSequence( 1 );
	} else if( pParent.getColor() == pCurrent.getColor() ){
		pCurrent.setSequence( pParent.getSequence() );
	} else { // pParent.getColor() != pCurrent.getColor()
		pCurrent.setSequence( pParent.getSequence()+1 );
	}
}
