#include <utils/registry/RegistryHelper.h>
#include <utils/ErrorCodes.h>
#include <utils/Exception.h>

RegistryHelper::~RegistryHelper()
{
    closeKey();
}

void RegistryHelper::closeKey()
{
    if (hKey)   CloseHandle(hKey);
}

void RegistryHelper::openKey(HKEY rootKey, std::string NodeName)
{
    closeKey();

    unsigned int ErrorCode	=	RegOpenKeyA(rootKey, NodeName.c_str() , &hKey);
    if (ErrorCode)	throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, "Error reading " + NodeName + ". No such node");
}


void RegistryHelper::openKeyEx(HKEY rootKey, std::string NodeName)
{
    /*  if key doesn't exist - create the key   */
    closeKey();

    unsigned int ErrorCode	=	RegOpenKeyA(rootKey, NodeName.c_str() , &hKey);
    //RegCreateKeyEx
    if (    ErrorCode == ERROR_NO_MATCH ||
            ErrorCode == ERROR_FILE_NOT_FOUND)
    {
        ErrorCode = RegCreateKeyA(rootKey,NodeName.c_str(), &hKey);
    }

    if (ErrorCode)	throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, "Error reading " + NodeName + ". No such node");
}

void    RegistryHelper::getNodeSubkeys(std::vector<std::string>& subkeys)
{
    unsigned int    ErrorCode;

    std::string     achKey(MAX_KEY_LENGTH,0);   // buffer for subkey name
    DWORD           cbName;                     // size of name string
    std::string     achClass(MAX_PATH,0);       // buffer for class name
    DWORD           cchClassName = MAX_PATH;    // size of class string
    DWORD           cSubKeys=0;                 // number of subkeys
    DWORD           cbMaxSubKey;                // longest subkey size
    DWORD           cchMaxClass;                // longest class string
    DWORD           cValues;                    // number of values for key
    DWORD           cchMaxValue;                // longest value name
    DWORD           cbMaxValueData;             // longest value data
    DWORD           cbSecurityDescriptor;       // size of security descriptor
    FILETIME        ftLastWriteTime;            // last write time



    std::string achValue(MAX_VALUE_NAME,0);
    //DWORD       cchValue = MAX_VALUE_NAME;

    subkeys.clear();

    if (!hKey)  throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, "Key is not set");

    ErrorCode = RegQueryInfoKeyA(
    hKey,                    // key handle
    const_cast<char*>(achClass.data()),                // buffer for class name
    &cchClassName,           // size of class string
    NULL,                    // reserved
    &cSubKeys,               // number of subkeys
    &cbMaxSubKey,            // longest subkey size
    &cchMaxClass,            // longest class string
    &cValues,                // number of values for this key
    &cchMaxValue,            // longest value name
    &cbMaxValueData,         // longest value data
    &cbSecurityDescriptor,   // security descriptor
    &ftLastWriteTime);       // last write time


    if (cSubKeys)
    {
        for (size_t i=0; i<cSubKeys; i++)
        {
            cbName = MAX_KEY_LENGTH;
            ErrorCode = RegEnumKeyExA(hKey, i,
                    const_cast<char*>(achKey.data()),
                    &cbName,
                    NULL,
                    NULL,
                    NULL,
                    &ftLastWriteTime);

            subkeys.push_back(achKey.substr(0,cbName));
        }
    }
}

void    RegistryHelper::getKeyValue(std::string Key, std::string& data)
{
    DWORD				size;
    string				registry_string;
    unsigned int		ErrorCode;
    registry_string.resize(1024,0);

    if (!hKey)  throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, "Key is not set");

    size = registry_string.size();
    ErrorCode	=	RegQueryValueExA(hKey,Key.c_str(),NULL,NULL,(LPBYTE)registry_string.data(),&size);
    if (!ErrorCode)	data = registry_string;
    else            throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, "Error getting ["+Key+"]");

    data.resize(size);
}

void RegistryHelper::getKeyValueOptional(std::string Key, std::string& data)
{
    if (!hKey)  throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, "Key is not set");
    try
    {
        getKeyValue(Key,data);
    }
    catch (Exception&)   {}
}

void    RegistryHelper::setValue(std::string Key, int keyType, const std::string& inpData)
{
    unsigned int		ErrorCode;

    if (!hKey)  throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, "Key is not set");

    ErrorCode = RegSetValueExA(hKey,Key.c_str(),0,keyType,(BYTE*) inpData.data(),inpData.size());
    if (ErrorCode) throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, "Set "+Key+" error");
}
