package com.daqo.dbinfo.table;

import java.util.Vector;

import com.daqo.dbinfo.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */
//读数据库，把输出结果放在Vector中
public class ReadDB {

  private String dbName, tableName; //数据库名，表名
  private int[] fieldcount = new int[1];
  private Vector vRecord = new Vector(0);//每条记录作为一个Vecotor,所有的记录放在vRecord中

  public ReadDB() {
  }

  public ReadDB(String dbname, String tablename) {
    dbName = dbname;
    tableName = tablename;
  }

  public void setDBName(String dbname) {
    dbName = dbname;
  }

  public void setTableName(String tablename) {
    tableName = tablename;
  }

//sortField,排序域名
  public void queryTable(String fieldname, String where, String sortField) {
    Dlllink pdll = Dlllink.getInstance();
    String sortfield = sortField;
    vRecord.setSize(0) ;
//    if (sortfield.equalsIgnoreCase("")) {
  //    pdll.getSortField(dbName, tableName);
   // }
    byte[][] buf = new byte[0][0];
    buf = pdll.queryAnyTable(dbName, tableName, fieldname, where, sortfield,
                             fieldcount, false);
    if (buf.length > 0 &&  fieldcount[0] != 0) {
      Vector vARecord = new Vector(0);
      String  aValue= "";
      for (int i = 0; i < buf.length / fieldcount[0]; i++) {
        vARecord = new Vector(0);
        for(int j = 0; j < fieldcount[0] ;j++)
          {
            aValue = pdll.substr(buf[i*fieldcount[0] + j]);
            vARecord.add(aValue) ;
          }//for j
        vRecord.add(vARecord) ;
      }//for i
    }
  }

  //得到记录数
  public int getRecords()
      {
        return vRecord.size() ;
      }

//得到域数
public int getFields()
    {
      return fieldcount[0];
    }

//得到一个域值
  public String getAFieldValue( int row, int col)
      {
        String svalue = "";
        if(row >= getRecords() || col > fieldcount[0])
            return svalue;
        svalue = ((Vector)vRecord.elementAt(row)).elementAt(col) .toString() ;
        return svalue;
      }
}
