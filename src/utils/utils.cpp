//--------------CPP
#define _CRT_SECURE_NO_WARNINGS
#include <utils/utils.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <Windows.h>
#include <algorithm>
#include <sstream>
#include <utils/Exception.h>
#include <Wtsapi32.h>
#define SECURITY_WIN32
#include <Security.h>


namespace utils
{

    ODBCParameters	getODBCParameters(string	odbc)
    {
        HKEY			hKey;
        DWORD			size;
        string			registry_string;
        ODBCParameters	result;
        registry_string.resize(1024);

        try
        {
            registry_string = "SOFTWARE\\ODBC\\ODBC.INI\\" + odbc;
            if (RegOpenKeyA(HKEY_LOCAL_MACHINE, registry_string.c_str(), &hKey))	throw std::runtime_error("Node ["+registry_string+"] could not be located.");

            size = registry_string.size();
            if (!RegQueryValueExA(hKey, "SERVER", NULL, NULL, (LPBYTE)registry_string.data(), &size))	result.m_host		= registry_string;
            else																						throw std::runtime_error("Parameter SERVER could not be located");

            size = registry_string.size();
            if (!RegQueryValueExA(hKey, "PORT", NULL, NULL, (LPBYTE)registry_string.data(), &size))		result.m_port		= stoi(registry_string);
            else																						throw std::runtime_error("Parameter PORT could not be located");

            size = registry_string.size();
            if (!RegQueryValueExA(hKey, "DATABASE", NULL, NULL, (LPBYTE)registry_string.data(), &size))	result.m_database	= registry_string;
            else																						throw std::runtime_error("Parameter DATABASE could not be located");

            size = registry_string.size();
            if (!RegQueryValueExA(hKey, "UID", NULL, NULL, (LPBYTE)registry_string.data(), &size))		result.m_uid		= registry_string;
            else																						throw std::runtime_error("Parameter UID could not be located");

            size = registry_string.size();
            if (!RegQueryValueExA(hKey, "PWD", NULL, NULL, (LPBYTE)registry_string.data(), &size))		result.m_pwd		= registry_string;
            else																						throw std::runtime_error("Parameter PWD could not be located");

            CloseHandle(hKey);

            return result;
        }
        catch (std::exception& ex)
        {
            CloseHandle(hKey);
            throw std::runtime_error(string("Registry read error: ") + ex.what());
        }
    }

    bool   GetPath(const string& fullPath, string& result)
    {
        size_t found = fullPath.find_last_of("/\\");
        if (found == string::npos)	return false;

        result = fullPath.substr(0,found);
        return true;
    }

    int GetCurrentSessionUserName(string& UserNameOut)
    {
      DWORD SessionId;
      if (!ProcessIdToSessionId(GetCurrentProcessId(), &SessionId))	return -1;
      else
      {
        char *UserName;
        DWORD UserNameLen = 0;

        if (!WTSQuerySessionInformationA(WTS_CURRENT_SERVER_HANDLE, SessionId, WTSUserName, (LPSTR *)&UserName, &UserNameLen))	return -1;
        else
        {
            UserNameOut	= UserName;
            WTSFreeMemory(UserName);
        }
      }
      return 0;
    }


    void GetCurrentProcessUserName(string& UserName)
    {
      char s1[255], s2[255];
      DWORD rv;
      DWORD size1=sizeof(s1), size2=sizeof(s2);
      memset(s1, 0, sizeof(s1)); memset(s2, 0, sizeof(s2));

      GetUserNameA(s1, &size1);
      rv = GetUserNameExA(NameDisplay, s2, &size2);
      if (rv)	sprintf((char*) UserName.c_str(), "%s (%s)", s2, s1);
      else		sprintf((char*) UserName.c_str(), "%s", s1);
    }

    void	getCurrentUserName(string& UserName)
    {
      if (GetCurrentSessionUserName(UserName) == -1)
        GetCurrentProcessUserName(UserName);
    }


    std::string getFileVersion(std::string fileName)
    {
        string result       = "";
        DWORD  verHandle    = NULL;
        UINT   size         = 0;
        DWORD  verSize      = GetFileVersionInfoSizeA(fileName.c_str(), &verHandle);
        VS_FIXEDFILEINFO    *FixedFileInfo;
        if (verSize != NULL)
        {
            BYTE* verData = new BYTE[verSize];
            memset(verData, 0, verSize);

            if (verData)
            {
                if (GetFileVersionInfoA( fileName.c_str(), 0, verSize, verData))
                {
                    if (VerQueryValueA(verData,"\\",(void**)&FixedFileInfo,&size))
                    {
                        stringstream versionStr;
                        versionStr  << (FixedFileInfo->dwFileVersionMS >> 16) << "."
                                    << (FixedFileInfo->dwFileVersionMS & 0xffff) << "."
                                    << (FixedFileInfo->dwFileVersionLS >> 16) << "."
                                    << (FixedFileInfo->dwFileVersionLS & 0xffff);
                        result = versionStr.str();
                    }
                }
            }
            delete[] verData;
        }

        return result;
    }
}//namspace utils
