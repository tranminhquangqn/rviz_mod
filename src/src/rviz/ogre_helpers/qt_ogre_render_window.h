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
#ifndef QT_OGRE_RENDER_WINDOW_OGRE_RENDER_WINDOW_H_
#define QT_OGRE_RENDER_WINDOW_OGRE_RENDER_WINDOW_H_

#include <QCursor>
#include <QEvent>
#include <QKeyEvent>
#include <QWheelEvent>

#include <boost/function.hpp>

//#include "render_widget.h"

#include <OgreColourValue.h>
#include <OgreRenderTargetListener.h>

namespace Ogre
{
class Root;
class RenderWindow;
class Viewport;
class Camera;
} // namespace Ogre

namespace rviz
{
/** Qt Ogre render window widget.  Similar in API to
 *  wxOgreRenderWindow from ogre_tools release 1.6, but with much of
 *  the guts replaced by new RenderSystem and RenderWidget classes
 *  inspired by the initialization sequence of Gazebo's renderer.
 */
class QtOgreRenderWindow : public Ogre::RenderTargetListener
{
public:
  /** Constructor.
    @param parent The parent wxWindow component.
   */
  QtOgreRenderWindow();

  /** Destructor.  */
  ~QtOgreRenderWindow() override;

  virtual void setFocus(Qt::FocusReason reason) = 0;
  virtual QPoint mapFromGlobal(const QPoint &) const = 0;
  virtual QPoint mapToGlobal(const QPoint &) const = 0;
  virtual void setCursor(const QCursor &) = 0;
  virtual bool containsPoint(const QPoint &) const = 0;
  virtual double getWindowPixelRatio() const = 0;
  virtual bool isVisible() const = 0;

  /* Mouse and keyboard events need to be daisy chained to the render panel */
  void setKeyPressEventCallback(const std::function<void(QKeyEvent *)> &function);
  void setWheelEventCallback(const std::function<void(QWheelEvent *)> &function);
  void setLeaveEventCallack(const std::function<void(QEvent *)> &function);
  void setMouseEventCallback(const std::function<void(QMouseEvent *)> &function);
  void setContextMenuEvent(const std::function<void(QContextMenuEvent *)> &function);

  /** Set the camera associated with this render window's viewport.
   */
  void setCamera(Ogre::Camera* camera);

  Ogre::Camera* getCamera() const
  {
    return camera_;
  }
  Ogre::RenderWindow* getRenderWindow() { return render_window_; }

  /**
   * \brief Set the scale of the orthographic window.  Only valid for an orthographic camera.
   * @param scale The scale
   */
  void setOrthoScale(float scale);

  /** \brief Enable or disable stereo rendering
   * If stereo is not supported this is ignored.
   * @return the old setting (whether stereo was enabled before)
   */
  bool enableStereo(bool enable);

  /** \brief Prepare to render in stereo if enabled and supported. */
  void setupStereo();

  void setAutoRender(bool auto_render)
  {
    auto_render_ = auto_render;
  }

  ////// Functions mimicked from Ogre::Viewport to satisfy timing of
  ////// after-constructor creation of Ogre::RenderWindow.
  void setOverlaysEnabled(bool overlays_enabled);
  void setBackgroundColor(Ogre::ColourValue color);
    /**
   * Set a callback which is called before each render
   * @param func The callback functor
   */
  virtual void setPreRenderCallback( boost::function<void ()> func );
  /**
     * Set a callback which is called after each render
     * @param func The callback functor
     */
  virtual void setPostRenderCallback( boost::function<void ()> func );

  /** Gets the associated Ogre viewport.  If this is called before
   * QWidget::show() on this widget, it will fail an assertion.
   * Several functions of Ogre::Viewport are duplicated in this class
   * which can be called before QWidget::show(), and their effects are
   * propagated to the viewport when it is created.
   */
  Ogre::Viewport* getViewport() const;

  virtual QRect rect() const = 0;

  virtual void updateScene() = 0;

protected:
  void emitKeyPressEvent(QKeyEvent *event);
  void emitWheelEvent(QWheelEvent *event);
  void emitLeaveEvent(QEvent *event);
  void emitMouseEvent(QMouseEvent *event);
  void emitContextMenuEvent(QContextMenuEvent *event);
  // When stereo is enabled, these are called before/after rendering each
  // viewport.
  virtual void preViewportUpdate(const Ogre::RenderTargetViewportEvent &evt);
  virtual void postViewportUpdate(const Ogre::RenderTargetViewportEvent &evt);

  void initializeRenderSystem();
  void initialize();

  /**
   * Sets the aspect ratio on the camera
   */
  void setCameraAspectRatio();

  /**
   * prepare a viewport's camera for stereo rendering.
   * This should only be called from StereoRenderTargetListener
   */
  void prepareStereoViewport(Ogre::Viewport*);


  Ogre::RenderWindow *render_window_;
  Ogre::Viewport *viewport_;

  boost::function<void()>
      pre_render_callback_; ///< Functor which is called before each render
  boost::function<void()>
      post_render_callback_; ///< Functor which is called after each render
  float ortho_scale_;
  bool auto_render_;

  Ogre::Camera *camera_;
  bool overlays_enabled_;
  Ogre::ColourValue background_color_;

  // stereo rendering
  bool stereo_enabled_;   // true if we were asked to render stereo
  bool rendering_stereo_; // true if we are actually rendering stereo
  Ogre::Camera *left_camera_;
  Ogre::Camera *right_camera_;
  Ogre::Viewport *right_viewport_;

private:
  std::function<void(QKeyEvent *)> key_press_event_callback_;
  std::function<void(QWheelEvent *)> wheel_event_callback_;
  std::function<void(QEvent *)> leave_event_callback_;
  std::function<void(QMouseEvent *)> mouse_event_callback_;
  std::function<void(QContextMenuEvent *)> context_menu_event_;
};

} // namespace rviz

#endif // QT_OGRE_RENDER_WINDOW_OGRE_RENDER_WINDOW_H_
