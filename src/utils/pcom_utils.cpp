#include <utils/pcom_utils.h>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sstream>
#include <iomanip>

#include <utils/ErrorCodes.h>
#include <utils/Exception.h>

#define MAX_DISPLAY_LEN 255*8
int OutputMode = 1;


typedef struct {
  char name[0x44];
  long from;
  long to;
} vv;


//------------------------------------------------------
int PCOM_FreeParams(sParameter *params)
{
  //for(int i=0;i<maxParams;i++) if (params[i].type != 0) { delete params[i].valStr; params[i].type = 0; params[i].valStr = NULL; }
  return 0;
}


//------------------------------------------------------
int PCOM_ProcessLine(char *line, int lineLen, sParameter *params, int *numParams, DLLDATA *DD)
{
  int pos;
  int state;
  int maxParams = *numParams;
  int curParam=0;
  int i;
  unsigned char buf[256];
  char c;
  *numParams = 0;
  pos=0;
  state=0;
  memset(params, 0, sizeof(sParameter)*maxParams);
  while(pos < lineLen)
  {
    c = *line;
    if ((c >= 'a') && (c <= 'z')) c -= 0x20;
    switch(c)
    {
    case '\n': case '\r': *line=0; state=0; break;
    case '"': 
      switch(state)
      {
      case 0: state = 1;        // Ready to parameter
        params[curParam].valStr=(line+1); 
        params[curParam].type=PTYPE_STRING; 
        params[curParam].size=0; 
        break;
      case 1: state = 2; *line = 0; break; // String parameter
      case 2: return 3;         // String end
      }
      break;
    case ' ': case '\t': 
      switch(state)
      {
      case 0: state = 0; break; // Ready
      case 1: state = 1; params[curParam].size++; break; // String parameter
      case 2:case 4:case 5:
        state = 0; curParam++; *line = 0; break; // Next parameter
      }
      break;
    default:
      switch(state)
      {
      case 0: 
        if (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F')))
        {
          params[curParam].valStr=line; 
          params[curParam].type=PTYPE_BINARY; 
          params[curParam].size=1; 
          state = 4;
        } else if (((c >= 'G') && (c <= 'R'))||(c == 'W'))
        {
          params[curParam].valStr=line; 
          params[curParam].type=PTYPE_BUFFER; 
          params[curParam].size=1; 
          params[curParam].buf=GetPCOMBuffer(DD, c);
          if (params[curParam].buf == NULL) return 6;
          state = 2;
        } else return 6;
        break; // Ready
      case 1: state = 1; params[curParam].size++; break; // String
      case 4: state = 4; if (((c >= '0') && (c <= '9'))|| ((c >= 'A') && (c <= 'F'))) params[curParam].size++; else return 4; break; // String
      case 2: return 3;
      }
    }
    line++; pos++;
  }
  *numParams = curParam+((params[curParam].type != 0)?1:0);
  for(i=0;i<*numParams;i++) if (params[i].type == PTYPE_BINARY)
  {
    int j = PCOM_HexToBin(buf, params[i].valStr, 0);
    memcpy(params[i].valStr, buf, j);
    params[i].size = j;
  }
  return 0;
}


//------------------------------------------------------
int PCOM_GetParamValue(PCOM32BUFFER *buf, sParameter param)
{
  unsigned char buf2[256];
  int j;
  switch(param.type)
  {
  case PTYPE_BINARY:
    buf->pData = (unsigned char *)param.valStr;
    buf->nLength = param.size;
    return 0;
  case PTYPE_BUFFER:
    buf->pData = param.buf->pData;
    buf->nLength = param.buf->nLength;
    return 0;
  case PTYPE_INTEGER:
    j = PCOM_HexToBin(buf2, param.valStr, 0);
    memcpy(param.valStr, buf2, j);
    param.size = j;

    buf->pData = (unsigned char *)param.valStr;
    buf->nLength = param.size;
    return 0;
  case PTYPE_STRING:
    buf->pData = (unsigned char *)param.valStr;
    buf->nLength = (int)strlen(param.valStr);
    return 0;
  }
  return -1;
}


//------------------------------------------------------
int PCOM_ShowInput(DLLDATA *DD, sParameter *params, int numParams)
{
  if (!OutputMode) return 0;
  DD->strDisplayData.nLength += sprintf_s((char *)&DD->strDisplayData.pData[DD->strDisplayData.nLength], MAX_DISPLAY_LEN, "\n\tINPUT:\n");
  for(int i=0;i<numParams;i++)
    DD->strDisplayData.nLength += sprintf_s((char *)&DD->strDisplayData.pData[DD->strDisplayData.nLength], MAX_DISPLAY_LEN, "\t\tP%d: '%s'\n", i+1, params[i].valStr);
  return 0;
}


//------------------------------------------------------
int PCOM_AddOutputLine(DLLDATA *DD, string fstr)
{
  if (!OutputMode) return 0;
  DD->strDisplayData.nLength += sprintf_s((char *)&DD->strDisplayData.pData[DD->strDisplayData.nLength], 
	  MAX_DISPLAY_LEN, "%s", fstr.c_str());
  return 0;
}


//------------------------------------------------------
int PCOM_SetOutputToBuffers(DLLDATA *DD, sSV *SV, int SVNum)
{
  int k, i;
  PCOM32BUFFER *buf;
  k = 0;
  for(i=0x80;i<SVNum;i++) if (SV[i].value != NULL) {
    k++;
    buf = GetPCOMBufferByIndex(DD, k);
    if (buf==NULL) return 1;
    if (SV[i].size > 256) return 2;
    buf->nLength = SV[i].size;
    memcpy(buf->pData, SV[i].value, SV[i].size);
  }
  return 0;
}


//------------------------------------------------------
int PCOM_ShowOutput(DLLDATA *DD, sSV *SV, int SVNum)
{
  int j, l, i;
  if (!OutputMode) return 0;

  PCOM_AddOutputLine(DD, "\n\tOUTPUT:\n");
  j = DD->strDisplayData.nLength;
  for(i=0x80;i<SVNum;i++) if (SV[i].value != NULL) {
    j += sprintf_s((char *)&DD->strDisplayData.pData[j], MAX_DISPLAY_LEN, "\t\tP%i: ",i-0x80);
    j += sprintf_s((char *)&DD->strDisplayData.pData[j], MAX_DISPLAY_LEN, "%02X ",SV[i].size);
    for(l=0;l<SV[i].size;l++) j += sprintf_s((char *)&DD->strDisplayData.pData[j], MAX_DISPLAY_LEN, "%02X", SV[i].value[l]);
    j += sprintf_s((char *)&DD->strDisplayData.pData[j], MAX_DISPLAY_LEN, " ");
    for(l=0;l<SV[i].size;l++) j += sprintf_s((char *)&DD->strDisplayData.pData[j], MAX_DISPLAY_LEN, "%c", (SV[i].value[l]>=32?SV[i].value[l]:' '));
    j += sprintf_s((char *)&DD->strDisplayData.pData[j], MAX_DISPLAY_LEN, "\n");
  }
  DD->strDisplayData.nLength = j;
  return 0;
}


//------------------------------------------------------
PCOM32BUFFER *GetPCOMBuffer(DLLDATA *DD, char Buf)
{
  switch(Buf)
  {
  case 'G': return &(DD->strHexG);
  case 'H': return &(DD->strHexH);
  case 'I': return &(DD->strHexI);
  case 'J': return &(DD->strHexJ);
  case 'K': return &(DD->strHexK);
  case 'L': return &(DD->strHexL);
  case 'M': return &(DD->strHexM);
  case 'N': return &(DD->strHexN);
  case 'O': return &(DD->strHexO);
  case 'P': return &(DD->strHexP);
  case 'Q': return &(DD->strHexP1);
  case 'R': return &(DD->strHexR);
  case 'W': return &(DD->strHexSW);
  default: return NULL;
  }
}

//------------------------------------------------------
PCOM32BUFFER *GetPCOMBufferByIndex(DLLDATA *DD, int Buf)
{
  switch(Buf)
  {
  case 1: return &(DD->strHexG);
  case 2: return &(DD->strHexH);
  case 3: return &(DD->strHexI);
  case 4: return &(DD->strHexJ);
  case 5: return &(DD->strHexK);
  case 6: return &(DD->strHexL);
  case 7: return &(DD->strHexM);
  case 8: return &(DD->strHexN);
  case 9: return &(DD->strHexO);
  case 10: return &(DD->strHexP);
  case 11: return &(DD->strHexR);
  default: return NULL;
  }
}


//------------------------------------------------------
int PCOM_BinToHex(char *hexData, BYTE *binData, int len)
//int BufToHex(BYTE *buf, BYTE *str, int len)
{
	int i;
	unsigned char c;
	for(i=0;i<len;i++) {
		c = *binData++;
		*hexData++ = (c >> 4) + ((c >> 4)>9?'A'-10:'0');
		*hexData++ = (c &0x0f) + ((c & 0x0f)>9?'A'-10:'0');
	}
	*hexData++ = 0;
	return len*2;
}


//------------------------------------------------------
int PCOM_HexToBin(BYTE *binData, char *hexData, int len)
//WORD BSC_FillArray (BYTE *bBuffer, char *cData, WORD len)
{
 WORD i;
 BYTE R,j,k,l;
 //int len;

 if (len == 0) len = (int)strlen(hexData);
 k=0; R=0; l=0;

 for(i=0;i<len;i++)
 {
   j = hexData[i];
   if ((j >= '0') && (j <= '9')) j -= '0'; else
   if ((j >= 'A') && (j <= 'F')) j -= 'A'-10; else 
   if ((j >= 'a') && (j <= 'f')) j -= 'a'-10; else return -1; 
   R<<=4;
   R|=j;
   if(l%2) binData[k++]=R;
   l++;
 }
 if(l%2) binData[k++]=R&0x0f;
 return k;
}

//-----------------------------------------------------
void PCOM_Assert(bool expr, unsigned int errcode, string description)
{
	if (expr)	throw Exception(errcode,description);
}


//-----------------------------------------------------
void PCOM_CheckParam(sParameter param,int size, int type)
{
	PCOM_Assert(  (param.size != size && size != 0) || ((param.type & type) == 0),
			errors::ER_PARAM_INVALID,
			"Wrong type of parameters");
}


//-----------------------------------------------------
void PCOM_SendMessage(DLLDATA* dlldata, string message)
{
	PCOM_AddOutputLine(dlldata, "\n\t\t\t*******************************************\n\n\t\t\t\t");
	PCOM_AddOutputLine(dlldata, message.c_str());
	PCOM_AddOutputLine(dlldata, "\n\n\t\t\t*******************************************\n");
}



