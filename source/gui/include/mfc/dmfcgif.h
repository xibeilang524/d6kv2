#ifndef _MFC_GIF_H
#define _MFC_GIF_H

#include "ddef/GPoint.h"
#include "ddef/GList.h"

class GFile;
class QWidget;
class QImage;
class QPixmap;

#define DISPOSAL_NO			0
#define DISPOSAL_NOT		4
#define DISPOSAL_RESTBACK	8
#define DISPOSAL_RESTORE	12

class GDE_EXPORT_MFCGIF GGifImage
{
public:

	GGifImage();
	GGifImage(const char* filePath);
	~GGifImage();

public:

	uint8   load(const char* filePath);
	uint8   append(const char* filePath);
	void    play(QWidget* p);
	uint8   append(const QImage& img);
	uint8   append(const QPixmap& pix);
	uint8   append(QImage* img);
	void	clear();
	int32	count() const;
	uint32	background() const;
	QImage* disposalImage(int32 index);
	QImage*	getImage(int32 index) const;
	int32	delayTime(int32 index) const;
	int32	disposalMethod(int32 index) const;
	uint8	isTrans(int32 index) const;
	uint32  transColor(int32 index) const;
	GPoint  getPos(int32 index) const;
	static uint8 isGif(const char* path);

protected:

	QImage*			m_previousImage;
	QImage*			m_redrawImage;
	int32			m_iGlobalColorSize;
	uint8*			m_pcGlobalColorTable;

	class GDE_EXPORT_MFCGIF GListQIMAGES:public GList<QImage*>{};
	class GDE_EXPORT_MFCGIF GListUINT8:public GList<uint8>{};
	class GDE_EXPORT_MFCGIF GListUINT32:public GList<uint32>{};
	class GDE_EXPORT_MFCGIF GListINT32:public GList<int32>{};
	class GDE_EXPORT_MFCGIF GListPTRS:public GList<void*>{};
	class GDE_EXPORT_MFCGIF GListPOINTS:public GList<GPoint>{};

	GListQIMAGES	m_imgs;
	GListUINT8		m_imgBTransIndex;
	GListUINT32		m_imgITransColor;
	GListINT32		m_imddisposalMethod;
	GListINT32		m_imgDelayTime;
	GListPOINTS		m_imgPos;
	int				m_currIndex;
	GListPTRS		m_players;

	uint8			m_bTransIndex;
	int32			m_iTransIndex;
	uint32			m_iBackgroundColor;
	uint16			m_iTop,m_iLeft;
	uint16			m_iWidth,m_iHeight;

	uint16			m_gifWidth,m_gifHeight;
	int32			m_iPass,m_iRow;
	uint8			m_cCurentByte,m_cPackedField;
	uint32			m_uBitSize,m_uPrimaryBitSize;
	uint32			m_uRemain,m_uReadByte,m_uBlockSize;
	int32			m_iTotalPos,m_iGifLen;
	int32			m_x,m_y;

protected:

	void	init();
	QImage* takeIt(GFile& fFile);
	void	outputImage(QImage& img,uint8 bit);
	uint8	getByte(GFile& fFile);
	uint16  getCode(GFile& fFile);

};

#endif
