#pragma once
#include "xmlMatchedTagsHighlighter.h"

#define WM_SCIWND_RBUTTONUP		WM_USER+987
#define WM_SCIWND_LBUTTONDOWN	WM_USER+988
#define WM_SCIWND_LBUTTONUP		WM_USER+989
#define WM_SCIWND_CLICK			WM_USER+990
#define WM_SCIWND_MOUSEMOVE		WM_USER+991

// CSciWnd
typedef  int SciDll_void;
typedef int (*SEND_EDITOR)(void*,int,int,int);

enum folderStyle {FOLDER_TYPE, FOLDER_STYLE_SIMPLE, FOLDER_STYLE_ARROW, FOLDER_STYLE_CIRCLE, FOLDER_STYLE_BOX, FOLDER_STYLE_NONE};
class CSciWnd : public CWnd
{
	DECLARE_DYNAMIC(CSciWnd)
public:
	CSciWnd();
	virtual ~CSciWnd();

	virtual BOOL Create (DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID=NULL);

	LRESULT SendEditor(UINT msg, WPARAM wParam=0, LPARAM lParam=0)
	{
		return m_sendeditor(m_pSendEditor, msg, wParam, lParam);
	}
	LRESULT execute(UINT msg, WPARAM wParam=0, LPARAM lParam=0)
	{
		return m_sendeditor(m_pSendEditor, msg, wParam, lParam);
	}

	BOOL LoadFile(LPCTSTR szPath);
	BOOL SaveFile(LPCTSTR szPath);

	void InitXML(const tagEditorConfig &opt);

	BOOL OnParentNotify(SCNotification *pMsg);

	void findMatchingBracePos(int & braceAtCaret, int & braceOpposite);
	bool braceMatch();
	bool BraceHighLightAttributes(int first, int second, int openTagTailLen);

protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
protected:
	void defineMarker(int marker, int markerType, COLORREF fore, COLORREF back, COLORREF foreActive) {
		execute(SCI_MARKERDEFINE, marker, markerType);
		execute(SCI_MARKERSETFORE, marker, fore);
		execute(SCI_MARKERSETBACK, marker, back);
		execute(SCI_MARKERSETBACKSELECTED, marker, foreActive);
	}
	LRESULT execute(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) const {
		return m_sendeditor(m_pSendEditor, static_cast<int>(Msg), static_cast<int>(wParam), static_cast<int>(lParam));
	}
	SEND_EDITOR m_sendeditor;
	void *m_pSendEditor;

	HMODULE m_hSciLexer;
public:
	/*----文字编辑功能Text retrieval and modification----*/

	//获取所有字符
	SciDll_void sci_GetTextAll(CStringA &Text);
	//获取一行字符
	SciDll_void sci_GetTextLine(int line, CStringA &Text);
	//获取一定长度的字符
	SciDll_void sci_GetText(int length, CStringA &Text);
	//设置字符到控件
	SciDll_void sci_SetText(LPCSTR pText);
	//设置文档已保存的标志
	SciDll_void sci_SetSavePoint();
	//替换已选择字符,如果未选择,在当前光标插入
	SciDll_void sci_ReplaceSel(LPCSTR pText);
	//设置只读模式
	SciDll_void sci_SetReadOnly(BOOL bReadOnly=TRUE);
	//获取是否只读模式
	BOOL sci_GetReadOnly();
	//获取范围内的字符,返回字符的长度
	int  sci_GetTextRange(int cpMin, int cpMax, CStringA &pTextRange);
	SciDll_void sci_GetStyledText(int cpMin, int cpMax, CStringA &pText);
	//设置文档缓冲区大小, 使创建的文档不会小于当前的文档
	SciDll_void sci_Allocate(int bytes);
	//在光标位置插入字符串
	SciDll_void sci_AddText(int length, LPCSTR pText);
	//在光标位置插入包含字符格式的字符串
	SciDll_void sci_AddStyledText(int length, LPCSTR pText);
	//在文档末尾插入字符串
	SciDll_void sci_AppendText(int length, LPCSTR pText);
	//在某个位置插入字符串.如果pos=-1,直接在光标位置插入
	SciDll_void sci_InsertText(int pos, LPCVOID pText);
	//清空文档所有字符
	SciDll_void sci_ClearTextAll();
	//删除范围内的字符
	SciDll_void sci_DeleteRange(int pos, int deleteLength);
	//清空文档字符的所有格式();
	SciDll_void sci_ClearDocunmentStyle();
	//获取某个位置的字符
	char sci_GetCharAt(int pos);
	//获取某个位置的字符字体风格
	int sci_GetStyleAt(int pos);
	//设置字符字体风格
	SciDll_void sci_SetStyleBits(int bits);
	//获取字符字体风格
	int sci_GetStyleBits();

	int sci_TargetAsUTF8(LPTSTR pText);
	int sci_EncodedFromUTF8(LPCSTR pUTF8, LPTSTR pText);
	SciDll_void sci_SetLengthForEncode(int bits);

	/*----查找----*/
	//查找字符串,失败返回-1
	//Flag = SCFIND_MATCHCASE 模糊匹配
	//FLAG = SCFIND_WHOLEWORD 准确匹配
	//FLAG = SCFIND_WORDSTART 从左开始匹配
	//FLAG = SCFIND_REGEXP    匹配注释部分的字符串
	//FLAG = SCFIND_POSIX
	int  sci_FindText(int searchFlags, Sci_TextToFind &ttf);	
	SciDll_void sci_SearchAnchor();
	int  sci_SearchNext(int searchFlags, LPCSTR pText);
	int  sci_SearchPrev(int searchFlags, LPCSTR pText);

	/*----查找和替换----*/
	SciDll_void sci_SetTargetStart(int pos);
	int  sci_GetTargetStart();
	SciDll_void sci_SetTargetEnd(int pos);
	int	 sci_GetTargetEnd();
	SciDll_void sci_TargetFromSelection();
	SciDll_void sci_SetSearchFlags(int searchFlags);
	int  sci_GetSearchFlags();
	int  sci_SearchInTarget(int length, LPCSTR pText);
	//替换内容不包含注释部分
	int sci_ReplaceTarget(int length, const char *pText);
	//替换内容包含了注释部分
	int sci_ReplaceTargetRe(int length, LPCSTR pText);
	SciDll_void sci_GetTag(int tagNumber, LPTSTR pText);

	/*----覆盖模式(相反的是插入模式)----*/
	//设置是否覆盖模式
	SciDll_void sci_OverType_SetOverType(BOOL bOverType=TRUE);
	//获取是否覆盖模式
	BOOL sci_OverType_GetOverType();

	/*----剪切,复制,粘贴,Redo,Undo----*/
	SciDll_void sci_Cut();
	SciDll_void sci_Copy();	
	SciDll_void sci_Paste();
	//返回是否可以复制
	BOOL sci_CanCopy();
	//返回是否可以剪切
	BOOL sci_CanCut();
	//返回是否可以粘贴
	BOOL sci_CanPaste();
	//删除选中内容
	SciDll_void sci_Clear(); 
	//类似Copy,区别是当选择内容为空时,复制当前整行内容
	SciDll_void sci_CopyAllowLine();

	SciDll_void sci_CopyRange(int start, int end);
	SciDll_void sci_CopyText(int length, LPCSTR pText);

	SciDll_void sci_SetPasteConvertEndings(BOOL convert);
	BOOL sci_GetPasteConvertEndings();

	SciDll_void sci_Undo();
	BOOL sci_CanUndo();
	//清空所有撤消信息
	SciDll_void sci_EmptyUndoBuffer();
	SciDll_void sci_Redo();
	BOOL sci_CanRedo();
	SciDll_void sci_SetUndoCollection(BOOL collectUndo);
	BOOL sci_GetUndoCollection();
	SciDll_void sci_BeginUndoAction();
	SciDll_void sci_EndUndoAction();
	SciDll_void sci_AddUndoAction(int token, int flags);


	/*----Error Handing----*/
	//SC_STATUS_OK 0 No failures 
	//SC_STATUS_FAILURE 1 Generic failure 
	//SC_STATUS_BADALLOC 2 Memory is exhausted 
	SciDll_void sci_SetStatus(int status);
	int  sci_GetStatus();

	/*----选中内容信息----*/
	//返回所有字符的bytes
	int  sci_GetTextLength();
	//返回文档内容bytes
	int  sci_GetLength();
	int  sci_GetLineCount();
	SciDll_void sci_SetFirstVisibleLine(int lineDisplay);
	int  sci_GetFirstVisibleLine();
	int  sci_LineSonScreen();
	BOOL sci_GetModify();
	SciDll_void sci_SetSel(int anchorPos, int currentPos);
	SciDll_void sci_GoToPos(int pos);
	SciDll_void sci_GoToLine(int line);
	SciDll_void sci_SetCurrentPos(int pos);
	int  sci_GetCurrentPos();
	SciDll_void sci_SetAnchor(int pos);
	int  sci_GetAnchor();
	SciDll_void sci_SetSelectionStart(int pos);
	SciDll_void sci_SetSelectionEnd(int pos);
	int  sci_GetSelectionStart();
	int  sci_GetSelectionEnd();
	SciDll_void sci_SetEmptySelection(int pos);
	SciDll_void sci_SelectAll();
	//取得pos所在的行
	int  sci_LineFromPosition(int pos);
	//取得line行第1个字符的pos
	int  sci_PositionFromLine(int line);
	int  sci_GetLineEndPosition(int line);
	int  sci_LineLength(int line);
	SciDll_void sci_GetSelText(CStringA &Text);

	//原来的函数没搞明白,我实现的函数,返回当前行号和当前行的字符内容,
	int  sci_GetCurLine(CStringA &Text);
	//返回当前的行号
	int  sci_GetCurLine();
	//返回选中内容是不是一个矩形区域
	BOOL sci_GetSelectionIsRectangle();
	SciDll_void sci_SetSelectionMode(int mode);
	int  sci_GetSelectionMode();
	int  sci_GetLineSelStartPosition(int line);
	int  sci_GetLineSelEndPosition(int line);
	//位置无效时,移动到最接近的位置,所有选择丢失.
	SciDll_void sci_MoveCaretinSideView();
	int  sci_WordEndPosition(int position, BOOL onlyWordCharacters=TRUE);
	int  sci_WordStartPosition(int position, BOOL onlyWordCharacters=TRUE);
	int  sci_PositionBefore(int postion);
	int  sci_PostionAfter(int postion);
	int  sci_CountCharacters(int startPos, int endPos);
	int  sci_TextWidth(int styleNumber, LPCSTR pText);
	int  sci_TextHeight(int line);
	int  sci_GetColumn(int pos);
	int  sci_FindColumn(int line, int column);
	int  sci_PostionFromPoint(int x, int y);
	int  sci_PostionFromPointClose(int x, int y);
	int  sci_CharPositionFromPoint(int x, int y);
	int  sci_CharPositionFromPointClose(int x, int y);
	int  sci_PointXFromPosition(int pos);
	int  sci_PointYFromPosition(int pos);
	SciDll_void sci_HideSelection(BOOL hide);
	SciDll_void sci_ChooseCaretX();
	SciDll_void sci_MoveSelectedLinesUp();
	SciDll_void sci_MoveSlectionLinesDown();

	/*----Multiple Selection and Virtual Space----*/

	/*-----------滚动条--------*/
	SciDll_void sci_LineScroll(int column, int line);
	SciDll_void sci_ScrollCaret();
	SciDll_void sci_SetXCaretPolicy(int caretPolicy, int caretSlop);
	SciDll_void sci_SetYCaretPolicy(int caretPolicy, int caretSlop);
	SciDll_void sci_SetVisiblePolicy(int caretPolicy, int caretSlop);
	SciDll_void sci_SetHScrollBar(BOOL visible);
	BOOL sci_GetHScrollBar();
	SciDll_void sci_SetVScrollBar(BOOL visible);
	BOOL sci_GetVScrollBar();
	SciDll_void sci_SetXOffset(int xOffset);
	int  sci_GetXOffset();
	SciDll_void sci_SetScrollWidth(int pixelWidth);
	int  sci_GetScrollWidth();
	//如果滚动条不够宽, 会调整大小
	SciDll_void sci_SetScrollWidthTracking(BOOL tracking);
	BOOL sci_GetScrollWidthTracking();
	SciDll_void sci_SetEndAtLastLine(BOOL endAtLastLine);
	BOOL sci_GetEndAtLastLine();

	/*------White space-------*/
	SciDll_void sci_SetViewWs(int wsMode);
	BOOL sci_GetViewWs();

	SciDll_void sci_SetWhiteSpaceFore(BOOL useWhitespaceForeColour, COLORREF colour);
	SciDll_void sci_SetWhiteSpaceBack(BOOL useWhitespaceBackColour, COLORREF colour);

	SciDll_void sci_SetWhiteSpaceSize(int size);
	int  sci_GetWhiteSpaceSize();

	SciDll_void sci_SetExtraAscent(int extraAscent);
	BOOL sci_GetExtraAscent();

	SciDll_void sci_SetExtraDescent(int extraDescent);
	BOOL sci_GetExtraDescent();

	/*---------鼠标指针------*/
	//SC_CURSORNORMAL -1 默认的鼠标指针
	//SC_CURSORWAIT 4 
	SciDll_void sci_SetCursor(int curType);
	int  sci_GetCursor();
	/*---------Mouse capture-------*/
	SciDll_void sci_SetMouseDownCaptures(BOOL captures);
	BOOL sci_GetMouseDownCaptures();
	/*----------Line Endings-------*/
	SciDll_void sci_SetEolMode(int eolMode);
	int  sci_GetEolMode();
	SciDll_void sci_ConvertEols(int eolMode);
	SciDll_void sci_SetViewEol(BOOL visible);
	BOOL sci_GetViewEol();
	/*----------Styling------------*/
	int sci_GetEndStyled();
	SciDll_void sci_StartStyling(int pos, int mask);
	SciDll_void sci_SetStyling(int length, int style);
	SciDll_void sci_SetStylingEx(int length, LPCSTR styles);
	SciDll_void sci_SetLineState(int line, int value);
	int  sci_GetLineState(int line);
	int  sci_GetMaxLineState();
	/*----------Style definition---*/
	SciDll_void sci_StyleResetDefault();
	//把所有格式设置成STYLE_DEFAULT
	SciDll_void sci_StyleClearAll();

	//设置字体
	SciDll_void sci_StyleSetFont(int styleNumer, LPCSTR fontName);
	SciDll_void sci_StyleGetFont(int styleNumer, CString &fontName);

	//设置字体大小
	SciDll_void sci_StyleSetSize(int styleNumber, int sizeInPoints);
	int  sci_StyleGetSize(int styleNumber);

	//字体大小包含小数点的,比如size=9.4, 应该穿传入940
	SciDll_void sci_StyleSetSizeFractional(int styleNumber, int sizeInHundredthPoints);
	int  sci_StyleGetSizeFractional(int styleNumber);

	//粗体
	SciDll_void sci_StyleSetBold(int styleNumber, bool bold);
	BOOL  sci_StyleGetBold(int styleNumber);

	//字体粗细
	SciDll_void sci_StyleSetWeight(int styleNumber, int weight);
	int  sci_StyleGetWeight(int styleNumber);

	//斜体
	SciDll_void sci_StyleSetItalic(int styleNumber, BOOL italic);
	BOOL sci_StyleSetItalic(int styleNumber);

	//下划线
	SciDll_void sci_StyleSetUnderline(int styleNumber, BOOL underline);
	BOOL sci_StyleGetUnderline(int styleNumber);

	//字符前景色
	SciDll_void sci_StyleSetFore(int styleNumber, COLORREF colour);
	COLORREF  sci_StyleGetFore(int styleNumber);

	//字符背景色
	SciDll_void sci_StyleSetBack(int styleNumber, COLORREF colour);
	COLORREF  sci_StyleGetBack(int styleNumber);

	//行尾的格式
	SciDll_void sci_StyleSetEolFilled(int styleNumber, BOOL eolFilled);
	int  sci_StyleGetEolFilled(int styleNumber);

	//设置字符集
	SciDll_void sci_StyleSetCharacterSet(int styleNumber, int charSet=SC_CHARSET_GB2312);
	int  sci_StyleGetCharacterSet(int styleNumber);

	//设置大小写,caseMode=SC_CASE_UPPER,SC_CASE_LOWER,SC_CASE_MIXED
	SciDll_void sci_StyleSetCase(int styleNumber, int caseMode);
	int  sci_StyleGetCase(int styleNumber);

	//是否可见
	SciDll_void sci_StyleSetVisible(int styleNumber, BOOL visible);
	BOOL sci_StyleGetVisible(int styleNumber);

	//不允许修改,如果是删除整个区域,区域内包含不允许修改的style也会被删掉.
	SciDll_void sci_StyleSetChangeable(int styleNumber, BOOL changeable);
	BOOL sci_StyleGetChangeable(int styleNumber);

	//设置热点区域，类似于HTML的超链接，鼠标移过去时，鼠标变成手型，前景色和背景色可以改变，并且有下划线，允许被点击
	SciDll_void sci_StyleSetHotspot(int styleNumber, BOOL hotspot);
	BOOL sci_StyleGetHotspot(int styleNumber);

	/*----Caret, selection, and hotspot styles-----*/
	//设置当前选中内容的前景色
	SciDll_void sci_SetSelFore(BOOL useSelForeColour, COLORREF colour);

	//设置当前选中内容的背景色
	SciDll_void sci_SetSelBack(BOOL useSelBackColour, COLORREF colour);

	//设置当前选中内容的通明度
	SciDll_void sci_SetSelAlpha(int alpha);
	int sci_GetSelAlpha();

	//???
	SciDll_void sci_SetSelEolFilled(BOOL filled);
	BOOL sci_GetSelEolFilled();

	//???
	SciDll_void sci_SetCaretFore(COLORREF colour);
	COLORREF sci_GetCaretFore();

	SciDll_void sci_SetCaretLineVisible(BOOL show);
	BOOL sci_GetCaretLineVisible();

	SciDll_void sci_SetCaretLineBack(COLORREF colour);
	COLORREF sci_GetCaretLineBack();

	SciDll_void sci_SetCaretLineBackAlpha(int alpha);
	int sci_GetCaretLineAlpha();

	//设置符号的闪动，millsends是可见和隐藏的切换时间,默认500
	SciDll_void sci_SetCaretPeriod(int millseconds);
	int sci_GetCaretPeriod();

	SciDll_void sci_SetCaretStyle(int style);
	int sci_GetCaretStyle();

	SciDll_void sci_SetCaretWidth(int pixels);
	int sci_GetCaretWidth();

	SciDll_void sci_SetHotSpotActiveFore(BOOL useHotSpotForeColour, COLORREF colour);
	COLORREF sci_GetHotSpotActiveFore();

	SciDll_void sci_SetHotSpotActiveBack(BOOL useHotSpotBackColour, COLORREF colour);
	COLORREF sci_GetHotSpotActiveBack();

	SciDll_void sci_SetHotSpotActiveUnderline(BOOL underLine);
	BOOL sci_GetHotSpotActiveUnderline();

	SciDll_void sci_SetHotSpotSingleLine(BOOL singelLine);
	BOOL sci_GetHotSpotSingleLine();

	SciDll_void sci_SetControlCharSymbol(int symbol);
	int sci_GetControlCharSymbol();

	//参数: SC_CARETSTICKY_OFF(0),SC_CARETSTICKY_ON(1),SC_CARETSTICKY_WHITESPACE(2),
	SciDll_void sci_SetCaretSticky(int useCaretStickyBehaviour);
	int sci_GetCaretSticky();
	SciDll_void sci_ToggleCaretSticky();

	/*----------Margins------------*/

	//SC_MARGIN_SYMBOL (0),
	//SC_MARGIN_BACK (2) ,
	//SC_MARGIN_FORE (3),
	//SC_MARGIN_TEXT (4),
	//SC_MARGIN_RTEXT (5) 
	//设置页边显示行号还是符号等等
	SciDll_void sci_SetMarginTypeN(int margin, int iType);
	int sci_GetMarginTypeN(int margin);

	//设置页边宽度
	SciDll_void sci_SetMarginWidthN(int margin, int PixelWidth);
	int sci_GetMarginWidthN(int margin);

	//设置页边掩码
	SciDll_void sci_SetMarginMaskN(int margin, int mask);
	int sci_GetMarginMaskN(int margin);

	//设置页边是否接受鼠标点击事件
	SciDll_void sci_SetMarginSensitiveN(int margin, BOOL sensitive);
	BOOL sci_GetMarginSensitiveN(int margin);

	//设置鼠标移到页边时的指针类型
	SciDll_void sci_SetMarginCursorN(int margin, int cursor);
	int sci_GetMarginCursorN(int margin);

	//设置页边内容左空白和右空白，页边的页边。
	SciDll_void sci_SetMarginLeft(int pixels);
	int sci_GetMarginLeft();
	SciDll_void sci_SetMarginRight(int pixels);
	int sci_GetMarginRight();

	SciDll_void sci_SetFoldMarginColour(BOOL useSetting, COLORREF colour);
	SciDll_void sci_SetFoldMarginHiColour(BOOL useSetting, COLORREF colour);

	SciDll_void sci_MarginSetText(int line, LPTSTR pText);
	SciDll_void sci_MarginGetText(int line, LPTSTR pText);

	//设置页边的风格
	SciDll_void sci_MarginSetStyle(int line, int style);
	int sci_MarginGetStyle(int line);

	SciDll_void sci_MarginSetStyles(int line, LPTSTR pStyle);
	SciDll_void sci_MarginGetStyles(int line, LPTSTR pStyle);

	SciDll_void sci_MarginTextClearAll();

	//设置字体风格和正文的偏移，以示区别
	SciDll_void sci_MarginSetStyleOffset(int style);
	int sci_MarginGetStyleOffset();

	SciDll_void sci_SetMarginOptions(int marginOptions);
	int sci_GetMarginOptions();

	/*----------Annotations 注释--------*/
	SciDll_void sci_AnnotationSetText(int line, LPTSTR pText);
	SciDll_void sci_AnnotationGetText(int line , LPTSTR pText);

	SciDll_void sci_AnnotationSetStyle(int line, LPTSTR pStyle);
	SciDll_void sci_AnnotationGetStyle(int line, LPTSTR pStyle);

	BOOL sci_AnnotationGetLines(int line);

	SciDll_void sci_AnnotationClearAll();

	SciDll_void sci_AnnotationSetVisible(int visible);
	int sci_AnnotationGetVisible();

	//参数
	//ANNOTATION_HIDDEN 0 隐藏注释
	//ANNOTATION_STANDARD 1 标准注释
	//ANNOTATION_BOXED 2 使用一种文本框显示
	SciDll_void sci_AnnotationSetStyleOffset(int style);
	int sci_AnnotationGetStyleOffset();

	/*----------Other settings-----*/
	SciDll_void sci_SetUsePalette(BOOL allowPaletteUse);
	BOOL sci_GetUsePalette();

	SciDll_void sci_SetBufferedDraw(BOOL isBuffered);
	BOOL sci_GetBufferedDraw();

	SciDll_void sci_SetTwoPhaseDraw(BOOL twoPhase);
	BOOL sci_GetTwoPhaseDraw();

	SciDll_void sci_SetTechnology(int technology);
	BOOL sci_GetTechnology();

	SciDll_void sci_SetFontQuality(int fontQuality);
	int sci_GetFontQuality();

	//codePage=936支持简体中文
	SciDll_void sci_SetCodePage(int codePage);
	int sci_GetCodePage();

	SciDll_void sci_SetKeysUnicode(BOOL KeysUnicode);
	BOOL sci_GetKeysUnicode();

	//设置控件中允许出现的字符
	SciDll_void sci_SetWordChars(LPCSTR pText);
	//设置控件不处理的字符,就是当成空白了.
	SciDll_void sci_SetWhiteSpaceChars(LPCSTR pText);
	//使用默认的字符和空白字符
	SciDll_void sci_SetCharsDefault();

	SciDll_void sci_GrabFocus();
	SciDll_void sci_SetFocus(BOOL bfocus=TRUE);
	BOOL sci_GetFocus();

	/*-------Brace highlighting----*/
	SciDll_void sci_BraceHighlight(int pos1, int pos2);
	SciDll_void sci_BraceBadLight(int pos1);
	//高亮指示器
	SciDll_void sci_BraceHighlightIndicator(BOOL useBraceHighlightIndicator, int indicatorNumber);
	SciDll_void sci_BraceBadLightIndicator(BOOL useBraceHighlightIndicator, int indicatorNumber);

	SciDll_void sci_BraceMatch(int pos, int maxReStyle);

	/*--Tabs and Indentation Guides-*/
	SciDll_void sci_SetTabWidth(int widthInChars);
	int sci_GetTabWidth();

	SciDll_void sci_SetUseTabs(BOOL useTabs);
	BOOL sci_GetUseTabs();

	SciDll_void sci_SetIndent(int widthInChars);
	int sci_GetIndent();

	SciDll_void sci_SetTabIndents(BOOL tabIndents);
	BOOL sci_GetTabIndents();

	SciDll_void sci_SetBackSpaceUnIndents(BOOL bsUnIndents);
	BOOL sci_GetBackSpaceUnIndents();

	SciDll_void sci_SetLineIndentation(int line, int indentation);
	int sci_GetLineIndentation(int line);

	int sci_GetLineIndentPosition(int line);

	//SC_IV_NONE No indentation guides are shown. 
	//SC_IV_REAL Indentation guides are shown inside real indentation white space. 
	//SC_IV_LOOKFORWARD Indentation guides are shown beyond the actual indentation up to the level of the next non-empty line. If the previous non-empty line was a fold header then indentation guides are shown for one more level of indent than that line. This setting is good for Python. 
	//SC_IV_LOOKBOTH 
	SciDll_void sci_SetIndentationGuides(int indentView);
	int sci_GetIndentationGuides();

	SciDll_void sci_SetHighlightGuide(int column);
	int sci_GetHighlightGuide();

	/*----------Markers------------*/
	//选择书签的样式
	//SC_MARK_CIRCLE, SC_MARK_ROUNDRECT, SC_MARK_ARROW, SC_MARK_SMALLRECT, 
	//SC_MARK_SHORTARROW, SC_MARK_EMPTY, SC_MARK_ARROWDOWN, SC_MARK_MINUS, 
	//SC_MARK_PLUS, SC_MARK_ARROWS, SC_MARK_DOTDOTDOT, SC_MARK_BACKGROUND, 
	//SC_MARK_LEFTRECT, SC_MARK_FULLRECT, and SC_MARK_UNDERLINE.
	SciDll_void sci_MarkerDefine(int markerNumber, int markerSymbols);

	//自定义书签样式
	SciDll_void sci_MarkerDefinePixmap(int markerNumber, LPCSTR pXPM);

	SciDll_void sci_RGBAImageSetWidth(int width);
	SciDll_void sci_RGBAImageSetHeight(int height);
	SciDll_void sci_MarkerDefineRGBAImage(int markerNumber, LPCSTR pPixels);

	SciDll_void sci_MarkerSymbolDefined(int markerNumber);

	SciDll_void sci_MarkerSetFore(int markerNumber, COLORREF colour);
	SciDll_void sci_MarkerSetBack(int markerNumber, COLORREF colour);
	SciDll_void sci_MarkerSetBackSelected(int markerNumber, COLORREF colour);

	SciDll_void sci_MarkerEnableHighlight(BOOL enable);

	SciDll_void sci_MarkerSetAlpha(int markerNumber, int alpha);

	SciDll_void sci_MarkerAdd(int line, int markerMask);
	SciDll_void sci_MarkerAddSet(int line, int markerMask);
	SciDll_void sci_MarkerDelete(int line, int markerNumber);
	SciDll_void sci_MarkerDeleteAll(int markerNumber);
	int sci_MarkerGet(int line);
	SciDll_void sci_MarkerNext(int lineStart, int markerMask);
	SciDll_void sci_MarkerPrevious(int lineStart, int markerMask);

	SciDll_void sci_MarkerLineFromHandle(int markerHandle);
	SciDll_void sci_MarkerDeleteHandle(int markerHandle);

	/*----------Indicators---------*/
	SciDll_void sci_IndicSetStyle(int indicatorNumber, int indicatorStyle);
	int sci_IndicGetStyle(int indicatorNumber);

	SciDll_void sci_IndicSetFore(int indicatorNumber, COLORREF colour);
	COLORREF sci_IndicGetFore(int indicatorNumber);

	SciDll_void sci_IndicSetAlpha(int indicatorNumber, int alpha);
	int sci_IndicGetAlpha(int indicatorNumber);

	SciDll_void sci_IndicSetUnder(int indicatorNumber, BOOL under);
	BOOL sci_IndicGetUnder(int indicatorNumber);

	/*----------Modern Indicators--------*/
	SciDll_void sci_SetIndicatorCurrent(int indicator);
	int sci_GetIndicatorCurrent();

	SciDll_void sci_SetIndicatorValue(int value);
	int sci_GetIndicatorValue();

	SciDll_void sci_IndicatorFillRange(int position, int fillLength);
	SciDll_void sci_IndicatorClearRange(int position, int clearLength);

	SciDll_void sci_IndicatorAllOnFor(int position);

	SciDll_void sci_IndicatorValueAt(int indicator, int postion);

	SciDll_void sci_IndicatorStart(int indicator, int postion);
	SciDll_void sci_IndicatorEnd(int indicator, int postion);

	/*----------Autocompletion-----*/
	SciDll_void sci_AutocShow(int lenEntered, const char *pList);
	SciDll_void sci_AutocCancel();
	SciDll_void sci_AutocActive();
	SciDll_void sci_AutocPosStart();
	SciDll_void sci_AutocComplete();
	SciDll_void sci_AutocStops(LPCSTR pText);
	SciDll_void sci_AutocSetSeparator(char separator);
	char sci_AutocGetSeparator();
	SciDll_void sci_AutocSelect(LPCSTR pSelect);

	int sci_AutocGetCurrent();
	SciDll_void sci_AutocGetCurrentText(CStringA &Text);

	SciDll_void sci_AutocSetCancelAtStart(BOOL cancel);
	BOOL sci_AutocGetCancelAtStart();

	SciDll_void sci_AutocSetFillups(LPCSTR pText);

	SciDll_void sci_AutocSetChooseSingle(BOOL chooseSingle);
	BOOL sci_AutocGetChooseSingle();

	//是否忽略大小写
	SciDll_void sci_AutocSetIgnoreCase(BOOL ignoreCase);
	BOOL sci_AutocGetIgnoreCase();

	SciDll_void sci_AutocSetAutoHide(BOOL autoHide);
	BOOL sci_AutocGetAutoHide();

	SciDll_void sci_AutocSetDropRestOfWord(BOOL dropRestOfWord);
	BOOL sci_AutocGetDropRestOfWord();

	SciDll_void sci_RegisterImage(int type, LPCSTR pXmpData);
	SciDll_void sci_RegisterRGBAImage(int type, LPCSTR pPixels);
	SciDll_void sci_ClearRegisteredImages();
	SciDll_void sci_AutocSetTypeSeparator(char separatorCharacter);
	char sci_AutocGetTypeSeparator();

	SciDll_void sci_AutocSetMaxHeight(int rowCount);
	int sci_AutocGetMaxHeight();

	SciDll_void sci_AutocSetMaxWidth(int characterCount);
	int sci_AutocGetMaxWidth();

	/*----------User lists------------*/
	SciDll_void sci_UserListShow(int listType, LPCSTR pList);
	/*----------Call tips------------*/
	SciDll_void sci_CallTipShow(int posStart, LPCSTR pDefinition);
	SciDll_void sci_CallTipCancel();
	SciDll_void sci_CallTipActive();
	SciDll_void sci_CallTipPosStart();
	SciDll_void sci_CallTipSetHlt(int hlStart, int hlEnd);
	SciDll_void sci_CallTipSetBack(COLORREF colour);
	SciDll_void sci_CallTipSetFore(COLORREF colour);
	SciDll_void sci_CallTipSetForeHlt(COLORREF colour);
	SciDll_void sci_CallTipUseStyle(int tabsize);
	SciDll_void sci_CallTipSetPosition(BOOL above);
	/*----------Keyboard commands------------*/
	/*----------Key bindings------------*/
	SciDll_void sci_AssignCmdKey(int keyDefinition, int sciCommand);
	SciDll_void sci_ClearCmdKey(int keyDefinition);
	SciDll_void sci_ClearAllCmdKeys();
	SciDll_void sci_NullCmdKey();
	/*----------Popup edit menu------------*/
	SciDll_void sci_UsePopup(BOOL bEnablePopup);
	/*----------Macro recording------------*/
	SciDll_void sci_StartRecord();
	SciDll_void sci_StopRecord();
	/*----------Printing------------*/
	SciDll_void sci_FormatRange(BOOL bDraw, Sci_RangeToFormat *pfr);

	SciDll_void sci_SetPrintMagnification(BOOL magnification);
	BOOL sci_GetPrintMagnification();

	SciDll_void sci_SetPrintColourMode(int mode);
	int sci_GetPrintColourMode();

	SciDll_void sci_SetPrintWrapMode(int wrapMode);
	BOOL sci_GetPrintWrapMode();
	/*----------Direct access------------*/
	//SciDll_void GetDirectFuntion();
	//void *GetDirectPointer();
	//void *GetCharacterPointer();
	/*----------Multiple views------------*/
	void *sci_GetDocPointer();
	SciDll_void sci_SetDocPointer(void *pDoc);
	void *sci_CreateDocument();
	SciDll_void sci_AddRefDocument(void *pDoc);
	SciDll_void sci_ReleaseDocument(void *pDoc);
	/*----------Background loading and saving------------*/
	/*----------Folding-----------*/	
	SciDll_void sci_VisibleFromDocLine(int docLine);
	SciDll_void sci_DocLineFromVisible(int displayLine);
	SciDll_void sci_ShowLines(int lineStart, int lineEnd);
	SciDll_void sci_HideLines(int lineStart, int lineEnd);
	BOOL sci_GetLineVisible(int line);
	BOOL sci_GetAllLinesVisible();

	SciDll_void sci_SetFoldLevel(int line, int level);
	int  sci_GetFoldLevel(int line);

	SciDll_void sci_SetFoldFlags(int flags);
	int  sci_GetLastChild(int line, int level);
	int  sci_GetFoldParent(int line);

	SciDll_void sci_SetFoldExpanded(int line, BOOL expanded);
	BOOL sci_GetFoldExpanded(int line);

	int sci_ContractedFoldNext(int lineStart);
	SciDll_void sci_ToggleFold(int line);
	SciDll_void sci_EnsureVisible(int line);
	SciDll_void sci_EnsureVisibleEnforcePolicy(int line);
	/*----------Line wrapping------------*/	
	SciDll_void sci_SetWrapMode(int wrapMode);
	int  sci_GetWrapMode();

	SciDll_void sci_SetWrapVisualFlags(int wrapVisualFlags);
	int  sci_GetWrapVisualFlags();

	SciDll_void sci_SetWrapVisualFlagsLocation(int wrapVisualFlagsLocation);
	int  sci_GetWrapVisualFlagsLocation();

	SciDll_void sci_SetWrapIndentMode(int indentMode);
	int  sci_GetWrapIndentMode();

	SciDll_void sci_SetWrapStartIndent(int indent);
	int  sci_GetWrapStartIndent();

	SciDll_void sci_SetLayoutCache(int cacheMode);
	int  sci_GetLayoutCache();

	SciDll_void sci_SetPositionCache(int size);
	int  sci_GetPositionCache();

	SciDll_void sci_LinesSplit(int pixelWidth);

	SciDll_void sci_LinesJoin();

	SciDll_void sci_WrapCount(int docLine);
	/*----------Zooming------------*/
	SciDll_void sci_ZoomIn();
	SciDll_void sci_ZoomOut();
	SciDll_void sci_SetZoom(int zoomInPoints);
	int  sci_GetZoom();
	/*----------Long lines------------*/
	SciDll_void sci_SetEdgeMode(int mode);
	int  sci_GetEdgeMode();

	SciDll_void sci_SetEdgeColumn(int column);
	int  sci_GetEdgeColumn();

	SciDll_void sci_SetEdgeColour(int colour);
	int  sci_GetEdgeColour();
	/*----------Lexer------------*/
	SciDll_void sci_SetLexer(int lexer);
	int  sci_GetLexer();

	SciDll_void sci_SetLexerLanguage(LPCSTR name);
	SciDll_void sci_GetLexerLanguage(CString &Text);

	SciDll_void sci_LoadLexerLibrary(LPCSTR path);

	SciDll_void sci_ColourIse(int start, int end);

	SciDll_void sci_ChangeLexerState(int start, int end);

	SciDll_void sci_PropertyNames(LPTSTR names);

	SciDll_void sci_PropertyType(LPTSTR name);

	SciDll_void sci_DescribeProperty(LPCSTR name, LPTSTR description);

	SciDll_void sci_SetProperty(LPCSTR key, LPCSTR value);
	SciDll_void sci_GetProperty(LPCSTR key, CString &Value);

	SciDll_void sci_GetPropertyExpanded(LPCSTR key, CString &Value);

	int  sci_GetPropertyInt(LPCSTR key, int ndefault);

	SciDll_void sci_DescribeKeywordSets(CString &descriptions);

	SciDll_void sci_SetKeyWords(int keyWordSet, LPCSTR keyWordList);

	int  sci_GetStyleBitsNeeded();
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*----------------Text retrieval and modification------------------*/
//获取所有字符
inline SciDll_void CSciWnd::sci_GetTextAll(CStringA &Text)
{
	int nRet = -1;
	long lLen = sci_GetLength();
	if (lLen > 0)
	{
		nRet = sci_GetText(lLen, Text);
	}
	return nRet;
}

//获取一行字符
inline SciDll_void CSciWnd::sci_GetTextLine(int line, CStringA &Text)
{
	int nRet = -1;
	int length = sci_LineLength(line)+1;
	if(length>0)
	{
		CHAR *pReturn = new CHAR[length];
		pReturn[length-1] = 0;
		if (pReturn != NULL)
		{
			*pReturn = '\0';
			nRet = execute(SCI_GETLINE, line, (LPARAM)pReturn);
			Text = pReturn;
			delete []pReturn;
		}
	}

	return nRet;
}

//获取一定长度的字符
inline SciDll_void CSciWnd::sci_GetText(int length, CStringA &Text)
{
	int nRet = -1;
	char *pReturn = new char[length + 1];
	if (pReturn != NULL)
	{
		*pReturn = '\0';
		nRet = execute(SCI_GETTEXT, length + 1, (LPARAM)pReturn);
		Text = pReturn;
		delete []pReturn;
	}
	return nRet;
}

//设置字符到控件
inline SciDll_void CSciWnd::sci_SetText(LPCSTR pText)
{
	return execute(SCI_SETTEXT, 0, (LPARAM)pText);
}

//设置文档已保存的标志
inline SciDll_void CSciWnd::sci_SetSavePoint()
{
	return execute(SCI_SETSAVEPOINT,0,0);
}

//替换已选择字符
inline SciDll_void CSciWnd::sci_ReplaceSel(LPCSTR pText)
{
	return execute(SCI_REPLACESEL,0,(LPARAM)pText);
}

//设置只读模式
inline SciDll_void CSciWnd::sci_SetReadOnly(BOOL bReadOnly)
{
	return execute(SCI_SETREADONLY,bReadOnly,0);
}

//获取是否只读模式
inline BOOL CSciWnd::sci_GetReadOnly()
{
	return execute(SCI_GETREADONLY,0,0);
}

//获取范围内的字符
inline int CSciWnd::sci_GetTextRange(int cpMin, int cpMax, CStringA &pTextRange)
{
	Sci_TextRange tr;
	tr.chrg.cpMin = cpMin;
	tr.chrg.cpMax = cpMax;
	tr.lpstrText = new char[cpMax-cpMin+2];

	int len = execute(SCI_GETTEXTRANGE,0,(LPARAM)&tr);
	pTextRange = tr.lpstrText;

	delete []tr.lpstrText;
	return len;
}

inline SciDll_void CSciWnd::sci_GetStyledText(int cpMin, int cpMax, CStringA &pText)
{
	Sci_TextRange tr;
	tr.chrg.cpMin = cpMin;
	tr.chrg.cpMax = cpMax;
	tr.lpstrText = new char[2*(cpMax-cpMin)+2];

	execute(SCI_GETTEXTRANGE,0,(LPARAM)&tr);
	pText = tr.lpstrText;

	delete []tr.lpstrText;
}

//设置文档缓冲区大小, 使创建的文档不会小于当前的文档
inline SciDll_void CSciWnd::sci_Allocate(int bytes)
{
	return execute(SCI_ALLOCATE, bytes,0);
}

//在光标位置插入字符串
inline SciDll_void CSciWnd::sci_AddText(int length, LPCSTR pText)
{
	return execute(SCI_ADDTEXT,length,(LPARAM)pText);
}

//在光标位置插入包含字符格式的字符串
inline SciDll_void CSciWnd::sci_AddStyledText(int length, LPCSTR pText)
{
	return execute(SCI_ADDSTYLEDTEXT,length,(LPARAM)pText);
}

//在文档末尾插入字符串
inline SciDll_void CSciWnd::sci_AppendText(int length, LPCSTR pText)
{
	return execute(SCI_APPENDTEXT,length,(LPARAM)pText);
}

//在某个位置插入字符串.如果pos=-1,直接在光标位置插入
inline SciDll_void CSciWnd::sci_InsertText(int pos, LPCVOID pText)
{
	return execute(SCI_INSERTTEXT,pos,(LPARAM)pText);
}

//清空文档所有字符
inline SciDll_void CSciWnd::sci_ClearTextAll()
{
	return execute(SCI_CLEARALL,0,0);
}

//删除范围内的字符
inline SciDll_void CSciWnd::sci_DeleteRange(int pos, int deleteLength)
{
	return execute(SCI_DELETERANGE, pos, deleteLength);
}

//清空文档字符的所有格式()
inline SciDll_void CSciWnd::sci_ClearDocunmentStyle()
{
	return execute(SCI_CLEARDOCUMENTSTYLE,0,0);
}

//获取某个位置的字符
inline char CSciWnd::sci_GetCharAt(int pos)
{
	return (char)execute(SCI_GETCHARAT,pos,0);
}

//获取某个位置的字符字体风格
inline int CSciWnd::sci_GetStyleAt(int pos)
{
	return execute(SCI_GETSTYLEAT,pos,0);
}

//设置字符字体风格
inline SciDll_void CSciWnd::sci_SetStyleBits(int bits)
{
	return execute(SCI_SETSTYLEBITS,bits,0);
}

//获取字符字体风格
inline int CSciWnd::sci_GetStyleBits()
{
	return execute(SCI_GETSTYLEBITS,0,0);	
}

inline int CSciWnd::sci_TargetAsUTF8(LPTSTR pText)
{
	return execute(SCI_TARGETASUTF8,0,(LPARAM)pText);	
}

inline int CSciWnd::sci_EncodedFromUTF8(LPCSTR pUTF8, LPTSTR pText)
{
	return execute(SCI_ENCODEDFROMUTF8,(LPARAM)pUTF8,(LPARAM)pText);	
}

inline SciDll_void CSciWnd::sci_SetLengthForEncode(int bits)
{
	return execute(SCI_SETLENGTHFORENCODE,bits,0);
}
//////////////////////////////////////////////////////////////////////////
/*--------------查找-----------------*/
inline int CSciWnd::sci_FindText(int searchFlags, Sci_TextToFind &ttf)
{
	return execute(SCI_FINDTEXT,searchFlags,(LPARAM)&ttf);
}

inline SciDll_void CSciWnd::sci_SearchAnchor()
{
	return execute(SCI_SEARCHANCHOR,0,0);
}

inline int CSciWnd::sci_SearchNext(int searchFlags, LPCSTR pText)
{
	return execute(SCI_SEARCHNEXT,searchFlags,(LPARAM)pText);
}

inline int CSciWnd::sci_SearchPrev(int searchFlags, LPCSTR pText)
{
	return execute(SCI_SEARCHPREV,searchFlags,(LPARAM)pText);	
}


//////////////////////////////////////////////////////////////////////////
/*----查找和替换----*/
inline SciDll_void CSciWnd::sci_SetTargetStart(int pos)
{
	return execute(SCI_SETTARGETSTART,pos,0);
}

inline int  CSciWnd::sci_GetTargetStart()
{
	return execute(SCI_GETTARGETSTART,0,0);
}

inline SciDll_void CSciWnd::sci_SetTargetEnd(int pos)
{
	return execute(SCI_SETTARGETEND,pos,0);	
}

inline int CSciWnd::sci_GetTargetEnd()
{
	return execute(SCI_GETTARGETEND,0,0);	
}

inline SciDll_void CSciWnd::sci_TargetFromSelection()
{
	return execute(SCI_TARGETFROMSELECTION,0,0);	
}

inline SciDll_void CSciWnd::sci_SetSearchFlags(int searchFlags)
{
	return execute(SCI_SETSEARCHFLAGS,searchFlags,0);	
}

inline int  CSciWnd::sci_GetSearchFlags()
{
	return execute(SCI_GETSEARCHFLAGS,0,0);;	
}

inline int CSciWnd::sci_SearchInTarget(int length, LPCSTR pText)
{
	return execute(SCI_SEARCHINTARGET,length,(LPARAM)pText);	
}

inline int CSciWnd::sci_ReplaceTarget(int length, const char *pText)
{
	return execute(SCI_REPLACETARGET,length,(LPARAM)pText);	
}

inline int CSciWnd::sci_ReplaceTargetRe(int length, LPCSTR pText)
{
	return execute(SCI_REPLACETARGETRE,length,(LPARAM)pText);	
}

inline SciDll_void CSciWnd::sci_GetTag(int tagNumber, LPTSTR pText)
{
	return execute(SCI_GETTAG,tagNumber,(LPARAM)pText);
}


//////////////////////////////////////////////////////////////////////////
/*----覆盖模式(相反的是插入模式)----*/
//设置是否覆盖模式
inline SciDll_void CSciWnd::sci_OverType_SetOverType(BOOL bOverType)
{
	return execute(SCI_SETOVERTYPE,bOverType,0);
}

//获取是否覆盖模式
inline BOOL CSciWnd::sci_OverType_GetOverType()
{
	return execute(SCI_GETOVERTYPE,0,0);
}


//////////////////////////////////////////////////////////////////////////
/*----剪切,复制,粘贴,Redo,Undo----*/
inline SciDll_void CSciWnd::sci_Cut()
{
	return execute(SCI_CUT,0,0);
}

inline SciDll_void CSciWnd::sci_Copy()
{
	return execute(SCI_COPY,0,0);	
}

inline SciDll_void CSciWnd::sci_Paste()
{
	return execute(SCI_PASTE,0,0);	
}

//返回是否可以复制
inline BOOL CSciWnd::sci_CanCopy()
{
	return sci_GetSelectionStart()-sci_GetSelectionEnd() != 0;	
}

//返回是否可以剪切
inline BOOL CSciWnd::sci_CanCut()
{
	return sci_GetSelectionStart()-sci_GetSelectionEnd() != 0;
}

//返回是否可以粘贴
inline BOOL CSciWnd::sci_CanPaste()
{
	return execute(SCI_CANPASTE,0,0);
}

//删除选中内容
inline SciDll_void CSciWnd::sci_Clear()
{
	return execute(SCI_CLEAR,0,0);
}

//类似Copy,区别是当选择内容为空时,复制当前整行内容
inline SciDll_void CSciWnd::sci_CopyAllowLine()
{
	return execute(SCI_COPYALLOWLINE,0,0);
}

inline SciDll_void CSciWnd::sci_CopyRange(int start, int end)
{
	return execute(SCI_COPYRANGE,start,end);
}

inline SciDll_void CSciWnd::sci_CopyText(int length, LPCSTR pText)
{
	return execute(SCI_COPYTEXT,length,(LPARAM)pText);
}

inline SciDll_void CSciWnd::sci_SetPasteConvertEndings(BOOL convert)
{
	return execute(SCI_SETPASTECONVERTENDINGS,convert,0);
}

inline BOOL CSciWnd::sci_GetPasteConvertEndings()
{
	return execute(SCI_GETPASTECONVERTENDINGS,0,0);
}

inline SciDll_void CSciWnd::sci_Undo()
{
	return execute(SCI_UNDO,0,0);
}

inline BOOL CSciWnd::sci_CanUndo()
{
	return execute(SCI_CANUNDO,0,0);;
}

inline SciDll_void CSciWnd::sci_EmptyUndoBuffer()
{
	return execute(SCI_EMPTYUNDOBUFFER,0,0);
}

inline SciDll_void CSciWnd::sci_Redo()
{
	return execute(SCI_REDO,0,0);
}

inline BOOL CSciWnd::sci_CanRedo()
{
	return execute(SCI_CANREDO,0,0);
}

inline SciDll_void CSciWnd::sci_SetUndoCollection(BOOL collectUndo)
{
	return execute(SCI_SETUNDOCOLLECTION,collectUndo,0);
}

inline BOOL CSciWnd::sci_GetUndoCollection()
{
	return execute(SCI_GETUNDOCOLLECTION,0,0);;
}

inline SciDll_void CSciWnd::sci_BeginUndoAction()
{
	return execute(SCI_BEGINUNDOACTION,0,0);
}

inline SciDll_void CSciWnd::sci_EndUndoAction()
{
	return execute(SCI_ENDUNDOACTION,0,0);
}

inline SciDll_void CSciWnd::sci_AddUndoAction(int token, int flags)
{
	return execute(SCI_ADDUNDOACTION,token,flags);
}


//////////////////////////////////////////////////////////////////////////
/*----Error Handing----*/ 
inline SciDll_void CSciWnd::sci_SetStatus(int status)
{
	return execute(SCI_SETSTATUS,status,0);
}

inline int  CSciWnd::sci_GetStatus()
{
	return execute(SCI_GETSTATUS,0,0);
}

//////////////////////////////////////////////////////////////////////////
/*----选中内容信息----*/
inline int  CSciWnd::sci_GetTextLength()
{
	return execute(SCI_GETTEXTLENGTH,0,0);
}

inline int  CSciWnd::sci_GetLength()
{
	return execute(SCI_GETLENGTH,0,0);
}

inline int  CSciWnd::sci_GetLineCount()
{
	return execute(SCI_GETLINECOUNT,0,0);
}

inline SciDll_void CSciWnd::sci_SetFirstVisibleLine(int lineDisplay)
{
	return execute(SCI_SETFIRSTVISIBLELINE,lineDisplay,0);
}

inline int  CSciWnd::sci_GetFirstVisibleLine()
{
	return execute(SCI_GETFIRSTVISIBLELINE,0,0);;	
}

inline int  CSciWnd::sci_LineSonScreen()
{
	return execute(SCI_LINESONSCREEN,0,0);;	
}

inline BOOL  CSciWnd::sci_GetModify()
{
	return execute(SCI_GETMODIFY,0,0);;
}

inline SciDll_void CSciWnd::sci_SetSel(int anchorPos, int currentPos)
{
	return execute(SCI_SETSEL,anchorPos,currentPos);
}

inline SciDll_void CSciWnd::sci_GoToPos(int pos)
{
	return execute(SCI_GOTOPOS,pos,0);
}

inline SciDll_void CSciWnd::sci_GoToLine(int line)
{
	return execute(SCI_GOTOLINE,line,0);
}

inline SciDll_void CSciWnd::sci_SetCurrentPos(int pos)
{
	return execute(SCI_SETCURRENTPOS,pos,0);
}

inline int  CSciWnd::sci_GetCurrentPos()
{
	return execute(SCI_GETCURRENTPOS,0,0);
}

inline SciDll_void CSciWnd::sci_SetAnchor(int pos)
{
	return execute(SCI_SETANCHOR,pos,0);
}

inline int  CSciWnd::sci_GetAnchor()
{
	return execute(SCI_GETANCHOR,0,0);
}

inline SciDll_void CSciWnd::sci_SetSelectionStart(int pos)
{
	return execute(SCI_SETSELECTIONSTART,pos,0);
}


inline SciDll_void CSciWnd::sci_SetSelectionEnd(int pos)
{
	return execute(SCI_SETSELECTIONEND,pos,0);
}

inline int  CSciWnd::sci_GetSelectionStart()
{
	return execute(SCI_GETSELECTIONSTART,0,0);
}

inline int  CSciWnd::sci_GetSelectionEnd()
{
	return execute(SCI_GETSELECTIONEND,0,0);
}

inline SciDll_void CSciWnd::sci_SetEmptySelection(int pos)
{
	return execute(SCI_SETEMPTYSELECTION,pos,0);
}

inline SciDll_void CSciWnd::sci_SelectAll()
{
	return execute(SCI_SELECTALL,0,0);
}

inline int  CSciWnd::sci_LineFromPosition(int pos)
{
	return execute(SCI_LINEFROMPOSITION,pos,0);
}

inline int  CSciWnd::sci_PositionFromLine(int line)
{
	return execute(SCI_POSITIONFROMLINE,line,0);
}

inline int  CSciWnd::sci_GetLineEndPosition(int line)
{
	return execute(SCI_GETLINEENDPOSITION,line,0);
}

inline int  CSciWnd::sci_LineLength(int line)
{
	return execute(SCI_LINELENGTH,line,0);
}

inline SciDll_void CSciWnd::sci_GetSelText(CStringA &Text)
{
	int nRet = -1;
	nRet = execute(SCI_GETSELTEXT,0,0);
	if(nRet>0)
	{
		char *p = new char[nRet];
		nRet = execute(SCI_GETSELTEXT,0,(LPARAM)p);
		Text = p;
		delete []p;
	}

	return nRet;
}

inline int  CSciWnd::sci_GetCurLine(CStringA &Text)
{
	int pos = sci_GetCurrentPos();
	if(pos < 0)
		return -1;

	int line = sci_LineFromPosition(pos);
	if(line < 0)
		return -1;

	sci_GetTextLine(line, Text);

	return line;
}

//返回当前的行号
inline int  CSciWnd::sci_GetCurLine()
{
	int pos = sci_GetCurrentPos();
	int line = sci_LineFromPosition(pos);
	return line;
}

inline BOOL CSciWnd::sci_GetSelectionIsRectangle()
{
	return execute(SCI_SELECTIONISRECTANGLE,0,0);
}

inline SciDll_void CSciWnd::sci_SetSelectionMode(int mode)
{
	execute(SCI_SETSELECTIONMODE,mode,0);
}

inline int  CSciWnd::sci_GetSelectionMode()
{
	return execute(SCI_GETSELECTIONMODE,0,0);
}

inline int  CSciWnd::sci_GetLineSelStartPosition(int line)
{
	return execute(SCI_GETLINESELSTARTPOSITION,line,0);
}

inline int  CSciWnd::sci_GetLineSelEndPosition(int line)
{
	return execute(SCI_GETLINESELENDPOSITION,line,0);
}

inline SciDll_void CSciWnd::sci_MoveCaretinSideView()
{
	execute(SCI_MOVECARETINSIDEVIEW,0,0);
}

inline int  CSciWnd::sci_WordEndPosition(int position, BOOL onlyWordCharacters)
{
	return execute(SCI_WORDENDPOSITION,position,onlyWordCharacters);
}

inline int  CSciWnd::sci_WordStartPosition(int position, BOOL onlyWordCharacters)
{
	return execute(SCI_WORDSTARTPOSITION,position,onlyWordCharacters);
}

inline int  CSciWnd::sci_PositionBefore(int postion)
{
	return execute(SCI_POSITIONBEFORE,postion,0);
}

inline int  CSciWnd::sci_PostionAfter(int postion)
{
	return execute(SCI_POSITIONAFTER,postion,0);
}

inline int  CSciWnd::sci_CountCharacters(int startPos, int endPos)
{
	return execute(SCI_COUNTCHARACTERS,startPos,endPos);
}

inline int  CSciWnd::sci_TextWidth(int styleNumber, LPCSTR pText)
{
	return execute(SCI_TEXTWIDTH,styleNumber,(LPARAM)pText);
}

inline int  CSciWnd::sci_TextHeight(int line)
{
	return execute(SCI_TEXTHEIGHT,line,0);
}

inline int  CSciWnd::sci_GetColumn(int pos)
{
	return execute(SCI_GETCOLUMN,pos,0);
}

inline int  CSciWnd::sci_FindColumn(int line, int column)
{
	return execute(SCI_FINDCOLUMN,line,column);
}

inline int  CSciWnd::sci_PostionFromPoint(int x, int y)
{
	return execute(SCI_POSITIONFROMPOINT,x,y);
}

inline int  CSciWnd::sci_PostionFromPointClose(int x, int y)
{
	return execute(SCI_POSITIONFROMPOINTCLOSE,x,y);
}

inline int  CSciWnd::sci_CharPositionFromPoint(int x, int y)
{
	return execute(SCI_CHARPOSITIONFROMPOINT,x,y);
}

inline int  CSciWnd::sci_CharPositionFromPointClose(int x, int y)
{
	return execute(SCI_CHARPOSITIONFROMPOINTCLOSE,x,y);
}

inline int  CSciWnd::sci_PointXFromPosition(int pos)
{
	return execute(SCI_POINTXFROMPOSITION,pos,0);
}

inline int  CSciWnd::sci_PointYFromPosition(int pos)
{
	return execute(SCI_POINTYFROMPOSITION,pos,0);
}

inline SciDll_void CSciWnd::sci_HideSelection(BOOL hide)
{
	execute(SCI_HIDESELECTION,hide,0);
}

inline SciDll_void CSciWnd::sci_ChooseCaretX()
{
	execute(SCI_CHOOSECARETX,0,0);
}

inline SciDll_void CSciWnd::sci_MoveSelectedLinesUp()
{
	execute(SCI_MOVESELECTEDLINESUP,0,0);
}

inline SciDll_void CSciWnd::sci_MoveSlectionLinesDown()
{
	execute(SCI_MOVESELECTEDLINESDOWN,0,0);
}

//////////////////////////////////////////////////////////////////////////
/*----Multiple Selection and Virtual Space----*/

/*-----------滚动条--------*/
inline SciDll_void CSciWnd::sci_LineScroll(int column, int line)
{
	return execute(SCI_LINESCROLL,column,line);
}

inline SciDll_void CSciWnd::sci_ScrollCaret()
{
	return execute(SCI_SCROLLCARET);
}

inline SciDll_void CSciWnd::sci_SetXCaretPolicy(int caretPolicy, int caretSlop)
{
	return execute(SCI_SETXCARETPOLICY,caretPolicy,caretSlop);
}

inline SciDll_void CSciWnd::sci_SetYCaretPolicy(int caretPolicy, int caretSlop)
{
	return execute(SCI_SETYCARETPOLICY, caretPolicy, caretSlop);
}

inline SciDll_void CSciWnd::sci_SetVisiblePolicy(int caretPolicy, int caretSlop)
{
	return execute(SCI_SETVISIBLEPOLICY, caretPolicy, caretSlop);
}

inline SciDll_void CSciWnd::sci_SetHScrollBar(BOOL visible)
{
	return execute(SCI_SETHSCROLLBAR, visible, 0);
}

inline BOOL CSciWnd::sci_GetHScrollBar()
{
	return execute(SCI_GETHSCROLLBAR);
}

inline SciDll_void CSciWnd::sci_SetVScrollBar(BOOL visible)
{
	return execute(SCI_SETVSCROLLBAR,visible,0);
}

inline BOOL CSciWnd::sci_GetVScrollBar()
{
	return execute(SCI_GETVSCROLLBAR);
}

inline SciDll_void CSciWnd::sci_SetXOffset(int xOffset)
{
	return execute(SCI_SETXOFFSET,xOffset,0);
}

inline int  CSciWnd::sci_GetXOffset()
{
	return execute(SCI_GETXOFFSET);
}

inline SciDll_void CSciWnd::sci_SetScrollWidth(int pixelWidth)
{
	return execute(SCI_SETSCROLLWIDTH,pixelWidth,0);
}

inline int  CSciWnd::sci_GetScrollWidth()
{
	return execute(SCI_GETSCROLLWIDTH);
}

inline SciDll_void CSciWnd::sci_SetScrollWidthTracking(BOOL tracking)
{
	return execute(SCI_SETSCROLLWIDTHTRACKING,tracking,0);
}

inline BOOL CSciWnd::sci_GetScrollWidthTracking()
{
	return execute(SCI_GETSCROLLWIDTHTRACKING);
}

inline SciDll_void CSciWnd::sci_SetEndAtLastLine(BOOL endAtLastLine)
{
	return execute(SCI_SETENDATLASTLINE);
}

inline BOOL CSciWnd::sci_GetEndAtLastLine()
{
	return execute(SCI_GETENDATLASTLINE);
}


//////////////////////////////////////////////////////////////////////////
/*------White space-------*/
inline SciDll_void CSciWnd::sci_SetViewWs(int wsMode)
{
	return execute(SCI_SETVIEWWS,wsMode,0);
}

inline BOOL CSciWnd::sci_GetViewWs()
{
	return execute(SCI_GETVIEWWS);
}

inline SciDll_void CSciWnd::sci_SetWhiteSpaceFore(BOOL useWhitespaceForeColour, COLORREF colour)
{
	return execute(SCI_SETWHITESPACEFORE,useWhitespaceForeColour,colour);
}

inline SciDll_void CSciWnd::sci_SetWhiteSpaceBack(BOOL useWhitespaceBackColour, COLORREF colour)
{
	return execute(SCI_SETWHITESPACEBACK,useWhitespaceBackColour,colour);
}

inline SciDll_void CSciWnd::sci_SetWhiteSpaceSize(int size)
{
	return execute(SCI_SETWHITESPACESIZE,size,0);
}

inline int  CSciWnd::sci_GetWhiteSpaceSize()
{
	return execute(SCI_GETWHITESPACESIZE);
}

inline SciDll_void CSciWnd::sci_SetExtraAscent(int extraAscent)
{
	return execute(SCI_SETEXTRAASCENT,extraAscent,0);
}

inline BOOL CSciWnd::sci_GetExtraAscent()
{
	return execute(SCI_GETEXTRAASCENT);
}

inline SciDll_void CSciWnd::sci_SetExtraDescent(int extraDescent)
{
	return execute(SCI_SETEXTRADESCENT,extraDescent,extraDescent);
}

inline BOOL CSciWnd::sci_GetExtraDescent()
{
	return execute(SCI_GETEXTRAASCENT);
}

//////////////////////////////////////////////////////////////////////////
/*---------鼠标指针------*/
inline SciDll_void CSciWnd::sci_SetCursor(int curType)
{
	return execute(SCI_SETCURSOR);
}

inline int  CSciWnd::sci_GetCursor()
{
	return execute(SCI_GETCURSOR);
}


//////////////////////////////////////////////////////////////////////////
/*---------Mouse capture-------*/
inline SciDll_void CSciWnd::sci_SetMouseDownCaptures(BOOL captures)
{
	return execute(SCI_SETMOUSEDOWNCAPTURES,captures,0);
}

inline BOOL CSciWnd::sci_GetMouseDownCaptures()
{
	return execute(SCI_GETMOUSEDOWNCAPTURES);
}

//////////////////////////////////////////////////////////////////////////
/*----------Line Endings-------*/
inline SciDll_void CSciWnd::sci_SetEolMode(int eolMode)
{
	return execute(SCI_SETEOLMODE,eolMode,0);
}

inline int  CSciWnd::sci_GetEolMode()
{
	return execute(SCI_GETEOLMODE);
}

inline SciDll_void CSciWnd::sci_ConvertEols(int eolMode)
{
	return execute(SCI_CONVERTEOLS,eolMode,0);
}

inline SciDll_void CSciWnd::sci_SetViewEol(BOOL visible)
{
	return execute(SCI_SETVIEWEOL,visible,0);
}

inline BOOL CSciWnd::sci_GetViewEol()
{
	return execute(SCI_GETVIEWEOL);
}


//////////////////////////////////////////////////////////////////////////
/*----------Styling------------*/
inline int CSciWnd::sci_GetEndStyled()
{
	return execute(SCI_GETENDSTYLED);
}

inline SciDll_void CSciWnd::sci_StartStyling(int pos, int mask)
{
	return execute(SCI_STARTSTYLING,pos,mask);
}

inline SciDll_void CSciWnd::sci_SetStyling(int length, int style)
{
	return execute(SCI_SETSTYLING,length,style);
}

inline SciDll_void CSciWnd::sci_SetStylingEx(int length, LPCSTR styles)
{
	return execute(SCI_SETSTYLINGEX,length,(LPARAM)styles);
}

inline SciDll_void CSciWnd::sci_SetLineState(int line, int value)
{
	return execute(SCI_SETLINESTATE,line,value);
}

inline int  CSciWnd::sci_GetLineState(int line)
{
	return execute(SCI_GETLINESTATE,line,0);
}

inline int  CSciWnd::sci_GetMaxLineState()
{
	return execute(SCI_GETMAXLINESTATE);
}

//////////////////////////////////////////////////////////////////////////
/*----------Style definition---*/
inline SciDll_void CSciWnd::sci_StyleResetDefault()
{
	return execute(SCI_STYLERESETDEFAULT);	
}

//把所有格式设置成STYLE_DEFAULT
inline SciDll_void CSciWnd::sci_StyleClearAll()
{
	return execute(SCI_STYLECLEARALL);
}

//设置字体
inline SciDll_void CSciWnd::sci_StyleSetFont(int styleNumer, LPCSTR fontName)
{
	return execute(SCI_STYLESETFONT,styleNumer,(LPARAM)fontName);	
}

inline SciDll_void CSciWnd::sci_StyleGetFont(int styleNumer, CString &fontName)
{
	int nRet = -1;
	char *p = new char[33];
	nRet = execute(SCI_STYLEGETFONT,styleNumer,(LPARAM)p);
	fontName = p;
	delete []p;
	return nRet;
}

//设置字体大小
inline SciDll_void CSciWnd::sci_StyleSetSize(int styleNumber, int sizeInPoints)
{
	return execute(SCI_STYLESETSIZE,styleNumber,sizeInPoints);
}

inline int  CSciWnd::sci_StyleGetSize(int styleNumber)
{
	return execute(SCI_STYLEGETSIZE,styleNumber,0);
}

//字体大小包含小数点的
inline SciDll_void CSciWnd::sci_StyleSetSizeFractional(int styleNumber, int sizeInHundredthPoints)
{
	return execute(SCI_STYLESETSIZEFRACTIONAL,styleNumber,sizeInHundredthPoints);
}

inline int  CSciWnd::sci_StyleGetSizeFractional(int styleNumber)
{
	return execute(SCI_STYLEGETSIZEFRACTIONAL,styleNumber,0);
}

//粗体
inline SciDll_void CSciWnd::sci_StyleSetBold(int styleNumber, bool bold)
{
	return execute(SCI_STYLESETBOLD,styleNumber,bold);
}

inline BOOL  CSciWnd::sci_StyleGetBold(int styleNumber)
{
	return execute(SCI_STYLEGETBOLD,styleNumber,0);
}

//字体粗细
inline SciDll_void CSciWnd::sci_StyleSetWeight(int styleNumber, int weight)
{
	return execute(SCI_STYLESETWEIGHT,styleNumber,weight);
}

inline int  CSciWnd::sci_StyleGetWeight(int styleNumber)
{
	return execute(SCI_STYLEGETWEIGHT,styleNumber,0);	
}

//斜体
inline SciDll_void CSciWnd::sci_StyleSetItalic(int styleNumber, BOOL italic)
{
	return execute(SCI_STYLESETITALIC,styleNumber,italic);
}

inline BOOL CSciWnd::sci_StyleSetItalic(int styleNumber)
{
	return execute(SCI_STYLEGETITALIC,styleNumber,0);
}

//下划线
inline SciDll_void CSciWnd::sci_StyleSetUnderline(int styleNumber, BOOL underline)
{
	return execute(SCI_STYLESETUNDERLINE,styleNumber,underline);
}

inline BOOL CSciWnd::sci_StyleGetUnderline(int styleNumber)
{
	return execute(SCI_STYLEGETUNDERLINE,styleNumber,0);
}

//字符前景色
inline SciDll_void CSciWnd::sci_StyleSetFore(int styleNumber, COLORREF colour)
{
	return execute(SCI_STYLESETFORE,styleNumber,colour);
}

inline COLORREF  CSciWnd::sci_StyleGetFore(int styleNumber)
{
	return execute(SCI_STYLEGETFORE,styleNumber,0);
}

//字符背景色
inline SciDll_void CSciWnd::sci_StyleSetBack(int styleNumber, COLORREF colour)
{
	return execute(SCI_STYLESETBACK,styleNumber,colour);
}

inline COLORREF  CSciWnd::sci_StyleGetBack(int styleNumber)
{
	return execute(SCI_STYLEGETBACK,styleNumber,0);
}

//行尾的格式
inline SciDll_void CSciWnd::sci_StyleSetEolFilled(int styleNumber, BOOL eolFilled)
{
	return execute(SCI_STYLESETEOLFILLED,styleNumber,eolFilled);
}

inline int  CSciWnd::sci_StyleGetEolFilled(int styleNumber)
{
	return execute(SCI_STYLEGETEOLFILLED,styleNumber,0);
}

//设置字符集
inline SciDll_void CSciWnd::sci_StyleSetCharacterSet(int styleNumber, int charSet)
{
	return execute(SCI_STYLESETCHARACTERSET,styleNumber,charSet);
}

inline int  CSciWnd::sci_StyleGetCharacterSet(int styleNumber)
{
	return execute(SCI_STYLEGETCHARACTERSET,styleNumber,0);
}

//设置大小写,caseMode=SC_CASE_UPPER,SC_CASE_LOWER,SC_CASE_MIXED
inline SciDll_void CSciWnd::sci_StyleSetCase(int styleNumber, int caseMode)
{
	return execute(SCI_STYLESETCASE,styleNumber,caseMode);
}

inline int  CSciWnd::sci_StyleGetCase(int styleNumber)
{
	return execute(SCI_STYLEGETCASE,styleNumber,0);
}

//是否可见
inline SciDll_void CSciWnd::sci_StyleSetVisible(int styleNumber, BOOL visible)
{
	return execute(SCI_STYLESETVISIBLE,styleNumber,visible);
}

inline BOOL CSciWnd::sci_StyleGetVisible(int styleNumber)
{
	return execute(SCI_STYLEGETVISIBLE,styleNumber);
}

//不允许修改
inline SciDll_void CSciWnd::sci_StyleSetChangeable(int styleNumber, BOOL changeable)
{
	return execute(SCI_STYLESETCHANGEABLE,styleNumber,changeable);
}

inline BOOL CSciWnd::sci_StyleGetChangeable(int styleNumber)
{
	return execute(SCI_STYLEGETCHANGEABLE,styleNumber,0);
}

//设置热点区域，类似于HTML的超链接，鼠标移过去时，鼠标变成手型，前景色和背景色可以改变，并且有下划线，允许被点击
inline SciDll_void CSciWnd::sci_StyleSetHotspot(int styleNumber, BOOL hotspot)
{
	return execute(SCI_STYLESETHOTSPOT,styleNumber,hotspot);
}

inline BOOL CSciWnd::sci_StyleGetHotspot(int styleNumber)
{
	return execute(SCI_STYLEGETHOTSPOT,styleNumber,0);
}

//////////////////////////////////////////////////////////////////////////
/*----Caret, selection, and hotspot styles-----*/
//设置当前选中内容的前景色
inline SciDll_void CSciWnd::sci_SetSelFore(BOOL useSelForeColour, COLORREF colour)
{
	return execute(SCI_SETSELFORE, useSelForeColour,colour);
}

//设置当前选中内容的背景色
inline SciDll_void CSciWnd::sci_SetSelBack(BOOL useSelBackColour, COLORREF colour)
{
	return execute(SCI_SETSELBACK,useSelBackColour,colour);
}

//设置当前选中内容的通明度
inline SciDll_void CSciWnd::sci_SetSelAlpha(int alpha)
{
	return execute(SCI_SETSELALPHA,alpha,0);
}

inline int CSciWnd::sci_GetSelAlpha()
{
	return execute(SCI_GETSELALPHA);
}

inline SciDll_void CSciWnd::sci_SetSelEolFilled(BOOL filled)
{
	return execute(SCI_SETSELEOLFILLED,filled,0);
}

inline BOOL CSciWnd::sci_GetSelEolFilled()
{
	return execute(SCI_GETSELEOLFILLED);
}

//???
inline SciDll_void CSciWnd::sci_SetCaretFore(COLORREF colour)
{
	return execute(SCI_SETCARETFORE,colour,0);
}

inline COLORREF CSciWnd::sci_GetCaretFore()
{
	return execute(SCI_GETCARETFORE);
}

inline SciDll_void CSciWnd::sci_SetCaretLineVisible(BOOL show)
{
	return execute(SCI_SETCARETLINEVISIBLE,show,0);
}

inline BOOL CSciWnd::sci_GetCaretLineVisible()
{
	return execute(SCI_GETCARETLINEVISIBLE);
}

inline SciDll_void CSciWnd::sci_SetCaretLineBack(COLORREF colour)
{
	return execute(SCI_SETCARETLINEBACK,colour,0);
}

inline COLORREF CSciWnd::sci_GetCaretLineBack()
{
	return execute(SCI_GETCARETLINEBACK);
}

inline SciDll_void CSciWnd::sci_SetCaretLineBackAlpha(int alpha)
{
	return execute(SCI_SETCARETLINEBACKALPHA,alpha,0);
}

inline int CSciWnd::sci_GetCaretLineAlpha()
{
	return execute(SCI_GETCARETLINEBACKALPHA);
}

//设置符号的闪动，millsends是可见和隐藏的切换时间,默认500
inline SciDll_void CSciWnd::sci_SetCaretPeriod(int millseconds)
{
	return execute(SCI_SETCARETPERIOD, millseconds, 0);
}

inline int CSciWnd::sci_GetCaretPeriod()
{
	return execute(SCI_GETCARETPERIOD);
}

inline SciDll_void CSciWnd::sci_SetCaretStyle(int style)
{
	return execute(SCI_SETCARETSTYLE, style, 0);
}

inline int CSciWnd::sci_GetCaretStyle()
{
	return execute(SCI_GETCARETSTYLE);
}

inline SciDll_void CSciWnd::sci_SetCaretWidth(int pixels)
{
	return execute(SCI_SETCARETWIDTH, pixels, 0);
}

inline int CSciWnd::sci_GetCaretWidth()
{
	return execute(SCI_GETCARETWIDTH);
}

inline SciDll_void CSciWnd::sci_SetHotSpotActiveFore(BOOL useHotSpotForeColour, COLORREF colour)
{
	return execute(SCI_SETHOTSPOTACTIVEFORE,useHotSpotForeColour,colour);
}

inline COLORREF CSciWnd::sci_GetHotSpotActiveFore()
{
	return execute(SCI_GETHOTSPOTACTIVEFORE);
}

inline SciDll_void CSciWnd::sci_SetHotSpotActiveBack(BOOL useHotSpotBackColour, COLORREF colour)
{
	return execute(SCI_SETHOTSPOTACTIVEBACK,useHotSpotBackColour,colour);
}

inline COLORREF CSciWnd::sci_GetHotSpotActiveBack()
{
	return execute(SCI_GETHOTSPOTACTIVEBACK);
}

inline SciDll_void CSciWnd::sci_SetHotSpotActiveUnderline(BOOL underLine)
{
	return execute(SCI_SETHOTSPOTACTIVEUNDERLINE,underLine,0);
}

inline BOOL CSciWnd::sci_GetHotSpotActiveUnderline()
{
	return execute(SCI_GETHOTSPOTACTIVEUNDERLINE);
}

inline SciDll_void CSciWnd::sci_SetHotSpotSingleLine(BOOL singelLine)
{
	return execute(SCI_SETHOTSPOTSINGLELINE,singelLine,0);
}

inline BOOL CSciWnd::sci_GetHotSpotSingleLine()
{
	return execute(SCI_GETHOTSPOTSINGLELINE);
}

inline SciDll_void CSciWnd::sci_SetControlCharSymbol(int symbol)
{
	return execute(SCI_SETCONTROLCHARSYMBOL,symbol,0);
}

inline int CSciWnd::sci_GetControlCharSymbol()
{
	return execute(SCI_GETCONTROLCHARSYMBOL);
}

//参数: SC_CARETSTICKY_OFF(0),SC_CARETSTICKY_ON(1),SC_CARETSTICKY_WHITESPACE(2),
inline SciDll_void CSciWnd::sci_SetCaretSticky(int useCaretStickyBehaviour)
{
	return execute(SCI_SETCARETSTICKY, useCaretStickyBehaviour, 0);
}

inline int CSciWnd::sci_GetCaretSticky()
{
	return execute(SCI_GETCARETSTICKY);
}

inline SciDll_void CSciWnd::sci_ToggleCaretSticky()
{
	return execute(SCI_TOGGLECARETSTICKY);
}

//////////////////////////////////////////////////////////////////////////
/*----------Margins------------*/
inline SciDll_void CSciWnd::sci_SetMarginTypeN(int margin, int iType)
{
	return execute(SCI_SETMARGINTYPEN,margin,iType);
}

inline int CSciWnd::sci_GetMarginTypeN(int margin)
{
	return execute(SCI_GETMARGINTYPEN, margin, 0);
}


inline SciDll_void CSciWnd::sci_SetMarginWidthN(int margin, int PixelWidth)
{
	return execute(SCI_SETMARGINWIDTHN,margin,PixelWidth);	
}

inline int CSciWnd::sci_GetMarginWidthN(int margin)
{
	return execute(SCI_GETMARGINWIDTHN,margin,0);
}

inline SciDll_void CSciWnd::sci_SetMarginMaskN(int margin, int mask)
{
	return execute(SCI_SETMARGINMASKN,margin,mask);
}

inline int CSciWnd::sci_GetMarginMaskN(int margin)
{
	return execute(SCI_GETMARGINMASKN,margin,0);
}

inline SciDll_void CSciWnd::sci_SetMarginSensitiveN(int margin, BOOL sensitive)
{
	return execute(SCI_SETMARGINSENSITIVEN,margin,sensitive);
}

inline BOOL CSciWnd::sci_GetMarginSensitiveN(int margin)
{
	return execute(SCI_GETMARGINSENSITIVEN, margin, 0);
}

inline SciDll_void CSciWnd::sci_SetMarginCursorN(int margin, int cursor)
{
	return execute(SCI_SETMARGINCURSORN,margin,cursor);
}

inline int CSciWnd::sci_GetMarginCursorN(int margin)
{
	return execute(SCI_GETMARGINCURSORN,margin,0);
}

inline SciDll_void CSciWnd::sci_SetMarginLeft(int pixels)
{
	return execute(SCI_SETMARGINLEFT,pixels,0);
}

inline int CSciWnd::sci_GetMarginLeft()
{
	return execute(SCI_GETMARGINLEFT);
}

inline SciDll_void CSciWnd::sci_SetMarginRight(int pixels)
{
	return execute(SCI_SETMARGINRIGHT,pixels,0);
}

inline int CSciWnd::sci_GetMarginRight()
{
	return execute(SCI_GETMARGINRIGHT);
}


inline SciDll_void CSciWnd::sci_SetFoldMarginColour(BOOL useSetting, COLORREF colour)
{
	return execute(SCI_SETFOLDMARGINCOLOUR,useSetting,colour);
}

inline SciDll_void CSciWnd::sci_SetFoldMarginHiColour(BOOL useSetting, COLORREF colour)
{
	return execute(SCI_SETFOLDMARGINHICOLOUR,useSetting,colour);
}

inline SciDll_void CSciWnd::sci_MarginSetText(int line, LPTSTR pText)
{
	return execute(SCI_MARGINSETTEXT,line,(LPARAM)pText);
}

inline SciDll_void CSciWnd::sci_MarginGetText(int line, LPTSTR pText)
{
	return execute(SCI_MARGINGETTEXT,line,(LPARAM)pText);
}

inline SciDll_void CSciWnd::sci_MarginSetStyle(int line, int style)
{
	return execute(SCI_MARGINSETSTYLE,line,style);
}

inline int CSciWnd::sci_MarginGetStyle(int line)
{
	return execute(SCI_MARGINGETSTYLE,line,0);
}

inline SciDll_void CSciWnd::sci_MarginSetStyles(int line, LPTSTR pStyle)
{
	return execute(SCI_MARGINSETSTYLE, line, (LPARAM)pStyle);
}

inline SciDll_void CSciWnd::sci_MarginGetStyles(int line, LPTSTR pStyle)
{
	return execute(SCI_MARGINGETSTYLES,line,(LPARAM)pStyle);
}

inline SciDll_void CSciWnd::sci_MarginTextClearAll()
{
	return execute(SCI_MARGINTEXTCLEARALL);
}

inline SciDll_void CSciWnd::sci_MarginSetStyleOffset(int style)
{
	return execute(SCI_MARGINSETSTYLEOFFSET,style,0);
}

inline int CSciWnd::sci_MarginGetStyleOffset()
{
	return execute(SCI_MARGINGETSTYLEOFFSET);
}

inline SciDll_void CSciWnd::sci_SetMarginOptions(int marginOptions)
{
	return execute(SCI_SETMARGINOPTIONS,marginOptions,0);
}

inline int CSciWnd::sci_GetMarginOptions()
{
	return execute(SCI_GETMARGINOPTIONS);
}


/*----------Annotations 注释--------*/
inline SciDll_void CSciWnd::sci_AnnotationSetText(int line, LPTSTR pText)
{
	return execute(SCI_ANNOTATIONSETTEXT,line,(LPARAM)pText);
}

inline SciDll_void CSciWnd::sci_AnnotationGetText(int line , LPTSTR pText)
{
	return execute(SCI_ANNOTATIONGETTEXT,line,(LPARAM)pText);
}

inline SciDll_void CSciWnd::sci_AnnotationSetStyle(int line, LPTSTR pStyle)
{
	return execute(SCI_ANNOTATIONSETSTYLE, line,(LPARAM)pStyle);
}

inline SciDll_void CSciWnd::sci_AnnotationGetStyle(int line, LPTSTR pStyle)
{
	return execute(SCI_ANNOTATIONGETSTYLE,line,(LPARAM)pStyle);
}

inline BOOL CSciWnd::sci_AnnotationGetLines(int line)
{
	return execute(SCI_ANNOTATIONGETLINES,line,0);
}

inline SciDll_void CSciWnd::sci_AnnotationClearAll()
{
	return execute(SCI_ANNOTATIONCLEARALL);
}

inline SciDll_void CSciWnd::sci_AnnotationSetVisible(int visible)
{
	return execute(SCI_ANNOTATIONSETVISIBLE,visible,0);
}

inline int CSciWnd::sci_AnnotationGetVisible()
{
	return execute(SCI_ANNOTATIONGETVISIBLE);
}

inline SciDll_void CSciWnd::sci_AnnotationSetStyleOffset(int style)
{
	return execute(SCI_ANNOTATIONSETSTYLEOFFSET,style,0);
}

inline int CSciWnd::sci_AnnotationGetStyleOffset()
{
	return execute(SCI_ANNOTATIONGETSTYLEOFFSET);
}


//////////////////////////////////////////////////////////////////////////
/*----------Other settings-----*/
inline SciDll_void CSciWnd::sci_SetUsePalette(BOOL allowPaletteUse)
{
	return -1;
	//return SendEditor(SCI_SETUSEPALETTE, allowPaletteUse, 0);
}

inline BOOL CSciWnd::sci_GetUsePalette()
{
	return -1;
	//return SendEditor(SCI_GETUSEPALETTE);
}

inline SciDll_void CSciWnd::sci_SetBufferedDraw(BOOL isBuffered)
{
	return execute(SCI_SETBUFFEREDDRAW,isBuffered,0);
}

inline BOOL CSciWnd::sci_GetBufferedDraw()
{
	return execute(SCI_GETBUFFEREDDRAW);
}

inline SciDll_void CSciWnd::sci_SetTwoPhaseDraw(BOOL twoPhase)
{
	return execute(SCI_SETTWOPHASEDRAW,twoPhase,0);
}

inline BOOL CSciWnd::sci_GetTwoPhaseDraw()
{
	return execute(SCI_GETTWOPHASEDRAW);
}

inline SciDll_void CSciWnd::sci_SetTechnology(int technology)
{
	return execute(SCI_SETTECHNOLOGY,technology,0);
}

inline BOOL CSciWnd::sci_GetTechnology()
{
	return execute(SCI_GETTECHNOLOGY);
}

inline SciDll_void CSciWnd::sci_SetFontQuality(int fontQuality)
{
	return execute(SCI_SETFONTQUALITY,fontQuality,0);
}

inline int CSciWnd::sci_GetFontQuality()
{
	return execute(SCI_GETFONTQUALITY);
}

inline SciDll_void CSciWnd::sci_SetCodePage(int codePage)
{
	return execute(SCI_SETCODEPAGE, codePage, 0);
}

inline int CSciWnd::sci_GetCodePage()
{
	return execute(SCI_GETCODEPAGE);
}

// inline SciDll_void CSciWnd::sci_SetKeysUnicode(BOOL KeysUnicode)
// {
// 	return SendEditor(SCI_SETKEYSUNICODE,KeysUnicode,0);
// }
// 
// inline BOOL CSciWnd::sci_GetKeysUnicode()
// {
// 	return SendEditor(SCI_GETKEYSUNICODE);
// }

//设置控件中允许出现的字符
inline SciDll_void CSciWnd::sci_SetWordChars(LPCSTR pText)
{
	return execute(SCI_SETWORDCHARS,0,(LPARAM)pText);
}

//设置控件不处理的字符,就是当成空白了.
inline SciDll_void CSciWnd::sci_SetWhiteSpaceChars(LPCSTR pText)
{
	return execute(SCI_SETWHITESPACECHARS,0,(LPARAM)pText);
}

//使用默认的字符和空白字符
inline SciDll_void CSciWnd::sci_SetCharsDefault()
{
	return execute(SCI_SETCHARSDEFAULT);
}

inline SciDll_void CSciWnd::sci_GrabFocus()
{
	return execute(SCI_GRABFOCUS);
}

inline SciDll_void CSciWnd::sci_SetFocus(BOOL bfocus)
{
	return execute(SCI_SETFOCUS, bfocus, 0);
}

inline BOOL CSciWnd::sci_GetFocus()
{
	return execute(SCI_GETFOCUS);
}

//////////////////////////////////////////////////////////////////////////
/*-------Brace highlighting----*/
inline SciDll_void CSciWnd::sci_BraceHighlight(int pos1, int pos2)
{
	return execute(SCI_BRACEHIGHLIGHT, pos1, pos2);
}

inline SciDll_void CSciWnd::sci_BraceBadLight(int pos1)
{
	return execute(SCI_BRACEBADLIGHT,pos1,0);
}

//高亮指示器
inline SciDll_void CSciWnd::sci_BraceHighlightIndicator(BOOL useBraceHighlightIndicator, int indicatorNumber)
{
	return execute(SCI_BRACEHIGHLIGHTINDICATOR, useBraceHighlightIndicator,indicatorNumber);
}

inline SciDll_void CSciWnd::sci_BraceBadLightIndicator(BOOL useBraceHighlightIndicator, int indicatorNumber)
{
	return execute(SCI_BRACEBADLIGHTINDICATOR,useBraceHighlightIndicator, indicatorNumber);
}

inline SciDll_void CSciWnd::sci_BraceMatch(int pos, int maxReStyle)
{
	return execute(SCI_BRACEMATCH,pos,maxReStyle);
}

//////////////////////////////////////////////////////////////////////////
/*--Tabs and Indentation Guides 自动缩排-*/
inline SciDll_void CSciWnd::sci_SetTabWidth(int widthInChars)
{
	return execute(SCI_SETTABWIDTH, widthInChars, 0);
}

inline int CSciWnd::sci_GetTabWidth()
{
	return execute(SCI_GETTABWIDTH);
}

inline SciDll_void CSciWnd::sci_SetUseTabs(BOOL useTabs)
{
	return execute(SCI_SETUSETABS,useTabs,0);
}

inline BOOL CSciWnd::sci_GetUseTabs()
{
	return execute(SCI_GETUSETABS);
}

inline SciDll_void CSciWnd::sci_SetIndent(int widthInChars)
{
	return execute(SCI_SETINDENT, widthInChars, 0);
}

inline int CSciWnd::sci_GetIndent()
{
	return execute(SCI_GETINDENT);
}

inline SciDll_void CSciWnd::sci_SetTabIndents(BOOL tabIndents)
{
	return execute(SCI_SETTABINDENTS, tabIndents, 0);
}

inline BOOL CSciWnd::sci_GetTabIndents()
{
	return execute(SCI_GETTABINDENTS);
}

inline SciDll_void CSciWnd::sci_SetBackSpaceUnIndents(BOOL bsUnIndents)
{
	return execute(SCI_SETBACKSPACEUNINDENTS, bsUnIndents, 0);
}

inline BOOL CSciWnd::sci_GetBackSpaceUnIndents()
{
	return execute(SCI_GETBACKSPACEUNINDENTS);
}

inline SciDll_void CSciWnd::sci_SetLineIndentation(int line, int indentation)
{
	return execute(SCI_GETLINEINDENTATION, line, indentation);
}

inline int CSciWnd::sci_GetLineIndentation(int line)
{
	return execute(SCI_GETLINEINDENTATION, line, 0);
}

inline int CSciWnd::sci_GetLineIndentPosition(int line)
{
	return execute(SCI_GETLINEINDENTPOSITION, line, 0);
}

inline SciDll_void CSciWnd::sci_SetIndentationGuides(int indentView)
{
	return execute(SCI_SETINDENTATIONGUIDES, indentView, 0);
}

inline int CSciWnd::sci_GetIndentationGuides()
{
	return execute(SCI_GETINDENTATIONGUIDES);
}

inline SciDll_void CSciWnd::sci_SetHighlightGuide(int column)
{
	return execute(SCI_SETHIGHLIGHTGUIDE, column, 0);
}

inline int CSciWnd::sci_GetHighlightGuide()
{
	return execute(SCI_GETHIGHLIGHTGUIDE);
}

//////////////////////////////////////////////////////////////////////////
/*----------Markers------------*/
inline SciDll_void CSciWnd::sci_MarkerDefine(int markerNumber, int markerSymbols)
{
	return execute(SCI_MARKERDEFINE, markerNumber, markerSymbols);
}

inline SciDll_void CSciWnd::sci_MarkerDefinePixmap(int markerNumber, LPCSTR pXPM)
{
	return execute(SCI_MARKERDEFINEPIXMAP, markerNumber, (LPARAM)pXPM);
}

inline SciDll_void CSciWnd::sci_RGBAImageSetWidth(int width)
{
	return execute(SCI_RGBAIMAGESETWIDTH, width, 0);
}

inline SciDll_void CSciWnd::sci_RGBAImageSetHeight(int height)
{
	return execute(SCI_RGBAIMAGESETHEIGHT, height, 0);
}

inline SciDll_void CSciWnd::sci_MarkerDefineRGBAImage(int markerNumber, LPCSTR pPixels)
{
	return execute(SCI_MARKERDEFINERGBAIMAGE, markerNumber, (LPARAM)pPixels);
}

inline SciDll_void CSciWnd::sci_MarkerSymbolDefined(int markerNumber)
{
	return execute(SCI_MARKERSYMBOLDEFINED, markerNumber, 0);
}

inline SciDll_void CSciWnd::sci_MarkerSetFore(int markerNumber, COLORREF colour)
{
	return execute(SCI_MARKERSETFORE, markerNumber, colour);
}

inline SciDll_void CSciWnd::sci_MarkerSetBack(int markerNumber, COLORREF colour)
{
	return execute(SCI_MARKERSETBACK, markerNumber, colour);
}

inline SciDll_void CSciWnd::sci_MarkerSetBackSelected(int markerNumber, COLORREF colour)
{
	return execute(SCI_MARKERSETBACKSELECTED, markerNumber, colour);
}

inline SciDll_void CSciWnd::sci_MarkerEnableHighlight(BOOL enable)
{
	return execute(SCI_MARKERENABLEHIGHLIGHT, enable, 0);
}

inline SciDll_void CSciWnd::sci_MarkerSetAlpha(int markerNumber, int alpha)
{
	return execute(SCI_MARKERSETALPHA, markerNumber, alpha);
}

inline SciDll_void CSciWnd::sci_MarkerAdd(int line, int markerMask)
{
	return execute(SCI_MARKERADD, line, markerMask);
}

inline SciDll_void CSciWnd::sci_MarkerAddSet(int line, int markerMask)
{
	return execute(SCI_MARKERADDSET, line, markerMask);
}

inline SciDll_void CSciWnd::sci_MarkerDelete(int line, int markerNumber)
{
	return execute(SCI_MARKERDELETE, line, markerNumber);
}

inline SciDll_void CSciWnd::sci_MarkerDeleteAll(int markerNumber)
{
	return execute(SCI_MARKERDELETEALL, markerNumber, 0);
}

inline int CSciWnd::sci_MarkerGet(int line)
{
	return execute(SCI_MARKERGET, line, 0);
}

inline SciDll_void CSciWnd::sci_MarkerNext(int lineStart, int markerMask)
{
	return execute(SCI_MARKERNEXT,lineStart,markerMask);
}

inline SciDll_void CSciWnd::sci_MarkerPrevious(int lineStart, int markerMask)
{
	return execute(SCI_MARKERPREVIOUS,lineStart,markerMask);
}

inline SciDll_void CSciWnd::sci_MarkerLineFromHandle(int markerHandle)
{
	return execute(SCI_MARKERLINEFROMHANDLE, markerHandle, 0);
}

inline SciDll_void CSciWnd::sci_MarkerDeleteHandle(int markerHandle)
{
	return execute(SCI_MARKERDELETEHANDLE, markerHandle, 0);
}

//////////////////////////////////////////////////////////////////////////
/*----------Indicators---------*/
inline SciDll_void CSciWnd::sci_IndicSetStyle(int indicatorNumber, int indicatorStyle)
{
	return execute(SCI_INDICSETSTYLE, indicatorNumber, indicatorStyle);
}

inline int CSciWnd::sci_IndicGetStyle(int indicatorNumber)
{
	return execute(SCI_INDICGETSTYLE, indicatorNumber, 0);
}

inline SciDll_void CSciWnd::sci_IndicSetFore(int indicatorNumber, COLORREF colour)
{
	return execute(SCI_INDICSETFORE, indicatorNumber, colour);
}

inline COLORREF CSciWnd::sci_IndicGetFore(int indicatorNumber)
{
	return execute(SCI_INDICGETFORE, indicatorNumber, 0);
}

inline SciDll_void CSciWnd::sci_IndicSetAlpha(int indicatorNumber, int alpha)
{
	return execute(SCI_INDICSETALPHA, indicatorNumber, alpha);	
}

inline int CSciWnd::sci_IndicGetAlpha(int indicatorNumber)
{
	return execute(SCI_INDICGETALPHA, indicatorNumber, 0);
}

inline SciDll_void CSciWnd::sci_IndicSetUnder(int indicatorNumber, BOOL under)
{
	return execute(SCI_INDICSETUNDER, indicatorNumber, under);
}

inline BOOL CSciWnd::sci_IndicGetUnder(int indicatorNumber)
{
	return execute(SCI_INDICGETUNDER, indicatorNumber, 0);
}


/*----------Modern Indicators--------*/
inline SciDll_void CSciWnd::sci_SetIndicatorCurrent(int indicator)
{
	return execute(SCI_SETINDICATORCURRENT, indicator, 0);
}

inline int CSciWnd::sci_GetIndicatorCurrent()
{
	return execute(SCI_GETINDICATORCURRENT);
}

inline SciDll_void CSciWnd::sci_SetIndicatorValue(int value)
{
	return execute(SCI_SETINDICATORVALUE, value, 0);
}

inline int CSciWnd::sci_GetIndicatorValue()
{
	return execute(SCI_GETINDICATORVALUE);
}

inline SciDll_void CSciWnd::sci_IndicatorFillRange(int position, int fillLength)
{
	return execute(SCI_INDICATORFILLRANGE, position, fillLength);
}

inline SciDll_void CSciWnd::sci_IndicatorClearRange(int position, int clearLength)
{
	return execute(SCI_INDICATORCLEARRANGE, position, clearLength);
}

inline SciDll_void CSciWnd::sci_IndicatorAllOnFor(int position)
{
	return execute(SCI_INDICATORALLONFOR, position, 0);
}

inline SciDll_void CSciWnd::sci_IndicatorValueAt(int indicator, int postion)
{
	return execute(SCI_INDICATORVALUEAT, indicator, postion);
}

inline SciDll_void CSciWnd::sci_IndicatorStart(int indicator, int postion)
{
	return execute(SCI_INDICATORSTART, indicator, postion);
}

inline SciDll_void CSciWnd::sci_IndicatorEnd(int indicator, int postion)
{
	return execute(SCI_INDICATOREND, indicator, postion);	
}


/*----------Autocompletion-----*/
inline SciDll_void CSciWnd::sci_AutocShow(int lenEntered, const char *pList)
{
	return execute(SCI_AUTOCSHOW, lenEntered, (LPARAM)pList);
}

inline SciDll_void CSciWnd::sci_AutocCancel()
{
	return execute(SCI_AUTOCCANCEL);
}

inline SciDll_void CSciWnd::sci_AutocActive()
{
	return execute(SCI_AUTOCACTIVE);
}

inline SciDll_void CSciWnd::sci_AutocPosStart()
{
	return execute(SCI_AUTOCPOSSTART);
}

inline SciDll_void CSciWnd::sci_AutocComplete()
{
	return execute(SCI_AUTOCCOMPLETE);
}

inline SciDll_void CSciWnd::sci_AutocStops(LPCSTR pText)
{
	return execute(SCI_AUTOCSTOPS, 0, (LPARAM)pText);
}

inline SciDll_void CSciWnd::sci_AutocSetSeparator(char separator)
{
	return execute(SCI_AUTOCSETSEPARATOR, separator, 0);	
}

inline char CSciWnd::sci_AutocGetSeparator()
{
	return (char)execute(SCI_AUTOCGETSEPARATOR);
}

inline SciDll_void CSciWnd::sci_AutocSelect(LPCSTR pSelect)
{
	return execute(SCI_AUTOCSELECT, 0, (LPARAM)pSelect);
}

inline int CSciWnd::sci_AutocGetCurrent()
{
	return execute(SCI_AUTOCGETCURRENT);
}

inline SciDll_void CSciWnd::sci_AutocGetCurrentText(CStringA &Text)
{
	char *pText = new char[256];
	int nRet = execute(SCI_AUTOCGETCURRENTTEXT, 0, (LPARAM)pText);
	pText[nRet] = '\0';
	Text = pText;
	return nRet;
}

inline SciDll_void CSciWnd::sci_AutocSetCancelAtStart(BOOL cancel)
{
	return execute(SCI_AUTOCSETCANCELATSTART, cancel, 0);
}

inline BOOL CSciWnd::sci_AutocGetCancelAtStart()
{
	return execute(SCI_AUTOCGETCANCELATSTART);
}

inline SciDll_void CSciWnd::sci_AutocSetFillups(LPCSTR pText)
{
	return execute(SCI_AUTOCSETFILLUPS, 0, (LPARAM)pText);
}

inline SciDll_void CSciWnd::sci_AutocSetChooseSingle(BOOL chooseSingle)
{
	return execute(SCI_AUTOCSETCHOOSESINGLE, chooseSingle, 0);
}

inline BOOL CSciWnd::sci_AutocGetChooseSingle()
{
	return execute(SCI_AUTOCGETCHOOSESINGLE);
}

//是否忽略大小写
inline SciDll_void CSciWnd::sci_AutocSetIgnoreCase(BOOL ignoreCase)
{
	return execute(SCI_AUTOCSETIGNORECASE, ignoreCase, 0);	
}

inline BOOL CSciWnd::sci_AutocGetIgnoreCase()
{
	return execute(SCI_AUTOCGETIGNORECASE);
}

inline SciDll_void CSciWnd::sci_AutocSetAutoHide(BOOL autoHide)
{
	return execute(SCI_AUTOCSETAUTOHIDE, autoHide, 0);
}

inline BOOL CSciWnd::sci_AutocGetAutoHide()
{
	return execute(SCI_AUTOCGETAUTOHIDE);
}

inline SciDll_void CSciWnd::sci_AutocSetDropRestOfWord(BOOL dropRestOfWord)
{
	return execute(SCI_AUTOCSETDROPRESTOFWORD, dropRestOfWord, 0);
}

inline BOOL CSciWnd::sci_AutocGetDropRestOfWord()
{
	return execute(SCI_AUTOCGETDROPRESTOFWORD);
}

inline SciDll_void CSciWnd::sci_RegisterImage(int type, LPCSTR pXmpData)
{
	return execute(SCI_REGISTERIMAGE, type, (LPARAM)pXmpData);
}

inline SciDll_void CSciWnd::sci_RegisterRGBAImage(int type, LPCSTR pPixels)
{
	return execute(SCI_REGISTERRGBAIMAGE, type, (LPARAM)pPixels);
}

inline SciDll_void CSciWnd::sci_ClearRegisteredImages()
{
	return execute(SCI_CLEARREGISTEREDIMAGES);
}

inline SciDll_void CSciWnd::sci_AutocSetTypeSeparator(char separatorCharacter)
{
	return execute(SCI_AUTOCSETTYPESEPARATOR, separatorCharacter, 0);
}

inline char CSciWnd::sci_AutocGetTypeSeparator()
{
	return (char)execute(SCI_AUTOCGETTYPESEPARATOR);
}

inline SciDll_void CSciWnd::sci_AutocSetMaxHeight(int rowCount)
{
	return execute(SCI_AUTOCSETMAXHEIGHT, rowCount, 0);
}

inline int CSciWnd::sci_AutocGetMaxHeight()
{
	return execute(SCI_AUTOCGETMAXHEIGHT);
}

inline SciDll_void CSciWnd::sci_AutocSetMaxWidth(int characterCount)
{
	return execute(SCI_AUTOCSETMAXWIDTH, characterCount);
}

inline int CSciWnd::sci_AutocGetMaxWidth()
{
	return execute(SCI_AUTOCGETMAXWIDTH);
}

//////////////////////////////////////////////////////////////////////////
/*----------User lists------------*/
inline SciDll_void CSciWnd::sci_UserListShow(int listType, LPCSTR pList)
{
	return execute(SCI_USERLISTSHOW, listType, (LPARAM)pList);
}

/*----------Call tips------------*/
inline SciDll_void CSciWnd::sci_CallTipShow(int posStart, LPCSTR pDefinition)
{
	return execute(SCI_CALLTIPSHOW, posStart, (LPARAM)pDefinition);
}

inline SciDll_void CSciWnd::sci_CallTipCancel()
{
	return execute(SCI_CALLTIPCANCEL);
}

inline SciDll_void CSciWnd::sci_CallTipActive()
{
	return execute(SCI_CALLTIPACTIVE);
}

inline SciDll_void CSciWnd::sci_CallTipPosStart()
{
	return execute(SCI_CALLTIPPOSSTART);
}

inline SciDll_void CSciWnd::sci_CallTipSetHlt(int hlStart, int hlEnd)
{
	return execute(SCI_CALLTIPSETHLT, hlStart, hlEnd);
}

inline SciDll_void CSciWnd::sci_CallTipSetBack(COLORREF colour)
{
	return execute(SCI_CALLTIPSETBACK, colour, 0);
}

inline SciDll_void CSciWnd::sci_CallTipSetFore(COLORREF colour)
{
	return execute(SCI_CALLTIPSETFORE, colour, 0);
}

inline SciDll_void CSciWnd::sci_CallTipSetForeHlt(COLORREF colour)
{
	return execute(SCI_CALLTIPSETFOREHLT, colour, 0);
}

inline SciDll_void CSciWnd::sci_CallTipUseStyle(int tabsize)
{
	return execute(SCI_CALLTIPUSESTYLE, tabsize, 0);
}

inline SciDll_void CSciWnd::sci_CallTipSetPosition(BOOL above)
{
	return execute(SCI_CALLTIPSETPOSITION, above, 0);
}

/*----------Keyboard commands------------*/
/*----------Key bindings------------*/
inline SciDll_void CSciWnd::sci_AssignCmdKey(int keyDefinition, int sciCommand)
{
	return execute(SCI_ASSIGNCMDKEY, keyDefinition, sciCommand);
}

inline SciDll_void CSciWnd::sci_ClearCmdKey(int keyDefinition)
{
	return execute(SCI_CLEARCMDKEY, keyDefinition, 0);
}

inline SciDll_void CSciWnd::sci_ClearAllCmdKeys()
{
	return execute(SCI_CLEARALLCMDKEYS);
}

inline SciDll_void CSciWnd::sci_NullCmdKey()
{
	return execute(SCI_NULL);
}

/*----------Popup edit menu------------*/
inline SciDll_void CSciWnd::sci_UsePopup(BOOL bEnablePopup)
{
	return execute(SCI_USEPOPUP, bEnablePopup, 0);
}

/*----------Macro recording------------*/
inline SciDll_void CSciWnd::sci_StartRecord()
{
	return execute(SCI_STARTRECORD);
}

inline SciDll_void CSciWnd::sci_StopRecord()
{
	return execute(SCI_STOPRECORD);
}

//////////////////////////////////////////////////////////////////////////
/*----------Printing------------*/
inline SciDll_void CSciWnd::sci_FormatRange(BOOL bDraw, Sci_RangeToFormat *pfr)
{
	return execute(SCI_FORMATRANGE, bDraw, (LPARAM)pfr);
}

inline SciDll_void CSciWnd::sci_SetPrintMagnification(BOOL magnification)
{
	return execute(SCI_SETPRINTMAGNIFICATION, magnification, 0);
}

inline BOOL CSciWnd::sci_GetPrintMagnification()
{
	return execute(SCI_GETPRINTMAGNIFICATION);
}

inline SciDll_void CSciWnd::sci_SetPrintColourMode(int mode)
{
	return execute(SCI_SETPRINTCOLOURMODE, mode, 0);
}

inline int CSciWnd::sci_GetPrintColourMode()
{
	return execute(SCI_GETPRINTCOLOURMODE);
}

inline SciDll_void CSciWnd::sci_SetPrintWrapMode(int wrapMode)
{
	return execute(SCI_SETPRINTWRAPMODE, wrapMode, 0);
}

inline BOOL CSciWnd::sci_GetPrintWrapMode()
{
	return execute(SCI_GETPRINTWRAPMODE);
}


//////////////////////////////////////////////////////////////////////////
/*----------Direct access------------*/
//SciDll_void GetDirectFuntion()
//void *GetDirectPointer()
//void *GetCharacterPointer()
//////////////////////////////////////////////////////////////////////////
/*----------Multiple views------------*/
inline void *CSciWnd::sci_GetDocPointer()
{
	return (void *)execute(SCI_GETDOCPOINTER,0,0);
}

inline SciDll_void CSciWnd::sci_SetDocPointer(void *pDoc)
{
	execute(SCI_SETDOCPOINTER,0,(LPARAM)pDoc);
}

inline void *CSciWnd::sci_CreateDocument()
{
	return (void *)execute(SCI_CREATEDOCUMENT,0,0);
}

inline SciDll_void CSciWnd::sci_AddRefDocument(void *pDoc)
{
	execute(SCI_ADDREFDOCUMENT,0,(LPARAM)pDoc);
}

inline SciDll_void CSciWnd::sci_ReleaseDocument(void *pDoc)
{
	execute(SCI_RELEASEDOCUMENT,0,(LPARAM)pDoc);
}

//////////////////////////////////////////////////////////////////////////
/*----------Background loading and saving------------*/
//////////////////////////////////////////////////////////////////////////
/*----------Folding-----------*/	
inline SciDll_void CSciWnd::sci_VisibleFromDocLine(int docLine)
{
	return execute(SCI_VISIBLEFROMDOCLINE, docLine, 0);
}

inline SciDll_void CSciWnd::sci_DocLineFromVisible(int displayLine)
{
	return execute(SCI_DOCLINEFROMVISIBLE, displayLine, 0);
}

inline SciDll_void CSciWnd::sci_ShowLines(int lineStart, int lineEnd)
{
	return execute(SCI_SHOWLINES, lineStart, lineEnd);
}

inline SciDll_void CSciWnd::sci_HideLines(int lineStart, int lineEnd)
{
	return execute(SCI_HIDELINES, lineStart, lineEnd);
}

inline BOOL CSciWnd::sci_GetLineVisible(int line)
{
	return execute(SCI_GETLINEVISIBLE, line, 0);
}

inline BOOL CSciWnd::sci_GetAllLinesVisible()
{
	return execute(SCI_GETALLLINESVISIBLE);
}

inline SciDll_void CSciWnd::sci_SetFoldLevel(int line, int level)
{
	return execute(SCI_SETFOLDLEVEL, line, level);
}

inline int  CSciWnd::sci_GetFoldLevel(int line)
{
	return execute(SCI_GETFOLDLEVEL, line, 0);
}

inline SciDll_void CSciWnd::sci_SetFoldFlags(int flags)
{
	return execute(SCI_SETFOLDFLAGS, flags, 0);
}

inline int  CSciWnd::sci_GetLastChild(int line, int level)
{
	return execute(SCI_GETLASTCHILD, line, level);
}

inline int  CSciWnd::sci_GetFoldParent(int line)
{
	return execute(SCI_GETFOLDPARENT, line, 0);
}

inline SciDll_void CSciWnd::sci_SetFoldExpanded(int line, BOOL expanded)
{
	return execute(SCI_SETFOLDEXPANDED, line, expanded);
}

inline BOOL CSciWnd::sci_GetFoldExpanded(int line)
{
	return execute(SCI_GETFOLDEXPANDED, line, 0);
}

inline int CSciWnd::sci_ContractedFoldNext(int lineStart)
{
	return execute(SCI_CONTRACTEDFOLDNEXT, lineStart, 0);
}

inline SciDll_void CSciWnd::sci_ToggleFold(int line)
{
	return execute(SCI_TOGGLEFOLD, line, 0);
}

inline SciDll_void CSciWnd::sci_EnsureVisible(int line)
{
	return execute(SCI_ENSUREVISIBLE, line, 0);
}

inline SciDll_void CSciWnd::sci_EnsureVisibleEnforcePolicy(int line)
{
	return execute(SCI_ENSUREVISIBLEENFORCEPOLICY, line, 0);
}


//////////////////////////////////////////////////////////////////////////
/*----------Line wrapping------------*/	
inline SciDll_void CSciWnd::sci_SetWrapMode(int wrapMode)
{
	return execute(SCI_SETWRAPMODE, wrapMode, 0);
}

inline int  CSciWnd::sci_GetWrapMode()
{
	return execute(SCI_GETWRAPMODE);
}

inline SciDll_void CSciWnd::sci_SetWrapVisualFlags(int wrapVisualFlags)
{
	return execute(SCI_SETWRAPVISUALFLAGS, wrapVisualFlags, 0);
}

inline int  CSciWnd::sci_GetWrapVisualFlags()
{
	return execute(SCI_GETWRAPVISUALFLAGS);
}

inline SciDll_void CSciWnd::sci_SetWrapVisualFlagsLocation(int wrapVisualFlagsLocation)
{
	return execute(SCI_SETWRAPVISUALFLAGSLOCATION, wrapVisualFlagsLocation, 0);
}

inline int  CSciWnd::sci_GetWrapVisualFlagsLocation()
{
	return execute(SCI_GETWRAPVISUALFLAGSLOCATION);
}

inline SciDll_void CSciWnd::sci_SetWrapIndentMode(int indentMode)
{
	return execute(SCI_SETWRAPINDENTMODE, indentMode, 0);
}

inline int  CSciWnd::sci_GetWrapIndentMode()
{
	return execute(SCI_GETWRAPINDENTMODE);
}

inline SciDll_void CSciWnd::sci_SetWrapStartIndent(int indent)
{
	return execute(SCI_SETWRAPSTARTINDENT, indent, 0);
}

inline int  CSciWnd::sci_GetWrapStartIndent()
{
	return execute(SCI_GETWRAPSTARTINDENT);
}

inline SciDll_void CSciWnd::sci_SetLayoutCache(int cacheMode)
{
	return execute(SCI_SETLAYOUTCACHE, cacheMode, 0);
}

inline int  CSciWnd::sci_GetLayoutCache()
{
	return execute(SCI_GETLAYOUTCACHE);
}

inline SciDll_void CSciWnd::sci_SetPositionCache(int size)
{
	return execute(SCI_SETPOSITIONCACHE, size, 0);
}

inline int  CSciWnd::sci_GetPositionCache()
{
	return execute(SCI_GETPOSITIONCACHE);
}

inline SciDll_void CSciWnd::sci_LinesSplit(int pixelWidth)
{
	return execute(SCI_LINESSPLIT, pixelWidth, 0);
}

inline SciDll_void CSciWnd::sci_LinesJoin()
{
	return execute(SCI_LINESJOIN);
}

inline SciDll_void CSciWnd::sci_WrapCount(int docLine)
{
	return execute(SCI_WRAPCOUNT, docLine, 0);
}

//////////////////////////////////////////////////////////////////////////
/*----------Zooming------------*/
inline SciDll_void CSciWnd::sci_ZoomIn()
{
	return execute(SCI_ZOOMIN);
}

inline SciDll_void CSciWnd::sci_ZoomOut()
{
	return execute(SCI_ZOOMOUT);
}

inline SciDll_void CSciWnd::sci_SetZoom(int zoomInPoints)
{
	return execute(SCI_SETZOOM, zoomInPoints, 0);
}

inline int  CSciWnd::sci_GetZoom()
{
	return execute(SCI_GETZOOM);
}


//////////////////////////////////////////////////////////////////////////
/*----------Long lines------------*/
inline SciDll_void CSciWnd::sci_SetEdgeMode(int mode)
{
	return execute(SCI_SETEDGEMODE, mode, 0);
}

inline int  CSciWnd::sci_GetEdgeMode()
{
	return execute(SCI_GETEDGEMODE);
}

inline SciDll_void CSciWnd::sci_SetEdgeColumn(int column)
{
	return execute(SCI_SETEDGECOLUMN, column, 0);
}

inline int  CSciWnd::sci_GetEdgeColumn()
{
	return execute(SCI_GETEDGECOLUMN);
}

inline SciDll_void CSciWnd::sci_SetEdgeColour(int colour)
{
	return execute(SCI_SETEDGECOLOUR, colour, 0);
}

inline int  CSciWnd::sci_GetEdgeColour()
{
	return execute(SCI_GETEDGECOLOUR);
}

//////////////////////////////////////////////////////////////////////////
/*----------Lexer------------*/
inline SciDll_void CSciWnd::sci_SetLexer(int lexer)
{
	return execute(SCI_SETLEXER, lexer, 0);
}

inline int  CSciWnd::sci_GetLexer()
{
	return execute(SCI_GETLEXER);
}

inline SciDll_void CSciWnd::sci_SetLexerLanguage(LPCSTR name)
{
	return execute(SCI_SETLEXERLANGUAGE, 0, (LPARAM)name);
}

inline SciDll_void CSciWnd::sci_GetLexerLanguage(CString &Text)
{
	int nRet = -1;
	char *p = new char[256];
	nRet = execute(SCI_GETLEXERLANGUAGE, 0, (LPARAM)p);
	Text = p;
	delete []p;
	return nRet;
}

inline SciDll_void CSciWnd::sci_LoadLexerLibrary(LPCSTR path)
{
	return execute(SCI_LOADLEXERLIBRARY, 0, (LPARAM)path);
}

inline SciDll_void CSciWnd::sci_ColourIse(int start, int end)
{
	return execute(SCI_COLOURISE, start, end);
}

inline SciDll_void CSciWnd::sci_ChangeLexerState(int start, int end)
{
	return execute(SCI_CHANGELEXERSTATE, start, end);
}

inline SciDll_void CSciWnd::sci_PropertyNames(LPTSTR names)
{
	return execute(SCI_PROPERTYNAMES, 0, (LPARAM)names);
}

inline SciDll_void CSciWnd::sci_PropertyType(LPTSTR name)
{
	return execute(SCI_PROPERTYTYPE, 0, (LPARAM)name);
}

inline SciDll_void CSciWnd::sci_DescribeProperty(LPCSTR name, LPTSTR description)
{
	return execute(SCI_DESCRIBEPROPERTY, (LPARAM)name, (LPARAM)description);
}

inline SciDll_void CSciWnd::sci_SetProperty(LPCSTR key, LPCSTR value)
{
	return execute(SCI_SETPROPERTY, (LPARAM)key, (LPARAM)value);
}

inline SciDll_void CSciWnd::sci_GetProperty(LPCSTR key, CString &Value)
{
	int nRet = -1;
	char *p = new char[256];
	nRet = execute(SCI_GETPROPERTY, (LPARAM)key, (LPARAM)p);
	if(nRet>0)
	{
		p[nRet-1] = 0;
		Value = p;
	}
	delete []p;
	return nRet;
}

inline SciDll_void CSciWnd::sci_GetPropertyExpanded(LPCSTR key, CString &Value)
{
	int nRet = -1;
	char *p = new char[256];
	nRet = execute(SCI_GETPROPERTYEXPANDED, (LPARAM)key, (LPARAM)p);
	if(nRet>0)
	{
		p[nRet-1] = 0;
		Value = p;
	}
	delete []p;
	return nRet;
}

inline int  CSciWnd::sci_GetPropertyInt(LPCSTR key, int ndefault)
{
	return execute(SCI_GETPROPERTYINT, (LPARAM)key, ndefault);
}

inline SciDll_void CSciWnd::sci_DescribeKeywordSets(CString &descriptions)
{
	int nRet = -1;
	char *p = new char[1024];
	nRet = execute(SCI_DESCRIBEKEYWORDSETS, 0, (LPARAM)p);
	if(nRet>0)
	{
		p[nRet-1] = 0;
		descriptions = p;
	}
	delete []p;
	return nRet;
}

inline SciDll_void CSciWnd::sci_SetKeyWords(int keyWordSet, LPCSTR keyWordList)
{
	return execute(SCI_SETKEYWORDS, keyWordSet, (LPARAM)keyWordList);
}

inline int CSciWnd::sci_GetStyleBitsNeeded()
{
	return execute(SCI_GETSTYLEBITSNEEDED);
}
