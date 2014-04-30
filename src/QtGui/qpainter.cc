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
#include "qpainter.h"
#include "qpixmap.h"
#include "qcolor.h"
#include "qpen.h"
#include "qwidget.h"
#include "qbrush.h"
#include "qimage.h"
#include "qpainterpath.h"
#include "qfont.h"
#include "qmatrix.h"

using namespace v8;

Persistent<Function> QPainterWrap::constructor;

QPainterWrap::QPainterWrap() {
  q_ = new QPainter();
}
QPainterWrap::~QPainterWrap() {
  delete q_;
}

void QPainterWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("QPainter"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("begin"),
      FunctionTemplate::New(Begin)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("end"),
      FunctionTemplate::New(End)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("isActive"),
      FunctionTemplate::New(IsActive)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("save"),
      FunctionTemplate::New(Save)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("restore"),
      FunctionTemplate::New(Restore)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setPen"),
      FunctionTemplate::New(SetPen)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setFont"),
      FunctionTemplate::New(SetFont)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setMatrix"),
      FunctionTemplate::New(SetMatrix)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("fillRect"),
      FunctionTemplate::New(FillRect)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("drawText"),
      FunctionTemplate::New(DrawText)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("drawPixmap"),
      FunctionTemplate::New(DrawPixmap)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("drawImage"),
      FunctionTemplate::New(DrawImage)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("strokePath"),
      FunctionTemplate::New(StrokePath)->GetFunction());

  NanAssignPersistent(Function, constructor, tpl->GetFunction());
  target->Set(String::NewSymbol("QPainter"), tpl->GetFunction());
}

NAN_METHOD(QPainterWrap::New) {
  NanScope();

  if (args.Length()>0) {
    return NanThrowTypeError("QPainterWrap: use begin() for initialization");
  }

  QPainterWrap* w = new QPainterWrap();
  w->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_METHOD(QPainterWrap::Begin) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  if (!args[0]->IsObject())
    return NanThrowTypeError("QPainterWrap:Begin: bad arguments");

  QString constructor_name =
    qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());

  // Determine argument type (from its constructor) so we can unwrap it
  if (constructor_name == "QPixmap") {
    // QPixmap
    QPixmapWrap* pixmap_wrap = ObjectWrap::Unwrap<QPixmapWrap>(
        args[0]->ToObject());
    QPixmap* pixmap = pixmap_wrap->GetWrapped();

    NanReturnValue(Boolean::New( q->begin(pixmap) ));
  } else if (constructor_name == "QWidget") {
    // QWidget
    QWidgetWrap* widget_wrap = ObjectWrap::Unwrap<QWidgetWrap>(
        args[0]->ToObject());
    QWidget* widget = widget_wrap->GetWrapped();

    NanReturnValue(Boolean::New( q->begin(widget) ));
  }

  // Unknown argument type
  NanReturnValue(Boolean::New( false ));
}

NAN_METHOD(QPainterWrap::End) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  NanReturnValue(Boolean::New( q->end() ));
}

NAN_METHOD(QPainterWrap::IsActive) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  NanReturnValue(Boolean::New( q->isActive() ));
}

NAN_METHOD(QPainterWrap::Save) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  q->save();

  NanReturnValue(Undefined());
}

NAN_METHOD(QPainterWrap::Restore) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  q->restore();

  NanReturnValue(Undefined());
}

// Supported implementations:
//   setPen( QPen pen )
NAN_METHOD(QPainterWrap::SetPen) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor =
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QPen")
    return NanThrowTypeError("QPainterWrap::SetPen: bad argument");

  // Unwrap obj
  QPenWrap* pen_wrap = ObjectWrap::Unwrap<QPenWrap>(
      args[0]->ToObject());
  QPen* pen = pen_wrap->GetWrapped();

  q->setPen(*pen);

  NanReturnValue(Undefined());
}

NAN_METHOD(QPainterWrap::SetFont) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor =
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QFont")
    return NanThrowTypeError("QPainterWrap::SetFont: bad argument");

  // Unwrap obj
  QFontWrap* font_wrap = ObjectWrap::Unwrap<QFontWrap>(
      args[0]->ToObject());
  QFont* font = font_wrap->GetWrapped();

  q->setFont(*font);

  NanReturnValue(Undefined());
}

// This seems to be undocumented in Qt, but it exists!
NAN_METHOD(QPainterWrap::SetMatrix) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor =
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QMatrix")
    return NanThrowTypeError("QPainterWrap::SetMatrix: bad argument");

  // Unwrap obj
  QMatrixWrap* matrix_wrap = ObjectWrap::Unwrap<QMatrixWrap>(
      args[0]->ToObject());
  QMatrix* matrix = matrix_wrap->GetWrapped();

  q->setMatrix(*matrix, args[1]->BooleanValue());

  NanReturnUndefined();
}

// Supported versions:
//   fillRect(int x, int y, int w, int h, QBrush brush)
//   fillRect(int x, int y, int w, int h, QColor color)
//   fillRect(int x, int y, int w, int h, Qt::GlobalColor color)
NAN_METHOD(QPainterWrap::FillRect) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() ||
      !args[3]->IsNumber())
    NanReturnUndefined();

  QString arg4_constructor;
  if (args[4]->IsObject()) {
    arg4_constructor =
        qt_v8::ToQString(args[4]->ToObject()->GetConstructorName());
  }

  if (arg4_constructor == "QBrush") {
    // fillRect(int x, int y, int w, int h, QBrush brush)

    // Unwrap QBrush
    QBrushWrap* brush_wrap = ObjectWrap::Unwrap<QBrushWrap>(
        args[4]->ToObject());
    QBrush* brush = brush_wrap->GetWrapped();

    q->fillRect(args[0]->IntegerValue(), args[1]->IntegerValue(),
                args[2]->IntegerValue(), args[3]->IntegerValue(),
                *brush);
  } else if (arg4_constructor == "QColor") {
    // fillRect(int x, int y, int w, int h, QColor color)

    // Unwrap QColor
    QColorWrap* color_wrap = ObjectWrap::Unwrap<QColorWrap>(
        args[4]->ToObject());
    QColor* color = color_wrap->GetWrapped();

    q->fillRect(args[0]->IntegerValue(), args[1]->IntegerValue(),
                args[2]->IntegerValue(), args[3]->IntegerValue(),
                *color);
  } else if (args[4]->IsNumber()) {
    // fillRect(int x, int y, int w, int h, Qt::GlobalColor color)

    q->fillRect(args[0]->IntegerValue(), args[1]->IntegerValue(),
                args[2]->IntegerValue(), args[3]->IntegerValue(),
                (Qt::GlobalColor)args[4]->IntegerValue());
  } else {
    return NanThrowTypeError("QPainterWrap:fillRect: bad arguments");
  }

  NanReturnUndefined();
}

// Supported versions:
//   drawText(int x, int y, "text")
NAN_METHOD(QPainterWrap::DrawText) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsString())
    return NanThrowTypeError("QPainterWrap:DrawText: bad arguments");

  q->drawText(args[0]->IntegerValue(), args[1]->IntegerValue(),
      qt_v8::ToQString(args[2]->ToString()));

  NanReturnUndefined();
}

// Supported versions:
//   drawPixmap(int x, int y, QPixmap pixmap)
NAN_METHOD(QPainterWrap::DrawPixmap) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg2_constructor;
  if (args[2]->IsObject()) {
    arg2_constructor =
        qt_v8::ToQString(args[2]->ToObject()->GetConstructorName());
  }

  if (arg2_constructor != "QPixmap" ) {
    return NanThrowTypeError("QPainterWrap::DrawPixmap: pixmap argument not recognized");
  }

  // Unwrap QPixmap
  QPixmapWrap* pixmap_wrap = ObjectWrap::Unwrap<QPixmapWrap>(
      args[2]->ToObject());
  QPixmap* pixmap = pixmap_wrap->GetWrapped();

  if (pixmap->isNull()) {
    return NanThrowTypeError("QPainterWrap::DrawPixmap: pixmap is null, no size set?");
  }

  q->drawPixmap(args[0]->IntegerValue(), args[1]->IntegerValue(), *pixmap);

  NanReturnUndefined();
}

// Supported versions:
//   drawImage( int x, int y, QImage image )
NAN_METHOD(QPainterWrap::DrawImage) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg2_constructor;
  if (args[2]->IsObject()) {
    arg2_constructor =
        qt_v8::ToQString(args[2]->ToObject()->GetConstructorName());
  }

  if (arg2_constructor != "QImage" ) {
    return NanThrowTypeError("QPainterWrap::DrawImage: image argument not recognized");
  }

  // Unwrap QImage
  QImageWrap* image_wrap = ObjectWrap::Unwrap<QImageWrap>(
      args[2]->ToObject());
  QImage* image = image_wrap->GetWrapped();

  if (image->isNull()) {
    return NanThrowTypeError("QPainterWrap::DrawImage: image is null, no size set?");
  }

  q->drawImage(args[0]->IntegerValue(), args[1]->IntegerValue(), *image);

  NanReturnUndefined();
}

// Supported versions:
//   strokePath( QPainterPath path, QPen pen )
NAN_METHOD(QPainterWrap::StrokePath) {
  NanScope();

  QPainterWrap* w = ObjectWrap::Unwrap<QPainterWrap>(args.This());
  QPainter* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor =
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }

  if (arg0_constructor != "QPainterPath" ) {
    return NanThrowTypeError("QPainterWrap::StrokePath: bad arguments");
  }

  QString arg1_constructor;
  if (args[1]->IsObject()) {
    arg1_constructor =
        qt_v8::ToQString(args[1]->ToObject()->GetConstructorName());
  }

  if (arg1_constructor != "QPen" ) {
    return NanThrowTypeError("QPainterWrap::StrokePath: bad arguments");
  }

  // Unwrap QPainterPath
  QPainterPathWrap* path_wrap = ObjectWrap::Unwrap<QPainterPathWrap>(
      args[0]->ToObject());
  QPainterPath* path = path_wrap->GetWrapped();

  // Unwrap QPen
  QPenWrap* pen_wrap = ObjectWrap::Unwrap<QPenWrap>(
      args[1]->ToObject());
  QPen* pen = pen_wrap->GetWrapped();

  q->strokePath(*path, *pen);

  NanReturnUndefined();
}
