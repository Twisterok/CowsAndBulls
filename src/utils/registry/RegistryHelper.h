#ifndef __REGISTRY_HELPER_H__
#define __REGISTRY_HELPER_H__

#include <string>
#include <vector>
#include <Windows.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

class RegistryHelper
{
    HKEY    hKey;

    void closeKey();
public:

    RegistryHelper()    : hKey(NULL)    {}
    ~RegistryHelper();
    void openKey(HKEY rootKey, std::string NodeName);   // if key doesn't exist - error
    void openKeyEx(HKEY rootKey, std::string NodeName); // if key doesn't exist - create

    void getNodeSubkeys(std::vector<std::string>& subkeys);
    void getKeyValue(std::string Key, std::string& data);
    void getKeyValueOptional(std::string Key, std::string& data);   //string equal to std::vector<char>
    void setValue(std::string Key, int keyType, const std::string& data);
};

#endif
