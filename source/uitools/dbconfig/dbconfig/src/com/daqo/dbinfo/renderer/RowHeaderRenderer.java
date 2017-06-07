package com.daqo.dbinfo.renderer;

import javax.swing.JLabel;
import javax.swing.table.TableCellRenderer;
import javax.swing.JTable;
import java.awt.Component;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.JTableHeader;
import javax.swing.UIManager;
import java.awt.Font;
import java.awt.Color;
import java.awt.FontMetrics;
import java.awt.Dimension;

import com.daqo.dbinfo.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class RowHeaderRenderer
    extends JLabel
    implements TableCellRenderer {

  public int longwidth = 0;
  private int rowheadcolnums = 0;
  public RowHeaderRenderer() {
    setBackground(new Color(ConstantDef.CRED ,ConstantDef.CGREEN ,ConstantDef.CBLUE )) ;
  }


  JTable reftable;

  public RowHeaderRenderer(JTable reftable) {
    this.reftable = reftable;
    setBackground(new Color(ConstantDef.CRED ,ConstantDef.CGREEN ,ConstantDef.CBLUE )) ;
  }

  public void setRowHeadColumns(int cols) {
    this.rowheadcolnums = cols;
  }

  public Component getTableCellRendererComponent(JTable table,
                                                 Object obj,
                                                 boolean isSelected,
                                                 boolean hasFocus,
                                                 int row, int col) {
    // if(row == 0)//动态刷新开放后，整个的表刷新有问题。特别 是某字段批量输入时，对话框显示不全。
    //   {
    //     this.longwidth  = 0;
    //   }
    int rowcount = reftable.getRowCount();
    ( (DefaultTableModel) table.getModel()).setRowCount(rowcount);
    JTableHeader header = reftable.getTableHeader();
    this.setOpaque(true);
    /*设为TableHeader的边框类型*/
   // setBorder(UIManager.getBorder("TableHeader.cellBorder"));   //hx
    setHorizontalAlignment(CENTER);
    /*设置背景色为TableHeader的背景色*/
    setBackground(header.getBackground());
    Font font = header.getFont();
    MyTableModel mytable = (MyTableModel) reftable.getModel();
  //  if (col == 0) {
      int rowflag = mytable.managemodel.getRowFlag(row);
      if(rowflag == ConstantDef.DB_CODE_ERR )
        {
          setForeground(Color.yellow );
          setBackground(Color.lightGray);
        }
       else if( rowflag == ConstantDef.DB_CHANGED )
        {
          setForeground(Color.blue  );
          setBackground(Color.lightGray);
        }
      else if(rowflag == ConstantDef.DB_INSERT )
        {
          setForeground(Color.red );
          setBackground(Color.lightGray);
        }
      else if (isSelect(row)) /* 当选取单元格时,在row header上设置成选取颜色 */ {
//        setBackground(java.awt.SystemColor.desktop);
//        setForeground(Color.white );
          setForeground(Color.blue );
          setBackground(Color.lightGray);
          setFont(font);
      }
      else {
        setForeground(header.getForeground());
        setFont(font);
      }
      setText(String.valueOf(row + 1));
 //   }
  //  else {
      if(col > 0)
        {
      String listtxt = mytable.getListField(row);
      setText(listtxt);
      if (rowheadcolnums > 1) {
        Font ft = this.getFont();
        FontMetrics fm = this.getFontMetrics(ft);
        int strwid = fm.stringWidth(listtxt);
        if (strwid > this.longwidth) {
          this.longwidth = strwid;
        }
        table.setPreferredScrollableViewportSize(new Dimension(longwidth + 60,
            0));
        table.getColumnModel().getColumn(1).setPreferredWidth(longwidth + 10);
      }
    }
    return this;
  }

  private boolean isSelect(int row) {
    int[] sel = reftable.getSelectedRows();
    for (int i = 0; i < sel.length; i++) {
      if (sel[i] == row) {
        return true;
      }
    }
    return false;
  }

}
