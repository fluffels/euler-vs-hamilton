#ifndef CAMERA_H
#define CAMERA_H

#include "include.h"

const float ROT_SCALE = 0.2f;
const float ROLL_AMOUNT = 2.f;

template <typename T, precision P = highp>
class Camera
{
public:
  typedef tvec3<T,P> vec3_type;
  typedef tvec4<T,P> vec4_type;
  typedef tmat4x4<T,P> mat4_type;

  vec3_type eye;
  Camera()
    : eye(vec3(2,2,2))
  {}
  virtual mat4 getView() const
  {
    vec3_type up, fwd, r;
    cameraAxes(up, fwd, r);
    return lookAt((vec3)eye, (vec3)(eye+fwd), (vec3)up);
  }
  virtual void mouseLook(T,T) = 0;
  virtual void doRoll(T) = 0;
  void move(vec3_type t)
  {
    vec3_type up, fwd, r;
    cameraAxes(up, fwd, r);
    eye += r * t.x + up * t.y + fwd * t.z;
  }
  void cameraAxes(vec3_type& up_out, vec3_type& fwd_out,
    vec3_type& r_out) const
  {
    mat4_type rot = getMat();
    up_out = normalize((vec3_type)(rot * vec4_type(0,1,0,0)).xyz());
    fwd_out = normalize((vec3_type)(rot * vec4_type(0,0,1,0)).xyz());
    r_out = normalize((vec3_type)(rot * vec4_type(1,0,0,0)).xyz());
  }
  virtual mat4_type getMat() const = 0;
  void doKeys(GLFWwindow* window)
  {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
      move(vec3_type(0,0,1));
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
      move(vec3_type(0,0,-1));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
      move(vec3_type(1,0,0));
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
      move(vec3_type(-1,0,0));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
      move(vec3_type(0,1,0));
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
      move(vec3_type(0,-1,0));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
      doRoll(-1);
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
      doRoll(1);
    }
  }
};

template <typename T, precision P = highp>
class EulerCamera : public Camera<T,P>
{
public:
  typedef tvec3<T,P> vec3_type;
  typedef tvec4<T,P> vec4_type;
  typedef tmat4x4<T,P> mat4_type;

  mat4_type rot;
  EulerCamera() : rot(mat4_type(1)) {}
  virtual void mouseLook(T dx, T dy)
  {
    rot = rotate(rotate(rot, dy * ROT_SCALE, vec3_type(1,0,0)),
      -dx * ROT_SCALE, vec3_type(0,1,0));
  }
  virtual void doRoll(T dz)
  {
    rot = rotate(rot, dz * ROLL_AMOUNT, vec3_type(0,0,1));
  }
  virtual mat4_type getMat() const
  {
    return rot;
  }
};

template <typename T, precision P = highp>
class QuatCamera : public Camera<T,P>
{
public:
  typedef tvec3<T,P> vec3_type;
  typedef tvec4<T,P> vec4_type;
  typedef tmat4x4<T,P> mat4_type;

  tquat<T,P> quat;
  virtual void mouseLook(T dx,T dy)
  {
    quat = quat * (angleAxis(-dx * ROT_SCALE, vec3_type(0,1,0)) *
      angleAxis(dy * ROT_SCALE, vec3_type(1,0,0)));
    quat = normalize(quat);
  }
  virtual void doRoll(T dz)
  {
    quat = quat * angleAxis(dz * ROLL_AMOUNT, vec3_type(0,0,1));
    quat = normalize(quat);
  }
  virtual mat4_type getMat() const
  {
    return mat4_cast(quat);
  }
};

#endif
