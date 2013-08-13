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
#include "../qt_v8.h"
#include "../QtCore/qsize.h"
#include "qwidget.h"
#include "qmouseevent.h"
#include "qkeyevent.h"

using namespace v8;

Persistent<Function> QWidgetWrap::constructor;

//
// QWidgetImpl()
//

QWidgetImpl::QWidgetImpl(QWidgetImpl* parent) : QWidget(parent) {
  // Initialize callbacks as boolean values so we can test if the callback
  // has been set via ->IsFunction() below
  NanAssignPersistent(Boolean, paintEventCallback_, Boolean::New(false));
  NanAssignPersistent(Boolean, mousePressCallback_, Boolean::New(false));
  NanAssignPersistent(Boolean, mouseReleaseCallback_, Boolean::New(false));
  NanAssignPersistent(Boolean, mouseMoveCallback_, Boolean::New(false));
  NanAssignPersistent(Boolean, keyPressCallback_, Boolean::New(false));
  NanAssignPersistent(Boolean, keyReleaseCallback_, Boolean::New(false));
}

QWidgetImpl::~QWidgetImpl() {
  NanDispose(paintEventCallback_);
  NanDispose(mousePressCallback_);
  NanDispose(mouseReleaseCallback_);
  NanDispose(mouseMoveCallback_);
  NanDispose(keyPressCallback_);
  NanDispose(keyReleaseCallback_);
}

void QWidgetImpl::paintEvent(QPaintEvent* e) {
  NanScope();

  if (!NanPersistentToLocal(paintEventCallback_)->IsFunction())
    return;

  const unsigned argc = 0;
  Handle<Value> argv[1] = {};
  Handle<Function> cb = NanPersistentToLocal(Persistent<Function>::Cast(paintEventCallback_));

  cb->Call(Context::GetCurrent()->Global(), argc, argv);
}

void QWidgetImpl::mousePressEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  NanScope();

  if (!NanPersistentToLocal(mousePressCallback_)->IsFunction())
    return;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(*e)
  };
  Handle<Function> cb = NanPersistentToLocal(Persistent<Function>::Cast(mousePressCallback_));

  cb->Call(Context::GetCurrent()->Global(), argc, argv);
}

void QWidgetImpl::mouseReleaseEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  NanScope();

  if (!NanPersistentToLocal(mouseReleaseCallback_)->IsFunction())
    return;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(*e)
  };
  Handle<Function> cb = NanPersistentToLocal(Persistent<Function>::Cast(mouseReleaseCallback_));

  cb->Call(Context::GetCurrent()->Global(), argc, argv);
}

void QWidgetImpl::mouseMoveEvent(QMouseEvent* e) {
  e->ignore(); // ensures event bubbles up

  NanScope();

  if (!NanPersistentToLocal(mouseMoveCallback_)->IsFunction())
    return;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QMouseEventWrap::NewInstance(*e)
  };
  Handle<Function> cb = NanPersistentToLocal(Persistent<Function>::Cast(mouseMoveCallback_));

  cb->Call(Context::GetCurrent()->Global(), argc, argv);
}

void QWidgetImpl::keyPressEvent(QKeyEvent* e) {
  e->ignore(); // ensures event bubbles up

  NanScope();

  if (!NanPersistentToLocal(keyPressCallback_)->IsFunction())
    return;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QKeyEventWrap::NewInstance(*e)
  };
  Handle<Function> cb = NanPersistentToLocal(Persistent<Function>::Cast(keyPressCallback_));

  cb->Call(Context::GetCurrent()->Global(), argc, argv);
}

void QWidgetImpl::keyReleaseEvent(QKeyEvent* e) {
  e->ignore(); // ensures event bubbles up

  NanScope();

  if (!NanPersistentToLocal(keyReleaseCallback_)->IsFunction())
    return;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = {
    QKeyEventWrap::NewInstance(*e)
  };
  Handle<Function> cb = NanPersistentToLocal(Persistent<Function>::Cast(keyReleaseCallback_));

  cb->Call(Context::GetCurrent()->Global(), argc, argv);
}

//
// QWidgetWrap()
//

QWidgetWrap::QWidgetWrap(QWidgetImpl* parent) {
  q_ = new QWidgetImpl(parent);
}

QWidgetWrap::~QWidgetWrap() {
  delete q_;
}

void QWidgetWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("QWidget"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Wrapped methods
  tpl->PrototypeTemplate()->Set(String::NewSymbol("resize"),
      FunctionTemplate::New(Resize)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("show"),
      FunctionTemplate::New(Show)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("close"),
      FunctionTemplate::New(Close)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("size"),
      FunctionTemplate::New(Size)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("width"),
      FunctionTemplate::New(Width)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("height"),
      FunctionTemplate::New(Height)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("parent"),
      FunctionTemplate::New(Parent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("objectName"),
      FunctionTemplate::New(ObjectName)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setObjectName"),
      FunctionTemplate::New(SetObjectName)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("update"),
      FunctionTemplate::New(Update)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("hasMouseTracking"),
      FunctionTemplate::New(HasMouseTracking)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setMouseTracking"),
      FunctionTemplate::New(SetMouseTracking)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setFocusPolicy"),
      FunctionTemplate::New(SetFocusPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("move"),
      FunctionTemplate::New(Move)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("x"),
      FunctionTemplate::New(X)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("y"),
      FunctionTemplate::New(Y)->GetFunction());

  // Events
  tpl->PrototypeTemplate()->Set(String::NewSymbol("paintEvent"),
      FunctionTemplate::New(PaintEvent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("mousePressEvent"),
      FunctionTemplate::New(MousePressEvent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("mouseReleaseEvent"),
      FunctionTemplate::New(MouseReleaseEvent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("mouseMoveEvent"),
      FunctionTemplate::New(MouseMoveEvent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("keyPressEvent"),
      FunctionTemplate::New(KeyPressEvent)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("keyReleaseEvent"),
      FunctionTemplate::New(KeyReleaseEvent)->GetFunction());

  NanAssignPersistent(Function, constructor, tpl->GetFunction());
  target->Set(String::NewSymbol("QWidget"), tpl->GetFunction());
}

NAN_METHOD(QWidgetWrap::New) {
  NanScope();
  QWidgetImpl* q_parent = 0;

  if (args.Length() > 0) {
    QWidgetWrap* w_parent = node::ObjectWrap::Unwrap<QWidgetWrap>(args[0]->ToObject());
    q_parent = w_parent->GetWrapped();
  }

  QWidgetWrap* w = new QWidgetWrap(q_parent);
  w->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_METHOD(QWidgetWrap::Resize) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->resize(args[0]->NumberValue(), args[1]->NumberValue());

  NanReturnUndefined();
}

NAN_METHOD(QWidgetWrap::Show) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->show();

  NanReturnUndefined();
}

NAN_METHOD(QWidgetWrap::Close) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->close();

  NanReturnUndefined();
}

NAN_METHOD(QWidgetWrap::Size) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanReturnValue( QSizeWrap::NewInstance(q->size()) );
}

NAN_METHOD(QWidgetWrap::Width) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanReturnValue( Integer::New(q->width()) );
}

NAN_METHOD(QWidgetWrap::Height) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanReturnValue( Integer::New(q->height()) );
}

NAN_METHOD(QWidgetWrap::ObjectName) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanReturnValue(qt_v8::FromQString(q->objectName()));
}

NAN_METHOD(QWidgetWrap::SetObjectName) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->setObjectName(qt_v8::ToQString(args[0]->ToString()));

  NanReturnUndefined();
}

//
// QUIRK:
// Here: Parent() returns the parent's name
// Qt: Parent() returns QObject
// Intended mostly for sanity checks
//
NAN_METHOD(QWidgetWrap::Parent) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanReturnValue(qt_v8::FromQString(q->parent()->objectName()));
}

//
// PaintEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::PaintEvent) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanAssignPersistent(Function, q->paintEventCallback_, Local<Function>::Cast(args[0]));

  NanReturnUndefined();
}

//
// MousePressEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::MousePressEvent) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanAssignPersistent(Function, q->mousePressCallback_, Local<Function>::Cast(args[0]));

  NanReturnUndefined();
}

//
// MouseReleaseEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::MouseReleaseEvent) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanAssignPersistent(Function, q->mouseReleaseCallback_, Local<Function>::Cast(args[0]));

  NanReturnUndefined();
}

//
// MouseMoveEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::MouseMoveEvent) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanAssignPersistent(Function, q->mouseMoveCallback_, Local<Function>::Cast(args[0]));

  NanReturnUndefined();
}

//
// KeyPressEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::KeyPressEvent) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanAssignPersistent(Function, q->keyPressCallback_, Local<Function>::Cast(args[0]));

  NanReturnUndefined();
}

//
// KeyReleaseEvent()
// Binds a callback to Qt's event
//
NAN_METHOD(QWidgetWrap::KeyReleaseEvent) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanAssignPersistent(Function, q->keyReleaseCallback_, Local<Function>::Cast(args[0]));

  NanReturnUndefined();
}

NAN_METHOD(QWidgetWrap::Update) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->update();

  NanReturnUndefined();
}

NAN_METHOD(QWidgetWrap::HasMouseTracking) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanReturnValue(Boolean::New(q->hasMouseTracking()));
}

NAN_METHOD(QWidgetWrap::SetMouseTracking) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->setMouseTracking(args[0]->BooleanValue());

  NanReturnUndefined();
}

NAN_METHOD(QWidgetWrap::SetFocusPolicy) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->setFocusPolicy((Qt::FocusPolicy)(args[0]->IntegerValue()));

  NanReturnUndefined();
}

// Supported implementations:
//    move (int x, int y)
NAN_METHOD(QWidgetWrap::Move) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  q->move(args[0]->IntegerValue(), args[1]->IntegerValue());

  NanReturnUndefined();
}

NAN_METHOD(QWidgetWrap::X) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanReturnValue(Integer::New(q->x()));
}

NAN_METHOD(QWidgetWrap::Y) {
  NanScope();

  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(args.This());
  QWidgetImpl* q = w->GetWrapped();

  NanReturnValue(Integer::New(q->y()));
}
