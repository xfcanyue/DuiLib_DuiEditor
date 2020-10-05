#pragma once
#include "SciWnd.h"


class CSciUndoBlock //允许多个操作合并为一个undo动作
{
public:
	CSciUndoBlock(CSciWnd *pSciWnd) : m_pSciWnd(pSciWnd)
	{
		m_pSciWnd->sci_BeginUndoAction();
	}
	~CSciUndoBlock()
	{
		m_pSciWnd->sci_EndUndoAction();
	}
	CSciWnd *m_pSciWnd;
};

class CSciXmlWriter
{
public:
	CSciXmlWriter(CSciWnd *pSciWnd);
	~CSciXmlWriter(void);

	void InitWriter(int beginPos, int beginDepth = 0, BOOL bRootBeginNeedEndLine = FALSE, BOOL bRootEndNeedEndLine = TRUE);
	const char *GetOutputBuffer();
	int GetOutputLength();
	void print(xml_node node);
	int GetStartPos() { return m_nBeginPos; }
	int GetCurPos() { return m_nBeginPos + m_strTempBuffer.GetLength(); }
	int GetOffset() { return m_strTempBuffer.GetLength(); }

protected:
	void write(const char *text);
	void write(char text);
	void writeEndLine();
	void flush();
	void node_output(xml_node root, xml_node node, int &pos, int &depth);
	void node_output_attribute(xml_attribute attr);
	void node_output_all_attributes(xml_node node);
	void node_output_close(xml_node node);
	void text_output(const char * text);
	void text_output_indent(int depth);

private:
	CSciWnd *m_pSciWnd;
	int m_nBeginPos;
	int m_nBeginDepth;

	CStringA m_strTempBuffer;
	BOOL m_bRootBeginNeedEndLine;	//root开始是否换行
	BOOL m_bRootEndNeedEndLine;		//root结束是否换行
protected:
	int  GetPosEndLinePosition(int pos); //获取pos行尾的位置
	bool IsEndline(int pos);	//判断是否行末尾
	bool IsWhiteSpace(char ch);
	bool IsEndDoc(char ch);
public:
	//设计界面中增加属性，即时更新XML
	void AddAttribute(xml_node node, xml_attribute attr);
	//设计界面中修改属性，即时更新XML
	void ModifyAttribute(xml_node node, xml_attribute attr);
	//设计界面中删除属性，即时更新XML
	void DeleteAttribute(xml_node node, xml_attribute attr);

	//设计界面中增加控件，即时更新XML
	void AddNode(xml_node node);
	//设计界面中删除控件，即时更新XML
	void DeleteNode(xml_node node);

	//使node以下的pos递增或递减
	void AdjustElseNodePos(xml_node node, int pos, int offset);
	//使node的attr以下的pos递增或递减
	void AdjustElseAttributesPos(xml_node node, xml_attribute attr, int pos, int offset);
};

