package com.daqo.dbinfo.renderer;

import javax.swing.JComboBox;
import java.util.Vector;
import java.awt.Color;

import com.daqo.dbinfo.ConstantDef;
import com.sun.java.swing.plaf.windows.WindowsTreeUI;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.Component;
import javax.swing.JTable;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//在用ComboBox输入了数据时，需要的实际是信息域的值,getSelectedItem完成此转换
//初始ComboBox时，由于JTable中cell的内容在ComboBox中没有，需要转换，setSelectedItem完成此转换
public class ComboBoxEx
    extends JComboBox {
  public Vector vDisp = new Vector(0); //显示域值
  public Vector vReal = new Vector(0); //实际值
  public ComboBoxEx() {
    setBackground(new Color(ConstantDef.CRED ,ConstantDef.CGREEN ,ConstantDef.CBLUE )) ;
    //this.setUI(new com.sun.java.swing.plaf.windows.WindowsComboBoxUI()) ;
  }

  //显示值转换到实际值
  public Object getSelectedItem() {
    Object o = dataModel.getSelectedItem();
    try {
      String tmp = o.toString().trim();
      int size = vDisp.size();
      for (int i = 0; i < size; i++) {
        if (vDisp.elementAt(i).toString().trim().equalsIgnoreCase(tmp)) { //在显示内容中找到了相等的值
          o = new String(vReal.elementAt(i).toString().trim()); //返回实际的值
          break;
        } //if
      } //for
    }
    catch (NullPointerException ne) {
    }
    return o;
  }

  //实际值转换到显示值
  public void setSelectedItem(Object anObject) {
    int count = vReal.size();
    Object oNew = anObject;
    try {
      String item = anObject.toString(); //实际内容
      for (int i = 0; i < count; i++) {
        if (item.equalsIgnoreCase(vReal.elementAt(i).toString())) {
          oNew = new String(vDisp.elementAt(i).toString());
          break;
        } //if
      } //for i
      dataModel.setSelectedItem(oNew);
    }
    catch (NullPointerException ne) {
    }
  }

}
