#pragma once

//XML编辑器的设置参数
struct tagEditorConfig 
{
	CString strXmlFontName;			//xml编辑，字体名称
	int		nXmlFontSize;			//xml编辑，字体大小
	int		nXmlLineSpace;			//xml编辑，行间距
	COLORREF crXmlBkColor;			//xml编辑，背景色
	COLORREF crXmlSelBkColor;		//xml编辑，选中文本背景色
	COLORREF crXmlCaretLineBkColor;	//xml编辑，焦点行背景色
	BOOL	bAutoWrapLine;			//xml编辑，自动换行

	COLORREF crDesignerBkColor;		//设计界面的背景色
	BOOL bDesignerShowShadow;		//设计界面的duilib窗口阴影
	CString strPropertyFile;		//指定duilib.xml的路径，这个设置允许设置其他目录的duilib.xml，必须是绝对路径。
	CString strAttachTestCommand;	//附加测试窗体的程序，可以把项目中的执行程序引进来
	int nControlTreeDeep;			//载入控件树时，默认展开的层数。
	BOOL bFilterPosWidthHeight;		//是否过滤掉pos属性

	int nFormatInsert;		//插入模式, 0=自动定位，1=绝对定位，2=相对定位
	BOOL bViewUiPreview;	//是否在编辑界面显示UI效果
	BOOL bViewRuleBar;		//是否显示标尺
	BOOL bViewGrid;			//是否显示网格
	BOOL bViewMouse;		//是否显示鼠标线

	BOOL bLangManager;		//是否自动管理语言包
	CString strLangPath;	//语言包路径

	void CopyFrom(const tagEditorConfig &cfg)
	{
		strXmlFontName = cfg.strXmlFontName;
		nXmlFontSize = cfg.nXmlFontSize;
		nXmlLineSpace = cfg.nXmlLineSpace;
		crXmlBkColor = cfg.crXmlBkColor;
		crXmlSelBkColor = cfg.crXmlSelBkColor;
		crXmlCaretLineBkColor = cfg.crXmlCaretLineBkColor;
		bAutoWrapLine = cfg.bAutoWrapLine;

		crDesignerBkColor = cfg.crDesignerBkColor;
		bDesignerShowShadow = cfg.bDesignerShowShadow;
		strPropertyFile = cfg.strPropertyFile;
		strAttachTestCommand = cfg.strAttachTestCommand;
		nControlTreeDeep = cfg.nControlTreeDeep;
		bFilterPosWidthHeight = cfg.bFilterPosWidthHeight;

		nFormatInsert = cfg.nFormatInsert;
		bViewUiPreview = cfg.bViewUiPreview;
		bViewRuleBar = cfg.bViewRuleBar;
		bViewGrid = cfg.bViewGrid;
		bViewMouse = cfg.bViewMouse;

		bLangManager = cfg.bLangManager;
		strLangPath = cfg.strLangPath;
	}

	void LoadConfig(LPCTSTR filename)
	{
		xml.load_file(filename, pugi::parse_full);
		xml_node root = xml.child(XTEXT("DuiEditor"));

		strXmlFontName = root.child(XTEXT("XmlFontName")).text().as_string(XTEXT("Courier New"));
		nXmlFontSize = root.child(XTEXT("XmlFontSize")).text().as_int(14);
		nXmlLineSpace = root.child(XTEXT("XmlLineSpace")).text().as_int(1);
		crXmlBkColor = root.child(XTEXT("XmlBkColor")).text().as_int(RGB(255,255,255));
		crXmlSelBkColor = root.child(XTEXT("XmlSelBkColor")).text().as_int(RGB(0xA0,0xCA,0xF0));
		crXmlCaretLineBkColor = root.child(XTEXT("XmlCaretLineBkColor")).text().as_int(RGB(215,215,247));
		bAutoWrapLine = root.child(XTEXT("AutoWrapLine")).text().as_bool();

		crDesignerBkColor = root.child(XTEXT("DesignerBkColor")).text().as_int(RGB(0xF0, 0xF0, 0xF0));
		bDesignerShowShadow = root.child(XTEXT("DesignerShowShadow")).text().as_bool();
		strPropertyFile = root.child(XTEXT("PropertyFile")).text().as_string();
		strAttachTestCommand = root.child(XTEXT("AttachTestCommand")).text().as_string();
		nControlTreeDeep = root.child(XTEXT("ControlTreeDeep")).text().as_int(10);
		bFilterPosWidthHeight = root.child(XTEXT("FilterPosWidthHeight")).text().as_bool();

		nFormatInsert = root.child(XTEXT("FormatInsert")).text().as_int(0);
		bViewUiPreview = root.child(XTEXT("ViewUiPreview")).text().as_bool();
		bViewRuleBar = root.child(XTEXT("ViewRuleBar")).text().as_bool();
		bViewGrid = root.child(XTEXT("ViewGrid")).text().as_bool();
		bViewMouse = root.child(XTEXT("ViewMouse")).text().as_bool();

		bLangManager = root.child(XTEXT("LangManager")).text().as_bool();
		strLangPath = XML2T(root.child(XTEXT("LangPath")).text().as_string());
	}

	void SaveConfig()
	{
		xml_node root = xml.child_auto(XTEXT("DuiEditor"));

		root.child_auto(XTEXT("XmlFontName")).text().set(T2XML(strXmlFontName));
		root.child_auto(XTEXT("XmlFontSize")).text().set(nXmlFontSize);
		root.child_auto(XTEXT("XmlLineSpace")).text().set(nXmlLineSpace);
		root.child_auto(XTEXT("XmlBkColor")).text().set((int)crXmlBkColor);
		root.child_auto(XTEXT("XmlSelBkColor")).text().set((int)crXmlSelBkColor);
		root.child_auto(XTEXT("XmlCaretLineBkColor")).text().set((int)crXmlCaretLineBkColor);
		root.child_auto(XTEXT("AutoWrapLine")).text().set(bAutoWrapLine);

		root.child_auto(XTEXT("DesignerBkColor")).text().set((int)crDesignerBkColor);
		root.child_auto(XTEXT("DesignerShowShadow")).text().set(bDesignerShowShadow);
		root.child_auto(XTEXT("PropertyFile")).text().set(T2XML(strPropertyFile));
		root.child_auto(XTEXT("AttachTestCommand")).text().set(T2XML(strAttachTestCommand));
		root.child_auto(XTEXT("ControlTreeDeep")).text().set(nControlTreeDeep);
		root.child_auto(XTEXT("FilterPosWidthHeight")).text().set(bFilterPosWidthHeight);

		root.child_auto(XTEXT("FormatInsert")).text().set(nFormatInsert);
		root.child_auto(XTEXT("ViewUiPreview")).text().set(bViewUiPreview);
		root.child_auto(XTEXT("ViewRuleBar")).text().set(bViewRuleBar);
		root.child_auto(XTEXT("ViewGrid")).text().set(bViewGrid);
		root.child_auto(XTEXT("ViewMouse")).text().set(bViewMouse);

		root.child_auto(XTEXT("LangManager")).text().set(bLangManager);
		root.child_auto(XTEXT("LangPath")).text().set(T2XML(strLangPath));

		xml.save_to_default_file();
	}

	xml_node Session()
	{
		return xml.child_auto(XTEXT("DuiEditor")).child_auto("Session");
	}
public:
	xml_document xml;
};