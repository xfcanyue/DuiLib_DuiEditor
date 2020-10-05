#pragma once

class CUIManager;
class CSciXmlParse
{
public:
	CSciXmlParse(void);
	~CSciXmlParse(void);

	void Parse(xml_node doc, xml_node xml);
	
protected:
	void ParseNode(xml_node nodeParent1, xml_node nodeParent2, xml_node node1, xml_node node2);
	void ParseAttributes(xml_node node1, xml_node node2, xml_attribute attr1, xml_attribute attr2);
	BOOL CompareNodeName(xml_node node1, xml_node node2);
	BOOL CompareAttrName(xml_attribute attr1, xml_attribute attr2);
	BOOL CompareAttrValue(xml_attribute attr1, xml_attribute attr2);
	BOOL CompareNodeAttributes(xml_node node1, xml_node node2);

	void AddControl(xml_node node, xml_node nodeSample);
	void DeleteControl(xml_node node);
	void AddControlAttribute(xml_node node, xml_attribute attr);
	void DeleteControlAttribute(xml_node node, xml_attribute attr);

	void AddUIFont(xml_node node, xml_node nodeSample);
	void DeleteUIFont(xml_node node);
	void ParseUIFontAttribute(xml_node node, xml_node nodeSample);
	
	void AddUIDefault(xml_node node, xml_node nodeSample);
	void DeleteUIDefault(xml_node node);
	void ParseUIDefaultAttribute(xml_node node, xml_node nodeSample);

	void AddUIStyle(xml_node node, xml_node nodeSample);
	void DeleteUIStyle(xml_node node);
	void ParseUIStyleAttribute(xml_node node, xml_node nodeSample);
private:
	xml_node src;
	xml_node dest;

public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
private:
	CUIManager *m_pUIManager;
};

