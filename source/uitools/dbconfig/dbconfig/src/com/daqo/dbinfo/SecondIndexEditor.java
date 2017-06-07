package com.daqo.dbinfo;

import javax.swing.DefaultCellEditor;
import javax.swing.*;
import java.awt.Component;
import java.awt.event.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class SecondIndexEditor
    extends DefaultCellEditor {
  Object currentString;

  public SecondIndexEditor(JButton b) {
    super(new JCheckBox()); //Unfortunately, the constructor
    //expects a check box, combo box,
    //or text field.
    editorComponent = b;
    setClickCountToStart(2); //This is usually 1 or 2.
    //Must do this so that editing stops when appropriate.
    b.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        fireEditingStopped();
      }
    });
  }

  protected void fireEditingStopped() {
    super.fireEditingStopped();
  }

  public Object getCellEditorValue() {
    return currentString;
  }

  public Component getTableCellEditorComponent(JTable table,
                                               Object value,
                                               boolean isSelected,
                                               int row,
                                               int column) {
    if (value == null) {
      ( (JButton) editorComponent).setText("");
    }
    else {
      ( (JButton) editorComponent).setText(value.toString());
    }
    currentString = value;
    return editorComponent;
  }
}
