#include "AppTools.h"

typedef struct _t9_symbol
{
    char Key;
    char Symbol[10];
} t9_symbol;

typedef struct _t9_py_index
{
    unsigned int chn_offset;
    unsigned int chn_len;
    char t9[8];
    char py[8];
    char py_table[220];
} t9_py_index;

t9_py_index  t9_pinyin_list[T9PY_MAX_NOMINEES];

char t9_tmp_buffer[2048];         //t9ʹ�õ���ʱ����
int t9_final_nominees;            //���յ�ƴ�����
int t9_cur_mode;              //��ǰ���뷨״̬
int  t9_current_py_pointer;         //��ǰѡ���ƴ��

#ifdef _SHARED_LIB
int t9_disp_line;           //���ַ�ʽ������
int t9_select_line;           //���ַ�ʽ������
int t9_alpha_key;         //ƴ��ѡ������뷽ʽ�л���
int t9_key_a;           //�ַ�ѡ���
int t9_key_b;           //�ַ�ѡ���
int t9_key_c;           //��ҳ��
int t9_key_d;           //��ҳ��

int t9_abc_cur;           //��ǰ���ַ���ABCabc
int t9_abc_pos;           //��ǰѡ��ľ����ַ�A

int t9_symbol_cur;          //��ǰ�ķ��ű�.<>
int t9_symbol_pos;          //��ǰѡ��ľ������"

int t9_config_flg;          //���ñ�־

t9_symbol t9_symbol_list_1[12]; //���������
t9_symbol t9_symbol_list_2[12]; //���������
char t9_abc_list[10][10];   //�ַ������

int t9_char_width;          //�и�
int t9_char_height;         //�����С

#else
static int t9_disp_line = 2;            //���ַ�ʽ������
static int t9_select_line = 3;            //���ַ�ʽ������
static int t9_alpha_key = bALPHAKEY;        //ƴ��ѡ������뷽ʽ�л���
static int t9_key_a = bKEYA;            //�ַ�ѡ���
static int t9_key_b = bKEYB;            //�ַ�ѡ���
static int t9_key_c = bKEYC;            //��ҳ��
static int t9_key_d = bKEYD;            //��ҳ��

static int t9_abc_cur = 0;              //��ǰ���ַ���ABCabc
static int t9_abc_pos = 0;              //��ǰѡ��ľ����ַ�A

static int t9_symbol_cur = 0;           //��ǰ�ķ��ű�.<>
static int t9_symbol_pos = 0;           //��ǰѡ��ľ������"

static int t9_config_flg = 0;           //���ñ�־

static t9_symbol t9_symbol_list_1[12] = //���������
{
    {bKEY0, " "},
    {bKEY1, ".!$"},
    {bKEY2, "%&("},
    {bKEY3, ")+-"},
    {bKEY4, "/:;"},
    {bKEY5, "<=>"},
    {bKEY6, "?@["},
    {bKEY7, "\\]^"},
    {bKEY8, "_`{"},
    {bKEY9, "|}~"},
    {'*', "*,'\""},
    {'#', "#"}
};

static t9_symbol t9_symbol_list_2[12] = //���������
{
    {bKEY0, " *#,'\""},
    {bKEY1, ".!$"},
    {bKEY2, "%&("},
    {bKEY3, ")+-"},
    {bKEY4, "/:;"},
    {bKEY5, "<=>"},
    {bKEY6, "?@["},
    {bKEY7, "\\]^"},
    {bKEY8, "_`{"},
    {bKEY9, "|}~"},
    {0, ""},
    {0, ""}
};

static char t9_abc_list[10][10] = //�ַ������
{
    {"QZqz"},
    {"ABCabc"},
    {"DEFdef"},
    {"GHIghi"},
    {"JKLjkl"},
    {"MNOmno"},
    {"PRSprs"},
    {"TUVtuv"},
    {"WXYwxy"},
    {""}
};

static int t9_char_width = 28;    //�и�
static int t9_char_height = 32;   //�����С

#endif

#ifdef _SHARED_LIB
void T9PY_InitPrm(void)
{
    //���������
    t9_symbol temp_list_1[12] =
    {
        {KEY0, " "},
        {KEY1, ".!$"},
        {KEY2, "%&("},
        {KEY3, ")+-"},
        {KEY4, "/:;"},
        {KEY5, "<=>"},
        {KEY6, "?@["},
        {KEY7, "\\]^"},
        {KEY8, "_`{"},
        {KEY9, "|}~"},
        {'*', "*,'\""},
        {'#', "#"}
    };

    //���������
    t9_symbol temp_list_2[12] =
    {
        {KEY0, " *#,'\""},
        {KEY1, ".!$"},
        {KEY2, "%&("},
        {KEY3, ")+-"},
        {KEY4, "/:;"},
        {KEY5, "<=>"},
        {KEY6, "?@["},
        {KEY7, "\\]^"},
        {KEY8, "_`{"},
        {KEY9, "|}~"},
        {0, ""},
        {0, ""}
    };

    int i = 0;

    for (i = 0; i < 12; i++)
        t9_symbol_list_1[i] =  temp_list_1[i];

    for (i = 0; i < 12; i++)
        t9_symbol_list_2[i] =  temp_list_2[i];

    //�ַ������
    strcpy(t9_abc_list[0], "QZqz");
    strcpy(t9_abc_list[1], "ABCabc");
    strcpy(t9_abc_list[2], "DEFdef");
    strcpy(t9_abc_list[3], "GHIghi");
    strcpy(t9_abc_list[4], "JKLjkl");
    strcpy(t9_abc_list[5], "MNOmno");
    strcpy(t9_abc_list[6], "PRSprs");
    strcpy(t9_abc_list[7], "TUVtuv");
    strcpy(t9_abc_list[8], "WXYwxy");
    strcpy(t9_abc_list[9], "");

    t9_disp_line = 2;           //���ַ�ʽ������
    t9_select_line = 3;           //���ַ�ʽ������
    t9_alpha_key = bALPHAKEY;       //ƴ��ѡ������뷽ʽ�л���
    t9_key_a = bKEYA;           //�ַ�ѡ���
    t9_key_b = bKEYB;           //�ַ�ѡ���
    t9_key_c = bKEYC;           //��ҳ��
    t9_key_d = bKEYD;           //��ҳ��

    t9_abc_cur = 0;             //��ǰ���ַ���ABCabc
    t9_abc_pos = 0;             //��ǰѡ��ľ����ַ�A

    t9_symbol_cur = 0;            //��ǰ�ķ��ű�.<>
    t9_symbol_pos = 0;            //��ǰѡ��ľ������"

    t9_config_flg = 0;            //���ñ�־

    t9_char_height = 32;          //�и�
    t9_char_width = 28;           //�����С

}
#endif

/* --------------------------------------------------------------------------
* FUNCTION NAME: PYLib_Search
* DESCRIPTION:  ��ƴ����������ƴ���������������
*               num -- ƴ����Ӧ��������ϣ���zhuang��Ӧ148264
*               len -- ƴ����Ӧ��������ϵĳ���,��zhuang��Ӧ��6
*               py_nomination -- num��Ӧ��������ƴ������е�������
* RETURN:
* NOTES:        py_nomination�����㹻��
* ------------------------------------------------------------------------ */

int T9PY_SearchPinYin(char *num, int len)
{
    int nominees = 0;

    int pyidx_handle = 0, chn_handle = 0;
    int rec_len = 0;
    t9_py_index  temp_t9;

    //ApplLog_Debug("ATOOL","t9 inputed=[%s] len=[%d]", num, len);

    pyidx_handle = open(PYINDEX, O_RDONLY);

    //ApplLog_Debug("ATOOL","pyidx_handle=[%d]", pyidx_handle);
    if (pyidx_handle < 0)
    {
        return nominees;
    }

    chn_handle = open(PYLIB, O_RDONLY);

    //ApplLog_Debug("ATOOL","chn_handle=[%d]", chn_handle);
    if (chn_handle < 0)
    {
        close(pyidx_handle);
        return nominees;
    }

    lseek(pyidx_handle, 0, SEEK_SET);

    while (1) /*�������t9ƴ����ѯ��*/
    {
        memset(&temp_t9, 0, sizeof(t9_py_index));
        rec_len = read(pyidx_handle, (char *) & temp_t9, 24);

        //ApplLog_Debug("ATOOL","offset=[%d] len=[%d]", temp_t9.chn_offset, temp_t9.chn_len);
        //ApplLog_Debug("ATOOL","t9=[%s] py=[%s]", temp_t9.t9, temp_t9.py);
        if (rec_len != 24)
        {
            close(pyidx_handle);
            close(chn_handle);
            return nominees;
        }

        if (strlen(temp_t9.t9) > len) //���¼���ȳ��������ַ������ȣ�����
        {
            close(pyidx_handle);
            close(chn_handle);
            return nominees;
        }

        if (!strncmp(num, temp_t9.t9, len))
        {
            if (nominees == 0)
                memset(t9_pinyin_list, 0, sizeof(t9_pinyin_list));

            lseek(chn_handle, temp_t9.chn_offset, SEEK_SET);
            rec_len = read(chn_handle, temp_t9.py_table, temp_t9.chn_len);

            //ApplLog_Debug("ATOOL","len=[%d] hz=[%s]", rec_len, temp_t9.py_table);
            if (rec_len != temp_t9.chn_len)
            {
                close(pyidx_handle);
                close(chn_handle);
                return nominees;
            }

            //ApplLog_Debug("ATOOL","nominees=[%d] py=[%s] hz_offset=[%d] hz-size=[%d]", nominees, temp_t9.py, temp_t9.chn_offset, temp_t9.chn_len);
            //ApplLog_Debug("ATOOL","hz=[%s]", temp_t9.py, temp_t9.py_table);

            t9_pinyin_list[nominees++] = temp_t9;
        }

        if (nominees == T9PY_MAX_NOMINEES)
            break;

    }

    close(pyidx_handle);
    close(chn_handle);
    return nominees;
}

// ��ʾABCabc�б�,lineΪ���ķ�ʽ������
static void T9PY_DispAbcList(int line)
{
    int  fp, j, k, m;
    char buf1[20];
    char font[30];
    BOOL bIsGbkFont16;

    memset(buf1, 0, sizeof(buf1));
    strncpy(buf1, t9_abc_list[t9_abc_cur-'1'], 6);
    m = strlen(buf1);

    if (IsTTFSupport())
    {
        MmiUtil_SetFontSize(t9_char_width);
        MmiUtil_ClearLine(line);
        MmiUtil_DisplayLine(buf1, line);
        invert_pixel_block(t9_abc_pos*t9_char_width/2, (line-1)*t9_char_height+HEADER_HEIGHT, (t9_abc_pos + 1)*t9_char_width/2, line*t9_char_height+HEADER_HEIGHT);

        return;
    }

    memset(font, 0, sizeof(font));
    //MmiUtil_GetFont(font);
    bIsGbkFont16 = isGbkFont16();	//added by baijz 20130515 for GBK
    if (bIsGbkFont16)
    {
        strcpy(font, GBKFONT_16);
    }
    else
    {
        strcpy(font, CHNFONT);
    }
    fp = open(font, O_RDONLY);

    memset(t9_tmp_buffer, 0, sizeof(t9_tmp_buffer));

    for (j = 0; j < m; j ++)
    {
        lseek(fp, buf1[j] * 16 + 16, SEEK_SET);//һ�������ַ�16���ֽڵ���
        read(fp, t9_tmp_buffer + j * 16, 16);

        if (j == t9_abc_pos)  //���Ե�ǰ���ַ�
        {
            for (k = 0; k < 16; k++)
                t9_tmp_buffer[j*16 + k] = ~t9_tmp_buffer[j*16+k];
        }
    }

    set_font(font);
    window(1, line, 16, line + 1);
    putpixelcol(t9_tmp_buffer, 256);

    close(fp);
}

// ��ʾ.$;�б�,lineΪ���ķ�ʽ������
static void T9PY_DispSymbolList(int line)
{
    int  fp, j, k, m;
    char font[30];
    char buf1[20];
    BOOL bIsGbkFont16;

    memset(buf1, 0, sizeof(buf1));

    for (j = 0; j < 12; j++)
    {
        if (t9_config_flg & T9PY_SYMBOL_FLG1)
        {
            //*�ż���#�ż������ڵ������
            if (t9_symbol_list_2[j].Key == t9_symbol_cur)
                strncpy(buf1, t9_symbol_list_2[j].Symbol, 8);
        }
        else if (t9_symbol_list_1[j].Key == t9_symbol_cur)
        {
            //*�ż���#�ż����ڵ������
            strncpy(buf1, t9_symbol_list_1[j].Symbol, 8);
        }
    }

    m = strlen(buf1);

    if (IsTTFSupport())
    {
        MmiUtil_SetFontSize(t9_char_width);
        MmiUtil_ClearLine(line);
        MmiUtil_DisplayLine(buf1, line);
        invert_pixel_block(t9_symbol_pos*t9_char_width/2, (line-1)*t9_char_height+HEADER_HEIGHT, (t9_symbol_pos + 1)*t9_char_width/2, line*t9_char_height+HEADER_HEIGHT);

        return;
    }

    memset(font, 0, sizeof(font));
    //MmiUtil_GetFont(font);
//    strcpy(font, CHNFONT);
    bIsGbkFont16 = isGbkFont16();	//added by baijz 20130515 for GBK
    if (bIsGbkFont16)
    {
        strcpy(font, GBKFONT_16);
    }
    else
    {
        strcpy(font, CHNFONT);
    }
    fp = open(font, O_RDONLY);

    memset(t9_tmp_buffer, 0, sizeof(t9_tmp_buffer));

    for (j = 0; j < m; j ++)
    {
        lseek(fp, buf1[j] * 16 + 16, SEEK_SET);//һ�������ַ�16���ֽڵ���
        read(fp, t9_tmp_buffer + j * 16, 16);

        if (j == t9_symbol_pos)  //���Ե�ǰ���ַ�
        {
            for (k = 0; k < 16; k++)
                t9_tmp_buffer[j*16 + k] = ~t9_tmp_buffer[j*16+k];
        }
    }

    set_font(font);
    window(1, line, 16, line + 1);
    putpixelcol(t9_tmp_buffer, 256);

    close(fp);
}

// ��ʾƴ���б�,������ѡ�е�ƴ��,lineΪӢ�ķ�ʽ������
static void T9PY_DispPinYinList(int line, int selected)
{
    int  fp, i, j, k, m, n = 0;
    long offset = 0, offset2 = 0;
    char buf1[20];

    if (IsTTFSupport())
    {
        MmiUtil_SetFontSize(t9_char_width);
        memset(t9_tmp_buffer, 0x00, sizeof(t9_tmp_buffer));

        for (i = 0; i < t9_final_nominees; i++)
        {
            memset(buf1, 0, sizeof(buf1));
            strcpy(buf1, t9_pinyin_list[i].py);
            strcat(buf1, " ");
            m = strlen(buf1);

            memcpy(t9_tmp_buffer + n, buf1, m);

            if (i == selected)  //���Ե�ǰ��ƴ��
            {
                offset = n;
                offset2 = n + m - 1;
            }

            n += m;
        }

        MmiUtil_ClearLine(line);
        MmiUtil_DisplayLine(t9_tmp_buffer, line);
        invert_pixel_block(offset*t9_char_width/2, (line-1)*t9_char_height+HEADER_HEIGHT, offset2*t9_char_width/2, line*t9_char_height+HEADER_HEIGHT);

        return;
    }

    memset(t9_tmp_buffer, 0x00, sizeof(t9_tmp_buffer));
    fp = open(T9PY_HELP_FONT, O_RDONLY);

    for (i = 0; i < t9_final_nominees; i++)
    {
        memset(buf1, 0, sizeof(buf1));
        strcpy(buf1, t9_pinyin_list[i].py);
        strcat(buf1, " ");
        m = strlen(buf1);

        for (j = 0; j < m; j ++)
        {

            offset = buf1[j] * 6 + 16;
            lseek(fp, offset, SEEK_SET);
            read(fp, t9_tmp_buffer + n + j*6, 6);

            if (i == selected)  //���Ե�ǰ��ƴ��
            {
                for (k = 0; k < 6; k++)//6X8���󣬹�6�ֽ�
                    t9_tmp_buffer[n + j*6 + k] = ~t9_tmp_buffer[n + j*6+k];
            }
        }

        n += m * 6;
    }

    set_font(T9PY_HELP_FONT);
    window(1, line, 21, line + 1);
    putpixelcol(t9_tmp_buffer, 168);//6X8����21��

    close(fp);
}

//��ʾ��ѡ�����б�������ʾѡ�е��ַ�,lineΪ���ַ�ʽ������
static void T9PY_DispCharList(char * str, int selected, int line)
{
    int           fp, i, j, k;
    long          offset;
    char          font[30];
    BOOL bIsGbkFont16;

    j = strlen(str);
    j /= 2;

    if (IsTTFSupport())
    {
        MmiUtil_SetFontSize(t9_char_width);
        MmiUtil_ClearLine(line);
        MmiUtil_DisplayLine(str, line);
        invert_pixel_block(selected*t9_char_width, (line-1)*t9_char_height+HEADER_HEIGHT, (selected + 1)*t9_char_width, line*t9_char_height+HEADER_HEIGHT);

        return;
    }

    memset(font, 0, sizeof(font));
    //MmiUtil_GetFont(font);
//    strcpy(font, CHNFONT);
    bIsGbkFont16 = isGbkFont16();	//added by baijz 20130515 for GBK
    if (bIsGbkFont16)
    {
        strcpy(font, GBKFONT_16);
    }
    else
    {
        strcpy(font, CHNFONT);
    }
    fp = open(font, O_RDONLY);

    memset(t9_tmp_buffer, 0, sizeof(t9_tmp_buffer));

    for (i = 0; i < j; i++)
    {
        if (bIsGbkFont16)	//added by baijz 20130515 for GBK
        {
            offset = ((str[i*2] - 0x81) * 191 + str[i*2+1] - 0x40 + 64) * 32L + 16;
        }
        else
        {
            offset = ((str[i*2] - 0xa1) * 94 + str[i*2+1] - 0xa1 + 64) * 32L + 16;
        }
        lseek(fp, offset, SEEK_SET);
        read(fp, t9_tmp_buffer + i*32, 32);

        if (i == selected)
        {
            for (k = 0; k < 32; k++)
                t9_tmp_buffer[i*32+k] = ~t9_tmp_buffer[i*32+k];
        }
    }

    set_font(font);
    window(1, line, 16, line + 1);
    putpixelcol(t9_tmp_buffer, 256);//8X16�ĵ���

    close(fp);
}


// ��ʾ����ģʽ,lineΪӢ�ķ�ʽ������
void T9PY_DispMode(int line)
{
    char  buf[10];

    memset(buf, 0, sizeof(buf));

    switch (t9_cur_mode)
    {
    case T9PY_PY_MODE:
        strcpy(buf, "#-T9 ");
        break;
    case T9PY_ABC_MODE:
        strcpy(buf, "#-Abc");
        break;
    case T9PY_NUMBER_MODE:
        strcpy(buf, "#-123");
        break;
    case T9PY_SYMBOL_MODE:
        strcpy(buf, "#-Symbol");
        break;
    }

    if (IsTTFSupport())
    {
        MmiUtil_ClearLine(line);
        MmiUtil_DispChinese(buf, 5, line);
        return;
    }

    set_font(T9PY_HELP_FONT);
    write_at(" \001   \000   ", 8, 1, line);
    write_at(buf, 5, 9, line);
    write_at("   \002   \003", 8, 14, line);
}

int T9PY_Input(char * szOutPut, int iLen, usint usTimeout)
{
    int   ch = 0;
    char  t9[10];
    int   t9_len = 0, buf_len = 0, char_len = 0, research_flag = 0, ret = 0;
    char  tmp[100];
    int   current_char_page = 0, char_pages = 0, char_pointer = 0, i = 0;
    t9_symbol * t9_symbol_list = t9_symbol_list_1;
    displayInfo_t dispInfo;

    memset(&dispInfo, 0, sizeof(displayInfo_t));
    SVC_INFO_DISPLAY_EXT(&dispInfo);

    if (IsTTFSupport())
    {
        if (dispInfo.height == 240)
        {
            t9_char_width = 24;
            t9_char_height = 26;
        }
    }

    //LOG_PRINTF(("t9_char_width=[%d] t9_char_height=[%d]", t9_char_width, t9_char_height));

    if (t9_config_flg & T9PY_SYMBOL_FLG1)
    {
        //*�ż���#�ż������ڵ������
        t9_symbol_list = t9_symbol_list_2;
    }

    if ((t9_alpha_key == '*') || (t9_alpha_key == '#')
            || (t9_key_a == '*') || (t9_key_a == '#')
            || (t9_key_b == '*') || (t9_key_b == '#')
            || (t9_key_c == '*') || (t9_key_c == '#')
            || (t9_key_d == '*') || (t9_key_d == '#'))
    {
        //*�ż���#�ż������ܼ����Զ�תΪDISABLE
        t9_config_flg |= T9PY_SYMBOL_FLG1;
        t9_symbol_list = t9_symbol_list_2;
    }

    memset(tmp, 0, sizeof(tmp));
    memset(t9, 0, sizeof(t9));

    t9_cur_mode = T9PY_NORMAL_MODE;  //��ʼ������ģʽ

    while (1)
    {
        int ch_counter = 0;

        //��ʼ���������뷨
        if (t9_cur_mode != T9PY_PY_MODE)
        {
            t9_current_py_pointer = 0;
            current_char_page = 0;
            char_pages = 0;
            char_pointer = 0;
            t9_len = 0;
            t9_final_nominees = 0;
            memset(t9, 0, sizeof(t9));
        }

        if (t9_cur_mode != T9PY_ABC_MODE)
        {
            t9_abc_cur = 0;
            t9_abc_pos = 0;
        }

        if (t9_cur_mode != T9PY_SYMBOL_MODE)
        {
            t9_symbol_cur = 0;
            t9_symbol_pos = 0;
        }

        buf_len = strlen((char *)szOutPut);

        //��ʾ�Ѿ��������Ϣ
        i = 0;

        if (buf_len > 16)
        {
            //�ܳ��ȳ���16���ֽ�
            //����16���ַ���ͳ�ƺ����ֽ���
            for (i = (buf_len - 1); i >= (buf_len - 16); i--)
            {
                if (szOutPut[i] > 0xA0) ch_counter++;
            }

            //�������16���ַ��ĺ����ֽ�Ϊ���������һ���϶���ǰ��һ�����ֵĺ�һ���ֽ�
            if (ch_counter % 2)i += 2;//ȥ���������
            else i += 1;
        }

        if (IsTTFSupport())
        {
            MmiUtil_SetFontSize(t9_char_width);
        }
        else
            MmiUtil_SetFontSize(FONT_16X16);

        MmiUtil_ClearLine(t9_disp_line);
        MmiUtil_DisplayLine(&szOutPut[i], t9_disp_line);

        if (t9_cur_mode == T9PY_NORMAL_MODE)
        {
            MmiUtil_UserClr(t9_select_line);
        }

        research_flag = 1;
        //add lanpw 2009-11-07
        buf_len = strlen(szOutPut);

        if (buf_len > 100)
        {
            MmiUtil_Warning((char *)"�����ַ�������!");
            return 0;
        }

        ch = MmiUtil_WaitKey(usTimeout);

        switch (ch)
        {
        case 0:
            return INPUT_TIMEOUT;

        case bKEY1:
        case bKEY2:
        case bKEY3:
        case bKEY4:
        case bKEY5:
        case bKEY6:
        case bKEY7:
        case bKEY8:
        case bKEY9:

            if ((t9_cur_mode == T9PY_NORMAL_MODE) || (t9_cur_mode == T9PY_PY_MODE))
            {
                if (buf_len >= iLen - 1)
                {
                    MmiUtil_Beep();
                    break;
                }

                //����ģʽ�º�ƴ��ģʽ�¶���Ϊֱ������ƴ��
                if (t9_len == T9PY_MAX_PYLEN)
                {
                    //ƴ����ĸ�6�ֽ�
                    MmiUtil_Beep();
                    break;
                }

                t9[t9_len++] = ch;
                t9_cur_mode = T9PY_PY_MODE;
            }

            if (t9_cur_mode == T9PY_ABC_MODE)
            {
                if (buf_len >= iLen)
                {
                    MmiUtil_Beep();
                    break;
                }

                //ABCģʽ�£�����ָ���ַ��б�
                t9_abc_cur = ch;
                t9_abc_pos = 0;
            }

            if (t9_cur_mode == T9PY_NUMBER_MODE)
            {

                if (buf_len >= iLen)
                {
                    MmiUtil_Beep();
                    break;
                }

                //����ģʽ�£�ֱ����������
                szOutPut[buf_len] = ch;
                buf_len++;
            }

            if (t9_cur_mode == T9PY_SYMBOL_MODE)
            {

                if (buf_len >= iLen)
                {
                    MmiUtil_Beep();
                    break;
                }

                t9_symbol_cur = ch;
                t9_symbol_pos = 0;
            }

            break;

        case bKEY0:

            if (buf_len >= iLen)
            {
                MmiUtil_Beep();
                break;
            }

            research_flag = 0;

            if (t9_cur_mode == T9PY_NUMBER_MODE)
            {
                //����ģʽ�£�ֱ����������
                szOutPut[buf_len] = ch;
                buf_len++;
            }

            if (t9_cur_mode == T9PY_SYMBOL_MODE)
            {
                t9_symbol_cur = ch;
                t9_symbol_pos = 0;
            }

            break;

        case bBKSP:

            if (t9_cur_mode == T9PY_PY_MODE)  /* Backspace�ظ����ɾ�����һ��ƴ���ַ�*/
            {
                //�Ѿ�������ƴ��
                t9[--t9_len] = 0x00;
                char_pointer = 0;

                if (t9_len == 0)
                    t9_cur_mode = T9PY_NORMAL_MODE;//û��ƴ���򷵻���ͨģʽ
            }
            else if ((t9_cur_mode == T9PY_ABC_MODE))
            {
                //��ABCģʽ�£���ɾ��ABC�б���������������ABC
                if (t9_abc_cur)
                {
                    t9_abc_cur = 0;
                }
                else if (buf_len > 0)
                {
                    if (szOutPut[buf_len-1] < 0xA0)
                    {
                        //���һ���ֽڷǺ���
                        szOutPut[buf_len-1] = 0;
                        buf_len -= 1;
                    }
                    else
                    {
                        //���һ���ֽ��Ǻ���
                        buf_len -= 2;
                        szOutPut[buf_len+1] = 0;
                        szOutPut[buf_len] = 0;
                    }
                }

                t9_abc_pos = 0;
            }
            else if (t9_cur_mode == T9PY_SYMBOL_MODE)
            {
                if (t9_symbol_cur)
                {
                    t9_symbol_cur = 0;
                }
                else if (buf_len > 0)
                {
                    if (szOutPut[buf_len-1] < 0xA0)
                    {
                        //���һ���ֽڷǺ���
                        szOutPut[buf_len-1] = 0;
                        buf_len -= 1;
                    }
                    else
                    {
                        //���һ���ֽ��Ǻ���
                        buf_len -= 2;
                        szOutPut[buf_len+1] = 0;
                        szOutPut[buf_len] = 0;
                    }
                }

                t9_symbol_pos = 0;
            }
            else if ((t9_cur_mode == T9PY_NUMBER_MODE) || (t9_cur_mode == T9PY_NORMAL_MODE)) // Backspace�ظ����ɾ�����һ���ַ�
            {
                if (buf_len > 0)
                {
                    if (szOutPut[buf_len-1] < 0xA0)
                    {
                        //���һ���ֽڷǺ���
                        szOutPut[buf_len-1] = 0;
                        buf_len -= 1;
                    }
                    else
                    {
                        //���һ���ֽ��Ǻ���
                        buf_len -= 2;
                        szOutPut[buf_len+1] = 0;
                        szOutPut[buf_len] = 0;
                    }
                }
            }

            break;

        case bENTER:

            if (t9_cur_mode == T9PY_PY_MODE)  // ѡ��ĳ����
            {
                memcpy(szOutPut + buf_len, t9_pinyin_list[t9_current_py_pointer].py_table + current_char_page*16 + char_pointer*2, 2);
                buf_len += 2;
                t9_cur_mode = T9PY_NORMAL_MODE;
            }
            else if (t9_cur_mode == T9PY_ABC_MODE)
            {
                //�����������ѡ��ǰѡ����ַ�
                if (t9_abc_cur >= bKEY1 && t9_abc_cur <= bKEY9)
                {
                    //�������ַ�
                    szOutPut[buf_len] = t9_abc_list[t9_abc_cur-bKEY1][t9_abc_pos];
                    buf_len += 1;
                    //ѡ��֮����������
                    t9_abc_cur = 0;
                    t9_abc_pos = 0;
                }
                else
                {
                    //����ĸģʽ���ҷ�ѡ����ĸʱ����ENTERֱ�ӷ���
                    return buf_len;
                }
            }
            else if (t9_cur_mode == T9PY_SYMBOL_MODE)
            {
                //�����������ѡ��ǰѡ����ַ�
                if (t9_symbol_cur != 0)
                {
                    for (i = 0; i < 12; i++)
                    {
                        //�������ַ�,�����ַ�ѡ��
                        if (t9_symbol_list[i].Key == t9_symbol_cur)
                        {
                            szOutPut[buf_len] = t9_symbol_list[i].Symbol[t9_symbol_pos];
                            buf_len += 1;
                        }
                    }

                    //ѡ��֮����������
                    t9_symbol_cur = 0;
                    t9_symbol_pos = 0;
                }
                else
                {
                    //���ַ�ģʽ���ҷ�ѡ���ַ�ʱ����ENTERֱ�ӷ���
                    return strlen(szOutPut);
                }
            }
            else if ((t9_cur_mode == T9PY_NUMBER_MODE) || (t9_cur_mode == T9PY_NORMAL_MODE))  // ������ɷ���
            {
                //�����ֺ���ͨģʽ�°�ENTERֱ�ӷ���
                return strlen(szOutPut);
            }

            break;

        case bESC:  //ȡ�������˳�

            if (t9_cur_mode != T9PY_NORMAL_MODE)
            {
                //���еķ���������ģʽ��ֱ���л�������ģʽ
                t9_cur_mode = T9PY_NORMAL_MODE;
            }
            else
            {
                return INPUT_USERABORT;
            }

            break;

        default:
            //�������ܼ���������������������������������
            //��һ�����ֱȽ϶��ƴ����zha������ѡ�����һҳ��
            //Ȼ��һ���޹أ�����ĸ�͹��ܼ����İ�������ʱ�ĳ���������һ��ƴ����ϣ���ҳ��ȴû�иı�
            //�����ƴ���������ֱȽ��٣��ڴ���������º�����ʾ���롣
            research_flag = 0;//lanpw add 2009-11-07
            // ƴ���б�ָ���л�
            if (ch == t9_alpha_key)
            {
                if (t9_cur_mode == T9PY_PY_MODE)
                {
                    if (t9_current_py_pointer < t9_final_nominees - 1)
                        t9_current_py_pointer++;
                    else
                        t9_current_py_pointer = 0;

                    current_char_page = 0;
                    char_pointer = 0;
                    research_flag = 0;
                }
                else if (t9_cur_mode == T9PY_NORMAL_MODE)
                {
                    //�����л�����ĸ
                    if ((t9_config_flg & T9PY_ABC_FLG) == 0)
                        t9_cur_mode = T9PY_ABC_MODE;
                    else if ((t9_config_flg & T9PY_NUMBER_FLG) == 0)
                        t9_cur_mode = T9PY_NUMBER_MODE;
                    else if ((t9_config_flg & T9PY_SYMBOL_FLG2) == 0)
                        t9_cur_mode = T9PY_SYMBOL_MODE;
                }
                else if (t9_cur_mode == T9PY_ABC_MODE)
                {
                    //�����л�������
                    if (t9_abc_cur == 0)
                    {
                        //��ѡ��ʱ��Ч
                        if ((t9_config_flg & T9PY_NUMBER_FLG) == 0)
                            t9_cur_mode = T9PY_NUMBER_MODE;
                        else if ((t9_config_flg & T9PY_SYMBOL_FLG2) == 0)
                            t9_cur_mode = T9PY_SYMBOL_MODE;
                        else
                            t9_cur_mode = T9PY_NORMAL_MODE;
                    }
                }
                else if (t9_cur_mode == T9PY_NUMBER_MODE)
                {
                    //�����л�������ģʽ
                    if ((t9_config_flg & T9PY_SYMBOL_FLG2) == 0)
                        t9_cur_mode = T9PY_SYMBOL_MODE;
                    else
                        t9_cur_mode = T9PY_NORMAL_MODE;
                }
                else if (t9_cur_mode == T9PY_SYMBOL_MODE)
                {
                    //���뷵����������ģʽ
                    if (t9_symbol_cur == 0)
                        t9_cur_mode = T9PY_NORMAL_MODE;
                }
            }
            //���ַ�ҳ�� ƴ���б�ҳָ��
            else if (ch == t9_key_c)
            {
                if (t9_cur_mode == T9PY_PY_MODE)
                {
                    if (current_char_page != 0)
                        current_char_page--;

                    char_pointer = 0;
                    research_flag = 0;
                }
            }
            else if (ch == t9_key_d)
            {
                if (t9_cur_mode == T9PY_PY_MODE)
                {
                    if (current_char_page < char_pages - 1)
                        current_char_page++;

                    char_pointer = 0;
                    research_flag = 0;
                }
            }
            // ѡ���֣��ַ�ָ�� 0 ~ 7
            else if (ch == t9_key_a)
            {
                if (t9_cur_mode == T9PY_PY_MODE)
                {
                    if (char_pointer != 0)
                        char_pointer--;
                    else if (current_char_page != 0)
                    {
                        current_char_page--;
                        char_pointer = 7;
                    }

                    research_flag = 0;
                }

                if ((t9_cur_mode == T9PY_ABC_MODE) && (t9_abc_cur >= KEY1 && t9_abc_cur <= KEY9))
                {
                    //��ABCģʽ�£������ַ�����
                    if (t9_abc_pos > 0)
                        t9_abc_pos--;
                }

                if ((t9_cur_mode == T9PY_SYMBOL_MODE) && (t9_symbol_cur != 0))
                {
                    //��ABCģʽ�£������ַ�����
                    if (t9_symbol_pos > 0)
                        t9_symbol_pos--;
                }
            }
            else if (ch == t9_key_b)
            {
                if (t9_cur_mode == T9PY_PY_MODE)
                {
                    if (char_pointer < char_len - 1)
                        char_pointer++;
                    else if (current_char_page < char_pages - 1)
                    {
                        current_char_page++;
                        char_pointer = 0;
                    }

                    research_flag = 0;
                }

                if ((t9_cur_mode == T9PY_ABC_MODE) && (t9_abc_cur >= KEY1 && t9_abc_cur <= KEY9))
                {
                    //��ABCģʽ�£������ַ�����
                    if (t9_abc_pos < (strlen(t9_abc_list[t9_abc_cur-KEY1]) - 1))
                        t9_abc_pos++;
                }

                if ((t9_cur_mode == T9PY_SYMBOL_MODE) && (t9_symbol_cur != 0))
                {
                    //��ABCģʽ�£������ַ�����
                    int symbol_len = 0;

                    for (i = 0; i < 12; i++)
                    {
                        if (t9_symbol_list[i].Key == t9_symbol_cur)
                        {
                            symbol_len = strlen(t9_symbol_list[i].Symbol);
                            break;
                        }
                    }

                    if (t9_symbol_pos < (symbol_len - 1))
                        t9_symbol_pos++;
                }
            }
            else if ((ch == '*') || (ch == '#'))
            {

                //*�ż��������������룬�������ܼ�ʱ��ǰ��ͻ���˵�
                if ((t9_cur_mode == T9PY_SYMBOL_MODE) && ((t9_config_flg & T9PY_SYMBOL_FLG1) == 0))
                {
                    if (buf_len >= iLen)
                    {
                        MmiUtil_Beep();
                        break;
                    }

                    t9_symbol_cur = ch;
                    t9_symbol_pos = 0;
                }
            }

            break;
        }

        if (t9_cur_mode == T9PY_PY_MODE)
        {
            if (research_flag == 1) /* ABCD��ALPHA��������������*/
            {
                ret = T9PY_SearchPinYin(t9, t9_len);

                if (ret == 0)
                {
                    //û���ҵ��µ���ϣ���ʹ��֮ǰ��ƴ�����
                    t9[--t9_len] = 0x00;
                }
                else  // ���غ�ѡ�б�
                {
                    //ƴ����ϵĸ���
                    t9_final_nominees = ret;
                }

                //add lanpw 2009-11-07,ÿ������ƴ����Ҫ�ص�һ��ҳ����һ��
                t9_current_py_pointer = 0;
                current_char_page = 0;
            }

            char_pages = t9_pinyin_list[t9_current_py_pointer].chn_len / 16 + ((t9_pinyin_list[t9_current_py_pointer].chn_len % 16) ? 1 : 0);

            // ��ʾƴ���б�
            if (IsTTFSupport())
            {
                T9PY_DispPinYinList(t9_select_line + 1, t9_current_py_pointer);//ת��ΪӢ�ķ�ʽ������
                T9PY_DispMode(t9_select_line + 2);//ת��ΪӢ�ķ�ʽ������
            }
            else
            {
                T9PY_DispPinYinList((t9_select_line + 1)*2 - 1, t9_current_py_pointer);//ת��ΪӢ�ķ�ʽ������
                T9PY_DispMode((t9_select_line + 1)*2);//ת��ΪӢ�ķ�ʽ������
            }

            // ��ʾ�����б�
            memset(tmp, 0, sizeof(tmp));
            strncpy(tmp, t9_pinyin_list[t9_current_py_pointer].py_table + current_char_page * 16, 16);
            char_len = strlen(tmp) / 2;
            T9PY_DispCharList(tmp, char_pointer, t9_select_line);
        }

        if ((t9_cur_mode == T9PY_ABC_MODE))
        {
            if (t9_abc_cur >= bKEY1 && t9_abc_cur <= bKEY9)
            {
                //����������ʾѡ���б�
                T9PY_DispAbcList(t9_select_line);
            }
            else
            {
                //û������������б�
                MmiUtil_ClearLine(t9_select_line);
                MmiUtil_ClearLine(t9_select_line + 1);
            }
        }

        if ((t9_cur_mode == T9PY_SYMBOL_MODE))
        {
            if (t9_symbol_cur != 0)
            {
                //����������ʾѡ���б�
                T9PY_DispSymbolList(t9_select_line);
            }
            else
            {
                //û������������б�
                MmiUtil_ClearLine(t9_select_line);
                MmiUtil_ClearLine(t9_select_line + 1);
            }
        }

// ��ʾ������ʾ
        if (IsTTFSupport())
        {
            T9PY_DispMode(t9_select_line + 2);//ת��ΪӢ�ķ�ʽ������
        }
        else
        {
            T9PY_DispMode((t9_select_line + 1)*2);//ת��ΪӢ�ķ�ʽ������
        }

    }
}


/*
 * void T9PY_Init(int disp, int select, int _ALPHAKEY,
       int _KEYA, int _KEYB, int _KEYC, int _KEYD)
 * ��ʼ��T9���뷨�����ù��ܼ�����ʾλ��
 * disp -- ������ַ���Ҫ��ʾ����λ�á�Ϊ0��ʹ��Ĭ��2, ���ַ�ʽ������
 * select -- ���뷨��ʾ����ʾ����ʼ��λ��.Ϊ0��ʹ��Ĭ��3,���ַ�ʽ������
 * _ALPHAKEY -- ���뷨ƴ���л��������뷨�л���Ϊ0��ʹ��Ĭ��ALPHA_KEY,
 * _KEYA -- ���뷨����ѡ�����Ϊ0��ʹ��Ĭ��KEY_a
 * _KEYB -- ���뷨����ѡ�����Ϊ0��ʹ��Ĭ��KEY_b
 * _KEYC -- ���뷨���ַ�ҳ����Ϊ0��ʹ��Ĭ��KEY_c
 * _KEYD -- ���뷨���ַ�ҳ����Ϊ0��ʹ��Ĭ��KEY_d
 * _DISABLE -- ���뷨֧�����ã�
              Ϊ0Ĭ������ȫ֧�֣���֧�ֺ��֣�֧����ĸ��֧�����֣�֧���ַ���֧��*�ź�#��
        &1��֧�ַ������뵫������*�ź�#�ż�
        &2��֧�ַ�������
        &4��֧����ĸ����
        &8��֧����������
*/
void T9PY_Init(int disp, int select, int _ALPHAKEY,
               int _KEYA, int _KEYB, int _KEYC, int _KEYD, int _DISABLE)
{
    //ApplLog_Debug("ATOOL", "t9 line [%d] [%d]", t9_disp_line, t9_select_line);

    if (disp) t9_disp_line = disp;

    if (select) t9_select_line = select;

    if (_ALPHAKEY) t9_alpha_key = _ALPHAKEY;

    if (_KEYA)t9_key_a = _KEYA;

    if (_KEYB)t9_key_b = _KEYB;

    if (_KEYC)t9_key_c = _KEYC;

    if (_KEYC)t9_key_d = _KEYD;

    t9_config_flg = _DISABLE;

    //ApplLog_Debug("ATOOL", "t9 line [%d] [%d]", t9_disp_line, t9_select_line);

}

/*
void T9PY_DispTest(void)
{
    int max_len = 0;
    int i = 0;
    int offset = 0;
    int str_len = 0;

    max_len = sizeof(t9_pinyin_list) / sizeof(t9_py_index);
    for(i = 0; i < max_len; i++)
    {
        offset = 0;
        str_len = strlen(t9_pinyin_list[i].py_table);
        while(offset < str_len)
        {
            MmiUtil_Reset();
            MmiUtil_DisplayCenter((char *)"T9ȫ������ʾ����", LINE1, TRUE);
            if((str_len-offset)>0)
            {
                MmiUtil_DisplayLine(t9_pinyin_list[i].py_table+offset, LINE2);
                offset+=16;
            }
            if((str_len-offset)>0)
            {
                MmiUtil_DisplayLine(t9_pinyin_list[i].py_table+offset, LINE3);
                offset+=16;
            }
            if((str_len-offset)>0)
            {
                MmiUtil_DisplayLine(t9_pinyin_list[i].py_table+offset, LINE4);
                offset+=16;
            }
            MmiUtil_WaitKey(0);
        }
    }
}
*/
