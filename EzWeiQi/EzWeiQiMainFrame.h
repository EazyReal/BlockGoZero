#ifndef __EZWEIQIMAINFRAME_H__
#define __EZWEIQIMAINFRAME_H__
#include "BaseMainFrm.h"

#include "ToolBarDemo.h"
/*!
Main Frame of EzWeiQi
It does not implement any new toolbar
@author Ivan
@ingroup EzWeiQiGroup
*/
class EzWeiQiMainFrame : public CBaseMainFrame
{
	DECLARE_DYNCREATE(EzWeiQiMainFrame)
	DECLARE_MESSAGE_MAP()
protected:
	EzWeiQiMainFrame();
public:
	virtual ~EzWeiQiMainFrame();
	virtual void registerDefaultBars();

protected:
  ToolBarDemo m_toolBarDemo;
  ToolBarPosition m_toolBarPosition;
};
#endif