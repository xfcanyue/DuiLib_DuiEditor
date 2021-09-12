#include "StdAfx.h"

namespace DuiLib {
///////////////////////////////////////////////////////////////////////////////////////
//
//
//
inline ui_pugi::xml_attribute impxmlattr(PVOID attr) { return ui_pugi::xml_attribute((ui_pugi::xml_attribute_struct *)attr); }

CXmlAttributeUI::CXmlAttributeUI() : _attr(0)
{
}

CXmlAttributeUI::CXmlAttributeUI(PVOID attr_struct) : _attr(attr_struct)
{
}

static void unspecified_bool_xml_attribute(CXmlAttributeUI***)
{
}

CXmlAttributeUI::operator CXmlAttributeUI::unspecified_bool_type() const
{
	return _attr ? unspecified_bool_xml_attribute : 0;
}

bool CXmlAttributeUI::operator!() const
{
	return !_attr;
}

bool CXmlAttributeUI::empty() const
{
	return impxmlattr(_attr).empty();
}

LPCTSTR CXmlAttributeUI::name() const
{
	return impxmlattr(_attr).name();
}

LPCTSTR CXmlAttributeUI::value() const
{
	return impxmlattr(_attr).value();
}

LPCTSTR CXmlAttributeUI::as_string(LPCTSTR def) const
{
	return impxmlattr(_attr).as_string(def);
}

int CXmlAttributeUI::as_int(int def) const
{
	return impxmlattr(_attr).as_int(def);
}

unsigned int CXmlAttributeUI::as_uint(unsigned int def) const
{
	return impxmlattr(_attr).as_uint(def);
}

double CXmlAttributeUI::as_double(double def) const
{
	return impxmlattr(_attr).as_double(def);
}

float CXmlAttributeUI::as_float(float def) const
{
	return impxmlattr(_attr).as_float(def);
}

bool CXmlAttributeUI::as_bool(bool def) const
{
	return impxmlattr(_attr).as_bool(def);
}

POINT CXmlAttributeUI::as_point(int def_x, int def_y) const
{
	CDuiPoint pt;
	if(!pt.FromString(as_string()))
	{
		pt.x = def_x;
		pt.y = def_y;
	}
	return pt;
}

SIZE CXmlAttributeUI::as_size(int def_cx, int def_cy) const
{
	CDuiSize sz;
	if(!sz.FromString(as_string()))
	{
		sz.cx = def_cx;
		sz.cy = def_cy;
	}
	return sz;
}

RECT CXmlAttributeUI::as_rect(int def_left, int def_top, int def_right, int def_bottom) const
{
	CDuiRect rc;
	if(!rc.FromString(as_string()))
	{
		rc.left = def_left;
		rc.top = def_top;
		rc.right = def_right;
		rc.bottom = def_bottom;
	}
	return rc;
}

DWORD CXmlAttributeUI::as_uicolor(DWORD def_color) const
{
// 	LPCTSTR pstr = as_string();
// 	if(!pstr || *pstr == _T('\0'))
// 		return def_color;
// 
// 	LPCTSTR cp = pstr;
// 	while (cp && _istxdigit(*cp))
// 	{
// 	}
	LPCTSTR pstrValue = as_string();
	DWORD clrColor = 0;
	while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
	if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
	LPTSTR pstr = NULL;
	clrColor = _tcstoul(pstrValue, &pstr, 16);
	return clrColor;
}

bool CXmlAttributeUI::set_name(LPCTSTR rhs)
{
	return impxmlattr(_attr).set_name(rhs);
}

bool CXmlAttributeUI::set_value(LPCTSTR rhs)
{
	return impxmlattr(_attr).set_value(rhs);
}

bool CXmlAttributeUI::set_value(int rhs)
{
	return impxmlattr(_attr).set_value(rhs);
}

bool CXmlAttributeUI::set_value(unsigned int rhs)
{
	return impxmlattr(_attr).set_value(rhs);
}

bool CXmlAttributeUI::set_value(long rhs)
{
	return impxmlattr(_attr).set_value(rhs);
}

bool CXmlAttributeUI::set_value(unsigned long rhs)
{
	return impxmlattr(_attr).set_value(rhs);
}

bool CXmlAttributeUI::set_value(double rhs)
{
	return impxmlattr(_attr).set_value(rhs);
}

bool CXmlAttributeUI::set_value(double rhs, int precision)
{
	return impxmlattr(_attr).set_value(rhs);
}

bool CXmlAttributeUI::set_value(float rhs)
{
	return impxmlattr(_attr).set_value(rhs);
}

bool CXmlAttributeUI::set_value(float rhs, int precision)
{
	return impxmlattr(_attr).set_value(rhs);
}

bool CXmlAttributeUI::set_value(bool rhs)
{
	return impxmlattr(_attr).set_value(rhs);
}

bool CXmlAttributeUI::set_value(POINT rhs)
{
	CDuiPoint pt(rhs);
	return set_value(pt.ToString().GetData());
}

bool CXmlAttributeUI::set_value(SIZE rhs)
{
	CDuiSize sz(rhs);
	return set_value(sz.ToString().GetData());
}

bool CXmlAttributeUI::set_value(RECT rhs)
{
	CDuiRect rc(rhs);
	return set_value(rc.ToString().GetData());
}

bool CXmlAttributeUI::set_uicolor(DWORD rhs)
{
	CDuiString s;
	s.Format(_T("0x%08X"), rhs);
	return set_value(s.GetData());
}

CXmlAttributeUI CXmlAttributeUI::next_attribute() const
{
	return CXmlAttributeUI(impxmlattr(_attr).next_attribute().internal_object());
}

CXmlAttributeUI CXmlAttributeUI::previous_attribute() const
{
	return CXmlAttributeUI(impxmlattr(_attr).previous_attribute().internal_object());
}

PVOID CXmlAttributeUI::internal_object() const
{
	return _attr;
}

} // namespace DuiLib
