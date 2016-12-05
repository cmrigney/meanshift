#ifndef MEANSHIFT_H
#define MEANSHIFT_H

#include <node.h>
#include <node_object_wrap.h>
#include "object-tracker/ObjectTracker.h"
#include <node_buffer.h>

namespace meanshift {

class Meanshift : public node::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);


 private:
  explicit Meanshift(int imageWidth, int imageHeight, IMAGE_TYPE imageType);
  ~Meanshift();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void InitObject(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Track(const v8::FunctionCallbackInfo<v8::Value>& args);
  static v8::Persistent<v8::Function> constructor;

  CObjectTracker *GetObjectTracker();
  
  CObjectTracker *m_pObjectTracker;
};

}  // namespace demo

#endif
