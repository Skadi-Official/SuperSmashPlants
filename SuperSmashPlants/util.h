#ifndef _UTIL_H_
#define _UTIL_H_
#include <graphics.h>

#pragma comment(lib, "MSIMG32.LIB")

// ��ͼ��ˮƽ��ת
inline void flip_image(IMAGE* src, IMAGE* dst)
{
	int w = src->getwidth(); // ��ȡ���
	int h = src->getheight();// ��ȡ�߶�
	Resize(dst, w, h); // ����Ŀ��ͼ��Ĵ�С��ʹ����Դͼ��Ŀ�Ⱥ͸߶���ͬ
	DWORD* src_buffer = GetImageBuffer(src); 
	DWORD* dst_buffer = GetImageBuffer(dst); // ��ȡԴͼ���Ŀ��ͼ������ػ�����ָ��
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



#endif // !_UTIL_H_
