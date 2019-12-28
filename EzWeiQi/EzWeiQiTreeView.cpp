#include "stdafx.h"
#include "EzWeiQiTreeView.h"
#include "EzWeiQiPiece.h"

IMPLEMENT_DYNCREATE(EzWeiQiTreeView, BaseTreeView)

BEGIN_MESSAGE_MAP(EzWeiQiTreeView, BaseTreeView)
END_MESSAGE_MAP()

EzWeiQiTreeView::EzWeiQiTreeView()
{
	// add our game pieces
	CBitmap	bitmapTreeBlueType[8];
	CBitmap	bitmapTreeRedType[8];
	bitmapTreeBlueType[0].LoadBitmap( IDB_EZWEIQI_TREE_BLUE_TYPE0 );
	bitmapTreeBlueType[1].LoadBitmap( IDB_EZWEIQI_TREE_BLUE_TYPE1 );
	bitmapTreeBlueType[2].LoadBitmap( IDB_EZWEIQI_TREE_BLUE_TYPE2 );
	bitmapTreeBlueType[3].LoadBitmap( IDB_EZWEIQI_TREE_BLUE_TYPE3 );
	bitmapTreeBlueType[4].LoadBitmap( IDB_EZWEIQI_TREE_BLUE_TYPE4 );
	bitmapTreeBlueType[5].LoadBitmap( IDB_EZWEIQI_TREE_BLUE_TYPE5 );
	bitmapTreeBlueType[6].LoadBitmap( IDB_EZWEIQI_TREE_BLUE_TYPE6 );
	bitmapTreeBlueType[7].LoadBitmap( IDB_EZWEIQI_TREE_BLUE_TYPE7 );
	bitmapTreeRedType[0].LoadBitmap( IDB_EZWEIQI_TREE_RED_TYPE0 );
	bitmapTreeRedType[1].LoadBitmap( IDB_EZWEIQI_TREE_RED_TYPE1 );
	bitmapTreeRedType[2].LoadBitmap( IDB_EZWEIQI_TREE_RED_TYPE2 );
	bitmapTreeRedType[3].LoadBitmap( IDB_EZWEIQI_TREE_RED_TYPE3 );
	bitmapTreeRedType[4].LoadBitmap( IDB_EZWEIQI_TREE_RED_TYPE4 );
	bitmapTreeRedType[5].LoadBitmap( IDB_EZWEIQI_TREE_RED_TYPE5 );
	bitmapTreeRedType[6].LoadBitmap( IDB_EZWEIQI_TREE_RED_TYPE6 );
	bitmapTreeRedType[7].LoadBitmap( IDB_EZWEIQI_TREE_RED_TYPE7 );
	for(int i=0; i<8; ++i){
		registerImageFiles(bitmapTreeBlueType[i]);
		registerImageFiles(bitmapTreeRedType[i]);
	}
}

EzWeiQiTreeView::~EzWeiQiTreeView()
{

}

CString EzWeiQiTreeView::getText(BaseTree::NodePointer node) const{
	return m_treeCtrl.GetItemText(node.getHtreeItem());
}

void EzWeiQiTreeView::insertMoveNode(BaseTree::NodePointer parent, BaseTree::NodePointer child, bool bIsSelect)
{
	//startUpdateTree();
	// setup position string at the right of image, while this function can be override
	CString sText;
	char x, y;
	CPoint pos = child.getLocation();

	//calculate the coordinate character and format the title of hTreeItem
	if(pos.x < 26)	x = static_cast<char>(pos.x + 'A');
	else					x = static_cast<char>(pos.x - 26 + '1');
	if(pos.y < 26)	y = static_cast<char>(pos.y + 'A');
	else					y = static_cast<char>(pos.y - 26 + '1');
	int degree;
	switch(child.getType()){
	case 0:case 2:case 6:case 8:case 12:case 14:case 18:case 19:case 20:
		degree = 0;
		break;
	case 1:case 3:case 7:case 9:case 13:case 15:
		degree = 90;
		break;
	case 4:case 10:case 16:
		degree = 180;
		break;
	case 5:case 11:case 17:
		degree = 270;
		break;
	}

	sText.Format("%c %2d --- %c %d¢X", 
		x, child.getLocation().y + 1, (child.isBlack()) ? 'B' : 'R', degree);

	// set image id mapping to the color defined in PlayerColor
	// if we set PlayerColor{black, white}, then the mapping image index in CImageList would be 2, 3
	// index 0 = open branch image
	// index 1 = close branch image
	int iPieceType = 0;
	switch(child.getType()){
	case 0:case 1:
		iPieceType = 0;
		break;
	case 2:case 3:case 4:case 5:
		iPieceType = 1;
		break;
	case 6:case 7:
		iPieceType = 2;
		break;
	case 12:case 13:
		iPieceType = 3;
		break;
	case 8:case 9:case 10:case 11:
		iPieceType = 4;
		break;
	case 14:case 15:case 16:case 17:
		iPieceType = 5;
		break;
	case 18:
		iPieceType = 6;
		break;
	case 19:case 20:
		iPieceType = 7;
		break;
	}
	HTREEITEM hItem = insertTreeItem(
		sText, child.getColor() + iPieceType*2 + 2, child.getColor() + iPieceType*2 + 2, 
		parent.getHtreeItem(), TVI_LAST, child, false
		);
	child.setHtreeItem(hItem);
}
