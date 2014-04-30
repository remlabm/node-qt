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
#include <QFrame>
#include "../qt_v8.h"
#include "../QtCore/qsize.h"
#include "qscrollarea.h"
#include "qwidget.h"
#include "qscrollbar.h"

using namespace v8;

Persistent<Function> QScrollAreaWrap::constructor;

// Supported implementations:
//   QScrollArea ( )
//   QScrollArea ( QWidget widget )
QScrollAreaWrap::QScrollAreaWrap(_NAN_METHOD_ARGS) {
  if (args.Length() == 0) {
    // QScrollArea ( )

    q_ = new QScrollArea;
    return;
  }

  // QScrollArea ( QWidget widget )

  QString arg0_constructor = 
      qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());

  if (arg0_constructor != "QWidget")
    ThrowException(Exception::TypeError(
      String::New("QScrollArea::constructor: bad argument")));

  // Unwrap obj
  QWidgetWrap* q_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
      args[0]->ToObject());
  QWidget* q = q_wrap->GetWrapped();

  q_ = new QScrollArea(q);
}

QScrollAreaWrap::~QScrollAreaWrap() {
  delete q_;
}

void QScrollAreaWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("QScrollArea"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Wrapped methods
  tpl->PrototypeTemplate()->Set(String::NewSymbol("resize"),
      FunctionTemplate::New(Resize)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("show"),
      FunctionTemplate::New(Show)->GetFunction());
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
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setFocusPolicy"),
      FunctionTemplate::New(SetFocusPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("move"),
      FunctionTemplate::New(Move)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("x"),
      FunctionTemplate::New(X)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("y"),
      FunctionTemplate::New(Y)->GetFunction());

  // QScrollArea-specific
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setWidget"),
      FunctionTemplate::New(SetWidget)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("widget"),
      FunctionTemplate::New(Widget)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setFrameShape"),
      FunctionTemplate::New(SetFrameShape)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setVerticalScrollBarPolicy"),
      FunctionTemplate::New(SetVerticalScrollBarPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setHorizontalScrollBarPolicy"),
      FunctionTemplate::New(SetHorizontalScrollBarPolicy)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("verticalScrollBar"),
      FunctionTemplate::New(VerticalScrollBar)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("horizontalScrollBar"),
      FunctionTemplate::New(HorizontalScrollBar)->GetFunction());

  NanAssignPersistent(Function, constructor, tpl->GetFunction());
  target->Set(String::NewSymbol("QScrollArea"), tpl->GetFunction());
}

NAN_METHOD(QScrollAreaWrap::New) {
  NanScope();

  QScrollAreaWrap* w = new QScrollAreaWrap(args);
  w->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_METHOD(QScrollAreaWrap::Resize) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->resize(args[0]->NumberValue(), args[1]->NumberValue());

  NanReturnUndefined();
}

NAN_METHOD(QScrollAreaWrap::Show) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->show();

  NanReturnUndefined();
}

NAN_METHOD(QScrollAreaWrap::Close) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->close();

  NanReturnUndefined();
}

NAN_METHOD(QScrollAreaWrap::Size) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  NanReturnValue( QSizeWrap::NewInstance(q->size()) );
}

NAN_METHOD(QScrollAreaWrap::Width) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  NanReturnValue( Integer::New(q->width()) );
}

NAN_METHOD(QScrollAreaWrap::Height) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  NanReturnValue( Integer::New(q->height()) );
}

NAN_METHOD(QScrollAreaWrap::ObjectName) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  NanReturnValue(qt_v8::FromQString(q->objectName()));
}

NAN_METHOD(QScrollAreaWrap::SetObjectName) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setObjectName(qt_v8::ToQString(args[0]->ToString()));

  NanReturnUndefined();
}

//
// QUIRK:
// Here: Parent() returns the parent's name
// Qt: Parent() returns QObject
// Intended mostly for sanity checks
//
NAN_METHOD(QScrollAreaWrap::Parent) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  NanReturnValue(qt_v8::FromQString(q->parent()->objectName()));
}

NAN_METHOD(QScrollAreaWrap::Update) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->update();

  NanReturnUndefined();
}

NAN_METHOD(QScrollAreaWrap::SetWidget) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QWidget")
    return NanThrowTypeError("QScrollArea::SetWidget: bad argument");

  // Unwrap obj
  QWidgetWrap* widget_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
      args[0]->ToObject());
  QWidget* widget = widget_wrap->GetWrapped();

  q->setWidget(widget);

  NanReturnUndefined();
}

// QUIRK:
// Does not return QWidget. Returns 1 if child widget exists, 0 otherwise
NAN_METHOD(QScrollAreaWrap::Widget) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  int retvalue = q->widget() ? 1 : 0;

  NanReturnValue(Integer::New(retvalue));
}

NAN_METHOD(QScrollAreaWrap::SetFrameShape) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setFrameShape((QFrame::Shape)(args[0]->IntegerValue()));

  NanReturnUndefined();
}

NAN_METHOD(QScrollAreaWrap::SetFocusPolicy) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setFocusPolicy((Qt::FocusPolicy)(args[0]->IntegerValue()));

  NanReturnUndefined();
}

// Supported implementations:
//    move (int x, int y)
NAN_METHOD(QScrollAreaWrap::Move) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->move(args[0]->IntegerValue(), args[1]->IntegerValue());

  NanReturnUndefined();
}

NAN_METHOD(QScrollAreaWrap::X) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  NanReturnValue(Integer::New(q->x()));
}

NAN_METHOD(QScrollAreaWrap::Y) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  NanReturnValue(Integer::New(q->y()));
}

NAN_METHOD(QScrollAreaWrap::SetVerticalScrollBarPolicy) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setVerticalScrollBarPolicy((Qt::ScrollBarPolicy)(args[0]->IntegerValue()));

  NanReturnUndefined();
}

NAN_METHOD(QScrollAreaWrap::SetHorizontalScrollBarPolicy) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  q->setHorizontalScrollBarPolicy((Qt::ScrollBarPolicy)
      (args[0]->IntegerValue()));

  NanReturnUndefined();
}

NAN_METHOD(QScrollAreaWrap::HorizontalScrollBar) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  NanReturnValue(
    QScrollBarWrap::NewInstance(q->horizontalScrollBar()) );
}

NAN_METHOD(QScrollAreaWrap::VerticalScrollBar) {
  NanScope();

  QScrollAreaWrap* w = node::ObjectWrap::Unwrap<QScrollAreaWrap>(args.This());
  QScrollArea* q = w->GetWrapped();

  NanReturnValue(
      QScrollBarWrap::NewInstance(q->verticalScrollBar()) );
}
