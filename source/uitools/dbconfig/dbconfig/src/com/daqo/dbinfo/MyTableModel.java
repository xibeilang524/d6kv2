package com.daqo.dbinfo;

import java.io.*;
import java.util.*;

import javax.swing.event.*;
import javax.swing.table.*;
import java.awt.Color;
import javax.swing.*;

//sort to begin
import javax.swing.JTable;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.InputEvent;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class MyTableModel
    extends AbstractTableModel
    implements Serializable {

  Dlllink pdll = Dlllink.getInstance();
  public Vector dataVector;
  protected Vector columnIdentifiers;

  protected Vector dataVectorBk; //
  protected Vector delData; //保存删除数据
  private Vector delListData = new Vector(0); //保存删除的列表域
  protected Vector rowProp; //0,unchanged;1,insert;2,changed
  private String keynums[]; //主关键字的列名
  private boolean initendflag = false; //初始化标志，SetValueAt初始化时会改变值，影响保存
  private boolean bCellChangedFlag = false;
  private boolean bSaveFlag = false; //需要保存标志
  private Vector vColType = new Vector(0); //记录列的类型，在记录数为0时用
  private String tableType = ""; //表的类型，"U" OR "V"
  private String modelDBName = ""; //表所在的参数库的名称
  Vector disEnableInput = new Vector(0); //不可输入域
  private Vector vLongValues = new Vector(0); //存储每列的最长值
  private String tablename = "";
  private String listfield = "";
  private Vector vValue = new Vector(0); //列表值
  public ManageModel managemodel;

  YKInfo ykinfo = null;
  int Srow,Scol;
  int[] colinputmode;
  //sort begin
  int indexes[]; //存储所有的行号,row based 0
  boolean sortchangedf; //是否有行改变位置的标志
  boolean ascending; //正序或逆序

  public MyTableModel(Object[][] data, Object[] columnNames) {
    setDataVector(data, columnNames);
    colinputmode = new int[columnNames.length];
    for (int i = 0; i < colinputmode.length; i++) {
      colinputmode[i] = 0;
    }
  }

  public void setManageModel(ManageModel managemodel)
      {
        this.managemodel = managemodel;
      }

  public void setYKInfo( YKInfo info)
      {
        this.ykinfo = info;
      }

  public void setKey(Vector vkey) {
    String keysname;
    keysname = "setKey关键字段名为：";
    keynums = new String[vkey.size()];
    for (int i = 0; i < vkey.size(); i++) {
      keynums[i] = vkey.elementAt(i).toString().trim();
      keysname += keynums[i];
      keysname += "   ";
    }

 //   dbginfo.writelog(tablename,keysname) ;
  }

  public int getKeyCount() {
    return keynums.length;
  }

  public String[] getkeys() {
    return this.keynums;
  }

  public void setTableType(String tabletype) {
    tableType = tabletype.toUpperCase();
  }

  public String getTableType() {
    return tableType;
  }

  public String getTableName() {
    return tablename;
  }

  public void setDBName(String modelDBName, String TableName) {
    this.modelDBName = modelDBName;
    tablename = TableName;
  }

  public void setListField(String name, Vector vValue) {
    this.listfield = name;
    for (int i = 0; i < vValue.size(); i++) {
      this.vValue.add(vValue.elementAt(i));
    }
  }

  public void setListField(int row, String ovalue) {
    if (this.getListFieldFlag()) {
      this.vValue.setElementAt(ovalue, row);
    }
  }

  public void addListField(String value) {
    if (this.getListFieldFlag()) {
      this.vValue.add(value);
    }
  }

  public void addListField(int row) {
    if (this.getListFieldFlag()) {
      if (row >= vValue.size()) {
        vValue.add(new String(""));
      }
      Object o = vValue.elementAt(row);
      vValue.add(o);
    }
  }

  public void insertListField(int row) {
    Object o = vValue.elementAt(row);
    vValue.add(row + 1, o);
  }

  public String getListField() {
    return listfield;
  }

  public boolean getListFieldFlag() {
    if (listfield.trim().equalsIgnoreCase("")) {
      return false;
    }
    return true;
  }

  public String getListField(int row) {
    if (!this.getListFieldFlag()) {
      return "";
    }
    if (vValue.size() == 0) {
      return "";
    }
    if (row >= vValue.size()) {
      return "";
    }
    return vValue.elementAt(row).toString();
  }

  public String getDBName() {
    return modelDBName;
  }

  public int getDelRows() {
    return delData.size();
  }

  public void setColType(Vector vcoltype) {
    int colsize = vcoltype.size();
    vColType.setSize(colsize);
    for (int i = 0; i < colsize; i++) {
      vColType.set(i, vcoltype.elementAt(i).toString());
    }
  }

  public void setLongValues(Vector longvalues) {
    vLongValues = (Vector) longvalues.clone();
  }

  public Vector getLongValues() {
    return vLongValues;
  }

  public void setSaveFlag(boolean saveflag) {
    bSaveFlag = saveflag;
  }

  public boolean getSaveFlag() {
    return bSaveFlag;
  }

  public void init() {

    initendflag = false;

    int size = dataVector.size();
    dataVectorBk = new Vector(size);
    rowProp = new Vector(size);
    delData = new Vector();
    for (int i = 0; i < size; i++) {
      Vector tmp = (Vector) dataVector.elementAt(i);
      dataVectorBk.addElement(tmp.clone());
      rowProp.addElement(Integer.toString(ConstantDef.DB_UNCHANGED));
    }
    ascending = true;
  }

  public void setCellChangedFlag(boolean flag) {
    bCellChangedFlag = flag;
  }

  public boolean getCellChangedFlag() {
    return bCellChangedFlag;
  }

  public void setColInputMode(int col, int mode) {
    if (colinputmode.length < col) {
      return;
    }
    colinputmode[col] = mode;
  }

  public int getInputMode(int col) {
    if (col >= colinputmode.length) {
      return 0;
    }
    return colinputmode[col];
  }

  public void setDisEnabledInputFieldName(Vector vfieldname) {
    try {
      for (int i = 0; i < vfieldname.size(); i++) {
        disEnableInput.add(i, vfieldname.elementAt(i).toString());
      }
    }
    catch (NullPointerException ne) {
    }
  }

  public void initEnds() {
    initendflag = true;
  }

//
// Querying and Modifying the data structure
//

  public Vector getDataVector() {
    return dataVector;
  }

  public void setDataVector(Vector newData, Vector columnNames) {
    if (newData == null) {
      throw new IllegalArgumentException("setDataVector() - Null parameter");
    }

    // Clear all the previous data.
    dataVector = new Vector(0);

    // Install the new column structure, this will fireTableStructureChanged
    setColumnIdentifiers(columnNames);

    // Add the new rows.
    dataVector = newData;

    // Make all the new rows the right length and generate a notification.
    newRowsAdded(new TableModelEvent(this, 0, getRowCount() - 1,
                                     TableModelEvent.ALL_COLUMNS,
                                     TableModelEvent.INSERT));
  }

  public void setDataVector(Object[][] newData, Object[] columnNames) {
    setDataVector(convertToVector(newData), convertToVector(columnNames));
  }

  public void newDataAvailable(TableModelEvent event) {
    fireTableChanged(event);
  }

  public void newRowsAdded(TableModelEvent event) {
    int start = event.getFirstRow();
    int end = event.getLastRow();
    if (start < 0) {
      start = 0;
    }
    if (end < 0) {
      end = getRowCount() - 1;

      // Have to make sure all the new columns have the correct
      // number of columns
    }
    for (int i = start; i < end; i++) {
      ( (Vector) dataVector.elementAt(i)).setSize(getColumnCount());
    }

    // Now we send the notification
    fireTableChanged(event);
  }

  public void rowsRemoved(TableModelEvent event) {
    fireTableChanged(event);
  }

  public void setColumnIdentifiers(Vector newIdentifiers) {
    if (newIdentifiers != null) {
      columnIdentifiers = newIdentifiers;
    }
    else {
      columnIdentifiers = new Vector();
    }

    // Generate notification
    fireTableStructureChanged();
  }

  public void setColumnIdentifiers(Object[] newIdentifiers) {
    setColumnIdentifiers(convertToVector(newIdentifiers));
  }

  public void setNumRows(int newSize) {
    if ( (newSize < 0) || (newSize == getRowCount())) {
      return;
    }

    int oldNumRows = getRowCount();
    if (newSize <= getRowCount()) {
      // newSize is smaller than our current size, so we can just
      // let Vector discard the extra rows
      dataVector.setSize(newSize);

      // Generate notification
      fireTableRowsDeleted(getRowCount(), oldNumRows - 1);
    }
    else {
      int columnCount = getColumnCount();
      // We are adding rows to the model
      while (getRowCount() < newSize) {
        Vector newRow = new Vector(columnCount);
        newRow.setSize(columnCount);
        dataVector.addElement(newRow);
      }

      // Generate notification
      fireTableRowsInserted(oldNumRows, getRowCount() - 1);
    }
  }

  public void setRowCount(int rowCount) {
    setNumRows(rowCount);
  }

  public void setColumnCount(int columnCount) {
    for (int r = 0; r < getRowCount(); r++) {
      Vector row = (Vector) dataVector.elementAt(r);
      row.setSize(columnCount);
    }
    columnIdentifiers.setSize(columnCount);
    fireTableStructureChanged();
  }

  public void addColumn(Object columnName) {
    addColumn(columnName, (Vector)null);
  }

  public void addColumn(Object columnName, Vector columnData) {
    if (columnName == null) {
      throw new IllegalArgumentException("addColumn() - null parameter");
    }

    columnIdentifiers.addElement(columnName);

    // Fill in the new column, with nulls or with columnData
    int index = 0;
    Enumeration enumeration = dataVector.elements();
    while (enumeration.hasMoreElements()) {
      Object value;

      if ( (columnData != null) && (index < columnData.size())) {
        value = columnData.elementAt(index);
      }
      else {
        value = null;

      }
      ( (Vector) enumeration.nextElement()).addElement(value);
      index++;
    }

    // Generate notification
    fireTableStructureChanged();
  }

  public void addColumn(Object columnName, Object[] columnData) {
    addColumn(columnName, convertToVector(columnData));
  }

  public int unRemove() {
    int remnums = delData.size();
    Vector v = new Vector(0);

    for (int i = 0; i < remnums; i++) {
      v = (Vector) delData.elementAt(remnums - i - 1);

      dataVector.addElement(v.clone());

      dataVectorBk.addElement(v.clone());
      rowProp.addElement(Integer.toString(ConstantDef.DB_UNCHANGED));
      if (this.getListFieldFlag()) {
        vValue.add(delListData.elementAt(remnums - i - 1));
        delListData.removeElementAt(remnums - i - 1);
      }
      delData.removeElementAt(remnums - i - 1);
    }
    newRowsAdded(new TableModelEvent(this, getRowCount() - 1, getRowCount() - 1,
                                     TableModelEvent.ALL_COLUMNS,
                                     TableModelEvent.INSERT));
    return remnums;
  }

  public void addRow(Vector rowData) {
    if (rowData == null) {
      rowData = new Vector(getColumnCount());
    }
    else {
      rowData.setSize(getColumnCount());
    }
    dataVector.addElement(rowData);

    dataVectorBk.addElement(rowData);
    rowProp.addElement(Integer.toString(ConstantDef.DB_INSERT)); //insert row
    if (PreProcessor.TableLock) {
      //表互斥修改   --洪祥
      byte[][] colvalue = new byte[1][0];
      if (ConstantDef.Vtablename.contains(getTableName()) == false) {
        ConstantDef.Vtablename.addElement(getTableName());
        int rtn = pdll.openTableInfo(ConstantDef.MODELDB.getBytes(),
                                     "table_status".getBytes());
        if (rtn <= 0) {
          pdll.closeTableInfo();
          return;
        }
        colvalue[0] = getTableName().getBytes();
        pdll.insertARecord(colvalue);
        pdll.closeTableInfo();
      }
  //表修改互斥
    }
    bSaveFlag = true;

    // Generate notification
    newRowsAdded(new TableModelEvent(this, getRowCount() - 1, getRowCount() - 1,
                                     TableModelEvent.ALL_COLUMNS,
                                     TableModelEvent.INSERT));
  }

  public void addRow(Object[] rowData) {
    addRow(convertToVector(rowData));
  }

  public void insertRow(int row, Vector rowData) {
    if (rowData == null) {
      rowData = new Vector(getColumnCount());
    }
    else {
      rowData.setSize(getColumnCount());
    }

    dataVector.insertElementAt(rowData, row);
    dataVectorBk.insertElementAt(rowData, row);

    rowProp.insertElementAt(Integer.toString(ConstantDef.DB_INSERT), row);
    if (PreProcessor.TableLock) {
    //表修改互斥   --洪祥
    byte[][] colvalue = new byte[1][0];
    if (ConstantDef.Vtablename.contains(getTableName()) == false) {
      ConstantDef.Vtablename.addElement(getTableName());
      int rtn = pdll.openTableInfo(ConstantDef.MODELDB.getBytes(),
                                   "table_status".getBytes());
      if (rtn <= 0) {
        pdll.closeTableInfo();
        return;
      }
      colvalue[0] = getTableName().getBytes();
      pdll.insertARecord(colvalue);
      pdll.closeTableInfo();
    }
  //表修改互斥
    }

    bSaveFlag = true;
    // Generate notification
    newRowsAdded(new TableModelEvent(this, row, row,
                                     TableModelEvent.ALL_COLUMNS,
                                     TableModelEvent.INSERT));
  }

  public void insertRow(int row, Object[] rowData) {
    insertRow(row, convertToVector(rowData));
  }

  public void moveRow(int startIndex, int endIndex, int toIndex) {
    if ( (startIndex < 0) || (startIndex >= getRowCount())) {
      throw new ArrayIndexOutOfBoundsException(startIndex);
    }
    if ( (endIndex < 0) || (endIndex >= getRowCount())) {
      throw new ArrayIndexOutOfBoundsException(endIndex);
    }
    if (startIndex > endIndex) {
      throw new ArrayIndexOutOfBoundsException();
    }

    if ( (startIndex <= toIndex) && (toIndex <= endIndex)) {
      return; // Nothing to move
    }

    boolean shift = toIndex < startIndex;

    // Do the move by first removing the row, then reinserting it
    for (int i = startIndex; i <= endIndex; i++) {
      Object aRow = dataVector.elementAt(i);
      dataVector.removeElementAt(i);
      dataVector.insertElementAt(aRow, toIndex);

      if (shift) {
        toIndex++;
      }
    }

    // Generate notification
    fireTableDataChanged();
  }

  public void removeRow(int row) {
    int rowp;
    rowp = Integer.parseInt(rowProp.elementAt(row).toString());
    if (rowp != ConstantDef.DB_INSERT) { //isn't insert
      delData.addElement(dataVectorBk.elementAt(row));
      if (getListFieldFlag()) {
        delListData.addElement(vValue.elementAt(row));
      }
    }
    if (getListFieldFlag()) {
      vValue.removeElementAt(row);
    }

    dataVector.removeElementAt(row);
    dataVectorBk.removeElementAt(row);
    rowProp.removeElementAt(row);
    if (PreProcessor.TableLock) {
    //表互斥修改   --洪祥
    byte[][] colvalue = new byte[1][0];
    if (ConstantDef.Vtablename.contains(getTableName()) == false) {
      ConstantDef.Vtablename.addElement(getTableName());
      int rtn = pdll.openTableInfo(ConstantDef.MODELDB.getBytes(),
                                   "table_status".getBytes());
      if (rtn <= 0) {
        pdll.closeTableInfo();
        return;
      }
      colvalue[0] = getTableName().getBytes();
      pdll.insertARecord(colvalue);
      pdll.closeTableInfo();
    }
  //表修改互斥
    }
    bSaveFlag = true;
    // Generate notification
    fireTableRowsDeleted(row, row);
  }

//
// Implementing the TableModel interface
//
  public int getRowCount() {
    return dataVector.size();
  }

  public int getColumnCount() {
    return columnIdentifiers.size();
  }

  public String getColumnName(int column) {
    if (columnIdentifiers == null || columnIdentifiers.size() <= column) {
      return super.getColumnName(column);
    }
    Object id = columnIdentifiers.elementAt(column);
    if (id == null) {
      return super.getColumnName(column);
    }
    else {
      return id.toString();
    }
  }

  public boolean isCellEditable(int row, int column) {
  if (PreProcessor.TableLock) {   //表互斥修改  ---洪祥，判断表是否在修改
    byte[][] buf;
    int[] fieldcount = new int[1];
    String wh;
    wh = "tablename = '" + getTableName() + "'";
    buf = pdll.queryAnyTable(ConstantDef.MODELDB, "table_status",
                             "tablename", wh, "", fieldcount, false);
    int len = buf.length;
     if (getSaveFlag() == false) {
       if (len > 0) {
        // JOptionPane.showMessageDialog(null, "此表正在编辑中", "提示",
        //                               JOptionPane.WARNING_MESSAGE);
         return false;
       }
     }
//表互斥修改
  }
    User user = ConstantDef.getUserInstance();
    if (user.getEditPara() == 0) {
      return false;
    }
    if (!tableType.equalsIgnoreCase("U")) {
      return false; //不是用户基本表，不允许编辑
    }
    if (keynums.length < 1) {
      return false; //没有主键，不允许编辑
    }
    //用户口令参数表只有管理员且为组长的人才有编辑的权限
    if (tablename.equals(ConstantDef.USER_PASSTABLE)) {
      if (user.isManageGroup() == false || user.getMainman() == 0) {
        return false;
      }
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
        return false;
      }
    }
    for (int i = 0; i < disEnableInput.size(); i++) {
      if (disEnableInput.elementAt(i).toString().equalsIgnoreCase(getColumnName(
          column))) {
        return false;
      }
    }
    return true;
  }

  public Object getValueAt(int row, int column) {
    Vector rowVector = (Vector) dataVector.elementAt(row);
    return rowVector.elementAt(column);
  }

  /*
   * JTable uses this method to determine the default renderer/
   * editor for each cell.  If we didn't implement this method,
   * then the  column would contain text ("true"/"false"),
   * rather than a check box.
   */
  public Class getColumnClass(int c) {

    try {
      return getValueAt(0, c).getClass();
    }
    catch (NullPointerException ne) {
      int type = Integer.parseInt(vColType.elementAt(c).toString());
      if (type == ConstantDef.FLOATTYPE) {
        return Float.class;
      }
      else if (type == ConstantDef.DATATYPE) {
        return Integer.class;
      }
      else if (type == ConstantDef.BOOLEAN) {
        return Boolean.class;
      }
      else {
        return String.class;
      }
    }

  }


  public void setValueAt(Object aValue, int row, int column) {

    boolean bValChanged = false;
    //处理了一个BUG hx
    if (row > dataVector.size() || row == dataVector.size()) {
      return;
    }
  //

    Vector rowVector = (Vector) dataVector.elementAt(row);
    Object oldValue = rowVector.elementAt(column);
    if (aValue == null) {
      return;
    }
    if (ykinfo != null)
      {
      if(ykinfo.isYKSomeField(this.getColumnName(column)))
        {
        if(ykinfo.checkYKNo(aValue, row, column)) {
          JOptionPane.showMessageDialog(null, "遥控号不允许重复", "提示", JOptionPane.WARNING_MESSAGE);   //广东测试需求，添加遥控号重复的提示。
          return; //遥控号不允许重复
        }
      }
    }
    if (oldValue == null) {
      bValChanged = true;
    }
    else if (oldValue.getClass() != aValue.getClass()) {
      if (oldValue.toString().equals(aValue.toString()) == false) {
        bValChanged = true;
      }
    }
    else if (oldValue.equals(aValue) == false) {
      bValChanged = true;
    }
    if (bValChanged) {
      rowVector.setElementAt(aValue, column);
      if (initendflag) { //初始化结束后才设改变标志，否则影响保存。
        int rowp;
        rowp = Integer.parseInt(rowProp.elementAt(row).toString());
        if (rowp != ConstantDef.DB_INSERT) { //isn't insert
          rowProp.setElementAt(Integer.toString(ConstantDef.DB_CHANGED), row);
          managemodel.updateArec(row) ;
        }
        bCellChangedFlag = true;
        if (PreProcessor.TableLock) {
        //表互斥修改   --洪祥
        byte[][] colvalue = new byte[1][0];
        if (ConstantDef.Vtablename.contains(getTableName()) == false) {
          ConstantDef.Vtablename.addElement(getTableName());
          int rtn = pdll.openTableInfo(ConstantDef.MODELDB.getBytes(),
                                       "table_status".getBytes());
          if (rtn <= 0) {
            pdll.closeTableInfo();
            return;
          }
          colvalue[0] = getTableName().getBytes();
          pdll.insertARecord(colvalue);
          pdll.closeTableInfo();
        }
        //表修改互斥
        }
        bSaveFlag = true;
      }
      // generate notification
      fireTableChanged(new TableModelEvent(this, row, row, column));
    }
  }

//
// Protected Methods
//

  protected static Vector convertToVector(Object[] anArray) {
    if (anArray == null) {
      return null;
    }

    Vector v = new Vector(anArray.length);
    for (int i = 0; i < anArray.length; i++) {
      v.addElement(anArray[i]);
    }
    return v;
  }

  protected static Vector convertToVector(Object[][] anArray) {
    if (anArray == null) {
      return null;
    }

    Vector v = new Vector(anArray.length);
    for (int i = 0; i < anArray.length; i++) {
      v.addElement(convertToVector(anArray[i]));
    }
    return v;
  }

  //sort begin
  public int compareRowsByColumn(int row1, int row2, int column) {
    Class type = getColumnClass(column);
    // Check for nulls.

    Object o1 = getValueAt(row1, column);
    Object o2 = getValueAt(row2, column);

    // If both values are null, return 0.
    if (o1 == null && o2 == null) {
      return 0;
    }
    else if (o1 == null) { // Define null less than everything.
      return -1;
    }
    else if (o2 == null) {
      return 1;
    }
  //  return o1.toString().compareTo(o2.toString() ) ;
    /*
     * We copy all returned values from the getValue call in case
     * an optimised model is reusing one object to return many
     * values.  The Number subclasses in the JDK are immutable and
     * so will not be used in this way but other subclasses of
     * Number might want to do this to save space and avoid
     * unnecessary heap allocation.
     */

    if (type.getSuperclass() == java.lang.Number.class) {
      double d1=0,d2=0;
      Number n1=null,n2=null;
      try{
        //lcg 20090414修改类型转换过程,由于修改了列,类型可能有改变
        if(o1.getClass()==String.class)
          n1=(Number)Integer.valueOf(o1.toString());
        else
          n1 = (Number) getValueAt(row1, column);
        d1 = n1.doubleValue();
        if(o2.getClass()==String.class)
          n2=(Number)Integer.valueOf(o2.toString());
        else
         n2 = (Number) getValueAt(row2, column);
        d2 = n2.doubleValue();
      }
      catch(Exception e){
        PreProcessor.log.fine(e.getMessage());
        d1=d2=0;  //lcg 异常后则直接返回相同,不在返回比较结果
      }


      if (d1 < d2) {
        return -1;
      }
      else if (d1 > d2) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if (type == java.util.Date.class) {
      Date d1 = (Date) getValueAt(row1, column);
      long n1 = d1.getTime();
      Date d2 = (Date) getValueAt(row2, column);
      long n2 = d2.getTime();

      if (n1 < n2) {
        return -1;
      }
      else if (n1 > n2) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if (type == String.class) {
      String s1 = (String) getValueAt(row1, column);
      String s2 = (String) getValueAt(row2, column);
      int result = s1.compareTo(s2);

      if (result < 0) {
        return -1;
      }
      else if (result > 0) {
        return 1;
      }
      else {
        return 0;
      }
    }
    else if (type == Boolean.class) {
      Boolean bool1 = (Boolean) getValueAt(row1, column);
      boolean b1 = bool1.booleanValue();
      Boolean bool2 = (Boolean) getValueAt(row2, column);
      boolean b2 = bool2.booleanValue();

      if (b1 == b2) {
        return 0;
      }
      else if (b1) { // Define false < true
        return 1;
      }
      else {
        return -1;
      }
    }
    else {
      Object v1 = getValueAt(row1, column);
      String s1 = v1.toString();
      Object v2 = getValueAt(row2, column);
      String s2 = v2.toString();
      int result = s1.compareTo(s2);

      if (result < 0) {
        return -1;
      }
      else if (result > 0) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }

  public void swap(int i, int j) {
    int tmp = indexes[i];
    indexes[i] = indexes[j];
    indexes[j] = tmp;
    sortchangedf = true;
  }

  public int compare(int column, boolean ascending, int row1, int row2) {
    //compares++;
    int result = compareRowsByColumn(row1, row2, column);
    if (result != 0) {
      return ascending ? result : -result;
    }
    return 0;
  }

  public void n2sort(int column, boolean ascending) {
    int rowCnt=getRowCount();
    for (int i = 0; i < rowCnt; i++) {
      for (int j = i + 1; j < rowCnt; j++) {
        if (compare(column, ascending, indexes[i], indexes[j]) == -1) {
          swap(i, j);
        }
      }
    }
  }

  /////////////////////////快速排序//////////////////
  public static final Random RND = new Random();
  private void swap(int[] array, int i, int j) {
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
    sortchangedf=true;
  }

  private int partition(int[] array, int begin, int end, int column) {
    int index = begin + RND.nextInt(end - begin + 1);
    int pivot = array[index];
    swap(array, index, end);
    for (int i = index = begin; i < end; ++i) {
      if (compare(column, ascending, array[i],pivot) == -1) {
        swap(array,index++,i);
      }
    }
    swap(array, index, end);
    return (index);
  }
  private void qsort(int[] array, int begin, int end, int column) {
    if (end > begin) {
      int index = partition(array, begin, end, column);
      qsort(array, begin, index - 1, column);
      qsort(array, index + 1, end, column);
    }
  }
  public void Quicksort(int[] array,int column) {
    qsort(array, 0, array.length - 1, column);
  }
  //////////////////////
  /////////////////////希尔排序
  void shellsort(int[] array, int size,int column) {
    int i, j, k, temp, gap;
    int[] gaps = {
        1, 5, 13, 43, 113, 297, 815, 1989, 4711, 11969, 27901, 84801,
        213331, 543749, 1355339, 3501671, 8810089, 21521774,
        58548857, 157840433, 410151271, 1131376761, 2147483647};
    for (k = 0; gaps[k] < size; k++);
    while (--k >= 0) {
      gap = gaps[k];
      for (i = gap; i < size; i++) {
        temp = array[i];
        j = i;
        while (j >= gap && compare(column, ascending, array[j-gap],temp) == -1) {
          array[j] = array[j - gap];
          j = j - gap;
          sortchangedf=true;
        }
        array[j] = temp;
      }
    }
  }

  //////////////////////

  public void sortByColumn(int column) {
    ascending = !ascending;
    int rowCount = getRowCount();
    indexes = new int[rowCount];
    for (int row = 0; row < rowCount; row++) {
      indexes[row] = row;
    }
//    n2sort(column, ascending);

    PreProcessor.log.info("排序开始");
    //n2sort(column, ascending);
    //Quicksort(indexes,column);
    shellsort(indexes,indexes.length,column);
    PreProcessor.log.info("排序结束");
  }

  //实施排序的结果
  public void changeOrder(int sortindex[]) {
    int indexlen = sortindex.length;
    if (indexlen == 0) {
      return;
    }
    if (indexlen != getRowCount()) {
      return;
    }
    Vector dataVector1 = (Vector) dataVector.clone();
    Vector dataVectorBk1 = (Vector) dataVectorBk.clone();
    Vector rowProp1 = (Vector) rowProp.clone();
    Vector vValue1 = new Vector(0);
    if (getListFieldFlag()) {
      vValue1 = (Vector) vValue.clone();
      vValue.setSize(0);
    }

    dataVector.setSize(0);
    dataVectorBk.setSize(0);
    rowProp.setSize(0);
    for (int i = 0; i < indexlen; i++) {
      int order = sortindex[i];
      dataVector.add(dataVector1.elementAt(order));
      dataVectorBk.add(dataVectorBk1.elementAt(order));
      rowProp.add(rowProp1.elementAt(order));
      if (getListFieldFlag()) {
        vValue.add(vValue1.elementAt(order));
      }
    }
    super.fireTableChanged(new TableModelEvent(this));
    sortchangedf = false;
  }


} // End of class DefaultTableModel
