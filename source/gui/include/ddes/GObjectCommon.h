#ifndef GOBJECTCOMMON_H_HEADER_INCLUDED_BC939898
#define GOBJECTCOMMON_H_HEADER_INCLUDED_BC939898

//##ModelId=4340AAE401C5
//##Documentation
//## 定义：
//##       图元类型定义
//## 功能：
//##       定义图元类型
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-16
enum  { 
    //##Documentation
    //## 未知图元类型
    GD_TYPE_UNKNOWN = -1, 
    //##Documentation
    //## 基本图元线
    GD_TYPE_LINE, 
    //##Documentation
    //## 基本图元椭圆
    GD_TYPE_ELLIPSE, 
    //##Documentation
    //## 基本图元圆
    GD_TYPE_CIRCLE, 
    //##Documentation
    //## 基本图元矩形
    GD_TYPE_RECT, 
    //##Documentation
    //## 圆角矩形
    GD_TYPE_ROUNDRECT, 
    //##Documentation
    //## 基本图元折线
    GD_TYPE_POLYLINE, 
    //##Documentation
    //## 基本图元多边形
    GD_TYPE_POLYGON, 
    //##Documentation
    //## 基本图元弧线
    GD_TYPE_ARC, 
    //##Documentation
    //## 基本图元按钮
    GD_TYPE_BUTTON, 
    //##Documentation
    //## 基本图元静态文本
    GD_TYPE_TEXT, 
    //##Documentation
    //## 基本图元图像
    GD_TYPE_IMAGE, 
    //##Documentation
    //## 基本图元动态文本
    GD_TYPE_DYNTEXT, 
    //##Documentation
    //## 粘结点
    GD_TYPE_PIN,
	//##Documentation
    //## 母线
	GD_TYPE_BUS,
	//##Documentation
    //## 连接线
	GD_TYPE_LINK,
	//##Documentation
    //## 潮流线
	GD_TYPE_FLOW,
    //##Documentation
    //## 组合图元
    GD_TYPE_COMOBJ, 
    //##Documentation
    //## 自定义图元
    GD_TYPE_CUSTOM,
	//##Documentation
    //## 插件图元
    GD_TYPE_PLUGIN,
	//##Documentation
    //## 样条曲线
	GD_TYPE_BEZIER,
	//##Documentation
	//##圆弧图元
	GD_TYPE_CARC,
	//##Documentation
	//##路径图元
	GD_TYPE_PATH,
	//间隔图元
	GD_TYPE_BAY};

//##ModelId=4340AAE401C5
//##Documentation
//## 定义：
//##       设备类型定义
//## 功能：
//##       定义设备类型
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-16
enum{
	//##Documentation
    //## 未知设备类型
	GD_DEVTYPE_UNKNOWN=0};

//##ModelId=4340AAE401C5
//##Documentation
//## 定义：
//##       设备状态定义
//## 功能：
//##       定义设备状态
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-16
enum{
	//##Documentation
    //## 未知设备状态
	GD_DEVSTATUS_UNKNOWN=0x00,
	//##Documentation
    //## 不参与拓扑状态
	GD_DEVSTATUS_NOTTOPO=0x01};

//##ModelId=436C57150213
//##Documentation
//## 定义：
//##        线型风格定义
//## 功能：
//##        定义线型风格
//## 设计：
//##       宋永生
//## 日期：
//##       2005-10-3
enum  { 
    //##Documentation
    //## 空画笔
    GLS_NULL , 
    //##Documentation
    //## 实画笔
    GLS_SOLID , 
    //##Documentation
    //## 折线画笔
    GLS_DASH, 
    //##Documentation
    //## 点画笔
    GLS_DOT, 
    //##Documentation
    //## 折点线
    GLS_DASHDOT, 
    //##Documentation
    //## 折点点线
    GLS_DASHDOTDOT,
	//##Documentation
    //## 折线空折线
	GLS_DASHDASH};

//##ModelId=436C59200128
//##Documentation
//## 定义：
//##        箭头形状定义
//## 功能：
//##        定义箭头形状
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-5
enum  { 
    //##Documentation
    //## 空箭头
    GLA_NULL    = 0, 
    //##Documentation
    //## 简单实心箭头
    GLA_PURE    = 1, 
    //##Documentation
    //## 打开空心箭头
    GLA_OPEN    = 2, 
    //##Documentation
    //## 回线头
    GLA_STEALTH = 3, 
    //##Documentation
    //## 菱形箭头
    GLA_DIAMOND = 4, 
    //##Documentation
    //## 椭圆型箭头
    GLA_OVAL    = 5, 
    //##Documentation
    //## 实心填充
    GLAF_SOLID = 0x0000,
	//##Documentation
    //## 空心填充
    GLAF_EMPTY = 0x1000 };

//##ModelId=436C63FF0280
//##Documentation
//## 定义：
//##        背景模式定义
//## 功能：
//##        定义背景模式
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-5
enum  { 
    //##Documentation
    //## 锁定图像大小
    GFMD_LOCK, 
    //##Documentation
    //## 居中
    GFMD_CENTER,
    //##Documentation
    //## 平铺
    GFMD_TILE,
    //##Documentation
    //## 拉伸
    GFMD_STRETCH,
	//##Documentation
    //## 横向平铺-锁定高度
	GFMDT_HORZ,
	//##Documentation
    //## 纵向平铺-锁定宽度
	GFMDT_VERT,
	//##Documentation
	//##图形来源文件
	GFMD_FILE=0x00,
	//##Documentation
	//##图形来源数据内嵌
	GFMD_DATA=0x01};

//##ModelId=436C7524001F
//##Documentation
//## 定义：
//##       图元内部数据掩码定义
//## 功能：
//##       定义图元内部数据掩码
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-5
enum  { 
    //##Documentation
    //## 内部数据使用线风格时
    //##    (getStyleMask()&GBOM_LINESTYLE)==1
    GBOM_LINESTYLE = 0x01, 
    //##Documentation
    //## 内部数据使用填充风格时
    //##    (getStyleMask()&GBOM_FILLSTYLE)==1
    GBOM_FILLSTYLE = 0x02, 
    //##Documentation
    //## 内部数据使用文本风格时
    //##    (getStyleMask()&GBOM_TEXTSTYLE)==1
    GBOM_TEXTSTYLE = 0x04, 
    //##Documentation
    //## 内部数据使用可视范围时
    //##    (getStyleMask()&GBOM_RANGE)==1
    GBOM_RANGE = 0x08, 
    //##Documentation
    //## 内部数据使用内部比例因子时
    //##    (getStyleMask()&GBOM_SCALE)==1
    GBOM_SCALE = 0x10, 
    //##Documentation
    //## 内部数据使用内部旋转因子时
    //##    (getStyleMask()&GBOM_ROTATE)==1
    GBOM_ROTATE = 0x20 , 
    //##Documentation
    //## 内部数据使用内部数据链表时
    //##    (getStyleMask()&GBOM_DATA)==1
    GBOM_DATA = 0x40 ,	
	//##Documentation
	//## 内部数据使用扩展风格时
    //##    (getStyleMask()&GBOM_DEVTYPE)==1
	GBOM_DEVTYPE = 0x80,
	//##Documentation
	//## 内部数据使用扩展风格时
    //##    (getStyleMask()&GBOM_DEVSTATUS)==1
	GBOM_DEVSTATUS = 0x100,
	//##Documentation
	//## 内部数据,用于图元更新
    //##    (getStyleMask()&GBOM_UPDATE)==1
	GBOM_UPDATE = 0x10000000 };

//##ModelId=436C9F1D036B
//##Documentation
//## 定义：
//##       缩放掩码定义
//## 功能：
//##        定义缩放掩码
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-5
enum  { 
    //##Documentation
    //## 缩放
    GSFM_SCALE = 1, 
    //##Documentation
    //## 旋转
    GSFM_ROTATE= 2 };

//##ModelId=436C53240222
//##Documentation
//## 定义：
//##        填充掩码定义
//## 功能：
//##        定义填充掩码
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-5
enum  { 
    //##Documentation
    //## 透明显示
    GFM_TRANS = 0, 
    //##Documentation
    //## 填充颜色，存储颜色
    GFM_SOLID = 1, 
    //##Documentation
    //## 光栅填充
    GFM_RASTER = 2,
    //##Documentation
    //## 填充模式，存储模式
    GFM_PATTERN = 3, 
    //##Documentation
    //## 填充图像，存储图像
    GFM_IMAGE = 4 };

//##ModelId=436D785E0109
//##Documentation
//## 定义：
//##        光栅填充掩码定义
//## 功能：
//##        定义光栅填充掩码
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-5
enum  { 
   //##Documentation
    //## 简单填充
    GFMR_SIMPLE = 0x00, 
    //##Documentation
    //## 双色渐变
    GFMR_SHADE = 0x01, 
    //##Documentation
    //## 双色复变
    GFMR_EXSHADE = 0x02, 
    //##Documentation
    //## GL立体模式填充
    GFMR_GLMF = 0x03, 
	//角度填充
	GFMR_CONICAL = 0x04,

	//辐射填充
	GFMR_RADIAL = 0x05,

	//以下4个为渐变填充的方向
    //##Documentation
    //## 横向填充
    GFMR_HORZ = 0x00, 
    //##Documentation
    //## 纵向填充
    GFMR_VERT = 0x01,
	//##Documentation
	//## 自动方向填充
	//## 如果W>H，则横向填充
	//## 如果W<H，则纵向填充
	GFMR_AUTO = 0x02,

	//正斜向填充
	GFMR_SLASH = 0x03,
	//反斜向填充
	GFMR_BACKSLASH = 0x04,

	//以下4个为角度填充的可选角度
	//0度
	GFMR_0 = 0x04,
	//90度
	GFMR_90 = 0x05,
	//180度
	GFMR_180 = 0x06,
	//270度
	GFMR_270 = 0x07
     };


//##ModelId=436DB3B60232
//##Documentation
//## 定义：
//##        字体风格掩码定义
//## 功能：
//##        定义字体风格掩码
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-6
enum  { 
   //##Documentation
    //## 正常字体
    GTFS_NORMAL = 0x00, 
    //##Documentation
    //## 加粗字体
    GTFS_BOLD = 0x01, 
    //##Documentation
    //## 倾斜字体
    GTFS_ITALIC = 0x100, 
    //##Documentation
    //## 下划线
    GTFS_UNDERLINE = 0x200, 
    //##Documentation
    //## 上划线
    GTFS_OVERLINE = 0x400, 
    //##Documentation
    //## 删除线
    GTFS_STRIKEOUT = 0x800, 
    //##Documentation
    //## 阴影
    GTFS_SHADOW = 0x1000,
	//##Documentation
    //## 边框
    GTFS_FRAME	= 0x2000 ,
	//## Documentation
    //## 竖排字体
	GTFS_VERTICAL= 0x4000,
	//## Documentation
    //## 适配字体
	GTFS_AUTOSIZE= 0x8000 };

//##ModelId=436DB53E01F4
//##Documentation
//## 定义：
//##        字体对齐掩码定义
//## 功能：
//##        定义字体对齐掩码
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-6
enum  { 
    //##Documentation
    //## 居左对齐
    GTFA_LEFT = 0x01, 
    //##Documentation
    //## 居中对齐
    GTFA_CENTER = 0x02, 
    //##Documentation
    //## 居右对齐
    GTFA_RIGHT = 0x04, 
    //##Documentation
    //## 居上对齐
    GTFA_TOP = 0x10, 
    //##Documentation
    //## 垂直居中对齐
    GTFA_VCENTER = 0x20, 
    //##Documentation
    //## 居下对齐
    GTFA_BOTTOM = 0x40, 
    //##Documentation
    //## 自动折行
    GTFA_WRAP = 0x100 };


//##ModelId=436DFBC7000F
//##Documentation
//## 定义：
//##        字体背景模式掩码定义
//## 功能：
//##        定义字体背景模式掩码
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-6
enum  { 
    //##Documentation
    //## 透明显示
    GTM_TRANS = 0x00, 
    //##Documentation
    //## 透明显示
    GTM_TRANSPARENT = 0x00, 
    //##Documentation
    //## 不透明的
    GTM_OPAQUE = 0x01 };

//##ModelId=4376F21200FA
//##Documentation
//## 定义：
//##        点与图元关系
//## 功能：
//##        点与图元关系
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-13
enum  { 
    //##Documentation
    //## 移动操作，处于移动区域
    GHT_MOVE = 0x000001, 
    //##Documentation
    //## 尺寸调整
    GHT_SIZE = 0x000002, 
    //##Documentation
    //## 旋转中心点
    GHT_ROTATE = 0x000004,
    //##Documentation
    //## 处于选中状态,
	//## 用于矩形区域进行选择
    GHT_SELECT = 0x000008,
    //##Documentation
    //## 水平调整左边
    GHTC_SIZEHL = 0x000010, 
	 //##Documentation
    //## 水平调整右边
    GHTC_SIZEHR = 0x000020, 
    //##Documentation
    //## 垂直调整上边
    GHTC_SIZEVT = 0x000040, 
	//##Documentation
    //## 垂直调整下边
    GHTC_SIZEVB = 0x000080, 
    //##Documentation
    //## 上右调整上
    GHTC_SIZEBT = 0x000100, 
	 //##Documentation
    //## 下左调整下
    GHTC_SIZEBB = 0x000200, 
	 //##Documentation
    //## 上左调整上
    GHTC_SIZEFT = 0x000400, 
    //##Documentation
    //## 下右调整下
    GHTC_SIZEFB = 0x000800, 

    //##Documentation
    //## 起始约束
    GHTH_OS = 0x001000, 
    //##Documentation
    //## 终止约束
    GHTH_OE = 0x002000, 
    //##Documentation
    //## 其他约束
    GHTH_OC = 0x004000, 
    //##Documentation
    //## 内部处理约束，如果该标志为真
    //## 则图元拦截鼠标消息
    GHTH_OI = 0x008000,
	//##Documentation
    //## 可以点击选择约束
	GHTH_SELECT=0x010000};

//##ModelId=43793A3E035B
//##Documentation
//## 定义：
//##      绘制焦点内容选项
//## 功能：
//##      绘制焦点内容选项
//## 设计：
//##      宋永生
//## 日期：
//##      2005-11-15
enum  { 
    //##Documentation
    //## 绘制旋转中心点
    GDF_CENTER = 0x0001, 
    //##Documentation
    //## 使用旋转点绘制
    GDF_ROTATE = 0x0002, 
    //##Documentation
    //## 绘制操作点
    GDF_OPERATOR = 0x0004, 
    //##Documentation
    //## 绘制边框
    GDF_FRAME = 0x0008 , 
    //##Documentation
    //## 绘制鼠标滑过
    GDF_OVER = 0x0010 ,
	//##Documentation
    //## 热点滑过
	GDF_HOT  =0x0020};


//##ModelId=437937B700DA
//##Documentation
//## 定义：
//##       图元镜像操作定义
//## 功能：
//##       定义图元镜像操作
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-15
enum  { 
    //##Documentation
    //## 以X轴为对称轴，进行镜像
    GMF_X = 0x01, 
    //##Documentation
    //## 以Y轴为对称轴，进行镜像
    GMF_Y = 0x02, 
    //##Documentation
    //## 以O轴为对称轴，进行镜像
    GMF_O = 0x03, 
    //##Documentation
    //## 以Y=X轴为对称轴，进行镜像
    GMF_XY = 0x04, 
    //##Documentation
    //## 以Y=-X为对称轴，进行镜像
    GMF_NXY = 0x05 };

//##ModelId=437ED1D2036B
//##Documentation
//## 定义：
//##       图层类型定义
//## 功能：
//##       定义图层类型
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-19
enum  { 
    //##Documentation
    //## 共享层－共享层可以与共享层机互斥层同时显示
    //##         不能与独立层同时显示
    GLT_SHARE = 0x0000, 
    //##Documentation
    //## 互斥层－互斥层不能与互斥层、独立层同时显示
    //##         可以与共享层同时显示
    //##         
    GLT_MUTEX = 0x0001, 
    //##Documentation
    //## 独立层－不能与其他层同时显示
    GLT_SELF = 0x0002, 
    //##Documentation
    //## 隐藏层－该层不显示
    GLT_HIDE = 0x0003, 
    //##Documentation
    //## 锁定图层－出于该状态的图形不能被编辑，只能与可
    //##           以同时显示的图层显示
    GLT_LOCK = 0x00010000, 
    //##Documentation
    //## 关闭图层－处于该状态的图层不显示，不被编辑
    //##           直到该图层被打开
    GLT_CLOSE = 0x00020000 };
//##ModelId=4387F8BC00BB
//##Documentation
//## 功能：
//##    图元对齐选项，用于对齐图元
//## 定义：
//##    定义图元对齐选项
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum  { 
    //##Documentation
    //## 左对齐
    GBA_LEFT = 0x01, 
    //##Documentation
    //## 水平居中对齐
    GBA_CENTER = 0x02, 
    //##Documentation
    //## 右对齐
    GBA_RIGHT = 0x04, 
    //##Documentation
    //## 上对齐
    GBA_TOP = 0x08, 
    //##Documentation
    //## 下对齐
    GBA_BOTTOM = 0x10, 
    //##Documentation
    //## 垂直居中
    GBA_VCENTER = 0x20 };

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    图元等尺寸选项，用于等尺寸图元
//## 定义：
//##    定义图元等尺寸选项
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum  { 
    //##Documentation
    //## 等宽
    GBS_WIDTH = 0x01, 
    //##Documentation
    //## 等高
    GBS_HEIGHT = 0x02, 
    //##Documentation
    //## 等宽高
    GBS_BOTH = 0x04 };

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    按钮凸起及凹陷
//## 定义：
//##    定义按钮凸起及凹陷
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum  {
	//##Documentation
    //## 凸起
	GDBS_RAISED	  = 0x010000,
	//##Documentation
    //## 凹陷
	GDBS_SUNKEN	  = 0x020000};

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    按钮命令字
//## 定义：
//##    按钮命令字
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum  {
	//##Documentation
    //## 切换图形 
	btnHotFile,
	//##Documentation
    //## 弹出图形 
	btnHotCommand,
	//##Documentation
    //## 调用过程
	btnHotMacro,
	//##Documentation
	//## 切换图层
	btnHotLayer};

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    动态图元显示模式定义
//## 定义：
//##    动态图元显示模式定义
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum  { 
	//##Documentation
    //## NORMAL显示模式
	GDT_NORMAL= 0x01,
    //##Documentation
    //## LCD显示模式
    GDT_LCD   = 0x02, 
	//##Documentation
    //## 箭头显示模式
	GDT_ARROW = 0x04,
    //##Documentation
    GDT_SIGN = 0x08,
    //## 上箭头
    GDTA_UP   = 0x10, 
    //##Documentation
    //## 下箭头
    GDTA_DOWN = 0x20,
	//##Documentation
    //## 左箭头
    GDTA_LEFT = 0x40, 
    //##Documentation
    //## 右箭头
    GDTA_RIGHT= 0x80,
	//##Documentation
    //## 无箭头
    GDTA_MASK = 0xF0};

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    图元行为定义
//## 定义：
//##    图元行为定义
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum  {
	//##Documentation
    //## 不处理
	GOAM_NONE=0x00,
	//##Documentation
    //## 闪烁
	GOAM_FLASH=0x01,
	//##Documentation
    //## 变前景色
	GOAM_FCLR=0x02,
	//##Documentation
    //## 变背景色
	GOAM_BCLR=0x04,
	//##Documentation
    //## 边框色
	GOAM_LCLR=0x08,
	//##Documentation
    //## 变字色
	GOAM_TCLR=0x10,
	//##Documentation
    //## 变字背景色
	GOAM_TBCLR=0x20,
	//##Documentation
    //## 自动旋转
	GOAM_ROTATE=0x40 };

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    图元行为定义
//## 定义：
//##    图元行为定义
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum {
	//##Documentation
    //## 不闪烁
	GOAMF_NONE,
	//##Documentation
    //## 可见与不可见闪烁
	GOAMF_VISIBLE,
	//##Documentation
    //## 前景与背景交换闪烁
	GOAMF_INVERT,
	//##Documentation
    //## 边框绘制闪烁
	GOAMF_FRAME   ,
	//##Documentation
    //## 填充与不填充闪烁
	GOAMF_FILL};

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    弧线绘制模式
//## 定义：
//##    弧线绘制模式
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum {
	//##Documentation
    //## 弧线
	GPAM_ARC,
	//##Documentation
    //## 弓形
	GPAM_CHD,
	//##Documentation
    //## 扇形
	GPAM_PIE};

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    路径绘命令码
//## 定义：
//##    路径绘命令码
//## 设计：
//##    宋永生
//## 日期：
//##    20065-06-19
enum 
{
	//##Documentation
    //## 线条移动到某点
	GPH_MOVETO  =0x06,
	//##Documentation
    //## 绘制封闭线
	GPH_CLOSE	=0x01,
	//##Documentation
    //## 线条绘制到某点
	GPH_LINETO  =0x02,
	//##Documentation
    //## 贝赛尔曲线绘制到
	GPH_CURVETO	=0x04,
	//##Documentation
    //## 弧线绘制到
	GPH_ARCTO	=0x08};

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    弧线绘制模式
//## 定义：
//##    弧线绘制模式
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum {
	////##Documentation
    //## 折线
	GPM_LINES=0x00,
	//##Documentation
    //## 封闭
	GPM_CLOSE=0x01,
	//##Documentation
    //## Winding模式
	GPM_WINDING=0x02,
	//##Documentation
    //## 奇偶模式
	GPM_EVENODD=0x04};

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    坐标转换
//## 定义：
//##    坐标转换
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum {
	//##Documentation
    //## 等比缩放
	COOR_SCALE,
	//##Documentation
    //## 缩放宽度
	COOR_SCALEW,
	//##Documentation
    //## 缩放高度
	COOR_SCALEH};

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    连接状态
//## 定义：
//##    连接状态
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
//## 应用于连接图元使用m_extFlag变量
//## 对应连接线属性
//## 高字
//##	高字节标识连接个数
//##	低字节标识是否属性
//## 低字
//##	高字节标识连接个数
//##	低字节标识是否属性
//## 对应母线和端子
//## 只使用低字忽略高字
//## 低字
//##	高字节标识连接个数
//##	低字节标识是否属性

enum {
	//##Documentation
    //## 不分方向-对应母线端子
	LO_IGNORE=0x00,
	//##Documentation
    //## 首连接－对应连接线
	LO_START =0x01,
	//##Documentation
    //## 尾连接－对应连接线
	LO_END   =0x02,
	//##Documentation
    //## 一般连接操作
	LO_LINK  =0x03,
	//##Documentation
    //## 连接失败
	LS_FAILED=0x00,
	//##Documentation
    //## 连接成功
	LS_LINK	 =0x01};

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    图元鼠标响应
//## 定义：
//##    图元鼠标响应
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum
{
	//##Documentation
	//## 鼠标右键
	GMM_RBTN  = 0x00,
	//##Documentation
	//## 鼠标左键
	GMM_LBTN  = 0x01,
	//##Documentation
	//## 鼠标移动
	GMM_MOUSE = 0x02,
	//##Documentation
	//## 按下
	GMM_DOWN  = 0x00,
	//##Documentation
	//## 弹起
	GMM_UP	  = 0x01,
	//##Documentation
	//## 进入
	GMM_ENTER = 0x00,
	//##Documentation
	//## 退出
	GMM_LEAVE = 0x01,
	//##Documentation
	//## 移动
	GMM_MOVE  = 0x02
};

//##ModelId=4387F9CC007D
//##Documentation
//## 功能：
//##    图形缩放规则
//## 定义：
//##    图形缩放规则
//## 设计：
//##    宋永生
//## 日期：
//##    2005-11-26
enum
{
	//##Documentation
	//## 原图显示
	GFR_NORMAL,
	//##Documentation
	//## 自动适屏
	GFR_AUTOSIZE,
	//##Documentation
	//## 宽度分屏
	GFR_AUTOWIDTH,
	//##Documentation
	//## 高度分屏
	GFR_AUTOHEIGHT
};

#endif /* GOBJECTCOMMON_H_HEADER_INCLUDED_BC939898 */
