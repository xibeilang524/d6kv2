package com.daqo.dbinfo;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.JButton;
import javax.swing.JComboBox;

import javax.swing.DefaultComboBoxModel;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JSeparator;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;
import javax.swing.JTable;
import java.awt.Rectangle;
import java.awt.Color;
import java.awt.Frame;

public class TableDialogFind
    extends JDialog {

  private JComboBox combo_replace;
  private JComboBox combo_find;
  private JRadioButton radio_down;
  private JRadioButton radio_up;
  private JRadioButton radio_all;
  private JRadioButton radio_select;
  public boolean bool = false;
  String find_array[] = {};
  String replace_array[] = {};
  private MyJTable table;
  public int intRowFindStart;
  public int intColFindStart;
  public int intSelectRowFindStart;
  public int intSelectColFindStart;
  public int rows[];
  public int cols[];
  public int intRowEnd;
  public int intColEnd;
  public int intSelectRowEnd;
  public int intSelectColEnd;
/*
  private static TableDialogFind tdFind;

  public static TableDialogFind getInstance() {
    if (tdFind == null)
      tdFind = new TableDialogFind();
    return tdFind;
  }
*/
  public void setTable(MyJTable ta) {
    table = ta;
  }

  /**
   * Create the dialog
   */


  public TableDialogFind(Frame frame, String title, boolean modal) {
  super(frame, title, modal);
    setResizable(false);
    getContentPane().setLayout(null);
    setTitle("查找");
    setBounds(300, 300, 240, 290);

    final JLabel label = new JLabel();
    label.setText("查找:");
    label.setBounds(10, 10, 37, 15);
    getContentPane().add(label);

    combo_find = new JComboBox();
    combo_find.setEditable(true);
    combo_find.setBounds(53, 6, 161, 23);
    getContentPane().add(combo_find);

    final JLabel label_1 = new JLabel();
    label_1.setText("替换为:");
    label_1.setBounds(10, 46, 60, 15);
    getContentPane().add(label_1);

    combo_replace = new JComboBox();
    combo_replace.setEditable(true);
    combo_replace.setBounds(53, 42, 161, 23);
    getContentPane().add(combo_replace);

    final JPanel panel = new JPanel();
    panel.setLayout(null);
    panel.setBorder(new TitledBorder(null, "方向",
                                     TitledBorder.DEFAULT_JUSTIFICATION,
                                     TitledBorder.DEFAULT_POSITION, null, null));
   // panel.setBounds(10, 89, 73, 83);
  //  getContentPane().add(panel);

    radio_down = new JRadioButton();
    radio_down.addItemListener(new ItemListener() {
      public void itemStateChanged(final ItemEvent e) {
        boolean bool = radio_down.isSelected();
        radio_up.setSelected(!bool);
      }
    });
    radio_down.setBounds(10, 19, 53, 23);
    panel.add(radio_down);
    radio_down.setText("向下");

    radio_up = new JRadioButton();
    radio_up.addItemListener(new ItemListener() {
      public void itemStateChanged(final ItemEvent e) {
        boolean bool = radio_up.isSelected();
        radio_down.setSelected(!bool);

      }
    });
    radio_up.setText("向上");
    radio_up.setBounds(10, 46, 53, 23);
    panel.add(radio_up);

    final JPanel panel_1 = new JPanel();
    panel_1.setLayout(null);
    panel_1.setBorder(new TitledBorder(null, "范围",
                                       TitledBorder.DEFAULT_JUSTIFICATION,
                                       TitledBorder.DEFAULT_POSITION, null, null));
    panel_1.setBounds(10, 89, 205, 83);
    getContentPane().add(panel_1);

    radio_all = new JRadioButton();
    radio_all.addItemListener(new ItemListener() {
      public void itemStateChanged(final ItemEvent e) {
        boolean bool = radio_all.isSelected();
        radio_select.setSelected(!bool);
      }
    });
    radio_all.setText("当前页面");
    radio_all.setBounds(11, 19, 130, 23);
    panel_1.add(radio_all);

    radio_select = new JRadioButton();
    radio_select.addItemListener(new ItemListener() {
      public void itemStateChanged(final ItemEvent e) {
        boolean bool = radio_select.isSelected();
        radio_all.setSelected(!bool);
      }
    });
    radio_select.setText("当前选中范围");
    radio_select.setBounds(10, 46, 130, 23);
    panel_1.add(radio_select);

    final JSeparator separator = new JSeparator();
    separator.setBounds(10, 178, 205, 23);
    getContentPane().add(separator);

    final JButton button = new JButton();
    button.addActionListener(new ActionListener() {
      public void actionPerformed(final ActionEvent e) {
        if (combo_find.getSelectedItem().equals("")) {
          JOptionPane.showMessageDialog(null, "查找内容不能为空", "查找",
                                        JOptionPane.WARNING_MESSAGE);
          combo_find.requestFocus();
        }
        else
          actionForward(e);
      }
    });
    button.setText("查找");
    button.setBounds(10, 191, 100, 25);
    getContentPane().add(button);

    final JButton button_1 = new JButton();
    button_1.addActionListener(new ActionListener() {
      public void actionPerformed(final ActionEvent e) {
        actionForward(e);
      }
    });
    button_1.setText("查找并替换");
    button_1.setBounds(116, 191, 100, 25);
    getContentPane().add(button_1);

    final JButton button_2 = new JButton();
    button_2.addActionListener(new ActionListener() {
      public void actionPerformed(final ActionEvent e) {
        actionForward(e);
      }
    });
    button_2.setText("替换");
    button_2.setBounds(10, 222, 100, 25);
    getContentPane().add(button_2);

    final JButton button_3 = new JButton();
    button_3.addActionListener(new ActionListener() {
      public void actionPerformed(final ActionEvent e) {
        actionForward(e);
      }
    });
    button_3.setText("全部替换");
    button_3.setBounds(116, 222, 100, 25);
    getContentPane().add(button_3);
    //
    init();

  }
  public TableDialogFind() {
     this(null, "", false);
   }
  //初始化
  private void init() {
    combo_find.setModel(new DefaultComboBoxModel(find_array));
    combo_replace.setModel(new DefaultComboBoxModel(replace_array));
    if (!radio_down.isSelected() && !radio_up.isSelected())
      radio_down.setSelected(true);
    if (!radio_select.isSelected() && !radio_all.isSelected())
      radio_all.setSelected(true);
  }

  //执行
  private void actionForward(ActionEvent e) {
    String strCommand = e.getActionCommand();
    if (strCommand.equals("查找") || strCommand.equals("查找并替换") ||
        strCommand.equals("替换") || strCommand.equals("全部替换")) {
      String str = String.valueOf(combo_find.getSelectedItem()).replaceAll(
          "null", "");
      addItem(str, 0);
      String str_replace = String.valueOf(combo_replace.getSelectedItem()).
          replaceAll("null", "");
      addItem(str_replace, 1);
      if (radio_down.isSelected()) {
        //查找
        if (strCommand.equals("查找"))
          findString(str, str_replace, 0);
        if (strCommand.equals("查找并替换"))
          findString(str, str_replace, 1);
        if (strCommand.equals("替换"))
          findString(str, str_replace, 2);
        if (strCommand.equals("全部替换"))
          findString(str, str_replace, 3);
      }
      else {
        //查找
        if (strCommand.equals("查找"))
          findString_back(str, str_replace, 0);
        if (strCommand.equals("查找并替换"))
          findString_back(str, str_replace, 1);
        if (strCommand.equals("替换"))
          findString_back(str, str_replace, 2);
        if (strCommand.equals("全部替换"))
          findString_back(str, str_replace, 3);
      }

    }
  }

  //判断是否已经存在，不存在则加入列表
  private void addItem(String item, int type) {

    if (type == 0 && !item.equals("")) {
      int len = find_array.length;
      if (len == 0) {
        find_array = new String[1];
        find_array[0] = item;
        combo_find.setModel(new DefaultComboBoxModel(find_array));
      }
      else {
        boolean isOk = false;
        for (int i = 0; i < len; i++) {
          if (find_array[i].equals(item)) {
            isOk = true;
            break;
          }
        } //end for
        if (!isOk) {
          String old[] = find_array;
          find_array = new String[len + 1];
          System.arraycopy(old, 0, find_array, 1, len);
          find_array[0] = item;
          combo_find.setModel(new DefaultComboBoxModel(find_array));
        }
      }
    }
    if (type == 1 && !item.equals("")) {
      int len = replace_array.length;
      if (len == 0) {
        replace_array = new String[1];
        replace_array[0] = item;
        combo_replace.setModel(new DefaultComboBoxModel(replace_array));
      }
      else {
        boolean isOk = false;
        for (int i = 0; i < len; i++) {
          if (replace_array[i].equals(item)) {
            isOk = true;
            break;
          }
        } //end for
        if (!isOk) {
          String old[] = replace_array;
          replace_array = new String[len + 1];
          System.arraycopy(old, 0, replace_array, 1, len);
          replace_array[0] = item;
          combo_replace.setModel(new DefaultComboBoxModel(replace_array));
        }
      }
    }
  }

  //过滤特殊字符
  //[ ] \ ^ $ . | ? * + ( )

  private String filterStr(String str) {
    if (str == null)
      str = "";
    else
      str = str.replaceAll("[", "\\[");
    return null;
  }

  //查找
  /**
   * type =0查找 1,查找并替换 2,替换 3,全部替换
   * @param
   * */
  private void findString(String find, String replace, int type) {

    String str;
    Object obj;
    int intFind;
    int intCount = 0;
    int i;
    //选中范围
    if (radio_select.isSelected()) {
      if (type == 0 || type == 1 || type == 2) {
        if (intSelectColFindStart == intSelectColEnd - 1 &&
            intSelectRowFindStart == intSelectRowEnd - 1) {
          intSelectColFindStart = cols[0];
          intSelectRowFindStart = rows[0];
          bool = false;
        }
        if (bool) {
          if (intSelectColFindStart == intSelectColEnd - 1) {
            intSelectRowFindStart++;
            intSelectColFindStart = cols[0];
          }
          else {
            intSelectColFindStart++;
          }
          bool = false;

        }
      }
      else
      {
        intSelectColFindStart = cols[0];
        intSelectRowFindStart = rows[0];
        bool=false;
      }

      /////////////////////////////////////////////////////////////////////
      for (i = intSelectRowFindStart; i < intSelectRowEnd; i++) {
        if (bool)
          break;
        for (int j = intSelectColFindStart; j < intSelectColEnd; j++) {
          obj = table.getValueAt(i, j);
            if (obj != null) {
              str = table.getValueAt(i,
                                     j).toString().trim();
              intFind = str.indexOf(find);

            if (intFind != -1 && !str.equals("")) {
         //     bool = true;
              intCount++;
              table.changeSelection(i, j, false, false);
              if (type == 0) {
                intSelectRowFindStart = i;
                intSelectColFindStart = j;
                bool = true;
                break;
              }
              if (type == 1 || type == 2) {
                str = str.replaceAll(find, replace);
                table.setValueAt(str, i, j);
                bool = true;
                intSelectRowFindStart = i;
                intSelectColFindStart = j;
                break;
              }
              if (type == 3) {
                str = str.replaceAll(find, replace);
                table.setValueAt(str, i, j);
              }
            }
          }
          if (j == intSelectColEnd - 1) {
            intSelectRowFindStart = intSelectRowFindStart +
                1;
            intSelectColFindStart = cols[0];
            if (intSelectRowFindStart == intSelectRowEnd) {
              intSelectRowFindStart = rows[0];
            }
          }

        }
      }

    }
    else {
      //整表范围
      if (type == 0 || type == 1 || type == 2) {
        if (intColFindStart == intColEnd - 1 &&
            intRowFindStart == intRowEnd - 1) {
          intColFindStart = 0;
          intRowFindStart = 0;
          bool = false;
        }
        if (bool) {
          if (intColFindStart == intColEnd - 1) {
            intRowFindStart++;
            intColFindStart = 0;
          }
          else {
            intColFindStart++;
          }
          bool = false;

        }
      }
      else{
        intColFindStart = 0;
        intRowFindStart = 0;
        bool=false;
      }

      for (i = intRowFindStart; i < intRowEnd; i++) {
        if (bool)
          break;
        for (int j = intColFindStart; j < intColEnd; j++) {

          obj = table.getValueAt(i, j);
          if (obj != null) {
            str = table.getValueAt(i,
                                   j).toString().trim();
            intFind = str.indexOf(find);
            if (intFind != -1 && !str.equals("")) {
              intCount++;
              table.changeSelection(i, j, false, false);

              if (type == 0) {
                intRowFindStart = i;
                intColFindStart = j;
                bool = true;
                break;
              }
              if (type == 1 || type == 2) {
                str = str.replaceAll(find, replace);
                table.setValueAt(str, i, j);
                bool = true;
                intRowFindStart = i;
                intColFindStart = j;
                break;
              }
              if (type == 3) {
                str = str.replaceAll(find, replace);
                table.setValueAt(str, i, j);
              }
            }
          }
          if (j == intColEnd - 1) {
            intRowFindStart = intRowFindStart +
                1;
            intColFindStart = 0;
            if (intRowFindStart == intRowEnd) {
              intRowFindStart = 0;
            }
          }
        }
      }
    }
  }

  //查找
  /**
   * type =0查找 1,查找并替换 2,替换 3,全部替换
   * @param
   * */
  private void findString_back(String find, String replace, int type) {

    int rows[];
    int cols[];

    String str;
    Object obj;
    int intFind;
    int intCount = 0;
    boolean bool = false;
    int i;
    //选中范围
    if (radio_select.isSelected()) {

      int intRowEnd = table.getSelectedRowCount();
      int intColEnd = table.getSelectedColumnCount();

      if (intColFindStart == 0 && intRowFindStart == 1) {
        intColFindStart = intColEnd;
        intRowFindStart = intRowEnd;
      }

      rows = table.getSelectedRows();
      cols = table.getSelectedColumns();

      if (type == 0) {
        if (intColFindStart == 1) {
          intRowFindStart--;
          intColFindStart = intColEnd;
        }
        intColFindStart--;
      }

      for (i = intRowFindStart; i >= 1; i--) {
        if (bool)
          break;
        for (int j = intColFindStart; j >= 1; j--) {
          obj = (Object) table.getValueAt(rows[i], cols[j]);
          if (obj instanceof String) {
            str = (String) obj;
            intFind = str.indexOf(find);
            if (intFind != -1 && !str.equals("")) {
              bool = true;
              intCount++;
              table.changeSelection(rows[i], cols[j], false, false);
              if (type == 0) {
                intRowFindStart = rows[i];
                intColFindStart = cols[j];
                bool = true;
                break;
              }
              if (type == 1 || type == 2) {
                str = str.replaceAll(find, replace);
                table.setValueAt(str, rows[i], cols[j]);
                bool = true;
                intRowFindStart = rows[i];
                intColFindStart = cols[j];
                break;
              }
              if (type == 3) {
                str = str.replaceAll(find, replace);
                table.setValueAt(str, rows[i], cols[j]);
              }
            }
          }
        }
      }
      if (i == 1) {
        String msg = "";
        if (intCount == 0)
          msg = "PWTable 找不到匹配项";
        else
          msg = "已完成文档搜索，共找到[" + intCount + "]个匹配项。";

        JOptionPane.showMessageDialog(null, msg, "查找",
                                      JOptionPane.WARNING_MESSAGE);
      }
    }
    else {

      int intRowEnd = table.getRowIntStart();
      int intColEnd = table.getColIntStart();

      if (intColFindStart == 0 && intRowFindStart == 1) {
        intColFindStart = intColEnd;
        intRowFindStart = intRowEnd;
      }

      //整表范围
      if (type == 0) {
        if (intColFindStart == 1) {
          intRowFindStart--;
          intColFindStart = intColEnd;
        }
        intColFindStart--;
      }

      for (i = intRowFindStart; i >= 1; i--) {
        if (bool)
          break;
        for (int j = intColFindStart; j >= 1; j--) {
          obj = (Object) table.getValueAt(i, j);
          if (obj instanceof String) {
            str = (String) obj;
            intFind = str.indexOf(find);
            if (intFind != -1 && !str.equals("")) {
              bool = true;
              intCount++;
              table.changeSelection(i, j, false, false);
              if (type == 0) {
                intRowFindStart = i;
                intColFindStart = j;
                bool = true;
                break;
              }
              if (type == 1 || type == 2) {
                str = str.replaceAll(find, replace);
                table.setValueAt(str, i, j);
                bool = true;
                intRowFindStart = i;
                intColFindStart = j;
                break;
              }
              if (type == 3) {
                str = str.replaceAll(find, replace);
                table.setValueAt(str, i, j);
              }
            }
          }
        }
      }
      if (i == 1) {
        String msg = "";
        if (intCount == 0)
          msg = "PWTable 找不到匹配项";
        else
          msg = "已完成文档搜索，共找到[" + intCount + "]个匹配项。";

        JOptionPane.showMessageDialog(null, msg, "查找",
                                      JOptionPane.WARNING_MESSAGE);
      }
    }
  }

}
