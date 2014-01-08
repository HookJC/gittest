/*
 * ===========================================================================
 *
 *       Filename:  unicode_utf-8.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2011��12��23�� 15ʱ23��45��
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
 * Unicode �� UTF-8 �Ĺ�ϵ:
 *
 *   Unicode ����                     UTF-8 �洢��
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
 *  (ǰ�漸��1�ʹ�����漸���ֽ�������һ���)
 *  
 */


/*
 * Unicode�ж��ֽڱ���(UCS-2)�����ֽڱ���(UCS-4)����, Ŀǰ���õĶ��ֽڱ���. ��
 * ������������UTF-8ת��Unicode(UCS-2)�ĺ���.
 */

#include "unicode_utf-8.h"
#include <string.h>
#include <assert.h>


/*****************************************************************************
 * �����ַ���UTF-8����ĵ�һ���ֽ�������ַ���UTF-8����洢ʱ����Ҫ���ٸ�
 * �ֽڿռ�. �����, ����ֻռһ���ֽڵ��ַ�(ASCII), ����ֵΪ 0 .
 *
 * ����:
 *    char c    �ַ���UTF-8����ĵ�һ���ֽڵ�ֵ
 *
 * ����ֵ:
 *   ���ַ���UTF-8����洢ʱ����Ҫ���ٸ��ֽڿռ�.
 *   �����, ����ֻռһ���ֽڵ��ַ�(ASCII), ����ֵΪ 0 .
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
 * ��UTF8����ת����Unicode(UCS-2)����.
 *
 * ����:
 *    char* pInput   ָ�������ַ���(��'\0'��β)��ָ��
 *    char* pOutput  ָ������ַ���ָ���ָ��
 *    int   outsize  pOutput ����Ĵ�С
 *
 * ����ֵ:
 *    ����ת�����Unicode�ַ������ֽ���, ��������򷵻� -1
 *
 * ע��:
 *     1. UTF8û���ֽ�������, ����Unicode�ַ����ֽ���,
 *        �ֽ����Ϊ���(Big Endian)��С��(Little Endian)����,
 *        ��Intel�������в���С�˷���ʾ, �����в���С�˷���ʾ. (�͵�ַ���λ)
 *     2. �뱣֤ pOutput ���������㹻�Ŀռ�, ���ռ䲻��, ��ֻ���в���ת��.
 ****************************************************************************/
int enc_utf8_to_unicode(const char* pInput, char* pOutput, int outsize)
{
    //assert(pInput != NULL && pOutput != NULL);
    //assert(outsize >= 3);

    int outputSize = 0;  //��¼ת�����Unicode�ַ������ֽ���
    char *pout = pOutput;

    while ( *pInput && outputSize + 2 + 1 <= outsize )
    {
        if (*pInput > 0x00 && *pInput <= 0x7F) // �����ֽ�UTF8�ַ�
        {
            *pout     = *pInput;
            *(pout+1) = 0;       // С�˷���ʾ, �ڸߵ�ַ�0
            pInput += 1;
        }
        else if (((*pInput) & 0xE0) == 0xC0) // ����˫�ֽ�UTF8�ַ�
        {
            char high = *pInput;
            char low  = *(pInput + 1);

            if ((low & 0xC0) != 0x80) // ����Ƿ�Ϊ�Ϸ���UTF8�ַ���ʾ
                return -1;

            *pout     = (high << 6) + (low & 0x3F);
            *(pout+1) = (high >> 2) & 0x07;
            pInput += 2;
        }
        else if (((*pInput) & 0xF0) == 0xE0) // �������ֽ�UTF8�ַ�
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
        else //���������ֽ�����UTF8�ַ������д���
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
 * ��UTF8����ת����Unicode(UCS-2��UCS-4)����.
 *
 * ����:
 *    char* pInput   ָ�������ַ���, ��UTF-8����
 *    char* pOutput  ָ������ַ���, ��Unicode����
 *
 * ����ֵ:
 *    ����ת������ַ���Unicode������ռ���ֽ���, ��������򷵻� -1 .
 *
 * ע��:
 *     1. UTF8û���ֽ�������, ����Unicode�ַ����ֽ���,
 *        �ֽ����Ϊ���(Big Endian)��С��(Little Endian)����,
 *        ��Intel�������в���С�˷���ʾ, �ڴ˲���С�˷���ʾ. (�͵�ַ���λ)
 *     2. �뱣֤ pOutput �������������� 4 �ֽڵĿռ��С!
 ****************************************************************************/
int enc_utf8_to_unicode_one(const char* pInput, char* pOutput)
{
    //assert(pInput != NULL && pOutput != NULL);

    // b1 ��ʾUTF-8�����pInput�еĸ��ֽ�, b2 ��ʾ�θ��ֽ�, ...
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

    *Output = 0x0; // �� *Output ��ʼ��Ϊȫ��
    ret = enc_utf8_to_unicode_one(pInput, (char *)Output);

    return ret;
}

#endif
