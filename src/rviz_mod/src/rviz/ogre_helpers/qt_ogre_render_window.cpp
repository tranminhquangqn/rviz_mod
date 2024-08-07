/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "qt_ogre_render_window.h"
#include "orthographic.h"
#include "render_system.h"

#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreStringConverter.h>
#include <OgreGpuProgramManager.h>
//#include <OgreRenderTargetListener.h>

#include <ros/console.h>
#include <ros/assert.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include <stdlib.h>
#endif

namespace rviz
{
QtOgreRenderWindow::QtOgreRenderWindow()
  : render_window_( nullptr )
  , viewport_(nullptr)
  , ortho_scale_(1.0f)
  , auto_render_(true)
  , camera_(nullptr)
  , overlays_enabled_(true)                     // matches the default of Ogre::Viewport.
  , background_color_(Ogre::ColourValue::Black) // matches the default of Ogre::Viewport.
  , stereo_enabled_(false)
  , rendering_stereo_(false)
  , left_camera_(nullptr)
  , right_camera_(nullptr)
  , right_viewport_(nullptr)
{
}
QtOgreRenderWindow::~QtOgreRenderWindow() {
  enableStereo(false);  // free stereo resources

  if( render_window_ != nullptr)
  {
    render_window_->removeViewport( 0 );
    render_window_->destroy();
    render_window_ = nullptr;
  }
}
void QtOgreRenderWindow::initialize()
{
  viewport_ = render_window_->addViewport(camera_);
  viewport_->setOverlaysEnabled(overlays_enabled_);
  viewport_->setBackgroundColour(background_color_);

#if OGRE_STEREO_ENABLE
  viewport_->setDrawBuffer(Ogre::CBT_BACK);
#endif
  enableStereo(true);

  setCameraAspectRatio();
}

void QtOgreRenderWindow::setCamera( Ogre::Camera* camera )
{
  if (camera)
  {
    camera_ = camera;
    viewport_->setCamera( camera );

    setCameraAspectRatio();

    if (camera_ && rendering_stereo_ && !left_camera_)
    {
      left_camera_ = camera_->getSceneManager()->createCamera(camera_->getName() + "-left");
    }
    if (camera_ && rendering_stereo_ && !right_camera_)
    {
      right_camera_ = camera_->getSceneManager()->createCamera(camera_->getName() + "-right");
    }

    updateScene();
  }
}

void QtOgreRenderWindow::setOrthoScale( float scale )
{
  ortho_scale_ = scale;

  setCameraAspectRatio();
}

//------------------------------------------------------------------------------
bool QtOgreRenderWindow::enableStereo(bool enable)
{
  bool was_enabled = stereo_enabled_;
  stereo_enabled_ = enable;
  setupStereo();
  return was_enabled;
}

void QtOgreRenderWindow::setupStereo()
{
  bool render_stereo = stereo_enabled_ && RenderSystem::get()->isStereoSupported();

  if (render_stereo == rendering_stereo_)
    return;

  rendering_stereo_ = render_stereo;

  if (rendering_stereo_)
  {
    right_viewport_ = render_window_->addViewport(nullptr, 1);
#if OGRE_STEREO_ENABLE
    right_viewport_->setDrawBuffer(Ogre::CBT_BACK_RIGHT);
    viewport_->setDrawBuffer(Ogre::CBT_BACK_LEFT);
#endif

    setOverlaysEnabled(overlays_enabled_);
    setBackgroundColor(background_color_);
    if (camera_)
      setCamera(camera_);

    // addListener causes preViewportUpdate() to be called when rendering.
    render_window_->addListener(this);
  }
  else
  {
    render_window_->removeListener(this);
    render_window_->removeViewport(1);
    right_viewport_ = nullptr;

#if OGRE_STEREO_ENABLE
    viewport_->setDrawBuffer(Ogre::CBT_BACK);
#endif

    if (left_camera_)
      left_camera_->getSceneManager()->destroyCamera(left_camera_);
    left_camera_ = nullptr;
    if (right_camera_)
      right_camera_->getSceneManager()->destroyCamera(right_camera_);
    right_camera_ = nullptr;
  }
}
void QtOgreRenderWindow::setOverlaysEnabled( bool overlays_enabled )
{
  overlays_enabled_ = overlays_enabled;
  viewport_->setOverlaysEnabled( overlays_enabled );
  if (right_viewport_)
  {
    right_viewport_->setOverlaysEnabled( overlays_enabled );
  }
}

void QtOgreRenderWindow::setBackgroundColor( Ogre::ColourValue background_color )
{
  background_color_ = background_color;
  viewport_->setBackgroundColour( background_color );
  if (right_viewport_)
  {
    right_viewport_->setBackgroundColour( background_color );
  }
}

void QtOgreRenderWindow::setPreRenderCallback( boost::function<void ()> func )
{
  pre_render_callback_ = func;
}

void QtOgreRenderWindow::setPostRenderCallback( boost::function<void ()> func )
{
  post_render_callback_ = func;
}

// this is called just before rendering either viewport when stereo is enabled.
void QtOgreRenderWindow::preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
{
  Ogre::Viewport* viewport = evt.source;

  const Ogre::Vector2& offset = camera_->getFrustumOffset();
  const Ogre::Vector3 pos = camera_->getPosition();
  const Ogre::Vector3 right = camera_->getRight();
  const Ogre::Vector3 up = camera_->getUp();

  if (viewport == right_viewport_)
  {
    if (camera_->getProjectionType() != Ogre::PT_PERSPECTIVE || !right_camera_)
    {
      viewport->setCamera(camera_);
      return;
    }

    Ogre::Vector3 newpos = pos + right * offset.x + up * offset.y;

    right_camera_->synchroniseBaseSettingsWith(camera_);
    right_camera_->setFrustumOffset(-offset);
    right_camera_->setPosition(newpos);
    viewport->setCamera(right_camera_);
  }
  else if (viewport == viewport_)
  {
    if (camera_->getProjectionType() != Ogre::PT_PERSPECTIVE || !left_camera_)
    {
      viewport->setCamera(camera_);
      return;
    }

    Ogre::Vector3 newpos = pos - right * offset.x - up * offset.y;

    left_camera_->synchroniseBaseSettingsWith(camera_);
    left_camera_->setFrustumOffset(offset);
    left_camera_->setPosition(newpos);
    viewport->setCamera(left_camera_);
  }
  else
  {
    ROS_WARN("Begin rendering to unknown viewport.");
  }
}

void QtOgreRenderWindow::postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
{
  Ogre::Viewport* viewport = evt.source;

  if (viewport == right_viewport_)
  {
    // nothing to do here
  }
  else if (viewport == viewport_)
  {
    viewport->setCamera(camera_);
  }
  else
  {
    ROS_WARN("End rendering to unknown viewport.");
  }
  if (!right_camera_) {
    return;
  }
  if (!right_camera_->isCustomProjectionMatrixEnabled())
  {
    right_camera_->synchroniseBaseSettingsWith(camera_);
    right_camera_->setFrustumOffset(-camera_->getFrustumOffset());
  }
  right_viewport_->setCamera(right_camera_);
}

Ogre::Viewport* QtOgreRenderWindow::getViewport() const
{
  return viewport_;
}


void QtOgreRenderWindow::setCameraAspectRatio()
{
  if (camera_)
  {
    camera_->setAspectRatio(Ogre::Real(rect().width()) / Ogre::Real(rect().height()));
    if (right_camera_)
    {
      right_camera_->setAspectRatio(Ogre::Real(rect().width()) / Ogre::Real(rect().height()));
    }

    if (camera_->getProjectionType() == Ogre::PT_ORTHOGRAPHIC)
    {
      Ogre::Matrix4 proj;
      buildScaledOrthoMatrix(proj, -rect().width() / ortho_scale_ / 2, rect().width() / ortho_scale_ / 2,
                             -rect().height() / ortho_scale_ / 2, rect().height() / ortho_scale_ / 2,
                             camera_->getNearClipDistance(), camera_->getFarClipDistance());
      camera_->setCustomProjectionMatrix(true, proj);
    }
  }
}

void QtOgreRenderWindow::setKeyPressEventCallback(const std::function<void (QKeyEvent *)> &function) {
  key_press_event_callback_ = function;
}

void QtOgreRenderWindow::setWheelEventCallback(const std::function<void (QWheelEvent *)> &function) {
  wheel_event_callback_ = function;
}

void QtOgreRenderWindow::setLeaveEventCallack(const std::function<void (QEvent *)> &function) {
  leave_event_callback_ = function;
}

void QtOgreRenderWindow::setMouseEventCallback(const std::function<void (QMouseEvent *)> &function) {
  mouse_event_callback_ = function;
}

void QtOgreRenderWindow::setContextMenuEvent(const std::function<void (QContextMenuEvent *)> &function)
{
  context_menu_event_ = function;
}

void QtOgreRenderWindow::emitKeyPressEvent(QKeyEvent *event) {
  if (key_press_event_callback_) {
    key_press_event_callback_(event);
  }
}

void QtOgreRenderWindow::emitWheelEvent(QWheelEvent *event) {
  if (wheel_event_callback_) {
    wheel_event_callback_(event);
  }
}

void QtOgreRenderWindow::emitLeaveEvent(QEvent *event) {
  if (leave_event_callback_) {
    leave_event_callback_(event);
  }
}

void QtOgreRenderWindow::emitMouseEvent(QMouseEvent *event) {
  if (mouse_event_callback_) {
    mouse_event_callback_(event);
  }
}

void QtOgreRenderWindow::emitContextMenuEvent(QContextMenuEvent *event)
{
  if (context_menu_event_) {
      context_menu_event_(event);
    }
}
} // namespace rviz
