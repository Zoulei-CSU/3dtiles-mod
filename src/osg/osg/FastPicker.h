#ifndef FastPicker_h__
#define FastPicker_h__
#include "FastPicker_dll.h"
#include "windows.h"
#include <osg/Camera>
#include <osgDB/WriteFile>
#include <osg/Image>
#include <OpenThreads/Mutex>
#include <osg/Matrix>
#include <osgViewer/Viewer>
#include <osg/GraphicsContext>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

class FastPicker_API FastPicker : public osg::Camera::DrawCallback
{
public:
	FastPicker(osg::ref_ptr<osgViewer::Viewer> viewer);
	void setParentNode(osg::ref_ptr<osg::Group> group);
	void setPickedNode(osg::ref_ptr<osg::Node> node);
	void setThreshold(int pixelThr);

	void resize(int width, int height);
	void update();

	bool pickPoint(float winX, float winY, osg::Vec3d& pt);

	void operator()(const osg::Camera& camera) const;
private:
	osg::ref_ptr<osg::Camera> initCamera();
private:
	osg::ref_ptr<osgViewer::Viewer> _viewer;
	osg::ref_ptr<osg::Group> _viewerNode;
	osg::ref_ptr<osg::Camera> _camera;
	osg::ref_ptr<osg::Image> _image;
	int _threshold;
};

#endif // FastPicker_h__
