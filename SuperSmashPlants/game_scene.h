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

extern IMAGE img_1P_winner;
extern IMAGE img_2P_winner;
extern IMAGE img_winner_bar;

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
		is_game_over = false;
		is_slide_out_started = false;

		// 结束提示信息文本框位置
		pos_img_winner_bar.x = -img_winner_bar.getwidth();
		pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;

		// 结束提示信息文本位置
		pos_img_winner_text.x = pos_img_winner_bar.x;
		pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;

		// 结算动效滑入定时器的初始化设置
		timer_winner_slide_in.restart();
		timer_winner_slide_in.set_wait_time(3500);
		timer_winner_slide_in.set_one_shot(true);
		timer_winner_slide_in.set_callback([&]()
			{
				is_slide_out_started = true;
			});

		// 结算动效滑出定时器的初始化设置，滑出结束后返回主菜单
		timer_winner_slide_out.restart();
		timer_winner_slide_out.set_wait_time(1000);
		timer_winner_slide_out.set_one_shot(true);
		timer_winner_slide_out.set_callback([&]()
			{
				scene_manager.switch_to(SceneManager::SceneType::Menu);
			});

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

		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);
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
		

		// 获取两个玩家位置并且判断他们是否已经掉落到深渊中，如果已经掉落要将其 hp 设置为 0
		const Vector2& position_player_1 = player_1->get_position();
		const Vector2& position_player_2 = player_2->get_position();
		if (position_player_1.y >= getheight())
		{
			player_1->set_hp(0);
		}
		if (position_player_2.y >= getheight())
		{
			player_2->set_hp(0);
		}

		// 任意一方血量归零游戏结束
		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0)
		{
			// 游戏结束后的第一帧，停止局内音乐，播放获胜音乐
			if (is_game_over == false)
			{
				mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
				mciSendString(_T("play ui_win from 0"), NULL, 0, NULL);
			}

			is_game_over = true;
		}

		// 为状态栏提供渲染所需要的值
		status_bar_1P.set_hp(player_1->get_hp());
		status_bar_1P.set_mp(player_1->get_mp());
		status_bar_2P.set_hp(player_2->get_hp());
		status_bar_2P.set_mp(player_2->get_mp());

		if (is_game_over)
		{
			pos_img_winner_bar.x += (int)(speed_winner_bar * delta);
			pos_img_winner_text.x += (int)(speed_winner_text * delta);

			if (is_slide_out_started == false)
			{
				timer_winner_slide_in.on_update(delta);

				if (pos_img_winner_bar.x > pos_x_img_winner_bar_dst)
				{
					pos_img_winner_bar.x = pos_x_img_winner_bar_dst;
				}
				if (pos_img_winner_text.x > pos_x_img_winner_text_dst)
				{
					pos_img_winner_text.x = pos_x_img_winner_text_dst;
				}

			}
			else
			{
				timer_winner_slide_out.on_update(delta);
			}
		}

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
		
		if (is_game_over)
		{
			putimage_alpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
			if (player_1->get_hp() > 0)
			{
				putimage_alpha(pos_img_winner_text.x, pos_img_winner_text.y, &img_1P_winner);
			}
			else
			{
				putimage_alpha(pos_img_winner_text.x, pos_img_winner_text.y, &img_2P_winner);
			}
		}
		else
		{
			status_bar_1P.on_draw();
			status_bar_2P.on_draw();
		}

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
		delete player_1;
		player_1 = nullptr;
		delete player_2;
		player_2 = nullptr;

		is_debug = false;

		bullet_list.clear();
		main_camera.reset();
	}
private:
	const float speed_winner_bar = 3.0f;		// 结算动效背景移动速度
	const float speed_winner_text = 3.0f;		// 结算动效文本移动速度

private:
	 POINT pos_img_sky = { 0 };		// 天空背景图位置
	 POINT pos_img_hills = { 0 };	// 山脉背景图位置	

	 StatusBar status_bar_1P;		// 玩家 1 的状态条
	 StatusBar status_bar_2P;		// 玩家 2 的状态条

	 bool is_game_over = false;		// 游戏是否结束

	 POINT pos_img_winner_bar = { 0 };		// 结算动效背景位置
	 POINT pos_img_winner_text = { 0 };		// 结算动效文本位置
	 int pos_x_img_winner_bar_dst = 0;		// 结算动效背景移动的目标位置（停下的位置）
	 int pos_x_img_winner_text_dst = 0;		// 结算动效文本移动的目标位置
	 Timer timer_winner_slide_in;			// 结算动效滑入定时器
	 Timer timer_winner_slide_out;			// 结算动效滑出定时器
	 bool is_slide_out_started = false;		// 结算动效是否开始滑出
};
#endif // !_GAME_SCENE_H_
