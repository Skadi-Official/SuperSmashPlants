#ifndef _UTIL_H_
#define _UTIL_H_
#include <graphics.h>
/*
util 头文件通常用于存放实用工具函数、宏定义、类型定义和其他一些通用功能，这些功能在多个地方可能会被重用。
它们通常与具体的类或模块无关，而是提供了一些独立且通用的功能。
*/
#pragma comment(lib, "MSIMG32.LIB")

// 将图像水平翻转
inline void flip_image(IMAGE* src, IMAGE* dst)
{
	int w = src->getwidth(); // 获取宽度
	int h = src->getheight();// 获取高度
	Resize(dst, w, h); // 调整目标图像的大小，使其与源图像的宽度和高度相同
	DWORD* src_buffer = GetImageBuffer(src); 
	DWORD* dst_buffer = GetImageBuffer(dst); // 获取源图像和目标图像的像素缓冲区指针
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int idx_src = y * w + x;
			int idx_dst = y * w + (w - x - 1);
			dst_buffer[idx_dst] = src_buffer[idx_src];
		}
	}
}

inline void putimage_alpha(int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), dst_x, dst_y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0 ,255, AC_SRC_ALPHA });
}

inline void putimage_alpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	const Vector2& pos_camera = camera.get_position();
	AlphaBlend(GetImageHDC(NULL), (int)(dst_x - pos_camera.x), (int)(dst_y - pos_camera.y), w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0 ,255, AC_SRC_ALPHA });
}

inline void putimage_alpha(int dst_x, int dst_y,int width, int height, IMAGE* img, int src_x, int src_y)
{
	int w = width >  0 ? width : img->getwidth();
	int h = height > 0 ? height: img->getheight();
	AlphaBlend(GetImageHDC(NULL), dst_x, dst_y, w, h,
		GetImageHDC(img), src_x, src_y, w, h, { AC_SRC_OVER, 0 ,255, AC_SRC_ALPHA });
}

inline void line(const Camera& camera, int x1, int y1, int x2, int y2)
{
	const Vector2& pos_camera = camera.get_position();
	line((int)(x1 - pos_camera.x), (int)(y1 - pos_camera.y), (int)(x2 - pos_camera.x), (int)(y2 - pos_camera.y));
}

inline void sketch_image(IMAGE* src, IMAGE* dst)
{
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int idx = y * w + x;
			dst_buffer[idx] = BGR(RGB(255, 255, 255) | (src_buffer[idx] & 0xFF000000));
		}
	}
}

#endif // !_UTIL_H_
