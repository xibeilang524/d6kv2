package com.daqo.dbinfo.table;

import javax.swing.JOptionPane;

import com.daqo.dbinfo.table.*;
import com.daqo.dbinfo.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */
//表的域属性

public class FieldProp {

  Dlllink pdll = Dlllink.getInstance();
  private String tableName; //表名
  private String dbName; //数据库名
  public int fieldnums = -1; //tablename域数
  public byte[][] buf = new byte[0][0]; //pdb_field中关于tableName的所有内容
  public byte[][] tableinfo = new byte[ConstantDef.MAX_COLNUM *
      3][ConstantDef.MAX_NAME_LEN]; //表的全部信息
  private String sTableType = ""; //表的类型,视图或基本表
  private String[]   sfieldname  = new String[0];

  public FieldProp() {
    }
  public FieldProp(String dbname, String tablename) {
    tableName = tablename.trim();
    dbName = dbname.trim();
  }

  public void setDBName(String dbname)
      {
        this.dbName = dbname;
      }

  public void setTableName(String tablename)
      {
        this.tableName = tablename;
      }

public int getTableFieldnums()
    {
      return fieldnums;
    }

  //从pdb_field中得到有关tableName的所有内容
  public void getPDBProp() {
     String wh;
     wh = ConstantDef.PDBF_TABLENAME + "= '" + tableName + "'";
     buf = pdll.queryPDBField(wh);
  }

  public String getTableType() {
    return sTableType;
  }

  //从数据库中得到表的基本信息
  public void getTableInfo() {
    byte[] table = tableName.getBytes();
    int[] maxlength = new int[1];
    byte[] type = new byte[1];
    byte[] db = dbName.getBytes();
    fieldnums = pdll.getTableInfo(db, table, tableinfo, maxlength, type);
    sTableType = new String(type);
    if(fieldnums >0 )
      {
        sfieldname = new String[fieldnums];
       for(int i = 0; i < fieldnums; i++)
         {
          sfieldname[i] =  pdll.substr(tableinfo[i * 3]);
         }
      }

  }

//得到某表的属性
  //col,列号；
  //propno,属性号。 0,域名；1，数据类型；2，是否主键
  public String getATableInfo(int col,int propno)
      {
    return pdll.substr(tableinfo[col*3 + propno]) ;
      }


  public String getTableFieldName(int col)
      {
        if(col >= sfieldname.length ) return "";

        return sfieldname[col];
      }

  //得到某列在表中的序号
  //序号的计算包含了不显示的列
  //如果列名是一个别名，则做一次转换，以获得正确的列号
  public int getColumnNo(String colName) {
    int colno = -1;
    int i;
    //
    String fieldname="";
    if(PreProcessor.ALIAS){
      if(ApplicationDB.frameDB!=null)
        fieldname=ApplicationDB.frameDB.GetFieldNameFromAlias(tableName,colName);
      if(fieldname.length()>0)
        colName=fieldname;
    }


    for (i = 0; i < sfieldname.length ; i++) {
      if (colName.equalsIgnoreCase(sfieldname[i])) {
        break;
      }
    }

    if (i < sfieldname.length ) {
      colno = i;
    }
    return colno;
  }
}
