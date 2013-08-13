// Copyright (c) 2012, Artur Adib
// All rights reserved.
//
// Author(s): Artur Adib <aadib@mozilla.com>
//
// You may use this file under the terms of the New BSD license as follows:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Artur Adib nor the
//       names of contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ARTUR ADIB BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <node.h>
#include "../QtCore/qpointf.h"
#include "qpainterpath.h"
#include "../qt_v8.h"

using namespace v8;

Persistent<Function> QPainterPathWrap::constructor;

// Supported implementations:
//   QPainterPath ( ??? )
QPainterPathWrap::QPainterPathWrap(_NAN_METHOD_ARGS) {
  q_ = new QPainterPath();
}

QPainterPathWrap::~QPainterPathWrap() {
  delete q_;
}

void QPainterPathWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("QPainterPath"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("moveTo"),
      FunctionTemplate::New(MoveTo)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("lineTo"),
      FunctionTemplate::New(LineTo)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("currentPosition"),
      FunctionTemplate::New(CurrentPosition)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("closeSubpath"),
      FunctionTemplate::New(CloseSubpath)->GetFunction());

  NanAssignPersistent(Function, constructor, tpl->GetFunction());
  target->Set(String::NewSymbol("QPainterPath"), tpl->GetFunction());
}

NAN_METHOD(QPainterPathWrap::New) {
  NanScope();

  QPainterPathWrap* w = new QPainterPathWrap(args);
  w->Wrap(args.This());

  NanReturnValue(args.This());
}

// Supported versions:
//   moveTo( QPointF() )
NAN_METHOD(QPainterPathWrap::MoveTo) {
  NanScope();

  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(args.This());
  QPainterPath* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor =
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QPointF")
    return NanThrowTypeError("QPainterPathWrap::MoveTo: argument not recognized");

  // moveTo( QPointF point )
  QPointFWrap* pointf_wrap = ObjectWrap::Unwrap<QPointFWrap>(
      args[0]->ToObject());
  QPointF* pointf = pointf_wrap->GetWrapped();

  q->moveTo(*pointf);

  NanReturnUndefined();
}

NAN_METHOD(QPainterPathWrap::CurrentPosition) {
  NanScope();

  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(args.This());
  QPainterPath* q = w->GetWrapped();

  NanReturnValue(QPointFWrap::NewInstance(q->currentPosition()));
}

// Supported versions:
//   lineTo( QPointF() )
NAN_METHOD(QPainterPathWrap::LineTo) {
  NanScope();

  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(args.This());
  QPainterPath* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor =
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QPointF")
    return NanThrowTypeError("QPainterPathWrap::MoveTo: argument not recognized");

  // lineTo( QPointF point )
  QPointFWrap* pointf_wrap = ObjectWrap::Unwrap<QPointFWrap>(
      args[0]->ToObject());
  QPointF* pointf = pointf_wrap->GetWrapped();

  q->lineTo(*pointf);

  NanReturnUndefined();
}

NAN_METHOD(QPainterPathWrap::CloseSubpath) {
  NanScope();

  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(args.This());
  QPainterPath* q = w->GetWrapped();

  q->closeSubpath();

  NanReturnUndefined();
}
