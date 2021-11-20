/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QRGBA64_H
#define QRGBA64_H

#include <QtGlobal>

#include <QtCore/qprocessordetection.h>

class QRgba64 {
  quint64 rgba;
  enum Shifts {
    RedShift = 0,
    GreenShift = 16,
    BlueShift = 32,
    AlphaShift = 48
  };

  explicit QRgba64(quint64 c) : rgba(c) { }
public:
  QRgba64() = default;

  static QRgba64 fromRgba64(quint64 c)
  {
    return QRgba64(c);
  }
  static QRgba64 fromRgba64(quint16 red, quint16 green, quint16 blue, quint16 alpha)
  {
    return fromRgba64(quint64(red)   << RedShift
                      | quint64(green) << GreenShift
                      | quint64(blue)  << BlueShift
                      | quint64(alpha) << AlphaShift);
  }
  static QRgba64 fromRgba(quint8 red, quint8 green, quint8 blue, quint8 alpha)
  {
    QRgba64 rgb64 = fromRgba64(red, green, blue, alpha);
    // Expand the range so that 0x00 maps to 0x0000 and 0xff maps to 0xffff.
    rgb64.rgba |= rgb64.rgba << 8;
    return rgb64;
  }
  static
  QRgba64 fromArgb32(uint rgb)
  {
    return fromRgba(quint8(rgb >> 16), quint8(rgb >> 8), quint8(rgb), quint8(rgb >> 24));
  }

  bool isOpaque() const
  {
    return (rgba & alphaMask()) == alphaMask();
  }
  bool isTransparent() const
  {
    return (rgba & alphaMask()) == 0;
  }

  quint16 red()   const { return quint16(rgba >> RedShift);   }
  quint16 green() const { return quint16(rgba >> GreenShift); }
  quint16 blue()  const { return quint16(rgba >> BlueShift);  }
  quint16 alpha() const { return quint16(rgba >> AlphaShift); }
  void setRed(quint16 _red)     { rgba = (rgba & ~(Q_UINT64_C(0xffff) << RedShift))   | (quint64(_red) << RedShift); }
  void setGreen(quint16 _green) { rgba = (rgba & ~(Q_UINT64_C(0xffff) << GreenShift)) | (quint64(_green) << GreenShift); }
  void setBlue(quint16 _blue)   { rgba = (rgba & ~(Q_UINT64_C(0xffff) << BlueShift))  | (quint64(_blue) << BlueShift); }
  void setAlpha(quint16 _alpha) { rgba = (rgba & ~(Q_UINT64_C(0xffff) << AlphaShift)) | (quint64(_alpha) << AlphaShift); }

  quint8 red8()   const { return div_257(red()); }
  quint8 green8() const { return div_257(green()); }
  quint8 blue8()  const { return div_257(blue()); }
  quint8 alpha8() const { return div_257(alpha()); }
  uint toArgb32() const
  {
    return uint((alpha8() << 24) | (red8() << 16) | (green8() << 8) | blue8());
  }
  ushort toRgb16() const
  {
    return ushort((red() & 0xf800) | ((green() >> 10) << 5) | (blue() >> 11));
  }

  QRgba64 premultiplied() const
  {
    const quint32 a = alpha();
    const quint16 r = div_65535(red()   * a);
    const quint16 g = div_65535(green() * a);
    const quint16 b = div_65535(blue()  * a);
    return fromRgba64(r, g, b, quint16(a));
  }

  QRgba64 unpremultiplied() const
  {
    return unpremultiplied_64bit();
  }

  operator quint64() const
  {
    return rgba;
  }

  QRgba64 operator=(quint64 _rgba)
  {
    rgba = _rgba;
    return *this;
  }

private:
  static  quint64 alphaMask() { return Q_UINT64_C(0xffff) << AlphaShift; }

  static  quint8 div_257_floor(uint x) { return quint8((x - (x >> 8)) >> 8); }
  static  quint8 div_257(quint16 x) { return div_257_floor(x + 128U); }
  static  quint16 div_65535(uint x) { return quint16((x + (x>>16) + 0x8000U) >> 16); }
  QRgba64 unpremultiplied_32bit() const
  {
    if (isOpaque() || isTransparent())
      return *this;
    const quint32 a = alpha();
    const quint16 r = quint16((red()   * 0xffff + a/2) / a);
    const quint16 g = quint16((green() * 0xffff + a/2) / a);
    const quint16 b = quint16((blue()  * 0xffff + a/2) / a);
    return fromRgba64(r, g, b, quint16(a));
  }
  QRgba64 unpremultiplied_64bit() const
  {
    if (isOpaque() || isTransparent())
      return *this;
    const quint64 a = alpha();
    const quint64 fa = (Q_UINT64_C(0xffff00008000) + a/2) / a;
    const quint16 r = quint16((red()   * fa + 0x80000000) >> 32);
    const quint16 g = quint16((green() * fa + 0x80000000) >> 32);
    const quint16 b = quint16((blue()  * fa + 0x80000000) >> 32);
    return fromRgba64(r, g, b, quint16(a));
  }
};

Q_DECLARE_TYPEINFO(QRgba64, Q_PRIMITIVE_TYPE);

inline QRgba64 qRgba64(quint16 r, quint16 g, quint16 b, quint16 a)
{
  return QRgba64::fromRgba64(r, g, b, a);
}

inline QRgba64 qRgba64(quint64 c)
{
  return QRgba64::fromRgba64(c);
}

inline QRgba64 qPremultiply(QRgba64 c)
{
  return c.premultiplied();
}

inline QRgba64 qUnpremultiply(QRgba64 c)
{
  return c.unpremultiplied();
}

inline  uint qRed(QRgba64 rgb)
{ return rgb.red8(); }

inline  uint qGreen(QRgba64 rgb)
{ return rgb.green8(); }

inline  uint qBlue(QRgba64 rgb)
{ return rgb.blue8(); }

inline  uint qAlpha(QRgba64 rgb)
{ return rgb.alpha8(); }

#endif // QRGBA64_H
