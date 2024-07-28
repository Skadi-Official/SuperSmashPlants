#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"
#include "scene_manager.h"
#include "util.h"
#include "platform.h"
#include "player.h"
#include "status_bar.h"
#include <vector>
#include <iostream>

extern Player* player_1;
extern Player* player_2;
extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;
extern IMAGE img_pea;

extern Camera main_camera;
extern std::vector<Platform> platform_list;

extern IMAGE* img_player_1_avater;
extern IMAGE* img_player_2_avater;

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter()
	{	
		status_bar_1P.set_avater(img_player_1_avater);
		status_bar_2P.set_avater(img_player_2_avater);

		status_bar_1P.set_position(235, 625);
		status_bar_2P.set_position(675, 625);

		player_1->set_position(200, 50);
		player_2->set_position(975, 50);
		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;
		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

		platform_list.resize(4); // 只有四个平台，直接设置

		/*
		large_platform 变成了 platform_list 中第一个元素的别名，
		通过 large_platform 可以直接操作 platform_list 中的第一个元素。
		任何对 large_platform 的修改都会直接影响 platform_list[0]。
		*/
		Platform& large_platform = platform_list[0];
		large_platform.img = &img_platform_large;
		large_platform.render_position.x = 122;
		large_platform.render_position.y = 455;
		large_platform.shape.left = (float)large_platform.render_position.x + 30;
		large_platform.shape.right = (float)large_platform.render_position.x + img_platform_large.getwidth() - 30;
		large_platform.shape.y = (float)large_platform.render_position.y + 60;

		Platform& small_platform_1 = platform_list[1];
		small_platform_1.img = &img_platform_small;
		small_platform_1.render_position.x = 175;
		small_platform_1.render_position.y = 360;
		small_platform_1.shape.left = (float)small_platform_1.render_position.x + 40;
		small_platform_1.shape.right = (float)small_platform_1.render_position.x + img_platform_small.getwidth() - 40;
		small_platform_1.shape.y = (float)small_platform_1.render_position.y + img_platform_small.getheight() / 2;



		Platform& small_platform_2 = platform_list[2];
		small_platform_2.img = &img_platform_small;
		small_platform_2.render_position.x = 855;
		small_platform_2.render_position.y = 360;
		small_platform_2.shape.left = (float)small_platform_2.render_position.x + 40;
		small_platform_2.shape.right = (float)small_platform_2.render_position.x + img_platform_small.getwidth() - 40;
		small_platform_2.shape.y = (float)small_platform_2.render_position.y + img_platform_small.getheight() / 2;

		Platform& small_platform_3 = platform_list[3];
		small_platform_3.img = &img_platform_small;
		small_platform_3.render_position.x = 515;
		small_platform_3.render_position.y = 225;
		small_platform_3.shape.left = (float)small_platform_3.render_position.x + 40;
		small_platform_3.shape.right = (float)small_platform_3.render_position.x + img_platform_small.getwidth() - 40;
		small_platform_3.shape.y = (float)small_platform_3.render_position.y + img_platform_small.getheight() / 2;
	}

	void on_update(int delta)
	{
		player_1->on_update(delta);
		player_2->on_update(delta);

		main_camera.on_update(delta);

		bullet_list.erase(
			std::remove_if(
				bullet_list.begin(),		// 需要检查的范围的开始
				bullet_list.end(),			// 需要检查的范围的结束
				[](const Bullet* bullet)	// Lambda 表达式，用来判断是否需要删除
				{
					bool deletable = bullet->check_can_remove();
					if (deletable)
					{
						delete bullet;
					}
					return deletable;
				}),
			bullet_list.end()				// 需要删除的范围的结束
		);

		for (Bullet* bullet : bullet_list)
		{
			//std::cout << "bullet_list on_update" << std::endl;
			bullet->on_update(delta);
		}
		
		status_bar_1P.set_hp(player_1->get_hp());
		status_bar_1P.set_mp(player_1->get_mp());
		status_bar_2P.set_hp(player_2->get_hp());
		status_bar_2P.set_mp(player_2->get_mp());
	}

	void on_draw(const Camera& camera)
	{
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

		for (const Platform& platform : platform_list)
			platform.on_draw(camera);

		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("已开启调试模式，按Q关闭"));
		}

		player_1->on_draw(camera);
		player_2->on_draw(camera);

		for (Bullet* bullet : bullet_list)
		{	
			//std::cout << "bullet_list on_draw" << std::endl;
			bullet->on_draw(camera);
		}
		
		status_bar_1P.on_draw();
		status_bar_2P.on_draw();
	}

	void on_input(const ExMessage& msg)
	{
		player_1->on_input(msg);
		player_2->on_input(msg);

		switch (msg.message)
		{
		case WM_KEYUP:
			// Q
			if (msg.vkcode == 0x51)
				is_debug = !is_debug;
			break;
		default:
			break;
		}
	}

	void on_exit()
	{
		
	}
private:
	 POINT pos_img_sky = { 0 };		// 天空背景图位置
	 POINT pos_img_hills = { 0 };	// 山脉背景图位置	

	 StatusBar status_bar_1P;		// 玩家 1 的状态条
	 StatusBar status_bar_2P;		// 玩家 2 的状态条
};
#endif // !_GAME_SCENE_H_
