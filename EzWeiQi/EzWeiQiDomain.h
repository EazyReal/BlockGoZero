#ifndef __EZWEIQIDOMAIN_H__
#define __EZWEIQIDOMAIN_H__

#include "EzWeiQiBoard.h"
#include "BaseNode.h"

const int PLAYERCOLOR_PIECE = 3;		//<! self define


enum DomainColor {
	DOMAINCOLOR_UNKNOWN = 0x00,
	DOMAINCOLOR_EMPTY	= 0x01,
	DOMAINCOLOR_BLACK	= 0x02,
	DOMAINCOLOR_WHITE	= 0x04,
	DOMAINCOLOR_PIECE	= 0x06,
};

/*!
Domain information of EzWeiQi
@author Ivan
@ingroup EzWeiQiGroup
*/
class EzWeiQiDomain
{
private:
	DomainColor m_pDomain[EZWEIQI_EDGE][EZWEIQI_EDGE];		//<! domain state
	EzWeiQiBoard *m_pBoard;									//<! board information
	bool visited[EZWEIQI_EDGE][EZWEIQI_EDGE];				//<! visited state
public:
	/*!
	Constructor initialize variables
	@param pBoard EzWeiQiBoard
	@author Ivan
	*/
	EzWeiQiDomain(EzWeiQiBoard *pBoard);
	
	int getAllEmptySize();
//////////////////////////////////////////////////////////////////////////
// Conversion functions
	bool isValid(int x, int y);
	
//////////////////////////////////////////////////////////////////////////
// m_Domain functions
	void initialDomain();
	void setDomainType(CPoint pos, DomainColor type);
	void setDomainType(int x, int y, DomainColor type);
	DomainColor getDomainType(CPoint pos);
	DomainColor getDomainType(int x, int y);
//////////////////////////////////////////////////////////////////////////
// visited functions
	void initialVisited();
	void setVisited(CPoint pos);
	bool isVisited(CPoint pos);
	bool isVisited(int x, int y);
	/*!
	Function to refresh the domain accoring board information
	@author Ivan
	*/
//////////////////////////////////////////////////////////////////////////
// BFS functions
	void refreshDomain();
	/*!
	Function to mark the domain starting from pos with playerColor
	@param domainColor 
	@param pos
	@author Ivan
	*/
	void markDomain(DomainColor domainColor, CPoint pos);
	/*!
	Function to check whether has oppoent's domain inside
	@param domainColor
	@param pos
	@author Ivan
	*/
	bool hasDomainInside(DomainColor domainColor, CPoint pos);
	/*!
	Function to caculate the 'playerColor' domain size with domain color = 'domainColor'
	@param domainColor 
	@param playerColor
	@param pos
	@author Ivan
	*/
	int domainSize(PlayerColor playerColor, CPoint pos);
	/*!
	Function to check whether playerColor has inner domain or not
	@param playerColor
	@param pos
	@author Ivan
	*/
	bool isInnerDomain(PlayerColor playerColor, CPoint pos);
};

#endif