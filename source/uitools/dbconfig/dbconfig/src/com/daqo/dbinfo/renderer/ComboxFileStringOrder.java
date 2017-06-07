package com.daqo.dbinfo.renderer;

import javax.swing.JComboBox;
import javax.swing.JTable;
import java.awt.Color;
import java.awt.Component;
import javax.swing.table.DefaultTableCellRenderer;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class ComboxFileStringOrder
    extends JComboBox {

  public ComboxFileStringOrder() {
  }

  public Object getItemAt(int index) {
    return dataModel.getElementAt(index);
  }

  //显示值转换到实际值
  public Object getSelectedItem() {
    int order = super.getSelectedIndex();
    if (order == -1) {
      return null;
    }

    Object o = new Integer(order);
    return o;
  }

  //实际值转换到显示值
  //实际值有可能为空,如，新增加一条记录时
  public void setSelectedItem(Object anObject) {

    int index;
    Class type = anObject.getClass();

    if (type == Integer.class || type == Short.class || type == Long.class) {
      index = Integer.parseInt(anObject.toString());
      dataModel.setSelectedItem(super.getItemAt(index));
    }
    else if (type == String.class) {
      int order = super.getSelectedIndex();
      if (order == -1) {
        return;
      }
      dataModel.setSelectedItem(anObject);
    }
  }

}
