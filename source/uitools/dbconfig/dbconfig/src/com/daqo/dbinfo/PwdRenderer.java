package com.daqo.dbinfo;

import javax.swing.JLabel;
import javax.swing.table.*;
import javax.swing.JTable;
import java.awt.Component;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */
//密码显示
public class PwdRenderer
    extends JLabel
    implements TableCellRenderer {

  public PwdRenderer() {
  }

  public Component getTableCellRendererComponent(
      JTable table, Object value,
      boolean isSelected, boolean hasFocus,
      int row, int column) {
      setText("********************");
    //  setText(value.toString());
    return this;
  }
}
