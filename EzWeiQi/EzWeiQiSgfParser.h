#ifndef	_EZWEIQISGFPARSER_H_
#define	_EZWEIQISGFPARSER_H_

#include "BaseParser.h"
/*!
sgf parser of EzWeiQi
@author Ivan
@ingroup EzWeiQiGroup
*/
class EzWeiQiSgfParser : public BaseParser
{
public :
	EzWeiQiSgfParser() ;
private:
	/*!
	@brief implementation of pure virtual function
	@author kk
	@param	sPropertyId [in] property Id got from parser
	@param	sPropertyValue [in] property value got from parser
	@param	pCurrent [inout] Node to store information
	*/
	virtual void parseMove(const CString & sPropertyId, CString& sPropertyValue, NodePointer pCurrent) ;
	/*!
	@brief translate sgf representation to integer coordinate
	@author kk
	@param	c [in] sgf representation
	@return	translated char for x or y coordinate stored in CPoint structure
	*/
	int translateCharToInt( TCHAR c );
	virtual void setSequence(const NodePointer& pParent, NodePointer& pCurrent ) ;
};


#endif // _GOSGFPARSER_H_
