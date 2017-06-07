package com.daqo.dbinfo.renderer;

import javax.swing.JComboBox;
import java.util.Vector;
import java.awt.Color;

import com.daqo.dbinfo.ConstantDef;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.Component;
import javax.swing.JTable;


/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */
//取文件列表字符串中的索引值专用
//在用ComboBox输入了数据时，需要的实际是索引，getSelectedItem完成此转换
//初始ComboBox时，由于JTable中cell的内容在ComboBox中没有，需要转换，setSelectedItem完成此转换
public class ComboBoxEvalueMode
    extends JComboBox {
  public ComboBoxEvalueMode() {
    setBackground(new Color(ConstantDef.CRED ,ConstantDef.CGREEN ,ConstantDef.CBLUE )) ;

  }

  public Object getItemAt(int index) {
    return dataModel.getElementAt(index);
  }

  //显示值转换到实际值
  public Object getSelectedItem() {
    Object o = dataModel.getSelectedItem();
    try {
      String tmp = o.toString().trim();
      int loc1 = tmp.indexOf("*");
      if (loc1 >= 1) {
        o = new String(tmp.substring(0, loc1));
      }
    }
    catch (NullPointerException ne) {
    }
    return o;
  }

  //实际值转换到显示值
  //实际值有可能为空,如，新增加一条记录时
  public void setSelectedItem(Object anObject) {
    int count = super.getItemCount();
    String tmp = "";
    Object oNew = anObject;
    try {
      for (int i = 0; i < count; i++) {
        String item = super.getItemAt(i).toString().trim();
        int loc1 = item.indexOf("*");
        if (loc1 >= 1) {
          tmp = item.substring(0, loc1);
        } //if
        if (tmp.equalsIgnoreCase(anObject.toString().trim())) {
          oNew = new String(item);
          break;
        } //if
      } //for i
      dataModel.setSelectedItem(oNew);
    }
    catch (NullPointerException ne) {
    }
  }

 }
