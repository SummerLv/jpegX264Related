#include "v4l2_use.h"
#include "image_process.h"

#include <jpeglib.h>
#include <setjmp.h>

extern unsigned char* dest_buffers;
extern unsigned char* rgb_buffers;

extern int numb;

static int frameNum;



long rgb_to_jpeg(const char *rgb, char *jpeg)
{
	

  long jpeg_size;
	struct jpeg_compress_struct jcs;
	struct jpeg_error_mgr jem;
	JSAMPROW row_pointer[1];
	int row_stride;
	
	jcs.err = jpeg_std_error(&jem);
	jpeg_create_compress(&jcs);

    //****!!! caution !!!
    // int *pBuf = NULL;
    // int len = 0;
    // jpeg_mem_dest(cinfo, &pBuf, &len);//you do not need to malloc mem for this function
    // memcpy(/*your dest mem*/, pBuf, len);
    //****!!! caution !!!	
	jpeg_mem_dest(&jcs, jpeg, &jpeg_size);//!!!!!!!!!!!!!!!!!!!!!
	
	jcs.image_width = WIDTH;
	jcs.image_height = HEIGHT;

	jcs.input_components = 3;//1;
	jcs.in_color_space = JCS_RGB;//JCS_GRAYSCALE;

	jpeg_set_defaults(&jcs);
	jpeg_set_quality(&jcs, 180, TRUE);
	
	jpeg_start_compress(&jcs, TRUE);
	row_stride =jcs.image_width * 3;
	
	while(jcs.next_scanline < jcs.image_height){//对每一行进行压缩
		row_pointer[0] = &rgb[jcs.next_scanline * row_stride];
		(void)jpeg_write_scanlines(&jcs, row_pointer, 1);
	}
	jpeg_finish_compress(&jcs);
	jpeg_destroy_compress(&jcs);
  printf("in  the  Version 1\n");


//------------>>>>>  JPEG  >>>>>>>>>>>>--------------------------

//#ifdef JPEG	//jpeg 保存
	FILE *jpeg_fd;
	char path3[50] = {'\0'};
	printf("*&*&*&*&*&*&* %d\n", frameNum);
	sprintf(path3, "/jpegPic/jpeg%d.jpg", frameNum);
	++frameNum;
	jpeg_fd = fopen(path3,"wb");
	if(jpeg_fd < 0 ){
		perror("open jpeg.jpg failed!\n");
		exit(-1);
	}
	
	fwrite(jpeg, jpeg_size, 1, jpeg_fd);
	fclose(jpeg_fd);
//#endif 
	return jpeg_size;
}


//  have to prepare <*filename ,quality>
long rgb_to_jpeg_v2(const char *rgb, char *jpeg,char * filename, int quality)
{
  /*  working space is allocated as needed by the JPEG library).
   */

  printf("in  the  Version 2\n");

  struct jpeg_compress_struct cinfo;
  /*
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct jpeg_error_mgr jerr;
  FILE * outfile;       /* target file */
  JSAMPROW row_pointer[1];  /* pointer to JSAMPLE row[s] */
  int row_stride;       /* physical row width in image buffer */

  /* Step 1: allocate and initialize JPEG compression object */
  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the <<<<<<<<JPEG compression object. >>>>>>>>*/
  jpeg_create_compress(&cinfo);

  /* Step 2: specify data destination (eg, a file) */
  /* Note: steps 2 and 3 can be done in either order. */

  /* Here we use the library-supplied code to send compressed data to a
   * stdio stream.  You can also write your own code to do something else.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to write binary files.
   */
  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  /* Step 3: set parameters for compression */

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */
  cinfo.image_width = WIDTH;  /* image width and height, in pixels */
  cinfo.image_height = HEIGHT;
  cinfo.input_components = 3;       /* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB;   /* colorspace of input image */
  /* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */
  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  /* Step 4: Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5: while (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */

  /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */
  row_stride = cinfo.image_width * 3; /* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    row_pointer[0] = & v4l2_use.rgb_buffers[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  /* Step 6: Finish compression */

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  fclose(outfile);

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  /* And we're done! */
}


void yuyv_to_rgb(unsigned char* yuv,unsigned char* rgb)
{
    printf("begin yuyv_to_rgb\n");
    unsigned int i;
    unsigned char* y0 = yuv + 0;   
    unsigned char* u0 = yuv + 1;
    unsigned char* y1 = yuv + 2;
    unsigned char* v0 = yuv + 3;

    unsigned  char* r0 = rgb + 0;
    unsigned  char* g0 = rgb + 1;
    unsigned  char* b0 = rgb + 2;
    unsigned  char* r1 = rgb + 3;
    unsigned  char* g1 = rgb + 4;
    unsigned  char* b1 = rgb + 5;
   
    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;

    for(i = 0; i <= (WIDTH * HEIGHT) / 2 ;i++)
    {
    		//printf("%d\n",i );
        bt0 = 1.164 * (*y0 - 16) + 2.018 * (*u0 - 128); 
        gt0 = 1.164 * (*y0 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128); 
        rt0 = 1.164 * (*y0 - 16) + 1.596 * (*v0 - 128); 
   
    	   bt1 = 1.164 * (*y1 - 16) + 2.018 * (*u0 - 128); 
        gt1 = 1.164 * (*y1 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128); 
        rt1 = 1.164 * (*y1 - 16) + 1.596 * (*v0 - 128); 
    
      
    if(rt0 > 250)  	rt0 = 255;
		if(rt0 < 0)    	rt0 = 0;	

		if(gt0 > 250) 	gt0 = 255;
		if(gt0 < 0)		gt0 = 0;	

		if(bt0 > 250)	     bt0 = 255;
		if(bt0 < 0)		bt0 = 0;	

		if(rt1 > 250)	     rt1 = 255;
		if(rt1 < 0)		rt1 = 0;	

		if(gt1 > 250)	     gt1 = 255;
		if(gt1 < 0)		gt1 = 0;	

		if(bt1 > 250)	     bt1 = 255;
		if(bt1 < 0)		bt1 = 0;	
					
		*r0 = (unsigned char)rt0;
		*g0 = (unsigned char)gt0;
		*b0 = (unsigned char)bt0;
	
		*r1 = (unsigned char)rt1;
		*g1 = (unsigned char)gt1;
		*b1 = (unsigned char)bt1;

        yuv = yuv + 4;
        rgb = rgb + 6;
        if(yuv == NULL)
        {
        	printf("yuyv covert finished\n");
        	break;
        }
          

        y0 = yuv;
        u0 = yuv + 1;
        y1 = yuv + 2;
        v0 = yuv + 3;
  
        r0 = rgb + 0;
        g0 = rgb + 1;
        b0 = rgb + 2;
        r1 = rgb + 3;
        g1 = rgb + 4;
        b1 = rgb + 5;
    }  
    printf("yuyv_to_rgb  Done \n"); 
}

int convert_yuv_to_rgb_pixel(int y, int u, int v)
{
 unsigned int pixel32 = 0;
 unsigned char *pixel = (unsigned char *)&pixel32;
 int r, g, b;
 r = y + (1.370705 * (v-128));
 g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
 b = y + (1.732446 * (u-128));
 if(r > 255) r = 255;
 if(g > 255) g = 255;
 if(b > 255) b = 255;
 if(r < 0) r = 0;
 if(g < 0) g = 0;
 if(b < 0) b = 0;
 pixel[0] = r * 220 / 256;
 pixel[1] = g * 220 / 256;
 pixel[2] = b * 220 / 256;
 return pixel32;
}

int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
 unsigned int in, out = 0;
 unsigned int pixel_16;
 unsigned char pixel_24[3];
 unsigned int pixel32;
 int y0, u, y1, v;
 printf("yuv2rgb  begin^^^^^^^^^\n");
 for(in = 0; in < width * height * 2; in += 4) {

  pixel_16 =
   yuv[in + 3] << 24 |
   yuv[in + 2] << 16 |
   yuv[in + 1] <<  8 |
   yuv[in + 0];

  y0 = (pixel_16 & 0x000000ff);
  u  = (pixel_16 & 0x0000ff00) >>  8;
  y1 = (pixel_16 & 0x00ff0000) >> 16;
  v  = (pixel_16 & 0xff000000) >> 24;

  pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
  pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
 }
 printf("yuv2rgb  done\n");
 return 0;
}




void rgb_stretch(char* src_buf, char* dest_buf, int des_width, int des_hight)
{
	//最临近插值算法
	//双线性内插值算法放大后马赛克很严重 而且帧率下降严重
	printf("des_width = %d, des_hight = %d \n ",des_width, des_hight);
	double rate_w = (double) WIDTH / des_width;//横向放大比
	double rate_h = (double) HEIGHT / des_hight;//轴向放大比
	
	int dest_line_size = ((des_width * BITCOUNT +31) / 32) * 4;	
	int src_line_size = BITCOUNT * WIDTH / 8;
	int i = 0, j = 0, k = 0;
	for (i = 0; i < des_hight; i++)//desH 目标高度
	{
		//选取最邻近的点
		int t_src_h = (int)(rate_h * i + 0.5);//rateH (double)srcH / desH;
		for (j = 0; j < des_width; j++)//desW 目标宽度
		{
			int t_src_w = (int)(rate_w * j + 0.5);                            
			memcpy(&dest_buf[i * dest_line_size] + j * BITCOUNT / 8, \
				&src_buf[t_src_h * src_line_size] + t_src_w * BITCOUNT / 8,\
				BITCOUNT / 8);            
		}
	}   
}
void rgb_to_bmp(unsigned char* pdata, FILE* bmp_fd)     
{
    //分别为rgb数据，要保存的bmp文件名，图片长宽  
  
#ifdef _BIG_

	int size = DEST_WIDTH * DEST_HEIGHT * 3 * sizeof(char); // 每个像素点3个字节   
   
    // 位图第一部分，文件信息  
    BMPFILEHEADER_T bfh;  
    bfh.bfType = (unsigned short)0x4d42;  //bm  
    bfh.bfSize = size  // data size  
        + sizeof( BMPFILEHEADER_T ) // first section size  
        + sizeof( BMPINFOHEADER_T ) // second section size  
        ;  
    bfh.bfReserved1 = 0; // reserved  
    bfh.bfReserved2 = 0; // reserved  
    bfh.bfOffBits = sizeof( BMPFILEHEADER_T )+ sizeof( BMPINFOHEADER_T );//真正的数据的位置 

	
    // 位图第二部分，数据信息  
    BMPINFOHEADER_T bih;  
    bih.biSize = sizeof(BMPINFOHEADER_T);  
    bih.biWidth = DEST_WIDTH;
	bih.biHeight = -DEST_HEIGHT;
    bih.biPlanes = 1;//为1，不用改  
    bih.biBitCount = BITCOUNT;  
	
    bih.biCompression = 0;//不压缩  
    bih.biSizeImage = size;  
    bih.biXPelsPerMeter = 0;//像素每米  
    bih.biYPelsPerMeter = 0;  
    bih.biClrUsed = 0;//已用过的颜色，为0,与bitcount相同  
    bih.biClrImportant = 0;//每个像素都重要   
    
    fwrite( &bfh, 8, 1, bmp_fd); 
    fwrite(&bfh.bfReserved2, sizeof(bfh.bfReserved2), 1, bmp_fd);  
    fwrite(&bfh.bfOffBits, sizeof(bfh.bfOffBits), 1, bmp_fd);  
    fwrite(&bih, sizeof(BMPINFOHEADER_T), 1, bmp_fd);  
    fwrite(pdata, size, 1, bmp_fd);   
	
#else

	int size = WIDTH * HEIGHT * 3 * sizeof(char); // 每个像素点3个字节
	
    // 位图第一部分，文件信息  
    BMPFILEHEADER_T bfh;  
    bfh.bfType = (unsigned short)0x4d42;  //bm  
    bfh.bfSize = size  // data size  
        + sizeof( BMPFILEHEADER_T ) // first section size  
        + sizeof( BMPINFOHEADER_T ) // second section size  
        ;  
    bfh.bfReserved1 = 0; // reserved  
    bfh.bfReserved2 = 0; // reserved  
    bfh.bfOffBits = sizeof( BMPFILEHEADER_T )+ sizeof( BMPINFOHEADER_T );//真正的数据的位置 
	
    // 位图第二部分，数据信息  
    BMPINFOHEADER_T bih;  
    bih.biSize = sizeof(BMPINFOHEADER_T);  
    bih.biWidth = WIDTH;  
    bih.biHeight = -HEIGHT;//BMP图片从最后一个点开始扫描，显示时图片是倒着的，所以用-height，这样图片就正了  
	
    bih.biPlanes = 1;//为1，不用改  
    bih.biBitCount = BITCOUNT;  
    bih.biCompression = 0;//不压缩  
    bih.biSizeImage = size;  
    bih.biXPelsPerMeter = 0;//像素每米  
    bih.biYPelsPerMeter = 0;  
    bih.biClrUsed = 0;//已用过的颜色，为0,与bitcount相同  
    bih.biClrImportant = 0;//每个像素都重要   
    
    fwrite( &bfh, 8, 1, bmp_fd); 
    fwrite(&bfh.bfReserved2, sizeof(bfh.bfReserved2), 1, bmp_fd);  
    fwrite(&bfh.bfOffBits, sizeof(bfh.bfOffBits), 1, bmp_fd);  
    fwrite(&bih, sizeof(BMPINFOHEADER_T), 1, bmp_fd);  
    fwrite(pdata, size, 1, bmp_fd);   
#endif


  
} 

//流输入的Jpeg的处理？？？



/*双线型内插值算法*/
	/*for (i = 0; i < des_hight; i++)
	{
		int tH = (int)(rate_h * i);
		int tH1 = min(tH + 1,HEIGHT - 1);
		float u = (float)(rate_h * i - tH);
		for (j = 0; j < des_width; j++)
		{
			int tW = (int)(rate_w * j); 
			int tW1 = min(tW + 1,WIDTH - 1);
			float v = (float)(rate_w * j - tW);

			//f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1) 
			for (k = 0; k < 3; k++)
			{
				dest_buf[i * dest_line_size + j * BITCOUNT / 8 + k] = 
				(1 - u)*(1 - v) * src_buf[tH * src_line_size + tW * BITCOUNT / 8 + k] + 
				(1 - u)*v*src_buf[tH1 * src_line_size + tW * BITCOUNT / 8+ k] + 
				u * (1 - v) * src_buf[tH * src_line_size + tW1 * BITCOUNT / 8 + k] + 
				u * v * src_buf[tH1 * src_line_size + tW1 * BITCOUNT / 8 + k];                     
			}            
		}
	}*/
	
/*
	jcs.raw_data_in = TRUE;
	jcs.jpeg_color_space = JCS_YCbCr;
	jcs.comp_info[0].h_samp_factor = 2;
	jcs.comp_info[0].v_samp_factor = 2;

*/

