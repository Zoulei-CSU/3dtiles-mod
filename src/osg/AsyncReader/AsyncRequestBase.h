#ifndef AsyncRequestBase_h__
#define AsyncRequestBase_h__
#include<windows.h>
#include<osg/Referenced>
#include<osg/ref_ptr>
#include<osg/observer_ptr>
#include<osg/Group>


class AsyncRequestBase;
class AsyncFunctorBase :public osg::Referenced
{
public:
	AsyncFunctorBase(osg::ref_ptr<AsyncRequestBase> request);
	
	virtual ~AsyncFunctorBase() {}
	virtual void doFunc();
	
	osg::ref_ptr<AsyncRequestBase> _request;

protected:
	void submit(osg::ref_ptr<osg::Node> node);
	void replace(osg::ref_ptr<osg::Node> node);
};

class AsyncRequestBase :public osg::Referenced
{
public:
	AsyncRequestBase() 
	{
		_functor = nullptr;
	}
	virtual ~AsyncRequestBase() 
	{
		if (_functor != nullptr)
			delete _functor;
	}
	osg::ref_ptr<AsyncFunctorBase> _functor;
	//AsyncFunctorBase* _functor;
	osg::observer_ptr<osg::Group> _container;
};
#endif // AsyncRequestBase_h__
