#include <napi.h>
#include <thread>
#include "napi-thread-safe-callback.hpp"

void request(const Napi::CallbackInfo &info) {}

void streamThreadSafe(const Napi::CallbackInfo &info) {
  int amount_of_invocations = info[0].As<Napi::Number>().Int32Value();
  auto listener =
      std::make_shared<ThreadSafeCallback>(info[1].As<Napi::Function>());
  std::thread t([amount_of_invocations, listener]() {
    for (int i = 0; i < amount_of_invocations; i++) {
      listener->call(
          [](Napi::Env env, std::vector<napi_value> &args) { args = {}; });
    }
  });
  t.detach();
}

void stream(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  int amount_of_invocations = info[0].As<Napi::Number>().Int32Value();
  auto listener = info[1].As<Napi::Function>();
  for (int i = 0; i < amount_of_invocations; i++) {
       listener.Call(env.Global(), {});
  }
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "request"),
              Napi::Function::New(env, request));
  exports.Set(Napi::String::New(env, "streamThreadSafe"),
              Napi::Function::New(env, streamThreadSafe));
              exports.Set(Napi::String::New(env, "stream"),
              Napi::Function::New(env, stream));
  return exports;
}

NODE_API_MODULE(node_addon_benchmark, init)