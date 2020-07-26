#pragma once
#include "SciWnd.h"

class CSciXmlWriter
{
public:
	CSciXmlWriter(CSciWnd *pSciWnd);
	~CSciXmlWriter(void);

	void print(xml_node node);

protected:
	void write(LPCTSTR text);
	void write(TCHAR text);
	void flush();
	void node_output(xml_node root, int &row, int depth);
	void node_output_attribute(xml_node node);
	void node_output_comment(xml_node node, int &row, int depth);
	void text_output(LPCTSTR text);

private:
	CSciWnd *m_pSciWnd;
	CString m_strTempBuffer;
};

