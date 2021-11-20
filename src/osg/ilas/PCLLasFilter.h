#ifndef PCLLasFilter_h__
#define PCLLasFilter_h__

class PclLasFilter
{
public:
	PclLasFilter(QString lasFileName, QtProgressTask::Ptr progress)
	{
		_lasFileName = lasFileName;
		_progress = progress;
	}
	~PclLasFilter()
	{

	}

	bool filter()
	{
		LASreadOpener opener;
		opener.set_file_name(_lasFileName.toLocal8Bit());
		opener.set_buffer_size(1024 * 1024 * 128);
		opener.set_io_ibuffer_size(1024 * 1024 * 128); // 重新设置读取缓冲区大小，默认初始值为256KB

		LASreader *reader = opener.open();
		if (reader == nullptr)
			return SetRunningResult(false, _progress, tr("Failed to open file:") + _lasFileName);

		LASheader headerNew = reader->header.;

		UpdateProgress(_progress, tr("Importing file:") + _lasFileName);
		UpdateProgress(_progress, 0);




		reader->close();
		delete reader;
		reader = nullptr;
	}
private:
	QString _lasFileName;
	QtProgressTask::Ptr _progress;
};
#endif // PCLLasFilter_h__
