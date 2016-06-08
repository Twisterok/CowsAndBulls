#ifndef _PM_UTILS_H_
#define _PM_UTILS_H_
#include "DllData.h"
#include <string>
#include <vector>
#include <utils/definitions.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#define PTYPE_STRING  1
#define PTYPE_INTEGER 2
#define PTYPE_BINARY  4
#define PTYPE_BUFFER  8

using namespace std;


#define PCOM_OUTPUT(PCOM_str, s, a) \
  sprintf_s(m_Msg, sizeof(m_Msg), s, a); \
  PCOM_AddOutputLine(PCOM_str, m_Msg);

#define CHECK_PARAM(PCOM_str, param, s, t) \
  PCOM_ASSERT(PCOM_str, ((((param).size != s) && (s != 0)) || (((param).type & t) == 0)), "Wrong type of parameters", 0);

#define CHECK_AND_GET_PARAM(PCOM_str, param, s, t, par) {\
  CHECK_PARAM(PCOM_str, param, s, t); \
  PCOM_GetParamValue(par, param); }
typedef struct tsSV
{
  WORD size;
  BYTE number;
  BYTE *value;
} sSV;

typedef struct tsParameter
{
  int type;
  union
  {
    char *valStr;
    int valInt;
  };
  int size;
  PCOM32BUFFER *buf;
} sParameter;


int				PCOM_ProcessLine(char *line, int lineLen, sParameter *params, int *numParams, DLLDATA *DD);
int				PCOM_GetParamValue(PCOM32BUFFER *buf, sParameter param);
int				PCOM_ShowInput(DLLDATA *DD, sParameter *params, int numParams);
int				PCOM_AddOutputLine(DLLDATA *DD, string fstr);
int				PCOM_ShowOutput(DLLDATA *DD, sSV *SV, int SVNum);
int				PCOM_SetOutputToBuffers(DLLDATA *DD, sSV *SV, int SVNum);
int				PCOM_DefineVariable(DLLDATA *DD, char *varName, char *varValue);
WORD			FillArray (BYTE *bBuffer, char *cData, WORD len);
PCOM32BUFFER	*GetPCOMBuffer(DLLDATA *DD, char Buf);
PCOM32BUFFER	*GetPCOMBufferByIndex(DLLDATA *DD, int Buf);
int				PCOM_HexToBin(BYTE *binData, char *hexData, int len);
int				PCOM_BinToHex(char *hexData, BYTE *binData, int len);
int				PCOM_FreeParams(sParameter *params);
bool			GetPath(char* fullPath, char* result);
void			PCOM_Assert(bool expr, unsigned int errcode, string description);
void			PCOM_CheckParam(sParameter param,int size, int type);
void			PCOM_SendMessage(DLLDATA* dlldata, string message);

#endif
