#ifndef _PLATFORM_T_
#define _PLATFORM_T_

#include "util.h"
#include "camera.h"

extern bool is_debug;

class Platform
{
public:
	// 碰撞外形
	struct CollisionShape 
	{
		float y;			// 平台等效为一条线的y轴坐标
		float left, right;	// 平台等效为一条线在x轴的起点与终点
	};

public:
	CollisionShape shape;			// 平台碰撞形状
	IMAGE* img = nullptr;			// 平台图片
	POINT render_position = { 0 };	// 平台图片渲染时的位置
public:
	Platform() = default;
	~Platform() = default;

	void on_draw(const Camera& camera) const
	{
		putimage_alpha(camera, render_position.x, render_position.y, img);

		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
		}
	}


};


#endif // !_PLATFORM_T_

