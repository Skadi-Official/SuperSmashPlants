#ifndef _PLATFORM_T_
#define _PLATFORM_T_

#include "util.h"
#include "camera.h"

extern bool is_debug;

class Platform
{
public:
	// ��ײ����
	struct CollisionShape 
	{
		float y;			// ƽ̨��ЧΪһ���ߵ�y������
		float left, right;	// ƽ̨��ЧΪһ������x���������յ�
	};

public:
	CollisionShape shape;			// ƽ̨��ײ��״
	IMAGE* img = nullptr;			// ƽ̨ͼƬ
	POINT render_position = { 0 };	// ƽ̨ͼƬ��Ⱦʱ��λ��
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

