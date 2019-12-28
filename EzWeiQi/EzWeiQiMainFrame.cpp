#include "stdafx.h"
#include "EzWeiQiMainFrame.h"

IMPLEMENT_DYNCREATE(EzWeiQiMainFrame, CBaseMainFrame)

BEGIN_MESSAGE_MAP(EzWeiQiMainFrame, CBaseMainFrame)
END_MESSAGE_MAP()

EzWeiQiMainFrame::EzWeiQiMainFrame() : CBaseMainFrame()
{

}
EzWeiQiMainFrame::~EzWeiQiMainFrame()
{

}

void EzWeiQiMainFrame::registerDefaultBars()
{
	CBaseMainFrame::registerDefaultBars();
	//register the toolbar only for this game
  registerBar( &m_toolBarDemo, "barGroup", "tictactoe" );
  registerBar(&m_toolBarPosition,  "EzWeiQi", "position");
}