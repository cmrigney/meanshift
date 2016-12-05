#include <node.h>
#include "meanshift.h"

namespace meanshift {

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

void Init(Local<Object> exports) {
  Meanshift::Init(exports);
}

NODE_MODULE(meanshift, Init)

}
