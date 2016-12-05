// Meanshift.cc
#include "meanshift.h"

namespace meanshift {

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;
using v8::Exception;

Persistent<Function> Meanshift::constructor;

Meanshift::Meanshift(int imageWidth, int imageHeight, IMAGE_TYPE imageType)
{
  m_pObjectTracker = new CObjectTracker(imageWidth, imageHeight, imageType);
}

Meanshift::~Meanshift() {
  delete m_pObjectTracker;
}

CObjectTracker *Meanshift::GetObjectTracker()
{
  return m_pObjectTracker;
}

void Meanshift::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Meanshift"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "initObject", InitObject);
  NODE_SET_PROTOTYPE_METHOD(tpl, "track", Track);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Meanshift"),
               tpl->GetFunction());
}

void Meanshift::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new Meanshift(...)`
    int imageWidth = args[0]->IsUndefined() ? 0 : args[0]->Int32Value();
    int imageHeight = args[1]->IsUndefined() ? 0 : args[1]->Int32Value();
    IMAGE_TYPE imageType = IMAGE_TYPE(args[2]->IsUndefined() ? 0 : args[2]->Int32Value());
    Meanshift* obj = new Meanshift(imageWidth, imageHeight, imageType);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `Meanshift(...)`, turn into construct call.
    const int argc = 3;
    Local<Value> argv[argc] = { args[0], args[1], args[2] };
    Local<Context> context = isolate->GetCurrentContext();
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Object> result =
        cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void Meanshift::InitObject(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Meanshift* obj = ObjectWrap::Unwrap<Meanshift>(args.Holder());
  
  if(args.Length() != 4) 
  {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if(!args[0]->IsInt32() || !args[1]->IsInt32() || !args[2]->IsInt32() || !args[3]->IsInt32())
  {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Invalid arguments")));
    return;
  }

  SINT16 x = (SINT16)args[0]->Int32Value();
  SINT16 y = (SINT16)args[1]->Int32Value();
  SINT16 width = (SINT16)args[2]->Int32Value();
  SINT16 height = (SINT16)args[3]->Int32Value();

  CObjectTracker *tracker = obj->GetObjectTracker();
  tracker->ObjectTrackerInitObjectParameters(x, y, width, height);
  //args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}

void Meanshift::Track(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Meanshift* obj = ObjectWrap::Unwrap<Meanshift>(args.Holder());
  
  if(args.Length() < 1 || args.Length() > 2)
  {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if(!args[0]->IsObject())
  {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Argument must be a Buffer of image data")));
    return;
  }

  int iterations = 5; //default
  
  if(args[1]->IsInt32())
  {
    iterations = args[1]->Int32Value();
    if(iterations <= 0)
    {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Iterations must be a positive number")));
      return;
    }
  }

  char *data = node::Buffer::Data(args[0]);
  CObjectTracker *tracker = obj->GetObjectTracker();

  Rectangle box = tracker->ObjeckTrackerHandlerByUser(reinterpret_cast<UBYTE8*>(data), iterations);

  Local<Object> result = Object::New(isolate);
  result->Set(String::NewFromUtf8(isolate, "x"), Number::New(isolate, box.x_));
  result->Set(String::NewFromUtf8(isolate, "y"), Number::New(isolate, box.y_));

  args.GetReturnValue().Set(result);
}

}
