//////////////////////////////////////////////////////////////////////////
// Declare of CRegKeyEx class

#pragma once

#include <atlbase.h>
namespace MyREG {


	//////////////////////////////////////////////////////////////////////////
	// CRegKeyEx
	class CRegKeyEx
	{
	public:
		CRegKeyEx() throw();
		CRegKeyEx(CRegKeyEx& key) throw();
		explicit CRegKeyEx(HKEY hKey) throw();
		~CRegKeyEx() throw();

		CRegKeyEx& operator =(CRegKeyEx& key);

		operator HKEY() const throw();

		// Operations
		LONG SetDWORDValueA(LPCSTR pszValueName, DWORD dwValue) throw();
		LONG SetStringValueA(LPCSTR pszValueName, LPCSTR pszValue, DWORD dwType = REG_SZ) throw();
		LONG SetStringValueW(LPCWSTR pszValueName, LPCWSTR pszValue, DWORD dwType = REG_SZ) throw();

		LONG QueryDWORDValueA(__in_z_opt LPCSTR pszValueName, DWORD& dwValue) throw();
		LONG QueryStringValueA(__in_z_opt LPCSTR pszValueName,
			__out_ecount_part_z_opt(*pnChars, *pnChars) __out_z LPSTR pszValue, __inout ULONG* pnChars) throw();
		LONG QueryStringValueW(__in_z_opt LPCWSTR pszValueName,
			__out_ecount_part_z_opt(*pnChars, *pnChars) __out_z LPWSTR pszValue, __inout ULONG* pnChars) throw();
		LONG QueryDataBlobA(__in_z_opt LPCSTR pszValueName, DATA_BLOB& dataBlob);

		// Get the key's security attributes.
		LONG GetKeySecurity(SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd, LPDWORD pnBytes) throw();
		// Set the key's security attributes.
		LONG SetKeySecurity(SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd) throw();

		// Create a new registry key (or open an existing one).
		LONG CreateA(__in HKEY hKeyParent, __in_z LPCSTR lpszKeyName,
			__in_z LPSTR lpszClass = REG_NONE, __in DWORD dwOptions = REG_OPTION_NON_VOLATILE,
			__in REGSAM samDesired = KEY_READ | KEY_WRITE,
			__in_opt LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
			__in_opt LPDWORD lpdwDisposition = NULL) throw();
		// Open an existing registry key.
		LONG OpenA(HKEY hKeyParent, LPCSTR lpszKeyName,
			REGSAM samDesired = KEY_READ | KEY_WRITE) throw();
		LONG OpenW(HKEY hKeyParent, LPCWSTR lpszKeyName,
			REGSAM samDesired = KEY_READ | KEY_WRITE) throw();

		// Close the registry key.
		LONG Close() throw();
		// Flush the key's data to disk.
		LONG Flush() throw();

		// Detach the CRegKeyEx object from its HKEY.  Releases ownership.
		HKEY Detach() throw();
		// Attach the CRegKeyEx object to an existing HKEY.  Takes ownership.
		void Attach(HKEY hKey) throw();

		// Enumerate the subkeys of the key.
		LONG EnumKeyA(__in DWORD iIndex, __out_ecount_part_z(*pnNameLength, *pnNameLength) LPSTR pszName,
			__inout LPDWORD pnNameLength, __out_opt FILETIME* pftLastWriteTime = NULL) throw();
		LONG NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent, BOOL bAsync = TRUE) throw();

		LONG DeleteSubKeyA(LPCSTR lpszSubKey) throw();
		LONG RecurseDeleteKeyA(LPCSTR lpszKey) throw();
		LONG DeleteValueA(LPCSTR lpszValue) throw();

	protected:
		HKEY m_hKey;

	};

	inline CRegKeyEx::CRegKeyEx() throw() : m_hKey(NULL)
	{
	}

	inline CRegKeyEx::CRegKeyEx(CRegKeyEx& key) throw() : m_hKey(NULL)
	{
		Attach(key.Detach());
	}

	inline CRegKeyEx::CRegKeyEx(HKEY hKey) throw() : m_hKey(hKey)
	{
	}

	inline CRegKeyEx::~CRegKeyEx() throw()
	{
		Close();
	}

	inline CRegKeyEx& CRegKeyEx::operator =(CRegKeyEx& key) throw()
	{
		if (m_hKey != key.m_hKey)
		{
			Close();
			Attach(key.m_hKey);
		}

		return *this;
	}

	inline CRegKeyEx::operator HKEY() const throw()
	{
		return m_hKey;
	}

	inline LONG CRegKeyEx::SetDWORDValueA(LPCSTR pszValueName, DWORD dwValue) throw()
	{
		ATLASSUME(m_hKey != NULL);
		return ::RegSetValueExA(m_hKey, pszValueName, NULL, REG_DWORD, reinterpret_cast<const BYTE*>(&dwValue), sizeof(DWORD));
	}

	inline LONG CRegKeyEx::SetStringValueA(LPCSTR pszValueName, LPCSTR pszValue, DWORD dwType /* = REG_SZ */) throw()
	{
		ATLASSUME(m_hKey != NULL);
		ATLENSURE(pszValue != NULL);
		ATLASSERT((dwType == REG_SZ) || (dwType == REG_EXPAND_SZ));
		return ::RegSetValueExA(m_hKey, pszValueName, NULL, dwType, reinterpret_cast<const BYTE*>(pszValue), static_cast<DWORD>((strlen(pszValue) + 1) * sizeof(CHAR)));
	}

	inline LONG CRegKeyEx::SetStringValueW(LPCWSTR pszValueName, LPCWSTR pszValue, DWORD dwType) throw()
	{
		ATLASSUME(m_hKey != NULL);
		ATLENSURE(pszValue != NULL);
		ATLASSERT((dwType == REG_SZ) || (dwType == REG_EXPAND_SZ));
		return ::RegSetValueExW(m_hKey, pszValueName, NULL, dwType, reinterpret_cast<const BYTE*>(pszValue), static_cast<DWORD>((wcslen(pszValue) + 1) * sizeof(WCHAR)));
	}

	inline LONG CRegKeyEx::QueryDWORDValueA(LPCSTR pszValueName, DWORD& dwValue) throw()
	{
		LONG lRes;
		ULONG nBytes;
		DWORD dwType;

		ATLASSUME(m_hKey != NULL);

		nBytes = sizeof(DWORD);
		lRes = ::RegQueryValueExA(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(&dwValue),
			&nBytes);
		if (lRes != ERROR_SUCCESS)
			return lRes;
		if (dwType != REG_DWORD)
			return ERROR_INVALID_DATA;

		return ERROR_SUCCESS;
	}

	inline LONG CRegKeyEx::QueryStringValueA(LPCSTR pszValueName, LPSTR pszValue, ULONG* pnChars) throw()
	{
		LONG lRes;
		DWORD dwType;
		ULONG nBytes;

		ATLASSUME(m_hKey != NULL);
		ATLASSERT(pnChars != NULL);

		nBytes = (*pnChars) * sizeof(CHAR);
		*pnChars = 0;
		lRes = ::RegQueryValueExA(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(pszValue), &nBytes);

		if (lRes != ERROR_SUCCESS)
		{
			return lRes;
		}

		if (dwType != REG_SZ && dwType != REG_EXPAND_SZ)
		{
			return ERROR_INVALID_DATA;
		}

		if (pszValue != NULL)
		{
			if (nBytes != 0)
			{
				if ((nBytes % sizeof(CHAR) != 0) || (pszValue[nBytes / sizeof(CHAR) - 1] != 0))
				{
					return ERROR_INVALID_DATA;
				}
			}
			else
			{
				pszValue[0] = '\0';
			}
		}

		*pnChars = nBytes / sizeof(CHAR);

		return ERROR_SUCCESS;
	}

	inline LONG CRegKeyEx::GetKeySecurity(SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd, LPDWORD pnBytes) throw()
	{
		ATLASSUME(m_hKey != NULL);
		ATLASSERT(pnBytes != NULL);

		return ::RegGetKeySecurity(m_hKey, si, psd, pnBytes);
	}

	inline LONG CRegKeyEx::SetKeySecurity(SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd) throw()
	{
		ATLASSUME(m_hKey != NULL);
		ATLASSERT(psd != NULL);

		return ::RegSetKeySecurity(m_hKey, si, psd);
	}

	inline LONG CRegKeyEx::CreateA(HKEY hKeyParent, LPCSTR lpszKeyName,
		LPSTR lpszClass /* = REG_NONE */,
		DWORD dwOptions /* = REG_OPTION_NON_VOLATILE */,
		REGSAM samDesired /* = KEY_READ | KEY_WRITE */,
		LPSECURITY_ATTRIBUTES lpSecAttr /* = NULL */,
		LPDWORD lpdwDisposition /* = NULL */) throw()
	{
		ATLASSERT(hKeyParent != NULL);
		DWORD dw;
		HKEY hKey = NULL;
		LONG lRes = RegCreateKeyExA(hKeyParent, lpszKeyName, 0,
			lpszClass, dwOptions, samDesired, lpSecAttr, &hKey, &dw);
		if (lpdwDisposition != NULL)
			*lpdwDisposition = dw;
		if (lRes == ERROR_SUCCESS)
		{
			lRes = Close();
			m_hKey = hKey;
		}

		return lRes;
	}

	inline LONG CRegKeyEx::OpenA(HKEY hKeyParent, LPCSTR lpszKeyName, REGSAM samDesired /* = KEY_READ | KEY_WRITE */) throw()
	{
		ATLASSERT(hKeyParent != NULL);
		HKEY hKey = NULL;
		LONG lRes = ::RegOpenKeyExA(hKeyParent, lpszKeyName, 0, samDesired, &hKey);
		if (lRes == ERROR_SUCCESS)
		{
			lRes = Close();
			ATLASSERT(lRes == ERROR_SUCCESS);
			m_hKey = hKey;
		}

		return lRes;
	}

	inline LONG CRegKeyEx::OpenW(HKEY hKeyParent, LPCWSTR lpszKeyName, REGSAM samDesired /* = KEY_READ | KEY_WRITE */) throw()
	{
		ATLASSERT(hKeyParent != NULL);
		HKEY hKey = NULL;
		LONG lRes = ::RegOpenKeyExW(hKeyParent, lpszKeyName, 0, samDesired, &hKey);
		if (lRes == ERROR_SUCCESS)
		{
			lRes = Close();
			ATLASSERT(lRes == ERROR_SUCCESS);
			m_hKey = hKey;
		}

		return lRes;
	}

	inline LONG CRegKeyEx::QueryStringValueW(LPCWSTR pszValueName, LPWSTR pszValue, ULONG* pnChars) throw()
	{
		LONG lRes;
		DWORD dwType;
		ULONG nBytes;

		ATLASSUME(m_hKey != NULL);
		ATLASSERT(pnChars != NULL);

		nBytes = (*pnChars) * sizeof(WCHAR);
		*pnChars = 0;
		lRes = ::RegQueryValueExW(m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(pszValue), &nBytes);

		if (lRes != ERROR_SUCCESS)
		{
			return lRes;
		}

		if (dwType != REG_SZ && dwType != REG_EXPAND_SZ)
		{
			return ERROR_INVALID_DATA;
		}

		if (pszValue != NULL)
		{
			if (nBytes != 0)
			{
				if ((nBytes % sizeof(WCHAR) != 0) || (pszValue[nBytes / sizeof(WCHAR) - 1] != 0))
				{
					return ERROR_INVALID_DATA;
				}
			}
			else
			{
				pszValue[0] = '\0';
			}
		}

		*pnChars = nBytes / sizeof(WCHAR);

		return ERROR_SUCCESS;
	}

	inline LONG CRegKeyEx::QueryDataBlobA(__in_z_opt LPCSTR pszValueName, DATA_BLOB& dataBlob)
	{
		ATLASSUME(m_hKey != NULL);
		ATLASSUME(dataBlob.pbData == NULL);

		LONG lRes;
		DWORD dwType;

		lRes = ::RegQueryValueExA(m_hKey, pszValueName, NULL, &dwType, dataBlob.pbData, &dataBlob.cbData);

		if (lRes != ERROR_SUCCESS)
		{
			return lRes;
		}

		if (dwType != REG_BINARY)
		{
			return ERROR_INVALID_DATA;
		}

		if (dataBlob.cbData > 0)
		{
			dataBlob.pbData = new BYTE[dataBlob.cbData];

			lRes = ::RegQueryValueExA(m_hKey, pszValueName, NULL, &dwType, dataBlob.pbData, &dataBlob.cbData);

			return lRes;
		}

		return ERROR_SUCCESS;
	}

	inline LONG CRegKeyEx::Close() throw()
	{
		LONG lRes = ERROR_SUCCESS;
		if (m_hKey != NULL)
		{
			lRes = ::RegCloseKey(m_hKey);
			m_hKey = NULL;
		}

		return lRes;
	}

	inline LONG CRegKeyEx::Flush() throw()
	{
		ATLASSUME(m_hKey != NULL);
		return ::RegFlushKey(m_hKey);
	}

	inline HKEY CRegKeyEx::Detach() throw()
	{
		HKEY hKey = m_hKey;
		m_hKey = NULL;
		return hKey;
	}

	inline void CRegKeyEx::Attach(HKEY hKey) throw()
	{
		ATLASSUME(m_hKey != NULL);
		m_hKey = hKey;
	}

	inline LONG CRegKeyEx::EnumKeyA(DWORD iIndex, LPSTR pszName, LPDWORD pnNameLength, FILETIME* pftLastWriteTime) throw()
	{
		FILETIME ftLastWriteTime;

		ATLASSUME(m_hKey != NULL);
		if (pftLastWriteTime == NULL)
		{
			pftLastWriteTime = &ftLastWriteTime;
		}

		return ::RegEnumKeyExA(m_hKey, iIndex, pszName, pnNameLength, NULL, NULL, NULL, pftLastWriteTime);
	}

	inline LONG CRegKeyEx::NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent, BOOL bAsync /* = TRUE */) throw()
	{
		ATLASSUME(m_hKey != NULL);
		ATLASSERT((hEvent != NULL) || !bAsync);
		return ::RegNotifyChangeKeyValue(m_hKey, bWatchSubtree, dwNotifyFilter, hEvent, bAsync);
	}

	inline LONG CRegKeyEx::DeleteSubKeyA(LPCSTR lpszSubKey) throw()
	{
		ATLASSUME(m_hKey != NULL);
		return ::RegDeleteKeyA(m_hKey, lpszSubKey);
	}

	inline LONG CRegKeyEx::RecurseDeleteKeyA(LPCSTR lpszKey) throw()
	{
		CRegKeyEx key;
		LONG lRes = key.OpenA(m_hKey, lpszKey, KEY_READ | KEY_WRITE);
		if (lRes != ERROR_SUCCESS)
		{
			if (lRes != ERROR_FILE_NOT_FOUND && lRes != ERROR_PATH_NOT_FOUND)
			{
				ATLTRACE(atlTraceCOM, 0, _T("CRegKey::RecurseDeleteKey : Failed to Open Key %s(Error = %d)\n"), lpszKey, lRes);
			}
			return lRes;
		}

		FILETIME time;
		DWORD dwSize = 256;
		CHAR szBuffer[256];
		while (::RegEnumKeyExA(key.m_hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL, &time) == ERROR_SUCCESS)
		{
			lRes = key.RecurseDeleteKeyA(szBuffer);
			if (lRes != ERROR_SUCCESS)
				return lRes;
			dwSize = 256;
		}
		key.Close();

		return DeleteSubKeyA(lpszKey);
	}

	inline LONG CRegKeyEx::DeleteValueA(LPCSTR lpszValue) throw()
	{
		ATLASSUME(m_hKey != NULL);
		return RegDeleteValueA(m_hKey, (LPSTR)lpszValue);
	}

}