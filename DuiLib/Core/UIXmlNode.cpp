#include "StdAfx.h"

namespace DuiLib {
///////////////////////////////////////////////////////////////////////////////////////
//
//
//
inline static ui_pugi::xml_node impxmlnode(PVOID node_struct) { return ui_pugi::xml_node((ui_pugi::xml_node_struct *)node_struct); }

inline static ui_pugi::xml_node_type impnodetype(xml_node_type_ui type)
{
	if(type == ui_node_null)		return ui_pugi::node_null;
	if(type == ui_node_document)	return ui_pugi::node_document;
	if(type == ui_node_element)		return ui_pugi::node_element;
	if(type == ui_node_pcdata)		return ui_pugi::node_pcdata;
	if(type == ui_node_cdata)		return ui_pugi::node_cdata;
	if(type == ui_node_comment)		return ui_pugi::node_comment;
	if(type == ui_node_pi)			return ui_pugi::node_pi;
	if(type == ui_node_declaration)	return ui_pugi::node_declaration;
	if(type == ui_node_doctype)		return ui_pugi::node_doctype;
	return ui_pugi::node_null;
}

inline static xml_node_type_ui impnodetype(ui_pugi::xml_node_type type)
{
	if(type == ui_pugi::node_null)			return ui_node_null;
	if(type == ui_pugi::node_document)		return ui_node_document;
	if(type == ui_pugi::node_element)		return ui_node_element;
	if(type == ui_pugi::node_pcdata)		return ui_node_pcdata;
	if(type == ui_pugi::node_cdata)			return ui_node_cdata;
	if(type == ui_pugi::node_comment)		return ui_node_comment;
	if(type == ui_pugi::node_pi)			return ui_node_pi;
	if(type == ui_pugi::node_declaration)	return ui_node_declaration;
	if(type == ui_pugi::node_doctype)		return ui_node_doctype;
	return ui_node_null;
}

CXmlNodeUI::CXmlNodeUI() : _root(0)
{
}

CXmlNodeUI::CXmlNodeUI(PVOID node_struct) : _root(node_struct)
{
}

static void unspecified_bool_xml_node(CXmlNodeUI***)
{
}

CXmlNodeUI::operator CXmlNodeUI::unspecified_bool_type() const
{
	return _root ? unspecified_bool_xml_node : 0;
}

bool CXmlNodeUI::operator!() const
{
	return !_root;
}

bool CXmlNodeUI::empty() const
{
	return !_root;
}

xml_node_type_ui CXmlNodeUI::type() const
{
	return impnodetype(impxmlnode(_root).type());
}

LPCTSTR CXmlNodeUI::name() const
{
	return impxmlnode(_root).name();
}

LPCTSTR CXmlNodeUI::value() const
{
	return impxmlnode(_root).value();
}

CXmlAttributeUI CXmlNodeUI::first_attribute() const
{
	return CXmlAttributeUI(impxmlnode(_root).first_attribute().internal_object());
}

CXmlAttributeUI CXmlNodeUI::last_attribute() const
{
	return CXmlAttributeUI(impxmlnode(_root).last_attribute().internal_object());
}

CXmlNodeUI CXmlNodeUI::first_child() const
{
	return CXmlNodeUI(impxmlnode(_root).first_child().internal_object());
}

CXmlNodeUI CXmlNodeUI::last_child() const
{
	return CXmlNodeUI(impxmlnode(_root).last_child().internal_object());
}

CXmlNodeUI CXmlNodeUI::next_sibling() const
{
	return CXmlNodeUI(impxmlnode(_root).next_sibling().internal_object());
}

CXmlNodeUI CXmlNodeUI::previous_sibling() const
{
	return CXmlNodeUI(impxmlnode(_root).previous_sibling().internal_object());
}

CXmlNodeUI CXmlNodeUI::parent() const
{
	return CXmlNodeUI(impxmlnode(_root).parent().internal_object());
}

CXmlNodeUI CXmlNodeUI::root() const
{
	return CXmlNodeUI(impxmlnode(_root).root().internal_object());
}

CXmlNodeUI CXmlNodeUI::child(LPCTSTR name) const
{
	return CXmlNodeUI(impxmlnode(_root).child(name).internal_object());
}

CXmlAttributeUI CXmlNodeUI::attribute(LPCTSTR name) const
{
	return CXmlAttributeUI(impxmlnode(_root).attribute(name).internal_object());
}

CXmlNodeUI CXmlNodeUI::next_sibling(LPCTSTR name) const
{
	return CXmlNodeUI(impxmlnode(_root).next_sibling(name).internal_object());
}

CXmlNodeUI CXmlNodeUI::previous_sibling(LPCTSTR name) const
{
	return CXmlNodeUI(impxmlnode(_root).previous_sibling(name).internal_object());
}

CXmlNodeUI CXmlNodeUI::child_auto(LPCTSTR name)
{
	CXmlNodeUI node = child(name);
	if(!node)
		node = append_child(name);
	return node;
}

CXmlAttributeUI CXmlNodeUI::attribute_auto(LPCTSTR name)
{
	CXmlAttributeUI attr = attribute(name);
	if(!attr)
		attr = append_attribute(name);
	return attr;
}

bool CXmlNodeUI::set_name(LPCTSTR rhs)
{
	return impxmlnode(_root).set_name(rhs);
}

bool CXmlNodeUI::set_value(LPCTSTR rhs)
{
	return impxmlnode(_root).set_value(rhs);
}

CXmlAttributeUI CXmlNodeUI::append_attribute(LPCTSTR name)
{
	return CXmlAttributeUI(impxmlnode(_root).append_attribute(name).internal_object());
}

CXmlAttributeUI CXmlNodeUI::prepend_attribute(LPCTSTR name)
{
	return CXmlAttributeUI(impxmlnode(_root).prepend_attribute(name).internal_object());
}

CXmlAttributeUI CXmlNodeUI::insert_attribute_after(LPCTSTR name, const CXmlAttributeUI& attr)
{
	return CXmlAttributeUI(impxmlnode(_root).insert_attribute_after(name, ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)attr.internal_object())).internal_object());
}

CXmlAttributeUI CXmlNodeUI::insert_attribute_before(LPCTSTR name, const CXmlAttributeUI& attr)
{
	return CXmlAttributeUI(impxmlnode(_root).insert_attribute_before(name, ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)attr.internal_object())).internal_object());
}

CXmlAttributeUI CXmlNodeUI::append_copy(const CXmlAttributeUI& proto)
{
	return CXmlAttributeUI(impxmlnode(_root).append_copy(ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)proto.internal_object())).internal_object());
}

CXmlAttributeUI CXmlNodeUI::prepend_copy(const CXmlAttributeUI& proto)
{
	return CXmlAttributeUI(impxmlnode(_root).prepend_copy(ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)proto.internal_object())).internal_object());
}

CXmlAttributeUI CXmlNodeUI::insert_copy_after(const CXmlAttributeUI& proto, const CXmlAttributeUI& attr)
{
	return CXmlAttributeUI(impxmlnode(_root).insert_copy_after(ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)proto.internal_object()), ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)attr.internal_object())).internal_object());
}

CXmlAttributeUI CXmlNodeUI::insert_copy_before(const CXmlAttributeUI& proto, const CXmlAttributeUI& attr)
{
	return CXmlAttributeUI(impxmlnode(_root).insert_copy_before(ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)proto.internal_object()), ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)attr.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::append_child(xml_node_type_ui type)
{
	return CXmlNodeUI(impxmlnode(_root).append_child(impnodetype(type)).internal_object());
}

CXmlNodeUI CXmlNodeUI::prepend_child(xml_node_type_ui type)
{
	return CXmlNodeUI(impxmlnode(_root).prepend_child(impnodetype(type)).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_child_after(xml_node_type_ui type, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_child_after(impnodetype(type), ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_child_before(xml_node_type_ui type, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_child_before(impnodetype(type), ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::append_child(LPCTSTR name)
{
	return CXmlNodeUI(impxmlnode(_root).append_child(name).internal_object());
}

CXmlNodeUI CXmlNodeUI::prepend_child(LPCTSTR name)
{
	return CXmlNodeUI(impxmlnode(_root).prepend_child(name).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_child_after(LPCTSTR name, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_child_after(name, ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_child_before(LPCTSTR name, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_child_before(name, ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::append_copy(const CXmlNodeUI& proto)
{
	return CXmlNodeUI(impxmlnode(_root).append_copy(ui_pugi::xml_node((ui_pugi::xml_node_struct *)proto.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::prepend_copy(const CXmlNodeUI& proto)
{
	return CXmlNodeUI(impxmlnode(_root).prepend_copy(ui_pugi::xml_node((ui_pugi::xml_node_struct *)proto.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_copy_after(const CXmlNodeUI& proto, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_copy_after(ui_pugi::xml_node((ui_pugi::xml_node_struct *)proto.internal_object()), ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_copy_before(const CXmlNodeUI& proto, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_copy_after(ui_pugi::xml_node((ui_pugi::xml_node_struct *)proto.internal_object()), ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::append_move(const CXmlNodeUI& moved)
{
	return CXmlNodeUI(impxmlnode(_root).append_move(ui_pugi::xml_node((ui_pugi::xml_node_struct *)moved.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::prepend_move(const CXmlNodeUI& moved)
{
	return CXmlNodeUI(impxmlnode(_root).prepend_move(ui_pugi::xml_node((ui_pugi::xml_node_struct *)moved.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_move_after(const CXmlNodeUI& moved, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_move_after(ui_pugi::xml_node((ui_pugi::xml_node_struct *)moved.internal_object()), ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_move_before(const CXmlNodeUI& moved, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_move_before(ui_pugi::xml_node((ui_pugi::xml_node_struct *)moved.internal_object()), ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
}

bool CXmlNodeUI::remove_attribute(const CXmlAttributeUI& a)
{
	return impxmlnode(_root).remove_attribute(ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)a.internal_object()));
}

bool CXmlNodeUI::remove_attribute(LPCTSTR name)
{
	return impxmlnode(_root).remove_attribute(name);
}

bool CXmlNodeUI::remove_attributes()
{
	return impxmlnode(_root).remove_attributes();
}

bool CXmlNodeUI::remove_child(const CXmlNodeUI& n)
{
	return impxmlnode(_root).remove_child(ui_pugi::xml_node((ui_pugi::xml_node_struct *)n.internal_object()));
}

bool CXmlNodeUI::remove_child(LPCTSTR name)
{
	return impxmlnode(_root).remove_child(name);
}

bool CXmlNodeUI::remove_children()
{
	return impxmlnode(_root).remove_children();
}

PVOID CXmlNodeUI::internal_object() const
{
	return _root;
}

} // namespace DuiLib
