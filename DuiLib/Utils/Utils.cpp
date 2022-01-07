#include "StdAfx.h"
#include "Utils.h"

namespace DuiLib
{

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CDuiPoint::CDuiPoint()
	{
		x = y = 0;
	}

	CDuiPoint::CDuiPoint(const POINT& src)
	{
		x = src.x;
		y = src.y;
	}

	CDuiPoint::CDuiPoint(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	CDuiPoint::CDuiPoint(LPARAM lParam)
	{
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);
	}

	CDuiPoint::CDuiPoint(LPCTSTR pstrValue)
	{
		FromString(pstrValue);
	}

	bool CDuiPoint::FromString(LPCTSTR pstrValue) //从"x,y"构造POINT
	{
		x = y = 0; 
		if (pstrValue == NULL || *pstrValue == _T('\0')) return false;
		LPTSTR pstr = NULL;
		x = _tcstol(pstrValue, &pstr, 10); if(!pstr) return false;
		y = _tcstol(pstr + 1, &pstr, 10);  if(!pstr) return false;
		return true;
	}

	CDuiString CDuiPoint::ToString()				//输出字符串"x,y"
	{
		CDuiString sPoint;
		sPoint.SmallFormat(_T("%ld,%ld"), x, y);
		return sPoint;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CDuiSize::CDuiSize()
	{
		cx = cy = 0;
	}

	CDuiSize::CDuiSize(const SIZE& src)
	{
		cx = src.cx;
		cy = src.cy;
	}

	CDuiSize::CDuiSize(const RECT rc)
	{
		cx = rc.right - rc.left;
		cy = rc.bottom - rc.top;
	}

	CDuiSize::CDuiSize(int _cx, int _cy)
	{
		cx = _cx;
		cy = _cy;
	}

	CDuiSize::CDuiSize(LPCTSTR pstrValue)
	{
		FromString(pstrValue);
	}

	bool CDuiSize::FromString(LPCTSTR pstrValue) //从"cx,cy"构造SIZE
	{
		cx = cy = 0;
		if (pstrValue == NULL || *pstrValue == _T('\0')) return false;
		LPTSTR pstr = NULL;
		cx = _tcstol(pstrValue, &pstr, 10);		if(!pstr) return false;
		cy = _tcstol(pstr + 1, &pstr, 10);      if(!pstr) return false;
		return true;
	}

	CDuiString CDuiSize::ToString()				//输出字符串"cx,cy"
	{
		CDuiString sSize;
		sSize.SmallFormat(_T("%ld,%ld"), cx, cy);
		return sSize;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CDuiRect::CDuiRect()
	{
		left = top = right = bottom = 0;
	}

	CDuiRect::CDuiRect(const RECT& src)
	{
		left = src.left;
		top = src.top;
		right = src.right;
		bottom = src.bottom;
	}

	CDuiRect::CDuiRect(int iLeft, int iTop, int iRight, int iBottom)
	{
		left = iLeft;
		top = iTop;
		right = iRight;
		bottom = iBottom;
	}

	CDuiRect::CDuiRect(LPCTSTR pstrValue)
	{
		FromString(pstrValue);
	}

	bool CDuiRect::FromString(LPCTSTR pstrValue) //从"left,top,right,bottom"构造RECT
	{
		left = top = right = bottom = 0;
		if (pstrValue == NULL || *pstrValue == _T('\0')) return false;
		LPTSTR pstr = NULL;
		left	= _tcstol(pstrValue, &pstr, 10);	if(!pstr) return false;
		top		= _tcstol(pstr + 1, &pstr, 10);		if(!pstr) return false;
		right	= _tcstol(pstr + 1, &pstr, 10);		if(!pstr) return false;
		bottom	= _tcstol(pstr + 1, &pstr, 10);		if(!pstr) return false;
		return true;
	}

	CDuiString CDuiRect::ToString()				//输出字符串"left,top,right,bottom"
	{
		CDuiString sRect;
		sRect.SmallFormat(_T("%ld,%ld,%ld,%ld"), left, top, right, bottom);
		return sRect;
	}

	int CDuiRect::GetWidth() const
	{
		return right - left;
	}

	int CDuiRect::GetHeight() const
	{
		return bottom - top;
	}

	void CDuiRect::Empty()
	{
		left = top = right = bottom = 0;
	}

	bool CDuiRect::IsNull() const
	{
		return (left == 0 && right == 0 && top == 0 && bottom == 0); 
	}

	void CDuiRect::Join(const RECT& rc)
	{
		if( rc.left < left ) left = rc.left;
		if( rc.top < top ) top = rc.top;
		if( rc.right > right ) right = rc.right;
		if( rc.bottom > bottom ) bottom = rc.bottom;
	}

	void CDuiRect::ResetOffset()
	{
		::OffsetRect(this, -left, -top);
	}

	void CDuiRect::Normalize()
	{
		if( left > right ) { int iTemp = left; left = right; right = iTemp; }
		if( top > bottom ) { int iTemp = top; top = bottom; bottom = iTemp; }
	}

	void CDuiRect::Offset(int cx, int cy)
	{
		::OffsetRect(this, cx, cy);
	}

	void CDuiRect::Inflate(int cx, int cy)
	{
		::InflateRect(this, cx, cy);
	}

	void CDuiRect::Deflate(int cx, int cy)
	{
		::InflateRect(this, -cx, -cy);
	}

	void CDuiRect::Union(CDuiRect& rc)
	{
		::UnionRect(this, this, &rc);
	}

	BOOL CDuiRect::IntersectRect(const RECT &rect1, const RECT &rect2)
	{
		return ::IntersectRect(this, (LPRECT)&rect1, (LPRECT)&rect2);
	}

	POINT CDuiRect::CenterPoint() const
	{
		POINT pt = {(left+right)/2, (top+bottom)/2};
		return pt;
	}

	void CDuiRect::AlignRect(const RECT &rc, UINT uAlign)
	{
		POINT ptCenter = {(rc.left+rc.right)/2, (rc.top+rc.bottom)/2};
		int width = right - left;
		int height = bottom - top;
		if((uAlign & DT_CENTER) == DT_CENTER)
		{
			left = ptCenter.x - width/2;
			right = ptCenter.x + width/2;
		}
		else if((uAlign & DT_LEFT) == DT_LEFT)
		{
			left = rc.left;
			right = rc.left + width;
		}
		else if((uAlign & DT_RIGHT) == DT_RIGHT)
		{
			left = rc.right - width;
			right = rc.right;
		}

		if((uAlign & DT_VCENTER) == DT_VCENTER)
		{
			top = ptCenter.y - height/2;
			bottom = ptCenter.y + height/2;
		}
		else if((uAlign & DT_TOP) == DT_TOP)
		{
			top = rc.top;
			bottom = rc.top + height;
		}
		else if((uAlign & DT_BOTTOM) == DT_BOTTOM)
		{
			top = rc.bottom - height;
			bottom = rc.bottom;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CStdPtrArray::CStdPtrArray(int iPreallocSize) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(iPreallocSize), m_bSaveIndexMap(false)
	{
		ASSERT(iPreallocSize>=0);
		if( iPreallocSize > 0 ) m_ppVoid = static_cast<LPVOID*>(malloc(iPreallocSize * sizeof(LPVOID)));
	}

	CStdPtrArray::CStdPtrArray(const CStdPtrArray& src) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(0), m_bSaveIndexMap(false)
	{
		for(int i=0; i<src.GetSize(); i++)
			Add(src.GetAt(i));
	}

	CStdPtrArray::~CStdPtrArray()
	{
		if( m_ppVoid != NULL ) free(m_ppVoid);
	}

	void CStdPtrArray::Empty()
	{
		if( m_ppVoid != NULL ) free(m_ppVoid);
		m_ppVoid = NULL;
		m_nCount = m_nAllocated = 0;
		m_mapIndex.clear();
	}

	void CStdPtrArray::Resize(int iSize)
	{
		Empty();
		m_ppVoid = static_cast<LPVOID*>(malloc(iSize * sizeof(LPVOID)));
		::ZeroMemory(m_ppVoid, iSize * sizeof(LPVOID));
		m_nAllocated = iSize;
		m_nCount = iSize;
	}

	bool CStdPtrArray::IsEmpty() const
	{
		return m_nCount == 0;
	}

	bool CStdPtrArray::Add(LPVOID pData)
	{
		if( ++m_nCount >= m_nAllocated) {
			int nAllocated = m_nAllocated * 2;
			if( nAllocated == 0 ) nAllocated = 11;
			LPVOID* ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, nAllocated * sizeof(LPVOID)));
			if( ppVoid != NULL ) {
				m_nAllocated = nAllocated;
				m_ppVoid = ppVoid;
			}
			else {
				--m_nCount;
				return false;
			}
		}
		m_ppVoid[m_nCount - 1] = pData;
		if(m_bSaveIndexMap){
			m_mapIndex[pData] = m_nCount - 1;
		}
		return true;
	}

	bool CStdPtrArray::InsertAt(int iIndex, LPVOID pData)
	{
		if( iIndex == m_nCount ) return Add(pData);
		if( iIndex < 0 || iIndex > m_nCount ) return false;
		if( ++m_nCount >= m_nAllocated) {
			int nAllocated = m_nAllocated * 2;
			if( nAllocated == 0 ) nAllocated = 11;
			LPVOID* ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, nAllocated * sizeof(LPVOID)));
			if( ppVoid != NULL ) {
				m_nAllocated = nAllocated;
				m_ppVoid = ppVoid;
			}
			else {
				--m_nCount;
				return false;
			}
		}
		memmove(&m_ppVoid[iIndex + 1], &m_ppVoid[iIndex], (m_nCount - iIndex - 1) * sizeof(LPVOID));
		m_ppVoid[iIndex] = pData;

		if(m_bSaveIndexMap)
		{
			for( int i = iIndex; i < m_nCount; i++ ) m_mapIndex[m_ppVoid[i]] = i;
		}
		return true;
	}

	bool CStdPtrArray::SetAt(int iIndex, LPVOID pData)
	{
		if( iIndex < 0 || iIndex >= m_nCount ) return false;

		if(m_bSaveIndexMap)
		{
			if(m_ppVoid[iIndex] != NULL) 
			{
				std::map<LPVOID, int>::iterator it = m_mapIndex.find(m_ppVoid[iIndex]);
				if(it != m_mapIndex.end()) m_mapIndex.erase(it);
			}
			m_mapIndex[pData] = iIndex;
		}

		m_ppVoid[iIndex] = pData;
		return true;
	}

	bool CStdPtrArray::Remove(int iIndex)
	{
		if( iIndex < 0 || iIndex >= m_nCount ) return false;

		if(m_bSaveIndexMap)
		{
			if(m_ppVoid[iIndex] != NULL) 
			{
				std::map<LPVOID, int>::iterator it = m_mapIndex.find(m_ppVoid[iIndex]);
				if(it != m_mapIndex.end()) m_mapIndex.erase(it);
			}
		}

		if( iIndex < --m_nCount ) ::CopyMemory(m_ppVoid + iIndex, m_ppVoid + iIndex + 1, (m_nCount - iIndex) * sizeof(LPVOID));

		if(m_bSaveIndexMap)
		{
			for( int i = iIndex; i < m_nCount; i++ ) m_mapIndex[m_ppVoid[i]] = i;
		}
		return true;
	}

	bool CStdPtrArray::Remove(LPVOID pData)
	{
		int nIndex = Find(pData);
		if(nIndex >= 0)
			return Remove(nIndex);
		return false;
	}

	int CStdPtrArray::Find(LPVOID pData) const
	{
		if(m_bSaveIndexMap)
		{
			CStdPtrArray *pArray = const_cast<CStdPtrArray *>(this);
			return pArray->FindInMap(pData);
		}
		for( int i = 0; i < m_nCount; i++ ) if( m_ppVoid[i] == pData ) return i;
		return -1;
	}

	int CStdPtrArray::FindInMap(LPVOID pData)
	{
		std::map<LPVOID, int>::iterator it = m_mapIndex.find(pData);
		if(it == m_mapIndex.end()) return -1;
		return it->second;
	}

	int CStdPtrArray::GetSize() const
	{
		return m_nCount;
	}

	LPVOID* CStdPtrArray::GetData()
	{
		return m_ppVoid;
	}

	LPVOID CStdPtrArray::GetAt(int iIndex) const
	{
		if( iIndex < 0 || iIndex >= m_nCount ) return NULL;
		return m_ppVoid[iIndex];
	}

	LPVOID CStdPtrArray::operator[] (int iIndex) const
	{
		ASSERT(iIndex>=0 && iIndex<m_nCount);
		return m_ppVoid[iIndex];
	}

	void CStdPtrArray::SetSaveIndexMap(bool bIndexMap)
	{
		m_bSaveIndexMap = bIndexMap;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CStdValArray::CStdValArray(int iElementSize, int iPreallocSize /*= 0*/) : 
	m_pVoid(NULL), 
		m_nCount(0), 
		m_iElementSize(iElementSize), 
		m_nAllocated(iPreallocSize)
	{
		ASSERT(iElementSize>0);
		ASSERT(iPreallocSize>=0);
		if( iPreallocSize > 0 ) m_pVoid = static_cast<LPBYTE>(malloc(iPreallocSize * m_iElementSize));
	}

	CStdValArray::~CStdValArray()
	{
		if( m_pVoid != NULL ) free(m_pVoid);
	}

	void CStdValArray::Empty()
	{   
		m_nCount = 0;  // NOTE: We keep the memory in place
	}

	bool CStdValArray::IsEmpty() const
	{
		return m_nCount == 0;
	}

	bool CStdValArray::Add(LPCVOID pData)
	{
		if( ++m_nCount >= m_nAllocated) {
			int nAllocated = m_nAllocated * 2;
			if( nAllocated == 0 ) nAllocated = 11;
			LPBYTE pVoid = static_cast<LPBYTE>(realloc(m_pVoid, nAllocated * m_iElementSize));
			if( pVoid != NULL ) {
				m_nAllocated = nAllocated;
				m_pVoid = pVoid;
			}
			else {
				--m_nCount;
				return false;
			}
		}
		::CopyMemory(m_pVoid + ((m_nCount - 1) * m_iElementSize), pData, m_iElementSize);
		return true;
	}

	bool CStdValArray::Remove(int iIndex)
	{
		if( iIndex < 0 || iIndex >= m_nCount ) return false;
		if( iIndex < --m_nCount ) ::CopyMemory(m_pVoid + (iIndex * m_iElementSize), m_pVoid + ((iIndex + 1) * m_iElementSize), (m_nCount - iIndex) * m_iElementSize);
		return true;
	}

	int CStdValArray::GetSize() const
	{
		return m_nCount;
	}

	LPVOID CStdValArray::GetData()
	{
		return static_cast<LPVOID>(m_pVoid);
	}

	LPVOID CStdValArray::GetAt(int iIndex) const
	{
		if( iIndex < 0 || iIndex >= m_nCount ) return NULL;
		return m_pVoid + (iIndex * m_iElementSize);
	}

	LPVOID CStdValArray::operator[] (int iIndex) const
	{
		ASSERT(iIndex>=0 && iIndex<m_nCount);
		return m_pVoid + (iIndex * m_iElementSize);
	}


	/////////////////////////////////////////////////////////////////////////////
	//
	//

	static UINT HashKey(LPCTSTR Key)
	{
		UINT i = 0;
		SIZE_T len = _tcslen(Key);
		while( len-- > 0 ) i = (i << 5) + i + Key[len];
		return i;
	}

	static UINT HashKey(const CDuiString& Key)
	{
		return HashKey((LPCTSTR)Key);
	};

	CStdStringPtrMap::CStdStringPtrMap(int nSize) : m_nCount(0)
	{
		if( nSize < 16 ) nSize = 16;
		m_nBuckets = nSize;
		m_aT = new TITEM*[nSize];
		memset(m_aT, 0, nSize * sizeof(TITEM*));
	}

	CStdStringPtrMap::~CStdStringPtrMap()
	{
		if( m_aT ) {
			int len = m_nBuckets;
			while( len-- ) {
				TITEM* pItem = m_aT[len];
				while( pItem ) {
					TITEM* pKill = pItem;
					pItem = pItem->pNext;
					delete pKill;
				}
			}
			delete [] m_aT;
			m_aT = NULL;
		}
	}

	void CStdStringPtrMap::RemoveAll()
	{
		this->Resize(m_nBuckets);
	}

	void CStdStringPtrMap::Resize(int nSize)
	{
		if( m_aT ) {
			int len = m_nBuckets;
			while( len-- ) {
				TITEM* pItem = m_aT[len];
				while( pItem ) {
					TITEM* pKill = pItem;
					pItem = pItem->pNext;
					delete pKill;
				}
			}
			delete [] m_aT;
			m_aT = NULL;
		}

		if( nSize < 0 ) nSize = 0;
		if( nSize > 0 ) {
			m_aT = new TITEM*[nSize];
			memset(m_aT, 0, nSize * sizeof(TITEM*));
		} 
		m_nBuckets = nSize;
		m_nCount = 0;
	}

	LPVOID CStdStringPtrMap::Find(LPCTSTR key, bool optimize) const
	{
		if( m_nBuckets == 0 || GetSize() == 0 ) return NULL;

		UINT slot = HashKey(key) % m_nBuckets;
		for( TITEM* pItem = m_aT[slot]; pItem; pItem = pItem->pNext ) {
			if( pItem->Key == key ) {
				if (optimize && pItem != m_aT[slot]) {
					if (pItem->pNext) {
						pItem->pNext->pPrev = pItem->pPrev;
					}
					pItem->pPrev->pNext = pItem->pNext;
					pItem->pPrev = NULL;
					pItem->pNext = m_aT[slot];
					pItem->pNext->pPrev = pItem;
					//将item移动至链条头部
					m_aT[slot] = pItem;
				}
				return pItem->Data;
			}        
		}

		return NULL;
	}

	bool CStdStringPtrMap::Insert(LPCTSTR key, LPVOID pData)
	{
		if( m_nBuckets == 0 ) return false;
		if( Find(key) ) return false;

		// Add first in bucket
		UINT slot = HashKey(key) % m_nBuckets;
		TITEM* pItem = new TITEM;
		pItem->Key = key;
		pItem->Data = pData;
		pItem->pPrev = NULL;
		pItem->pNext = m_aT[slot];
		if (pItem->pNext)
			pItem->pNext->pPrev = pItem;
		m_aT[slot] = pItem;
		m_nCount++;
		return true;
	}

	LPVOID CStdStringPtrMap::Set(LPCTSTR key, LPVOID pData)
	{
		if( m_nBuckets == 0 ) return pData;

		if (GetSize()>0) {
			UINT slot = HashKey(key) % m_nBuckets;
			// Modify existing item
			for( TITEM* pItem = m_aT[slot]; pItem; pItem = pItem->pNext ) {
				if( pItem->Key == key ) {
					LPVOID pOldData = pItem->Data;
					pItem->Data = pData;
					return pOldData;
				}
			}
		}

		Insert(key, pData);
		return NULL;
	}

	bool CStdStringPtrMap::Remove(LPCTSTR key)
	{
		if( m_nBuckets == 0 || GetSize() == 0 ) return false;

		UINT slot = HashKey(key) % m_nBuckets;
		TITEM** ppItem = &m_aT[slot];
		while( *ppItem ) {
			if( (*ppItem)->Key == key ) {
				TITEM* pKill = *ppItem;
				*ppItem = (*ppItem)->pNext;
				if (*ppItem)
					(*ppItem)->pPrev = pKill->pPrev;
				delete pKill;
				m_nCount--;
				return true;
			}
			ppItem = &((*ppItem)->pNext);
		}

		return false;
	}

	int CStdStringPtrMap::GetSize() const
	{
#if 0//def _DEBUG
		int nCount = 0;
		int len = m_nBuckets;
		while( len-- ) {
			for( const TITEM* pItem = m_aT[len]; pItem; pItem = pItem->pNext ) nCount++;
		}
		ASSERT(m_nCount==nCount);
#endif
		return m_nCount;
	}

	LPCTSTR CStdStringPtrMap::GetAt(int iIndex) const
	{
		if( m_nBuckets == 0 || GetSize() == 0 ) return false;

		int pos = 0;
		int len = m_nBuckets;
		while( len-- ) {
			for( TITEM* pItem = m_aT[len]; pItem; pItem = pItem->pNext ) {
				if( pos++ == iIndex ) {
					return pItem->Key.GetData();
				}
			}
		}

		return NULL;
	}

	LPCTSTR CStdStringPtrMap::operator[] (int nIndex) const
	{
		return GetAt(nIndex);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	CDuiStringArray::CDuiStringArray()
	{

	}

	CDuiStringArray::CDuiStringArray(const CDuiStringArray& src)
	{
		RemoveAll();
		for (int i=0; i<src.GetSize(); i++)
		{
			Add(src.GetAt(i), false);
		}
	}

	CDuiStringArray::~CDuiStringArray()
	{
		RemoveAll();
	}

	int CDuiStringArray::GetSize() const
	{
		return m_map.GetSize();
	}

	bool CDuiStringArray::IsEmpty() const
	{
		return m_map.GetSize() <= 0;
	}

	void CDuiStringArray::RemoveAll()
	{
		CDuiString *pstr;
		for( int i = 0; i< m_map.GetSize(); i++ )
		{
			if(LPCTSTR key = m_map.GetAt(i))
			{
				pstr = static_cast<CDuiString *>(m_map.Find(key));
				delete pstr; 
			}
		}
		m_map.RemoveAll();
	}

	bool CDuiStringArray::Add(LPCTSTR newElement, bool bNoRepeat)
	{
		if(newElement == NULL || *newElement == _T('\0')) return false;
		if(bNoRepeat && Find(newElement) != NULL) return false;

		CDuiString *pstr = new CDuiString(newElement);
		if(!m_map.Insert(newElement, pstr))
		{
			delete pstr;
			return false;
		}
		return true;
	}

	bool CDuiStringArray::Remove(LPCTSTR element)
	{
		CDuiString *pstr = static_cast<CDuiString *>(m_map.Find(element));
		if(!pstr) return false;
		delete pstr;
		m_map.Remove(element);
		return true;
	}

	bool CDuiStringArray::Find(LPCTSTR element)
	{
		return m_map.Find(element) != NULL;
	}

	CDuiString CDuiStringArray::GetAt(int iIndex) const
	{
		LPCTSTR key = m_map.GetAt(iIndex);
		if(key == NULL) return _T("");
		CDuiString *pstr = static_cast<CDuiString *>(m_map.Find(key));
		return *pstr;
	}

	CDuiString& CDuiStringArray::operator[] (int nIndex) const
	{
		//数组方式返回引用.
		LPCTSTR key = m_map.GetAt(nIndex);
		ASSERT(key);
		CDuiString *pstr = static_cast<CDuiString *>(m_map.Find(key));
		return *pstr;
	}

	void CDuiStringArray::SplitString(LPCTSTR src, LPCTSTR sp)
	{
		if(sp == NULL || *sp == _T('\0'))
		{
			CDuiString s;
			while (*src != _T('\0'))
			{
				if(!_istalnum(*src))
				{
					if(!s.IsEmpty())
					{
						Add(s); s.Empty();
					}
				}
				else
				{
					s += *src;
				}
				src++;
			}
			if(!s.IsEmpty()) Add(s);
		}
		else
		{
			CDuiString text(src);
			int pos = text.Find(sp, 0);
			while (pos >= 0)
			{
				CDuiString t = text.Left(pos);
				if(!t.IsEmpty()) Add(t);
				text = text.Right(text.GetLength() - pos - _tcslen(sp));
				pos = text.Find(sp);
			}
			if(!text.IsEmpty()) Add(text);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	CDuiWaitCursor::CDuiWaitCursor()
	{
		m_hOrigCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	}

	CDuiWaitCursor::~CDuiWaitCursor()
	{
		::SetCursor(m_hOrigCursor);
	}

} // namespace DuiLib