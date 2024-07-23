#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include "scene.h"
#include "animation.h"
#include "scene_manager.h"

extern IMAGE img_VS;									// VS ������ͼƬ
extern IMAGE img_1P;									// 1P �ı�ͼƬ
extern IMAGE img_2P;									// 2P �ı�ͼƬ
extern IMAGE img_1P_desc;								// 1P ��λ����ͼƬ
extern IMAGE img_2P_desc;								// 2P ��λ����ͼƬ
extern IMAGE img_gravestone_left;						// �����Ĺ��ͼƬ
extern IMAGE img_gravestone_right;						// ���ҵ�Ĺ��ͼƬ
extern IMAGE img_selector_tip;							// ѡ�ǽ�����ʾ��Ϣ�ı�ͼƬ
extern IMAGE img_selector_background;					// ѡ�ǽ��汳��ͼ
extern IMAGE img_1P_selector_btn_idle_left;				// 1P ����ѡ��ťĬ��״̬ͼƬ
extern IMAGE img_1P_selector_btn_idle_right;			// 1P ����ѡ��ťĬ��״̬ͼƬ
extern IMAGE img_1P_selector_btn_down_left;				// 1P ����ѡ��ť����״̬ͼƬ
extern IMAGE img_1P_selector_btn_down_right;			// 1P ����ѡ��ť����״̬ͼƬ
extern IMAGE img_2P_selector_btn_idle_left;				// 2P ����ѡ��ťĬ��״̬ͼƬ
extern IMAGE img_2P_selector_btn_idle_right;			// 2P ����ѡ��ťĬ��״̬ͼƬ
extern IMAGE img_2P_selector_btn_down_left;				// 2P ����ѡ��ť����״̬ͼƬ
extern IMAGE img_2P_selector_btn_down_right;			// 2P ����ѡ��ť����״̬ͼƬ
extern IMAGE img_peashooter_selector_background_left;	// ѡ�ǽ��泯������㶹���ֱ���ͼƬ
extern IMAGE img_peashooter_selector_background_right;	// ѡ�ǽ��泯���ҵ��㶹���ֱ���ͼƬ
extern IMAGE img_sunflower_selector_background_left;	// ѡ�ǽ��泯��������տ�����ͼƬ
extern IMAGE img_sunflower_selector_background_right;	// ѡ�ǽ��泯���ҵ����տ�����ͼƬ

extern Atlas atlas_peashooter_idle_right;				// �㶹���ֳ����ҵ�Ĭ�϶���ͼ��
extern Atlas atlas_sunflower_idle_right;				// ���տ����ҵ�Ĭ�϶���ͼ��

extern IMAGE img_avatar_peashooter;						// �㶹����ͷ��ͼƬ
extern IMAGE img_avatar_sunflower;						// ���տ�ͷ��ͼƬ

extern SceneManager scene_manager;						// ����������

class SelectorScene : public Scene
{
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	void on_enter()
	{
		animation_peashooter.set_atlas(&atlas_peashooter_idle_right);
		animation_sunflower.set_atlas(&atlas_sunflower_idle_right);
		animation_peashooter.set_interval(100);
		animation_sunflower.set_interval(100);

		static const int OFFSET_X = 50;

		pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;
		pos_img_VS.y = (getheight() - img_VS.getheight()) / 2;

		pos_img_tip.x = (getwidth() - img_selector_tip.getwidth()) / 2;
		pos_img_tip.y = getheight() - 125;

		pos_img_1P.x = (getwidth() / 2 - img_1P.getwidth()) / 2 - OFFSET_X;
		pos_img_1P.y = 35;
		pos_img_2P.x = getwidth() / 2 + (getwidth() / 2 - img_2P.getwidth()) / 2 + OFFSET_X;
		pos_img_2P.y = pos_img_1P.y;

		pos_img_1P_desc.x = (getwidth() / 2 - img_1P_desc.getwidth()) / 2 - OFFSET_X;
		pos_img_1P_desc.y = getheight() - 150;
		pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2P_desc.getwidth()) / 2 + OFFSET_X;
		pos_img_2P_desc.y = pos_img_1P_desc.y;

		pos_img_1P_gravestone.x = (getwidth() / 2 - img_gravestone_right.getwidth()) / 2 - OFFSET_X;
		pos_img_1P_gravestone.y = pos_img_1P.y + img_1P.getheight() + 35;
		pos_img_2P_gravestone.x = getwidth() / 2 + (getwidth() / 2 - img_gravestone_left.getwidth()) / 2 + OFFSET_X;
		pos_img_2P_gravestone.y = pos_img_1P_gravestone.y;

		pos_animation_1P.x = (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 - OFFSET_X;
		pos_animation_1P.y = pos_img_1P_gravestone.y + 80;
		pos_animation_2P.x = getwidth() / 2 + (getwidth() / 2 + atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 - OFFSET_X;
		pos_animation_2P.y = pos_animation_1P.y;
		pos_img_1P_name.y = pos_animation_1P.y + 155;
		pos_img_2P_name.y = pos_img_1P_name.y;
		pos_1P_selector_btn_left.x = pos_img_1P_gravestone.x - img_1P_selector_btn_idle_left.getwidth();
		pos_1P_selector_btn_left.y = pos_img_1P_gravestone.y + (img_gravestone_right.getheight() - img_1P_selector_btn_down_left.getheight()) / 2;
		pos_1P_selector_btn_right.x = pos_img_1P_gravestone.x + img_gravestone_right.getwidth();
		pos_1P_selector_btn_right.y = pos_1P_selector_btn_left.y;
		pos_2P_selector_btn_left.x = pos_img_2P_gravestone.x - img_2P_selector_btn_idle_left.getwidth();
		pos_2P_selector_btn_left.y = pos_1P_selector_btn_left.y;
		pos_2P_selector_btn_right.x = pos_img_2P_gravestone.x + img_gravestone_left.getwidth();
		pos_2P_selector_btn_right.y = pos_1P_selector_btn_left.y;
	}

	void on_update(int delta)
	{
		animation_peashooter.on_update(delta);
		animation_sunflower.on_update(delta);

		selector_background_scroll_offset_x += 1; // ÿ�������������ƶ�������صľ���
		if (selector_background_scroll_offset_x >= img_peashooter_selector_background_left.getwidth())
			selector_background_scroll_offset_x = 0;
	}

	void on_draw(const Camera& camera)
	{
		IMAGE* img_p1_seletor_background = nullptr;
		IMAGE* img_p2_seletor_background = nullptr;

		switch (player_type_2)
		{
		case SelectorScene::PlayerType::Peashooter:
			img_p1_seletor_background = &img_peashooter_selector_background_right;
			break;
		case SelectorScene::PlayerType::Sunflower:
			img_p1_seletor_background = &img_sunflower_selector_background_right;
			break;
		default:
			img_p1_seletor_background = &img_peashooter_selector_background_right;
			break;
		}

		switch (player_type_1)
		{
		case SelectorScene::PlayerType::Peashooter:
			img_p2_seletor_background = &img_peashooter_selector_background_left;
			break;
		case SelectorScene::PlayerType::Sunflower:
			img_p2_seletor_background = &img_sunflower_selector_background_left;
			break;
		default:
			img_p2_seletor_background = &img_peashooter_selector_background_left;
			break;
		}


		putimage(0, 0, &img_selector_background);

		// ƫ������ߵ�ͼƬ��������ࣩ������Ҫ����
		putimage_alpha(selector_background_scroll_offset_x - img_p1_seletor_background->getwidth(), 0, img_p1_seletor_background);
		// ƫ�����ұߵ�ͼƬ��������ࣩ,��Ҫ����
		putimage_alpha(selector_background_scroll_offset_x, 0, 
			img_p1_seletor_background->getwidth() - selector_background_scroll_offset_x, 0, img_p1_seletor_background, 0, 0);
		// ƫ������ߵ�ͼƬ�������Ҳࣩ����Ҫ����
		putimage_alpha(getwidth() - img_p2_seletor_background->getwidth(), 0, img_p2_seletor_background->getwidth() - selector_background_scroll_offset_x, 0, img_p2_seletor_background,
			selector_background_scroll_offset_x, 0);
		// ƫ�����ұߵ�ͼƬ�������Ҳࣩ������Ҫ����
		putimage_alpha(getwidth() - selector_background_scroll_offset_x, 0, img_p2_seletor_background);

		putimage_alpha(pos_img_VS.x, pos_img_VS.y, &img_VS);

		putimage_alpha(pos_img_1P.x, pos_img_1P.y, &img_1P);
		putimage_alpha(pos_img_2P.x, pos_img_2P.y, &img_2P);
		putimage_alpha(pos_img_1P_gravestone.x, pos_img_1P_gravestone.y, &img_gravestone_right);
		putimage_alpha(pos_img_2P_gravestone.x, pos_img_2P_gravestone.y, &img_gravestone_left);

		switch (player_type_1)
		{
		case SelectorScene::PlayerType::Peashooter:
			animation_peashooter.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_peashooter_name)) / 2;
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_peashooter_name);
			break;
		case SelectorScene::PlayerType::Sunflower:
			animation_sunflower.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_sunflower_name)) / 2;
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_sunflower_name);
			break;
		default:
			break;
		}

		switch (player_type_2)
		{
		case SelectorScene::PlayerType::Peashooter:
			animation_peashooter.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_peashooter_name)) / 2;
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_peashooter_name);
			break;
		case SelectorScene::PlayerType::Sunflower:
			animation_sunflower.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_sunflower_name)) / 2;
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_sunflower_name);
			break;
		default:
			break;
		}
		// 1P ��ѡ�ǰ�ť
		putimage_alpha(pos_1P_selector_btn_left.x, pos_1P_selector_btn_left.y,
			is_btn_1P_left_down ? &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);
		putimage_alpha(pos_1P_selector_btn_right.x, pos_1P_selector_btn_right.y,
			is_btn_1P_right_down ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
		// 2P ��ѡ�ǰ�ť
		putimage_alpha(pos_2P_selector_btn_left.x, pos_2P_selector_btn_left.y,
			is_btn_2P_left_down ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
		putimage_alpha(pos_2P_selector_btn_right.x, pos_2P_selector_btn_right.y,
			is_btn_2P_right_down ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);

		putimage_alpha(pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1P_desc);
		putimage_alpha(pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2P_desc);

		putimage_alpha(pos_img_tip.x, pos_img_tip.y, &img_selector_tip);
	}

	void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
			case 0x41:
				is_btn_1P_left_down = true;
				// A
				break;
			case 0x44:
				is_btn_1P_right_down = true;
				// D
				break;
			case VK_LEFT:
				is_btn_2P_left_down = true;
				// ��
				break;
			case VK_RIGHT:
				is_btn_2P_right_down = true;
				// ��
				break;
			}
			break;
		case WM_KEYUP:
			switch (msg.vkcode)
			{
			case 0x41:
				is_btn_1P_left_down = false;
				player_type_1 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_1 - 1) % (int)PlayerType::Invalid);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				// A
				break;
			case 0x44:
				is_btn_1P_right_down = false;
				player_type_1 = (PlayerType)(((int)player_type_1 + 1) % (int)PlayerType::Invalid);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				// D
				break;
			case VK_LEFT:
				is_btn_2P_left_down = false;
				player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_2 - 1) % (int)PlayerType::Invalid);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				// ��
				break;
			case VK_RIGHT:
				is_btn_2P_right_down = false;
				player_type_2 = (PlayerType)(((int)player_type_2 + 1) % (int)PlayerType::Invalid);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				// ��
				break;
			}
			break;
		default:
			break;
		}
	}

	void on_exit()
	{

	}

private:
	enum class PlayerType
	{
		Peashooter = 0,
		Sunflower,
		Invalid
	};

private:
	POINT pos_img_VS = { 0 };					// VS ������ͼƬλ��
	POINT pos_img_tip = { 0 };					// ��ʾ��Ϣ�ı�ͼƬλ��
	POINT pos_img_1P = { 0 };					// 1P �ı�ͼƬλ��
	POINT pos_img_2P = { 0 };					// 2P �ı�ͼƬλ��
	POINT pos_img_1P_desc = { 0 };				// 1P ��λ����ͼƬλ��
	POINT pos_img_2P_desc = { 0 };				// 2P ��λ����ͼƬλ��
	POINT pos_img_1P_name = { 0 };				// 1P ��ɫ�����ı�λ��
	POINT pos_img_2P_name = { 0 };				// 2P ��ɫ�����ı�λ��
	POINT pos_animation_1P = { 0 };				// 1P ��ɫ����λ��
	POINT pos_animation_2P = { 0 };				// 2P ��ɫ����λ��
	POINT pos_img_1P_gravestone = { 0 };		// 1P Ĺ��ͼƬλ��
	POINT pos_img_2P_gravestone = { 0 };		// 2P Ĺ��ͼƬλ��
	POINT pos_1P_selector_btn_left = { 0 };		// 1P �����л���ťλ��
	POINT pos_1P_selector_btn_right = { 0 };	// 1P �����л���ťλ��
	POINT pos_2P_selector_btn_left = { 0 };		// 2P �����л���ťλ��
	POINT pos_2P_selector_btn_right = { 0 };	// 2P �����л���ťλ��

	Animation animation_peashooter;				// �㶹���ֶ���
	Animation animation_sunflower;				// ���տ�����

	PlayerType player_type_1 = PlayerType::Peashooter;	// 1P ��ɫ����
	PlayerType player_type_2 = PlayerType::Sunflower;	// 2P ��ɫ����

	LPCTSTR str_peashooter_name = _T("�㶹����");	// �㶹���ֽ�ɫ����
	LPCTSTR str_sunflower_name = _T("���տ�");		// ���տ���ɫ����

	int selector_background_scroll_offset_x = 0;	// ������������룬��ֱ������ˮƽ�����Ϲ����ľ���

	bool is_btn_1P_left_down = false;			// 1P �����л���ť�Ƿ���
	bool is_btn_1P_right_down = false;			// 1P �����л���ť�Ƿ���
	bool is_btn_2P_left_down = false;			// 2P �����л���ť�Ƿ���
	bool is_btn_2P_right_down = false;			// 2P �����л���ť�Ƿ���

private:
	void outtextxy_shaded(int x, int y, LPCTSTR str)	// ���ƴ���Ӱ������
	{
		settextcolor(RGB(45, 45, 45));
		outtextxy(x + 3, y + 3, str);
		settextcolor(RGB(255, 255, 255));
		outtextxy(x, y, str);
	}
};



#endif // !_SELECTOR_SCENE_H_
