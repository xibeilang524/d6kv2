package com.daqo.dbinfo;

import javax.swing.table.JTableHeader;
import javax.swing.table.TableColumnModel;
import java.awt.Font;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import javax.swing.JOptionPane;
import java.awt.Color;
import javax.swing.table.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.ImageIcon;


/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class MyJTableHeader extends JTableHeader {

  MyJTable jtable;
  Dlllink pdll = Dlllink.getInstance();
  int column;
  MyTableModel myTableModelsort;
  ImageIcon imageinsertCell,imagefindCell;
  public MyJTableHeader() {
    setBackground(new Color(ConstantDef.CRED ,ConstantDef.CGREEN ,ConstantDef.CBLUE )) ;
  }

  public MyJTableHeader(TableColumnModel cm) {
    super(cm);
    setBackground(new Color(ConstantDef.CRED ,ConstantDef.CGREEN ,ConstantDef.CBLUE )) ;

  }

public void init()
    {
      imageinsertCell = new ImageIcon(com.daqo.dbinfo.MyJTable.class.getResource(
          "icon/insertall.png"));
      imagefindCell = new ImageIcon(com.daqo.dbinfo.MyJTable.class.getResource(
          "icon/mhgl.png"));
     jtable = (MyJTable)this.getTable() ;
      setReorderingAllowed(false); //
      setUpdateTableInRealTime(true);
      setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, jtable.getFontSize() ));
      addMouseListener(new ListMouseListener());
      setBackground(new Color(ConstantDef.CRED ,ConstantDef.CGREEN ,ConstantDef.CBLUE )) ;//hx
    }


  class ListMouseListener
      extends MouseAdapter {
    public void mouseClicked(MouseEvent e) {
      listMouse_mouseClicked(e);
    }
  }

  public void listMouse_mouseClicked(MouseEvent e) {
    ConstantDef.setLoginTime();
    TableColumnModel columnModel = jtable.getColumnModel();
    int viewColumn = columnModel.getColumnIndexAtX(e.getX());
    column = jtable.convertColumnIndexToModel(viewColumn);
    myTableModelsort = (MyTableModel) jtable.getModel();
    if (e.getClickCount() == 1 && column != -1 &&
        ( (e.getModifiers() & e.BUTTON1_MASK) == e.BUTTON1_MASK)) {
      myTableModelsort.sortchangedf = false;
      myTableModelsort.sortByColumn(column);
      if (myTableModelsort.sortchangedf) { //有顺序调换
        MyJTable mjtabletmp = jtable;
        MyTableModel mjtb = myTableModelsort;
        if (jtable.getTabCount() > 1) {
          for (int i = 0; i < jtable.getTabCount(); i++) {
            if (i == 0) {
              mjtabletmp = jtable.getFirstJTable();
              mjtb = (MyTableModel) jtable.getFirstJTable().getModel();
            }
            else {
              mjtabletmp = jtable.getFirstJTable().getJTable(i - 1);
              mjtb = (MyTableModel) mjtabletmp.getModel();
            }
            mjtb.changeOrder(myTableModelsort.indexes);
            mjtabletmp.updateRow();
          }
        }
        else {
          mjtb.changeOrder(myTableModelsort.indexes);
          mjtabletmp.updateRow();
        }
        mjtb.managemodel.changeOrder(myTableModelsort.indexes);

      }
    }
    else if (e.getClickCount() == 1 && column != -1 &&
             ( (e.getModifiers() & e.BUTTON3_MASK) == e.BUTTON3_MASK)) {
      /*
      //添加模糊过滤功能
      JPopupMenu popmenu = new JPopupMenu("Table Operater");
      JMenuItem jMenuInsertCell = new JMenuItem();
      JMenuItem jMenuFindCell = new JMenuItem();
      jMenuInsertCell.setText("输入");
       jMenuInsertCell.setIcon(imageinsertCell);
      jMenuFindCell.setText("过滤");
        jMenuFindCell.setIcon(imagefindCell);
      popmenu.add(jMenuInsertCell);
      popmenu.add(jMenuFindCell);
      popmenu.show(e.getComponent(), e.getX(), e.getY());
      jMenuInsertCell.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          batchUpdate(myTableModelsort, column);
        }

      });
      jMenuFindCell.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          Fuzzyfind(myTableModelsort, column);
        }

      });
*/
      batchUpdate(myTableModelsort, column);
    }
  }
/*
  //批量修改域值
//当该域有CellRenderer 时，输入的值直接写入库，没有经过装换
  public void Fuzzyfind(MyTableModel tableModel, int column) {
    String fielddes = tableModel.getColumnName(column);
    int intFind;
    String str;
    Object obj;
    String s = (String) JOptionPane.showInputDialog(new FrameDB(),
        "域 \"" + fielddes + "\" 模糊过滤的条件:",
        "模糊过滤",
        JOptionPane.PLAIN_MESSAGE,
        null,
        null,
        "");
    if (s != null) {
      int row = tableModel.getRowCount();
      for (int rowi = 0; rowi < row; rowi++) {
        obj = table.getValueAt(rowi, column);
        if (obj != null) {
          str = table.getValueAt(rowi,
                                 column).toString().trim();
          intFind = str.indexOf(s);
          if (intFind == -1 || str.equals("")) {

            if(jtable.getTabCount() > 1){
              jtable.getFirstJTable().setRowHeight(rowi,1);
              jtable.getFirstJTable().rowHeaderTable.setRowHeight(rowi,1);
              for(int tab=1;tab<jtable.getTabCount();tab++){
                jtable.getFirstJTable().getJTable(tab-1).setRowHeight(rowi,1);
                jtable.getFirstJTable().getJTable(tab-1).rowHeaderTable.setRowHeight(rowi,1);
              }
            }
            else
            {
              jtable.setRowHeight(rowi, 1);
              jtable.rowHeaderTable.setRowHeight(rowi, 1);
            }
          }

        }
      }
    }

  }



*/
  //批量修改域值
  //当该域有CellRenderer 时，输入的值直接写入库，没有经过装换
  public void batchUpdate(MyTableModel tableModel, int column) {
    String tablename = tableModel.getTableName();
 if (PreProcessor.TableLock) {  //表互斥修改  ---洪祥，判断表是否在修改
    byte[][] buf;
    int[] fieldcount = new int[1];
    String wh;
    wh = "tablename = '" + tablename + "'";
    buf = pdll.queryAnyTable(ConstantDef.MODELDB, "table_status",
                             "tablename", wh, "", fieldcount, false);
    int len = buf.length;
     if (tableModel.getSaveFlag() == false) {
       if (len > 0) {
     //    JOptionPane.showMessageDialog(null, "此表正在编辑中", "提示",
     //                                  JOptionPane.WARNING_MESSAGE);
         return;
       }
     }
  //表互斥修改
 }
    String fielddes = tableModel.getColumnName(column);
    String s = (String) JOptionPane.showInputDialog( new FrameDB(),
        "域\"" + fielddes + "\"新的值为:",
        "输入域值",
        JOptionPane.PLAIN_MESSAGE,
        null,
        null,
        "");
    if (s != null) {
      int row = tableModel.getRowCount();
      for (int rowi = 0; rowi < row; rowi++) {
        tableModel.setValueAt(s, rowi, column);
      }
    }
  }


}
