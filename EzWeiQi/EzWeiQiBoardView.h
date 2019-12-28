#ifndef __EZWEIQIBOARDVIEW_H__
#define __EZWEIQIBOARDVIEW_H__

#include "BaseBoardView.h"
#include "BaseNode.h"
#include "EzWeiQiBoard.h"
#include "EzWeiQiDomain.h"
#include <algorithm>



/*!
Board view of EzWeiQi
@author Ivan
@ingroup EzWeiQiGroup
*/
class EzWeiQiBoardView : public BaseBoardView
{
	DECLARE_DYNCREATE(EzWeiQiBoardView)
	DECLARE_MESSAGE_MAP()
public:

	int m_iPieceSize;								//!< pixel size of a single piece
	int m_iVariantSize;								//!< pixel size of a variant
	int m_iXOffset;									//!< board line offset of x-axis from left
	int m_iYOffset;									//!< board line offset of y-axis from top
	int m_nLines;									//!< board line

	/*!
	Constructor
	@author Ivan
	*/
	EzWeiQiBoardView();
	/*!
	Destructor
	@author Ivan
	*/
	~EzWeiQiBoardView();
	//////////////////////////////////////////////////////////////////////////
	// Drawing functions
	/*!
	draw EzWeiQi board, override the function of base class.
	@param pDC MFC Paint Object
	@author Ivan
	*/
	void drawBoard(CDC* pDC);
	/*!
	draw all the index of rows and columns
	@param pDC MFC Paint Object
	@author kk
	*/
	void drawAllIndex(CDC* pDC);
	/*!
	draw the pieces and variants on the board
	@param pDC MFC Paint Object
	@author Ivan
	*/
	void drawBoardContent(CDC* pDC);
	/*!
	override base function.
	@author kk
	*/
	void getBoardArea(CRect &rect);
	//////////////////////////////////////////////////////////////////////////
	// Conversion functions
	/*!
	check whether the point(x, y) is inside the legal region of the board 
	@param x x-axis index (0 to (number of lines - 1))
	@param y y-axis index (0 to (number of lines - 1))
	@return bool whether the point(x, y) is inside the legal region of the board
	@author kk
	*/
	bool isValid(int x, int y);
	/*!
	transform the point of true pixel coordinate to the board coordinate
	@param devicePoint true pixel coordinate
	@return CPoint the board coordinate
	@author kk
	*/
	virtual CPoint transformToBoardPoint(const CPoint& logicPoint);
	/*!
	get the local coordinate of the corresponding position
	@param devicePoint the board coordinate
	@return CPoint true pixel coordinate
	@author kk
	*/
	virtual CPoint transformToDevicePoint(const CPoint& boardPoint);	
	/*!
	Function to click on left button
	@author Ivan
	*/
	virtual void clickLeft(UINT nFlags, CPoint point);
	/*!
	Function to click on right button
	@author Ivan
	*/
	virtual void clickRight(UINT nFlags, CPoint point);
	/*!
	Function to register draw functions
	@author Ivan
	*/
	void registerDrawFunction();
	/*!
	Function to draw baseboard functions
	@param pDC MFC Paint Object
	@author Ivan
	*/
	void drawBaseBoardFunction(CDC* pDC);
	/*!
	Function to draw piecese
	@param pDC MFC Paint Object
	@author Ivan
	*/
	void drawPiecese(CDC* pDC);
	/*!
	Function to draw piecese border
	@param pDC MFC Paint Object
	@param x_offset start x
	@param y_offset start y
	@author Ivan
	*/
	void drawPieceBorder(CDC* pDC, int x_offset, int y_offset);
	/*!
	Function to draw Domain
	@param pDC MFC Paint Object
	@author Ivan
	*/
	void drawDomain(CDC* pDC );
	/*!
	Function to find the Domain is belongs to whom by BFS
	@param pDomain used to store who has the domain at each point
	@param x x_axis
	@param y y_axis
	@author Ivan
	*/
	int findDomainColorBFS(DomainColor **pDomain, int x, int y);
	/*!
	Function to find whether the color can contain the domain or not
	@param visited store visited state to avoid compute the same piece again
	@param iColor the player color
	@param x x_axis
	@param y y_axis
	@author Ivan
	*/
	bool checkHasDomainBFS( bool **visited, PlayerColor iColor, int x, int y);
};

#endif