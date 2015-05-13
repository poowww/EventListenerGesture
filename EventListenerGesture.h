//
//  EventListenerGesture.h
//
//
//  Licence: MIT
//  Copyright (c) 2015 Shotaro Takagi (poowww)
//

#ifndef __SnowEdge__EventListenerGesture__
#define __SnowEdge__EventListenerGesture__

#include "cocos2d.h"


class EventListenerGesture : public cocos2d::EventListenerTouchOneByOne
{
public:
    enum class SwipeDirection
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    enum class GestureType
    {
        NONE,
        TAP,
        LONG_TAP,
        SWIPE
    };

    static EventListenerGesture* create();

    virtual ~EventListenerGesture();

    virtual EventListenerGesture* clone() override;
    virtual bool checkAvailable() override;

    void setLongTapThreshouldSeconds(const float& threshould);
    void setSwipeThreshouldDistance(const float& threshould);

    std::function<void(cocos2d::Vec2)> onTap;
    std::function<void(cocos2d::Vec2)> onLongTapBegan;
    std::function<void(cocos2d::Vec2)> onLongTapEnded;
    std::function<void(SwipeDirection)> onSwipe;

protected:
    EventListenerGesture();
    bool init();

private:
    using EventListenerTouchOneByOne::LISTENER_ID;
    using EventListenerTouchOneByOne::onTouchBegan;
    using EventListenerTouchOneByOne::onTouchMoved;
    using EventListenerTouchOneByOne::onTouchEnded;
    using EventListenerTouchOneByOne::onTouchCancelled;

    float _longTapThresholdSeconds;
    float _swipeThresholdDistance;

    std::chrono::system_clock::time_point _touchStartTime;
    std::chrono::system_clock::time_point _beforeTapEndTime;
    cocos2d::Vec2 _touchStartPos;
    cocos2d::Vec2 _touchNowPos;
    GestureType _gestureType;
    bool _isTouched;
    void _updateInTouch(float);

    friend class EventDispatcher;
};

#endif /* defined(__SnowEdge__EventListenerGesture__) */