// 18.03.2013 CrcCrypto.c                    

// *********************************
// КОНТРОЛЬНЫЕ СУММЫ и КРИПТОФУНКЦИИ
// *********************************
// Л.В.Быстров
// (С) 2009-2013 УДОБНЫЙ МАРШРУТ                         
// *********************************


#include "WinANSI.h"
#include "stdafx.h"
#include "CrcCrypto.h"


// ==================================================================
// КОНТРОЛЬНЫЕ СУММЫ BCC8, CRC8 и CRC16, а также вычисление кода LUHN
// ==================================================================


                                // =====================================================================
int Luhn(                       // РАСЧЕТ КОДА Luhn ЦИФРОВОЙ ПОСЛЕДОВАТЕЛЬНОСТИ (ISO/IEC 7812) 
                                // =====================================================================
                                // Возвращает 0 и код Luhn (ISO/IEC 7812) цифровой последовательности, 
                                // как символ цифры (например, '8') в последнем элементе массива ciphers
                                // или 1, если массив содержит хотя бы один нецифровой символ, либо
                                // длина цифровой последовательности лежит вне пределов, установленных
                                // стандартом ISO/IEC 7812
                                // =====================================================================
   unsigned short len,          // Длина цифровой последовательности (включая код Luhn),
                                //    согласно ISO/IEC 7812 len варьируется от 13 до 19 
   unsigned char *ciphers)      // Цифровая последовательность с цифрами в виде символов, 
                                //    например '9', '6', '4', '3', и т.д., для которой надо
                                //    рассчитать код Luhn; последний элемент массива на входе
                                //    должен быть зарезервирован под рассчитываемый код Luhn
                                // =====================================================================
                                //    КОММЕНТАРИИ:
                                //    О коде Luhn см.
                                //    http://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%9B%D1%83%D0%BD%D0%B0
                                //    http://apetrenko.com/algorithms/checkdigits/
                                //    http://apetrenko.com/algorithms/checkdigits/luhn.html
                                // =====================================================================
{
   int sum=0, i, j, k=2;
 
   if (len<13 || len>19) return(1);
   for (i=len-1; --i>=0;) 
   {
      if (ciphers[i]<0x30 || ciphers[i]>0x39) return(1);
      j=k*((int)(ciphers[i]-0x30));
      if (j>9) j=1+j-10*(j/10);
      sum=sum+j;
      if (k==2) k=1;
      else k=2;
   }
   k=10*((sum+10)/10);
   if ((sum+10-k)==0) j=0;
   else j=k-sum;
   ciphers[len-1]=(unsigned char)(j+0x30);
   return(0);
}  // ... Luhn(


                                // =========================================================
unsigned char BCC8(             // ВЫЧИСЛЕНИЕ КОНТРОЛЬНОЙ СУММЫ BCC8 МАССИВА БАЙТОВ
                                // BCC8 = bufer[0] XOR buffer[1] ... XOR buffer[length]
                                // =========================================================
   unsigned short  length,      // Длина массива (до 65535 байтов)
   unsigned char  *buffer)      // Массив байтов, для которого вычисляется контрольная сумма
                                // =========================================================
{
   unsigned char BCC=0;
   
   while (length--) BCC^=*buffer++;
   return BCC;
}  // ... BCC8(


                                // ===========================================
                                // ВЫЧИСЛЕНИЕ CRC8 (x^8 + x^4 + x^3 + x^2 + 1) 
                                // ===========================================

void add_crc(unsigned char ch, unsigned char *CRC); // Вспомогательная функция

                                // ==========================================================
unsigned char CRC8(             // ТАБЛИЧНОЕ ВЫЧИСЛЕНИЕ КОНТРОЛЬНОЙ СУММЫ CRC8 МАССИВА БАЙТОВ
                                // ==========================================================
   unsigned char  length,       // Длина массива (до 256 байтов!)
   unsigned char *buffer)       // Массив байтов, для которого вычисляется контрольная сумма
                                // ==========================================================
{
   unsigned char i;
   unsigned char CRC;                                         
   
   CRC=0xC7; // bit-swapped 0xE3
   for (i=0; i<length; i++) add_crc(buffer[i], &CRC);
   return CRC;
}  // ... CRC8(


unsigned char Values[]= // Таблица для вычисления CRC8 сообщения длиной до 256 байтов
{
   0x00,0x1D,0x3A,0x27,0x74,0x69,0x4E,0x53,0xE8,0xF5,0xD2,0xCF,0x9C,0x81,0xA6,0xBB,
   0xCD,0xD0,0xF7,0xEA,0xB9,0xA4,0x83,0x9E,0x25,0x38,0x1F,0x02,0x51,0x4C,0x6B,0x76,
   0x87,0x9A,0xBD,0xA0,0xF3,0xEE,0xC9,0xD4,0x6F,0x72,0x55,0x48,0x1B,0x06,0x21,0x3C,
   0x4A,0x57,0x70,0x6D,0x3E,0x23,0x04,0x19,0xA2,0xBF,0x98,0x85,0xD6,0xCB,0xEC,0xF1,
   0x13,0x0E,0x29,0x34,0x67,0x7A,0x5D,0x40,0xFB,0xE6,0xC1,0xDC,0x8F,0x92,0xB5,0xA8,
   0xDE,0xC3,0xE4,0xF9,0xAA,0xB7,0x90,0x8D,0x36,0x2B,0x0C,0x11,0x42,0x5F,0x78,0x65,
   0x94,0x89,0xAE,0xB3,0xE0,0xFD,0xDA,0xC7,0x7C,0x61,0x46,0x5B,0x08,0x15,0x32,0x2F,
   0x59,0x44,0x63,0x7E,0x2D,0x30,0x17,0x0A,0xB1,0xAC,0x8B,0x96,0xC5,0xD8,0xFF,0xE2,
   0x26,0x3B,0x1C,0x01,0x52,0x4F,0x68,0x75,0xCE,0xD3,0xF4,0xE9,0xBA,0xA7,0x80,0x9D,
   0xEB,0xF6,0xD1,0xCC,0x9F,0x82,0xA5,0xB8,0x03,0x1E,0x39,0x24,0x77,0x6A,0x4D,0x50,
   0xA1,0xBC,0x9B,0x86,0xD5,0xC8,0xEF,0xF2,0x49,0x54,0x73,0x6E,0x3D,0x20,0x07,0x1A,
   0x6C,0x71,0x56,0x4B,0x18,0x05,0x22,0x3F,0x84,0x99,0xBE,0xA3,0xF0,0xED,0xCA,0xD7,
   0x35,0x28,0x0F,0x12,0x41,0x5C,0x7B,0x66,0xDD,0xC0,0xE7,0xFA,0xA9,0xB4,0x93,0x8E,
   0xF8,0xE5,0xC2,0xDF,0x8C,0x91,0xB6,0xAB,0x10,0x0D,0x2A,0x37,0x64,0x79,0x5E,0x43,
   0xB2,0xAF,0x88,0x95,0xC6,0xDB,0xFC,0xE1,0x5A,0x47,0x60,0x7D,0x2E,0x33,0x14,0x09,
   0x7F,0x62,0x45,0x58,0x0B,0x16,0x31,0x2C,0x97,0x8A,0xAD,0xB0,0xE3,0xFE,0xD9,0xC4
};

void add_crc( // Add byte to CRC8 (вспомогательная)
   unsigned char  ch, 
   unsigned char *CRC)
{
   *CRC=Values[ch^*CRC];
   return;
}  // ... add_crc(



#define CRC_POLYNOM 0x1021
#define CRC_PRESET  0xFFFF
                                 // ==========================================================
                                 // ВЫЧИСЛЕНИЕ КОНТРОЛЬНОЙ СУММЫ CRC16 (x^16 + x^12 + x^5 + 1) 
void CRC16(                      // ==========================================================
   short               len,      // Длина массива
   const unsigned char buffer[], // Массив байтов, для которого вычисляется контрольная сумма
   unsigned char      *crc16)    // Контрольная сумма массива
                                 // ==========================================================
{
   int   i, j;
   short crc;

   crc=CRC_PRESET;
   for (i=0; i<len; i++)
   {
      crc^=(unsigned int)buffer[i]<<8;
      for (j=0; j<8; j++)
      {
         if (crc&0x8000) crc=(crc<<1)^CRC_POLYNOM;
         else crc=(crc<<1);
      }
   }
   crc16[0]=(unsigned char)(crc>>8);
   crc16[1]=(unsigned char)crc;
   return;
}  // ... crc16calc(



// ============================================================================
// КРИПТОФУНКЦИИ - вычисление D.E.S. и Triple D.E.S. (ISO/IEC 18033-3), а также 
//                 варианта Triple D.E.S. с опцией Cipher Block Chaining (CBC),
//                 вычисление Message Authentication Code по алгоритмам:
//                 MAC ISO/IEC 9797-1:1999 Algorithms 1, 3 и 6 Padding method 2
// ============================================================================

#define ENCRYPT              0  // Шифровать
#define DECRYPT              1  // Расшифровывать
                                
#define DES3_2KEYS           0  // Длина ключа для Triple D.E.S. 128 bits (Двойной ключ, 16 байтов)
#define DES3_3KEYS           1  // Длина ключа для Triple D.E.S. 192 bits (Тройной ключ, 24 байта)



                                // ======================================================
void DES1(                      // БЛОЧНОЕ (по 8 байтов) ШИФРОВАНИЕ ПО АЛГОРИТМУ D.E.S.
                                //                                     ISO/IEC 18033-3
                                // Текст  см. ниже  -  БАЗОВЫЕ ФУНКЦИИ АЛГОРИТМА D.E.S.
                                // Определение приведено здесь для функции DES3...
                                // ======================================================
   unsigned char  option,       // Шифровать (ENCRYPT) или расшифровать (DECRYPT)
   unsigned char *key,          // Секретный ключ (8 байтов)
   unsigned char *tinp,         // Исходное массив (8 байтов)
   unsigned char *tout);        // Результат (зашифрованный или расшифрованный, 8 байтов)
                                // ======================================================



                                // ===========================================================
void DES3(                      // БЛОЧНОЕ (по 8 байтов) ШИФРОВАНИЕ ПО АЛГОРИТМУ Triple D.E.S.
                                //                                            ISO/IEC 18033-3
                                // ===========================================================
   unsigned char  CryptoMode,   // ENCRYPT - зашифровать, DECRYPT - расшифровать
   unsigned char  KeyMode,      // DES3_2KEYS - длина ключа 16 байтов, DES3_3KEYS - 24 байта
   unsigned char *Key,          // Ключ (длина определена константой KeyMode)
   unsigned char *InMessage,    // Входное сообщение (8 байтов)
   unsigned char *OutMessage)   // Выходное сообщение (8 байтов)
                                // ===========================================================
{
   unsigned char tmp[16];

   if (CryptoMode==ENCRYPT) // Шифруем
   {
      DES1(ENCRYPT, Key, InMessage, tmp);
      DES1(DECRYPT, Key+8, tmp, tmp+8);
      DES1(ENCRYPT, Key+((KeyMode&1)<<4), tmp+8, OutMessage);
   }
   else // Расшифровываем
   {
      DES1(DECRYPT, Key+((KeyMode&1)<<4), InMessage, tmp);
      DES1(ENCRYPT, Key+8, tmp, tmp+8);
      DES1(DECRYPT, Key, tmp+8, OutMessage);
   }
   return;
}  // ... void DES3(

                                // ====================================================================
unsigned char CBC_3DES(         // ШИФРОВАНИЕ ПО АЛГОРИТМУ Triple D.E.S. (ISO/IEC 18033-3) с опцией CBC
                                // ====================================================================
   unsigned char  CryptoMode,   // ENCRYPT - зашифровать, DECRYPT - расшифровать
   unsigned char  KeyMode,      // DES3_2KEYS - длина ключа 16 байтов, DES3_3KEYS - 24 байта
   unsigned char *Key,          // Ключ (длина определена константой KeyMode)
   unsigned short DataLen,      // Длина входного/выходного сообщения (должна быть кратна восьми!)
   unsigned char *Data)         // Входное/выходное сообщение (DataLen байтов)
                                // ====================================================================
                                // ВНИМАНИЕ: В функции нет никакого паддинга! 
                                //           Длина входного сообщения должна быть строго кратна восьми!
                                //           В противном случае функция возвращает 8
                                // ====================================================================
                                // Если входные массовы пустые, то возвращается 7
                                // Если все нормально, то возвращается 0
                                // ====================================================================
                                //    КОММЕНТАРИИ:
                                //    Режим сцепления блоков шифрованного текста (англ. Cipher Block Chaining, CBC) - один из вариантов 
                                //    использования симметричного блочного шифра с использованием механизма обратной связи
                                //    Каждый блок открытого текста (кроме первого) побитно складывается по модулю 2 (операция XOR) 
                                //    с предыдущим результатом шифрования 
                                //    В качестве криптографического алгоритма (процедуры шифрования/дешифрования) используется Triple DES
                                //    Ссылка:
                                //    http://ru.wikipedia.org/wiki/%D0%A0%D0%B5%D0%B6%D0%B8%D0%BC_%D1%81%D1%86%D0%B5%D0%BF%D0%BB%D0%B5%D0%BD%D0%B8%D1%8F_%D0%B1%D0%BB%D0%BE%D0%BA%D0%BE%D0%B2_%D1%88%D0%B8%D1%84%D1%80%D0%BE%D1%82%D0%B5%D0%BA%D1%81%D1%82%D0%B0
{
   unsigned short  i, j, k;
   unsigned char   DecryptData[8]={0}, EncryptData[8]={0}, InitVector[8]={0};
   
   /*
   printf("\n\n CBC_3DES  start: ");
   for (i=0; i<DataLen; i++) printf("%02X ", Data[i]);
   */

   if (Key==NULL || Data==NULL) return 7;                              // ERROR 7 - "Пустые" данные

   if (DataLen%8>0) return 8;                                          // ERROR 8 - Длина входного сообщения не кратна восьми

   k=DataLen/8;                                                        // Количество восьмибайтных блоков в массиве Data

   if (CryptoMode==ENCRYPT)                                            // Ш И Ф Р У Е М ...
   {
      memset(InitVector, 0x00, 8);                                     // Формируем начальный вектор
      for (j=0; j<k; j++)                                              // Шифруем DataLen блоков по 8 байтов каждый
      {
         for (i=0; i<8; i++) DecryptData[i]=InitVector[i]^Data[8*j+i]; // CBC Option
         DES3(ENCRYPT, KeyMode, Key, DecryptData, EncryptData);        // DES3 encryption
         memcpy(Data+8*j, EncryptData, 8);                             // Зашифрованные данные записываем в Data для последующего расшифрования
         memcpy(InitVector, EncryptData, 8);                           // Готовим новый вектор (зашифрованный текст) для XOR со следующим блоком исходных данных
      }
   }
   else                                                                // Р А С Ш И Ф Р О В Ы В А Е М ...
   {
      memset(InitVector, 0x00, 8);                                     // Формируем начальный вектор
      for (j=0; j<k; j++)                                              // Расшифровываем DataLen блоков по 8 байтов каждый
      {
         memcpy(EncryptData, Data+8*j, 8);                             // Помещаем в EncryptData блок зашифрованного текста
         DES3(DECRYPT, KeyMode, Key, EncryptData, DecryptData);        // DES3 decryption
         for (i=0; i<8; i++) Data[8*j+i]=InitVector[i]^DecryptData[i]; // CBC Option
         memcpy(InitVector, EncryptData, 8);                           // Готовим новый вектор для XOR со следующим зашифрованным блоком
      }
   }

   /*
   printf("  \n CBC_3DES finish: ");
   for (i=0; i<DataLen; i++) printf("%02X ", Data[i]);
   printf("\n");
   */

   return 0;
}  // ... unsigned char CBC_3DES(


                                // =====================================================
void MAC979712(                 // РАСЧЕТ MESSAGE AUTHENTICATION CODE (MAC) ПО СТАНДАРТУ
                                // ISO/IEC 9797-1:1999 Algorithm 1 Padding method 2
                                // =====================================================
   unsigned char  *Message,     // Сообщение, для которого расчитывается MAC
   unsigned short  Length,      // Длина этого сообщения (до 65535)
   unsigned char  *Key,         // Ключ длиной 8 байтов
   unsigned char   Truncating,  // Число байтов усечения справа MAC
   unsigned char  *MAC)         // 8 байтов MAC (усеченные байты равны 0х00)
                                // =====================================================
{
   unsigned short i, lb;
   unsigned char  j, tmp[8], pad[8]={0};

   memset(MAC, 0x00, 8);
   lb=Length>>3;
   memcpy(pad, Message+(lb<<3), Length&7); // padding always
   pad[Length&7]=0x80;

   for (i=0; i<lb; i++) 
   {
      for (j=0; j<8; j++) tmp[j]=MAC[j]^Message[(i<<3)+j];
      DES1(ENCRYPT, Key, tmp, MAC);
   }
   for (j=0; j<8; j++) tmp[j]=MAC[j]^pad[j];
   DES1(ENCRYPT, Key, tmp, MAC);

   memset(MAC+(8-Truncating), 0x00, Truncating);
   return;
}  // ...  MAC979712(                              


                                // =====================================================
void MAC979732(                 // РАСЧЕТ MESSAGE AUTHENTICATION CODE (MAC) ПО СТАНДАРТУ
                                // ISO/IEC 9797-1:1999 Algorithm 3 Padding method 2
                                // =====================================================
   unsigned char  *Message,     // Сообщение, для которого расчитывается MAC
   unsigned short  Length,      // Длина этого сообщения (до 65535)
   unsigned char  *Key,         // Ключ длиной 16 байтов
   unsigned char   Truncating,  // Число байтов усечения справа MAC
   unsigned char  *MAC)         // 8 байтов MAC (усеченные байты равны 0х00)
                                // =====================================================
{
   unsigned short i, lb;
   unsigned char  j, tmp[8], pad[8]={0};

   memset(MAC, 0x00, 8);
   lb=Length>>3;
   memcpy(pad, Message+(lb<<3), Length&7); // padding always
   pad[Length&7]=0x80;

   /*
   // Для отладки паддинга
   printf("\nLength=%u lb=%u Length&7=%u\nMessage+pad:", Length, lb, Length&7);
   for (i=0; i<lb; i++) for (j=0; j<8; j++) printf(" %02X", Message[(i<<3)+j]);
   for (i=0; i<8; i++) printf(" %02X", pad[i]);
   */

   for (i=0; i<lb; i++) 
   {
      for (j=0; j<8; j++) tmp[j]=MAC[j]^Message[(i<<3)+j];
      DES1(ENCRYPT, Key, tmp, MAC);
   }
   for (j=0; j<8; j++) tmp[j]=MAC[j]^pad[j];
   DES3(ENCRYPT, DES3_2KEYS, Key, tmp, MAC);

   memset(MAC+(8-Truncating), 0x00, Truncating);
   return;
}  // ...  MAC979732(                              


                                // =====================================================
void MAC979762(                 // РАСЧЕТ MESSAGE AUTHENTICATION CODE (MAC) ПО СТАНДАРТУ
                                // ISO/IEC 9797-1:1999 Algorithm 6 Padding method 2
                                // =====================================================
   unsigned char  *Message,     // Сообщение, для которого расчитывается MAC
   unsigned short  Length,      // Длина этого сообщения (до 65535)
   unsigned char  *Key,         // Ключ длиной 16 байтов
   unsigned char   Truncating,  // Число байтов усечения справа MAC
   unsigned char  *MAC)         // 8 байтов MAC (усеченные байты равны 0х00)
                                // =====================================================
{
   unsigned short i, lb;
   unsigned char  j, tmp[8], pad[8]={0};

   memset(MAC, 0x00, 8);
   lb=Length>>3;
   memcpy(pad, Message+(lb<<3), Length&7); // padding always
   pad[Length&7]=0x80;

   for (i=0; i<lb; i++) 
   {
      for (j=0; j<8; j++) tmp[j]=MAC[j]^Message[(i<<3)+j];
      DES3(ENCRYPT, DES3_2KEYS, Key, tmp, MAC);
   }
   for (j=0; j<8; j++) tmp[j]=MAC[j]^pad[j];
   DES3(ENCRYPT, DES3_2KEYS, Key, tmp, MAC);

   memset(MAC+(8-Truncating), 0x00, Truncating);
   return;
}  // ...  MAC979762(                              

 


// ================================
// БАЗОВЫЕ ФУНКЦИИ АЛГОРИТМА D.E.S.
// ================================


               // *****************************************
#ifdef DES_PC  // АЛГОРИТМ DES ДЛЯ ПЕРСОНАЛЬНОГО КОМПЬЮТЕРА
               // *****************************************
   
                                // ==============================================
   int CODING(                  // АЛГОРИТМ D.E.S. ДЛЯ ПК
                                // ==============================================
      int   option,             // Шифровать (ENCRYPT) или расшифровать (DECRYPT)
      char *key,                // Секретный ключ (8 байтов)
      char *tinp,               // Исходное сообщение
      char *tout,               // Результат (зашифрованный или расшифрованный)
      int   tlen);              // Длина сообщения
                                // ==============================================
   
   typedef struct {
     char row;
     char bit[2][64];
   } ST;

   
   int CODING_transp(ST *p,char *tab,int n);
   int CODING_rotenc(ST *k);
   int CODING_rotdec(ST *k);
   int CODING_f(int type,int n,ST *k,ST *t);
   
   
   #define ROW(x) ((ST *) x) -> row
   #define BIT(x) ((ST *) x) -> bit
   
   static char initTr[]=
   {
      58, 50, 42, 34, 26, 18, 10,  2, 60, 52, 44, 36, 28, 20, 12,  4,
      62, 54, 46, 38, 30, 22, 14,  6, 64, 56, 48, 40, 32, 24, 16,  8,
      57, 49, 41, 33, 25, 17,  9,  1, 59, 51, 43, 35, 27, 19, 11,  3,
      61, 53, 45, 37, 29, 21, 13,  5, 63, 55, 47, 39, 31, 23, 15,  7
   };
   
   static char finalTr[]=
   {
      40,  8, 48, 16, 56, 24, 64, 32, 39,  7, 47, 15, 55, 23, 63, 31,
      38,  6, 46, 14, 54, 22, 62, 30, 37,  5, 45, 13, 53, 21, 61, 29,
      36,  4, 44, 12, 52, 20, 60, 28, 35,  3, 43, 11, 51, 19, 59, 27,
      34,  2, 42, 10, 50, 18, 58, 26, 33,  1, 41,  9, 49, 17, 57, 25
   };
   
   static char swap[]=
   {
      33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
      49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
       1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
      17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
   };
   
   static char keyTr1[]=
   {
      57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
      10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
      63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
      14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
   };
   
   static char keyTr2[]=
   {
      14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
      23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
      41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
      44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
   };
   
   static char etr[]=
   {
      32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
       8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
      16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
      24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
   };
   
   static char ptr[]=
   {
      16,  7, 20, 21, 29, 12, 28, 17,  1, 15, 23, 26,  5, 18, 31, 10,
       2,  8, 24, 14, 32, 27,  3,  9, 19, 13, 30,  6, 22, 11,  4, 25
   };
   
   static char s[8][64]=
   {
      14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
       0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
       4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
      15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
   
      15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
       3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
       0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
      13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
   
      10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
      13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
      13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
       1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
   
       7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
      13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
      10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
       3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
   
       2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
      14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
       4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
      11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
   
      12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
      10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
       9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
       4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
   
       4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
      13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
       1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
       6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
   
      13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
       1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
       7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
       2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
   };
   
   static char rots[]=
   {
       1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1
   };
   
   
                                // ==============================================
   int CODING(                  // БАЗОВЫЙ АЛГОРИТМ D.E.S.
                                // ==============================================
      int   option,             // Шифровать (ENCRYPT) или расшифровать (DECRYPT)
      char *key,                // Секретный ключ (8 байтов)
      char *tinp,               // Исходное сообщение
      char *tout,               // Результат (зашифрованный или расшифрованный)
      int   tlen)               // Длина сообщения
                                // ==============================================
   {
      int   type;
      int   i,x;
      ST    kbit;
      ST    tbit;
      ST   *k=&kbit;
      ST   *t=&tbit;
   
      if (option==DECRYPT) type=1; else type=0;
      tlen&=~7;
      ROW(k)=0;
      ROW(t)=0;
      for (i=0;i<64;i++) {
         BIT(k)[0][i]=key[i>>3]>>7-(i&7)&1;
      }
      CODING_transp(k,keyTr1,56);        /* initial key transposition */
      for (tlen+=(int)tinp;(int)tinp<tlen;tinp+=8,tout+=8) {
         for (i=0;i<64;i++) {
            BIT(t)[ROW(t)][i]=tinp[i>>3]>>7-(i&7)&1;
         }
         CODING_transp(t,initTr,64);    /* initial text transposition */
         for (i=0;i<16;i++) {           /* here come the 16 iterations */
            CODING_transp(t,swap,64);   /* swap left right halves */
            CODING_f(type,i,k,t);       /* compute f(r[i-1],k[i]) */
            ROW(t)^=1;
            for (x=32;x<64;x++) BIT(t)[ROW(t)][x]^=BIT(t)[ROW(t)^1][x];
         }
         CODING_transp(t,swap,64);      /* swap left right halves */
         CODING_transp(t,finalTr,64);   /* final transposition */
         for (i=0;i<8;i++)
            for (tout[i]=0,x=0;x<8;x++)
               tout[i]|=BIT(t)[ROW(t)][(i<<3)+x]<<7-x;
      }
      return 0;
   }  // ... int CODING(
   
   
   int CODING_transp(ST *p,char *tab,int n)
   {
      while (--n>=0) BIT(p)[ROW(p)^1][n]=BIT(p)[ROW(p)][tab[n]-1];
      ROW(p)^=1;
      return 0;
   }  // ... int CODING_transp(
   
   
   int CODING_rotenc(ST *k)
   {
      int i;
   
      for (i=0;i<55;i++) BIT(k)[ROW(k)^1][i]=BIT(k)[ROW(k)][i+1];
      BIT(k)[ROW(k)^1][27]=BIT(k)[ROW(k)][0];
      BIT(k)[ROW(k)^1][55]=BIT(k)[ROW(k)][28];
      ROW(k)^=1;
      return 0;
   }  // ... int CODING_rotenc(
   
   
   int CODING_rotdec(ST *k)
   {
      int i;
   
      for (i=1;i<56;i++) BIT(k)[ROW(k)^1][i]=BIT(k)[ROW(k)][i-1];
      BIT(k)[ROW(k)^1][0]=BIT(k)[ROW(k)][27];
      BIT(k)[ROW(k)^1][28]=BIT(k)[ROW(k)][55];
      ROW(k)^=1;
      return 0;
   }  // ... int CODING_rotdec(
   
   
   int CODING_f(int type,int n,ST *k,ST *t)  /* f(r[i-1], k[i]) */
   {
      int i,x;
   
      CODING_transp(t,etr,48);         /* expand data block to 48 bits */
      if (!type) {
         for (i=rots[n];i>0;i--) CODING_rotenc(k);
      }
      CODING_transp(k,keyTr2,48);
      for (i=0;i<48;i++) BIT(t)[ROW(t)][i]^=BIT(k)[ROW(k)][i];
      ROW(k)^=1;
      if (type) {
         for (i=rots[15-n];i>0;i--) CODING_rotdec(k);
      }
      for (i=0;i<8;i++) {
         x=BIT(t)[ROW(t)][6*i]<<5;
         x|=BIT(t)[ROW(t)][6*i+5]<<4;
         for (n=1;n<5;n++) x|=BIT(t)[ROW(t)][6*i+n]<<4-n;
         for (n=0;n<4;n++) BIT(t)[ROW(t)][(i<<2)+n]=s[i][x]>>3-n&1;
      }
      for (i=0;i<32;i++) BIT(t)[ROW(t)][i+32]=BIT(t)[ROW(t)][ptr[i]-1];
      return 0;
   }  // ... int CODING_f(
   
   
      // **********************************************************************************
      // **********************************************************************************
      
                                // ========================================================
      void DES1(                // БАЗОВЫЙ АЛГОРИТМ D.E.S. ДЛЯ ПК (интерфейс ПК/Контроллер)
                                // ========================================================
         unsigned char  option, // Шифровать (ENCRYPT) или расшифровать (DECRYPT)
         unsigned char *key,    // Секретный ключ (8 байтов)
         unsigned char *tinp,   // Исходное сообщение (8 байтов)
         unsigned char *tout)   // Результат (зашифрованный или расшифрованныйб 8 байтов)
                                // ========================================================
      {
          CODING(option, key, tinp, tout, 8);
          return;
      }   // ... DES1 для ПК
      
      // **********************************************************************************
      // **********************************************************************************
   
   /*
   #endif // ... #ifdef DES_PC
   
   
     
      
                         // **************************************
   #ifdef DES_CONTROLLER // АЛГОРИТМ DES ДЛЯ КОНТРОЛЛЕРА (быстрый)
                         // **************************************
   */

#else

   #define EN0     0       /* MODE == encrypt */
   #define DE1     1       /* MODE == decrypt */
   
   typedef unsigned char byte;
   
   
    void deskey(byte key[8], short mode);
   /*
    * Sets the internal key register according to the hexadecimal
    * key contained in the 8 bytes of hexkey, according to the DES,
    * for encryption or decryption according to MODE.
    */
   
    void des(byte in[8], byte out[8]);
   /*
    * Encrypts/Decrypts (according to the key currently loaded in the
    * internal key register) one block of eight bytes from 'in'
    * into the block at address 'to'.  They can be the same.
    */
    void desNx8(byte * in, byte * out, int sz);
   
   
   #define ERR_CRYPT_LEN    (-11)       /* lenght crypt packet error        */
   #define ERR_CRYPT_CRC    (-12)       /* bad crc crypt packet             */
   /*************************************************************************
             Double & triple length DES algorithms functions:
    *************************************************************************/
   /* Validation sets:
    *
    * Single-length key, single-length plaintext -
    * Key    : 0123 4567 89ab cdef
    * Plain  : 0123 4567 89ab cde7
    * Cipher : c957 4425 6a5e d31d
    *
    * Double-length key, single-length plaintext -
    * Key    : 0123 4567 89ab cdef fedc ba98 7654 3210
    * Plain  : 0123 4567 89ab cde7
    * Cipher : 7f1d 0a77 826b 8aff
    *
    * Double-length key, double-length plaintext -
    * Key    : 0123 4567 89ab cdef fedc ba98 7654 3210
    * Plain  : 0123 4567 89ab cdef 0123 4567 89ab cdff
    * Cipher : 27a0 8440 406a df60 278f 47cf 42d6 15d7
    *
    * Triple-length key, single-length plaintext -
    * Key    : 0123 4567 89ab cdef fedc ba98 7654 3210 89ab cdef 0123 4567
    * Plain  : 0123 4567 89ab cde7
    * Cipher : de0b 7c06 ae5e 0ed5
    *
    * Triple-length key, double-length plaintext -
    * Key    : 0123 4567 89ab cdef fedc ba98 7654 3210 89ab cdef 0123 4567
    * Plain  : 0123 4567 89ab cdef 0123 4567 89ab cdff
    * Cipher : ad0d 1b30 ac17 cf07 0ed1 1c63 81e4 4de5
    *
    * d3des V5.0a rwo 9208.07 18:44 Graven Imagery
    **********************************************************************/
   
   typedef unsigned long dwrd;
   typedef unsigned int Uint;
   
   void usekey(register dwrd *from);
   
   static void  scrunch(byte *, dwrd *);
   static void  unscrun(dwrd *, byte *);
   static void  desfunc(dwrd *, dwrd *);
   static void  cookey(dwrd *);
   
   static dwrd  KnL[32] = { 0L };
   
   static unsigned short  bytebit[8] =  //octal:
                          { 0200, 0100, 040, 020, 010, 04, 02, 01 };
   
   static dwrd  bigbyte[24] =
                { 0x800000L, 0x400000L, 0x200000L, 0x100000L,
                  0x80000L,  0x40000L,  0x20000L,  0x10000L,
                  0x8000L,   0x4000L,   0x2000L,   0x1000L,
                  0x800L,    0x400L,    0x200L,    0x100L,
                  0x80L,     0x40L,     0x20L,     0x10L,
                  0x8L,      0x4L,      0x2L,      0x1L
                };
   
   /* Use the key schedule specified in the Standard (ANSI X3.92-1981). */
   
   static byte  pc1[56] =
          { 56, 48, 40, 32, 24, 16,  8,     0, 57, 49, 41, 33, 25, 17,
             9,  1, 58, 50, 42, 34, 26,    18, 10,  2, 59, 51, 43, 35,
            62, 54, 46, 38, 30, 22, 14,     6, 61, 53, 45, 37, 29, 21,
            13,  5, 60, 52, 44, 36, 28,    20, 12,  4, 27, 19, 11,  3
          };
   
   static byte  totrot[16] =
                { 1,2,4,6,8,10,12,14,15,17,19,21,23,25,27,28 };
   
   static byte pc2[48] = {
           13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9,
           22, 18, 11,  3, 25,  7, 15,  6, 26, 19, 12,  1,
           40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
           43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31 };
   
   void deskey(byte key[8], short edf) 
   {
     register int i, j, l, m, n;
     byte pc1m[56], pcr[56];
     dwrd kn[32];
   
     for(j = 0; j < 56; j++)
     {
       l = pc1[j];
       m = l & 07;
       pc1m[j] = (key[l >> 3] & bytebit[m]) ? 1 : 0;
     }
   
     for(i = 0; i < 16; i++)
     {
       if(edf == DE1)
         m = (15 - i) << 1;
       else
         m = i << 1;
   
       n = m + 1;
       kn[m] = kn[n] = 0L;
   
       for(j = 0; j < 28; j++)
       {
         l = j + totrot[i];
         if(l < 28)
           pcr[j] = pc1m[l];
         else
           pcr[j] = pc1m[l - 28];
       }
       for(j = 28; j < 56; j++)
       {
         l = j + totrot[i];
         if(l < 56)
           pcr[j] = pc1m[l];
         else
           pcr[j] = pc1m[l - 28];
       }
       for(j = 0; j < 24; j++)
       {
         if(pcr[pc2[j]])
           kn[m] |= bigbyte[j];
         if(pcr[pc2[j+24]])
           kn[n] |= bigbyte[j];
       }
     }
     cookey(kn);
     return;
   }
   
   static void cookey(register dwrd *raw1)
   {
     register dwrd *cook, *raw0;
     dwrd dough[32];
     register int i;
   
     cook = dough;
     for(i = 0; i < 16; i++, raw1++)
     {
       raw0 = raw1++;
       *cook    = (*raw0 & 0x00fc0000L) << 6;
       *cook   |= (*raw0 & 0x00000fc0L) << 10;
       *cook   |= (*raw1 & 0x00fc0000L) >> 10;
       *cook++ |= (*raw1 & 0x00000fc0L) >> 6;
       *cook    = (*raw0 & 0x0003f000L) << 12;
       *cook   |= (*raw0 & 0x0000003fL) << 16;
       *cook   |= (*raw1 & 0x0003f000L) >> 4;
       *cook++ |= (*raw1 & 0x0000003fL);
     }
     usekey(dough);
     return;
   }
   
   
   void usekey(register dwrd *from)
   {
     register dwrd *to, *endp;
   
     to = KnL, endp = &KnL[32];
     while(to < endp)
       *to++ = *from++;
     return;
   }
   
   /*
    *   NAME:  desNx8 ()
    *
    * SCRIPT:  Encrypt/decrypt input buffer and copy it to output buffer.
    *          Size of input buffer MUST BE divisible by 8.
    *
    * SYNTAX:  void  desNx8 (byte * in, byte * out, int sz)
    *
    * RETURN:  None.
    *
    *  NOTE:   If size of input buffer is not divisible by 8,
    *          reminder is not crypted and just copied.
    */
   void desNx8(byte * in, byte * out, int sz)
   { 
     int  i;
   
     for (i=0; i < sz; i+=8)
       des(in+i, out+i);
   
     if (sz%8)  //copy reminder
       memcpy(out+sz/8, in+sz/8, sz%8);
   
     return;
   }
   
   /* --------------------------------------------------------------------- *
    *   NAME:  des ()
    *
    * SCRIPT:  Encrypt/decrypt 8 bytes from input buffer
    *          and copy its to output buffer.
    *
    * SYNTAX:  void  des (byte in[8], byte out[8])
    *
    * RETURN:  None.
    * --------------------------------------------------------------------- */
   void des(byte *inblock, byte *outblock) 
   {
     dwrd work[2];
   
     scrunch(inblock, work);
     desfunc(work, KnL);
     unscrun(work, outblock);
   
     return;
   }
   
   
   // *************************************************************************************
   // *************************************************************************************

                                // ========================================================
   void DES1(                   // БАЗОВЫЙ АЛГОРИТМ D.E.S. ОПТИМИЗИРОВАННЫЙ ДЛЯ КОНТРОЛЛЕРА
                                // (интерфейс ПК/Контроллер)
                                // ========================================================
      unsigned char  option,    // Шифровать (ENCRYPT) или расшифровать (DECRYPT)
      unsigned char *key,       // Секретный ключ (8 байтов)
      unsigned char *tinp,      // Исходное сообщение (8 байтов)
      unsigned char *tout)      // Результат (зашифрованный или расшифрованныйб 8 байтов)
                                // ========================================================
   {
       if (option==ENCRYPT) deskey(key, ENCRYPT);
       else deskey(key, DECRYPT);
       des(tinp, tout);
       return;
   }   // ... DES1 для контроллера
   
   // *************************************************************************************
   // *************************************************************************************

   
   static void  scrunch (register byte *outof, register dwrd *into)
   {
     *into    = (*outof++ & 0xffL) << 24;
     *into   |= (*outof++ & 0xffL) << 16;
     *into   |= (*outof++ & 0xffL) << 8;
     *into++ |= (*outof++ & 0xffL);
     *into    = (*outof++ & 0xffL) << 24;
     *into   |= (*outof++ & 0xffL) << 16;
     *into   |= (*outof++ & 0xffL) << 8;
     *into   |= (*outof   & 0xffL);
   
     return;
   }
   
   static void  unscrun(register dwrd *outof, register byte *into)
   {
     *into++ = (char)(*outof >> 24)/* & 0xffL*/;
     *into++ = (char)(*outof >> 16)/* & 0xffL*/;
     *into++ = (char)(*outof >> 8) /* & 0xffL*/;
     *into++ = (char)(*outof++ & 0xffL);
     *into++ = (char)(*outof >> 24)/* & 0xffL*/;
     *into++ = (char)(*outof >> 16)/* & 0xffL*/;
     *into++ = (char)(*outof >> 8) /* & 0xffL*/;
     *into   = (char)(*outof & 0xffL);
   
     return;
   }
   
   
   static dwrd SP1[64] = {
           0x01010400L, 0x00000000L, 0x00010000L, 0x01010404L,
           0x01010004L, 0x00010404L, 0x00000004L, 0x00010000L,
           0x00000400L, 0x01010400L, 0x01010404L, 0x00000400L,
           0x01000404L, 0x01010004L, 0x01000000L, 0x00000004L,
           0x00000404L, 0x01000400L, 0x01000400L, 0x00010400L,
           0x00010400L, 0x01010000L, 0x01010000L, 0x01000404L,
           0x00010004L, 0x01000004L, 0x01000004L, 0x00010004L,
           0x00000000L, 0x00000404L, 0x00010404L, 0x01000000L,
           0x00010000L, 0x01010404L, 0x00000004L, 0x01010000L,
           0x01010400L, 0x01000000L, 0x01000000L, 0x00000400L,
           0x01010004L, 0x00010000L, 0x00010400L, 0x01000004L,
           0x00000400L, 0x00000004L, 0x01000404L, 0x00010404L,
           0x01010404L, 0x00010004L, 0x01010000L, 0x01000404L,
           0x01000004L, 0x00000404L, 0x00010404L, 0x01010400L,
           0x00000404L, 0x01000400L, 0x01000400L, 0x00000000L,
           0x00010004L, 0x00010400L, 0x00000000L, 0x01010004L };
   
   static dwrd SP2[64] = {
           0x80108020L, 0x80008000L, 0x00008000L, 0x00108020L,
           0x00100000L, 0x00000020L, 0x80100020L, 0x80008020L,
           0x80000020L, 0x80108020L, 0x80108000L, 0x80000000L,
           0x80008000L, 0x00100000L, 0x00000020L, 0x80100020L,
           0x00108000L, 0x00100020L, 0x80008020L, 0x00000000L,
           0x80000000L, 0x00008000L, 0x00108020L, 0x80100000L,
           0x00100020L, 0x80000020L, 0x00000000L, 0x00108000L,
           0x00008020L, 0x80108000L, 0x80100000L, 0x00008020L,
           0x00000000L, 0x00108020L, 0x80100020L, 0x00100000L,
           0x80008020L, 0x80100000L, 0x80108000L, 0x00008000L,
           0x80100000L, 0x80008000L, 0x00000020L, 0x80108020L,
           0x00108020L, 0x00000020L, 0x00008000L, 0x80000000L,
           0x00008020L, 0x80108000L, 0x00100000L, 0x80000020L,
           0x00100020L, 0x80008020L, 0x80000020L, 0x00100020L,
           0x00108000L, 0x00000000L, 0x80008000L, 0x00008020L,
           0x80000000L, 0x80100020L, 0x80108020L, 0x00108000L };
   
   static dwrd SP3[64] = {
           0x00000208L, 0x08020200L, 0x00000000L, 0x08020008L,
           0x08000200L, 0x00000000L, 0x00020208L, 0x08000200L,
           0x00020008L, 0x08000008L, 0x08000008L, 0x00020000L,
           0x08020208L, 0x00020008L, 0x08020000L, 0x00000208L,
           0x08000000L, 0x00000008L, 0x08020200L, 0x00000200L,
           0x00020200L, 0x08020000L, 0x08020008L, 0x00020208L,
           0x08000208L, 0x00020200L, 0x00020000L, 0x08000208L,
           0x00000008L, 0x08020208L, 0x00000200L, 0x08000000L,
           0x08020200L, 0x08000000L, 0x00020008L, 0x00000208L,
           0x00020000L, 0x08020200L, 0x08000200L, 0x00000000L,
           0x00000200L, 0x00020008L, 0x08020208L, 0x08000200L,
           0x08000008L, 0x00000200L, 0x00000000L, 0x08020008L,
           0x08000208L, 0x00020000L, 0x08000000L, 0x08020208L,
           0x00000008L, 0x00020208L, 0x00020200L, 0x08000008L,
           0x08020000L, 0x08000208L, 0x00000208L, 0x08020000L,
           0x00020208L, 0x00000008L, 0x08020008L, 0x00020200L };
   
   static dwrd SP4[64] = {
           0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
           0x00802080L, 0x00800081L, 0x00800001L, 0x00002001L,
           0x00000000L, 0x00802000L, 0x00802000L, 0x00802081L,
           0x00000081L, 0x00000000L, 0x00800080L, 0x00800001L,
           0x00000001L, 0x00002000L, 0x00800000L, 0x00802001L,
           0x00000080L, 0x00800000L, 0x00002001L, 0x00002080L,
           0x00800081L, 0x00000001L, 0x00002080L, 0x00800080L,
           0x00002000L, 0x00802080L, 0x00802081L, 0x00000081L,
           0x00800080L, 0x00800001L, 0x00802000L, 0x00802081L,
           0x00000081L, 0x00000000L, 0x00000000L, 0x00802000L,
           0x00002080L, 0x00800080L, 0x00800081L, 0x00000001L,
           0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
           0x00802081L, 0x00000081L, 0x00000001L, 0x00002000L,
           0x00800001L, 0x00002001L, 0x00802080L, 0x00800081L,
           0x00002001L, 0x00002080L, 0x00800000L, 0x00802001L,
           0x00000080L, 0x00800000L, 0x00002000L, 0x00802080L };
   
   static dwrd SP5[64] = {
           0x00000100L, 0x02080100L, 0x02080000L, 0x42000100L,
           0x00080000L, 0x00000100L, 0x40000000L, 0x02080000L,
           0x40080100L, 0x00080000L, 0x02000100L, 0x40080100L,
           0x42000100L, 0x42080000L, 0x00080100L, 0x40000000L,
           0x02000000L, 0x40080000L, 0x40080000L, 0x00000000L,
           0x40000100L, 0x42080100L, 0x42080100L, 0x02000100L,
           0x42080000L, 0x40000100L, 0x00000000L, 0x42000000L,
           0x02080100L, 0x02000000L, 0x42000000L, 0x00080100L,
           0x00080000L, 0x42000100L, 0x00000100L, 0x02000000L,
           0x40000000L, 0x02080000L, 0x42000100L, 0x40080100L,
           0x02000100L, 0x40000000L, 0x42080000L, 0x02080100L,
           0x40080100L, 0x00000100L, 0x02000000L, 0x42080000L,
           0x42080100L, 0x00080100L, 0x42000000L, 0x42080100L,
           0x02080000L, 0x00000000L, 0x40080000L, 0x42000000L,
           0x00080100L, 0x02000100L, 0x40000100L, 0x00080000L,
           0x00000000L, 0x40080000L, 0x02080100L, 0x40000100L };
   
   static dwrd SP6[64] = {
           0x20000010L, 0x20400000L, 0x00004000L, 0x20404010L,
           0x20400000L, 0x00000010L, 0x20404010L, 0x00400000L,
           0x20004000L, 0x00404010L, 0x00400000L, 0x20000010L,
           0x00400010L, 0x20004000L, 0x20000000L, 0x00004010L,
           0x00000000L, 0x00400010L, 0x20004010L, 0x00004000L,
           0x00404000L, 0x20004010L, 0x00000010L, 0x20400010L,
           0x20400010L, 0x00000000L, 0x00404010L, 0x20404000L,
           0x00004010L, 0x00404000L, 0x20404000L, 0x20000000L,
           0x20004000L, 0x00000010L, 0x20400010L, 0x00404000L,
           0x20404010L, 0x00400000L, 0x00004010L, 0x20000010L,
           0x00400000L, 0x20004000L, 0x20000000L, 0x00004010L,
           0x20000010L, 0x20404010L, 0x00404000L, 0x20400000L,
           0x00404010L, 0x20404000L, 0x00000000L, 0x20400010L,
           0x00000010L, 0x00004000L, 0x20400000L, 0x00404010L,
           0x00004000L, 0x00400010L, 0x20004010L, 0x00000000L,
           0x20404000L, 0x20000000L, 0x00400010L, 0x20004010L };
   
   static dwrd SP7[64] = {
           0x00200000L, 0x04200002L, 0x04000802L, 0x00000000L,
           0x00000800L, 0x04000802L, 0x00200802L, 0x04200800L,
           0x04200802L, 0x00200000L, 0x00000000L, 0x04000002L,
           0x00000002L, 0x04000000L, 0x04200002L, 0x00000802L,
           0x04000800L, 0x00200802L, 0x00200002L, 0x04000800L,
           0x04000002L, 0x04200000L, 0x04200800L, 0x00200002L,
           0x04200000L, 0x00000800L, 0x00000802L, 0x04200802L,
           0x00200800L, 0x00000002L, 0x04000000L, 0x00200800L,
           0x04000000L, 0x00200800L, 0x00200000L, 0x04000802L,
           0x04000802L, 0x04200002L, 0x04200002L, 0x00000002L,
           0x00200002L, 0x04000000L, 0x04000800L, 0x00200000L,
           0x04200800L, 0x00000802L, 0x00200802L, 0x04200800L,
           0x00000802L, 0x04000002L, 0x04200802L, 0x04200000L,
           0x00200800L, 0x00000000L, 0x00000002L, 0x04200802L,
           0x00000000L, 0x00200802L, 0x04200000L, 0x00000800L,
           0x04000002L, 0x04000800L, 0x00000800L, 0x00200002L };
   
   static dwrd SP8[64] = {
           0x10001040L, 0x00001000L, 0x00040000L, 0x10041040L,
           0x10000000L, 0x10001040L, 0x00000040L, 0x10000000L,
           0x00040040L, 0x10040000L, 0x10041040L, 0x00041000L,
           0x10041000L, 0x00041040L, 0x00001000L, 0x00000040L,
           0x10040000L, 0x10000040L, 0x10001000L, 0x00001040L,
           0x00041000L, 0x00040040L, 0x10040040L, 0x10041000L,
           0x00001040L, 0x00000000L, 0x00000000L, 0x10040040L,
           0x10000040L, 0x10001000L, 0x00041040L, 0x00040000L,
           0x00041040L, 0x00040000L, 0x10041000L, 0x00001000L,
           0x00000040L, 0x10040040L, 0x00001000L, 0x00041040L,
           0x10001000L, 0x00000040L, 0x10000040L, 0x10040000L,
           0x10040040L, 0x10000000L, 0x00040000L, 0x10001040L,
           0x00000000L, 0x10041040L, 0x00040040L, 0x10000040L,
           0x10040000L, 0x10001000L, 0x10001040L, 0x00000000L,
           0x10041040L, 0x00041000L, 0x00041000L, 0x00001040L,
           0x00001040L, 0x00040040L, 0x10000000L, 0x10041000L };
   
   
   static void desfunc(register dwrd *block,
                       register dwrd *keys)
   {
     register dwrd fval, work, right, leftt;
     register int round;
   
     leftt = block[0];
     right = block[1];
     work = ((leftt >> 4) ^ right) & 0x0f0f0f0fL;
     right ^= work;
     leftt ^= (work << 4);
     work = ((leftt >> 16) ^ right) & 0x0000ffffL;
     right ^= work;
     leftt ^= (work << 16);
     work = ((right >> 2) ^ leftt) & 0x33333333L;
     leftt ^= work;
     right ^= (work << 2);
     work = ((right >> 8) ^ leftt) & 0x00ff00ffL;
     leftt ^= work;
     right ^= (work << 8);
     right = ((right << 1) | ((right >> 31) & 1L)) & 0xffffffffL;
     work = (leftt ^ right) & 0xaaaaaaaaL;
     leftt ^= work;
     right ^= work;
     leftt = ((leftt << 1) | ((leftt >> 31) & 1L)) & 0xffffffffL;
   
     for(round = 0; round < 8; round++)
     {
       work  = (right << 28) | (right >> 4);
       work ^= *keys++;
       fval  = SP7[(Uint)work       & 0x3f];
       fval |= SP5[(Uint)(work>> 8) & 0x3f];
       fval |= SP3[(Uint)(work>>16) & 0x3f];
       fval |= SP1[(Uint)(work>>24) & 0x3f];
       work  = right ^ *keys++;
       fval |= SP8[(Uint)work       & 0x3f];
       fval |= SP6[(Uint)(work>> 8) & 0x3f];
       fval |= SP4[(Uint)(work>>16) & 0x3f];
       fval |= SP2[(Uint)(work>>24) & 0x3f];
       leftt ^= fval;
       work  = (leftt << 28) | (leftt >> 4);
       work ^= *keys++;
       fval  = SP7[(Uint)work       & 0x3f];
       fval |= SP5[(Uint)(work>> 8) & 0x3f];
       fval |= SP3[(Uint)(work>>16) & 0x3f];
       fval |= SP1[(Uint)(work>>24) & 0x3f];
       work  = leftt ^ *keys++;
       fval |= SP8[(Uint)work       & 0x3f];
       fval |= SP6[(Uint)(work>> 8) & 0x3f];
       fval |= SP4[(Uint)(work>>16) & 0x3f];
       fval |= SP2[(Uint)(work>>24) & 0x3f];
       right ^= fval;
     }
   
     right = (right << 31) | (right >> 1);
     work = (leftt ^ right) & 0xaaaaaaaaL;
     leftt ^= work;
     right ^= work;
     leftt = (leftt << 31) | (leftt >> 1);
     work = ((leftt >> 8) ^ right) & 0x00ff00ffL;
     right ^= work;
     leftt ^= (work << 8);
     work = ((leftt >> 2) ^ right) & 0x33333333L;
     right ^= work;
     leftt ^= (work << 2);
     work = ((right >> 16) ^ leftt) & 0x0000ffffL;
     leftt ^= work;
     right ^= (work << 16);
     work = ((right >> 4) ^ leftt) & 0x0f0f0f0fL;
     leftt ^= work;
     right ^= (work << 4);
     *block++ = right;
     *block = leftt;
   
     return;
   }
     
   
#endif // ... #else на #ifdef DES_PC 


