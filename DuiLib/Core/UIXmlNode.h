#ifndef __UIXMLNODE_H__
#define __UIXMLNODE_H__

#pragma once

namespace DuiLib {

	enum xml_node_type_ui
	{
		ui_node_null,			// Empty (null) node handle
		ui_node_document,		// A document tree's absolute root
		ui_node_element,		// Element tag, i.e. '<node/>'
		ui_node_pcdata,			// Plain character data, i.e. 'text'
		ui_node_cdata,			// Character data, i.e. '<![CDATA[text]]>'
		ui_node_comment,		// Comment tag, i.e. '<!-- text -->'
		ui_node_pi,				// Processing instruction, i.e. '<?name?>'
		ui_node_declaration,	// Document declaration, i.e. '<?xml version="1.0"?>'
		ui_node_doctype			// Document type declaration, i.e. '<!DOCTYPE doc>'
	};

	class UILIB_API CXmlNodeUI
	{
	public:
		CXmlNodeUI();
		explicit CXmlNodeUI(PVOID node_struct);

		typedef void (*unspecified_bool_type)(CXmlNodeUI***);
		// Safe bool conversion operator
		operator unspecified_bool_type() const;

		bool operator!() const;

		// Check if node is empty.
		bool empty() const;

		// Get node type
		xml_node_type_ui type() const;

		LPCTSTR name() const;
		LPCTSTR value() const;

		// Get attribute list
		CXmlAttributeUI first_attribute() const;
		CXmlAttributeUI last_attribute() const;

		// Get children list
		CXmlNodeUI first_child() const;
		CXmlNodeUI last_child() const;

		// Get next/previous sibling in the children list of the parent node
		CXmlNodeUI next_sibling() const;
		CXmlNodeUI previous_sibling() const;

		// Get parent node
		CXmlNodeUI parent() const;

		// Get root of DOM tree this node belongs to
		CXmlNodeUI root() const;

		// Get child, attribute or next/previous sibling with the specified name
		CXmlNodeUI child(LPCTSTR name) const;
		CXmlAttributeUI attribute(LPCTSTR name) const;
		CXmlNodeUI next_sibling(LPCTSTR name) const;
		CXmlNodeUI previous_sibling(LPCTSTR name) const;

		//child不存在时，自动插入新的并且返回
		CXmlNodeUI child_auto(LPCTSTR name);

		//attr不存在时，自动插入新的并且返回
		CXmlAttributeUI attribute_auto(LPCTSTR name);

		// Set node name/value (returns false if node is empty, there is not enough memory, or node can not have name/value)
		bool set_name(LPCTSTR rhs);
		bool set_value(LPCTSTR rhs);


		// Add attribute with specified name. Returns added attribute, or empty attribute on errors.
		CXmlAttributeUI append_attribute(LPCTSTR name);
		CXmlAttributeUI prepend_attribute(LPCTSTR name);
		CXmlAttributeUI insert_attribute_after(LPCTSTR name, const CXmlAttributeUI& attr);
		CXmlAttributeUI insert_attribute_before(LPCTSTR name, const CXmlAttributeUI& attr);

		// Add a copy of the specified attribute. Returns added attribute, or empty attribute on errors.
		CXmlAttributeUI append_copy(const CXmlAttributeUI& proto);
		CXmlAttributeUI prepend_copy(const CXmlAttributeUI& proto);
		CXmlAttributeUI insert_copy_after(const CXmlAttributeUI& proto, const CXmlAttributeUI& attr);
		CXmlAttributeUI insert_copy_before(const CXmlAttributeUI& proto, const CXmlAttributeUI& attr);

		// Add child node with specified type. Returns added node, or empty node on errors.
		CXmlNodeUI append_child(xml_node_type_ui type = ui_node_element);
		CXmlNodeUI prepend_child(xml_node_type_ui type = ui_node_element);
		CXmlNodeUI insert_child_after(xml_node_type_ui type, const CXmlNodeUI& node);
		CXmlNodeUI insert_child_before(xml_node_type_ui type, const CXmlNodeUI& node);

		// Add child element with specified name. Returns added node, or empty node on errors.
		CXmlNodeUI append_child(LPCTSTR name);
		CXmlNodeUI prepend_child(LPCTSTR name);
		CXmlNodeUI insert_child_after(LPCTSTR name, const CXmlNodeUI& node);
		CXmlNodeUI insert_child_before(LPCTSTR name, const CXmlNodeUI& node);

		// Add a copy of the specified node as a child. Returns added node, or empty node on errors.
		CXmlNodeUI append_copy(const CXmlNodeUI& proto);
		CXmlNodeUI prepend_copy(const CXmlNodeUI& proto);
		CXmlNodeUI insert_copy_after(const CXmlNodeUI& proto, const CXmlNodeUI& node);
		CXmlNodeUI insert_copy_before(const CXmlNodeUI& proto, const CXmlNodeUI& node);

		// Move the specified node to become a child of this node. Returns moved node, or empty node on errors.
		CXmlNodeUI append_move(const CXmlNodeUI& moved);
		CXmlNodeUI prepend_move(const CXmlNodeUI& moved);
		CXmlNodeUI insert_move_after(const CXmlNodeUI& moved, const CXmlNodeUI& node);
		CXmlNodeUI insert_move_before(const CXmlNodeUI& moved, const CXmlNodeUI& node);

		// Remove specified attribute
		bool remove_attribute(const CXmlAttributeUI& a);
		bool remove_attribute(LPCTSTR name);

		// Remove all attributes
		bool remove_attributes();

		// Remove specified child
		bool remove_child(const CXmlNodeUI& n);
		bool remove_child(LPCTSTR name);

		// Remove all children
		bool remove_children();

		// Get internal pointer
		PVOID internal_object() const;
	protected:
		PVOID _root; //xml_node_struct*
	};

} // namespace DuiLib

#endif // __UIXMLNODE_H__
