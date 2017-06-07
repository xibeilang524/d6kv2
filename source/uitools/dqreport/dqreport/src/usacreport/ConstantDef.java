package usacreport;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class ConstantDef {

  public ConstantDef() {
  }

  public static final int CS_CHAR_TYPE = 0;
  public static final int CS_BINARY_TYPE = 1;
  public static final int CS_LONGCHAR_TYPE = 2;
  public static final int CS_LONGBINARY_TYPE	 = 3;
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

  public static final String MODELDB = "modeldb";//SERVICE_TYPE,PARA_TABLE_DESCRIBLE,PARA_FIELD_DESCRIBLE都写定了放在此DB内
  public static final String HISDB = "modeldb";
  public static final String DBINFOPATH = "dbinfo";//数据库维护初始化文件的目录
  public static final int MAX_NAME_LEN = 128;
  public static final int MAX_COLNUM = 96;
  public static final int MAX_COLUMN_LEN = 256;//最大的列长度
  public static final float SPLIT1LOCATE = 0.18f;
  public static final float SPLITOUTLOCATE = 0.9f;
  public static final int DATATYPE = 1;//数据类型
  public static final int FLOATTYPE = 2;//浮点类型
  public static final int STRINGTYPE = 3;//字符类型
  public static final int BOOLEAN = 4;//boolean 类型
  public static final int MSGBUFSIZE = 2048;//错误信息缓冲区大小

  public static final String PARA_TABLE_DESCRIBLE = "pdb_table";//参数表属性表
  public static final String  TABLENAME = "tablename";//
  public static final String  APPTYPE = "apptype";//
  public static final String  LISTFIELD = "listfield";//
  public static final String  SORTFIELD = "sortfield";//
  public static final String  SORTINDEX = "sortindex";//
  public static final String  SEARCHFIELD = "searchfield";//
  public static final String  SEARCHEDTABLE = "searchedtable";//
  public static final String  SEARCHEDFIELD = "searchedfield";//
  public static final String  SHOWFIELD = "showfield";//
  public static final String  TABCOUNT = "tabcount";//
  public static final String  TAB1 = "tab1";//
  public static final String  TAB2 = "tab2";//
  public static final String  TAB3 = "tab3";//
  public static final String  TAB4 = "tab4";//
  public static final String  TAB5 = "tab5";//

  public static final int  BASICORDER = 0;//
  public static  int  TABLENAMEID  = BASICORDER;
  public static  int  APPTYPEID = BASICORDER+1;
  public static  int  LISTFIELDID = BASICORDER+2;
  public static  int  SORTFIELDID = BASICORDER+3;
  public static  int  SORTINDEXID = BASICORDER+4;
  public static  int  SEARCHFIELDID = BASICORDER+5;
  public static  int  SEARCHEDTABLEID = BASICORDER+6;
  public static  int  SEARCHEDFIELDID = BASICORDER+7;
  public static  int  SHOWFIELDID = BASICORDER+8;
  public static  int  TABCOUNTID = BASICORDER+9;
  public static  int  TAB1ID = BASICORDER+10;
  public static  int  TAB2ID = BASICORDER+11;
  public static  int  TAB3ID = BASICORDER+12;
  public static  int  TAB4ID = BASICORDER+13;
  public static  int  TAB5ID = BASICORDER+14;




  public static final String PARA_FIELD_DESCRIBLE = "pdb_field";//参数域属性表
  public static final String PDBF_GROUPNO = "groupno";//分组序号
  public static final String  PDBF_DESCRIBE = "describe";//描述
  public static final String  PDBF_TABLENAME = "tablename";//所属表名
  public static final String  PDBF_DISPLAYSTYLE = "displaystyle";//显示风格
  public static final String  PDBF_DOTNUM = "dotnum";//小数位数
  public static final String  PDBF_EVALUATEMODE = "evaluatemode";//赋值方式
  public static final String  PDBF_DATASOURCE = "datasource";//初始化信息源名
  public static final String  PDBF_SRCDATAFIELD = "srcdatafield";//列表域名
  public static final String  PDBF_SEARCHEDFIELD = "searchedfield";//信息域名
  public static final String  PDBF_CONDITIONFLAG = "conditionflag";//有选择域
  public static final String  PDBF_CONDITIONFIELD = "conditionfield";//选择域名
  public static final String  PDBF_VALUEFROM = "valuefrom";//选择域初值
  public static final String  PDBF_VALUETO = "valueto";//选择域终值
  public static final String  PDBF_RELATEFIELD = "relatefield";//关联域名
  public static final String  PDBF_RELVALUEFROM = "relvaluefrom";//关联域初值
  public static final String  PDBF_RELVALUETO = "relvalueto";//关联域终值
  public static final String  PDBF_CALCFIELD1 = "calcfield1";//计算源域1
  public static final String  PDBF_CALCFIELD2 = "calcfield2";//计算源域2
  public static final String  PDBF_CALCSYMBOLE = "calcsymbole";//计算关系
  public static final String  PDBF_STATUS = "status";//状态
  public static final String  PDBF_TABNO = "tabno";//所属表分区
  public static final String  PDBF_DEFAULTVALUE = "defaultvalue";//域缺省值
  public static final String  PDBF_FIELDPROP = "fieldprop";//域属性
  //兼容DLLLINK添加以下3个字段
  public static final String DBINFOINI = "dbinfoini";
  public static final String RTDB_TABLE = "rtdb_table"; //实时库表模式
  public static final String RTDB_FIELD = "rtdb_field";

  public static final int IN_BYKEYBOARD = 0;//键盘输入
  public static final int IN_CHECKBOX = 1;//checkbox
  public static final int IN_BYFILE_STRING = 2;//  取文件列表字符串
  public static final int IN_BYFILE_INDEX = 3;//取文件列表字符串中的索引值
  public static final int IN_BYTABLE_SELECTEDSTRING = 4;//取表记录列表字符串
  public static final int IN_BYTABLE_SPECIFIED_FIELD = 5;//取表指定域的域值
  public static final int IN_FIELDNAME = 6;//取表的域名信息
  public static final int IN_UNINPUT = 7;//不可输入域
  public static final int IN_SECOND_INDEX = 8;//表二次分类检索信息
  public static final int IN_TABLENAME = 9;//取表名
  public static final int IN_PASSWORD = 10;//密码
  public static final int IN_AUTO_INTABLENAME = 11;//自动显示表名
  public static final int IN_THIRD_INDEX = 12;//域二次分类检索信息,即三次分类检索
  public static final int IN_BYFILE_ORDER = 13;//取文件列表字符串序号
  public static final int IN_SECOND_INDEX2 = 14;//多表二次分类检索

  public static final int IN_FLOAT = 100;//浮点型数据

  public static final Boolean IF_HISDB_PARTITION = true;//历史表分区 add by hongxiang 20170117
}
