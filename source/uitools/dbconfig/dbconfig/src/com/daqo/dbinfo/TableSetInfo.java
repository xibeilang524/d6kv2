package com.daqo.dbinfo;

import java.util.Vector;
import com.daqo.dbinfo.table.FieldProp;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */
//表的设置信息
//列值的输入方式
//在pdb_table和pdb_field中的信息等等

public class TableSetInfo {
  public Vector fieldeInputMode = new Vector(0); //域值输入方式
  public Vector thiscol = new Vector(0); //相应的列号

  FieldProp tableFieldInfo;
  private int tabcount; //分区数

  public TableSetInfo() {

  }

  public int getTabCount() {
    return tabcount;
  }

  public void setTabCount(int tabcount) {
    this.tabcount = tabcount;
    for (int i = 0; i < tabcount; i++) {
      fieldeInputMode.add(new Vector(0));
      thiscol.add(new Vector(0));
    }
  }

  public void setInputMode(int tabno, int col, int mode) {
    ( (Vector) fieldeInputMode.elementAt(tabno)).addElement(Integer.toString(
        mode)); //tabno is based 0//保存列号
    ( (Vector) thiscol.elementAt(tabno)).addElement(Integer.toString(
        col)); //保存列的输入方式

  }
}
