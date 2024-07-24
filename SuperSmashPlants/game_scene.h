#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"
#include "scene_manager.h"
#include "util.h"
extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern Camera main_camera;
extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter()
	{
		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;
		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;
	}

	void on_update(int delta)
	{
		
	}

	void on_draw(const Camera& camera)
	{
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);
	}

	void on_input(const ExMessage& msg)
	{
		
	}

	void on_exit()
	{
		
	}
private:
	 POINT pos_img_sky = { 0 };		// Ìì¿Õ±³¾°Í¼Î»ÖÃ
	 POINT pos_img_hills = { 0 };	// É½Âö±³¾°Í¼Î»ÖÃ	
};
#endif // !_GAME_SCENE_H_
