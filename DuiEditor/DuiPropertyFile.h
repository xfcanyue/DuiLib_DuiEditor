#pragma once
#include "DuiEditorViewDesign.h"

//文件定义的DuiLib属性列表
#define	TREENODETYPE_ACTIVE				0
#define	TREENODETYPE_BUTTON				1
#define	TREENODETYPE_COMBO				2
#define	TREENODETYPE_CONTAINER			3
#define	TREENODETYPE_CONTROL			4
#define	TREENODETYPE_DEFAULT			5
#define	TREENODETYPE_EDIT				6
#define	TREENODETYPE_FONT				7
#define	TREENODETYPE_HORIZONTALLAYOUT	8
#define	TREENODETYPE_LABEL				9
#define	TREENODETYPE_OPTION				10
#define	TREENODETYPE_POINTER			11
#define	TREENODETYPE_PROGRESS			12
#define	TREENODETYPE_SCROLLBAR			13
#define	TREENODETYPE_SLIDER				14
#define	TREENODETYPE_TABLAYOUT			15
#define	TREENODETYPE_TEXT				16
#define	TREENODETYPE_TILELAYOUT			17
#define	TREENODETYPE_UNKNOW				18
#define	TREENODETYPE_VERTICALLAYOUT		19
#define	TREENODETYPE_WINDOW				20
#define	TREENODETYPE_LIST				21
#define	TREENODETYPE_RICHEDIT			22
#define	TREENODETYPE_CHECKBOX			23
#define	TREENODETYPE_GRIDCTRL			24
#define	TREENODETYPE_IEXPLORER			25
#define	TREENODETYPE_LIBCEF				26
#define	TREENODETYPE_TREEVIEW			27
#define	TREENODETYPE_TREENODE			28
#define	TREENODETYPE_DATETIME			29
#define	TREENODETYPE_CUSTOMCONTROL		30
#define	TREENODETYPE_CHILDLAYOUT		31
#define	TREENODETYPE_PICTUREBOX			32
#define	TREENODETYPE_GROUPBOX			33
#define	TREENODETYPE_MENU				34
#define	TREENODETYPE_GIFANIM			35
#define	TREENODETYPE_STYLE				36
#define	TREENODETYPE_COLOR_PALETTE		37
#define	TREENODETYPE_IPADDRESS			38
#define	TREENODETYPE_KEYBOARD			39
#define	TREENODETYPE_PICTURE			40
#define	TREENODETYPE_LISTITEM			41
#define	TREENODETYPE_MSGWND				42

class CUIManager;
class CDuiPropertyFile
{
public:
	struct tagToolboxIco 
	{
		CString ControlName;
		UINT index;
	};
public:
	CDuiPropertyFile(void);
	virtual ~CDuiPropertyFile(void);

	BOOL LoadPropertyFile();
	xml_node GetRoot();
	/*
	BOOL IsControlUI(LPCTSTR className);
	BOOL IsContainerUI(LPCTSTR className);
	BOOL IsWindowForm(LPCTSTR className);
	*/
	BOOL IsControlUI(xml_node nodeNode);
	BOOL IsContainerUI(xml_node nodeNode);
	BOOL IsWindowForm(xml_node nodeNode);
	BOOL IsFontNode(xml_node nodeNode);
	BOOL IsDefaultNode(xml_node node);
	BOOL IsStyleNode(xml_node node);

	//是否有定义text字段
	BOOL IsNeedInputText(xml_node node);

	xml_node FindControl(LPCTSTR className); //查找控件的定义
	xml_node FindAttribute(LPCTSTR className, LPCTSTR attrName); //查找控件定义的属性
	xml_node FindAttribute(xml_node node, LPCTSTR attrName); //查找控件定义的属性

	//获得控件的图标序号
	static UINT GetControlIconIndex(xml_node nodeDoc);

	//为了防止属性冗余, 过滤默认属性
	void FilterDefaultValue(xml_node nodeDoc, CUIManager *pUIManager);

	//过滤多余的pos属性
	void FilterPosWidthHeight(xml_node nodeDoc, CUIManager *pUIManager);

	//插入属性到布局文件中, 遵循属性文件中属性定义的顺序
	xml_attribute AddAttribute(xml_node nodeControl, LPCTSTR attrName, RECT rc, CUIManager *pUIManager);
	xml_attribute AddAttribute(xml_node nodeControl, LPCTSTR attrName, int attrValue, CUIManager *pUIManager);
	xml_attribute AddAttribute(xml_node nodeControl, LPCTSTR attrName, bool attrValue, CUIManager *pUIManager);
	xml_attribute AddAttribute(xml_node nodeControl, LPCTSTR attrName, LPCTSTR attrValue, CUIManager *pUIManager);

protected:
	void InitToolBoxIcon();
	xml_attribute _addAttribute(xml_node nodeControl, LPCTSTR attrName, LPCTSTR attrValue);
	BOOL IsCorrectOrder(LPCTSTR controlName, LPCTSTR attrNameAppend, LPCTSTR attrName);
	void _FilterDefaultValue(xml_node nodeDoc, xml_attribute attr, xml_node nodeDuiProp, CUIManager *pUIManager);
public:
	xml_document xml;
	CImageList m_ImageList;
};

