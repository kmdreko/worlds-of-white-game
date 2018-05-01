#include "framebuffer.h"

#include "../logging/LoggingManager.h"
namespace { auto logger = wilt::logging.createLogger("graphics-framebuffer"); }

Framebuffer::Framebuffer()
  : _id{ 0 }
{ }

Framebuffer::Framebuffer(GLuint id)
  : _id{ id }
{ }

Framebuffer::Framebuffer(Texture colorTexture)
  : Framebuffer{ std::move(colorTexture), Texture() }
{ }

Framebuffer::Framebuffer(Texture colorTexture, Texture depthTexture)
  : Framebuffer{ std::move(colorTexture), std::move(depthTexture), Texture() }
{ }

Framebuffer::Framebuffer(Texture colorTexture, Texture depthTexture, Texture stencilTexture)
  : _id{ 0 }
  , _colorTexture{ std::move(colorTexture) }
  , _depthTexture{ std::move(depthTexture) }
  , _stencilTexture{ std::move(stencilTexture) }
{
  glCreateFramebuffers(1, &_id);

  glBindFramebuffer(GL_FRAMEBUFFER, _id);

  if (_colorTexture.loaded())
  {
    _colorTexture.setMinFilter(GL_LINEAR);
    _colorTexture.setMagFilter(GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorTexture.id(), 0);
  }

  if (_depthTexture.loaded())
  {
    _depthTexture.setMinFilter(GL_NEAREST);
    _depthTexture.setMagFilter(GL_NEAREST);
    _depthTexture.setWrapS(GL_CLAMP_TO_EDGE);
    _depthTexture.setWrapT(GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture.id(), 0);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    logger.error("framebuffer is incomplete");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::Framebuffer(Framebuffer&& framebuffer)
  : _id{ framebuffer._id }
  , _colorTexture{ std::move(framebuffer._colorTexture) }
  , _depthTexture{ std::move(framebuffer._depthTexture) }
  , _stencilTexture{ std::move(framebuffer._stencilTexture) }
{
  framebuffer._id = 0;
}

Framebuffer& Framebuffer::operator= (Framebuffer&& framebuffer)
{
  release();

  _id = framebuffer._id;
  _colorTexture = std::move(framebuffer._colorTexture);
  _depthTexture = std::move(framebuffer._depthTexture);
  _stencilTexture = std::move(framebuffer._stencilTexture);

  framebuffer._id = 0;

  return *this;
}

Framebuffer::~Framebuffer()
{
  release();
}

GLuint Framebuffer::id() const
{
  return _id;
}

void Framebuffer::release()
{
  if (_id != 0)
  {
    glDeleteFramebuffers(1, &_id);
    _id = 0;
  }
}

bool Framebuffer::loaded() const
{
  return _id != 0;
}

const Texture& Framebuffer::colorTexture() const
{
  return _colorTexture;
}

const Texture& Framebuffer::depthTexture() const
{
  return _depthTexture;
}

const Texture& Framebuffer::stencilTexture() const
{
  return _stencilTexture;
}

void Framebuffer::resize(GLsizei width, GLsizei height)
{
  if (_colorTexture.loaded())
    _colorTexture.resize(NULL, width, height);
  if (_depthTexture.loaded())
    _depthTexture.resize(NULL, width, height);
  if (_stencilTexture.loaded())
    _stencilTexture.resize(NULL, width, height);
}
