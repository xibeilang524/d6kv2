package com.daqo.dbinfo;

import javax.swing.table.*;
import java.awt.event.*;
import java.util.Vector;

import javax.swing.*;
import com.daqo.dbinfo.renderer.*;
import java.awt.*;
import java.awt.datatransfer.*;
import java.util.*;

import com.daqo.dbinfo.table.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.border.Border;
import javax.swing.border.EtchedBorder;
import java.awt.event.AdjustmentListener;
import java.util.ArrayList;
import javax.swing.event.CellEditorListener;
import javax.swing.event.ChangeEvent;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class MyJTable
    extends JTable {
  JTable rowHeaderTable;
  JScrollPane jscrolltab;
  JScrollBar jsbar;
  MyTableModel myTableModel;
  TableSetInfo tablesetinfo;
  PDB_FieldTable pdb_fieldtable;
  Dlllink pdll = Dlllink.getInstance();
  String m_db, m_realtable, m_where;
  DefaultMutableTreeNode m_node;
  Vector vCellRenderer = new Vector(0); //放CellRenderer对象
  int colSelect;
  boolean select;
  int fontsize;
  public int curTabno; //当前区号   //修改by洪祥 20120817
  private int m_tabnoCount; //总的分区数
  private Vector vMyTable = new Vector(0); //多分区时，存储第一个之外的JTable
  private int colcount; //多分区时，保存列的总数
  private int[] keycol = new int[0]; //关键列号
  private MyJTable firstJTable = null; //多分区时，连接到第一个分区
  private String rowstring,value;
  private Clipboard system;
  private StringSelection stsel;
  ImageIcon imageCopyCell,imagepasteCell,imagefind;
//----hx
  private int intRowStart ;// TableModelConest.Init_Row;
  private int intColStart ;// TableModelConest.Init_Col;
  //private TableDialogFind findDialog =null;
  public void init() {

    setFontSize(fontsize);
    setColumnSelectionAllowed(true);
    mouseMotionEvent();
    setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
    //setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
    this.setCellSelectionEnabled(true);
    this.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);

    setGridColor(Color.black);
    setAutoscrolls(true);
    setFont(new java.awt.Font(ConstantDef.FONTNAME, Font.PLAIN , fontsize)); //("Dialog", 0, 14));
    myTableModel = (MyTableModel)this.getModel();
    m_db = myTableModel.getDBName();

    Vector disEnabledInput = new Vector(0); //不可输入域
    setColumnEditor(disEnabledInput);
    setUpIntegerEditor();

    myTableModel.setDisEnabledInputFieldName(disEnabledInput);

    //Column的颜色
    DefaultTableCellRenderer dcr = new DefaultTableCellRenderer();
    dcr.setForeground(Color.blue);
    dcr.setBackground(this.getTableHeader().getBackground() ) ;
    Font font = ((MyJTableHeader)getTableHeader()).getFont()  ;
    dcr.setFont(font) ;
    Border border = getTableHeader().getBorder() ;
    dcr.setBorder(border) ;
   // dcr.setBorder(UIManager.getBorder("TableHeader.cellBorder"));
    // dcr.setAlignmentX(DefaultTableCellRenderer.CENTER_ALIGNMENT);

    /*  Border blackline, raisedetched, loweredetched,
             raisedbevel, loweredbevel, empty;
      blackline = BorderFactory.createLineBorder(Color.black);
      raisedetched = BorderFactory.createEtchedBorder(EtchedBorder.RAISED);
      loweredetched = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);
      raisedbevel = BorderFactory.createRaisedBevelBorder();
      loweredbevel = BorderFactory.createLoweredBevelBorder();
      empty = BorderFactory.createEmptyBorder();
       dcr.setBorder(raisedbevel)  ;*/

    String[] keyname = myTableModel.getkeys();
    int[] tmpcol = new int[keyname.length];
    int tmocount = 0;
    for (int i = 0; i < myTableModel.getKeyCount(); i++) {
      for (int j = 0; j < myTableModel.getColumnCount(); j++) {
        if (PreProcessor.ALIAS) {
          String strTmp=ApplicationDB.frameDB.GetFieldNameFromAlias(myTableModel.getTableName(),myTableModel.getColumnName(j));
          if(strTmp.length()==0)
            strTmp=myTableModel.getColumnName(j);
          if (keyname[i].equalsIgnoreCase(strTmp)) {
            TableColumn tc = this.getColumnModel().getColumn(j);
            tc.setHeaderRenderer(dcr);
            tmpcol[tmocount++] = j;
            break;
          }
        }
        else {
          if (keyname[i].equalsIgnoreCase(myTableModel.getColumnName(j))) {
            TableColumn tc = this.getColumnModel().getColumn(j);
            tc.setHeaderRenderer(dcr);
            tmpcol[tmocount++] = j;
            break;
          }
        }
      }
    }
    keycol = new int[tmocount];
    for (int i = 0; i < keycol.length; i++) {
      keycol[i] = tmpcol[i];
    }

    //开始Row Header
    boolean blistfield = myTableModel.getListFieldFlag();
    int rowheadcolnums = 2;
    if (!blistfield) {
      rowheadcolnums = 1;

    }
    DefaultTableModel rowheadmodel = new DefaultTableModel(getRowCount(),
        rowheadcolnums);
    this.rowHeaderTable = new JTableRowHeader(rowheadmodel);
    rowheadmodel.setColumnCount(rowheadcolnums);
    if (rowheadcolnums == 1) {
      rowheadmodel.setColumnIdentifiers(new Object[] {""});
    }
    else {
      rowheadmodel.setColumnIdentifiers(new Object[] {"", ""});
    }
    rowHeaderTable.setEnabled(false);
    rowHeaderTable.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
    rowHeaderTable.getColumnModel().getColumn(0).setPreferredWidth(50);
    if (rowheadcolnums > 1) {
      rowHeaderTable.getColumnModel().getColumn(1).setPreferredWidth(10);
    }

    jscrolltab.setRowHeaderView(rowHeaderTable);
    //改变JScrollPane的背景色设置 hx
    jscrolltab.getViewport().setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                        ConstantDef.CBLUE));
    rowHeaderTable.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                        ConstantDef.CBLUE));

    int headwid;
    headwid = rowHeaderTable.getColumnModel().getColumn(0).getPreferredWidth();
    if (rowheadcolnums > 1) {
      headwid += rowHeaderTable.getColumnModel().getColumn(1).getPreferredWidth();
    }
    rowHeaderTable.setPreferredScrollableViewportSize(new Dimension
        (headwid, 0));
    rowHeaderTable.setRowHeight(getRowHeight());
    RowHeaderRenderer headRender = new RowHeaderRenderer(this);
    headRender.setRowHeadColumns(rowheadcolnums);
    rowHeaderTable.setDefaultRenderer(rowHeaderTable.getColumnClass(0),
                                      headRender);

    addMouseListener(new ListTreeRowHeaderRender(rowHeaderTable) {});
    setRowHeaderTable(rowHeaderTable);
    initColumnSizes();
  }

  public MyJTable(TableModel dm) {
    super(dm);
    intRowStart = 0;
    intColStart = 0;
    fontsize = 14;  ///by hx
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  public void setCurTabno(int curTabno) {
    this.curTabno = curTabno;
  }

  public int getCurTabno() {
    return curTabno;
  }

  public String getTableType() {
    return myTableModel.getTableType();
  }

  public int getTabCount() {
    return m_tabnoCount;
  }

  public void setTableSetInfo(TableSetInfo tablesetinfo) {
    this.tablesetinfo = tablesetinfo;
    m_tabnoCount = tablesetinfo.getTabCount();
  }

  public TableSetInfo getTableSetInfo() {
    return this.tablesetinfo;
  }

  public void setFontSize(int fontsize) {
    this.fontsize = fontsize;
  }

  public int getFontSize() {
    return this.fontsize;
  }

  public void setScrollTab(JScrollPane jscrolltab) {
    this.jscrolltab = jscrolltab;
    jsbar = this.jscrolltab.getVerticalScrollBar();
    jsbar.addAdjustmentListener(new JScrollBarAdjustmentListener());
  }

  public MyJTable getFirstJTable() {
    return this.firstJTable;
  }

  public void setRowHeaderTable(JTable rowHeaderTable) {
    this.rowHeaderTable = rowHeaderTable;
  }

  public void setPDBField(PDB_FieldTable pdb_fieldtable) {
    this.pdb_fieldtable = pdb_fieldtable;
  }

  public void setRealTable(String realtable) {
    this.m_realtable = realtable;
  }

  public void setNode(DefaultMutableTreeNode node) {
    this.m_node = node;
  }

  public void setWhere(String where) {
    this.m_where = where;
  }

  public void setFirstJTable(MyJTable firstJTable) {
    this.firstJTable = firstJTable;
  }

  public void setColCount(int colcount) {
    this.colcount = colcount;
  }

  public int getColCount() {
    return this.colcount;
  }
//----hx
  public void setColIntStart(int intS) {
    intColStart = intS;
  }

  public int getColIntStart() {
    return intColStart;
  }

  public void setRowIntStart(int intS) {
    intRowStart = intS;
  }

  public int getRowIntStart() {
    return intRowStart;
  }


//----hx
  public void addJTable(JTable jtable) {
    vMyTable.add(jtable); //没有包括第一个
  }

  public MyJTable getJTable(int no) {
    return (MyJTable) (vMyTable.elementAt(no));
  }

  public void updateRow() {
    if (rowHeaderTable != null) {
      ( (DefaultTableModel) rowHeaderTable.getModel()).setRowCount(this.
          getRowCount());
      rowHeaderTable.updateUI();
    }

  }

  public TableCellRenderer getCellRenderer(int row, int viewColumn) {
    Vector fieldInputMode = (Vector) (tablesetinfo.fieldeInputMode.elementAt(
        curTabno));
    Vector thiscol = (Vector) (tablesetinfo.thiscol.elementAt(curTabno));
    int column = convertColumnIndexToModel(viewColumn);
    int size = fieldInputMode.size();
    for (int k = 0; k < size; k++) {
      if (column == Integer.parseInt(thiscol.elementAt(k).toString())) {
        int evalutemode = Integer.parseInt(fieldInputMode.elementAt(k).toString());
        if (evalutemode == ConstantDef.IN_PASSWORD) {
          return new PwdRenderer();
        }
        else if (evalutemode == ConstantDef.IN_FLOAT) {
          CellFloatRenderer cellfolat = new CellFloatRenderer();
          cellfolat.setFont(new Font(ConstantDef.FONTNAME,Font.PLAIN ,fontsize)) ;
          return  cellfolat;
        }
        else {
          int cellsize = vCellRenderer.size();
          for (int m = 0; m < cellsize; m++) {
            CellRenderer cellrend = (CellRenderer) vCellRenderer.elementAt(m);
            if (cellrend.getCol() == column) {
              return cellrend;
            }
          }
        }
      } //if column
    } //for k

    return super.getCellRenderer(row, column);
  }

  private void setUpIntegerEditor() {
    //Set up the editor for the integer cells.
    final WholeNumberField integerField = new WholeNumberField(0, 5);
    integerField.setHorizontalAlignment(WholeNumberField.RIGHT);

    DefaultCellEditor integerEditor =
        new DefaultCellEditor(integerField) {
      //Override DefaultCellEditor's getCellEditorValue method
      //to return an Integer, not a String:
      public Object getCellEditorValue() {
        return new Integer(integerField.getValue());
      }
    };
    DefaultCellEditor shortIntEditor =
        new DefaultCellEditor(integerField) {
      //Override DefaultCellEditor's getCellEditorValue method
      //to return an Integer, not a String:
      public Object getCellEditorValue() {
        return new Short( (short) integerField.getValue());
      }
    };

    DefaultCellEditor longEditor =
        new DefaultCellEditor(integerField) {
      //Override DefaultCellEditor's getCellEditorValue method
      //to return an Integer, not a String:
      public Object getCellEditorValue() {
        return new Long(integerField.getValue());
      }
    };

    setDefaultEditor(Integer.class, integerEditor);
    setDefaultEditor(Short.class, shortIntEditor);
    setDefaultEditor(Long.class, longEditor);
  }

  //初始化列的宽度
  public void initColumnSizes() {
    MyTableModel model = (MyTableModel)this.getModel();
    TableColumn column = null;
    Component comp = null;
    int headerWidth = 0;
    int cellWidth = 0;

    int columns = model.getColumnCount(); //列数
    TableCellRenderer headerRenderer = getTableHeader().
        getDefaultRenderer();
    MyJTableHeader head = (MyJTableHeader) getTableHeader();
    // boolean berr = false;

    Font font = getFont();
    FontMetrics fm = getFontMetrics(font);
    Font headfont = head.getFont();
    FontMetrics headfm = head.getFontMetrics(headfont);

    for (int i = 0; i < columns; i++) {
      column = getColumnModel().getColumn(i);
      try {
        String value = model.getColumnName(i);
        //计算列宽
        int d = headfm.stringWidth(value);
        int e = 20;
        headerWidth = d + e;
      }
      catch (NullPointerException e) {
        System.err.println("Null pointer exception!");
        System.err.println("  getHeaderRenderer returns null in 1.3.");
        System.err.println("  The replacement is getDefaultRenderer.");
      }

      String maxlongcol = model.getLongValues().elementAt(i).toString();
      cellWidth = fm.stringWidth(maxlongcol) + 20;
      int maxCellWidth = Math.max(headerWidth, cellWidth);
      column.setPreferredWidth(maxCellWidth);
    }
  }

//关键字段列变色
  public Component prepareRenderer(TableCellRenderer renderer,
                                   int row,
                                   int column) {
    Component component = null;
    try {
       component = super.prepareRenderer(renderer, row, column);
    }catch(ClassCastException ex){

    }

    /* boolean bkeycol = false;
     for(int i = 0 ;i <keycol.length;i++)
     {
       if(column == keycol[i])
       {
         bkeycol =true;
         break;
       }
     }
     Color c = component.getBackground() ;
     int r = c.getRed() ;
     int g =  c.getGreen() ;
     int b = c.getBlue() ;
     if(bkeycol)
       {
        component.setBackground(new Color(200, 100, 30)) ;
       }
       else
       {
         component.setBackground(new Color(255,255,255) ) ;
       }
     */
    return component;
  }

  //处理鼠标drag事件
  public void mouseMotionEvent() {
    addMouseMotionListener(new MouseMotionListener() {
      boolean bpressleftmouse = false;
      boolean bstartdrag = false;
      public void mouseMoved(MouseEvent e) {
        ConstantDef.setLoginTime();
        if (ConstantDef.getUserInstance().getEditPara() == 0) {
          return;
        }

        int editRow = getSelectedRow();
        int editCol = getSelectedColumn();
        if (editRow == -1 || editCol == -1) {
          return;
        }
        MyTableModel mytablemodel = (MyTableModel) getModel();
        int inputmode = mytablemodel.getInputMode(editCol);
        if (inputmode != ConstantDef.IN_BYKEYBOARD) {
          return; //只有键盘输入方式的域能有这样的方式
        }
        Rectangle rect = getCellRect(editRow, editCol, true);
        int x = e.getX();
        int y = e.getY();
        int rightx = rect.x + rect.width;
        int righty = rect.y + rect.height;
        //if(rect.contains(x,y) )
        if (x >= (rightx - 3) && x <= rightx && y >= (righty - 3) &&
            y <= righty) {
          Cursor cursor = new Cursor(Cursor.CROSSHAIR_CURSOR);
          setCursor(cursor);
          bstartdrag = true;
          setColumnSelectionAllowed(true);
        }
        else {
          Cursor cursor = new Cursor(Cursor.DEFAULT_CURSOR);
          setCursor(cursor);
          bstartdrag = false;
          if (bpressleftmouse) {
            bpressleftmouse = false;
            int[] row = getSelectedRows();
            if (row.length <= 1) {
              return;
            }
            MyTableModel tablemodel = (MyTableModel) getModel();
            Object srcObj = tablemodel.getValueAt(row[0], editCol); //源cell
            if (srcObj == null) {
              return;
            }
            Class type = tablemodel.getColumnClass(editCol); //列的类型
            int col1 = row[0];
            int col2 = col1;
            for (int i = 1; i < row.length; i++) {

              if (row[i] > col2) {
                col2 = row[i];
              }
              if (row[i] < col1) {
                col1 = row[i];

              }
              if (type == java.lang.Integer.class || type == java.lang.Short.class) {
                if (e.isControlDown()) {
                 tablemodel.setValueAt(srcObj, row[i], editCol);
                }
                else{
                  Object increaserow = new Integer(Integer.parseInt(srcObj.
                      toString()) + row[i] - row[0]);
                  tablemodel.setValueAt(increaserow, row[i], editCol);
                }
              }
              else if (type == java.lang.Long.class) {
                if (e.isControlDown()) {
                  tablemodel.setValueAt(srcObj, row[i], editCol);
                }else{
                  Object increaserow = new Long(Long.parseLong(srcObj.toString()) +
                      row[i] - row[0]);
                  tablemodel.setValueAt(increaserow, row[i], editCol);
                }
              }
              else {
                tablemodel.setValueAt(srcObj, row[i], editCol);
              }
            } //for i
            //   listtable.setRowSelectionInterval(col1,col2) ;
          } //f(bpressleftmouse)
          setColumnSelectionAllowed(true);
          setRowSelectionAllowed(true);
        } //f(rect.contains(x,y) )
      }

      public void mouseDragged(MouseEvent e) {
        if (bstartdrag) {
          bpressleftmouse = true;
        }
      }
    }
    );

    addMouseListener(new MouseListener() {
      public void mouseClicked(MouseEvent e) {

        User user = ConstantDef.getUserInstance();
        MyTableModel mytablemodel = (MyTableModel) getModel();
        String tablename= mytablemodel.getTableName();
        if (PreProcessor.TableLock) {       //表互斥修改  ---洪祥，判断表是否在修改
        byte[][] buf;
        int[] fieldcount = new int[1];
        String wh;
        wh = "tablename = '" + tablename + "'";
        buf = pdll.queryAnyTable(ConstantDef.MODELDB, "table_status",
                                 "tablename", wh, "", fieldcount, false);
        int len = buf.length;
        if (mytablemodel.getSaveFlag() == false) {
          if (len > 0) {
            return;
          }
        }
        //表互斥修改
        }
        if (user.getDesc().equalsIgnoreCase("调度员")) {
          if (tablename.equalsIgnoreCase("SCD开关参数表") ||
              tablename.equalsIgnoreCase("SCD主变压器参数表") ||
              tablename.equalsIgnoreCase("SCD刀闸参数表") ||
              tablename.equalsIgnoreCase("SCD熔断器参数表") ||
              tablename.equalsIgnoreCase("SCD手车参数表") ||
              tablename.equalsIgnoreCase("SCD电容器参数表") ||
              tablename.equalsIgnoreCase("SCD电抗器参数表") ||
              tablename.equalsIgnoreCase("SCD消弧线圈参数表") ||
              tablename.equalsIgnoreCase("SCD互感器参数表") ||
              tablename.equalsIgnoreCase("SCD线路参数表") ||
              tablename.equalsIgnoreCase("SCD母线参数表") ||
              tablename.equalsIgnoreCase("SCD发电机参数表") ||
              tablename.equalsIgnoreCase("SCD负荷参数表") ||
              tablename.equalsIgnoreCase("SCD保护装置参数表") ||
              tablename.equalsIgnoreCase("SCD配电变压器参数表")) {
          }else{
            return;
          }
        }
        if (e.getID() == e.MOUSE_CLICKED) {
          if ( (e.getModifiers() & e.BUTTON3_MASK) == e.BUTTON3_MASK) {
            JPopupMenu popmenu = new JPopupMenu("Table Operater");
            JMenuItem jMenuCopyCell = new JMenuItem();
            JMenuItem jMenuPasteCell = new JMenuItem();
            //适应广州测试   -hx
            JMenuItem jMenuFindCell = new JMenuItem();
            jMenuCopyCell.setText("复制");
            jMenuCopyCell.setIcon(imageCopyCell);
            jMenuPasteCell.setText("粘贴");
            jMenuPasteCell.setIcon(imagepasteCell);
            jMenuFindCell.setText("查找");
            jMenuFindCell.setIcon(imagefind);
            popmenu.add(jMenuCopyCell);
            popmenu.add(jMenuPasteCell);
            popmenu.add(jMenuFindCell);
            // KeyStroke copy = KeyStroke.getKeyStroke(KeyEvent.VK_C,ActionEvent.CTRL_MASK,false);
            //  KeyStroke paste = KeyStroke.getKeyStroke(KeyEvent.VK_V,ActionEvent.CTRL_MASK,false);
            popmenu.show(e.getComponent(), e.getX(), e.getY());
            jMenuCopyCell.addActionListener(new ActionListener() {
              public void actionPerformed(ActionEvent e) {
                if (e.getActionCommand().compareTo("复制") == 0) {
                  StringBuffer sbf = new StringBuffer();
                  // Check to ensure we have selected only a contiguous block of
                  // cells
                  int numcols = getSelectedColumnCount();
                  int numrows = getSelectedRowCount();
                  int[] rowsselected = getSelectedRows();
                  int[] colsselected = getSelectedColumns();
                  if(numcols!=0){
                    if (! ( (numrows - 1 ==
                             rowsselected[rowsselected.length - 1] -
                             rowsselected[0] &&
                             numrows == rowsselected.length) &&
                           (numcols - 1 ==
                            colsselected[colsselected.length - 1] -
                            colsselected[0] &&
                            numcols == colsselected.length))) {
                      JOptionPane.showMessageDialog(null,
                          "不能这样复制！",
                          "警告",
                          JOptionPane.ERROR_MESSAGE);
                      return;
                    }
                  }else{
                    JOptionPane.showMessageDialog(null,
                                                  "您没有选中要复制的单元格！",
                                                  "警告",
                                                  JOptionPane.ERROR_MESSAGE);
                    return;
                  }
                  for (int i = 0; i < numrows; i++) {
                    for (int j = 0; j < numcols; j++) {
                      sbf.append(getValueAt(rowsselected[i],
                          colsselected[j]));
                      if (j < numcols - 1)
                        sbf.append("\t");
                    }
                    sbf.append("\n");
                  }
                  stsel = new StringSelection(sbf.toString());
                  System.out.println("String is:" + sbf.toString());
                  system = Toolkit.getDefaultToolkit().getSystemClipboard();
                  system.setContents(stsel, stsel);
                }
              }
            });
            jMenuPasteCell.addActionListener(new ActionListener() {
              public void actionPerformed(ActionEvent e) {
                if (e.getActionCommand().compareTo("粘贴") == 0) {
             if (getSelectedRows().length!=0){
                  //System.out.println("Trying to Paste");
                  int startRow = (getSelectedRows())[0];
                  int startCol = (getSelectedColumns())[0];
                    try {
                      system = Toolkit.getDefaultToolkit().getSystemClipboard();
                      String trstring = (String) (system.getContents(system).
                                                  getTransferData(DataFlavor.
                          stringFlavor));
                     //  System.out.println("String is:" + trstring);
                      StringTokenizer st1 = new StringTokenizer(trstring, "\n");
                      for (int i = 0; st1.hasMoreTokens(); i++) {
                        rowstring = st1.nextToken();
                        StringTokenizer st2 = new StringTokenizer(rowstring,
                            "\t");
                        for (int j = 0; st2.hasMoreTokens(); j++) {
                          value = (String) st2.nextToken();
                          if (startRow + i < getRowCount() &&
                              startCol + j < getColumnCount())
                            setValueAt(value, startRow + i,
                                       startCol + j);
                                /*  System.out.println("Putting " + value + "atrow=" +
                             startRow + i + "column=" + startCol +
                                             j);*/
                        }
                      }
                    }
                    catch (Exception ex) {
                      ex.printStackTrace();
                    }
                  }
                }
              }
            });
            //广东测试添加表格的查找功能
            jMenuFindCell.addActionListener(new ActionListener() {
              public void actionPerformed(ActionEvent e) {
                int numcols = getSelectedColumnCount();
                int numrows = getSelectedRowCount();
                int[] rowsselected = getSelectedRows();
                int[] colsselected = getSelectedColumns();
                if (numcols > 0 && numrows > 0) {
                  TableDialogFind findDialog = new TableDialogFind(
                      ApplicationDB.frameDB, "时间选择", true);
                  findDialog.intColFindStart = 0;
                  findDialog.intRowFindStart = 0;
                  findDialog.intColEnd = myTableModel.getColumnCount();
                  findDialog.intRowEnd =  myTableModel.getRowCount();
                  findDialog.intSelectColFindStart = colsselected[0];
                  findDialog.intSelectRowFindStart = rowsselected[0];
                  findDialog.intSelectColEnd = colsselected[0] +
                      getSelectedColumnCount();
                  findDialog.intSelectRowEnd = rowsselected[0] +
                      getSelectedRowCount();
                  findDialog.rows = getSelectedRows();
                  findDialog.cols = getSelectedColumns();
                  findDialog.bool = false;
                  //--------------------------------------------------------------------
                  //     int row = getSelectedRow();
                  if (m_tabnoCount > 1) {
                    if (curTabno != 0) {
                        MyJTable myjtabtmp = firstJTable.getJTable(curTabno-1);
                        findDialog.setTable(myjtabtmp);
                    }
                    else
                      findDialog.setTable(firstJTable);

                  }
                  else {
                    findDialog.setTable(firstJTable);
                  }

                  findDialog.setModal(true);
                  findDialog.setLocationRelativeTo(null);
                  findDialog.show();
                  findDialog.pack();
                }
              }
            });

          } //if
          }
          }
      public void mousePressed(MouseEvent e) {
        //firstJTable.setSelect(getSelectedColumn(),false);
      }

      public void mouseReleased(MouseEvent e) {
        int row = getSelectedRow();
        if (m_tabnoCount > 1 && row > 0) {
          if (curTabno != 0) {
            firstJTable.setRowSelectionInterval(row, row);
          }
          for (int i = 1; i < m_tabnoCount; i++) {
            if (i == curTabno) {
              continue;
            }
            MyJTable myjtabtmp = firstJTable.getJTable(i - 1);
            myjtabtmp.setRowSelectionInterval(row, row);
          }
        }
      }

      public void mouseEntered(MouseEvent e) {
      }

      public void mouseExited(MouseEvent e) {
      }
    });
  }

  public void setColumnEditor(Vector disEnabledInput) {

    MyTableModel mytablemodel = (MyTableModel) getModel();
    int keynum = mytablemodel.getKeyCount(); //关键字段数
    int colnum = mytablemodel.getColumnCount(); //列数

    if (tablesetinfo.tableFieldInfo.buf.length < pdb_fieldtable.fieldnums) { //如果pdb_field中没有关于此表的设置，就全部用缺省的输入、显示方式
      return;
    }

    int recNums = tablesetinfo.tableFieldInfo.buf.length /
        pdb_fieldtable.fieldnums; //记录数
    int i, j;

    for (i = 0; i < colnum; i++) { //ModelTable的域数
      getColumnModel().getColumn(i).sizeWidthToFit();
      String fieldname1 = mytablemodel.getColumnName(i); //得到每一列的名称，找到pdb_field中对该列的设置，后根据相应的设置来显示该列
      if (PreProcessor.ALIAS) {
        String field = ApplicationDB.frameDB.GetFieldNameFromAlias(m_realtable,
            fieldname1);
        if (field.length() > 0)
          fieldname1 = field;
      }

      for (j = 0; j < recNums; j++) {
        String fieldname2 = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                        pdb_fieldtable.
                                        pdb_field_describle_no +
                                        j * pdb_fieldtable.fieldnums]);

        if (fieldname2.equalsIgnoreCase(fieldname1)) {
          break; //在pdb_field中找到了这个域的设置
        }
      } //for j
      if (j < recNums) { //在pdb_field中找到了这个域的设置,判断设置的内容
        TableColumn mycolumn = getColumnModel().getColumn(i);
        int evaluemode = Integer.parseInt(pdll.substr(tablesetinfo.
            tableFieldInfo.buf[
            pdb_fieldtable.pdb_field_evaluemode_no +
            j * pdb_fieldtable.fieldnums])); //赋值方式
        mytablemodel.setColInputMode(i, evaluemode);
        switch (evaluemode) {
          case ConstantDef.IN_BYKEYBOARD: { //键盘输入
            break;
          }
          case ConstantDef.IN_CHECKBOX: { //checkbox
            break;
          }
          case ConstantDef.IN_BYFILE_STRING: { // 取文件列表字符串
            JComboBox comboBox = new JComboBox();
            comboBox.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
            String filename = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                          pdb_fieldtable.
                                          pdb_field_datasource_no +
                                          j * pdb_fieldtable.fieldnums]);
            ReadFile rf = new ReadFile(filename);
            rf.openFile(false);
            int count = rf.vlist.size();
            for (int n = 0; n < count; n++) {
              comboBox.addItem( (String) rf.vlist.elementAt(n));
            }
            comboBox.setEditable(true);
            mycolumn.setCellEditor(new DefaultCellEditor(comboBox));
            break;
          }
          case ConstantDef.IN_BYFILE_INDEX: { //取文件列表字符串中的索引值
            ComboBoxEvalueMode comboBox = new ComboBoxEvalueMode();

            comboBox.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
            String filename = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                          pdb_fieldtable.
                                          pdb_field_datasource_no +
                                          j * pdb_fieldtable.fieldnums]);
            ReadFile rf = new ReadFile(filename);
            rf.openFile(true);
            int count = rf.vlist.size(); //行记录
            int indexcount = rf.vindexlist.size(); //索引记录
            CellRenderer cellrend = new CellRenderer();
            cellrend.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
            cellrend.setCol(i);
            cellrend.setInputMode(ConstantDef.IN_BYFILE_INDEX);
            cellrend.setRealContent(rf.vindexlist);
            cellrend.setShowContent(rf.vindexcontent);
            vCellRenderer.add(cellrend);
            //文件内容记录
            for (int n = 0; n < count; n++) {
              comboBox.addItem( (String) rf.vlist.elementAt(n));
            }
            comboBox.setEditable(true);
            mycolumn.setCellEditor(new DefaultCellEditor(comboBox));
            break;
          }
          case ConstantDef.IN_BYTABLE_SELECTEDSTRING: { //取表记录列表字符串
            JComboBox comboBox = new JComboBox();
            comboBox.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
            String datsrcname = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                            pdb_fieldtable.
                                            pdb_field_datasource_no +
                                            j * pdb_fieldtable.fieldnums]); //初始化信息源名
            String datsrclistname = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                                pdb_fieldtable.
                                                pdb_field_srcdatafield_no +
                                                j * pdb_fieldtable.fieldnums]); //列表域名
            ReadDB readdb = new ReadDB(m_db, datsrcname);
            String where = "";
            if (datsrcname.trim() != "" && datsrclistname.trim() != "") {
              String conditionflag = pdll.substr(tablesetinfo.tableFieldInfo.
                                                 buf[
                                                 pdb_fieldtable.
                                                 pdb_field_conditionflag_no +
                                                 j * pdb_fieldtable.fieldnums]); //条件域值
              String conditionfield = pdll.substr(tablesetinfo.tableFieldInfo.
                                                  buf[
                                                  pdb_fieldtable.
                                                  pdb_field_conditionfield_no +
                                                  j * pdb_fieldtable.fieldnums]); //选择域名
              String valuefrom = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                             pdb_fieldtable.
                                             pdb_field_valuefrom_no +
                                             j * pdb_fieldtable.fieldnums]); //选择域初值
              String valueto = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                           pdb_fieldtable.
                                           pdb_field_valueto_no +
                                           j * pdb_fieldtable.fieldnums]); //选择域终值
              String calcfield1 = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                              pdb_fieldtable.
                                              pdb_field_calcfield1_no +
                                              j * pdb_fieldtable.fieldnums]); //用来做排序域名
              if (conditionflag != "0" && conditionfield != "" &&
                  (valuefrom != "" || valueto != "")) { //有条件域值可选择
                int fieldtype = pdll.getFieldType(m_db, datsrcname,
                                                  conditionfield); //选择域名的数据类型
                switch (fieldtype) {
                  case ConstantDef.DATATYPE: {
                    if (valuefrom == "" && valueto != "") {
                      where = conditionfield + "<=" + Integer.parseInt(valueto);
                    }
                    else if (valuefrom != "" && valueto == "") {
                      where = conditionfield + ">=" +
                          Integer.parseInt(valuefrom);
                    }
                    else {
                      where = conditionfield + ">=" +
                          Integer.parseInt(valuefrom) + " and " +
                          conditionfield + "<=" + Integer.parseInt(valueto);
                    }
                    break;
                  }
                  case ConstantDef.FLOATTYPE: {
                    if (valuefrom == "" && valueto != "") {
                      where = conditionfield + "<=" + Float.parseFloat(valueto);
                    }
                    else if (valuefrom != "" && valueto == "") {
                      where = conditionfield + ">=" +
                          Float.parseFloat(valuefrom);
                    }
                    else {
                      where = conditionfield + ">=" +
                          Float.parseFloat(valuefrom) + " and " +
                          conditionfield + "<=" + Float.parseFloat(valueto);
                    }
                    break;
                  }
                  case ConstantDef.STRINGTYPE: {
                    where = conditionfield + ">= '" + valuefrom + "' and " +
                        conditionfield + "<= '" + valueto + "'";
                    break;
                  }
                }
              }
              readdb.queryTable(datsrclistname.trim(), where.trim(), calcfield1);
              int count = readdb.getRecords();
              for (int n = 0; n < count; n++) {
                comboBox.addItem(readdb.getAFieldValue(n, 0));
              }
            }
            comboBox.setEditable(true);
            mycolumn.setCellEditor(new DefaultCellEditor(comboBox));
            break;
          }
          case ConstantDef.IN_BYTABLE_SPECIFIED_FIELD: { //取表指定域的域值
            ComboBoxEx comboBox = new ComboBoxEx();
            comboBox.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
            String datsrcname = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                            pdb_fieldtable.
                                            pdb_field_datasource_no +
                                            j * pdb_fieldtable.fieldnums]); //初始化信息源名
            String datsrclistname = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                                pdb_fieldtable.
                                                pdb_field_srcdatafield_no +
                                                j * pdb_fieldtable.fieldnums]); //列表域名
            String searchedfield = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                               pdb_fieldtable.
                                               pdb_field_searchedfield_no +
                                               j * pdb_fieldtable.fieldnums]); //信息域名
            ReadDB readdb = new ReadDB(m_db, datsrcname);
            String where = "";
            if (datsrcname.trim() != "" && datsrclistname.trim() != "" &&
                searchedfield != "") { //初始化信息源名,列表域名,信息域名都不能为空
              String fieldname;
              if (datsrclistname.equalsIgnoreCase(searchedfield)) {
                fieldname = datsrclistname;
              }
              else {
                fieldname = datsrclistname + "," + searchedfield;
              }
              String conditionflag = pdll.substr(tablesetinfo.tableFieldInfo.
                                                 buf[
                                                 pdb_fieldtable.
                                                 pdb_field_conditionflag_no +
                                                 j * pdb_fieldtable.fieldnums]); //条件域值
              String conditionfield = pdll.substr(tablesetinfo.tableFieldInfo.
                                                  buf[
                                                  pdb_fieldtable.
                                                  pdb_field_conditionfield_no +
                                                  j * pdb_fieldtable.fieldnums]); //选择域名
              String valuefrom = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                             pdb_fieldtable.
                                             pdb_field_valuefrom_no +
                                             j * pdb_fieldtable.fieldnums]); //选择域初值
              String valueto = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                           pdb_fieldtable.
                                           pdb_field_valueto_no +
                                           j * pdb_fieldtable.fieldnums]); //选择域终值
              String calcfield1 = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                              pdb_fieldtable.
                                              pdb_field_calcfield1_no +
                                              j * pdb_fieldtable.fieldnums]); //用来做排序域名
              if ( (!conditionflag.equalsIgnoreCase("0")) &&
                  (!conditionfield.equalsIgnoreCase("")) &&
                  ( (!valuefrom.equalsIgnoreCase("")) ||
                   !valueto.equalsIgnoreCase(""))) { //有条件域值可选择
                int fieldtype = pdll.getFieldType(m_db, datsrcname,
                                                  conditionfield); //选择域名的数据类型
                switch (fieldtype) {
                  case ConstantDef.DATATYPE: {
                    if (valuefrom.equalsIgnoreCase("") &&
                        !valueto.equalsIgnoreCase("")) {
                      where = conditionfield + "<=" + Integer.parseInt(valueto);
                    }
                    else if (!valuefrom.equalsIgnoreCase("") &&
                             valueto.equalsIgnoreCase("")) {
                      where = conditionfield + ">=" +
                          Integer.parseInt(valuefrom);
                    }
                    else {
                      where = conditionfield + ">=" +
                          Integer.parseInt(valuefrom) + " and " +
                          conditionfield + "<=" + Integer.parseInt(valueto);
                    }
                    break;
                  }
                  case ConstantDef.FLOATTYPE: {
                    if (valuefrom.equalsIgnoreCase("") &&
                        !valueto.equalsIgnoreCase("")) {
                      where = conditionfield + "<=" + Float.parseFloat(valueto);
                    }
                    else if (!valuefrom.equalsIgnoreCase("") &&
                             valueto.equalsIgnoreCase("")) {
                      where = conditionfield + ">=" +
                          Float.parseFloat(valuefrom);
                    }
                    else {
                      where = conditionfield + ">=" +
                          Float.parseFloat(valuefrom) + " and " +
                          conditionfield + "<=" + Float.parseFloat(valueto);
                    }
                    break;
                  }
                  case ConstantDef.STRINGTYPE: {
                    where = conditionfield + ">= '" + valuefrom + "' and " +
                        conditionfield + "<= '" + valueto + "'";
                    break;
                  } //case
                } //switch
              } //if
              readdb.queryTable(fieldname.trim(), where.trim(), calcfield1);
              int count = readdb.getRecords();

              CellRenderer cellrend = new CellRenderer();
              cellrend.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
              cellrend.setCol(i);
              cellrend.setInputMode(ConstantDef.IN_BYTABLE_SPECIFIED_FIELD);

              Vector vSearchedField = new Vector(0); //放信息域名的值
              Vector vDatsrcListName = new Vector(0); //列表域名的值
              String tmpStr = "", tmpStr1 = "";
              for (int n = 0; n < count; n++) {
                tmpStr = readdb.getAFieldValue(n, 0).trim();
                comboBox.addItem(tmpStr); //列表域名
                vDatsrcListName.add(n, tmpStr); //列表域名的值
                comboBox.vDisp.add(n, tmpStr);
                if (readdb.getFields() == 1) {
                  vSearchedField.add(n, tmpStr); //信息域名
                  comboBox.vReal.add(tmpStr);
                }
                else {
                  tmpStr1 = readdb.getAFieldValue(n, 1).trim();
                  vSearchedField.add(n, tmpStr1); //信息域名
                  comboBox.vReal.add(tmpStr1);
                }
              } //for n
              //a segment begin
              cellrend.setRealContent(vSearchedField);
              cellrend.setShowContent(vDatsrcListName);
              vCellRenderer.add(cellrend);

              //a segment end
            } //if
            comboBox.setEditable(true);
            mycolumn.setCellEditor(new DefaultCellEditor(comboBox));
            break;
          } //case
          case ConstantDef.IN_FIELDNAME: { //取表的域名信息
            JComboBox comboBox = new JComboBox();
            comboBox.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
            FieldProp realtableinfo = new FieldProp(m_db, m_realtable);
            realtableinfo.getTableInfo();
            for (int n = 0; n < realtableinfo.fieldnums; n++) { //表的域数
              String tablefn = pdll.substr(realtableinfo.tableinfo[n * 3]);
              comboBox.addItem(tablefn);
            }
            mycolumn.setCellEditor(new DefaultCellEditor(comboBox));
            comboBox.setEditable(true);
            break;
          }
          case ConstantDef.IN_UNINPUT: { //不可输入域
            disEnabledInput.add(mytablemodel.getColumnName(i));
            break;
          }
          case ConstantDef.IN_SECOND_INDEX: { //二次分类检索信息,从datasource得到数据，只不过其数据用索引方式产生
            SecIndex secindex = new SecIndex(m_db,(String)m_node.getUserObject());
            secindex.datsrcname = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                              pdb_fieldtable.
                                              pdb_field_datasource_no +
                                              j * pdb_fieldtable.fieldnums]); //初始化信息源名
            secindex.datsrclistname = pdll.substr(tablesetinfo.tableFieldInfo.
                                                  buf[
                                                  pdb_fieldtable.
                                                  pdb_field_srcdatafield_no +
                                                  j * pdb_fieldtable.fieldnums]); //列表域名
            secindex.searchedfield = pdll.substr(tablesetinfo.tableFieldInfo.
                                                 buf[
                                                 pdb_fieldtable.
                                                 pdb_field_searchedfield_no +
                                                 j * pdb_fieldtable.fieldnums]); //信息域名
            if (secindex.doFirstIndex()) { //第一步索引成功

              ReadDB readdb = new ReadDB(m_db, secindex.datsrcname);
              String fieldname = "";
              if (secindex.datsrclistname.equalsIgnoreCase(secindex.
                  searchedfield)) {
                fieldname = secindex.datsrclistname;
              }
              else {
                fieldname = secindex.datsrclistname + "," +
                    secindex.searchedfield;
              }
              readdb.queryTable(fieldname, "", "");
              int count = readdb.getRecords();

              CellRenderer cellrend = new CellRenderer();
              cellrend.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
              cellrend.setCol(i);
              cellrend.setInputMode(ConstantDef.IN_SECOND_INDEX);

              Vector vSearchedField = new Vector(0); //放信息域名的值
              Vector vDatsrcListName = new Vector(0); //列表域名的值
              String tmpStr = "", tmpStr1 = "";
              for (int n = 0; n < count; n++) {
                tmpStr = readdb.getAFieldValue(n, 0);
                vDatsrcListName.add(n, tmpStr); //列表域名的值
                if (readdb.getFields() == 1) {
                  vSearchedField.add(n, tmpStr); //信息域名
                }
                else {
                  tmpStr1 = readdb.getAFieldValue(n, 1);
                  vSearchedField.add(n, tmpStr1); //信息域名
                } //else
              } //for n
              //a segment begin
              cellrend.setRealContent(vSearchedField);
              cellrend.setShowContent(vDatsrcListName);
              vCellRenderer.add(cellrend);

              secindex.conditionflag = pdll.substr(tablesetinfo.tableFieldInfo.
                  buf[pdb_fieldtable.pdb_field_conditionflag_no +
                  j * pdb_fieldtable.fieldnums]); //条件域值
              secindex.conditionfield = pdll.substr(tablesetinfo.tableFieldInfo.
                  buf[pdb_fieldtable.pdb_field_conditionfield_no +
                  j * pdb_fieldtable.fieldnums]); //选择域名
              secindex.valuefrom = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                               pdb_fieldtable.
                                               pdb_field_valuefrom_no +
                                               j * pdb_fieldtable.fieldnums]); //选择域初值
              secindex.valueto = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                             pdb_fieldtable.
                                             pdb_field_valueto_no +
                                             j * pdb_fieldtable.fieldnums]); //选择域终值

              secindex.doSecondIndex();
              SecondIndexEditor siEditor = secindex.siEditor;
              if (siEditor != null) {
                mycolumn.setCellEditor(siEditor);
              }
            }
            break;
          }
          case ConstantDef.IN_TABLENAME: { //取表名
            JComboBox comboBox = new JComboBox();
            comboBox.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
            String datsrcname = "sysobjects";
            String datsrclistname = "name";
            String where = "type = 'U' or type = 'V'";
            ReadDB readdb = new ReadDB(m_db.trim(), datsrcname.trim());
            readdb.queryTable(datsrclistname, where, "");
            int count = readdb.getRecords();
            for (int n = 0; n < count; n++) {
              comboBox.addItem(readdb.getAFieldValue(n, 0));
            }
            mycolumn.setCellEditor(new DefaultCellEditor(comboBox));
            comboBox.setEditable(true);
            break;
          }
          case ConstantDef.IN_PASSWORD: { //密码
            JPasswordField jpwdfield = new JPasswordField();
            jpwdfield.enableInputMethods(true);
            jpwdfield.setEchoChar('#');
            mycolumn.setCellEditor(new DefaultCellEditor(jpwdfield));
            break;
          }
          case ConstantDef.IN_AUTO_INTABLENAME: { //自动显示表名
            disEnabledInput.add(mytablemodel.getColumnName(i));

            CellRenderer cellrend = new CellRenderer();
            cellrend.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
            cellrend.setCol(i);
            cellrend.setInputMode(ConstantDef.IN_AUTO_INTABLENAME);

            Vector vtablename = new Vector(0);
            vtablename.addElement(m_realtable);

            cellrend.setRealContent(vtablename);
            cellrend.setShowContent(vtablename);
            vCellRenderer.add(cellrend);

            break;
          } //case
          case ConstantDef.IN_THIRD_INDEX: //用显示表的第二级索引的二次分类检索,即三次分类检索
          //要求该表必须要有二级索引
          //该域的数据源为在pdb_field中设置的datasource,索引为该表的第二级索引
          {
            ThirdIndex thrindex = new ThirdIndex(m_db);

            thrindex.datsrcname = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                              pdb_fieldtable.
                                              pdb_field_datasource_no +
                                              j * pdb_fieldtable.fieldnums]); //初始化信息源名
            thrindex.datsrclistname = pdll.substr(tablesetinfo.tableFieldInfo.
                                                  buf[
                                                  pdb_fieldtable.
                                                  pdb_field_srcdatafield_no +
                                                  j * pdb_fieldtable.fieldnums]); //列表域名
            thrindex.searchedfield = pdll.substr(tablesetinfo.tableFieldInfo.
                                                 buf[
                                                 pdb_fieldtable.
                                                 pdb_field_searchedfield_no +
                                                 j * pdb_fieldtable.fieldnums]); //信息域名

            if (thrindex.doIndex(m_node)) {
              if (thrindex.comboBox != null) {
                mycolumn.setCellEditor(new DefaultCellEditor(thrindex.comboBox));
              }
            }

            break;
          }
          case ConstantDef.IN_BYFILE_ORDER: { //取文件列表字符串序号
            ComboxFileStringOrder comboBox = new ComboxFileStringOrder();
            comboBox.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
            String filename = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                          pdb_fieldtable.
                                          pdb_field_datasource_no +
                                          j * pdb_fieldtable.fieldnums]);
            ReadFile rf = new ReadFile(filename);
            rf.openFileNew();
            int count = rf.vlist.size(); //行记录
            CellRenderer cellrend = new CellRenderer();
            cellrend.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
            cellrend.setCol(i);
            cellrend.setInputMode(ConstantDef.IN_BYFILE_ORDER);
            cellrend.setRealContent(rf.vindexlist);
            cellrend.setShowContent(rf.vindexcontent);
            vCellRenderer.add(cellrend);

            //文件内容记录
            for (int n = 0; n < count; n++) {
              comboBox.addItem( (String) rf.vlist.elementAt(n));
            }
            comboBox.setEditable(true);
            mycolumn.setCellEditor(new DefaultCellEditor(comboBox));
            break;
          }
          case ConstantDef.IN_SECOND_INDEX2: { //多表二次检索
            SecIndex2 secindex2 = new SecIndex2(m_db);
            secindex2.datsrcname1 = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                                pdb_fieldtable.
                                                pdb_field_datasource_no +
                                                j * pdb_fieldtable.fieldnums]); //初始化信息源名
            secindex2.datsrclistname = pdll.substr(tablesetinfo.tableFieldInfo.
                buf[
                pdb_fieldtable.pdb_field_srcdatafield_no +
                j * pdb_fieldtable.fieldnums]); //列表域名
            secindex2.searchedfield = pdll.substr(tablesetinfo.tableFieldInfo.
                                                  buf[
                                                  pdb_fieldtable.
                                                  pdb_field_searchedfield_no +
                                                  j * pdb_fieldtable.fieldnums]); //信息域名
            secindex2.doIndex();
            SecondIndexEditor siEditor = secindex2.siEditor;
            if (siEditor != null) {
              mycolumn.setCellEditor(siEditor);

            }
            break;
          }
          case ConstantDef.IN_KGBH_FIELD: { //开关保护关系表域输入
            ComboBoxEx comboBox = new ComboBoxEx();
            comboBox.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
            String datsrcname = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                            pdb_fieldtable.
                                            pdb_field_datasource_no +
                                            j * pdb_fieldtable.fieldnums]); //初始化信息源名
            String datsrclistname = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                                pdb_fieldtable.
                                                pdb_field_srcdatafield_no +
                                                j * pdb_fieldtable.fieldnums]); //列表域名
            String searchedfield = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                               pdb_fieldtable.
                                               pdb_field_searchedfield_no +
                                               j * pdb_fieldtable.fieldnums]); //信息域名
            ReadDB readdb = new ReadDB(m_db, datsrcname);
            String where = "";
            if (datsrcname.trim() != "" && datsrclistname.trim() != "" &&
                searchedfield != "") { //初始化信息源名,列表域名,信息域名都不能为空
              String fieldname;
              if (datsrclistname.equalsIgnoreCase(searchedfield)) {
                fieldname = datsrclistname;
              }
              else {
                fieldname = datsrclistname + "," + searchedfield;
              }
              String conditionflag = pdll.substr(tablesetinfo.tableFieldInfo.
                                                 buf[
                                                 pdb_fieldtable.
                                                 pdb_field_conditionflag_no +
                                                 j * pdb_fieldtable.fieldnums]); //条件域值
              String conditionfield = pdll.substr(tablesetinfo.tableFieldInfo.
                                                  buf[
                                                  pdb_fieldtable.
                                                  pdb_field_conditionfield_no +
                                                  j * pdb_fieldtable.fieldnums]); //选择域名
              String valuefrom = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                             pdb_fieldtable.
                                             pdb_field_valuefrom_no +
                                             j * pdb_fieldtable.fieldnums]); //选择域初值
              String valueto = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                           pdb_fieldtable.
                                           pdb_field_valueto_no +
                                           j * pdb_fieldtable.fieldnums]); //选择域终值
              String calcfield1 = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                              pdb_fieldtable.
                                              pdb_field_calcfield1_no +
                                              j * pdb_fieldtable.fieldnums]); //用来做排序域名
              if ( (!conditionflag.equalsIgnoreCase("0")) &&
                  (!conditionfield.equalsIgnoreCase("")) &&
                  ( (!valuefrom.equalsIgnoreCase("")) ||
                   !valueto.equalsIgnoreCase(""))) { //有条件域值可选择
                int fieldtype = pdll.getFieldType(m_db, datsrcname,
                                                  conditionfield); //选择域名的数据类型
                switch (fieldtype) {
                  case ConstantDef.DATATYPE: {
                    if (valuefrom.equalsIgnoreCase("") &&
                        !valueto.equalsIgnoreCase("")) {
                      where = conditionfield + "<=" + Integer.parseInt(valueto);
                    }
                    else if (!valuefrom.equalsIgnoreCase("") &&
                             valueto.equalsIgnoreCase("")) {
                      where = conditionfield + ">=" +
                          Integer.parseInt(valuefrom);
                    }
                    else {
                      where = conditionfield + ">=" +
                          Integer.parseInt(valuefrom) + " and " +
                          conditionfield + "<=" + Integer.parseInt(valueto);
                    }
                    break;
                  }
                  case ConstantDef.FLOATTYPE: {
                    if (valuefrom.equalsIgnoreCase("") &&
                        !valueto.equalsIgnoreCase("")) {
                      where = conditionfield + "<=" + Float.parseFloat(valueto);
                    }
                    else if (!valuefrom.equalsIgnoreCase("") &&
                             valueto.equalsIgnoreCase("")) {
                      where = conditionfield + ">=" +
                          Float.parseFloat(valuefrom);
                    }
                    else {
                      where = conditionfield + ">=" +
                          Float.parseFloat(valuefrom) + " and " +
                          conditionfield + "<=" + Float.parseFloat(valueto);
                    }
                    break;
                  }
                  case ConstantDef.STRINGTYPE: {
                    where = conditionfield + ">= '" + valuefrom + "' and " +
                        conditionfield + "<= '" + valueto + "'";
                    break;
                  } //case
                } //switch
              } //if
              where += m_where;
              readdb.queryTable(fieldname.trim(), where.trim(), calcfield1);
              int count = readdb.getRecords();

              CellRenderer cellrend = new CellRenderer();
              cellrend.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
              cellrend.setCol(i);
              cellrend.setInputMode(ConstantDef.IN_KGBH_FIELD);

              Vector vSearchedField = new Vector(0); //放信息域名的值
              Vector vDatsrcListName = new Vector(0); //列表域名的值
              String tmpStr = "", tmpStr1;
              for (int n = 0; n < count; n++) {
                tmpStr = readdb.getAFieldValue(n, 0);
                comboBox.addItem(tmpStr); //列表域名
                vDatsrcListName.add(n, tmpStr); //列表域名的值
                comboBox.vDisp.add(n, tmpStr);
                if (readdb.getFields() == 1) {
                  vSearchedField.add(n, tmpStr); //信息域名
                  comboBox.vReal.add(tmpStr);
                }
                else {
                  tmpStr1 = readdb.getAFieldValue(n, 1);
                  vSearchedField.add(n, tmpStr1); //信息域名
                  comboBox.vReal.add(tmpStr1);
                }
              } //for n
              //a segment begin

              cellrend.setRealContent(vSearchedField);
              cellrend.setShowContent(vDatsrcListName);
              vCellRenderer.add(cellrend);

              //a segment end
            } //if
            comboBox.setEditable(true);
            mycolumn.setCellEditor(new DefaultCellEditor(comboBox));
            break;
          } //case
        } //switch
      } //if(j < recNums)
    } //for i
  }

  class JScrollBarAdjustmentListener
      implements java.awt.event.AdjustmentListener {
    public void adjustmentValueChanged(AdjustmentEvent e) {
      int value = jsbar.getValue();
      if (m_tabnoCount > 1) {
        if (curTabno != 0) {
          firstJTable.jsbar.setValue(value);
        }
        for (int i = 1; i < m_tabnoCount; i++) {
          if (i == curTabno) {
            continue;
          }
          MyJTable myjtabtmp = firstJTable.getJTable(i - 1);
          myjtabtmp.jsbar.setValue(value);
        }
      }
    }
  }

  public MyJTable() {
    super();
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }
  private void jbInit() throws Exception {
    //this.setBackground(SystemColor.textHighlightText);
//    this.setBackground(new Color(193, 255, 202));
    //表格区背景色
    this.setBackground(new Color(199, 237, 204));
    this.setForeground(new Color(0, 0, 0));

    //选中单元格背景色
    this.setSelectionBackground(Color.pink);
    //图表区的复制、粘贴、查找
    imageCopyCell = new ImageIcon(com.daqo.dbinfo.MyJTable.class.getResource("icon/copy_cell.png"));
    imagepasteCell = new ImageIcon(com.daqo.dbinfo.MyJTable.class.getResource("icon/paste_cell.png"));
    imagefind = new ImageIcon(com.daqo.dbinfo.MyJTable.class.getResource("icon/find_cell.png"));

//    imageCopyCell = new ImageIcon(com.daqo.dbinfo.MyJTable.class.getResource("icon/copy.png"));
//    imagepasteCell = new ImageIcon(com.daqo.dbinfo.MyJTable.class.getResource("icon/paste.png"));
//    imagefind = new ImageIcon(com.daqo.dbinfo.MyJTable.class.getResource("icon/find.png"));
   // ExcelAdapter myAd = new ExcelAdapter(rowHeaderTable);
    //this.setCellSelectionEnabled(true);
  //  this.setShowHorizontalLines(false);
   // this.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
   // this.setBackground(Color.pink);
  }
  /*
  public void setSelect(int col, boolean select){
      colSelect = col;
      this.select = select;
     }
    //这个方法返回某个单元是否被选中------del -by hx

    public boolean isCellSelected(int row, int column)
      throws IllegalArgumentException{
      //重载verride the method for the column set in setSelect()
      if (colSelect == column){
          if (select)
            return true;
          else
            return false;
      } else {
          return super.isCellSelected(row, column);
      }
    }
    */
  }
