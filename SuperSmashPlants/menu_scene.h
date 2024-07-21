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
		
	}

	void on_update(int delta) 
	{ 
		
	}

	void on_draw(const Camera& camera) 
	{ 
		putimage(0, 0, &img_menu_background);
	}

	void on_input(const ExMessage& msg)
	{
		
	}


private:
	Timer timer;
};


#endif // !_MENU_SCENE_H_
