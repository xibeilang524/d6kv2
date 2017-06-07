package com.daqo.dbinfo;

import java.io.Serializable;
import java.util.Calendar;
import java.util.Vector;

/**
 * <p>Title: 数据库维护工具</p>
 * <p>Description: 数据库维护工具</p>
 * <p>Copyright: Copyright (c) 2016</p>
 * <p>Company: 南京大全自动化科技有限公司</p>
 * @author hongxiang
 * @version 1.0
 */

public class ConstantDef
    implements Serializable {

  public ConstantDef() {
  }
  public static Vector Vtablename =new Vector();//hx 存放被修改过的表名称

  public static final int CS_CHAR_TYPE = 0;
  public static final int CS_BINARY_TYPE = 1;
  public static final int CS_LONGCHAR_TYPE = 2;
  public static final int CS_LONGBINARY_TYPE = 3;
  public static final int CS_TEXT_TYPE = 4;
  public static final int CS_IMAGE_TYPE = 5;
  public static final int CS_TINYINT_TYPE = 6;
  public static final int CS_SMALLINT_TYPE = 7;
  public static final int CS_INT_TYPE = 8;
  public static final int CS_REAL_TYPE = 9;
  public static final int CS_FLOAT_TYPE = 10;
  public static final int CS_BIT_TYPE = 11;
  public static final int CS_DATETIME_TYPE = 12;
  public static final int CS_DATETIME4_TYPE = 13;
  public static final int CS_MONEY_TYPE = 14;
  public static final int CS_MONEY4_TYPE = 15;
  public static final int CS_NUMERIC_TYPE = 16;
  public static final int CS_DECIMAL_TYPE = 17;
  public static final int CS_VARCHAR_TYPE = 18;
  public static final int CS_VARBINARY_TYPE = 19;
  public static final int CS_LONG_TYPE = 20;
  public static final int CS_SENSITIVITY_TYPE = 21;
  public static final int CS_BOUNDARY_TYPE = 22;
  public static final int CS_VOID_TYPE = 23;
  public static final int CS_USHORT_TYPE = 24;

  public static final String MODELDB = "modeldb"; //SERVICE_TYPE,PARA_TABLE_DESCRIBLE,PARA_FIELD_DESCRIBLE都写定了放在此DB内
  public static final String HISDB = "";
  public static final String DBINFOPATH = "resource/dbconfig"; //数据库维护初始化文件的目录
  public static final int MAX_NAME_LEN = 128;
  public static final int MAX_COLNUM = 96;
  public static final int MAX_COLUMN_LEN = 256; //最大的列长度
  public static final float SPLIT1LOCATE = 0.18f;
  public static final float SPLITOUTLOCATE = 0.9f;
  public static final int DATATYPE = 1; //数据类型
  public static final int FLOATTYPE = 2; //浮点类型
  public static final int STRINGTYPE = 3; //字符类型
  public static final int BOOLEAN = 4; //boolean 类型
  public static final int MSGBUFSIZE = 2048; //错误信息缓冲区大小

  public static final String PARA_TABLE_DESCRIBLE = "pdb_table"; //参数表属性表
  public static final String TABLENAME = "tablename"; //

  public static final String APPTYPE = "apptype"; //
  public static final String LISTFIELD = "listfield"; //
  public static final String SORTFIELD = "sortfield"; //
  public static final String SORTINDEX = "sortindex"; //
  public static final String SEARCHFIELD = "searchfield"; //
  public static final String SEARCHEDTABLE = "searchedtable"; //
  public static final String SEARCHEDFIELD = "searchedfield"; //
  public static final String SHOWFIELD = "showfield"; //
  public static final String TABCOUNT = "tabcount"; //
  public static final String TAB1 = "tab1"; //
  public static final String TAB2 = "tab2"; //
  public static final String TAB3 = "tab3"; //
  public static final String TAB4 = "tab4"; //
  public static final String TAB5 = "tab5"; //
  public static final String ALIAS     = "alias";   //hx

  public static final int BASICORDER = 0; //
  public static int TABLENAMEID = BASICORDER;
  public static int APPTYPEID = BASICORDER + 1;
  public static int LISTFIELDID = BASICORDER + 2;
  public static int SORTFIELDID = BASICORDER + 3;
  public static int SORTINDEXID = BASICORDER + 4;
  public static int SEARCHFIELDID = BASICORDER + 5;
  public static int SEARCHEDTABLEID = BASICORDER + 6;
  public static int SEARCHEDFIELDID = BASICORDER + 7;
  public static int SHOWFIELDID = BASICORDER + 8;
  public static int TABCOUNTID = BASICORDER + 9;
  public static int TAB1ID = BASICORDER + 10;
  public static int TAB2ID = BASICORDER + 11;
  public static int TAB3ID = BASICORDER + 12;
  public static int TAB4ID = BASICORDER + 13;
  public static int TAB5ID = BASICORDER + 14;

  public static final String PARA_FIELD_DESCRIBLE = "pdb_field"; //参数域属性表
  public static final String PDBF_GROUPNO = "groupno"; //分组序号
  public static final String PDBF_DESCRIBE = "describe"; //描述
  public static final String PDBF_TABLENAME = "tablename"; //所属表名
  public static final String PDBF_DISPLAYSTYLE = "displaystyle"; //显示风格
  public static final String PDBF_DOTNUM = "dotnum"; //小数位数
  public static final String PDBF_EVALUATEMODE = "evaluatemode"; //赋值方式
  public static final String PDBF_DATASOURCE = "datasource"; //初始化信息源名
  public static final String PDBF_SRCDATAFIELD = "srcdatafield"; //列表域名
  public static final String PDBF_SEARCHEDFIELD = "searchedfield"; //信息域名
  public static final String PDBF_CONDITIONFLAG = "conditionflag"; //有选择域
  public static final String PDBF_CONDITIONFIELD = "conditionfield"; //选择域名
  public static final String PDBF_VALUEFROM = "valuefrom"; //选择域初值
  public static final String PDBF_VALUETO = "valueto"; //选择域终值
  public static final String PDBF_RELATEFIELD = "relatefield"; //关联域名
  public static final String PDBF_RELVALUEFROM = "relvaluefrom"; //关联域初值
  public static final String PDBF_RELVALUETO = "relvalueto"; //关联域终值
  public static final String PDBF_CALCFIELD1 = "calcfield1"; //计算源域1
  public static final String PDBF_CALCFIELD2 = "calcfield2"; //计算源域2
  public static final String PDBF_CALCSYMBOLE = "calcsymbole"; //计算关系
  public static final String PDBF_STATUS = "status"; //状态
  public static final String PDBF_TABNO = "tabno"; //所属表分区
  public static final String PDBF_DEFAULTVALUE = "defaultvalue"; //域缺省值
  public static final String PDBF_FIELDPROP = "fieldprop"; //域属性
  public static final String PDBF_ALIAS     = "alias";     //别名属性

  public static final int IN_BYKEYBOARD = 0; //键盘输入
  public static final int IN_CHECKBOX = 1; //checkbox
  public static final int IN_BYFILE_STRING = 2; //  取文件列表字符串
  public static final int IN_BYFILE_INDEX = 3; //取文件列表字符串中的索引值
  public static final int IN_BYTABLE_SELECTEDSTRING = 4; //取表记录列表字符串
  public static final int IN_BYTABLE_SPECIFIED_FIELD = 5; //取表指定域的域值
  public static final int IN_FIELDNAME = 6; //取表的域名信息
  public static final int IN_UNINPUT = 7; //不可输入域

  /*数据源是”datasource”域中指定的表，
     只不过是用该表在pdb_table中设置的分类索引列出选择值。如图所示。*/
  public static final int IN_SECOND_INDEX = 8; //表二次分类检索信息

  /*该域值是表名，在下拉列表框中选择。*/
  public static final int IN_TABLENAME = 9; //取表名
  public static final int IN_PASSWORD = 10; //密码

  /*该域不用人工输入，自动写入表名。这种输入方法只用在pdb_field的tablename输入上。*/
  public static final int IN_AUTO_INTABLENAME = 11; //自动显示表名

  /*由于该类型输入比较复杂，用举例的方式来说明。
     如：遥测参数表中的”设备代码” 用的就是这种输入方法。
     如果不用分类索引，列出全部的设备代码，那将要在非常长的一串代码中选一个值，效率低且易出错。
     而事实是遥测参数表已经二次分类索引，每次只列出一个终端的遥测参数。
     在遥测参数表二次索引的基础上再对设备代码索引,三次索引也就是这个意思。*/
  public static final int IN_THIRD_INDEX = 12; //域二次分类检索信息,即三次分类检索

  /*输入方式”2*取文件列表字符串”中，实际输入值为文件中的字符串，而这种方法实际输入值是
     该字符串在文件中的行序号,序号从0开始，逐行加1，遇到空行(要加至少1个空字符)也加1。*/
  public static final int IN_BYFILE_ORDER = 13; //取文件列表字符串序号

  /*在输入方式 8*二次分类检索中，数据源”datasource”只能是一个表。而这种方法却可以是
     多个表，每个表用”,”隔开。典型应用于”追忆点参数表”的”追忆对象代码”的输入中。
     因为追忆对象代码可以是”遥测参数表”中的代码，也可以是”遥信参数表”中的代码。
     如果用”8*二次分类检索”方式输入，每次只能有一个表中的代码列出来供选择。而该方法却可以把
     两个或多个表的代码列出来供选择，前提就是这几个表的二次分类索引方法完全一样。*/
  public static final int IN_SECOND_INDEX2 = 14; //多表二次分类检索
  /*开关保护关系参数表和开关保护模块关系参数表由于缺少”站端序号”字段，在索引查看和
     字段输入时存在困难，该输入方法是为这两个表特制的。字段含义同  “5*取表指定域的域值”。*/
  public static final int IN_KGBH_FIELD = 15; //开关保护关系表域输入

  public static final int IN_FLOAT = 100; //浮点型数据

  public static  final String SCDTYPE = "类型";
  public  static final String SCDDEVID = "devid";
  public static final String SCDDEVTABLE = "SCD设备参数表";
  public static final int SCDDEVIDCOLNO = 2;//SCD设备参数表的DEVID列序号
  public static final int SCDDEVTYPECOLNO = 7;//类型列号

  public static final String KGBHMKGXPARA = "开关保护模块关系参数表"; //开关保护模块关系参数表
  public static final String KGBHGXPARA = "开关保护关系参数表"; //开关保护关系参数表
  public static final String YXPARATABLE = "遥信参数表"; //遥信参数表
  public static final String YCPARATABLE = "遥测参数表";//
  //public static final String PARA_TABLE_DESCRIBLEKEY = TABLENAME; //表名
  public static final String PARA_TABLE_DESCRIBLEKEY = TABLENAME; //表名+别名
  public static final String PARA_TABLE_DESCRIBLEKEY_ALIAS=TABLENAME+","+ALIAS; //表名+别名
  public static final String USER_PASSTABLE = "用户口令参数表";
  public static final String SERVICE_TYPE = "服务类型参数表";
  public static final String SYSTEM_DM = "SYSTEM_DM";
  public static final String DBINFOINI = "dbinfoini";
  public static final String RTDB_TABLE = "rtdb_table"; //实时库表模式
  public static final String RTDB_FIELD = "rtdb_field";
  //CIM模型专用 hx
  public static final String DEV = "DEV"; //设备组的定义
  public static final String Area_TABLE ="区域参数表";
  public static final String Station_TABLE ="站所线系参数表";
  public static final String Voltage_TABLE ="电压等级表";
  public static final String Separate_TABLE ="间隔参数表";
  public static final String ScdType_TABLE ="SCD设备类型表";
  public static final String Dev_TABLE ="SCD设备参数表";
  public static final String ZSXXDM="站所线系代码";
  public static final String LX="类型";
  public static final String DYDJ="高电压等级";
  public static final String SBDM="设备代码";
  public static final String AreaID = "区域序号"; //
  public static final String SEPARATE="所属间隔";//南网测试2012
  public static final String SSCZ="所属厂站";//南网测试2012
  //CIM模型专用
  public static User userINSTANCE = new User();
  public static long loginMinuteTime = 0; //登录时的时间分
  public static int logoutTime = 15;      //登录保持时间分,测试改为1分钟
  public static String statusText;
  public static void setLoginTime() {
    loginMinuteTime = Calendar.getInstance().getTime().getTime() / (1000 * 60);
  }

  public static User getUserInstance() {
    return userINSTANCE;
  }

  //用户组
  public static int USERGROUP_SYS = 0; //系统管理
  public static int USERGROUP_YD = 1; //远动
  public static int USERGROUP_DD = 2; //调度
  public static int USERGROUP_PROTECT = 3; //保护

  //数据行操作方式
  public static int DB_UNCHANGED = 0;
  public static int DB_INSERT = 1;
  public static int DB_CHANGED = 2;

  public static int DB_CODE_ERR = 10;//代码偏差

  public static int DB_RECORD_ADD = 1;
  public static int DB_RECORD_INSERT = 2;


  //表的域信息序号
  public static int FIELD_NAME = 0;//域名
  public static int  FIELD_TYPE = 1;//数据类型
  public static int FIELD_KEY = 2;//是否主键

  public static int CRED =  238;//212;
  public static int CGREEN = 238;//208;
  public static int CBLUE = 238;//200;


  public static String FONTNAME = "Dialog";

}
