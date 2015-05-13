# EventListenerGesture
cocos2d-x3 EventListener for Gesture(Tap, LongTap, Swipe).



## Example

```c++

auto listener = EventListenerGesture::create();

//if dont set this value is 0.1f.
listener->setLongTapThreshouldSeconds(1.0f);
//if dont set this value is 10.0f.
listener->setSwipeThreshouldDistance(150.0f);
listener->onTap = [](Vec2 vec2){log("onTap called.");};
listener->onLongTapBegan = [](Vec2 vec2){log("onLongTapBegan called.");};
listener->onLongTapEnded = [](Vec2 vec2){log("onLongTapEnded called.");};
listener->onSwipe = [](EventListenerGesture::SwipeDirection direction)
{
   switch (direction) {
       case EventListenerGesture::SwipeDirection::UP:
           log("Swipe UP.");
           break;
       case EventListenerGesture::SwipeDirection::DOWN:
           log("Swipe DOWN.");
           break;
       case EventListenerGesture::SwipeDirection::RIGHT:
           log("Swipe RIGHT.");
           break;
       case EventListenerGesture::SwipeDirection::LEFT:
           log("Swipe LEFT.");
           break;
       default:
           break;
   }
};

getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

```
