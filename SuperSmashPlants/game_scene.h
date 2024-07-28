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

		// ������ʾ��Ϣ�ı���λ��
		pos_img_winner_bar.x = -img_winner_bar.getwidth();
		pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;

		// ������ʾ��Ϣ�ı�λ��
		pos_img_winner_text.x = pos_img_winner_bar.x;
		pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;

		// ���㶯Ч���붨ʱ���ĳ�ʼ������
		timer_winner_slide_in.restart();
		timer_winner_slide_in.set_wait_time(3500);
		timer_winner_slide_in.set_one_shot(true);
		timer_winner_slide_in.set_callback([&]()
			{
				is_slide_out_started = true;
			});

		// ���㶯Ч������ʱ���ĳ�ʼ�����ã����������󷵻����˵�
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

		platform_list.resize(4); // ֻ���ĸ�ƽ̨��ֱ������

		/*
		large_platform ����� platform_list �е�һ��Ԫ�صı�����
		ͨ�� large_platform ����ֱ�Ӳ��� platform_list �еĵ�һ��Ԫ�ء�
		�κζ� large_platform ���޸Ķ���ֱ��Ӱ�� platform_list[0]��
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
				bullet_list.begin(),		// ��Ҫ���ķ�Χ�Ŀ�ʼ
				bullet_list.end(),			// ��Ҫ���ķ�Χ�Ľ���
				[](const Bullet* bullet)	// Lambda ���ʽ�������ж��Ƿ���Ҫɾ��
				{
					bool deletable = bullet->check_can_remove();
					if (deletable)
					{
						delete bullet;
					}
					return deletable;
				}),
			bullet_list.end()				// ��Ҫɾ���ķ�Χ�Ľ���
		);

		for (Bullet* bullet : bullet_list)
		{
			//std::cout << "bullet_list on_update" << std::endl;
			bullet->on_update(delta);
		}
		

		// ��ȡ�������λ�ò����ж������Ƿ��Ѿ����䵽��Ԩ�У�����Ѿ�����Ҫ���� hp ����Ϊ 0
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

		// ����һ��Ѫ��������Ϸ����
		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0)
		{
			// ��Ϸ������ĵ�һ֡��ֹͣ�������֣����Ż�ʤ����
			if (is_game_over == false)
			{
				mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
				mciSendString(_T("play ui_win from 0"), NULL, 0, NULL);
			}

			is_game_over = true;
		}

		// Ϊ״̬���ṩ��Ⱦ����Ҫ��ֵ
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
			outtextxy(15, 15, _T("�ѿ�������ģʽ����Q�ر�"));
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
	const float speed_winner_bar = 3.0f;		// ���㶯Ч�����ƶ��ٶ�
	const float speed_winner_text = 3.0f;		// ���㶯Ч�ı��ƶ��ٶ�

private:
	 POINT pos_img_sky = { 0 };		// ��ձ���ͼλ��
	 POINT pos_img_hills = { 0 };	// ɽ������ͼλ��	

	 StatusBar status_bar_1P;		// ��� 1 ��״̬��
	 StatusBar status_bar_2P;		// ��� 2 ��״̬��

	 bool is_game_over = false;		// ��Ϸ�Ƿ����

	 POINT pos_img_winner_bar = { 0 };		// ���㶯Ч����λ��
	 POINT pos_img_winner_text = { 0 };		// ���㶯Ч�ı�λ��
	 int pos_x_img_winner_bar_dst = 0;		// ���㶯Ч�����ƶ���Ŀ��λ�ã�ͣ�µ�λ�ã�
	 int pos_x_img_winner_text_dst = 0;		// ���㶯Ч�ı��ƶ���Ŀ��λ��
	 Timer timer_winner_slide_in;			// ���㶯Ч���붨ʱ��
	 Timer timer_winner_slide_out;			// ���㶯Ч������ʱ��
	 bool is_slide_out_started = false;		// ���㶯Ч�Ƿ�ʼ����
};
#endif // !_GAME_SCENE_H_
