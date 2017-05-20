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

char t9_tmp_buffer[2048];         //t9使用的临时缓冲
int t9_final_nominees;            //最终的拼音组合
int t9_cur_mode;              //当前输入法状态
int  t9_current_py_pointer;         //当前选择的拼音

#ifdef _SHARED_LIB
int t9_disp_line;           //汉字方式的行数
int t9_select_line;           //汉字方式的行数
int t9_alpha_key;         //拼音选择和输入方式切换键
int t9_key_a;           //字符选择键
int t9_key_b;           //字符选择键
int t9_key_c;           //翻页键
int t9_key_d;           //翻页键

int t9_abc_cur;           //当前的字符表ABCabc
int t9_abc_pos;           //当前选择的具体字符A

int t9_symbol_cur;          //当前的符号表.<>
int t9_symbol_pos;          //当前选择的具体符号"

int t9_config_flg;          //配置标志

t9_symbol t9_symbol_list_1[12]; //符号掩码表
t9_symbol t9_symbol_list_2[12]; //符号掩码表
char t9_abc_list[10][10];   //字符掩码表

int t9_char_width;          //行高
int t9_char_height;         //字体大小

#else
static int t9_disp_line = 2;            //汉字方式的行数
static int t9_select_line = 3;            //汉字方式的行数
static int t9_alpha_key = bALPHAKEY;        //拼音选择和输入方式切换键
static int t9_key_a = bKEYA;            //字符选择键
static int t9_key_b = bKEYB;            //字符选择键
static int t9_key_c = bKEYC;            //翻页键
static int t9_key_d = bKEYD;            //翻页键

static int t9_abc_cur = 0;              //当前的字符表ABCabc
static int t9_abc_pos = 0;              //当前选择的具体字符A

static int t9_symbol_cur = 0;           //当前的符号表.<>
static int t9_symbol_pos = 0;           //当前选择的具体符号"

static int t9_config_flg = 0;           //配置标志

static t9_symbol t9_symbol_list_1[12] = //符号掩码表
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

static t9_symbol t9_symbol_list_2[12] = //符号掩码表
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

static char t9_abc_list[10][10] = //字符掩码表
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

static int t9_char_width = 28;    //行高
static int t9_char_height = 32;   //字体大小

#endif

#ifdef _SHARED_LIB
void T9PY_InitPrm(void)
{
    //符号掩码表
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

    //符号掩码表
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

    //字符掩码表
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

    t9_disp_line = 2;           //汉字方式的行数
    t9_select_line = 3;           //汉字方式的行数
    t9_alpha_key = bALPHAKEY;       //拼音选择和输入方式切换键
    t9_key_a = bKEYA;           //字符选择键
    t9_key_b = bKEYB;           //字符选择键
    t9_key_c = bKEYC;           //翻页键
    t9_key_d = bKEYD;           //翻页键

    t9_abc_cur = 0;             //当前的字符表ABCabc
    t9_abc_pos = 0;             //当前选择的具体字符A

    t9_symbol_cur = 0;            //当前的符号表.<>
    t9_symbol_pos = 0;            //当前选择的具体符号"

    t9_config_flg = 0;            //配置标志

    t9_char_height = 32;          //行高
    t9_char_width = 28;           //字体大小

}
#endif

/* --------------------------------------------------------------------------
* FUNCTION NAME: PYLib_Search
* DESCRIPTION:  在拼音表中搜索拼音，并输出索引表
*               num -- 拼音对应的数字组合，如zhuang对应148264
*               len -- 拼音对应的数字组合的长度,如zhuang对应的6
*               py_nomination -- num对应的真正的拼音组合有的索引表
* RETURN:
* NOTES:        py_nomination必须足够大
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

    while (1) /*逐个搜索t9拼音查询表*/
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

        if (strlen(temp_t9.t9) > len) //表记录长度超过输入字符串长度，返回
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

// 显示ABCabc列表,line为中文方式的行数
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
        lseek(fp, buf1[j] * 16 + 16, SEEK_SET);//一个西文字符16个字节点阵
        read(fp, t9_tmp_buffer + j * 16, 16);

        if (j == t9_abc_pos)  //反显当前的字符
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

// 显示.$;列表,line为中文方式的行数
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
            //*号键和#号键不存在的情况下
            if (t9_symbol_list_2[j].Key == t9_symbol_cur)
                strncpy(buf1, t9_symbol_list_2[j].Symbol, 8);
        }
        else if (t9_symbol_list_1[j].Key == t9_symbol_cur)
        {
            //*号键和#号键存在的情况下
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
        lseek(fp, buf1[j] * 16 + 16, SEEK_SET);//一个西文字符16个字节点阵
        read(fp, t9_tmp_buffer + j * 16, 16);

        if (j == t9_symbol_pos)  //反显当前的字符
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

// 显示拼音列表,并反显选中的拼音,line为英文方式的行数
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

            if (i == selected)  //反显当前的拼音
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

            if (i == selected)  //反显当前的拼音
            {
                for (k = 0; k < 6; k++)//6X8点阵，共6字节
                    t9_tmp_buffer[n + j*6 + k] = ~t9_tmp_buffer[n + j*6+k];
            }
        }

        n += m * 6;
    }

    set_font(T9PY_HELP_FONT);
    window(1, line, 21, line + 1);
    putpixelcol(t9_tmp_buffer, 168);//6X8点阵、21个

    close(fp);
}

//显示备选汉字列表，反显显示选中的字符,line为汉字方式的行数
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
    putpixelcol(t9_tmp_buffer, 256);//8X16的点阵

    close(fp);
}


// 显示输入模式,line为英文方式的行数
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
        //*号键和#号键不存在的情况下
        t9_symbol_list = t9_symbol_list_2;
    }

    if ((t9_alpha_key == '*') || (t9_alpha_key == '#')
            || (t9_key_a == '*') || (t9_key_a == '#')
            || (t9_key_b == '*') || (t9_key_b == '#')
            || (t9_key_c == '*') || (t9_key_c == '#')
            || (t9_key_d == '*') || (t9_key_d == '#'))
    {
        //*号键和#号键做功能键则自动转为DISABLE
        t9_config_flg |= T9PY_SYMBOL_FLG1;
        t9_symbol_list = t9_symbol_list_2;
    }

    memset(tmp, 0, sizeof(tmp));
    memset(t9, 0, sizeof(t9));

    t9_cur_mode = T9PY_NORMAL_MODE;  //初始化输入模式

    while (1)
    {
        int ch_counter = 0;

        //初始化各个输入法
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

        //显示已经输入的信息
        i = 0;

        if (buf_len > 16)
        {
            //总长度超过16个字节
            //倒数16个字符，统计汉字字节数
            for (i = (buf_len - 1); i >= (buf_len - 16); i--)
            {
                if (szOutPut[i] > 0xA0) ch_counter++;
            }

            //如果倒数16个字符的汉字字节为奇数，则第一个肯定是前面一个汉字的后一个字节
            if (ch_counter % 2)i += 2;//去掉半个汉字
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
            MmiUtil_Warning((char *)"输入字符串超长!");
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

                //正常模式下和拼音模式下都认为直接输入拼音
                if (t9_len == T9PY_MAX_PYLEN)
                {
                    //拼音字母最长6字节
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

                //ABC模式下，重新指定字符列表
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

                //数字模式下，直接输入数字
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
                //数字模式下，直接输入数字
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

            if (t9_cur_mode == T9PY_PY_MODE)  /* Backspace回格键，删除最后一个拼音字符*/
            {
                //已经有输入拼音
                t9[--t9_len] = 0x00;
                char_pointer = 0;

                if (t9_len == 0)
                    t9_cur_mode = T9PY_NORMAL_MODE;//没有拼音则返回普通模式
            }
            else if ((t9_cur_mode == T9PY_ABC_MODE))
            {
                //在ABC模式下，按删除ABC列表，但可以重新输入ABC
                if (t9_abc_cur)
                {
                    t9_abc_cur = 0;
                }
                else if (buf_len > 0)
                {
                    if (szOutPut[buf_len-1] < 0xA0)
                    {
                        //最后一个字节非汉字
                        szOutPut[buf_len-1] = 0;
                        buf_len -= 1;
                    }
                    else
                    {
                        //最后一个字节是汉字
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
                        //最后一个字节非汉字
                        szOutPut[buf_len-1] = 0;
                        buf_len -= 1;
                    }
                    else
                    {
                        //最后一个字节是汉字
                        buf_len -= 2;
                        szOutPut[buf_len+1] = 0;
                        szOutPut[buf_len] = 0;
                    }
                }

                t9_symbol_pos = 0;
            }
            else if ((t9_cur_mode == T9PY_NUMBER_MODE) || (t9_cur_mode == T9PY_NORMAL_MODE)) // Backspace回格键，删除最后一个字符
            {
                if (buf_len > 0)
                {
                    if (szOutPut[buf_len-1] < 0xA0)
                    {
                        //最后一个字节非汉字
                        szOutPut[buf_len-1] = 0;
                        buf_len -= 1;
                    }
                    else
                    {
                        //最后一个字节是汉字
                        buf_len -= 2;
                        szOutPut[buf_len+1] = 0;
                        szOutPut[buf_len] = 0;
                    }
                }
            }

            break;

        case bENTER:

            if (t9_cur_mode == T9PY_PY_MODE)  // 选中某个字
            {
                memcpy(szOutPut + buf_len, t9_pinyin_list[t9_current_py_pointer].py_table + current_char_page*16 + char_pointer*2, 2);
                buf_len += 2;
                t9_cur_mode = T9PY_NORMAL_MODE;
            }
            else if (t9_cur_mode == T9PY_ABC_MODE)
            {
                //如果有输入则选择当前选择的字符
                if (t9_abc_cur >= bKEY1 && t9_abc_cur <= bKEY9)
                {
                    //有输入字符
                    szOutPut[buf_len] = t9_abc_list[t9_abc_cur-bKEY1][t9_abc_pos];
                    buf_len += 1;
                    //选择之后重新输入
                    t9_abc_cur = 0;
                    t9_abc_pos = 0;
                }
                else
                {
                    //在字母模式下且非选择字母时，按ENTER直接返回
                    return buf_len;
                }
            }
            else if (t9_cur_mode == T9PY_SYMBOL_MODE)
            {
                //如果有输入则选择当前选择的字符
                if (t9_symbol_cur != 0)
                {
                    for (i = 0; i < 12; i++)
                    {
                        //有输入字符,根据字符选择
                        if (t9_symbol_list[i].Key == t9_symbol_cur)
                        {
                            szOutPut[buf_len] = t9_symbol_list[i].Symbol[t9_symbol_pos];
                            buf_len += 1;
                        }
                    }

                    //选择之后重新输入
                    t9_symbol_cur = 0;
                    t9_symbol_pos = 0;
                }
                else
                {
                    //在字符模式下且非选择字符时，按ENTER直接返回
                    return strlen(szOutPut);
                }
            }
            else if ((t9_cur_mode == T9PY_NUMBER_MODE) || (t9_cur_mode == T9PY_NORMAL_MODE))  // 输入完成返回
            {
                //在数字和普通模式下按ENTER直接返回
                return strlen(szOutPut);
            }

            break;

        case bESC:  //取消键－退出

            if (t9_cur_mode != T9PY_NORMAL_MODE)
            {
                //所有的非正常输入模式，直接切换回正常模式
                t9_cur_mode = T9PY_NORMAL_MODE;
            }
            else
            {
                return INPUT_USERABORT;
            }

            break;

        default:
            //其他功能键或者垃圾按键都不用重新搜索，否则
            //在一个汉字比较多的拼音（zha）处，选择翻最后一页。
            //然后按一个无关（非字母和功能键）的按键，这时的程序跳到第一个拼音组合，但页码却没有改变
            //该组合拼音关联汉字比较少，内存溢出，导致汉字提示乱码。
            research_flag = 0;//lanpw add 2009-11-07
            // 拼音列表指针切换
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
                    //输入切换到字母
                    if ((t9_config_flg & T9PY_ABC_FLG) == 0)
                        t9_cur_mode = T9PY_ABC_MODE;
                    else if ((t9_config_flg & T9PY_NUMBER_FLG) == 0)
                        t9_cur_mode = T9PY_NUMBER_MODE;
                    else if ((t9_config_flg & T9PY_SYMBOL_FLG2) == 0)
                        t9_cur_mode = T9PY_SYMBOL_MODE;
                }
                else if (t9_cur_mode == T9PY_ABC_MODE)
                {
                    //输入切换到数字
                    if (t9_abc_cur == 0)
                    {
                        //非选择时有效
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
                    //输入切换到符号模式
                    if ((t9_config_flg & T9PY_SYMBOL_FLG2) == 0)
                        t9_cur_mode = T9PY_SYMBOL_MODE;
                    else
                        t9_cur_mode = T9PY_NORMAL_MODE;
                }
                else if (t9_cur_mode == T9PY_SYMBOL_MODE)
                {
                    //输入返回正常输入模式
                    if (t9_symbol_cur == 0)
                        t9_cur_mode = T9PY_NORMAL_MODE;
                }
            }
            //汉字翻页， 拼音列表页指针
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
            // 选择汉字，字符指针 0 ~ 7
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
                    //在ABC模式下，且有字符输入
                    if (t9_abc_pos > 0)
                        t9_abc_pos--;
                }

                if ((t9_cur_mode == T9PY_SYMBOL_MODE) && (t9_symbol_cur != 0))
                {
                    //在ABC模式下，且有字符输入
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
                    //在ABC模式下，且有字符输入
                    if (t9_abc_pos < (strlen(t9_abc_list[t9_abc_cur-KEY1]) - 1))
                        t9_abc_pos++;
                }

                if ((t9_cur_mode == T9PY_SYMBOL_MODE) && (t9_symbol_cur != 0))
                {
                    //在ABC模式下，且有字符输入
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

                //*号键可以做符号输入，当做功能键时，前面就会过滤掉
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
            if (research_flag == 1) /* ABCD和ALPHA键不再重新搜索*/
            {
                ret = T9PY_SearchPinYin(t9, t9_len);

                if (ret == 0)
                {
                    //没有找到新的组合，则使用之前的拼音组合
                    t9[--t9_len] = 0x00;
                }
                else  // 返回候选列表
                {
                    //拼音组合的个数
                    t9_final_nominees = ret;
                }

                //add lanpw 2009-11-07,每次搜索拼音都要回到一个页，第一个
                t9_current_py_pointer = 0;
                current_char_page = 0;
            }

            char_pages = t9_pinyin_list[t9_current_py_pointer].chn_len / 16 + ((t9_pinyin_list[t9_current_py_pointer].chn_len % 16) ? 1 : 0);

            // 显示拼音列表
            if (IsTTFSupport())
            {
                T9PY_DispPinYinList(t9_select_line + 1, t9_current_py_pointer);//转化为英文方式的行数
                T9PY_DispMode(t9_select_line + 2);//转化为英文方式的行数
            }
            else
            {
                T9PY_DispPinYinList((t9_select_line + 1)*2 - 1, t9_current_py_pointer);//转化为英文方式的行数
                T9PY_DispMode((t9_select_line + 1)*2);//转化为英文方式的行数
            }

            // 显示汉字列表
            memset(tmp, 0, sizeof(tmp));
            strncpy(tmp, t9_pinyin_list[t9_current_py_pointer].py_table + current_char_page * 16, 16);
            char_len = strlen(tmp) / 2;
            T9PY_DispCharList(tmp, char_pointer, t9_select_line);
        }

        if ((t9_cur_mode == T9PY_ABC_MODE))
        {
            if (t9_abc_cur >= bKEY1 && t9_abc_cur <= bKEY9)
            {
                //有输入则显示选择列表
                T9PY_DispAbcList(t9_select_line);
            }
            else
            {
                //没有输入则清除列表
                MmiUtil_ClearLine(t9_select_line);
                MmiUtil_ClearLine(t9_select_line + 1);
            }
        }

        if ((t9_cur_mode == T9PY_SYMBOL_MODE))
        {
            if (t9_symbol_cur != 0)
            {
                //有输入则显示选择列表
                T9PY_DispSymbolList(t9_select_line);
            }
            else
            {
                //没有输入则清除列表
                MmiUtil_ClearLine(t9_select_line);
                MmiUtil_ClearLine(t9_select_line + 1);
            }
        }

// 显示输入提示
        if (IsTTFSupport())
        {
            T9PY_DispMode(t9_select_line + 2);//转化为英文方式的行数
        }
        else
        {
            T9PY_DispMode((t9_select_line + 1)*2);//转化为英文方式的行数
        }

    }
}


/*
 * void T9PY_Init(int disp, int select, int _ALPHAKEY,
       int _KEYA, int _KEYB, int _KEYC, int _KEYD)
 * 初始化T9输入法，配置功能键和显示位置
 * disp -- 输入的字符所要显示的行位置。为0则使用默认2, 汉字方式的行数
 * select -- 输入法提示的显示的起始行位置.为0则使用默认3,汉字方式的行数
 * _ALPHAKEY -- 输入法拼音切换键和输入法切换，为0则使用默认ALPHA_KEY,
 * _KEYA -- 输入法汉字选择键，为0则使用默认KEY_a
 * _KEYB -- 输入法汉字选择键，为0则使用默认KEY_b
 * _KEYC -- 输入法汉字翻页键，为0则使用默认KEY_c
 * _KEYD -- 输入法汉字翻页键，为0则使用默认KEY_d
 * _DISABLE -- 输入法支持配置，
              为0默认配置全支持，即支持汉字，支持字母，支持数字，支持字符，支持*号和#号
        &1则支持符号输入但不存在*号和#号键
        &2则不支持符号输入
        &4则不支持字母输入
        &8则不支持数字输入
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
            MmiUtil_DisplayCenter((char *)"T9全汉字显示测试", LINE1, TRUE);
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
