/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.17
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "OptionsSence.h"
#include "Based/GlobalVariable.h"

OptionsMenu::OptionsMenu():
	_userDefault(UserDefault::getInstance()),
	_director(Director::getInstance())
{
}
 
OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::addLayer(Node* node)
{
	_dialogNode = node;
	_dialogNode->addChild(this, 1);
}

bool OptionsMenu::init()
{
	if (!Layer::init())return false;

	createShieldLayer(this);

	this->createDialog();

	return true;
}

void OptionsMenu::createDialog()
{
	_option = Sprite::createWithTexture(_director->getTextureCache()->getTextureForKey(_global->userInformation->getImagePath().find("options_menuback")->second));
	_option->setPosition(_director->getWinSize() / 2);
	_option->setScale(1.7f);
	_option->setName("_option");
	this->addChild(_option);
	
	/* 触摸移动监听 */
	createTouchtListener(_option);

	/* 创建滑动条 */
	this->createSlider(Vec2(250, 350), Vec2(140, 350), _global->userInformation->getGameText().find("音乐")->second, OptionScene_Slider::音乐, "options_sliderslot", "options_sliderslot", "options_sliderknob2");
	this->createSlider(Vec2(250, 310), Vec2(140, 310), _global->userInformation->getGameText().find("音效")->second, OptionScene_Slider::音效, "options_sliderslot", "options_sliderslot", "options_sliderknob2");

	/* 创建复选框 */
	this->createCheckBox(Vec2(300, 280), Vec2(140, 280), _global->userInformation->getGameText().find("信息")->second, OptionScene_CheckBox::显示信息, "options_checkbox0", "options_checkbox1");
	this->createCheckBox(Vec2(300, 245), Vec2(140, 245), _global->userInformation->getGameText().find("全屏")->second, OptionScene_CheckBox::全屏, "options_checkbox0", "options_checkbox1");
	this->createCheckBox(Vec2(300, 210), Vec2(140, 210), _global->userInformation->getGameText().find("高帧率")->second, OptionScene_CheckBox::高帧率, "options_checkbox0", "options_checkbox1");
	this->createCheckBox(Vec2(300, 175), Vec2(140, 175), _global->userInformation->getGameText().find("拉伸显示")->second, OptionScene_CheckBox::拉伸显示, "options_checkbox0", "options_checkbox1");

	/* 创建彩蛋 */
	if (_global->userInformation->getIsShowEggs() &&
		(UserDefault::getInstance()->getIntegerForKey(_global->userInformation->getSystemCaveFileName(_global->userInformation->getUserCaveFileNumber()).c_str()) >= 52 ||
			UserDefault::getInstance()->getIntegerForKey(_global->userInformation->getSystemDifCaveFileName(_global->userInformation->getUserCaveFileNumber()).c_str()) >= 52))
	{
		auto button = Button::create(_global->userInformation->getImagePath().find("button")->second, _global->userInformation->getImagePath().find("button_down")->second);
		button->setTitleLabel(label("制作者", 20, Vec2(0, 0), 0, Color3B::GRAY, 0.5f));
		button->setPosition(Vec2(210, 140));
		button->setScaleX(2.0f);
		_option->addChild(button);
		button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
			{
				switch (type)
				{
				case Widget::TouchEventType::BEGAN:
					AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
					break;
				case Widget::TouchEventType::ENDED:
					Director::getInstance()->pushScene(TransitionFade::create(0.5f, GameEasterEggs::createScene()));
					break;
				}
			});
	}

    /* 创建按钮 */
	this->createButton();
}

ControlSlider* OptionsMenu::createSlider(Vec2 &vec2, Vec2 &vec2_, const std::string &Label, OptionScene_Slider slider_type,const std::string bgFile, const std::string progressFile,const std::string thumbFile,const bool IsNew)
{
	/* 创建滑动条 */
	auto slider = ControlSlider::create((_global->userInformation->getImagePath().find(bgFile)->second).c_str(), (_global->userInformation->getImagePath().find(progressFile)->second).c_str(), (_global->userInformation->getImagePath().find(thumbFile)->second).c_str());
	slider->setMinimumValue(0);
	slider->setMaximumValue(100);
	slider->setPosition(vec2);
	_option->addChild(slider);

	switch (slider_type)
	{
	case OptionScene_Slider::音乐:
		slider->setValue(_global->userInformation->getBackGroundMusicVolume() * 100);
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(OptionsMenu::backGroundMusicVolumeChangeCallBack), Control::EventType::VALUE_CHANGED);
		break;
	case OptionScene_Slider::音效:
		slider->setValue(_global->userInformation->getSoundEffectVolume() * 100);
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(OptionsMenu::soundEffectMusicVolumeChangeCallBack), Control::EventType::VALUE_CHANGED);
		break;
	}

	/* 创建标签 */
	switch (IsNew)
	{
	case true:
		_option->addChild(label(Label, 30, vec2_, 0, Color3B::WHITE));
		break;
	case false:
		_option->addChild(label(Label, 20, vec2_, 0, Color3B::GRAY));
		break;
	}

	return slider;
}

ControlSlider* OptionsMenu::createSlider(Vec2& vec2, Vec2& vec2_, const std::string& Label, OptionScene_Slider slider_type, Sprite* BgFile, Sprite* progressFile, Sprite* thumbFile, const bool IsNew)
{
	/* 创建滑动条 */
	auto slider = ControlSlider::create(BgFile, progressFile, thumbFile);
	slider->setMinimumValue(0);
	slider->setMaximumValue(100);
	slider->setPosition(vec2);
	_option->addChild(slider);

	switch (slider_type)
	{
	case OptionScene_Slider::音乐:
		slider->setValue(_global->userInformation->getBackGroundMusicVolume() * 100);
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(OptionsMenu::backGroundMusicVolumeChangeCallBack), Control::EventType::VALUE_CHANGED);
		break;
	case OptionScene_Slider::音效:
		slider->setValue(_global->userInformation->getSoundEffectVolume() * 100);
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(OptionsMenu::soundEffectMusicVolumeChangeCallBack), Control::EventType::VALUE_CHANGED);
		break;
	default:
		break;
	}

	/* 创建标签 */
	switch (IsNew)
	{
	case true:
		_option->addChild(label(Label, 30, vec2_, 0, Color3B::WHITE));
		break;
	case false:
		_option->addChild(label(Label, 20, vec2_, 0, Color3B::GRAY));
		break;
	}

	return slider;
}

void OptionsMenu::backGroundMusicVolumeChangeCallBack(Ref* sender, Control::EventType type)
{
	auto slider = (ControlSlider*)sender;
	for (auto sp : _global->userInformation->getBackgroundMusic())  /* 循环设置音乐音量 */
	{
		AudioEngine::setVolume(sp, slider->getValue() / 100.0f);
	}

	_global->userInformation->setBackGroundMusicVolume(slider->getValue() / 100.0f);
	_userDefault->setFloatForKey("GLOBALMUSIC", _global->userInformation->getBackGroundMusicVolume());

	if (slider->getSelectedThumbSprite()->getContentSize().height == 40)
	{
		/* 旋转动画 */
		slider->getThumbSprite()->setRotation(slider->getValue() * 10);
		slider->getSelectedThumbSprite()->setRotation(slider->getValue() * 10);
	}	
}

void OptionsMenu::soundEffectMusicVolumeChangeCallBack(Ref* sender, Control::EventType type)
{
	/* 设置音效音量 */
	auto slider = (ControlSlider*)sender;
	_global->userInformation->setSoundEffectVolume(slider->getValue() / 100.0f);
	_userDefault->setFloatForKey("SOUNDEFFECT", _global->userInformation->getSoundEffectVolume());

	if (slider->getSelectedThumbSprite()->getContentSize().height == 40)
	{
		/* 旋转动画 */
		slider->getThumbSprite()->setRotation(slider->getValue() * 10);
		slider->getSelectedThumbSprite()->setRotation(slider->getValue() * 10);
	}
}

void OptionsMenu::deleteDialog()
{
	setMouseListenerEnable();
	_dialogNode->removeFromParent();
}

CheckBox* OptionsMenu::createCheckBox(Vec2 &vec2, Vec2 &vec2_, const std::string &Label, OptionScene_CheckBox button_type,
	const std::string NotSelect, const std::string Select, const bool IsNew)
{
	auto checkbox = CheckBox::create();
	checkbox->loadTextureBackGround(_global->userInformation->getImagePath().find(NotSelect)->second);
	checkbox->loadTextureFrontCross(_global->userInformation->getImagePath().find(Select)->second);
	checkbox->setPosition(vec2);
	_option->addChild(checkbox);

	switch (button_type)
	{
	case OptionScene_CheckBox::显示信息:
		checkbox->setSelected(_global->userInformation->getIsShowInformation() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	case OptionScene_CheckBox::全屏:
		checkbox->setSelected(_global->userInformation->getIsSelectFullScreen() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	case OptionScene_CheckBox::高帧率:
		checkbox->setSelected(_global->userInformation->getIsSelectHighFPS() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	case OptionScene_CheckBox::鼠标隐藏:
		checkbox->setSelected(_global->userInformation->getIsSelectCursorNotHide() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	case OptionScene_CheckBox::拉伸显示:
		checkbox->setSelected(_global->userInformation->getIsSelectStretchingShow() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	default:
		break;
	}
	
	checkbox->addEventListener([=](Ref* sender, CheckBox::EventType type)
	{
		switch (type)
		{
		case CheckBox::EventType::SELECTED:
			switch (button_type)
			{
			case OptionScene_CheckBox::显示信息: /* 显示信息 */
				_userDefault->setBoolForKey("SHOWINFORMATION", true);
				_global->userInformation->setIsShowInformation(CheckBox::EventType::SELECTED); /* 更新 */
				_director->setDisplayStats(true);
				break;
			case OptionScene_CheckBox::全屏: /* 全屏 */
				_userDefault->setBoolForKey("SHOWFULLSCREEN", true);
				_global->userInformation->setIsSelectFullScreen(CheckBox::EventType::SELECTED);
				((GLViewImpl*)_director->getOpenGLView())->setFullscreen();
				break;
			case OptionScene_CheckBox::高帧率: /* 高帧率 */
				_userDefault->setBoolForKey("SHOWHIGHFPS", true);
				_global->userInformation->setIsSelectHighFPS(CheckBox::EventType::SELECTED);
				_director->setAnimationInterval(1.0f / 60);
				_global->userInformation->setFps(60);
				break;
			case OptionScene_CheckBox::鼠标隐藏: /* 鼠标隐藏 */
				_userDefault->setBoolForKey("CURSORHIDE", true);
				_global->userInformation->setIsSelectCursorNotHide(CheckBox::EventType::SELECTED);
				break;
			case OptionScene_CheckBox::拉伸显示: /* 拉伸显示 */
				_userDefault->setBoolForKey("STRETCHINGSHOW", true);
				_director->getOpenGLView()->setDesignResolutionSize(_director->getWinSize().width, _director->getWinSize().height, ResolutionPolicy::EXACT_FIT);
				_global->userInformation->setIsSelectStretchingShow(CheckBox::EventType::SELECTED);
				break;
			default:
				break;
			}
			break; 
		case CheckBox::EventType::UNSELECTED:
			switch (button_type)
			{
			case OptionScene_CheckBox::显示信息:
				_userDefault->setBoolForKey("SHOWINFORMATION", false);
				_global->userInformation->setIsShowInformation(CheckBox::EventType::UNSELECTED); /* 更新 */
				_director->setDisplayStats(false);
				break;
			case OptionScene_CheckBox::全屏:
				_userDefault->setBoolForKey("SHOWFULLSCREEN", false);
				_global->userInformation->setIsSelectFullScreen(CheckBox::EventType::UNSELECTED);
				((GLViewImpl*)_director->getOpenGLView())->setWindowed(1280, 720);
				break;
			case OptionScene_CheckBox::高帧率:
				_userDefault->setBoolForKey("SHOWHIGHFPS", false);
				_global->userInformation->setIsSelectHighFPS(CheckBox::EventType::UNSELECTED);
				_director->setAnimationInterval(1.0f / 45);
				_global->userInformation->setFps(45);
				break;
			case OptionScene_CheckBox::鼠标隐藏:
				_userDefault->setBoolForKey("CURSORHIDE", false);
				_global->userInformation->setIsSelectCursorNotHide(CheckBox::EventType::UNSELECTED);
				break;
			case OptionScene_CheckBox::拉伸显示:
				_userDefault->setBoolForKey("STRETCHINGSHOW", false);
				_director->getOpenGLView()->setDesignResolutionSize(_director->getWinSize().width, _director->getWinSize().height, ResolutionPolicy::SHOW_ALL);
				_global->userInformation->setIsSelectStretchingShow(CheckBox::EventType::UNSELECTED);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	});

	switch (IsNew)
	{
	case true:
		_option->addChild(label(Label, 30, vec2_, 0, Color3B::WHITE));
		break;
	case false:
		_option->addChild(label(Label, 20, vec2_, 0, Color3B::GRAY));
		break;
	}
	
	return checkbox;
}

void OptionsMenu::createButton()
{
	auto button = Button::create(_global->userInformation->getImagePath().find("options_backtogamebutton0")->second, _global->userInformation->getImagePath().find("options_backtogamebutton2")->second);
	button->setTitleLabel(label(_global->userInformation->getGameText().find("返回")->second, 30));
	button->setPosition(Vec2(210, 55));
	_option->addChild(button);
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
			break;
		case Widget::TouchEventType::ENDED:
			_userDefault->flush();
			deleteDialog();
			break;
		}
	});
}