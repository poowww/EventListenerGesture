//
//  EventListenerGesture.h
//
//
//  Licence: MIT
//  Copyright (c) 2015 Shotaro Takagi (poowww)
//

#include "EventListenerGesture.h"

using namespace cocos2d;

constexpr float DefaultLongTapThresholdSeconds = 0.1f;
constexpr float DefaultSwipeThresholdDistance = 10.0f;

EventListenerGesture::EventListenerGesture()
: _longTapThresholdSeconds(DefaultLongTapThresholdSeconds)
, _swipeThresholdDistance(DefaultSwipeThresholdDistance)
, onTap(nullptr)
, onLongTapBegan(nullptr)
, onLongTapEnded(nullptr)
, onSwipe(nullptr)
, _gestureType(GestureType::NONE)
, _isTouched(false)
{
}

EventListenerGesture::~EventListenerGesture()
{
    CCLOGINFO("In the destructor of EventListenerGesture, %p", this);
}

bool EventListenerGesture::init()
{
    if (!EventListenerTouchOneByOne::init())
    {
        return false;
    }

    onTouchBegan = [this](Touch* touch, Event* event) -> bool
    {
        if(_isTouched)
        {
            return false;
        }

        _isTouched = true;
        _touchStartTime = std::chrono::system_clock::now();
        _touchStartPos = touch->getLocation();
        _touchNowPos = touch->getLocation();

        Director::getInstance()->getScheduler()->schedule(schedule_selector(EventListenerGesture::_updateInTouch), this, 0.05f, false);
        return true;
    };

    onTouchCancelled = [this](Touch* touch, Event* event)
    {
        _isTouched = false;
        _gestureType = GestureType::NONE;
    };

    onTouchMoved = [this](Touch* touch, Event* event)
    {
        if(_gestureType != GestureType::NONE)
        {
            return;
        }

        _touchNowPos = touch->getLocation();
        auto xDiff = _touchNowPos.x - _touchStartPos.x;
        auto xDiffAbs = abs(static_cast<int>(xDiff));
        auto yDiff = _touchNowPos.y - _touchStartPos.y;
        auto yDiffAbs = abs(static_cast<int>(yDiff));
        auto swipeDirection = SwipeDirection::NONE;

        if(xDiffAbs >= yDiffAbs)
        {
            if(xDiffAbs > _swipeThresholdDistance)
                swipeDirection = xDiff >= 0 ? SwipeDirection::RIGHT : SwipeDirection::LEFT;
        }
        else
        {
            if(yDiffAbs > _swipeThresholdDistance)
                swipeDirection = yDiff >= 0 ? SwipeDirection::UP : SwipeDirection::DOWN;
        }

        if(swipeDirection != SwipeDirection::NONE)
        {
            _gestureType = GestureType::SWIPE;
            if(onSwipe) onSwipe(swipeDirection, Vec2(xDiff, yDiff));
        }
    };

    onTouchEnded = [this](Touch* touch, Event* event)
    {
        if(_gestureType == GestureType::NONE)
        {
            if(onTap) onTap(touch->getDelta());
        }
        else if(_gestureType == GestureType::LONG_TAP)
        {
            if(onLongTapEnded) onLongTapEnded(touch->getDelta());
        }

        Director::getInstance()->getScheduler()->
            unschedule(schedule_selector(EventListenerGesture::_updateInTouch), this);
        _gestureType = GestureType::NONE;
        _isTouched = false;
    };

    return true;
}

EventListenerGesture* EventListenerGesture::create()
{
    auto ret = new (std::nothrow) EventListenerGesture();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void EventListenerGesture::setLongTapThreshouldSeconds(const float& threshould)
{
    if (threshould > 0)
    {
        _longTapThresholdSeconds = threshould;
    }
    else
    {
        CCLOGWARN("EventListenerGesture::LongTapThreshouldTime can set over 0.0f!");
    }
}

void EventListenerGesture::setSwipeThreshouldDistance(const float& threshould)
{
    if(threshould > 0)
    {
        _swipeThresholdDistance = threshould;
    }
    else
    {
        CCLOGWARN("EventListenerGesture::setSwipeThreshouldDistance can set over 0.0f!");
    }
}

bool EventListenerGesture::checkAvailable()
{
    if (onTouchBegan == nullptr)
    {
        CCASSERT(false, "Invalid EventListenerGesture!");
        return false;
    }

    return true;
}

EventListenerGesture* EventListenerGesture::clone()
{
    auto ret = new (std::nothrow) EventListenerGesture();
    if (ret && ret->init())
    {
        ret->autorelease();

        ret->onTap = onTap;
        ret->onLongTapBegan = onLongTapBegan;
        ret->onLongTapEnded = onLongTapEnded;
        ret->onSwipe = onSwipe;

        ret->_longTapThresholdSeconds = _longTapThresholdSeconds;
        ret->_swipeThresholdDistance = _swipeThresholdDistance;
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void EventListenerGesture::_updateInTouch(float)
{
    if(_gestureType != GestureType::NONE)
    {
        Director::getInstance()->getScheduler()->
            unschedule(schedule_selector(EventListenerGesture::_updateInTouch), this);
        return;
    }

    std::chrono::duration<float> duration = std::chrono::system_clock::now() - _touchStartTime;
    if(duration.count() > _longTapThresholdSeconds)
    {
        _gestureType = GestureType::LONG_TAP;
        if (onLongTapBegan) onLongTapBegan(_touchNowPos);
    }
}

