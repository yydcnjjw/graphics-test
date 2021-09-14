#pragma once

#include <glad/gl.h>

#define SK_GL
#include <include/core/SkCanvas.h>
#include <include/core/SkData.h>
#include <include/core/SkImage.h>
#include <include/core/SkStream.h>
#include <include/core/SkSurface.h>
#include <include/gpu/GrDirectContext.h>
#include <include/gpu/gl/GrGLInterface.h>
#include <memory>

namespace my {

class Canvas {
public:
  using self_t = Canvas;

  Canvas(int w, int h) {
    _gr_ctx = GrDirectContext::MakeGL(GrGLMakeNativeInterface());

    GrGLFramebufferInfo fb_info{};
    {
      GLint fbo{};
      ::glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
      if (::glGetError() != GL_NO_ERROR) {
        throw std::runtime_error("framebuffer binding error");
      }
      fb_info.fFBOID = fbo;
      fb_info.fFormat = GL_RGBA8;
    }

    SkColorType color_type{kRGBA_8888_SkColorType};
    
    GrBackendRenderTarget backendRenderTarget(w, h,
                                              0,
                                              0,
                                              fb_info);

    _sk_surface = SkSurface::MakeFromBackendRenderTarget(
        _gr_ctx.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin,
        color_type, nullptr, nullptr);
  }

  void draw() {
    auto canvas = _sk_surface->getCanvas();
    canvas->save();
    canvas->clear(SkColors::kWhite);
    SkPaint paint;
    paint.setColor(SkColors::kBlack);
    canvas->drawRect(SkRect::MakeIWH(100, 100), paint);
    canvas->restore();
    _gr_ctx->flush();
  }

private:
  sk_sp<GrDirectContext> _gr_ctx;
  sk_sp<SkSurface> _sk_surface;
};

} // namespace my
