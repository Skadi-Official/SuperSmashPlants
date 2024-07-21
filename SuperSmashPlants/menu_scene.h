#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"
#include "scene_manager.h"

extern IMAGE img_menu_background;

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() 
	{ 
		mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);
	}

	void on_draw(const Camera& camera) 
	{ 
		putimage(0, 0, &img_menu_background);
	}

	void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYUP)
		{
			mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
			scene_manager.switch_to(SceneManager::SceneType::Selector); // 按下任意键进入选人菜单
		}
	}


private:
	Timer timer;
};


#endif // !_MENU_SCENE_H_
