
#include "stdafx.h"
#include "EzWeiQiSgfSerializer.h"
#include "EzWeiQiPiece.h"

EzWeiQiSgfSerializer::EzWeiQiSgfSerializer() 
{
	setGameType(1234); //GAMETYPE_EZWEIQI
}


void EzWeiQiSgfSerializer::serializeMove(CString& sResult, const NodePointer& pCurrent) 
{
	TCHAR color = '?';
	switch ( pCurrent.getColor() ) {
		case PLAYERCOLOR_BLACK: color = 'B'; break;
		case PLAYERCOLOR_WHITE: color = 'W'; break;
		default: color = '?';
	}
	sResult.Empty();
	sResult.AppendChar( color );
	sResult.AppendChar( '[' );
	sResult.AppendChar( translateIntToChar(pCurrent.getLocation().x) );	//loaction x
	sResult.AppendChar( translateIntToChar(pCurrent.getLocation().y) ); //loaction y
	sResult.AppendChar( translateIntToChar(pCurrent.getType()) );		//piece type
	sResult.AppendChar( ']' );

}

inline char EzWeiQiSgfSerializer::translateIntToChar( int c ) 
{
	if(c < 26)
		return 'A'+c;
	else
		return c - 26 + '1';
}