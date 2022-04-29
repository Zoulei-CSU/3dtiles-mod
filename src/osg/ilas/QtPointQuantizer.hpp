#ifndef QTPOINTQUANTIZER_HPP
#define QTPOINTQUANTIZER_HPP

template<typename T>
struct QtPointQuantizerTemplate
{
public:
  double x_scale_factor;
  double y_scale_factor;
  double z_scale_factor;
  double x_offset;
  double y_offset;
  double z_offset;

  inline double get_x(const T X) const { return x_scale_factor*X+x_offset; }
  inline double get_y(const T Y) const { return y_scale_factor*Y+y_offset; }
  inline double get_z(const T Z) const { return z_scale_factor*Z+z_offset; }

  inline T get_X(const double x) const { if (x >= x_offset) return (T)((x-x_offset)/x_scale_factor+0.5); else return (T)((x-x_offset)/x_scale_factor-0.5); }
  inline T get_Y(const double y) const { if (y >= y_offset) return (T)((y-y_offset)/y_scale_factor+0.5); else return (T)((y-y_offset)/y_scale_factor-0.5); }
  inline T get_Z(const double z) const { if (z >= z_offset) return (T)((z-z_offset)/z_scale_factor+0.5); else return (T)((z-z_offset)/z_scale_factor-0.5); }

  QtPointQuantizerTemplate()
  {
    x_scale_factor = 0.01;
    y_scale_factor = 0.01;
    z_scale_factor = 0.01;
    x_offset = 0.0;
    y_offset = 0.0;
    z_offset = 0.0;
  }

  QtPointQuantizerTemplate & operator=(const QtPointQuantizerTemplate & quantizer)
  {
    this->x_scale_factor = quantizer.x_scale_factor;
    this->y_scale_factor = quantizer.y_scale_factor;
    this->z_scale_factor = quantizer.z_scale_factor;
    this->x_offset = quantizer.x_offset;
    this->y_offset = quantizer.y_offset;
    this->z_offset = quantizer.z_offset;
    return *this;
  }
  bool operator ==(const QtPointQuantizerTemplate & quantizer)
  {
    if (x_scale_factor == quantizer.x_scale_factor
        && y_scale_factor == quantizer.y_scale_factor
        && z_scale_factor == quantizer.z_scale_factor
        && x_offset == quantizer.x_offset
        && y_offset == quantizer.y_offset
        && z_offset == quantizer.z_offset)
    {
      return true;
    }
    return false;
  }

  void AutoScaleAndOffset(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z)
  {
    // auto scale
    if (-360 < min_x  && -360 < min_y && max_x < 360 && max_y < 360)
    {
      x_scale_factor = 0.0000001;
      y_scale_factor = 0.0000001;
    }
    else
    {
      x_scale_factor = 0.01;
      y_scale_factor = 0.01;
    }
    z_scale_factor = 0.01;

    // auto offset
    if (-360 < min_x  && -360 < min_y && max_x < 360 && max_y < 360)
    {
      x_offset = 0;
      y_offset = 0;
      z_offset = 0;
    }
    else
    {
      x_offset = ((int)((min_x + max_x)/200000))*100000;
      y_offset = ((int)((min_y + max_y)/200000))*100000;
      z_offset = ((int)((min_z + max_z)/200000))*100000;
    }
  }
};

typedef QtPointQuantizerTemplate<int> QtPointQuantizer;

#endif // QTPOINTQUANTIZER_HPP
