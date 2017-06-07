package com.daqo.dbinfo;

import java.io.Serializable;
import java.util.Vector;
import javax.swing.tree.DefaultMutableTreeNode;

import com.daqo.dbinfo.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class Dlllink
    implements Serializable {

  private static final Dlllink INSTANCE = new Dlllink();
  public static int dbstatus = 0;
  public static String runhome = "";

  public static Dlllink getInstance() {
    return INSTANCE;
  }

  public Dlllink() {
  }

  public String substr(byte[] str) {
    int i = 0;
    for (i = 0; i < str.length; i++) {
      if (str[i] == 0) {
        break;
      }
    }
    String tmp = new String(str, 0, i);
    return tmp.trim();

  }

  public native int startdb(); //连接数据库

  public native int closedb();

  public native byte[] getenvvar(); //得到环境变量

  public native int getTableInfo(byte[] dbname, byte[] tablename,
                                 byte[][] tableinfo, int[] maxlength,
                                 byte[] tableType);

  public native int openTableInfo(byte[] dbname, byte[] tablename);

  public native void closeTableInfo();

  public native int readdata(byte[][] buf);

  public native int readRecorders(byte[] dbname, byte[] tablename,
                                  byte[] fieldname, byte[] where,
                                  byte[] orderfn, int[] fieldcount,
                                  boolean bDistinct);

  public native int insertARecord(byte[][] colvalue);

  public native int updateARecord(byte[][] colvalue, int[] field,
                                  int keycolnums);

  public native int removeARecord(byte[] where);

  public native int reCall(byte[][] server, byte[][] name); //加载实时库

  public native int reloadRtdb(byte[][] server); //加载实时库

  public native int reloadMode(int load_type);

  public native int sendMsgToFert(int groupno, byte type, byte[] tablename);

  public native int sendMsgToWeb();

  public native int sendEventMsg(byte[] username,byte[] tablename,byte[] hostname);

//maxminfalg,最大最小值标志. ==0,最大，＝＝1最小
  public native int getAMaxMinVal( byte[]dbname,byte[]table,byte[]field,
                                   byte maxminfalg);

  //执行一个过程
  public native int execproc(int flag ,int devid,int devtype,int olddevtype);

  public native byte[] makeBcp(byte[] dbname, byte[] srvname, byte[] filename,
                               byte[] username, byte[] pass, boolean inoutf); //生成bcp文件

  static {
    try {
      System.loadLibrary("dblink");
    }
    catch (SecurityException se) {
      se.printStackTrace() ;
    }
    catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace() ;
    }
    catch(Exception e)
    {
      e.printStackTrace() ;
    }
  }


  //得到表中和一个srcfieldname对应的域值(objfieldname)
  //objdatatype为数据类型
  public byte[][] getAMatchFieldValue(String dbname, String tablename,
                                      String objfieldname, String srcfieldname,
                                      String srcfieldvalue, int[] objdatatype) {
    byte[][] buf = new byte[0][0];
    int srcfieldtype = getFieldType(dbname, tablename, srcfieldname);
    objdatatype[0] = getFieldType(dbname, tablename, objfieldname);
    String wh;
    if (srcfieldtype == ConstantDef.DATATYPE) {
      wh = srcfieldname + "=" + Integer.parseInt(srcfieldvalue);
    }
    else if (srcfieldtype == ConstantDef.FLOATTYPE) {
      wh = srcfieldname + "='" + Float.parseFloat(srcfieldvalue) + "'";
    }
    else {
      wh = srcfieldname + "='" + srcfieldvalue + "'";
    }

    int[] fieldcount = new int[1];
    buf = queryAnyTable(dbname, tablename, objfieldname, wh, "", fieldcount, true);

    return buf;
  }

  //判断某一个域的数据类型
  public int getFieldType(String dbname, String tablename, String fieldname) {
    byte[][] tableinfo = new byte[ConstantDef.MAX_COLNUM *
        3][ConstantDef.MAX_NAME_LEN];
    int[] maxlength = new int[1];
    byte[] tableType = new byte[1];
    int fieldnums = getTableInfo(dbname.getBytes(), tablename.trim().getBytes(),
                                 tableinfo, maxlength, tableType);
    if (fieldnums > 0) {
      for (int i = 0; i < fieldnums; i++) {
        if (fieldname.equalsIgnoreCase(substr(tableinfo[i * 3])) == true) {
          String type = substr(tableinfo[i * 3 + 1]);
          int itype = Integer.parseInt(type);
          switch (itype) {
            case ConstantDef.CS_TINYINT_TYPE:
            case ConstantDef.CS_SMALLINT_TYPE:
            case ConstantDef.CS_USHORT_TYPE:
            case ConstantDef.CS_INT_TYPE:
            case ConstantDef.CS_LONG_TYPE: {
              return ConstantDef.DATATYPE;
            }
            case ConstantDef.CS_CHAR_TYPE:
            case ConstantDef.CS_VARCHAR_TYPE: {
              return ConstantDef.STRINGTYPE;
            }
            case ConstantDef.CS_FLOAT_TYPE:
            case ConstantDef.CS_REAL_TYPE: {
              return ConstantDef.FLOATTYPE;
            }
          }
        }
      }
    }
    return -1;
  }

  //判断某多个域的数据类型
  public int[] getFieldsType(String dbname, String tablename,
                             String[] fieldname) {
    byte[][] tableinfo = new byte[ConstantDef.MAX_COLNUM *
        3][ConstantDef.MAX_NAME_LEN];
    int[] maxlength = new int[1];
    int fieldnameArrLen = fieldname.length;
    int[] fieldtype = new int[fieldnameArrLen];
    int count = 0;
    if (fieldnameArrLen < 1) {
      return fieldtype;
    }
    byte[] tableType = new byte[1];
    int fieldnums = getTableInfo(dbname.getBytes(), tablename.getBytes(),
                                 tableinfo, maxlength, tableType);
    if (fieldnums > 0) {
      int j;
      for (j = 0; j < fieldnameArrLen; j++) {
        for (int i = 0; i < fieldnums; i++) {
          if (fieldname[j].equalsIgnoreCase(substr(tableinfo[i * 3])) == true) {
            String type = substr(tableinfo[i * 3 + 1]);
            int itype = Integer.parseInt(type);
            switch (itype) {
              case ConstantDef.CS_TINYINT_TYPE:
              case ConstantDef.CS_SMALLINT_TYPE:
              case ConstantDef.CS_USHORT_TYPE:
              case ConstantDef.CS_INT_TYPE:
              case ConstantDef.CS_LONG_TYPE: {
                fieldtype[count++] = ConstantDef.DATATYPE;
                break;
              }
              case ConstantDef.CS_CHAR_TYPE:
              case ConstantDef.CS_VARCHAR_TYPE: {
                fieldtype[count++] = ConstantDef.STRINGTYPE;
                break;
              }
              case ConstantDef.CS_FLOAT_TYPE:
              case ConstantDef.CS_REAL_TYPE: {
                fieldtype[count++] = ConstantDef.FLOATTYPE;
                break;
              }
            } //switch
            if (count == fieldnameArrLen) {
              break;
            }
          } //if
        } //for i
      } //for j
    } //if
    return fieldtype;
  } //public

  public boolean addIndexNodes(DefaultMutableTreeNode node, String dbname,
                               String tablename, String fieldname, String where,
                               String orderfn, boolean bDistinct) {
    boolean ret = false;
    int[] fieldcount = new int[1];
    byte[][] buf = queryAnyTable(dbname, tablename, fieldname, where, orderfn,
                                 fieldcount, bDistinct);
    if (buf.length > 0) {
      ret = true;
      for (int i = 0; i < buf.length; i++) {
        DefaultMutableTreeNode nodetable = new DefaultMutableTreeNode(substr(
            buf[i]).trim());
        node.add(nodetable);
      }
    }
    return ret;
  }

  public byte[][] queryAnyTable(String dbname, String tablename,
                                String fieldname, String where, String orderfd,
                                int[] fieldcount, boolean bDistinct) {
    byte[][] buf = new byte[0][0];
    if (dbstatus != 0) {
      byte[] table = tablename.trim().getBytes();
      byte[] fn = fieldname.trim().getBytes();
      byte[] wh = where.trim().getBytes();
      byte[] ord = orderfd.trim().getBytes();
      byte[] db = dbname.trim().getBytes();
      int valuecount = readRecorders(db, table, fn, wh, ord, fieldcount,
                                     bDistinct);
      if (valuecount > 0) {
        buf = new byte[valuecount][ConstantDef.MAX_COLUMN_LEN];
        valuecount = readdata(buf);
      }
    }
    return buf;
  }

  //把查到的内容放入j行i列的值Vector
  public int queryAnyTable(String dbname, String tablename, String fieldname,
                           String where, String orderfd, Vector vContent,
                           boolean bDistinct) {
    if (fieldname == null || fieldname.equalsIgnoreCase("")) {
      return -1;
    }
    byte[][] buf;
    int[] fieldcount = new int[1];
    buf = queryAnyTable(dbname, tablename, fieldname, where, orderfd,
                        fieldcount, bDistinct);
    if (fieldcount[0] == 0) {
      return 0;
    }
    for (int i = 0; i < fieldcount[0]; i++) {
      Vector v = new Vector(0); //每个vector放1列的值
      vContent.add(i, v);
    }
    int rowx = buf.length / fieldcount[0]; //列数
    for (int i = 0; i < fieldcount[0]; i++) {
      for (int j = 0; j < rowx; j++) {
        ( (Vector) vContent.elementAt(i)).add(j,
                                              substr(buf[j * fieldcount[0] + i])); //j行i列的值
      }
    }
    return vContent.size();
  }

  public byte[][] queryPDBTable(String where) { //pdb_table
    byte[][] buf;
    int[] fieldcount = new int[1];
    buf = queryAnyTable(ConstantDef.MODELDB, ConstantDef.PARA_TABLE_DESCRIBLE,
                        "*", where, "", fieldcount, false);
    return buf;
  }

  public byte[][] querydbinfoini(String where) { //查询dbinfoini表的信息
    byte[][] buf;
    int[] fieldcount = new int[1];
    buf = queryAnyTable(ConstantDef.MODELDB, ConstantDef.DBINFOINI, "项目内容",
                        where, "序号", fieldcount, false);
    return buf;
  }

  //根据条件查询pdb_field信息
  public byte[][] queryPDBField(String where) { //pdb_field
    byte[][] buf;
    int[] fieldcount = new int[1];
    buf = queryAnyTable(ConstantDef.MODELDB, ConstantDef.PARA_FIELD_DESCRIBLE,
                        "*", where, "", fieldcount, false);
    return buf;
  }

  //查询表信息
  public byte[][] queryTableInfo(String dbname, String tablename,
                                 int[] fieldcount) {
    byte[] table = tablename.trim().getBytes();
    int[] maxlength = new int[1];
    byte[][] buf = new byte[0][0];
    byte[][] tableinfo = new byte[ConstantDef.MAX_COLNUM *
        3][ConstantDef.MAX_NAME_LEN]; //表的全部信息
    byte[] tableType = new byte[1];
    int fieldnums = getTableInfo(dbname.trim().getBytes(), table, tableinfo,
                                 maxlength, tableType);
    fieldcount[0] = fieldnums;
    if (fieldnums > 0) {
      buf = tableinfo;
    }
    return buf;
  }

  public String getSortField(String dbname, String tablename) { //得到排序域名
    String wh = ConstantDef.TABLENAME + "='" + tablename + "'";
    int[] fieldcount = new int[1];
    byte[][] buf = queryAnyTable(dbname, ConstantDef.PARA_TABLE_DESCRIBLE,
                                 ConstantDef.SORTFIELD, wh, "", fieldcount, false);
    if (buf.length > 0) {
      String rtn = substr(buf[0]);
      return rtn;
    }
    return "";
  }

  //得到实时库的相关装载表的对应表名和服务器
  public int getRelatedLoadTable(String dbname, String tablename, Vector vtb) {
    //vtb:Vector1,表名；Vector2,服务器名
    Vector vContent = new Vector(0);
    String fieldname = "paratable,sername,relateloadtable";
    queryAnyTable(dbname, ConstantDef.RTDB_TABLE, fieldname, "", "tag", vContent, false);
    if (vContent.size() == 0) {
      return 0;
    }
    String tmptable = "";
    Vector v1 = ( (Vector) vContent.elementAt(2)); //relateloadtable
    Vector vname = new Vector(0);
    Vector vsername = new Vector(0);
    int size = v1.size();
    for (int i = 0; i < size; i++) {
      tmptable = v1.elementAt(i).toString();
      if (tmptable.equalsIgnoreCase(tablename)) {
        vname.addElement( ( (Vector) vContent.elementAt(0)).elementAt(i));
        vsername.addElement( ( (Vector) vContent.elementAt(1)).elementAt(i));
      }
      else {
        int loc1 = tmptable.indexOf(",");
        if (loc1 >= 0) {
          while (loc1 >= 0) {
            String tmptable1 = tmptable.substring(0, loc1);
            if (tmptable1.equalsIgnoreCase(tablename)) {
              vname.addElement( ( (Vector) vContent.elementAt(0)).elementAt(i));
              vsername.addElement( ( (Vector) vContent.elementAt(1)).elementAt(
                  i));
              break;
            }
            else {
              tmptable = tmptable.substring(loc1 + 1);
              loc1 = tmptable.indexOf(",");
              if (loc1 < 0) {
                if (tmptable.equalsIgnoreCase(tablename)) {
                  vname.addElement( ( (Vector) vContent.elementAt(0)).elementAt(
                      i));
                  vsername.addElement(((Vector) vContent.elementAt(1)).
                                      elementAt(i));
                }
              }
            } //else
          } //while
        } //if(loc1>=1)
      } //if
    } //for i
    if (vname.size() > 0) {
      vtb.addElement(vname); //表名
      vtb.addElement(vsername); //服务器名
    }
    return vtb.size();
  } //public
}
