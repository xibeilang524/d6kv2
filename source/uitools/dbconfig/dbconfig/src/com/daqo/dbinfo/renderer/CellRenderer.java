package com.daqo.dbinfo.renderer;

import javax.swing.JLabel;
import javax.swing.table.*;
import javax.swing.JTable;
import java.util.Vector;

import com.daqo.dbinfo.*;
import java.awt.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */
//cell的显示
public class CellRenderer
    extends JLabel
    implements TableCellRenderer {

  private int longwidth = 0;
  private int col = -1; //列号
  private int inputmode = -1; //输入模式
  private Vector vRealContent = new Vector(0); //实际内容
  private Vector vShowContent = new Vector(0); //显示内容

  //实际内容和显示内容一一对应
  public CellRenderer() {

  }

  public void setCol(int col) {
    this.col = col;
  }

  public int getCol() {
    return this.col;
  }

  public void setInputMode(int inputmode) {
    this.inputmode = inputmode;
  }

  public int getInputMode() {
    return this.inputmode;
  }

  public void setRealContent(Vector vRealContent) {
    this.vRealContent.add(vRealContent.clone());
  }

  public void setShowContent(Vector vShowContent) {
    this.vShowContent.add(vShowContent);
  }


  public Component getTableCellRendererComponent(
      JTable table, Object value,
      boolean isSelected, boolean hasFocus,
      int row, int column) {
    String celltxt = "";
    if(isSelected) {

    }
    if (inputmode == ConstantDef.IN_AUTO_INTABLENAME) { //自动显示表名
      Object objcellTxt = ((Vector)vShowContent.elementAt(0)).elementAt(0) ;
      celltxt = objcellTxt.toString().trim();
      setText(celltxt);
      Object oo = table.getValueAt(row, column);
      if (oo == null) {
        table.setValueAt(celltxt, row, column);
      }
    }
    else {
      Object oo = table.getValueAt(row, column);
      if (oo != null) {
        String cellvalue = table.getValueAt(row, column).toString().trim();
        int size1 = ((Vector)vRealContent.elementAt(0)) .size();
        int j;
        for (j = 0; j < size1; j++) {
          if (((Vector)vRealContent.elementAt(0)).elementAt(j).toString().trim().equalsIgnoreCase(
              cellvalue)) { //找到了表格内容
            celltxt = ((Vector)vShowContent.elementAt(0)).elementAt(j).toString().trim(); //显示值
            setText(celltxt);
            break;
          } //if
        } //for j
        if (j >= size1) { //没有找到相应的显示内容,显示原内容
          celltxt = cellvalue;
          setText(celltxt);
        }
      } //if(oo != null)
      else {
        setText(null);
      }
    } //if自动显示表名
    Font ft = this.getFont();
    FontMetrics fm = this.getFontMetrics(ft);
    int strwid = fm.stringWidth(celltxt);
    if (strwid > this.longwidth) {
      this.longwidth = strwid;
    }
    if (longwidth > 0) { //没有记录时，增加的第一条记录会使字段不可见
      JTableHeader jth = table.getTableHeader();
      Font headft = jth.getFont();
      FontMetrics headfm = jth.getFontMetrics(ft);
      String headtxt = table.getColumnName(column);
      int headwid = headfm.stringWidth(headtxt) + 20;
      if (longwidth + 5 > headwid) {
        int exwindth = 0;
        if(inputmode == ConstantDef.IN_BYFILE_STRING  ||
            inputmode == ConstantDef.IN_BYFILE_INDEX ||
            inputmode == ConstantDef.IN_BYTABLE_SELECTEDSTRING ||
            inputmode == ConstantDef.IN_BYTABLE_SPECIFIED_FIELD ||
            inputmode == ConstantDef.IN_FIELDNAME ||
            inputmode == ConstantDef.IN_KGBH_FIELD)
          exwindth = 20;
        table.getColumnModel().getColumn(column).setPreferredWidth(longwidth +
            exwindth + 5);
      }
    }
    return this;  //by hx
  }
}
