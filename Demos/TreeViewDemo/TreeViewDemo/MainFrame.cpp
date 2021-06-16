#include "StdAfx.h"
#include "MainFrame.h"


CMainFrame::CMainFrame(void)
{
}


CMainFrame::~CMainFrame(void)
{

}

void CMainFrame::InitWindow()
{
	UI_BINDCONTROL(CTreeUI, m_tree, _T("tree_main"));
	
	TNodeData* hRoot = m_tree->InsertNode(_T("营销公司"), _T("images/edtico.png"), TNODE_ROOT, TNODE_LAST);

	TNodeData* h1 = m_tree->InsertNode(_T("研发中心"), hRoot);
	TNodeData* h1_1 = m_tree->InsertNode(_T("研发部"), h1);
 	m_tree->InsertNode(_T("Windows"), h1_1);
	m_tree->InsertNode(_T("Linux嵌入式"), h1_1);
	m_tree->InsertNode(_T("安卓"), h1_1);
	m_tree->InsertNode(_T("WEB前端"), h1_1);
	TNodeData* h1_1_1 = m_tree->InsertNode(_T("算法"), h1_1, TNODE_FIRST);
	m_tree->InsertNode(_T("固件开发"), h1_1, h1_1_1);

 	m_tree->InsertNode(_T("设计部"), h1);
	m_tree->InsertNode(_T("技术部"), h1);

	TNodeData* h2 = m_tree->InsertNode(_T("营销中心"), hRoot);
	m_tree->InsertNode(_T("市场部"), h2);
	m_tree->InsertNode(_T("销售部"), h2);
	m_tree->InsertNode(_T("网销部"), h2);
	m_tree->InsertNode(_T("自营部"), h2);
	m_tree->InsertNode(_T("物流部"), h2);

	TNodeData* h3 = m_tree->InsertNode(_T("财务中心"), hRoot);
	m_tree->InsertNode(_T("财务部"), h3);

	TNodeData* h4 = m_tree->InsertNode(_T("企管中心"), hRoot);
	m_tree->InsertNode(_T("行政部"), h4);
	m_tree->InsertNode(_T("人事部"), h4);
	
	//大数据测试	
	TNodeData* h5 = m_tree->InsertNode(_T("N条数据开始"), hRoot);
	TNodeData* ha = TNODE_LAST;
	for (int a=0; a<100000; a++)
	{
		CDuiString sa;
		sa.Format(_T("a-%04d"), a);
		ha = m_tree->InsertNode(sa, h5, ha);

// 		for (int b=0; b<2; b++)
// 		{
// 			CDuiString sb;
// 			sb.Format(_T("%s  -  b-%04d"), sa.GetData(), b);
// 			TNodeData* hb = m_tree->InsertNode(sb, ha);
// 
// 			for (int c=0; c<2; c++)
// 			{
// 				CDuiString sc;
// 				sc.Format(_T("%s  -  c-%04d"), sb.GetData(), c);
// 				TNodeData* hc = m_tree->InsertNode(sc, hb);
// 
// 				for (int d=0; d<2; d++)
// 				{
// 					CDuiString sd;
// 					sd.Format(_T("%s  -  d-%04d"), sc.GetData(), d);
// 					TNodeData* hd = m_tree->InsertNode(sd, hc);
// 
// 					for (int e=0; e<2; e++)
// 					{
// 						CDuiString se;
// 						se.Format(_T("%s  -  e-%04d"), sd.GetData(), e);
// 						TNodeData* he = m_tree->InsertNode(se, hd);
// 					}
// 				}
// 			}
// 		}
	}
	
	return;
}

bool CMainFrame::OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CMainFrame::OnMenuCommand(const MenuCmd *cmd)
{
	return false;
}

bool CMainFrame::OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI)
{
	return false;
}

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{

}