#include "AppTools.h"
#include "jbig.h"

#define PRINT_WIDTH			384
#define LEFT_BLANK 			(PRINT_WIDTH-INNER_SIGN_WIDTH)/2

#define SIGN_COLOR 			0x000000FF
#define ERROR_START 		-99

int eSign_SetReSignTimes(unsigned char Rs232Dev,byte uReSignTimes,int timeout);
int eSign_SetByPassFlag(unsigned char Rs232Dev,boolean bByPassFlag,int timeout);
int eSign_SetPicZone(unsigned char Rs232Dev,int PicHeight,int PicWidth,int timeout);

static char ineSign_cComPort[ 16 ]= {"/DEV/COM1"};
static unsigned long total_length = 0;
static unsigned char esign_bitmap[INNER_SIGN_HEIGHT * INNER_SIGN_WIDTH / 8];

// 13 x 13 pixels for each code's bitmap, 0~9 + A~F = 16 codes, 13 pixel per line = 2 bytes per line
static unsigned char code_bitmap[13*2*16] =
{
    /*0*/
    0x00, 0x00, 0x06, 0x00, 0x09, 0x00, 0x10, 0x80, 0x10, 0x80, 0x10, 0x80, 0x10, 0x80, 0x10, 0x80, 0x10, 0x80, 0x10, 0x80, 0x09, 0x00, 0x06, 0x00, 0x00, 0x00,
    /*1*/
    0x00, 0x00, 0x02, 0x00, 0x0E, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x07, 0x00, 0x00, 0x00,
    /*2*/
    0x00, 0x00, 0x0E, 0x00, 0x11, 0x00, 0x10, 0x80, 0x00, 0x80, 0x00, 0x80, 0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0x08, 0x00, 0x10, 0x80, 0x1F, 0x80, 0x00, 0x00,
    /*3*/
    0x00, 0x00, 0x0F, 0x00, 0x10, 0x80, 0x00, 0x80, 0x01, 0x00, 0x02, 0x00, 0x07, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x01, 0x00, 0x1E, 0x00, 0x00, 0x00,
    /*4*/
    0x00, 0x00, 0x00, 0x80, 0x01, 0x80, 0x02, 0x80, 0x02, 0x80, 0x04, 0x80, 0x08, 0x80, 0x10, 0x80, 0x1F, 0xC0, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00,
    /*5*/
    0x00, 0x00, 0x07, 0x80, 0x08, 0x00, 0x08, 0x00, 0x1E, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x01, 0x00, 0x1E, 0x00, 0x00, 0x00,
    /*6*/
    0x00, 0x00, 0x03, 0x80, 0x04, 0x00, 0x08, 0x00, 0x08, 0x00, 0x16, 0x00, 0x19, 0x00, 0x10, 0x80, 0x10, 0x80, 0x10, 0x80, 0x09, 0x00, 0x06, 0x00, 0x00, 0x00,
    /*7*/
    0x00, 0x00, 0x0F, 0xC0, 0x10, 0x40, 0x00, 0x40, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00,
    /*8*/
    0x00, 0x00, 0x0F, 0x00, 0x10, 0x80, 0x10, 0x80, 0x10, 0x80, 0x09, 0x00, 0x06, 0x00, 0x09, 0x00, 0x10, 0x80, 0x10, 0x80, 0x10, 0x80, 0x0F, 0x00, 0x00, 0x00,
    /*9*/
    0x00, 0x00, 0x0E, 0x00, 0x09, 0x00, 0x10, 0x80, 0x10, 0x80, 0x10, 0x80, 0x09, 0x80, 0x06, 0x80, 0x01, 0x00, 0x01, 0x00, 0x06, 0x00, 0x18, 0x00, 0x00, 0x00,
    /*A*/
    0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x08, 0x80, 0x08, 0x80, 0x0F, 0x80, 0x10, 0x40, 0x10, 0x40, 0x78, 0xF0, 0x00, 0x00,
    /*B*/
    0x00, 0x00, 0x3F, 0x00, 0x08, 0x80, 0x08, 0x40, 0x08, 0x40, 0x08, 0x80, 0x0F, 0x80, 0x08, 0x40, 0x08, 0x20, 0x08, 0x20, 0x08, 0x40, 0x3F, 0x80, 0x00, 0x00,
    /*C*/
    0x00, 0x00, 0x07, 0xA0, 0x08, 0x60, 0x10, 0x20, 0x20, 0x20, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x10, 0x20, 0x08, 0x40, 0x07, 0x80, 0x00, 0x00,
    /*D*/
    0x00, 0x00, 0x3F, 0x80, 0x08, 0x40, 0x08, 0x20, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x20, 0x08, 0x40, 0x3F, 0x80, 0x00, 0x00,
    /*E*/
    0x00, 0x00, 0x3F, 0xC0, 0x08, 0x40, 0x08, 0x00, 0x08, 0x00, 0x08, 0x80, 0x0F, 0x80, 0x08, 0x80, 0x08, 0x00, 0x08, 0x20, 0x08, 0x40, 0x3F, 0xC0, 0x00, 0x00,
    /*F*/
    0x00, 0x00, 0x3F, 0xC0, 0x08, 0x40, 0x08, 0x00, 0x08, 0x00, 0x08, 0x80, 0x0F, 0x80, 0x08, 0x80, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x3E, 0x00, 0x00, 0x00
};

#define set_bitmap(x,y,n)	\
	do{\
		n=(((y)*INNER_SIGN_WIDTH)>>3)+((x)>>3);\
		if (n < sizeof(esign_bitmap))\
    		esign_bitmap[n] |= 1 << (8 - (x) % 8 - 1);\
	}while(0);\
 

/*
 * Read an ASCII integer number from file f and skip any PBM
 * comments which are encountered.
 */
int getint(FILE *f)
{
    int c;
    int i = 0;
    char buffer[10]= {0};

    while ((c = getc(f)) != EOF && !isdigit(c))
        if (c == '#')
            while ((c = getc(f)) != EOF && !(c == 13 || c == 10)) ;
    if (c != EOF)
    {
        ungetc(c, f);
        fscanf(f, "%s", buffer);
        i=atoi(buffer);
    }

    return i;
}

/*
 * Callback procedure which is used by JBIG encoder to deliver the
 * encoded data. It simply sends the bytes to the output file.
 */
static void data_out(unsigned char *start, size_t len, void *file)
{
    fwrite(start, len, 1, (FILE *) file);
    total_length += len;
    return;
}

/*
 * Remalloc a buffer and append a file f into its content.
 * If *buflen == 0, then malloc a buffer first.
 */
int read_file(unsigned char **buf, size_t *buflen, size_t *len, FILE *f)
{
    if (*buflen == 0)
    {
        *buflen = 4000;
        *len = 0;
        *buf = (unsigned char *) malloc(*buflen);

        if (!*buf)
        {
            //LOG_PRINTF_LIB("Sorry, not enough memory available!\n");
            return JBG_ENOMEM;
        }
    }

    do
    {
        *len += fread(*buf + *len, 1, *buflen - *len, f);

        if (*len == *buflen)
        {
            *buflen *= 2;
            *buf = (unsigned char *) realloc(*buf, *buflen);

            if (!*buf)
            {
                //LOG_PRINTF_LIB("Sorry, not enough memory available!\n");
                //exit(1);
                return JBG_ENOMEM;
            }
        }

        if (ferror(f))
        {
            //perror("Problem while reading input file");
            //exit(1);
            free(*buf);
            return JBG_EABORT;
        }
    }
    while (!feof(f));

    *buflen = *len;
    *buf = (unsigned char *) realloc(*buf, *buflen);

    if (!*buf)
    {
        //LOG_PRINTF_LIB("Oops, realloc failed when shrinking buffer!\n");
        // exit(1);
        return JBG_ENOMEM;
    }

    return JBG_EOK;
}

void eSign_SetComPort( char *pcComString )
{
    sprintf( ineSign_cComPort, "%s", pcComString );
}

void eSign_PutPixel(int x, int y, int color)
{
    int n;
    //dbprintf("---x:[%d],y:[%d]\n",x,y);

    //save the pixels into the bit map data
    write_pixels(x, y, x, y, color);
    set_bitmap(x, y, n);

    //在每个点的左/右边加一个点，使其有加粗的效果
    write_pixels(x+1,y,x+1,y,color);
    set_bitmap(x+1,y,n);
    write_pixels(x-1,y,x-1,y,color);
    set_bitmap(x-1,y,n);
    write_pixels(x,y+1,x,y+1,color);
    set_bitmap(x,y+1,n);
    write_pixels(x,y-1,x,y-1,color);
    set_bitmap(x,y-1,n);
}

void eSign_InitWindow(int *OrigColor, int NewColor)
{
    *OrigColor = get_display_color(FOREGROUND_COLOR);

    set_display_coordinate_mode(PIXEL_MODE);
    set_display_color(CURRENT_PALETTE, 1);
    set_display_color(FOREGROUND_COLOR, NewColor);

    draw_line(0, 117, 239, 117, 2, 0x000000F0);
    draw_line(0, 242, 239, 242, 2, 0x000000F0);
    window(0, 120, 239, 239);
}

void eSign_QuitWindow(int OrigColor)
{
    window(1, 1, 240, 320);
    set_display_coordinate_mode(CHARACTER_MODE);
    set_display_color(FOREGROUND_COLOR, OrigColor);
}

void eSign_EmbedCode(unsigned char* bitmap, int width, int height, char * code, int len)
{
    int x, y;
    int i, j, k;
    int offset = 0;
    char b;

    // top left position of the code display area
    x = (width - len * 13) / 2;
    y = (height - 13) / 2;

    for (i = 0; i < len; i++)
    {
        if ((code[i] >= '0') && (code[i] <= '9'))
            offset = code[i] - '0';
        else if ((code[i] >= 'A') && (code[i] <= 'F'))
            offset = code[i] - 'A' + 10;
        else if ((code[i] >= 'a') && (code[i] <= 'f'))
            offset = code[i] - 'a' + 10;

        offset = offset * 13 * 2;

        // 13 lines for each code, 2 bytes per line
        for (j = 0; j < 13; j++) //bitmap[(y*width/8) + x/8]
        {
            for (k = 0; k < 13; k++) // each bit for pixel
            {
                //dbprintf("i=%d, j=%d, x=%d, y=%d, bitmap[%d], code[%d]=%d", i, j, x, y, (y+j)*width/8 + x/8+i*2, offset+j*2, code_bitmap[offset+j*2]);
                b = code_bitmap[offset+j*2+k/8] << (k % 8);

                if (b >> 7)
                    bitmap[(y+j)*width/8 + (x+i*13+k)/8] |= 1 << (7 - (x + i * 13 + k) % 8);
            }
        }
    }
}

void eSign_ShowBitmap(unsigned char* bitmap, int width, int height, int color)
{
    int x, y;
    unsigned char b;

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {

            b = bitmap[(y*width/8) + x/8] << (x % 8);

            if (b >> 7)
            {
                //dbprintf("x[%d], y[%d], bitmap[%d] = [%d]", x, y, (y*width/8) + x/8, bitmap[(y*width/8) + x/8]);
                write_pixels(x, y, x, y, color);
            }
        }
    }
}

//函数功能：画一条彩色线
//x1,y1:线的起始点水平和垂直坐标
//x2,y2:线的结束点水平和垂直坐标
//color：彩色值
void eSign_DrawLine(int x1,int y1,int x2,int y2,int color)
{
    int dx,dy,e;
    dx=x2-x1;
    dy=y2-y1;

    if (dx>=0)//x2>=x1
    {
        if (dy >= 0) //y2>=y1
        {
            if (dx>=dy) // 1/8 octant
            {
                //x每次一步长,y则根据e的值确定是否行进一步长
                e=dy-(dx>>1);
                while (x1<=x2)
                {
                    eSign_PutPixel(x1,y1,color);

                    if (e>0)
                    {
                        y1+=1;
                        e-=dx;
                    }

                    x1+=1;
                    e+=dy;
                }
            }
            else    // dy>dx  2/8 octant
            {
                //y每次一步长,x则根据e的值确定是否行进一步长
                e=dx-(dy>>1);
                while (y1<=y2)
                {
                    eSign_PutPixel(x1,y1,color);
                    if (e>0)
                    {
                        x1+=1;
                        e-=dy;
                    }
                    y1+=1;
                    e+=dx;
                }
            }
        }//end if(dy >= 0)
        else// dy<0
        {
            dy=-dy;   // dy=abs(dy)

            if (dx>=dy) // 8/8 octant
            {
                e=dy-(dx>>1);
                while (x1<=x2)
                {
                    eSign_PutPixel(x1,y1,color);
                    if (e>0)
                    {
                        y1-=1;
                        e-=dx;
                    }
                    x1+=1;
                    e+=dy;
                }
            }
            else        //dy>dx  7/8 octant
            {
                e=dx-(dy>>1);
                while (y1>=y2)
                {
                    eSign_PutPixel(x1,y1,color);
                    if (e>0)
                    {
                        x1+=1;
                        e-=dy;
                    }
                    y1-=1;
                    e+=dx;
                }
            }
        }
    }//end if(dx>=0)
    else //dx<0
    {
        dx=-dx;     //dx=abs(dx)
        if (dy >= 0) // dy>=0
        {
            if (dx>=dy) // 4/8 octant
            {
                e=dy-(dx>>1);
                while (x1>=x2)
                {
                    eSign_PutPixel(x1,y1,color);
                    if (e>0)
                    {
                        y1+=1;
                        e-=dx;
                    }
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 3/8 octant
            {
                e=dx-(dy>>1);
                while (y1<=y2)
                {
                    eSign_PutPixel(x1,y1,color);
                    if (e>0)
                    {
                        x1-=1;
                        e-=dy;
                    }
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)

            if (dx>=dy) // 5/8 octant
            {
                e=dy-(dx>>1);
                while (x1>=x2)
                {
                    eSign_PutPixel(x1,y1,color);
                    if (e>0)
                    {
                        y1-=1;
                        e-=dx;
                    }
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 6/8 octant
            {
                e=dx-(dy>>1);
                while (y1>=y2)
                {
                    eSign_PutPixel(x1,y1,color);
                    if (e>0)
                    {
                        x1-=1;
                        e-=dy;
                    }
                    y1-=1;
                    e+=dx;
                }
            }
        }
    }
}

int eSign_Beeper(int freq, int t)
{
    long timeout;

    timeout = set_itimeout(-1, t, TM_SECONDS);

    while (read_ticks() < timeout)
    {
        SVC_WAIT(freq);
        beep(500);
    }

    return 0;
}

int eSign_GetPbm(const char *f_bitmap, unsigned char *bitmap, int bitmap_size)
{
    int fin;

    if (!f_bitmap)
    {
        return ESIGN_FILEFAIL;
    }

    fin = open(f_bitmap, O_RDONLY);

    if (fin <= 0)
    {
        //dbprintf("Can't open input file '%s", f_bitmap);
        return ESIGN_FILEFAIL;
    }

    /* PBM raw binary format */
    if (read(fin, (char *)bitmap, bitmap_size) != bitmap_size)
    {
        //dbprintf("Problem while reading input file '%s", f_bitmap);
        close(fin);
        return ESIGN_FILEFAIL;
    }

    close(fin);

    return ESIGN_OK;
}

int eSign_SavePbm(unsigned char *buf, int len, char* filename)
{
    int fPbm;
    File_Remove(filename);
    if ((fPbm = open(filename, O_CREAT | O_RDWR)) < 0)
    {
        return ESIGN_FILEFAIL;
    }

    if (write(fPbm, (char*)buf, len) != len)
    {
        close(fPbm);
        return ESIGN_FILEFAIL;
    }

    close(fPbm);
    return 0 ;
}

int eSign_InnerSign(char * code, int len, int timeout, char *f_bitmap, int ReSigntimes,boolean bypass)
{
    long end = 0;
    int ch, ret,pbm_len;
    int OrigColor;
    int RetFlag = FALSE;
    int times = 1;
    int x = 0, y = 0;
    int prev_x = -1, prev_y = -1;
    unsigned long NowTicks, PrevTicks;
    char pbm_buffer[INNER_SIGN_WIDTH*INNER_SIGN_HEIGHT/8 + 30] = {0};

    times = ReSigntimes;
    if ((MmiUtil_GetTerminalType()!=_VX820)&&
            (MmiUtil_GetTerminalType()!=_VX680)&&
            (MmiUtil_GetTerminalType()!=_VX680C))
    {
        MmiUtil_Warning("不支持内置签名");
        return ESIGN_FAIL;
    }
    if (ReSigntimes < 0)
    {
        MmiUtil_Warning("参数错误");
        return ESIGN_FAIL;
    }
    MmiUtil_Clr();
    /*设置显示字体,add by yunlong @ 2015.05.05*/
    MmiUtil_SetFontSize(FONT_28X28);

    MmiUtil_DisplayCenter("交易成功",1,FALSE);
    MmiUtil_DisplayCenter("请在下框内签名",2,FALSE);
    MmiUtil_DisplayCenter("-红键-黄键-绿键-", 8, FALSE);
    MmiUtil_DisplayCenter(" 取消 重签 确认 ", 9, FALSE);
    eSign_InitWindow(&OrigColor, 0x0000001F);

    memset(esign_bitmap, 0, INNER_SIGN_HEIGHT * INNER_SIGN_WIDTH / 8);
    eSign_EmbedCode(esign_bitmap, INNER_SIGN_WIDTH, INNER_SIGN_HEIGHT, code, len);
    eSign_ShowBitmap(esign_bitmap, INNER_SIGN_WIDTH, INNER_SIGN_HEIGHT, SIGN_COLOR);

    eSign_Beeper(500, 2);

    end = set_itimeout(-1, timeout, TM_SECONDS);

    PrevTicks = read_ticks();
    MmiUtil_flushkbd();
    while (1)
    {
        while (!CHK_TIMEOUT(-1, end))
        {
            eSign_QuitWindow(OrigColor);
            clrscr();
            return ESIGN_TIMEOUT;
        }

        if (KBHIT())
        {
            ch = get_char();

            if (ch == KEY_CANCEL)
            {
                eSign_QuitWindow(OrigColor);
                clrscr();
                return ESIGN_CANCEL;
            }
            else if (ch == KEY_BS)
            {
                //此处待优化,yunlong add @2014.08.13
                //1.循环处理.
                //2.提示信息可以用回调函数处理,由应用决定显示内容.
                if (TRUE == RetFlag)
                {
                    if (ReSigntimes == 0)
                    {
                        times = 2;
                    }
                    if (times)
                    {
                        RetFlag = FALSE;
                        eSign_QuitWindow(OrigColor);
                        if (times == 1)
                        {
                            clrscr();
                            MmiUtil_DisplayCenter("交易成功", 1, 0);
                            MmiUtil_DisplayCenter("请最后一次签名", 2, 0);
                            MmiUtil_DisplayCenter("[红键]取消", 8, FALSE);
                            MmiUtil_DisplayCenter("[绿键]确认", 9, FALSE);
                        }
                        eSign_InitWindow(&OrigColor, 0x0000001F);

                        normal_tone();
                        normal_tone();
                        clrscr();
                        memset(esign_bitmap, 0, INNER_SIGN_HEIGHT * INNER_SIGN_WIDTH / 8);
                        eSign_EmbedCode(esign_bitmap, INNER_SIGN_WIDTH, INNER_SIGN_HEIGHT, code, len);
                        eSign_ShowBitmap(esign_bitmap, INNER_SIGN_WIDTH, INNER_SIGN_HEIGHT, SIGN_COLOR);

                        prev_x = prev_y = -1;

                        times--;
                    }
                    else
                    {
                        window(1, 60, 240, 115);
                        clrscr();

                        eSign_QuitWindow(OrigColor);
                        MmiUtil_DisplayCenter("已不允许重新签名", 2, 0);
                        //MmiUtil_DisplayCenter("[X]取消  [O]确认", 9, FALSE);

                        eSign_InitWindow(&OrigColor, 0x0000001F);
                    }
                }
            }
            else if (ch == KEY_CR)
            {
                if (TRUE == RetFlag)
                {
                    //convert bitmap to pbm;
                    memset(pbm_buffer,0,sizeof(pbm_buffer));
                    sprintf(pbm_buffer,"%s\n%d %d\n","P4",INNER_SIGN_WIDTH,INNER_SIGN_HEIGHT);
                    pbm_len = strlen(pbm_buffer);
                    memcpy(pbm_buffer+pbm_len, esign_bitmap, sizeof(esign_bitmap));
                    pbm_len += sizeof(esign_bitmap);
                    ret = eSign_SavePbm((unsigned char*)pbm_buffer, pbm_len, f_bitmap);

                    if (ret != 0)
                        return ret;

                    eSign_QuitWindow(OrigColor);
                    clrscr();
                    return ESIGN_OK;
                }
                else if (TRUE == bypass)
                {
                    return ESIGN_FAIL;
                }
            }
        }

        if (get_touchscreen(&x, &y) != 0)
        {
            // ignore the outside dots
            if (x < 0 || x > (INNER_SIGN_WIDTH-1))			continue;
            if (y < 120 || y > 239)	continue;
            else y -= INNER_SIGN_HEIGHT;

            //dbprintf("x:[%d],y:[%d]\n",x,y);
            RetFlag = TRUE;

            NowTicks = read_ticks();
            if (NowTicks - PrevTicks > 100)
            {
                prev_x = prev_y = -1;
                PrevTicks = NowTicks;
            }

            if ((x != prev_x) || (y != prev_y))
            {
                if (prev_x > 0 && prev_y > 0)
                {
                    eSign_DrawLine(prev_x, prev_y, x, y, SIGN_COLOR);
                }
                else
                {
                    eSign_PutPixel(x, y, SIGN_COLOR);
                }

                prev_x = x;
                prev_y = y;
            }
        }
    }
}


//初始化串口
sint eSign_InitComm(unsigned char Rs232Dev)
{
    struct Opn_Blk com_parm;

    com_parm.rate = Rt_115200;
    com_parm.format = Fmt_A8N1;
    com_parm.protocol = P_char_mode;
    com_parm.parameter = 0;

    if (ComDev_Init( ineSign_cComPort, &com_parm) != COMDEV_OK)
    {
        //USB PP1000se check fail, then check USB PP1000seC
        if (strcmp(ineSign_cComPort, DEV_USBSER) == 0)
        {
            memset(ineSign_cComPort,0,sizeof(ineSign_cComPort));
            strcpy(ineSign_cComPort, DEV_COM6);
            if (ComDev_Init( ineSign_cComPort, &com_parm) != COMDEV_OK)
            {
                return ESIGN_FAIL;
            }
        }
        else
        {
            return ESIGN_FAIL;
        }
    }

    return ESIGN_OK;
}

//发送数据
int eSign_SendData(unsigned char Rs232Dev, unsigned char *Data, unsigned int DataLen)
{
    unsigned char *p, *s;
    unsigned char dbuf[SIGN_MAX_BUF_LEN];
    int len;


    memset(dbuf, 0, sizeof(dbuf));

    while (ComDev_Read(ineSign_cComPort, (char*)dbuf, 1) > 0);

    len = DataLen;
    memset(dbuf, 0, sizeof(dbuf));

    p = (byte*)dbuf;
    s = p;
    *s = 0x02;
    s ++ ;
    *s = (DataLen + 1) / 256;     //长度要包括ETX
    *(s + 1) = (DataLen + 1) % 256;
    s += 2;
    memcpy(s, Data, DataLen);
    s += DataLen;
    *s = 0x03;
    s ++ ;
    *s = BankUtil_GenLrc(p + 1, DataLen + 3);
    len = DataLen + 5;

    if (ComDev_Write( ineSign_cComPort, (char *)p, len) != len)
    {
        return ESIGN_FAIL;
    }

    return ESIGN_OK;
}

//接收数据
sint eSign_RecvData(unsigned char Rs232Dev, unsigned char *Data, unsigned int *DataLen, unsigned int Timeout)
#if 1
{
    int rv;
    int data_len;
    int iOffset;
    long tstart;
    unsigned char rbuf[SIGN_MAX_BUF_LEN];

    tstart = read_ticks();
    memset( rbuf, 0, sizeof( rbuf ) );
    *DataLen = 0;

    for ( ; ; )
    {
        if ( read_ticks() - tstart > Timeout * 1000 )
        {
            return ESIGN_TIMEOUT;
        }

        rv = ComDev_Read( ineSign_cComPort, (char *)rbuf, 1 );
        if ( rv == 1 && rbuf[ 0 ] == 0x02 )
        {
            SVC_WAIT( 50 );

            rv = ComDev_Read( ineSign_cComPort, (char *)rbuf + 1, 2 );
            if ( rv != 2 )
            {
                return ESIGN_TIMEOUT;
            }
            data_len = rbuf[ 1 ] * 256 + rbuf[ 2 ];
            if ( data_len > sizeof( rbuf ) )
                data_len = sizeof( rbuf );

            iOffset = 0;
            while ( read_ticks() - tstart < Timeout * 1000 )
            {
                rv = ComDev_Read( ineSign_cComPort, (char *)rbuf + 3 + iOffset, data_len - iOffset );
                if ( rv > 0 )
                    iOffset += rv;

                if ( iOffset >= data_len )
                    break;

                SVC_WAIT( 50 );
            }

            if ( iOffset < data_len )
            {
                return ESIGN_TIMEOUT;
            }

            //心跳包
            if (rbuf[ 3 ] == 0xD1 )
            {
                memset( rbuf, 0, sizeof( rbuf ) );
                tstart = read_ticks();
                continue;
            }

            *DataLen = data_len;
            memcpy( Data, rbuf + 3, data_len );

            // clear com buffer
            ComDev_Read( ineSign_cComPort, (char *)rbuf, sizeof( rbuf ) );

            return ESIGN_OK;
        }
    }
}
#else
{
    int    i,  stxflag = 0, lenflag = 0, offset = 0, rv, detect_flag = FALSE;
    long    tstart, tnow, tout;
    char    rbuf[SIGN_MAX_BUF_LEN], s;
    int     len,data_len;

    //但愿不会溢出...
    Timeout = Timeout * 1000;
    tout = Timeout;
    memset(rbuf, 0, sizeof(rbuf));

    tstart = read_ticks();
    len = data_len = 0;
    for (; ;)
    {
        tnow = read_ticks();

        if ((tnow - tstart) > tout)
        {
            return ESIGN_TIMEOUT;
        }

        len = SIGN_MAX_BUF_LEN - len;

        rv = ComDev_Read(ineSign_cComPort, rbuf + offset, len);

        if (rv < 0)
        {
            return ESIGN_FAIL;
        }
        else if (rv == 0)
        {
            //只有签名命令才有心跳包.
            //其他指令的超时时间均不超过2秒,
            //如果2秒内没有心跳报文,或者正确的指令,
            //则返回失败.
            if ((tnow - tstart) > 5000)
                return ESIGN_FAIL;
            else
                continue;
        }

        offset += rv;

        // STX arrived
        if (!stxflag)
        {
            for (i = 0; i < len; i++)
            {
                if (rbuf[i] == 0x02)
                {
                    //delete the leading messy chars
                    memcpy(rbuf, rbuf + i, (offset - i));
                    offset -= i;
                    stxflag = 1;
                    break;
                }

            }

            if (!stxflag)
            {
                continue;
            }
        }

        // two len char arrived
        if (!lenflag)
        {
            if (offset > 2)
            {
                lenflag = 1;
                data_len = rbuf[1] * 256 + rbuf[2];
                data_len--;   //len1已经包括了ETX

                if (data_len > *DataLen)
                {
                    return ESIGN_FAIL;
                }

                //*DataLen = data_len;

            }
            else
                continue;
        }

        //心跳包
        if (rbuf[3] == 0xD1)
        {
            detect_flag = TRUE;
            tout = 1000;
        }

        // all packet arrived
        if (offset >= (data_len + 5))
        {
            //接收完整心跳包后,清空数据和标志位,重新校准timeout,
            if (detect_flag)
            {
                Timeout -= (tstart - tnow);
                tstart = tnow;
                tout = Timeout;
                memset(rbuf,0,sizeof(rbuf));
                data_len = lenflag = stxflag = 0;
                detect_flag = FALSE;
                continue;
            }
            else
            {
                *DataLen = data_len;
                memcpy(Data, rbuf + 3, *DataLen);
                //不做LRC校验
                s = BankUtil_GenLrc((byte *)rbuf + 1, *DataLen + 3);
                if (s == rbuf[*DataLen + 4])
                {
                    return ESIGN_OK;
                }
                else
                {
                    return ESIGN_OK; //不做LRC校验
                }
            }
        }//end 接收完整数据
    }//end for

}
#endif

//握手
int eSign_HandShake(unsigned char Rs232Dev, unsigned int TimeOut, unsigned char *State)
{
    unsigned char    buf[SIGN_MAX_BUF_LEN];
    unsigned int  len;
    int ret;

    if (State == NULL)
    {
        return ESIGN_FAIL;
    }

    ret = eSign_InitComm(Rs232Dev);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_HANDSHAKE_REQ;
    len = 1;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    ret = eSign_RecvData(Rs232Dev, buf, &len, TimeOut);

    if (ret != ESIGN_OK)
    {

        return ret;
    }

    if (buf[0] != SIGN_HANDSHAKE_RES)
    {
        return ESIGN_FAIL;
    }

    *State = buf[1];
    return ESIGN_OK;
}

/*前一笔签字上传状态通知
* 输入参数：
*       Rs232Dev    串口号
*       SeqNum          电子签字编号
*       UploadStatus    前一笔交易上传状态
*       TimeOut     超时
*
* 输出参数：
*       DealStatus->电子签名板应答状态
*
*/
int eSign_InformUploadStatus(unsigned char Rs232Dev, unsigned int SeqNum,
                             unsigned char UploadStatus,  unsigned int TimeOut, unsigned char *DealStatus)
{
    unsigned char    buf[SIGN_MAX_BUF_LEN];
    unsigned int  len;
    int ret;

    if (DealStatus == NULL)
    {
        return ESIGN_FAIL;
    }

    ret = eSign_InitComm(Rs232Dev);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_INFORM_UPLOAD_STATUS_REQ;
    buf[1] = ((SeqNum >> 16) & 0xFF);
    buf[2] = ((SeqNum >> 8) & 0xFF);
    buf[3] = (SeqNum & 0xFF);
    buf[4] = UploadStatus;
    len = 5;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    ret = eSign_RecvData(Rs232Dev, buf, &len, TimeOut);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    if (buf[0] != SIGN_INFORM_UPLOAD_STATUS_RES)
    {
        return ESIGN_FAIL;
    }

    *DealStatus = buf[1];
    return ESIGN_OK;
}

/*签名开始
输入参数：
*       Rs232Dev    串口号
*       ConditionCode   交易特征码
*       TimeOut     超时
*
* 输出参数：
*       SeqNum   电子签字编号
*       Data     电子签字数据
*       DataLen  电子签字数据长度
*/
int eSign_SignatureBegin(unsigned char Rs232Dev, unsigned char *ConditionCode, unsigned int TimeOut,
                         unsigned int *SeqNum, unsigned char *Data, unsigned int *DataLen)
{
    unsigned char buf[SIGN_MAX_BUF_LEN];
    unsigned int len;
    int   ret;

    if ((SeqNum == NULL) || (DataLen == NULL)
            || (Data == NULL))
    {
        return ESIGN_FAIL;
    }

    ret = eSign_InitComm(Rs232Dev);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_SIGNATURE_BEGIN_REQ;
    memcpy(buf + 1, ConditionCode, 8);
    len = 9;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    ret = eSign_RecvData(Rs232Dev, buf, &len, TimeOut);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    if (buf[0] != SIGN_SIGNATURE_BEGIN_RES_OK)  //签字失败
    {
        return ESIGN_FAIL;
    }

    *SeqNum = (buf[1] << 16) + (buf[2] << 8) + (buf[3]);
    *DataLen = len - 4;
    memcpy(Data, buf + 4, *DataLen);

    return ESIGN_OK;
}

/*
* 电子签字结束
* 输入参数：
*       Rs232Dev    串口号
*       TimeOut     超时
*
* 输出参数：无
*/
int eSign_SignatureEnd(unsigned char Rs232Dev, unsigned int TimeOut)
{
    unsigned char    buf[SIGN_MAX_BUF_LEN];
    unsigned int len;
    int ret;

    ret = eSign_InitComm(Rs232Dev);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_SIGNATURE_END_REQ;
    len = 1;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    ret = eSign_RecvData(Rs232Dev, buf, &len, TimeOut);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    if (buf[0] != SIGN_SIGNATURE_END_RES)
    {
        return ESIGN_FAIL;
    }

    return ESIGN_OK;
}

/*批量上传失败签名
*
* 输入参数：
*       Rs232Dev    串口号
*       TimeOut     超时
*
* 输出参数：
*   SigninfNum    0xF1: 不支持存储功能其他0x00-0xFF：（《=100） 表示存储在电子签字板剩余签字总数（即未删除的），包含此条正在上传的记录
*       SeqNum      电子签字编号
*       DataLen     电子签字信息长度
*       Data      电子签字信息
*       Flag        是否还有下一笔标志 0无 1有
*/
int eSign_BatchUploadBegin(unsigned char Rs232Dev, unsigned char *SigninfNum, unsigned int *SeqNum,
                           unsigned int TimeOut, unsigned char *Data, unsigned int *DataLen)
{
    unsigned char    buf[SIGN_MAX_BUF_LEN];
    unsigned int len;
    int   ret;

    if ((SigninfNum == NULL) || (DataLen == NULL)
            || (Data == NULL))
    {
        return ESIGN_FAIL;
    }

    ret = eSign_InitComm(Rs232Dev);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_BATCH_UPLOAD_BEGIN_REQ;
    len = 1;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    ret = eSign_RecvData(Rs232Dev, buf, &len, TimeOut);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    if (Data[0] != SIGN_BATCH_UPLOAD_BEGIN_RES)
    {
        return ESIGN_FAIL;
    }

    *SigninfNum = buf[1];
    *SeqNum = (buf[2] << 16) + (buf[3] << 8) + (buf[4]);
    *DataLen = len - 5;
    memcpy(Data, buf + 5, *DataLen);

    return ESIGN_OK;
}


/*批量上传失败签名结束
* 输入参数：
*       Rs232Dev  串口号
*       TimeOut   超时
*
* 输出参数：无
*/
int eSign_BatchUploadEnd(unsigned char Rs232Dev, unsigned int TimeOut)
{
    unsigned char    buf[SIGN_MAX_BUF_LEN];
    unsigned int  len;
    int ret;

    ret = eSign_InitComm(Rs232Dev);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_BATCH_UPLOAD_END_REQ;
    len = 1;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    ret = eSign_RecvData(Rs232Dev, buf, &len, TimeOut);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    if (buf[0] != SIGN_BATCH_UPLOAD_END_RES)
    {
        return ESIGN_FAIL;
    }

    return ESIGN_OK;
}


/*接收批量上传失败签字成功
* 输入参数：
*       Rs232Dev  串口号
*
* 输出参数：无
*/
int eSign_BatchUploadRecvOk(unsigned char Rs232Dev, unsigned int SeqNum)
{
    unsigned char    buf[SIGN_MAX_BUF_LEN];
    unsigned int  len;
    int ret;

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_BATCH_UPLOAD_RECV_OK;
    buf[1] = ((SeqNum >> 16) & 0xFF);
    buf[2] = ((SeqNum >> 8) & 0xFF);
    buf[3] = (SeqNum & 0xFF);
    len = 4;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    return ESIGN_OK;
}

/*接收批量上传失败签字失败
* 输入参数：
*       Rs232Dev  串口号
*
* 输出参数：无
*/
int eSign_BatchUploadRecvErr(unsigned char Rs232Dev, unsigned int SeqNum)
{
    unsigned char    buf[SIGN_MAX_BUF_LEN];
    unsigned int  len;
    int ret;

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_BATCH_UPLOAD_RECV_ERR;
    buf[1] = ((SeqNum >> 16) & 0xFF);
    buf[2] = ((SeqNum >> 8) & 0xFF);
    buf[3] = (SeqNum & 0xFF);
    len = 4;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    return ESIGN_OK;
}

int eSign_ConvertBMPtoPBM(char *in, unsigned int in_len, char*out, unsigned int *out_len)
{
    int i,j;
    char *linedata;
    int padding, m_size;
    int ofs, out_ofs, width, height;
    unsigned int widthActual = 0;
    unsigned short BitsPerPixel = 0;
    char fileheader[14] = {0};
    char infoheader[48] = {0};

    ofs =0;
    memcpy(fileheader, in, sizeof(fileheader));
    ofs += sizeof(fileheader);
    memcpy(infoheader, in+ofs, sizeof(infoheader));
    ofs += sizeof(infoheader);

    //**************************************************************************************
    //  Check the header for valid bitmap (only monochrome and its dimensions(Width x Hieght)

    // BITMAPFILEHEADER [14 bytes]
    // WORD    bfType;
    // DWORD   bfSize;
    // WORD    bfReserved1;
    // WORD    bfReserved2;
    // DWORD   bfOffBits;

    // BITMAPINFOHEADER [48 bytes]
    // DWORD  Size;             /* Size of this structure in bytes */
    // DWORD  Width;            /* Bitmap width in pixels */
    // DWORD  Height;           /* Bitmap height in pixel */
    // WORD   NumPlanes;        /* Number of bit planes (color depth) */
    // WORD   BitsPerPixel;     /* Number of bits per pixel per plane */
    // DWORD  Compression;      /* Bitmap compression scheme */
    // DWORD  ImageDataSize;    /* Size of bitmap data in bytes */
    // DWORD  XResolution;      /* X resolution of display device */
    // DWORD  YResolution;      /* Y resolution of display device */
    // DWORD  ColorsUsed;       /* Number of color table indices used */
    // DWORD  ColorsImportant;  /* Number of important color indices */
    //***************************************************************************************
    //Read the 2 bytes of the bits per pixel data
    BitsPerPixel = infoheader[15] << 8 | infoheader[14] << 0;

    if (BitsPerPixel != 1)
    {
        return ERROR_START-1;
    }

    // width is 4 bytes
    width = infoheader[7] << 24 | infoheader[6] << 16 | infoheader[5] << 8 | infoheader[4] << 0;
    // width is 4 bytes
    height = infoheader[11] << 24 | infoheader[10] << 16 | infoheader[9] << 8 | infoheader[8] << 0;

    if ((width || height) <= 0)
    {
        return ERROR_START-2;
    }
    //**************************************************************************************
    //  get the width since each bit represnts a pixel we need to get the actual size
    //  widthactual=width/8
    //  mod is found to match the correct exact pixel to be printed
    //  35%8=3 ie the last byte should only print 3 bits
    //**************************************************************************************

    if ((width % 8) > 0)
    {
        widthActual = (width / 8) + 1;
    }
    else
    {
        widthActual = (width / 8);
    }
    //**************************************************************************************
    //The bitmap will store the data in DWORD format ie 4 bytes of data
    // suppose the last width of the container stores only data of 1 byte required
    // then the 3 bytes of data has to be ommited while retriving the data
    // hence the padding value is found to omit the data while reading
    //**************************************************************************************

    padding = 4 - (widthActual % 4);

    if (padding == 4)
        padding = 0;
    //**************************************************************************************
    // Add pbm header and calculate the offset.
    //**************************************************************************************
    sprintf(out, "P4\n%d %d\n",width,height);
    out_ofs = strlen(out);

    //**************************************************************************************
    //Get the actual size of the bitmap to stored
    //Create the dynamic buffers for Storing the bitmap data and converting it
    //
    //**************************************************************************************
    m_size = height * widthActual;
    if ((m_size + out_ofs) > *out_len)
    {
        return ERROR_START-3;
    }
    linedata = (char *)malloc(widthActual);

    if (linedata == NULL)
    {
        return ERROR_START-4;
    }

    //**************************************************************************************
    //Read the complete bitmap data ommiting the unrequired data at the end of the width container
    //Also the data needs to the reversed ie OR 'ING
    //**************************************************************************************

#if 1

    for (i = height-1; i >=0 ; i--)
    {
        memset(linedata, 0, widthActual);
        //Reac the complete width required
        memcpy(linedata, in+ofs, widthActual);
        ofs += widthActual;
        //get each byte to the required pixel
        for (j = 0; j < widthActual; ++j)
        {
            out[out_ofs + (i*widthActual)+j] = linedata[j];

        }
        //omit the unrequired data at the the end
        ofs += padding;
    }
#else
    for (i=0; i<height; ++i)
    {
        memset(linedata,0,widthActual);
        //Reac the complete width required
        memcpy(linedata, in+ofs, widthActual);
        //get each byte to the required pixel
        for (j=0; j<widthActual; ++j)
        {
            out[out_ofs+(i*widthActual)+j] = ~linedata[j];
        }
        //omit the unrequired data at the the end
        ofs += padding;
        //ofs
        ofs += widthActual;
    }
#endif
    *out_len = m_size + out_ofs;
    free(linedata);
    return ESIGN_OK;

}

int eSign_ExternalSign(char * code, int len, int timeout, char *f_bitmap,int times,boolean bypass)
{
    int ret;
    unsigned int SeqNum;
    byte State;
    unsigned int  bmp_datalen, pbm_datalen;
    byte bmp_data[SIGN_MAX_BUF_LEN] = {0};
    byte pbm_data[SIGN_MAX_BUF_LEN] = {0};

    ret = eSign_HandShake(1, 3, &State);
    if (ret != ESIGN_OK)
    {
        //MmiUtil_Warning3("签名板通迅失败", 5);
        return ret;
    }

    ret = eSign_SetReSignTimes(1,times,2);
    if (ret != ESIGN_OK)
    {
        return ret;
    }
    ret = eSign_SetByPassFlag(1,bypass,2);
    if (ret != ESIGN_OK)
    {
        return ret;
    }
    ret = eSign_SetPicZone(1,80,240,2);
    if (ret != ESIGN_OK)
    {
        return ret;
    }


    MmiUtil_UserClr(LINE2);
    MmiUtil_UserClr(LINE3);
    MmiUtil_DisplayLine("交易成功", LINE2);
    MmiUtil_DisplayLine("请在签名板签字", LINE3);

    bmp_datalen = sizeof(bmp_data);
    memset(bmp_data, 0, sizeof(bmp_data));

    ret = eSign_SignatureBegin(1, (unsigned char *)code, timeout, &SeqNum, bmp_data, &bmp_datalen);
    if (ret != ESIGN_OK)
    {
        //根据规范,异常时应发送签字结束请求
        eSign_SignatureEnd(1, 2);
        return ret;
    }
    File_Remove(f_bitmap);

    State = 0;
    if ( memcmp( bmp_data, "VX820-ESIGN", 11 ) == 0 )
    {
        State = 1;
        pbm_datalen = bmp_datalen - 11;
        memcpy( pbm_data, bmp_data + 11, bmp_datalen );
    }
    else
    {
        //convert bmp to pbm.
        memset(pbm_data,0,sizeof(pbm_data));
        pbm_datalen = sizeof(pbm_data);
        ret = eSign_ConvertBMPtoPBM((char *)bmp_data, bmp_datalen,(char *)pbm_data, &pbm_datalen);
        if (ret != ESIGN_OK)
        {
            eSign_SignatureEnd(1, 2);
            return ret;
        }
    }

    ret = eSign_SavePbm( pbm_data, pbm_datalen, f_bitmap);
    if (ret != ESIGN_OK)
    {
        eSign_SignatureEnd(1, 2);
        return ret;
    }

    ret = eSign_SignatureEnd(1, 2);
    if (ret != ESIGN_OK)
    {
        if ( State == 0 )
        {
            return ret;
        }
    }
    return ESIGN_OK;
}

int eSign_Signature(char * code, int len, int timeout, char signpad_type, char *f_bitmap, int times,boolean bypass)
{
    int ret;
    if (INNER_SIGNPAD == signpad_type)
    {
        ret = eSign_InnerSign(code,len,timeout,f_bitmap,times,bypass);
    }
    else if (EXTERNAL_SIGNPAD == signpad_type)
    {
        ret = eSign_ExternalSign(code,len,timeout,f_bitmap,times,bypass);
    }
    else
    {
        ret = ESIGN_FAIL;
    }
    return ret;
}

//extern int dbprintf(const char* fmt,...);

int eSign_PrintPBMImage(const char *f_bitmap, char cPosFlag)
{
    FILE *fin;
    char type;
    int h_printer;
    short bits = 0;
    short heightIndex = 0;
    unsigned char *bitmap = NULL;
    char ioctl_status[4];
    char Printmode[4] = {0};
    int task_id, local_taskid,bitcount, bitset;
    int c, i,j;
    int width, height, bitmap_size,retVal;
    int widthActual, mod, offset = 0, ReqSize,offsetwidth;
    unsigned char *bitmapConv;
    unsigned char DataBuf[1] = {0};
    unsigned char TransBuf[1] = {0};
    unsigned int counter[8] = {1, 2, 4, 8, 16, 32, 64, 128};

    static int count = 0;

    local_taskid = get_task_id();
    h_printer = get_owner(DEV_COM4, &task_id);
    if (h_printer <= 0 || task_id != local_taskid)
    {
        return PRINT_NOHANDLE;
    }

    fin = fopen(f_bitmap, "rb");
    if (!fin)
    {
        return PRINT_FAIL;
    }
    /* read PBM header */
    while ((c = getc(fin)) != EOF && (isspace(c) || c == '#'))
        if (c == '#')
            while ((c = getc(fin)) != EOF && !(c == 13 || c == 10)) ;
    if (c != 'P')
    {
        return PRINT_FAIL;
    }
    /* get PBM width height*/
    type = fgetc(fin); // type = '4'
    width = getint(fin);
    height = getint(fin);
    //dbprintf("width:%d, height:%d",width, height);
    fgetc(fin);    	/* skip line feed */
    /* read PBM data */
    if (type == '4')
    {
        if ((width % 8) > 0)
        {
            mod = width % 8;
            widthActual = (width / 8) + 1;
        }
        else
        {
            widthActual = (width / 8);
            mod = 7;
        }
        bitmap_size = widthActual * height;
        bitmap = (unsigned char *) malloc(bitmap_size);
        if (NULL == bitmap)
        {
            return PRINT_FAIL;
        }
        fread(bitmap, bitmap_size, 1, fin);
    }
    else
    {
        fclose(fin);
        free(bitmap);
        return PRINT_FAIL;
    }
    fclose(fin);

    if ((384 - 12 - (widthActual * 8)) < 0)
    {
        offset =0;
    }
    else
    {
        if (cPosFlag == PRN_CENTER)
        {
            offset = (384 - 12 - (widthActual * 8)) / 2/ 6;
        }
        else if (cPosFlag == PRN_LEFT)
        {
            offset = 0;
        }
        else if (cPosFlag == PRN_RIGHT)
        {
            offset = (384 - 12 - (widthActual * 8)) /  6;
        }
    }

    //dbprintf("offset:%d,widthActual:%d",offset,widthActual);
    // the required size will depend on the offset plus the new line charecter added
    // at the end of the line we calculate the size with summing the (width+offset+newline)*(hieght)
    ReqSize = ((widthActual * 8) + (offset * 8)) / 6 * (height+2);
    //dbprintf("ReqSize:%d",ReqSize);
    bitmapConv = (unsigned char*)malloc(ReqSize + height);

    if (bitmapConv == NULL)
    {
        return NO_MEMORY;
    }

    memset(bitmapConv, 0xC0, ReqSize);

//**************************************************************************************
//Get the offset data ranging from 0-60 this value
// each offset is 6 bit of data the max range is 384 ie 64 such contianers
// this offset value is manipulated to get the cordinate which data has to start printing
// if the data exceeds the req width then it has to be trimmed to the width
//**************************************************************************************

// offset width remaining has to be calculated we leave the 2 offset on extreme right and left
    offsetwidth = (384 - ((offset * 6) + 12));

// get the width where in the data has to be got
    if ((offsetwidth % 8) > 0)
    {
        width = (offsetwidth / 8) + 1;
    }
    else
    {
        width = (offsetwidth / 8);
    }

    // if the width is less than widthActual then widthActual is taken as the actual value
    if (widthActual <= width)
    {
        width = widthActual;
    }
    else//the pixels of the lasr column has to be calculated to no of bits the last column has to print
    {
        mod = offsetwidth % 8;
    }

//**************************************************************************************
// Start the graphics mode of printing the data
//ESC g is the command for going to the graphicsa mode
//**************************************************************************************
    memset(Printmode, 0, 4);
    memset(ioctl_status, 0, 4);
    Printmode[0] = 0x1b;
    Printmode[1] = 0x67;

    while (get_port_status(h_printer, ioctl_status) != 0)
        SVC_WAIT(2);

    retVal = write(h_printer, Printmode, 2);

    if (retVal < 0)
    {
        return  WRITE_FAIL;
    }

//**************************************************************************************
//Now thw bitmap will have the data in the reverse hence the last data of the bitmap
//will be the first data hnece get the data in reverse
//the value of the buffer is memset to 0x0c 1100 0000 which will not print data
//Container should have 1 1     0 0 0 0 0 0
//            | |     | | | | | |
//          Non printing  Printing data
//**************************************************************************************

    count = 0;

    for (i =0; i < height; i++)
    {
        bitset = 5;// only 6 bits of data has to be read in the 8 bits and rest will go into another byte
        count = count + offset;// offset the value with buffer of the offset for each row

        for (j = 0; j < width; ++j)
        {
            if ((j == (width - 1)))
                bits = 7 - (mod - 1);
            else
                bits = 0;

            DataBuf[0] = bitmap[(i*widthActual)+j];

            for (bitcount = 7; bitcount >= bits; bitcount--)
            {
                TransBuf[0] = 0;
                TransBuf[0] = DataBuf[0] & counter[bitcount];// Get the bit data
                TransBuf[0] = TransBuf[0] >> bitcount;// right shift to the 0th position
                TransBuf[0] = TransBuf[0] << bitset;//left shift to the required bit position
                bitmapConv[count] |= TransBuf[0]; // Or the value of the bit
                bitset--;//decrese the bit position

                if (bitset < 0)// if bit are filled then move to the other container
                {
                    bitset = 5;
                    count++;
                }
            }
        }
        //dbprintf("count:%d",count);
        ++heightIndex;// go the next row
        count++;//increase the count
        bitmapConv[count++] = 0x21;//new line
    }// end of for loop

    // Print the remaining data
    do
    {
        //Dinesh_s4 Commented to address the $custom IC10099& IC10098
        //while(get_port_status(handle, ioctl_status)!=0)
        //SVC_WAIT(0);
        retVal = write(h_printer, (char *)bitmapConv, count);
    }
    while ((retVal < 0) && (errno == ENOSPC || errno == ENXIO));

    //***********************************************************************************************
    // Exit from the graphics mode passing the 0x18 CAN command ie the 0001 0100 the fifth bit should be set to 1
    //***********************************************************************************************

    memset(Printmode, 0, 4);
    Printmode[0] = 0x18;

    while (get_port_status(h_printer, ioctl_status) != 0)
        SVC_WAIT(10);

    retVal = write(h_printer, Printmode, 1);

    if (retVal < 0)
    {
        return  WRITE_FAIL;
    }
    free(bitmapConv);
    bitmapConv = NULL;
    free(bitmap);
    SVC_WAIT(10);
    return PRINT_OK;
}

int eSign_CompressImage(const char *fnin, const char *fnout)
{
    char type;
    int y,c;
    FILE *fin;
    FILE *fout = stdout;
    size_t bitmap_size;
    unsigned char **bitmap;
    unsigned long width, height;

    unsigned long bpl;
    int encode_planes = -1;
    struct jbg_enc_state s;
    int delay_at = 0, reset = 0;
    long mwidth = 640, mheight = 480;
    int dl = -1, dh = -1, d = -1, mx = -1;
    unsigned long l0 = 0, y1 = 0;
    char *comment = NULL;
    int options = JBG_TPDON | JBG_TPBON | JBG_DPON;
    int order = JBG_ILEAVE | JBG_SMID;

    if (!fnin || !fnout)
    {
        MmiUtil_Warning( "fnin or fnout fail!" );
        return ESIGN_FILEFAIL;
    }

    fin = fopen(fnin, "rb");
    if (!fin)
    {
        MmiUtil_Warning( "open fnin file fail!" );
        return ESIGN_FILEFAIL;
    }

    fout = fopen(fnout, "wb");

    if (!fout)
    {
        fclose(fin);
        MmiUtil_Warning( "open fnout file fail!" );
        return ESIGN_FILEFAIL;
    }

    /* read PBM header */
    while ((c = getc(fin)) != EOF && (isspace(c) || c == '#'))
        if (c == '#')
            while ((c = getc(fin)) != EOF && !(c == 13 || c == 10)) ;
    if (c != 'P')
    {
        fclose(fin);
        fclose(fout);
        MmiUtil_Warning( "read pbm header fail!" );
        return ESIGN_FILEFAIL;
    }
    /*get width and height*/
    type = fgetc(fin);		/*type='4'*/
    width = getint(fin);
    height = getint(fin);
    fgetc(fin);    	/* skip line feed */
    l0 = 7;
    encode_planes = 1;

    if (type == '4')
    {
        /* read PBM image data */
        bpl = (width + 7) / 8;     /* bytes per line */
        bitmap_size = bpl * (size_t) height;

        bitmap = (unsigned char **) malloc(sizeof(unsigned char *));
        if (!bitmap)
        {
            fclose(fin);
            fclose(fout);
            MmiUtil_Warning( "malloc momemory fail!" );
            return ESIGN_NOMEMORY;
        }
        bitmap[0] = (unsigned char *) malloc(bitmap_size);
        if (!bitmap[0])
        {
            free(bitmap);
            fclose(fin);
            fclose(fout);
            MmiUtil_Warning( "malloc momemory2 fail!" );
            return ESIGN_NOMEMORY;
        }
        /* PBM raw binary format */
        if (fread((char *)bitmap[0],bitmap_size,1,fin) != 1)
        {
            free(bitmap[0]);
            free(bitmap);

            fclose(fin);
            fclose(fout);
            MmiUtil_Warning( "read bitmap fail!" );
            return ESIGN_FILEFAIL;
        }
    }
    else
    {
        fclose(fin);
        fclose(fout);
        MmiUtil_Warning( "type!=4 fail!" );
        return ESIGN_FILEFAIL;
    }

    fclose(fin);
    /* Test the final byte in each image line for correct zero padding */
    if (width & 7)
    {
        for (y = 0; y < height; y++)
            if (bitmap[0][y * bpl + bpl - 1] & ((1 << (8 - (width & 7))) - 1))
            {
                break;
            }
    }

    /* Apply JBIG algorithm and write BIE to output file */

    /* initialize parameter struct for JBIG encoder*/
    jbg_enc_init(&s, width, height, encode_planes, bitmap, data_out, fout);

    //LOG_PRINTF_LIB("d[%d] mwidth[%ld] mheight[%ld]", d, mwidth, mheight);
    //LOG_PRINTF_LIB("delay_at[%d] reset[%ld] y1[%d]", delay_at, reset, y1);

    /* Select number of resolution layers either directly or based
     * on a given maximum size for the lowest resolution layer */
    if (d >= 0)
        jbg_enc_layers(&s, d);
    else
        jbg_enc_lrlmax(&s, mwidth, mheight);

    /* Specify a few other options (each is ignored if negative) */
    if (delay_at)
        options |= JBG_DELAY_AT;

    if (reset)
        options |= JBG_SDRST;

    if (comment)
    {
        s.comment_len = strlen(comment);
        s.comment = (unsigned char *) comment;
    }

    if (y1)
        s.yd1 = y1;

    jbg_enc_lrange(&s, dl, dh);
    jbg_enc_options(&s, order, options, l0, mx, -1);

    /* now encode everything and send it to data_out() */
    jbg_enc_out(&s);

    /* give encoder a chance to free its temporary data structures */
    jbg_enc_free(&s);

    /* check for file errors and close fout */
    if (ferror(fout))
    {
        free(bitmap[0]);
        free(bitmap);
        fclose(fout);
        MmiUtil_Warning( "ferror(fout) fail!" );
        return ESIGN_FILEFAIL-9;
    }

    free(bitmap[0]);
    free(bitmap);
    fclose(fout);

    return ESIGN_OK;
}

int eSign_UnCompressImage(const char *fnin, const char *fnout)
{
    FILE *fin = stdin, *fout = stdout;
    int i, result;
    struct jbg_dec_state s;
    unsigned char *buffer, *p;
    size_t buflen, len, cnt;
    size_t bytes_read = 0;
    unsigned long xmax = 4294967295UL, ymax = 4294967295UL, max;
    int plane = -1, use_graycode = 1, multi = 0;

    buflen = 8000;
    buffer = (unsigned char *) malloc(buflen);

    if (!buffer)
    {
        //LOG_PRINTF_LIB("Sorry, not enough memory available!");
        return ESIGN_NOMEMORY;
    }

    if ((fin = fopen(fnin, "rb")) < 0)
    {
        free(buffer);
        buffer = NULL;

        //LOG_PRINTF_LIB("fpfail[%d]", fin);
        return ESIGN_FILEFAIL;
    }

    if ((fout = fopen(fnout, "wb")) < 0)
    {
        free(buffer);
        buffer = NULL;
        fclose(fin);

        //LOG_PRINTF_LIB("fpfail[%d]", fout);
        return ESIGN_FILEFAIL;
    }

    /* send input file to decoder */
    jbg_dec_init(&s);
    jbg_dec_maxsize(&s, xmax, ymax);
    /* read BIH first to check VLENGTH */
    len = fread(buffer, 1, 20, fin);

    if (len < 20)
    {
#if 0
        LOG_PRINTF_LIB("Input file '%s' (%d bytes) must be at least "
                       "20 bytes long\n", fnin, len);
#endif
        if (fout != stdout)
        {
            fclose(fout);
            remove(fnout);
        }

        free(buffer);
        buffer = NULL;
        fclose(fin);

        return ESIGN_FILEFAIL;
    }

    //LOG_PRINTF_HEX("BIN", (char *)buffer, len);

    if (buffer[19] & JBG_VLENGTH)
    {
        /* VLENGTH = 1 => we might encounter a NEWLEN, therefore read entire
         * input file into memory and run two passes over it */
        result = read_file(&buffer, &buflen, &len, fin);
        if (result != JBG_EOK)
            goto RESULT;
        /* scan for NEWLEN marker segments and update BIE header accordingly */
        result = jbg_newlen(buffer, len);

        /* feed data to decoder */
        if (result == JBG_EOK)
        {
            p = (unsigned char *) buffer;
            result = JBG_EAGAIN;

            while (len > 0 &&
                    (result == JBG_EAGAIN || (result == JBG_EOK && multi)))
            {
                result = jbg_dec_in(&s, p, len, &cnt);
                p += cnt;
                len -= cnt;
                bytes_read += cnt;
            }
        }
    }
    else
    {
        /* VLENGTH = 0 => we can simply pass the input file directly to decoder */
        result = JBG_EAGAIN;

        do
        {
            cnt = 0;
            p = (unsigned char *) buffer;

            while (len > 0 &&
                    (result == JBG_EAGAIN || (result == JBG_EOK && multi)))
            {
                result = jbg_dec_in(&s, p, len, &cnt);
                p += cnt;
                len -= cnt;
                bytes_read += cnt;
            }

            if (!(result == JBG_EAGAIN || (result == JBG_EOK && multi)))
                break;

            len = fread(buffer, 1, buflen, fin);
        }
        while (len > 0);

        if (ferror(fin))
        {
            //LOG_PRINTF_LIB("Problem while reading input file '%s", fnin);

            if (fout != stdout)
            {
                fclose(fout);
                remove(fnout);
            }

            free(buffer);
            buffer = NULL;
            fclose(fin);

            return ESIGN_FILEFAIL;
        }
    }

RESULT:
    if (result != JBG_EOK && result != JBG_EOK_INTR)
    {
#if 0
        LOG_PRINTF_LIB("Problem with input file '%s': %s\n"
                       "(error code 0x%02x, %lu = 0x%04lx BIE bytes processed)\n",
                       fnin, jbg_strerror(result), result,
                       (unsigned long) bytes_read, (unsigned long) bytes_read);
#endif
        if (fout != stdout)
        {
            fclose(fout);
            remove(fnout);
        }

        free(buffer);
        buffer = NULL;
        fclose(fin);

        return ESIGN_FILEFAIL;
    }

    if (plane >= 0 && jbg_dec_getplanes(&s) <= plane)
    {
        //LOG_PRINTF_LIB("Image has only %d planes!\n", jbg_dec_getplanes(&s));

        if (fout != stdout)
        {
            fclose(fout);
            remove(fnout);
        }

        free(buffer);
        buffer = NULL;
        fclose(fin);

        return ESIGN_FAIL;
    }

    if (jbg_dec_getplanes(&s) == 1 || plane >= 0)
    {
        /* write PBM header.add by yunlong@2014/2/17*/
        fprintf(fout, "P4\n%d %d\n", jbg_dec_getwidth(&s),
                jbg_dec_getheight(&s));
        /* write PBM output file */
        fwrite(jbg_dec_getimage(&s, plane < 0 ? 0 : plane), 1,
               jbg_dec_getsize(&s), fout);
    }
    else
    {
        /* write PGM output file */
        if ((size_t) jbg_dec_getplanes(&s) > sizeof(unsigned long) * 8)
        {
            if (fout != stdout)
            {
                fclose(fout);
                remove(fnout);
            }

            free(buffer);
            buffer = NULL;
            fclose(fin);

            return ESIGN_FAIL;
        }

        max = 0;

        for (i = jbg_dec_getplanes(&s); i > 0; i--)
            max = (max << 1) | 1;
        fprintf(fout, "P4\n%d\n %d\n%d\n", jbg_dec_getwidth(&s),
                jbg_dec_getheight(&s), max);
        jbg_dec_merge_planes(&s, use_graycode, data_out, fout);
    }

    /* check for file errors and close fout */
    if (ferror(fout) || fclose(fout))
    {
        //LOG_PRINTF_LIB("Problem while writing output file '%s", fnout);
        return ESIGN_FILEFAIL;
    }

    jbg_dec_free(&s);

    free(buffer);
    buffer = NULL;
    fclose(fin);
    fclose(fout);

    return ESIGN_OK;
}




int eSign_SetReSignTimes(unsigned char Rs232Dev,byte uReSignTimes,int timeout)
{
    unsigned char    buf[SIGN_MAX_BUF_LEN];
    unsigned int len;
    int   ret;


    ret = eSign_InitComm(Rs232Dev);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_RESIGN_TIME;
    buf[1] = uReSignTimes;
    len = 2;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    ret = eSign_RecvData(Rs232Dev, buf, &len, timeout);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    if (buf[0] != SIGN_RESIGN_TIME)  //签字失败
    {
        return ESIGN_FAIL;
    }

    if (buf[1] != 0)
    {
        return ESIGN_FAIL;
    }


    return ESIGN_OK;
}

int eSign_SetWritingDegree(unsigned char Rs232Dev,byte uWritingDegree,int timeout)
{
    unsigned char    buf[SIGN_MAX_BUF_LEN];
    unsigned int len;
    int   ret;


    ret = eSign_InitComm(Rs232Dev);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_WRITING_DEGREE;
    buf[1] = uWritingDegree;
    len = 2;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    ret = eSign_RecvData(Rs232Dev, buf, &len, timeout);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    if (buf[1] != 0)
    {
        return ESIGN_FAIL;
    }

    return ESIGN_OK;
}

int eSign_SetByPassFlag(unsigned char Rs232Dev,boolean bByPassFlag,int timeout)
{
    unsigned char    buf[SIGN_MAX_BUF_LEN];
    unsigned int len;
    int   ret;


    ret = eSign_InitComm(Rs232Dev);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_BYPASS;
    buf[1] = bByPassFlag;
    len = 2;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    ret = eSign_RecvData(Rs232Dev, buf, &len, timeout);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    if (buf[0] != SIGN_BYPASS)  //签字失败
    {
        return ESIGN_FAIL;
    }

    if (buf[1] != 0)
    {
        return ESIGN_FAIL;
    }


    return ESIGN_OK;
}


int eSign_Check(char signpad_type)
{
    int ret;
    byte State;

    if (signpad_type == INNER_SIGNPAD)
    {
        if ((MmiUtil_GetTerminalType()!=_VX820)&&
                (MmiUtil_GetTerminalType()!=_VX680)&&
                (MmiUtil_GetTerminalType()!=_VX680C))
        {
            return ESIGN_FAIL;
        }
        return ESIGN_OK;
    }
    else if (signpad_type == EXTERNAL_SIGNPAD)
    {
        ret = eSign_HandShake(1, 2, &State);

        if (ret != ESIGN_OK)
        {
            return ret;
        }


        ret = eSign_SetWritingDegree( 1, 1, 2 );
        return ret;
    }
    else
    {
        return ESIGN_FAIL;
    }
}

int eSign_SetPicZone(unsigned char Rs232Dev,int PicHeight,int PicWidth,int timeout)
{
    unsigned char    buf[SIGN_MAX_BUF_LEN];
    unsigned int len;
    int   ret;
    int nZoneHeight = 160;
    int nZoneWidth = 480;
    ret = eSign_InitComm(Rs232Dev);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_SETPIC_ZONE;
    buf[1] = nZoneWidth/256;
    buf[2] = nZoneWidth%256;
    buf[3] = nZoneHeight/256;
    buf[4] = nZoneHeight%256;
    buf[5] = PicWidth/256;
    buf[6] = PicWidth%256;
    buf[7] = PicHeight/256;
    buf[8] = PicHeight%256;
    len = 9;
    ret = eSign_SendData(Rs232Dev, buf, len);

    if (ret != ESIGN_OK)
    {
        return ret;
    }

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    ret = eSign_RecvData(Rs232Dev, buf, &len, timeout);
    if (ret != ESIGN_OK)
    {
        return ret;
    }
    if (buf[0] != SIGN_SETPIC_ZONE)  //签字失败
    {
        return ESIGN_FAIL;
    }

    if (buf[1] != 0)
    {
        return buf[1];
    }
    return ESIGN_OK;
}

#if 0
/* --------------------------------------------------------------------------
* FUNCTION NAME: eSign_SetSignatureTimeout
* DESCRIPTION:   设置外置签名板超时时间
* PARAMETERS:
*   iTimeOut  	(in) -- 超时时间<256s
* RETURN: 目前暂时针对Vx820
* NOTES:
* ------------------------------------------------------------------------ */
int eSign_SetSignatureTimeout( unsigned int iTimeOut )
{
    unsigned char buf[16];
    unsigned int len;
    int ret;

    ret = eSign_InitComm( 1 );
    if (ret != ESIGN_OK)
        return ret;

    memset(buf, 0, sizeof(buf));
    buf[0] = SIGN_SIGN_TIMEOUT;
    buf[1] = iTimeOut;
    len = 2;
    ret = eSign_SendData( 1, buf, len );
    if (ret != ESIGN_OK)
        return ret;

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    ret = eSign_RecvData(1, buf, &len, 5 );
    if (ret != ESIGN_OK)
        return ret;

    if( buf[0] != SIGN_SIGN_TIMEOUT )
        return ESIGN_FAIL;

    return ESIGN_OK;
}
#endif

