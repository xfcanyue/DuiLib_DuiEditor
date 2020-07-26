#include "StdAfx.h"
#include "SciXmlWriter.h"


CSciXmlWriter::CSciXmlWriter(CSciWnd *pSciWnd) : m_pSciWnd(pSciWnd)
{
}


CSciXmlWriter::~CSciXmlWriter(void)
{
}

void CSciXmlWriter::print(xml_node node)
{
	m_pSciWnd->sci_ClearTextAll();
	m_strTempBuffer.Empty();

	if(node)
	{
		int row = 0;
		int depth = 0;
		node_output(node, row, depth);
	}

	m_pSciWnd->sci_SetSavePoint();
	//m_pSciWnd->sci_EmptyUndoBuffer(); //不清理历史记录，xml写错时，就可以无限后退了
}

void CSciXmlWriter::write(LPCTSTR text)
{
	m_strTempBuffer += text;
}

void CSciXmlWriter::write(TCHAR text)
{
	m_strTempBuffer += text;
}

void CSciXmlWriter::flush()
{
	LSSTRING_CONVERSION;
	const char *pstring = LST2UTF8(m_strTempBuffer);
	m_pSciWnd->SendEditor(SCI_APPENDTEXT, strlen(pstring), (LPARAM)pstring);
	m_strTempBuffer.Empty();
}

void CSciXmlWriter::node_output(xml_node root, int &row, int depth)
{
	xml_node node = root;

	if(node.type() == node_comment)
	{
		node_output_comment(node, row, depth);
		flush();
		return;
	}

	//保存节点对应代码文本行
	node.set_row(row);

	for (int i=0; i<depth; i++)
		write(_T("\t"));
	write(_T("<"));
	write(node.name());
	
	//打印属性
	node_output_attribute(node);

	if(!node.first_child())
	{
		write(_T(" />\n"));
		row++;
	}
	else
	{
		if(node.first_attribute())
			write(_T(" >\n"));
		else
			write(_T(">\n"));
		row++;
	}

	//打印完一行，flush一下
	flush();

	//打印子节点
	if(node.first_child()) depth++;
	for (xml_node nodeChild=node.first_child(); nodeChild; nodeChild=nodeChild.next_sibling())
	{
		node_output(nodeChild, row, depth);
	}
	if(node.first_child()) depth--;

	//打印兄弟节点
	while (node != root)
	{
		if(node.next_sibling())
			node_output(node.next_sibling(), row, depth);
	}

	//有子节点，则打印结尾 </xxxx>
	if(node.first_child())
	{
		for (int i=0; i<depth; i++)
			write(_T("\t"));

		write(_T("</"));
		write(node.name());
		write(_T(">\n"));
		row++;
	}

	//打印完一行，flush一下
	flush();
}

void CSciXmlWriter::node_output_attribute(xml_node node)
{
	for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
	{
		write(_T(" "));
		write(attr.name());
		write(_T("=\""));
		text_output(attr.value()); 
		write(_T("\""));
	}
}

void CSciXmlWriter::node_output_comment(xml_node node, int &row, int depth)
{
	if(!node) return;

	for (int i=0; i<depth; i++)
		write(_T("\t"));
	write(_T("<!--"));

	CString strValue = node.value();
	write(strValue);

	//strValue有几行？
	int pos = 0;
	for (;;)
	{
		int posx = strValue.Find('\n', pos);
		if(posx < 0)
			break;

		row++;
		pos = posx + 1;
	}

	for (int i=0; i<depth; i++)
		write(_T("\t"));
	write(_T("-->\n"));
	row++;
}

void CSciXmlWriter::text_output(LPCTSTR text)
{
	LPCTSTR s = text;

	//检查是否有转义字符
	BOOL IsEscapeChar = FALSE;
	while (*s)
	{
		switch (*s)
		{
		case 0: break;
		case '&':
			IsEscapeChar = TRUE;
			break;
		case '<':
			IsEscapeChar = TRUE;
			break;
		case '>':
			IsEscapeChar = TRUE;
			break;
		case '"':
			IsEscapeChar = TRUE;
			break;
		default:
			break;
		}

		if(IsEscapeChar)
			break;

		++s;
	}

	if(!IsEscapeChar)
	{
		write(text);
		return;
	}

	s = text;
	while (*s)
	{
		switch (*s)
		{
		case 0: break;
		case '&':
			write(_T("&amp;"));
			++s;
			break;
		case '<':
			write(_T("&lt;"));
			++s;
			break;
		case '>':
			write(_T("&gt;"));
			++s;
			break;
		case '"':
			write(_T("&quot;"));
			++s;
			break;
		default: 
			write(*s);
			++s;
			break;
		}
	}
}