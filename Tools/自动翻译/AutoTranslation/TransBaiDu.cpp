#include "StdAfx.h"
#include "TransBaiDu.h"


CTransBaiDu::CTransBaiDu(void)
{
	curl = curl_easy_init();
}

CTransBaiDu::~CTransBaiDu(void)
{
	/* always cleanup */ 
	curl_easy_cleanup(curl);
}

static std::string g_strResponse;
static size_t callback_get_head(void *buffer, size_t size, size_t nmemb, void *userp) 
{ 
	std::string* str = dynamic_cast<std::string*>((std::string *)userp);  
	if( NULL == str || NULL == buffer )  
	{  
		return -1;  
	}  

	char* pData = (char*)buffer;  
	str->append(pData, size * nmemb);  
	return nmemb;  
}

BOOL CTransBaiDu::Translate(LPCTSTR sSource, CString &sDest)
{
	UISTRING_CONVERSION;
	CString src = sSource;
	CString temp;
	for (int i=0; i<src.GetLength(); i++)
	{
		TCHAR ch = src[i];

		//扫描一次有没有 %s %d %f 等等格式化字符串的东西，不加入翻译
		if(ch == _T('%'))
		{
			CString temp1;
			temp1 += ch;
			if(i < src.GetLength()-1)
			{
				if(src[i+1] == _T('d') || src[i+1] == _T('s') || src[i+1] == _T('f'))
				{
					temp1 += src[i+1];
					i++;
				}
			}

			while(i < src.GetLength()-1)
			{
				if(src[i+1] == _T(' ') || src[i+1] == _T('.') || src[i+1] == _T(',') || 
					src[i+1] == _T('。') || src[i+1] == _T('，'))
				{
					temp1 += src[i+1];
					i++;
				}
				else
					break;
			}

			CString sTempDest;
			__Translate(temp, sTempDest);
			sDest += sTempDest;
			temp.Empty();

			sDest += temp1;
			continue;
		}

		temp += ch;
	}

	CString sTempDest;
	__Translate(temp, sTempDest);
	sDest += sTempDest;
	temp.Empty();

	return TRUE;
}

BOOL CTransBaiDu::__Translate(LPCTSTR sSource, CString &sDest)
{
	BOOL bRet = FALSE;

	if(sSource==NULL || _tcslen(sSource)==0)
		return bRet;

	CMsgWndUI::InsertMsgV(_T("开始翻译：%s"), sSource);

	UISTRING_CONVERSION;
	CStringA sSourceUtf8 = UIT2UTF8(sSource);

	//urlencode
	CStringA sSourceUtf8_urlencode;
	for (int i=0; i<sSourceUtf8.GetLength(); i++)
	{
		char ch = sSourceUtf8.GetAt(i);
		if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <='Z') || (ch >= '0' && ch <= '9'))
			sSourceUtf8_urlencode += ch;
		else if(ch == '-' || ch == '_' || ch == '~' || ch == '.')
			sSourceUtf8_urlencode += ch;
		else
			sSourceUtf8_urlencode.AppendFormat("%%%02X", (BYTE)ch);
	}

	CURLcode res;
	char myurl[1000] = "http://api.fanyi.baidu.com/api/trans/vip/translate?";
	const char *appid = (const char *)m_appid;
	const char *q = (const char *)sSourceUtf8;
	char salt[60];
	int a = rand();
	sprintf(salt,"%d",a);
	const char *secret_key = (const char *)m_key;
	char sign[1024 * 10] = {0};
	strcat(sign,appid);
	strcat(sign,q);
	strcat(sign,salt);
	strcat(sign,secret_key);
	printf("%s\n",sign);
	unsigned char md[16];
	int i;
	char tmp[3]={'\0'},buf[33]={'\0'};
	MD5((unsigned char *)sign,strlen((const char *)sign),md);
	for (i = 0; i < 16; i++){
		sprintf(tmp,"%2.2x",md[i]);
		strcat(buf,tmp);
	}
	printf("%s\n",buf);
	strcat(myurl,"appid=");
	strcat(myurl,appid);
	strcat(myurl,"&q=");
	strcat(myurl, sSourceUtf8_urlencode);
	strcat(myurl,"&from=");
	strcat(myurl,m_from);
	strcat(myurl,"&to=");
	strcat(myurl,m_to);
	strcat(myurl,"&salt=");
	strcat(myurl,salt);
	strcat(myurl,"&sign=");
	strcat(myurl,buf);
	printf("%s\n",myurl);


	//设置访问的地址
	curl_easy_setopt(curl, CURLOPT_URL, myurl);


	//设置下载数据的回调函数 
	g_strResponse.clear();
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_get_head); 
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&g_strResponse);

	CMsgWndUI::InsertMsgV(_T("Send: %s"), UIUTF82T(myurl));

	res = curl_easy_perform(curl);
	/* Check for errors */ 
	if(res != CURLE_OK)
	{
		CMsgWndUI::InsertMsgV(_T("curl_easy_perform() failed: %s"), UIA2T(curl_easy_strerror(res)));
	}
	else
	{
		Json::Value va;
		Json::Reader reader;
		if(reader.parse(g_strResponse, va, true))
		{
			if(va.isMember("trans_result"))
			{
				Json::Value va_trans_result = va["trans_result"];
				Json::Value va_dst = va_trans_result[UINT(0)];
				const char *dst = va_dst["dst"].asCString();
				sDest = UIUTF82T(dst);

				//最后面的.可能被省略了.
				if(sSourceUtf8.GetAt(sSourceUtf8.GetLength() - 1) == '.')
					sDest += _T(".");
				CMsgWndUI::InsertMsgV(_T("翻译结果：%s"), sDest);
				bRet = TRUE;
			}
			if(va.isMember("error_code"))
			{
				CMsgWndUI::InsertMsgV(_T("翻译错误：%s"), UIA2T(g_strResponse.c_str()));
			}
		}
	}
		
	return bRet;
}