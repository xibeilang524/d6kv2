package com.daqo.dbinfo.table;

import com.daqo.dbinfo.*;
import javax.swing.JOptionPane;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class ReadPDBTable {
  private  byte[][] buf = new byte[0][0]; //pdb_table中关于tableName的所有内容
  Dlllink pdll = Dlllink.getInstance();
  public ReadPDBTable() {
  }

  //从pdb_field中得到有关tableName的所有内容
  //返回域值个数
  public int getPDBProp(String tablename) {
    String wh;
    wh = ConstantDef.PDBF_TABLENAME + "= '" + tablename + "'";
    buf = pdll.queryPDBTable(wh);
    return buf.length ;
  }

  public int getPDBPropwhere(String where){
    buf = pdll.queryPDBTable(where);
    return buf.length ;
  }

  public String getAField(int col)
  {

    if(col >= buf.length ) return "";
    return   pdll.substr(buf[col]);
  }


}
