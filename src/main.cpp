/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>

/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;

const float INITIAL_INTERVAL = 10;

float jumpscareScale = 4.0f;
float jumpscareInterval = 1;
int jumpscareChance = 10000;
bool raMode = false;

float jumpscareCountdown = 99999;
bool jumpscareAnimating = false;

const float JUMPSCARE_FRAME_TIME = 0.04;
const int JUMPSCARE_FRAME_COUNT = 14;

const int FRAME_WIDTH = 1024;
const int FRAME_HEIGHT = 768;

const int FRAME_X_OFFSET = 1;
const int FRAME_Y_OFFSET = 1;

bool garunteeJumpscare = false;

int jumpscareFrame = 0;
float frameCountdown = 0;

CCTexture2D* frames[14] = {};

CCSprite *jumpscareSprite = nullptr;

#include <Geode/modify/GameManager.hpp>
class $modify(MyGameManager, GameManager) {
	void update(float delta) {
		GameManager::update(delta);

		jumpscareCountdown = jumpscareCountdown - delta;

		if (jumpscareCountdown <= 0 && !jumpscareAnimating) {
			// Reset jumpscare interval
			jumpscareCountdown = jumpscareInterval;

			int random = 1 + (rand() % jumpscareChance);
			bool doJumpscare = random == 1;

			geode::log::info("Interval hit");
			auto randomString = std::to_string(random);
			std::cout << "Random number: " << randomString << std::endl;
			
			if (garunteeJumpscare) geode::log::info("Jumpscare garunteed");

			if (doJumpscare == true || garunteeJumpscare == true) MyGameManager::startJumpscare();
			garunteeJumpscare = false;
		}

		MyGameManager::updateJumpscare(delta);

		return;
	}

	void startJumpscare() {
		geode::log::info("Jumpscaring");
		if (raMode) {
			geode::log::info("RA MODE");
			auto scene = CCScene::create();
			//auto scene = CCDirector::sharedDirector()->getRunningScene();
			//scene->removeAllChildrenWithCleanup(true);
			auto raScene = SecretLayer4::create();
			scene->addChild(raScene);
			

			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, scene));
			return;
		}

		jumpscareAnimating = true;
		jumpscareFrame = 0;
		frameCountdown = JUMPSCARE_FRAME_TIME;

		// Play jumpscare sound
		FMODAudioEngine::sharedEngine()->playEffect("JumpscareSound.mp3"_spr);

		// Show sprite
		jumpscareSprite = CCSprite::create("Jumpscare0.png"_spr);
		//auto size = CCDirector::sharedDirector()->getWinSize();
		CCSize screenSize = CCDirector::get()->getWinSize();

		jumpscareSprite->retain();
		jumpscareSprite->setScale(jumpscareScale);
		jumpscareSprite->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
		auto scene = CCDirector::sharedDirector()->getRunningScene();
		if (scene == NULL) { geode::log::error("scene is NULL"); }
		scene->addChild(jumpscareSprite, 999);
	}

	void updateJumpscare(float delta) {
		if (!jumpscareAnimating) return;
		geode::log::info("Updating Jumpscare");

		if (jumpscareSprite == NULL) { geode::log::error("JumpscareSprite is NULL"); }

		frameCountdown -= delta;
		if (frameCountdown <= 0) {
			frameCountdown = JUMPSCARE_FRAME_TIME;
			jumpscareFrame += 1;

			if (jumpscareFrame >= JUMPSCARE_FRAME_COUNT) {
				geode::log::info("Reached last frame");
				MyGameManager::stopJumpscare();
				return;
			}
		}

		jumpscareSprite->setTexture(frames[jumpscareFrame]);

		/*int x = std::floor(jumpscareFrame / 8);
		int y = 8 - (jumpscareFrame % 8);

		int xPos = FRAME_X_OFFSET * (x + 1) + FRAME_WIDTH * x;
		int yPos = FRAME_Y_OFFSET * (y + 1) + FRAME_HEIGHT * y;

		std::cout << "(" << std::to_string(x) << "," << std::to_string(y) << ")" << std::endl;

		auto rect = CCRect(xPos, yPos, FRAME_WIDTH, FRAME_HEIGHT);
		jumpscareSprite->setTextureRect(rect);*/
	}

	void stopJumpscare() {
		geode::log::info("Stopping Jumpscare");
		jumpscareAnimating = false;
		jumpscareSprite->removeFromParentAndCleanup(true);
		jumpscareSprite->release();
	}
};

#include <Geode/modify/UILayer.hpp>
class $modify(MyUILayer, UILayer) {
	virtual void keyDown(cocos2d::enumKeyCodes p0) {
		UILayer::keyDown(p0);

		if (p0 == cocos2d::enumKeyCodes::KEY_Q) {
			jumpscareCountdown = 0;
			garunteeJumpscare = true;
		}
	}
};

$execute{
	frames[0] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare0.png"_spr, true);
	frames[1] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare1.png"_spr, true);
	frames[2] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare2.png"_spr, true);
	frames[3] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare3.png"_spr, true);
	frames[4] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare4.png"_spr, true);
	frames[5] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare5.png"_spr, true);
	frames[6] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare6.png"_spr, true);
	frames[7] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare7.png"_spr, true);
	frames[8] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare8.png"_spr, true);
	frames[9] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare9.png"_spr, true);
	frames[10] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare10.png"_spr, true);
	frames[11] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare11.png"_spr, true);
	frames[12] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare12.png"_spr, true);
	frames[13] = CCTextureCache::sharedTextureCache()->addImage("Jumpscare13.png"_spr, true);

	jumpscareInterval = Mod::get()->getSettingValue<float>("jumpscare-interval");
	jumpscareCountdown = INITIAL_INTERVAL;

	listenForSettingChanges("jumpscare-interval", [](float value) {
		jumpscareInterval = value;
		jumpscareCountdown = value;
	});

	jumpscareChance = Mod::get()->getSettingValue<int>("jumpscare-chance");
	listenForSettingChanges("jumpscare-chance", [](int value) {
		jumpscareChance = value;
	});

	raMode = Mod::get()->getSettingValue<bool>("chamber-of-time-mode");
	listenForSettingChanges("chamber-of-time-mode", [](bool value) {
		raMode = value;
	});

	jumpscareScale = Mod::get()->getSettingValue<float>("jumpscare-scale");
	listenForSettingChanges("jumpscare-scale", [](float value) {
		jumpscareScale = value;
	});
};