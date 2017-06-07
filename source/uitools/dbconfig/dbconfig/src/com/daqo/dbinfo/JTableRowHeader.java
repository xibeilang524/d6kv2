package com.daqo.dbinfo;

import javax.swing.JTable;
import javax.swing.table.TableModel;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class JTableRowHeader
    extends JTable {

  public JTableRowHeader(TableModel dm) {
    super(dm);
  }

  public Object getValueAt(int row,
                           int column) {
    if (row >= this.getRowCount()) {
      return null;
    }
    else {
      return super.getValueAt(row, column);
    }
  }
}
