package com.daqo.dbinfo.table;


import com.daqo.dbinfo.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class ReadPDBField {

  public ReadPDBField() {
  }
  private  byte[][] buf = new byte[0][0]; //pdb_field中关于tableName的所有内容
  int[] fieldcount = new int[1];

  //从pdb_field中得到有关tableName的所有内容
  public void getPDBProp(String tablename) {
    String wh;
    wh = ConstantDef.PDBF_TABLENAME + "= '" + tablename + "'";

 //根据条件查询pdb_field信息
    buf = Dlllink.getInstance().queryAnyTable(ConstantDef.MODELDB, ConstantDef.PARA_FIELD_DESCRIBLE,
        "*", wh, "", fieldcount, false);
    return ;
  }

  public int getRecs()
  {
    if(fieldcount[0] <=0) return 0;
    return buf.length /fieldcount[0];
  }

  public String getAFieldValue(int row,int col)
      {
      return Dlllink.getInstance().substr(buf[row*fieldcount[0] + col]) ;
      }
}
