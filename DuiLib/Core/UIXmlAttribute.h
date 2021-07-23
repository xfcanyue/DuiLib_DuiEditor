#ifndef __UIXMLATTRIBUTE_H__
#define __UIXMLATTRIBUTE_H__

#pragma once

namespace DuiLib {

	class UILIB_API CXmlAttributeUI
	{
		
	public:
		CXmlAttributeUI();
		explicit CXmlAttributeUI(PVOID attr_struct);

		typedef void (*unspecified_bool_type)(CXmlAttributeUI***);
		// Safe bool conversion operator
		operator unspecified_bool_type() const;

		bool operator!() const;

		bool empty() const;

		LPCTSTR name() const;
		LPCTSTR value() const;

		LPCTSTR		as_string(LPCTSTR def = _T("")) const;
		int			as_int(int def = 0) const;
		unsigned int as_uint(unsigned int def = 0) const;
		double		as_double(double def = 0) const;
		float		as_float(float def = 0) const;
		bool		as_bool(bool def = false) const;

		POINT		as_point(int def_x=0, int def_y=0) const;
		SIZE		as_size(int defc_x=0, int def_cy=0) const;
		RECT		as_rect(int def_left=0, int def_top=0, int def_right=0, int def_bottom=0) const;
		DWORD		as_uicolor(DWORD def_color = 0) const;

		bool set_name(LPCTSTR rhs);

		bool set_value(LPCTSTR rhs);
		bool set_value(int rhs);
		bool set_value(unsigned int rhs);
		bool set_value(long rhs);
		bool set_value(unsigned long rhs);
		bool set_value(double rhs);
		bool set_value(double rhs, int precision);
		bool set_value(float rhs);
		bool set_value(float rhs, int precision);
		bool set_value(bool rhs);

		bool set_value(POINT rhs);
		bool set_value(SIZE rhs);
		bool set_value(RECT rhs);
		bool set_uicolor(DWORD rhs);

		// Get next/previous attribute in the attribute list of the parent node
		CXmlAttributeUI next_attribute() const;
		CXmlAttributeUI previous_attribute() const;

		// Get internal pointer
		PVOID internal_object() const;
	protected:
		PVOID _attr; //xml_attribute_struct*
	};

} // namespace DuiLib

#endif // __UIXMLATTRIBUTE_H__
