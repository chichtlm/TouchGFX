/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.5.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * This file is free software and is provided for example purposes. You may
 * use, copy, and modify within the terms and conditions of the license
 * agreement.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#include <gui/common/SwipeContainer.hpp>
#include <gui/common/Constants.hpp>
#include <touchgfx/EasingEquations.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>

using namespace touchgfx;

SwipeContainer::SwipeContainer(uint16_t width, uint16_t height) :
    currentState(NO_ANIMATION),
    numberOfScreens(0),
    animationCounter(0),
    swipeCutoff(80),
    dragX(0),
    currentScreen(0),
    screens(EAST)
{
    setWidth(width);
    setHeight(height);

    touchgfx::Application::getInstance()->registerTimerWidget(this);

    setTouchable(true);

    backgroundSwipeAreaLeft.setWidth(60);
    backgroundSwipeAreaRight.setWidth(60);

    dotIndicator.setBitmaps(Bitmap(BITMAP_DOT_INACTIVE_ID), Bitmap(BITMAP_DOT_ACTIVE_ID));

    Container::add(screens);
    Container::add(dotIndicator);
}

SwipeContainer::~SwipeContainer()
{
    touchgfx::Application::getInstance()->unregisterTimerWidget(this);
}


void SwipeContainer::add(Drawable& screen)
{
    screens.add(screen);
    numberOfScreens++;

    dotIndicator.setNumberOfDots(numberOfScreens);
    dotIndicator.setXY((getWidth() - dotIndicator.getWidth()) / 2, 130);
}

void SwipeContainer::handleTickEvent()
{
    if (currentState == ANIMATE_SWIPE_CANCELLED_LEFT)
    {
        animateSwipeCancelledLeft();
    }
    else if (currentState == ANIMATE_SWIPE_CANCELLED_RIGHT)
    {
        animateSwipeCancelledRight();
    }
    else if (currentState == ANIMATE_LEFT)
    {
        animateLeft();
    }
    else if (currentState == ANIMATE_RIGHT)
    {
        animateRight();
    }
}

void SwipeContainer::handleClickEvent(const ClickEvent& evt)
{
    // If an animation is already in progress do not
    // react to clicks
    if (currentState != NO_ANIMATION)
    {
        return;
    }

    if (evt.getType() == ClickEvent::RELEASED)
    {
        // Save current position for use during animation
        animateDistance = dragX;
        startX = screens.getX();

        if (dragX < 0)
        {
            if (currentScreen == getNumberOfScreens() - 1 || dragX > -swipeCutoff)
            {
                currentState = ANIMATE_SWIPE_CANCELLED_LEFT;
            }
            else
            {
                currentState = ANIMATE_LEFT;
            }
        }
        else if (dragX > 0)
        {
            if (currentScreen == 0 || dragX < swipeCutoff)
            {
                currentState = ANIMATE_SWIPE_CANCELLED_RIGHT;
            }
            else
            {
                currentState = ANIMATE_RIGHT;
            }
        }
    }
}

void SwipeContainer::handleDragEvent(const DragEvent& evt)
{
    // If an animation is already in progress do not
    // react to drags
    if (currentState != NO_ANIMATION)
    {
        return;
    }

    dragX += evt.getDeltaX();

    // Do not show too much background next to end screens
    if (currentScreen == 0 && dragX > backgroundSwipeAreaLeft.getWidth())
    {
        dragX = backgroundSwipeAreaLeft.getWidth();
    }
    else if (currentScreen == getNumberOfScreens() - 1 && dragX < -backgroundSwipeAreaRight.getWidth())
    {
        dragX = -backgroundSwipeAreaRight.getWidth();
    }

    adjustScreens();
}

void SwipeContainer::handleGestureEvent(const GestureEvent& evt)
{
    // Do not accept gestures while animating
    if (currentState != NO_ANIMATION)
    {
        return;
    }

    if (evt.getType() == evt.SWIPE_HORIZONTAL)
    {
        // Save current position for use during animation
        animateDistance = dragX;
        startX = screens.getX();

        if (evt.getVelocity() < 0  && currentScreen < getNumberOfScreens() - 1)
        {
            currentState = ANIMATE_LEFT;
        }
        else if (evt.getVelocity() > 0  && currentScreen > 0)
        {
            currentState = ANIMATE_RIGHT;
        }
    }
}

void SwipeContainer::adjustScreens()
{
    screens.moveTo(-(currentScreen * getWidth()) + dragX, 0);
}

void SwipeContainer::animateSwipeCancelledLeft()
{
    uint8_t duration = 14;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::backEaseOut(animationCounter, 0, -animateDistance, duration);
        dragX = animateDistance + delta;

        adjustScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        dragX = 0;
        adjustScreens();
    }
    animationCounter++;
}

void SwipeContainer::animateSwipeCancelledRight()
{
    uint8_t duration = 14;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::backEaseOut(animationCounter, 0, animateDistance, duration);
        dragX = animateDistance - delta;

        adjustScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        dragX = 0;
        adjustScreens();
    }
    animationCounter++;
}

void SwipeContainer::animateLeft()
{
    uint8_t duration = 10;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::cubicEaseOut(animationCounter, 0, getWidth() + animateDistance, duration);
        dragX = animateDistance - delta;

        adjustScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        currentScreen++;
        dragX = 0;
        adjustScreens();
        dotIndicator.goRight();
    }
    animationCounter++;
}

void SwipeContainer::animateRight()
{
    uint8_t duration = 10;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::cubicEaseOut(animationCounter, 0, getWidth() - animateDistance, duration);
        dragX = animateDistance + delta;

        adjustScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        currentScreen--;
        dragX = 0;
        adjustScreens();
        dotIndicator.goLeft();
    }
    animationCounter++;
}

