/*
 * ===========================================================================
 *
 *       Filename:  unicode_utf-8.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2011年12月23日 15时23分45秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  nuoerlz (nuoliu), nuoerlz@gmail.com
 *        Company:  mhtt
 *
 *      CopyRight:  Reserve
 *
 * ===========================================================================
 */

/*
 * Unicode 与 UTF-8 的关系:
 *
 *   Unicode 编码                     UTF-8 存储码
 * ========================  =====================================================
 * U-00000000 - U-0000007F:  0xxxxxxx
 * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
 * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
                                 ====   ====--   ------
 * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                                  +++   ++====   ====--   ------
 * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                                   ^^   ++++++   ++====   ====--   ------
 * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                                    ^   ^^^^^^   ++++++   ++====   ====--   ------
 * 
 *  (前面几个1就代表后面几个字节是属于一起的)
 *  
 */


/*
 * Unicode有二字节编码(UCS-2)和四字节编码(UCS-4)两种, 目前常用的二字节编码. 下
 * 面程序给出的是UTF-8转成Unicode(UCS-2)的函数.
 */

#include "unicode_utf-8.h"
#include <string.h>
#include <assert.h>


/*****************************************************************************
 * 根据字符的UTF-8编码的第一个字节求出该字符用UTF-8编码存储时所需要多少个
 * 字节空间. 特殊地, 对于只占一个字节的字符(ASCII), 返回值为 0 .
 *
 * 参数:
 *    char c    字符的UTF-8编码的第一个字节的值
 *
 * 返回值:
 *   该字符用UTF-8编码存储时所需要多少个字节空间.
 *   特殊地, 对于只占一个字节的字符(ASCII), 返回值为 0 .
 ****************************************************************************/
int enc_get_utf8_size(char c)
{
    char t = 1 << 7;
    char r = c;
    int count = 0;
    while (r & t)
    {
        r = r << 1;
        count++;
    }
    return count;
}


/*****************************************************************************
 * 将UTF8编码转换成Unicode(UCS-2)编码.
 *
 * 参数:
 *    char* pInput   指向输入字符串(以'\0'结尾)的指针
 *    char* pOutput  指向输出字符串指针的指针
 *    int   outsize  pOutput 缓冲的大小
 *
 * 返回值:
 *    返回转换后的Unicode字符串的字节数, 如果出错则返回 -1
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode字符有字节序,
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种,
 *        在Intel处理器中采用小端法表示, 本例中采用小端法表示. (低地址存低位)
 *     2. 请保证 pOutput 缓冲区有足够的空间, 若空间不足, 则只进行部分转换.
 ****************************************************************************/
int enc_utf8_to_unicode(const char* pInput, char* pOutput, int outsize)
{
    //assert(pInput != NULL && pOutput != NULL);
    //assert(outsize >= 3);

    int outputSize = 0;  //记录转换后的Unicode字符串的字节数
    char *pout = pOutput;

    while ( *pInput && outputSize + 2 + 1 <= outsize )
    {
        if (*pInput > 0x00 && *pInput <= 0x7F) // 处理单字节UTF8字符
        {
            *pout     = *pInput;
            *(pout+1) = 0;       // 小端法表示, 在高地址填补0
            pInput += 1;
        }
        else if (((*pInput) & 0xE0) == 0xC0) // 处理双字节UTF8字符
        {
            char high = *pInput;
            char low  = *(pInput + 1);

            if ((low & 0xC0) != 0x80) // 检查是否为合法的UTF8字符表示
                return -1;

            *pout     = (high << 6) + (low & 0x3F);
            *(pout+1) = (high >> 2) & 0x07;
            pInput += 2;
        }
        else if (((*pInput) & 0xF0) == 0xE0) // 处理三字节UTF8字符
        {
            char high   = *pInput;
            char middle = *(pInput + 1);
            char low    = *(pInput + 2);

            if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
                return -1;

            *pout     = (middle << 6) + (low & 0x3F);
            *(pout+1) = (high << 4) + ((middle >> 2) & 0x0F);
            pInput += 3;
        }
        else //对于其他字节数的UTF8字符不进行处理
        {
            return -1;
        }

        pout += 2;
        outputSize += 2;
    }

    *pout = 0;

    return outputSize;
}

#if 1
/*****************************************************************************
 * 将UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
 *
 * 参数:
 *    char* pInput   指向输入字符串, 以UTF-8编码
 *    char* pOutput  指向输出字符串, 以Unicode编码
 *
 * 返回值:
 *    返回转换后的字符的Unicode编码所占的字节数, 如果出错则返回 -1 .
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode字符有字节序,
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种,
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 *     2. 请保证 pOutput 缓冲区有最少有 4 字节的空间大小!
 ****************************************************************************/
int enc_utf8_to_unicode_one(const char* pInput, char* pOutput)
{
    //assert(pInput != NULL && pOutput != NULL);

    // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
    char b1, b2, b3, b4, b5, b6;
    int utfbytes = enc_get_utf8_size(*pInput);

    switch ( utfbytes )
    {
        case 0:
            *pOutput     = *pInput;
            return 1;
            break;
        case 2:
            b1 = *pInput;
            b2 = *(pInput + 1);
            if ( (b2 & 0xE0) != 0x80 )
                return -1;
            *pOutput     = (b1 << 6) + (b2 & 0x3F);
            *(pOutput+1) = (b1 >> 2) & 0x07;
            return 2;
            break;
        case 3:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
                return -1;
            *pOutput     = (b2 << 6) + (b3 & 0x3F);
            *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            return 2;
            break;
        case 4:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) 
                    || ((b4 & 0xC0) != 0x80) )
                return -1;
            *pOutput     = (b3 << 6) + (b4 & 0x3F);
            *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
            return 3;
            break;
        case 5:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) 
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
                return -1;
            *pOutput     = (b4 << 6) + (b5 & 0x3F);
            *(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *(pOutput+3) = (b1 << 6);
            return 4;
            break;
        case 6:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            b6 = *(pInput + 5);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) 
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) 
                    || ((b6 & 0xC0) != 0x80) )
                return -1;
            *pOutput     = (b5 << 6) + (b6 & 0x3F);
            *(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            return 4;
            break;
        default:
            assert(0 && "pInput is NOT a UTF-8 encoding!");
            break;
    }

    return -1;
}

int enc_utf8_to_unicode_one_uint(const char* pInput, unsigned int *Output)
{
    int          ret;

    assert(sizeof(*Output) >= 4);

    *Output = 0x0; // 把 *Output 初始化为全零
    ret = enc_utf8_to_unicode_one(pInput, (char *)Output);

    return ret;
}

#endif
