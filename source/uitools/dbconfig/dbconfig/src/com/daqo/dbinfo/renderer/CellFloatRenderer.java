package com.daqo.dbinfo.renderer;

import javax.swing.JLabel;
import javax.swing.table.TableCellRenderer;
import java.util.Vector;
import java.awt.Component;
import javax.swing.JTable;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Color;
import javax.swing.table.DefaultTableCellRenderer;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class CellFloatRenderer
    extends JLabel
    implements TableCellRenderer {

  private int longwidth = 0;
  public CellFloatRenderer() {
  }
  public static final DefaultTableCellRenderer DEFAULT_RENDERER =
    new DefaultTableCellRenderer();   //by hx

  public Component getTableCellRendererComponent(
      JTable table, Object value,
      boolean isSelected, boolean hasFocus,
      int row, int column) {
    //---by hx
    Component renderer =
        DEFAULT_RENDERER.getTableCellRendererComponent(table, value,
        isSelected, hasFocus, row, column);
    Color foreground, background;
    if (isSelected) {
      background = Color.pink;

    }else
      background=new Color(199, 237, 204);
      renderer.setBackground(background);
      //----
    String celltxt = "";
    Object oo = table.getValueAt(row, column);
    if (oo != null) {
      celltxt = oo.toString();
      setText(celltxt);
    }
    else {
      setText(null);
    }

    return renderer;  ///by hx
  }
}
