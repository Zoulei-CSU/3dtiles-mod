#ifndef AsyncReader_h__
#define AsyncReader_h__
#include<osg/Referenced>
#include<osg/ref_ptr>
#include<osg/OperationThread>
#include<OpenThreads/Mutex>
#include<OpenThreads/Thread>
#include "AsyncRequestBase.h"
#include<list>


class AsyncReader :public osg::Referenced
{
public:
	static osg::ref_ptr<AsyncReader> instance();
	~AsyncReader();
	void addReadRequest(osg::ref_ptr<AsyncRequestBase> request);
	void cancel();
	void setThreadPause(bool pause);
protected:
	AsyncReader();
private:
	AsyncReader(AsyncReader const&) = delete;
	void operator=(AsyncReader const&) = delete;

protected:
	class AsyncReadThread :public osg::Referenced, public OpenThreads::Thread
	{
	public:
		AsyncReadThread(osg::ref_ptr<AsyncReader> reader)
			:_reader(reader), _done(false), _active(false)
		{
		}
		void setDone(bool done);
		bool getDone() const;

		void setActive(bool active);
		bool getActive() const;

		virtual int cancel();
		virtual void run();
	private:
		OpenThreads::Atomic _done;
		volatile bool _active;
		osg::ref_ptr<AsyncReader> _reader;
	};
	class AsyncRequestQueue :public osg::Referenced
	{
	public:
		typedef std::list< osg::ref_ptr<AsyncRequestBase> > RequestList;

		AsyncRequestQueue(osg::ref_ptr<AsyncReader> asyncReader) :
			_asyncReader(asyncReader)
		{
		}
		void addRequest(osg::ref_ptr<AsyncRequestBase> request);
		osg::ref_ptr<AsyncRequestBase> takeFirst();
// 		void block();
// 		void release();
// 		virtual void updateBlock();
// 		osg::ref_ptr<osg::RefBlock> _block;
		RequestList _requestList;
		osg::ref_ptr<AsyncReader> _asyncReader;
		OpenThreads::Mutex _mutex_list;
	};

	osg::ref_ptr<AsyncRequestQueue> _requestQueue;

	osg::ref_ptr<AsyncReadThread> _asyncThread;
	bool _threadPause;
	bool _running;
	bool _done;
	OpenThreads::Mutex _run_mutex;
};
#endif // AsyncReader_h__
