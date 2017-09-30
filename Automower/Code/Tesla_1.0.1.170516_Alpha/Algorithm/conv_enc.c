
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			conv_enc.c
  Author:			   	rui.fang
  Date:			    	2017.9.28
  Version:        
  Description:    //采用生成多项式为[171 133]的卷积码进行信道编码
                 
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/

#include "conv_enc.h"


/*
 * Arguments    : const boolean_T msg_data[]
 *                const int msg_size[2]
 *                boolean_T varargout_1_data[]
 *                int varargout_1_size[2]
 * Return Type  : void
 */
static void convenc(const boolean_T msg_data[], const int msg_size[2], boolean_T
                    varargout_1_data[], int varargout_1_size[2])
{
  int code_size_idx_1;
  int fstate;
  int inIdx;
  int tmp;
  static const signed char iv0[128] = { 0, 3, 1, 2, 0, 3, 1, 2, 3, 0, 2, 1, 3, 0,
    2, 1, 3, 0, 2, 1, 3, 0, 2, 1, 0, 3, 1, 2, 0, 3, 1, 2, 2, 1, 3, 0, 2, 1, 3, 0,
    1, 2, 0, 3, 1, 2, 0, 3, 1, 2, 0, 3, 1, 2, 0, 3, 2, 1, 3, 0, 2, 1, 3, 0, 3, 0,
    2, 1, 3, 0, 2, 1, 0, 3, 1, 2, 0, 3, 1, 2, 0, 3, 1, 2, 0, 3, 1, 2, 3, 0, 2, 1,
    3, 0, 2, 1, 1, 2, 0, 3, 1, 2, 0, 3, 2, 1, 3, 0, 2, 1, 3, 0, 2, 1, 3, 0, 2, 1,
    3, 0, 1, 2, 0, 3, 1, 2, 0, 3 };

  static const signed char iv1[128] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
    7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17,
    17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26,
    27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36,
    36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45,
    46, 46, 47, 47, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 53, 54, 54, 55,
    55, 56, 56, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63 };

  int startIdx;
  boolean_T code_data[104];
  int j;
  code_size_idx_1 = msg_size[1] << 1;
  fstate = 0;
  for (inIdx = 0; inIdx < msg_size[1]; inIdx++) {
    fstate += (unsigned int)msg_data[inIdx] << 6;
    tmp = iv0[fstate];
    fstate = iv1[fstate];
    startIdx = 1 + inIdx << 1;
    for (j = 0; j < 2; j++) {
      code_data[startIdx - j - 1] = (tmp & 1U) != 0U;
      tmp = (int)((unsigned int)tmp >> 1U);
    }
  }

  varargout_1_size[0] = 1;
  varargout_1_size[1] = code_size_idx_1;
  for (fstate = 0; fstate < code_size_idx_1; fstate++) {
    varargout_1_data[fstate] = code_data[fstate];
  }
}

/*
 * Description  : 卷积码信道编码主函数
 * Arguments    : double data_raw
 *                boolean_T dataEnc_data[]
 *                int dataEnc_size[2]
 * Return Type  : void
 */
void ch_enc(double data_raw, boolean_T dataEnc_data[], int dataEnc_size[2])
{
  float x;
  unsigned char bytes[4];
  int j;
  int i0;
  double di;
  double p16;
  unsigned char b_di;
  char h2[2];
  int b_j;
  boolean_T exitg1;
  int data_bin_size_idx_1;
  char data_hex[8];
  unsigned char r;
  char data_bin_data[52];
  int b_data_hex;
  int firstcol;
  boolean_T p;
  int data_bin_size[2];
  char b_data_bin_data[52];
  boolean_T c_data_bin_data[52];

  /* 'ch_enc:3' data_hex = num2hex(single(data_raw)); */
  x = (float)data_raw;
  memcpy((void *)&bytes[0], (void *)&x, (size_t)4 * sizeof(unsigned char));
  for (j = 0; j < 4; j++) {
    for (i0 = 0; i0 < 2; i0++) {
      h2[i0] = '0';
    }

    b_di = bytes[3 - j];
    b_j = 2;
    exitg1 = false;
    while (!exitg1 && b_j > 0) {
      r = b_di;
      b_di = (unsigned char)((unsigned int)b_di >> 4);
      r = (unsigned char)((unsigned int)r - (b_di << 4));
      if (r > 9) {
        r = (unsigned char)(r + 7U);
      }

      h2[b_j - 1] = (signed char)(r + 48);
      if (b_di > 0) {
        b_j--;
      } else {
        exitg1 = true;
      }
    }

    if (h2[0] >= 'A' && h2[0] <= 'F') {
      h2[0] = (signed char)(h2[0] + 32);
    }

    if (h2[1] >= 'A' && h2[1] <= 'F') {
      h2[1] = (signed char)(h2[1] + 32);
    }

    data_hex[j << 1] = h2[0];
    data_hex[(j << 1) + 1] = h2[1];
  }

  /* 'ch_enc:4' data_bin = dec2bin(hex2dec(data_hex),32); */
  di = 0.0;
  p16 = 1.0;
  for (j = 0; j < 8; j++) {
    if (data_hex[7 - j] != ' ') {
      if ((unsigned char)data_hex[7 - j] != 0 && (unsigned char)data_hex[7 - j]
          != 48) {
        if ((unsigned char)data_hex[7 - j] <= 57) {
          b_data_hex = (unsigned char)data_hex[7 - j] - 48;
        } else if ((unsigned char)data_hex[7 - j] > 70) {
          b_data_hex = (unsigned char)data_hex[7 - j] - 87;
        } else {
          b_data_hex = (unsigned char)data_hex[7 - j] - 55;
        }

        di += (double)b_data_hex * p16;
      }

      p16 *= 16.0;
    }
  }

  data_bin_size_idx_1 = 52;
  for (i0 = 0; i0 < 52; i0++) {
    data_bin_data[i0] = '0';
  }

  j = 52;
  exitg1 = false;
  while (!exitg1 && j > 0) {
    p16 = di;
    di = floor(di / 2.0);
    if (2.0 * di < p16) {
      data_bin_data[j - 1] = '1';
    }

    if (di > 0.0) {
      j--;
    } else {
      exitg1 = true;
    }
  }

  firstcol = 21;
  j = 1;
  exitg1 = false;
  while (!exitg1 && j <= 20) {
    p = false;
    if (data_bin_data[j - 1] != '0') {
      p = true;
    }

    if (p) {
      firstcol = j;
      exitg1 = true;
    } else {
      j++;
    }
  }

  if (firstcol > 1) {
    for (j = firstcol; j < 53; j++) {
      data_bin_data[j - firstcol] = data_bin_data[j - 1];
    }

    b_j = 53 - firstcol;
    for (i0 = 0; i0 < b_j; i0++) {
      b_data_bin_data[i0] = data_bin_data[i0];
    }

    data_bin_size_idx_1 = 53 - firstcol;
    b_j = 53 - firstcol;
    for (i0 = 0; i0 < b_j; i0++) {
      data_bin_data[i0] = b_data_bin_data[i0];
    }
  }

  /* 'ch_enc:5' data_bin = (data_bin == '1'); */
  /* 'ch_enc:6' lc = 7; */
  /* 'ch_enc:7' poly = [171 133]; */
  /* 'ch_enc:8' trellis = poly2trellis(lc,poly); */
  /* 'ch_enc:9' dataEnc = convenc(data_bin,trellis); */
  data_bin_size[0] = 1;
  data_bin_size[1] = data_bin_size_idx_1;
  for (i0 = 0; i0 < data_bin_size_idx_1; i0++) {
    c_data_bin_data[i0] = data_bin_data[i0] == '1';
  }

  convenc(c_data_bin_data, data_bin_size, dataEnc_data, dataEnc_size);
}

/*******convert binary to hex ****************/
void bin2hex( char *input, char *output)
{
	int len1 = 64;          //conversion length
	int pos = len1 / 4 + 1; 
	int i,j = 0;
	
	if (len1 % 4 == 0){
		pos = pos - 1;
	}
	
	while (len1>0){
		volatile char sum = 0;
		for (i=0; i<4 && len1>0; i++, len1--){
				sum = sum + (input[len1-1])*pow(2, i);

		}
		output[--pos] = sum;		
	}
}

/******接口函数：进行信道编码并将编码结果通过串口发送*************/
void conv_enc_tx(float data_tx)
{
	char send[16];
	char send_short[8];
	char *temp;
    boolean_T b_tx[64];
	int bin_size[2] = {1,64};
	
	ch_enc(data_tx,b_tx, bin_size);
    bin2hex(b_tx, send);
	
	
	
	rt_debug(send,16);   
}
