#include "stdafx.h"
#include "EzWeiQiDomain.h"
#include <list>

using namespace std;

EzWeiQiDomain::EzWeiQiDomain(EzWeiQiBoard *pBoard)
{
	m_pBoard = pBoard;
	for(int i=0; i<EZWEIQI_EDGE; ++i)
		for(int j=0; j<EZWEIQI_EDGE; ++j){
			m_pDomain[i][j] = DOMAINCOLOR_UNKNOWN;
			visited[i][j] = false;
		}
}

//////////////////////////////////////////////////////////////////////////
// Conversion functions
bool EzWeiQiDomain::isValid(int x, int y)
{
	if(x < 0 || y < 0 || x >= EZWEIQI_EDGE || y >= EZWEIQI_EDGE) return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// m_Domain functions
void EzWeiQiDomain::initialDomain()
{
	for(int i=0; i<EZWEIQI_EDGE; ++i)
		for(int j=0; j<EZWEIQI_EDGE; ++j)
			m_pDomain[i][j] = DOMAINCOLOR_UNKNOWN;
}
void EzWeiQiDomain::setDomainType(CPoint pos, DomainColor type)
{
	m_pDomain[pos.x][pos.y] = type;
}

void EzWeiQiDomain::setDomainType(int x, int y, DomainColor type)
{
	m_pDomain[x][y] = type;
}

DomainColor EzWeiQiDomain::getDomainType(CPoint pos)
{
	return m_pDomain[pos.x][pos.y];
}

DomainColor EzWeiQiDomain::getDomainType(int x, int y)
{
	return m_pDomain[x][y];
}

//////////////////////////////////////////////////////////////////////////
// visited functions
void EzWeiQiDomain::initialVisited()
{
	for(int i=0; i<EZWEIQI_EDGE; ++i)
		for(int j=0; j<EZWEIQI_EDGE; ++j)
			visited[i][j] = false;
}

void EzWeiQiDomain::setVisited(CPoint pos)
{
	visited[pos.x][pos.y] = true;
}

bool EzWeiQiDomain::isVisited(CPoint pos)
{
	return visited[pos.x][pos.y];
}

bool EzWeiQiDomain::isVisited(int x, int y)
{
	return visited[x][y];
}
//////////////////////////////////////////////////////////////////////////
// BFS functions
void EzWeiQiDomain::refreshDomain()
{
	/*
	case 1:���i�IĲ����
		��mark��:
			�u�ݶ¦�
		�Amark��:
			�Y�¥]�t��:
				�h�¤��֦��ӻ��a
			�Y�ե]�t��:
				�h�դ��֦��ӻ��a

	case 2:�i�IĲ����
		����domain���Ů���
		�Y�Ů���==�H�Y�C�⬰���ɩҺ��X���Ů���
			�h��domain�ݩ����C��
	*/

	initialDomain();
	//<! First mark color black
	//mark inner of black chess(ignoring white pieces)
	initialVisited();
	for(int i=0; i<EZWEIQI_EDGE; ++i){
		for(int j=0; j<EZWEIQI_EDGE; ++j){
			if(!isVisited(i, j) && !m_pBoard->isBlack(i, j) && isInnerDomain(PLAYERCOLOR_BLACK, CPoint(i, j)))
				markDomain(DOMAINCOLOR_BLACK, CPoint(i, j));
		}
	}
	//<! Second mark color white and will check having problem
	initialVisited();
	for(int i=0; i<EZWEIQI_EDGE; ++i){
		for(int j=0; j<EZWEIQI_EDGE; ++j){
			if(!isVisited(i, j) && !m_pBoard->isWhite(i, j) && isInnerDomain(PLAYERCOLOR_WHITE, CPoint(i, j))){
				if(getDomainType(i,j) == DOMAINCOLOR_BLACK) //is both black inner and white inner
					markDomain(DOMAINCOLOR_EMPTY, CPoint(i, j));
				if(!hasDomainInside(DOMAINCOLOR_BLACK, CPoint(i, j)))
					markDomain(DOMAINCOLOR_WHITE, CPoint(i, j));
			}
		}
	}
	//<! Third mark color
	for(int i=0; i<EZWEIQI_EDGE; ++i){
		for(int j=0; j<EZWEIQI_EDGE; ++j){
			//if pos is not a piece and undecided
			if(getDomainType(i, j) == DOMAINCOLOR_UNKNOWN && !m_pBoard->isPiece(i, j)){
				int allEmptySize = getAllEmptySize(); //calc space that are not pieces
				initialVisited();
				//get domain size, boundary = all piece
				int unknownDomainSize = domainSize(PLAYERCOLOR_PIECE, CPoint(i, j));
				if(allEmptySize != unknownDomainSize){						// Someone has its own domain
					initialVisited();
					if(domainSize(PLAYERCOLOR_BLACK, CPoint(i, j)) == unknownDomainSize){	// Black has the domain
						markDomain(DOMAINCOLOR_BLACK, CPoint(i, j));
						continue;
					}
					initialVisited();
					if(domainSize(PLAYERCOLOR_WHITE, CPoint(i, j)) == unknownDomainSize){	// White has the domain
						markDomain(DOMAINCOLOR_WHITE, CPoint(i, j));
						continue;
					}
				}
				markDomain(DOMAINCOLOR_EMPTY, CPoint(i, j));			// No on has the domain
			}
		}
	}
}

void EzWeiQiDomain::markDomain(DomainColor domainColor, CPoint pos)
	//set bfs points satart at pos to domaincolor, bondary = unknownDomainSize piece + black peice(not dead)+ white piece(not dead)
	int iXShift[4] = {1, -1, 0, 0};
	int iYShift[4] = {0, 0, 1, -1};
	list<CPoint> queue;									// Create a queue for BFS
	CPoint currentPos;									// A CPoint used to stroe current vertex during BFS
	DomainColor originColor = getDomainType(pos);

	setDomainType(pos, domainColor);					// Set Domain Type as domainColor
	queue.push_back(pos);								// Enqueue the current vertex
	while(!queue.empty()){
		currentPos = queue.front();						// Dequeue a vertex from queue
		queue.pop_front();
		for(int i=0; i<4; ++i){
			if(isValid(currentPos.x+iXShift[i], currentPos.y+iYShift[i])){
				if(getDomainType(currentPos.x+iXShift[i], currentPos.y+iYShift[i]) != originColor)		// if is originColor then replace with domainColor
					continue;
				if(originColor == DOMAINCOLOR_UNKNOWN && m_pBoard->isPiece(currentPos.x+iXShift[i], currentPos.y+iYShift[i]))
					continue;
				if(domainColor == DOMAINCOLOR_BLACK && m_pBoard->isBlack(currentPos.x+iXShift[i], currentPos.y+iYShift[i]) ||
					domainColor == DOMAINCOLOR_WHITE && m_pBoard->isWhite(currentPos.x+iXShift[i], currentPos.y+iYShift[i]))
					continue;

				setDomainType(currentPos.x+iXShift[i], currentPos.y+iYShift[i], domainColor);
				queue.push_back(CPoint(currentPos.x+iXShift[i], currentPos.y+iYShift[i]));
			}
		}
	}
}

bool  EzWeiQiDomain::hasDomainInside(DomainColor domainColor, CPoint pos)
{
	//has "DOMAINCOLOR" around pos, boundary = board color == opposite of domainColor
	int iXShift[4] = {1, -1, 0, 0};
	int iYShift[4] = {0, 0, 1, -1};
	bool iResult = false;
	list<CPoint> queue;									// Create a queue for BFS
	CPoint currentPos;									// A CPoint used to stroe current vertex during BFS

	setVisited(pos);									// Mark the current vertex as visited
	queue.push_back(pos);								// Enqueue the current vertex
	while(!queue.empty()){
		currentPos = queue.front();						// Dequeue a vertex from queue
		queue.pop_front();
		if(getDomainType(currentPos) == domainColor)
			iResult = true;
		for(int i=0; i<4; ++i){
			if(!isValid(currentPos.x+iXShift[i], currentPos.y+iYShift[i]))		// If touch the edge then continue
				continue;
			if(isVisited(currentPos.x+iXShift[i], currentPos.y+iYShift[i]) == false &&
				!(domainColor == DOMAINCOLOR_BLACK && m_pBoard->isWhite(currentPos.x+iXShift[i], currentPos.y+iYShift[i]) ||
				  domainColor == DOMAINCOLOR_WHITE && m_pBoard->isBlack(currentPos.x+iXShift[i], currentPos.y+iYShift[i])))
			{
				queue.push_back(CPoint(currentPos.x+iXShift[i], currentPos.y+iYShift[i]));
				setVisited(CPoint(currentPos.x+iXShift[i], currentPos.y+iYShift[i]));
			}
		}
	}
	return iResult;
}

int EzWeiQiDomain::domainSize(PlayerColor playerColor, CPoint pos)
{
	//calculate domain size with boundary = (board's)black/white/peice, start at position
	int iXShift[4] = {1, -1, 0, 0};
	int iYShift[4] = {0, 0, 1, -1};
	int iDomainSize = 0;
	list<CPoint> queue;									// Create a queue for BFS
	CPoint currentPos;									// A CPoint used to stroe current vertex during BFS

	setVisited(pos);									// Mark the current vertex as visited
	queue.push_back(pos);								// Enqueue the current vertex
	while(!queue.empty()){
		currentPos = queue.front();						// Dequeue a vertex from queue
		queue.pop_front();
		if(!m_pBoard->isPiece(currentPos.x, currentPos.y))
			iDomainSize++;
		for(int i=0; i<4; ++i){
			if(!isValid(currentPos.x+iXShift[i], currentPos.y+iYShift[i]))		// If touch the edge then continue
				continue;
			if(isVisited(currentPos.x+iXShift[i], currentPos.y+iYShift[i]) == false &&
				!(playerColor == PLAYERCOLOR_BLACK && m_pBoard->isBlack(currentPos.x+iXShift[i], currentPos.y+iYShift[i]) ||
				  playerColor == PLAYERCOLOR_WHITE && m_pBoard->isWhite(currentPos.x+iXShift[i], currentPos.y+iYShift[i]) ||
				  playerColor == PLAYERCOLOR_PIECE && m_pBoard->isPiece(currentPos.x+iXShift[i], currentPos.y+iYShift[i])))
			{
				queue.push_back(CPoint(currentPos.x+iXShift[i], currentPos.y+iYShift[i]));
				setVisited(CPoint(currentPos.x+iXShift[i], currentPos.y+iYShift[i]));
			}
		}
	}
	return iDomainSize;
}
bool EzWeiQiDomain::isInnerDomain(PlayerColor playerColor, CPoint pos)
{
	//bfs, boundary = boardcolor is playerColor, if touch edge=>true, else return false
	int iXShift[4] = {1, -1, 0, 0};
	int iYShift[4] = {0, 0, 1, -1};
	bool iResult = true;
	list<CPoint> queue;									// Create a queue for BFS
	CPoint currentPos;									// A CPoint used to stroe current vertex during BFS

	setVisited(pos);									// Mark the current vertex as visited
	queue.push_back(pos);								// Enqueue the current vertex
	while(!queue.empty()){
		currentPos = queue.front();						// Dequeue a vertex from queue
		queue.pop_front();
		for(int i=0; i<4; ++i){
			if(!isValid(currentPos.x+iXShift[i], currentPos.y+iYShift[i])){		// If touch the edge then return false
				iResult = false;
				continue;
			}
			if(isVisited(currentPos.x+iXShift[i], currentPos.y+iYShift[i]) == false &&
				!(playerColor == PLAYERCOLOR_BLACK && m_pBoard->isBlack(currentPos.x+iXShift[i], currentPos.y+iYShift[i]) ||
				  playerColor == PLAYERCOLOR_WHITE && m_pBoard->isWhite(currentPos.x+iXShift[i], currentPos.y+iYShift[i])))
			{
				queue.push_back(CPoint(currentPos.x+iXShift[i], currentPos.y+iYShift[i]));
				setVisited(CPoint(currentPos.x+iXShift[i], currentPos.y+iYShift[i]));
			}
		}
	}
	return iResult;
}
int EzWeiQiDomain::getAllEmptySize()
{
	int iEmptyCount = 0;
	for(int i=0; i<EZWEIQI_EDGE; ++i)
		for(int j=0; j<EZWEIQI_EDGE; ++j){
			if(!m_pBoard->isPiece(i, j))
				iEmptyCount++;
		}
		return iEmptyCount;
}
