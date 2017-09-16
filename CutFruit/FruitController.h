#pragma once
#include "SpriteController.h"
#include "MyGameEngine.h"
class FruitController : public SpriteController {
public:
	FruitController(MouseBrush *pMouseBrush);
	~FruitController();
	void UpdateSprites();

};