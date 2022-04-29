#ifndef QTLAYER_H
#define QTLAYER_H

#include <QObject>
#include <QMutex>
#include <QList>
#include "lasconv_global.h"
#include "QtLayerInfo.h"
#include "QtAttributes.hpp"

class QtLayerManager;

#define INVALID_LAYER_ID (-1)
#define DEFINE_LAYER_INFO_KEY(key, value) static QString key() { return QString(value); }

class LASCONV_EXPORT QtLayer : public QObject
{
  Q_OBJECT
signals:
  void Locked(int nID);
  void Unlocked(int nID);
  void Selected(int nID);
  void Unselected(int nID);
  void Shown(int nID);
  void Hidden(int nID);
  void AboutToDestroy(int nID);

public:
  enum LayerTypeFlag
  {
    LayerType_Invalid       = -1,
    LayerType_PointCloud    = 0,
    LayerType_Strip         = 1,
    LayerType_Raster        = 2,
    LayerType_Model         = 3,
    LayerType_Vector        = 4,
    LayerType_RasterTerrain = 5,
    LayerType_MultiSpectral = 6
  };

public:
  DEFINE_LAYER_INFO_KEY(KEY_LAYER_NAME,   "layer_name")
  DEFINE_LAYER_INFO_KEY(KEY_LAYER_ID,     "layer_id")
  DEFINE_LAYER_INFO_KEY(KEY_LAYER_TYPE,   "layer_type")
  DEFINE_LAYER_INFO_KEY(KEY_LAYER_FORMAT, "layer_format")

public:
  explicit QtLayer(LayerTypeFlag type, QObject *parent = nullptr);
  virtual ~QtLayer();

  QString LastError() const;

  /* 初始化接口 */
  virtual bool Initialize() = 0;
  virtual bool InitFromAttributes(const QtStrAttributes &attrs) = 0;

  /* 图层基础属性 */
  int           LayerID() const;     // 图层唯一，只有添加到图层管理器的图层，其ID有效
  QString       LayerName() const;   // 图层名称
  LayerTypeFlag LayerType() const;   // 图层类型
  QString       LayerFormat() const; // 图层格式，用于关联驱动
  QtLayerInfo*  LayerInfo() const;   // 图层信息，保存图层的所有属性，实时同步更新

  /* 图层状态 */
  bool Lock();
  void Unlock();
  bool IsLocked() const;

  void Select();
  void Unselect();
  bool IsSelected() const;
  void SetSelected(bool bSelected);

  void Show();
  void Hide();
  bool IsVisible() const;
  void SetVisible(bool bVisible);

protected:
  void    SetLayerID(int nID);
  void    SetLayerName(const QString &name);
  void    SetLayerType(LayerTypeFlag type);
  void    SetLayerFormat(const QString &strFormat);

  QString GetAttribute(const QString &strKey) const;
  void    SetAttribute(const QString &strKey, const QString &strVal);

  bool    RaiseError(const QString &strError);

  static int CreateLayerID();

private:
  QMutex       m_mtxLock;
  bool         m_bLocked;
  bool         m_bSelected;
  bool         m_bVisible;
  QtLayerInfo* m_pLayerInfo;
  QString      m_strLastError;
};

typedef QList<QtLayer*> QtLayerList;

#endif // QTLAYER_H
