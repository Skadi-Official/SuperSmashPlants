#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include"atlas.h"
#include"util.h"
#include"camera.h"

#include<graphics.h>
#include<functional>

class Animation
{
public:
	Animation() = default;
	~Animation() = default;

	// ���ö�����״̬
	void reset()
	{
		timer = 0;
		idx_frame = 0;
	}

	// ���ö���ʹ�õ�ͼ��
	void set_atlas(Atlas* new_atlas)
	{
		reset(); // �����ظ����ò���֡����Խ������
		atlas = new_atlas;
	}

	// ���ö����Ƿ�ѭ������
	void set_loop(bool flag)
	{
		is_loop = flag;
	}

	// ���ö���֡���
	void set_interval(int ms)
	{
		interval = ms;
	}

	// ��ȡ��ǰ֡����
	int get_idx_frame()
	{
		return idx_frame;
	}

	// ��ȡ��ǰ֡ͼƬ
	IMAGE* get_frame()
	{
		return atlas->get_image(idx_frame);
	}

	// ����Ƿ񲥷Ž���
	bool check_finished()
	{
		if (is_loop) return false;

		return (idx_frame == atlas->get_size() - 1);
	}

	// ������ĺ������󱣴浽������� callback ��
	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

	// �������£��ı�index���ı���Ƶ�ͼ
	void on_update(int delta)
	{
		timer += delta;
		if (timer >= interval)
		{
			timer = 0;
			idx_frame++;
			if (idx_frame >= atlas->get_size())
			{
				idx_frame = is_loop ? 0 : atlas->get_size() - 1;

				if (!is_loop && callback)
					callback();
			}
		}
	}

	// ����
	void on_draw(const Camera& camera, int x, int y)const
	{
		putimage_alpha(camera, x, y, atlas->get_image(idx_frame));
	}

	
private:
	int timer = 0;			// ��ʱ��
	int interval = 0;		// ֡���
	int idx_frame = 0;		// ֡����
	bool is_loop = true;	// �Ƿ�ѭ�� 
	Atlas* atlas = nullptr;
	std::function<void()> callback;
};

#endif // !_ANIMATION_H_
