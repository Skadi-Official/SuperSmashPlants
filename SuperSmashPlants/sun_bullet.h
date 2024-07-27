#ifndef _SUN_BULLET_H_
#define _SUN_BULLET_H_
#include"bullet.h"
#include"animation.h"

extern Atlas atlas_sun;
extern Atlas atlas_sun_explode;

extern Camera main_camera;

class SunBullet : public Bullet

{
public:
	SunBullet()
	{
		size.x = 96, size.y = 96;

		damege = 20;

		animation_idle.set_atlas(&atlas_sun);
		animation_idle.set_interval(50);

		animation_explode.set_atlas(&atlas_sun_explode);
		animation_explode.set_interval(50);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]() {can_remove = true; });

		IMAGE* frame_idle = animation_idle.get_frame();
		IMAGE* frame_explode = animation_explode.get_frame();

		// 爆炸特效略大于日光素材，需要在日光素材更靠左上的位置绘制
		explode_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;
	}
	~SunBullet() = default;

	void on_collide()
	{
		Bullet::on_collide();

		main_camera.shake(5, 250);

		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);
	}

	void on_update(int delta)
	{	
		//std::cout << "sun bullet update" << std::endl;
		if (valid)
		{
			velocity.y += gravity * delta;
			position += velocity * (float)delta;
		}

		if (!valid)
		{
			animation_explode.on_update(delta);
		}
		else
		{
			animation_idle.on_update(delta);
		}

		if (check_if_exceeds_screen())
		{
			can_remove = true;
		}
	}

	void on_draw(const Camera& camera) const
	{
		//std::cout << "sun_bullet drawing" << std::endl;
		if (valid)
		{
			animation_idle.on_draw(camera, (int)position.x, (int)position.y);
		}
		else
		{
			animation_explode.on_draw(camera,
				(int)(position.x + explode_render_offset.x),
				(int)(position.y + explode_render_offset.y));
		}
	}

private:
	const float gravity = 1e-3f;		// 日光炸弹重力
private:
	Animation animation_idle;			// 日光炸弹默认动画
	Animation animation_explode;		// 日光炸弹爆炸动画
	Vector2 explode_render_offset;		// 爆炸动画渲染偏移，爆炸特效的素材大小与日光不一致，需要偏移量保证二者矩形中心一致
	
};

#endif // !_SUN_BULLET_H_
