#ifndef __EZWEIQITREEVIEW_H__
#define __EZWEIQITREEVIEW_H__

#include "BaseTreeView.h"
/*!
tree view of EzWeiQi
@author Ivan
@ingroup EzWeiQiGroup
*/
class EzWeiQiTreeView : public BaseTreeView
{
	DECLARE_DYNCREATE(EzWeiQiTreeView)
	DECLARE_MESSAGE_MAP()
public:
	/*!
	Constructor
	@author Ivan
	*/
	EzWeiQiTreeView();
	/*!
	Destructor
	@author Ivan
	*/
	~EzWeiQiTreeView();
	/*!
	get the title of hTreeItem of that node
	@author kk
	*/
	CString getText(BaseTree::NodePointer node) const;
	/*!
	override insertMoveNode to setup my tree view
	@author Ivan
	*/
	void insertMoveNode(BaseTree::NodePointer parent, BaseTree::NodePointer child, bool bIsSelect);
};

#endif