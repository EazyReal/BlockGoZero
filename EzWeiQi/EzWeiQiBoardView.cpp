#include "StdAfx.h"
#include "EzWeiQiBoardView.h"
#include "EzWeiQiEditorDoc.h"

IMPLEMENT_DYNCREATE(EzWeiQiBoardView, BaseBoardView)

BEGIN_MESSAGE_MAP(EzWeiQiBoardView, BaseBoardView)
	
END_MESSAGE_MAP()

EzWeiQiBoardView::EzWeiQiBoardView()
:BaseBoardView(EZWEIQI_LINES, EZWEIQI_LINES, 100)
{
	//set somed environment value
	m_nLines = EZWEIQI_LINES;
	m_iVariantSize = 10;
	m_iPieceSize = EZWEIQI_PIECE_SIZE;
	m_iXOffset = EZWEIQI_BOARD_X_OFFSET;
	m_iYOffset = EZWEIQI_BOARD_Y_OFFSET;

	m_iXFrom = 1;	
	m_iXTo = m_nLines;				
	m_iYFrom = 1;	
	m_iYTo = m_nLines;
	
	//register draw functions	
	registerDrawFunction();
	
	setSymbolSizeX(EZWEIQI_PIECE_SIZE);
	setSymbolSizeY(EZWEIQI_PIECE_SIZE);
}
EzWeiQiBoardView::~EzWeiQiBoardView()
{
}

//////////////////////////////////////////////////////////////////////////
// Drawing functions
void EzWeiQiBoardView::drawAllIndex(CDC* pDC)
{
	CFont font; 	
	font.CreatePointFont(EZWEIQI_FONT_SIZE * 10, "Arial"); 

	for(int i = 0;i < m_nLines - 1; i++){
		CString sRowIndex;	sRowIndex.Format("%2d", m_nLines - 1 - i);
		CString sColIndex;	sColIndex.Format("%c", (i < 26) ? ('A' + i) : ('1' + i - 26));

		//calculate x and y position of Row Coordinate, draw text 1 - 19
		int iRowX = m_iXOffset - m_iPieceSize / 2;
		int iRowY = m_iYOffset + m_iPieceSize * i + m_iPieceSize / 2;
		drawText( pDC, iRowX, iRowY, m_iPieceSize, &font, sRowIndex, COLOR_BLACK);

		//calculate x and y position of Column Coordinate, draw text A - Z
		int iColX = m_iXOffset + m_iPieceSize * i + m_iPieceSize / 2;
		int iColY = m_iYOffset + m_iPieceSize * (m_nLines - 1) + m_iPieceSize / 2;
		drawText( pDC, iColX, iColY, m_iPieceSize, &font, sColIndex, COLOR_BLACK);
	}
}

void EzWeiQiBoardView::drawBoardContent(CDC* pDC)
{

	CFont font; 	
	font.CreatePointFont(EZWEIQI_FONT_SIZE * 10, "Arial"); 
	CString sSequence;	

	//draw legal region pieces
	for(int i =-1;i < m_nLines; i++){
		for(int j = -1;j < m_nLines; j++){
			if(dynamic_cast<EzWeiQiEditorDoc*>(GetDocument())->getBoard().getType(i, j) == EZWEIQIGRIDTYPE_EMPTY) continue;
			
			CPoint transformedPos = CPoint(i, j);
			dynamic_cast<EzWeiQiEditorDoc*>(GetDocument())->getBoard().transformFromRealToTransformed(transformedPos);
			//center coordinate of this grid
			CPoint midPoint = transformToDevicePoint(transformedPos);
			//upper-left coordinate of this grid
			CPoint LUpoint = transformToDevicePoint(transformedPos) - CPoint( m_iPieceSize / 2, m_iPieceSize / 2 );
			//check the type of this grid
			int iType = dynamic_cast<EzWeiQiEditorDoc*>(GetDocument())->getBoard().getType(i, j);
			bool bIsVariant = ((iType & EZWEIQIGRIDTYPE_VARIANT) != 0);
			bool bIsVariantCandidate = ((iType & EZWEIQIGRIDTYPE_VARIANT_CANDIDATE) != 0);

			//is variant
			if(bIsVariant){
				CPoint variantPos = CPoint( midPoint.x - m_iVariantSize / 2,  midPoint.y - m_iVariantSize / 2);
				if(iType & EZWEIQIGRIDTYPE_BLACK)
					drawOnePiece(pDC, variantPos.x, variantPos.y, COLOR_BLUE, m_iVariantSize);	//draw variant
				if(iType & EZWEIQIGRIDTYPE_WHITE)
					drawOnePiece(pDC, variantPos.x, variantPos.y, COLOR_RED, m_iVariantSize);	//draw variant
			}
			else if(bIsVariantCandidate){
				CPoint variantPos = CPoint( midPoint.x - m_iVariantSize / 2,  midPoint.y - m_iVariantSize / 2);
				drawOnePiece(pDC, variantPos.x, variantPos.y, COLOR_GRAY, m_iVariantSize);	//draw variant candidate
			}
			else{
				if(iType & EZWEIQIGRIDTYPE_BLACK)
					drawRectangle(pDC, LUpoint.x, LUpoint.y, EZWEIQI_PIECE_SIZE, EZWEIQI_PIECE_SIZE, COLOR_WHITE, COLOR_BLUE);
				if(iType & EZWEIQIGRIDTYPE_WHITE)
					drawRectangle(pDC, LUpoint.x, LUpoint.y, EZWEIQI_PIECE_SIZE, EZWEIQI_PIECE_SIZE, COLOR_WHITE, COLOR_RED);
				if(iType & EZWEIQIGRIDTYPE_PIECE){
					if(dynamic_cast<EzWeiQiEditorDoc*>(GetDocument())->getBoard().getPointer(i, j).isNotNull()){
						//string of sequence number
						CString sSequence;	
						sSequence.Format("%d", dynamic_cast<EzWeiQiEditorDoc*>(GetDocument())->getBoard().getPointer(i, j).getSequence());
						drawText( pDC, midPoint.x, midPoint.y, m_iPieceSize, &font, sSequence, COLOR_BLACK);
					}
					if(iType & EZWEIQIGRIDTYPE_CURRENT){
						//draw rectangle broder
						CBrush myBrush(COLOR_WHITE);
						for(int i = 1; i <= 3; i++)
							pDC->FrameRect( CRect(LUpoint.x + i, LUpoint.y + i, LUpoint.x + m_iPieceSize - i, LUpoint.y + m_iPieceSize - i), &myBrush);
					}
				}
			}
		}
	}
	//draw out-of-bound shift tag piece at (...)
}

void EzWeiQiBoardView::drawBoard(CDC* pDC)
{
}
//////////////////////////////////////////////////////////////////////////
// Conversion functions
bool EzWeiQiBoardView::isValid(int x, int y)
{
	if(x < 0 || y < 0 || x >= m_nLines - 1 || y >= m_nLines - 1) return false;
	return true;
}

CPoint EzWeiQiBoardView::transformToDevicePoint(const CPoint& boardPoint)
{
	//由board grid座標轉成中心的pixel座標
	return CPoint(m_iXOffset + boardPoint.x * m_iPieceSize + m_iPieceSize / 2, 
		m_iYOffset + (m_nLines - boardPoint.y - 2) * m_iPieceSize + m_iPieceSize / 2);
}
CPoint EzWeiQiBoardView::transformToBoardPoint(const CPoint& logicPoint)
{
	//由pixel座標轉成board grid座標
	int x = logicPoint.x - m_iXOffset;
	int y = logicPoint.y - m_iYOffset;
	x = (x > 0) ? (x) : (x - m_iPieceSize);	//考慮負數的四捨五入
	y = (y > 0) ? (y) : (y - m_iPieceSize);	//考慮負數的四捨五入
	x = x / m_iPieceSize;
	y = y / m_iPieceSize;
	y = (m_nLines - 2) - y ;
	return CPoint(x, y);
}

void EzWeiQiBoardView::getBoardArea( CRect &rect )
{
	rect.left = m_iPieceSize * (m_iXFrom - 1);
	rect.right = m_iXOffset + m_iPieceSize * (m_iXTo + 1) - m_iPieceSize / 2;
	rect.top = m_iPieceSize * (m_nLines - m_iYFrom + 3);
	rect.bottom = m_iYOffset + m_iPieceSize * (m_nLines - m_iYTo - 1);
}
void EzWeiQiBoardView::clickLeft(UINT nFlags, CPoint point)
{
	EzWeiQiEditorDoc* pDoc = dynamic_cast<EzWeiQiEditorDoc*>(GetDocument());
	//<! if is selecting the piece then refresh the views and simple return
	if(pDoc->checkSelect(point) == true){
		pDoc->getBoard().updateVariants(pDoc->getCurrentNode());
		Invalidate();//refresh the views
		return;
	}
	//<! not selecting then do add one piece
	CPoint boardPoint = transformToBoardPoint(point);
	pDoc->getBoard().transformFromTransformedToReal(boardPoint);
	pDoc->addOnePiece(boardPoint);
}

void EzWeiQiBoardView::clickRight(UINT nFlags, CPoint point)
{
	EzWeiQiEditorDoc* pDoc = dynamic_cast<EzWeiQiEditorDoc*>(GetDocument());
	CPoint boardPoint = transformToBoardPoint(point);
	pDoc->getBoard().transformFromTransformedToReal(boardPoint);
	pDoc->goToPreviousPieceWithMouse(boardPoint);
}

void EzWeiQiBoardView::registerDrawFunction()
{	
	BaseBoardView::addZOrder<EzWeiQiBoardView>(1,this,&EzWeiQiBoardView::drawBaseBoardFunction);
	BaseBoardView::addZOrder<EzWeiQiBoardView>(50,this,&EzWeiQiBoardView::drawBoardContent);
}

void EzWeiQiBoardView::drawBaseBoardFunction(CDC* pDC)
{

	//draw board, lines and coordinate index	
	drawSquareBoardImage(pDC, m_iPieceSize, m_nLines+11);
	drawSquareLines(pDC, m_iPieceSize, m_iXOffset, m_iYOffset, m_nLines, COLOR_WHITE);
	drawAllIndex(pDC);
	
	//draw domain(who has it)
	if(dynamic_cast<EzWeiQiEditorDoc*>(GetDocument())->getMoveList().getIndex() > 1)
		drawDomain(pDC);

	//draw four start point
	drawRectangle(pDC, m_iXOffset+EZWEIQI_PIECE_SIZE*3, m_iYOffset+EZWEIQI_PIECE_SIZE*3, EZWEIQI_PIECE_SIZE, EZWEIQI_PIECE_SIZE, COLOR_WHITE, COLOR_WHITE);
	drawRectangle(pDC, m_iXOffset+EZWEIQI_PIECE_SIZE*9, m_iYOffset+EZWEIQI_PIECE_SIZE*3, EZWEIQI_PIECE_SIZE, EZWEIQI_PIECE_SIZE, COLOR_WHITE, COLOR_WHITE);
	drawRectangle(pDC, m_iXOffset+EZWEIQI_PIECE_SIZE*3, m_iYOffset+EZWEIQI_PIECE_SIZE*9, EZWEIQI_PIECE_SIZE, EZWEIQI_PIECE_SIZE, COLOR_WHITE, COLOR_WHITE);
	drawRectangle(pDC, m_iXOffset+EZWEIQI_PIECE_SIZE*9, m_iYOffset+EZWEIQI_PIECE_SIZE*9, EZWEIQI_PIECE_SIZE, EZWEIQI_PIECE_SIZE, COLOR_WHITE, COLOR_WHITE);

	//draw nine piecese
	drawPieceBorder(pDC, (int)(EZWEIQI_PIECE_SIZE*0.3), (int)(EZWEIQI_PIECE_SIZE*0.9));
	drawPieceBorder(pDC, (int)(EZWEIQI_PIECE_SIZE*20.5), (int)(EZWEIQI_PIECE_SIZE*0.9));
	drawPiecese(pDC);		
	
}

void EzWeiQiBoardView::drawPiecese( CDC* pDC )
{
	EzWeiQiEditorDoc* pDoc = dynamic_cast<EzWeiQiEditorDoc*>(GetDocument());
	int iPlayerColors[2] = {PLAYERCOLOR_BLACK, PLAYERCOLOR_WHITE};
	
	for(int i = 0; i<2; i++){
		EzWeiQiPiece** pPieceses =  pDoc->getBoard().getEzWeiQiPiecese(iPlayerColors[i]);//PLAYERCOLOR_BLACK , PLAYERCOLOR_WHITE 
		int x_axis;
		int y_axis;
		COLORREF iPieceColor;

		if(iPlayerColors[i] == PLAYERCOLOR_BLACK)									//set color, black == red, white == blue
			iPieceColor = COLOR_BLUE;
		else if(iPlayerColors[i] == PLAYERCOLOR_WHITE)
			iPieceColor = COLOR_RED;

		for(int j = 0; j<EZWEIQI_PIECE_NUM; j++){
			if(pPieceses[j]->isUsed == true)
				continue;
			for(int k = 0; k<4; k++){
				x_axis = pPieceses[j]->locations[k].x;
				y_axis = pPieceses[j]->locations[k].y;
				drawRectangle(pDC, x_axis, y_axis, EZWEIQI_OUTBOARD_PIECE_SIZE, EZWEIQI_OUTBOARD_PIECE_SIZE, COLOR_WHITE, iPieceColor);
				if(k==0 && pPieceses[j]->isSelected == true){
					CBrush myBrush(COLOR_WHITE);
					for(int i = 1; i <= 3; i++)
						pDC->FrameRect( CRect(pPieceses[j]->locations[0].x + i, pPieceses[j]->locations[0].y + i,
							pPieceses[j]->locations[0].x + EZWEIQI_OUTBOARD_PIECE_SIZE - i, pPieceses[j]->locations[0].y + EZWEIQI_OUTBOARD_PIECE_SIZE - i), &myBrush);
				}
				if(j>=19) break;
			}
			if(pPieceses[j]->isInSelecteList == true){
				int iOriginPointX = j%3;
				int iOriginPointY = (j/3)%7;
				CPen *pOriPen = pDC->GetCurrentPen();
				CPen linePen(PS_SOLID, 2, COLOR_GREEN);
				pDC->SelectObject(&linePen);
				if(pPieceses[j]->getColor() == PLAYERCOLOR_BLACK){
					//draw vertical board lines (Columns)
					pDC->MoveTo((int)(iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.3+3), (int)(iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9+3));
					pDC->LineTo((int)(iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.3+3), (int)((iOriginPointY+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9-3));
					pDC->MoveTo((int)((iOriginPointX+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.3-3), (int)(iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9+3));
					pDC->LineTo((int)((iOriginPointX+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.3-3), (int)((iOriginPointY+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9-3));
					//draw horizontal board lines (Rows)
					pDC->MoveTo((int)(iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.3+3), (int)(iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9+3));
					pDC->LineTo((int)((iOriginPointX+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.3-3), (int)(iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9+3));
					pDC->MoveTo((int)(iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.3+3), (int)((iOriginPointY+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9-3));
					pDC->LineTo((int)((iOriginPointX+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.3-3), (int)((iOriginPointY+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9-3));
				}
				else if(pPieceses[j]->getColor() == PLAYERCOLOR_WHITE){
					//draw vertical board lines (Columns)
					pDC->MoveTo((int)(iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*20.5+3), (int)(iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9+3));
					pDC->LineTo((int)(iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*20.5+3), (int)((iOriginPointY+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9-3));
					pDC->MoveTo((int)((iOriginPointX+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*20.5-3), (int)(iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9+3));
					pDC->LineTo((int)((iOriginPointX+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*20.5-3), (int)((iOriginPointY+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9-3));
					//draw horizontal board lines (Rows)
					pDC->MoveTo((int)(iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*20.5+3), (int)(iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9+3));
					pDC->LineTo((int)((iOriginPointX+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*20.5-3), (int)(iOriginPointY*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9+3));
					pDC->MoveTo((int)(iOriginPointX*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*20.5+3), (int)((iOriginPointY+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9-3));
					pDC->LineTo((int)((iOriginPointX+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*20.5-3), (int)((iOriginPointY+1)*EZWEIQI_OUTBOARD_PIECE_SIZE*4.5 + EZWEIQI_PIECE_SIZE*0.9-3));
				}
				pDC->SelectObject(&pOriPen);
			}
		}
	}
}

void EzWeiQiBoardView::drawPieceBorder(CDC* pDC, int x_offset, int y_offset)
{
	CPen *pOriPen = pDC->GetCurrentPen();
	CPen linePen(PS_SOLID, 2, COLOR_WHITE);
	pDC->SelectObject(&linePen);
	//draw vertical board lines (Columns)
	pDC->MoveTo((int)(x_offset), y_offset);
	pDC->LineTo((int)(x_offset), y_offset+63*7);
	pDC->MoveTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*1), y_offset+63*6);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*1), y_offset+63*7);
	pDC->MoveTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*2), y_offset+63*0);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*2), y_offset+63*1);
	pDC->MoveTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*2), y_offset+63*2);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*2), y_offset+63*3);
	pDC->MoveTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*2), y_offset+63*4);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*2), y_offset+63*5);
	pDC->MoveTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*2), y_offset+63*6);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*2), y_offset+63*7);
	pDC->MoveTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*3), y_offset);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*3), y_offset+63*7);
	
	//draw horizontal board lines (Rows)
	pDC->MoveTo((int)(x_offset), y_offset);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*3), y_offset);
	pDC->MoveTo((int)(x_offset), y_offset+63*1);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*2), y_offset+63*1);
	pDC->MoveTo((int)(x_offset), y_offset+63*2);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*3), y_offset+63*2);
	pDC->MoveTo((int)(x_offset), y_offset+63*3);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*2), y_offset+63*3);
	pDC->MoveTo((int)(x_offset), y_offset+63*4);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*3), y_offset+63*4);
	pDC->MoveTo((int)(x_offset), y_offset+63*5);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*2), y_offset+63*5);
	pDC->MoveTo((int)(x_offset), y_offset+63*6);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*3), y_offset+63*6);
	pDC->MoveTo((int)(x_offset), y_offset+63*7);
	pDC->LineTo((int)(x_offset+EZWEIQI_OUTBOARD_PIECE_SIZE*4.5*3), y_offset+63*7);
	pDC->SelectObject(&pOriPen);
}

void EzWeiQiBoardView::drawDomain(CDC* pDC)
{
	static EzWeiQiDomain *m_domain = new EzWeiQiDomain(&(dynamic_cast<EzWeiQiEditorDoc*>(GetDocument()))->getBoard());

	m_domain->refreshDomain();
	//<! Draw domain color
	COLORREF COLOR_LIGHT_BLUE		= RGB(150, 150, 255);
	COLORREF COLOR_LIGHT_RED		= RGB(255, 150, 150);
	for(int i=0; i<EZWEIQI_EDGE; ++i){
		for(int j=0; j<EZWEIQI_EDGE; ++j){
			CPoint pos = transformToDevicePoint(CPoint(i, j)) - CPoint( m_iPieceSize / 2, m_iPieceSize / 2 );
			if(m_domain->getDomainType(i, j) == DOMAINCOLOR_BLACK)
				drawRectangle(pDC, pos.x, pos.y, m_iPieceSize, m_iPieceSize, COLOR_WHITE, COLOR_LIGHT_BLUE);
			else if(m_domain->getDomainType(i, j) == DOMAINCOLOR_WHITE)
				drawRectangle(pDC, pos.x, pos.y, m_iPieceSize, m_iPieceSize, COLOR_WHITE, COLOR_LIGHT_RED);
		}
	}
}