#ifndef	_EZWEIQISGFSERIALIZER_H_
#define	_EZWEIQISGFSERIALIZER_H_

#include "BaseSerializer.h"
/*!
sgf serializer of EzWeiQi
@author Ivan
@ingroup EzWeiQiGroup
*/
class EzWeiQiSgfSerializer : public BaseSerializer
{


public :

	EzWeiQiSgfSerializer() ;

private:
	/*!
	@brief implementation of pure virtual function
	@author kk
	@param	sResult	[out] serialized string 
	@param	pCurrent [in] MoveNode to be serialized ( only move part, i.e. excluded comment & unknown property )
	*/
	virtual void serializeMove(CString& sResult, const NodePointer& );
	/*!
	@brief translate integer coordinate to sgf representation
	@author kl
	@param	c [in] x or y coordinate stored in CPoint structure
	@return	translated char for sgf representation
	*/
	inline char translateIntToChar( int c );
};


#endif // _GOSGFPARSER_H_
