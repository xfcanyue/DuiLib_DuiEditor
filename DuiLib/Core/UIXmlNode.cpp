#include "StdAfx.h"

namespace DuiLib {
///////////////////////////////////////////////////////////////////////////////////////
//
//
//
#ifdef _UNICODE
	//
	#define XMLSTRA(x)	CDuiString(x)
	//
	#define XMLSTRW(x)	x
#else
	//
	#define XMLSTRA(x)	x
	//
	#define XMLSTRW(x)	CDuiString(x)
#endif

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

CXmlNodeUI CXmlNodeUI::child(LPCSTR name) const
{
	return CXmlNodeUI(impxmlnode(_root).child(XMLSTRA(name)).internal_object());
}

CXmlNodeUI CXmlNodeUI::child(LPCWSTR name) const
{
	return CXmlNodeUI(impxmlnode(_root).child(XMLSTRW(name)).internal_object());
}

CXmlAttributeUI CXmlNodeUI::attribute(LPCSTR name) const
{
	return CXmlAttributeUI(impxmlnode(_root).attribute(XMLSTRA(name)).internal_object());
}

CXmlAttributeUI CXmlNodeUI::attribute(LPCWSTR name) const
{
	return CXmlAttributeUI(impxmlnode(_root).attribute(XMLSTRW(name)).internal_object());
}

CXmlNodeUI CXmlNodeUI::next_sibling(LPCSTR name) const
{
	return CXmlNodeUI(impxmlnode(_root).next_sibling(XMLSTRA(name)).internal_object());
}

CXmlNodeUI CXmlNodeUI::next_sibling(LPCWSTR name) const
{
	return CXmlNodeUI(impxmlnode(_root).next_sibling(XMLSTRW(name)).internal_object());
}

CXmlNodeUI CXmlNodeUI::previous_sibling(LPCSTR name) const
{
	return CXmlNodeUI(impxmlnode(_root).previous_sibling(XMLSTRA(name)).internal_object());
}

CXmlNodeUI CXmlNodeUI::previous_sibling(LPCWSTR name) const
{
	return CXmlNodeUI(impxmlnode(_root).previous_sibling(XMLSTRW(name)).internal_object());
}

CXmlNodeUI CXmlNodeUI::child_auto(LPCSTR name)
{
	CXmlNodeUI node = child(XMLSTRA(name));
	if(!node)
		node = append_child(XMLSTRA(name));
	return node;
}

CXmlNodeUI CXmlNodeUI::child_auto(LPCWSTR name)
{
	CXmlNodeUI node = child(XMLSTRW(name));
	if(!node)
		node = append_child(XMLSTRW(name));
	return node;
}

CXmlAttributeUI CXmlNodeUI::attribute_auto(LPCSTR name)
{
	CXmlAttributeUI attr = attribute(XMLSTRA(name));
	if(!attr)
		attr = append_attribute(XMLSTRA(name));
	return attr;
}

CXmlAttributeUI CXmlNodeUI::attribute_auto(LPCWSTR name)
{
	CXmlAttributeUI attr = attribute(XMLSTRW(name));
	if(!attr)
		attr = append_attribute(XMLSTRW(name));
	return attr;
}

bool CXmlNodeUI::set_name(LPCSTR rhs)
{
	return impxmlnode(_root).set_name(XMLSTRA(rhs));
}

bool CXmlNodeUI::set_name(LPCWSTR rhs)
{
	return impxmlnode(_root).set_name(XMLSTRW(rhs));
}

bool CXmlNodeUI::set_value(LPCTSTR rhs)
{
	return impxmlnode(_root).set_value(rhs);
}

CXmlAttributeUI CXmlNodeUI::append_attribute(LPCSTR name)
{
	return CXmlAttributeUI(impxmlnode(_root).append_attribute(XMLSTRA(name)).internal_object());
}

CXmlAttributeUI CXmlNodeUI::append_attribute(LPCWSTR name)
{
	return CXmlAttributeUI(impxmlnode(_root).append_attribute(XMLSTRW(name)).internal_object());
}

CXmlAttributeUI CXmlNodeUI::prepend_attribute(LPCSTR name)
{
	return CXmlAttributeUI(impxmlnode(_root).prepend_attribute(XMLSTRA(name)).internal_object());
}

CXmlAttributeUI CXmlNodeUI::prepend_attribute(LPCWSTR name)
{
	return CXmlAttributeUI(impxmlnode(_root).prepend_attribute(XMLSTRW(name)).internal_object());
}

CXmlAttributeUI CXmlNodeUI::insert_attribute_after(LPCSTR name, const CXmlAttributeUI& attr)
{
	return CXmlAttributeUI(impxmlnode(_root).insert_attribute_after(XMLSTRA(name), ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)attr.internal_object())).internal_object());
}

CXmlAttributeUI CXmlNodeUI::insert_attribute_after(LPCWSTR name, const CXmlAttributeUI& attr)
{
	return CXmlAttributeUI(impxmlnode(_root).insert_attribute_after(XMLSTRW(name), ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)attr.internal_object())).internal_object());
}

CXmlAttributeUI CXmlNodeUI::insert_attribute_before(LPCSTR name, const CXmlAttributeUI& attr)
{
	return CXmlAttributeUI(impxmlnode(_root).insert_attribute_before(XMLSTRA(name), ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)attr.internal_object())).internal_object());
}

CXmlAttributeUI CXmlNodeUI::insert_attribute_before(LPCWSTR name, const CXmlAttributeUI& attr)
{
	return CXmlAttributeUI(impxmlnode(_root).insert_attribute_before(XMLSTRW(name), ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)attr.internal_object())).internal_object());
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

CXmlNodeUI CXmlNodeUI::append_child(LPCSTR name)
{
	return CXmlNodeUI(impxmlnode(_root).append_child(XMLSTRA(name)).internal_object());
}

CXmlNodeUI CXmlNodeUI::append_child(LPCWSTR name)
{
	return CXmlNodeUI(impxmlnode(_root).append_child(XMLSTRW(name)).internal_object());
}

CXmlNodeUI CXmlNodeUI::prepend_child(LPCSTR name)
{
	return CXmlNodeUI(impxmlnode(_root).prepend_child(XMLSTRA(name)).internal_object());
}

CXmlNodeUI CXmlNodeUI::prepend_child(LPCWSTR name)
{
	return CXmlNodeUI(impxmlnode(_root).prepend_child(XMLSTRW(name)).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_child_after(LPCSTR name, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_child_after(XMLSTRA(name), ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_child_after(LPCWSTR name, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_child_after(XMLSTRW(name), ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_child_before(LPCSTR name, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_child_before(XMLSTRA(name), ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
}

CXmlNodeUI CXmlNodeUI::insert_child_before(LPCWSTR name, const CXmlNodeUI& node)
{
	return CXmlNodeUI(impxmlnode(_root).insert_child_before(XMLSTRW(name), ui_pugi::xml_node((ui_pugi::xml_node_struct *)node.internal_object())).internal_object());
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

bool CXmlNodeUI::remove_attribute(LPCSTR name)
{
	return impxmlnode(_root).remove_attribute(XMLSTRA(name));
}

bool CXmlNodeUI::remove_attribute(LPCWSTR name)
{
	return impxmlnode(_root).remove_attribute(XMLSTRW(name));
}

bool CXmlNodeUI::remove_attributes()
{
	return impxmlnode(_root).remove_attributes();
}

bool CXmlNodeUI::remove_child(const CXmlNodeUI& n)
{
	return impxmlnode(_root).remove_child(ui_pugi::xml_node((ui_pugi::xml_node_struct *)n.internal_object()));
}

bool CXmlNodeUI::remove_child(LPCSTR name)
{
	return impxmlnode(_root).remove_child(XMLSTRA(name));
}

bool CXmlNodeUI::remove_child(LPCWSTR name)
{
	return impxmlnode(_root).remove_child(XMLSTRW(name));
}

bool CXmlNodeUI::remove_children()
{
	return impxmlnode(_root).remove_children();
}

CXmlNodeUI CXmlNodeUI::find_child_by_attribute(LPCSTR name, LPCSTR attr_name, LPCTSTR attr_value)
{
	return CXmlNodeUI(impxmlnode(_root).find_child_by_attribute(XMLSTRA(name), XMLSTRA(attr_name), attr_value).internal_object());
}

CXmlNodeUI CXmlNodeUI::find_child_by_attribute(LPCWSTR name, LPCWSTR attr_name, LPCTSTR attr_value)
{
	return CXmlNodeUI(impxmlnode(_root).find_child_by_attribute(XMLSTRW(name), XMLSTRW(attr_name), attr_value).internal_object());
}

PVOID CXmlNodeUI::internal_object() const
{
	return _root;
}

struct ui_xml_string_writer : ui_pugi::xml_writer
{
	virtual void write(const void* data, size_t size)
	{
		m_str += std::string((const char *)data, size);
	}
	std::string m_str;
};

CDuiString CXmlNodeUI::ToString()
{
	ui_xml_string_writer wt;
	impxmlnode(_root).print(wt);

	UISTRING_CONVERSION;
	return UIUTF82T(wt.m_str.c_str());
}

} // namespace DuiLib
