#ifndef _IMAGE_PROCESS_H
#define _IMAGE_PROCESS_H



/*******BMP 文件头**********/
typedef struct {
    unsigned short   bfType;  
    unsigned long    bfSize;  
    unsigned short   bfReserved1;  
    unsigned short   bfReserved2;  
    unsigned long    bfOffBits;  
}BMPFILEHEADER_T;  

/*******BMP 信息头**********/ 
typedef struct{
    unsigned long    biSize;  
    long             biWidth;  
    long             biHeight;  
    unsigned short   biPlanes;  
    unsigned short   biBitCount;  
    unsigned long    biCompression;  
    unsigned long    biSizeImage;  
    long             biXPelsPerMeter;  
    long             biYPelsPerMeter;  
    unsigned long    biClrUsed;  
    unsigned long    biClrImportant;  
} BMPINFOHEADER_T;  

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))


void rgb_to_bmp(unsigned char* pdata, FILE* bmp_fd);     
long rgb_to_jpeg(const char *rgb, char *jpeg);
long rgb_to_jpeg_v2(const char *rgb, char *jpeg, char * filename, int quality);  
int convert_yuv_to_rgb_pixel(int y, int u, int v);
int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
void rgb_stretch(char* src_buf, char* dest_buf, int des_width, int des_hight);  


#endif