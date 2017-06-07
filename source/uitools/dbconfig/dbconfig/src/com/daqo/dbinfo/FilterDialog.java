package com.daqo.dbinfo;

import javax.swing.JDialog;
import com.borland.jbcl.layout.XYLayout;
import com.borland.jbcl.layout.*;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JTextArea;
import javax.swing.JComboBox;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.JLabel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Frame;
import javax.swing.tree.DefaultMutableTreeNode;

import com.daqo.dbinfo.table.FieldProp;
import java.awt.Color;
import javax.swing.UIManager;
import java.util.Vector;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class FilterDialog
    extends JDialog {

  public String flwhere;
  public int fllevel;
  Dlllink pdll = Dlllink.getInstance();
  String m_db = "modeldb";
  String m_tablename;
  TableSetInfo tablesetinfo;
  XYLayout xYLayout1 = new XYLayout();
  JButton jBtOK = new JButton();
  JButton jBtcancle = new JButton();
  JTextArea textarea = new JTextArea();
  JComboBox comboxfield = new JComboBox();
  JComboBox comboxoper = new JComboBox();
  JTextField textvalue = new JTextField();
  JLabel jLabel1 = new JLabel();
  JLabel jLabel2 = new JLabel();
  JLabel jLabel3 = new JLabel();
  JButton jBtoper = new JButton();
  JButton jButton1 = new JButton();
  private void jbInit() throws Exception {
    this.getContentPane().setLayout(xYLayout1);
    jBtOK.setText("确定");
    jBtOK.addActionListener(new FilterDialog_jBtOK_actionAdapter(this));
    jBtoper.setText("添加规则");
    jBtoper.addActionListener(new FilterDialog_jBtoper_actionAdapter(this));
    jLabel3.setText("值：");
    jLabel2.setText("操作符：");
    jLabel1.setText("域名：");
    jBtcancle.setText("取消");
    jBtcancle.addActionListener(new FilterDialog_jBtcancle_actionAdapter(this));
    xYLayout1.setWidth(286);
    xYLayout1.setHeight(323);
    this.setForeground(Color.pink);
    textarea.setBackground(UIManager.getColor(
        "InternalFrame.inactiveTitleForeground"));
    textarea.setForeground(Color.red);
    textarea.setBackground(Color.white);
    comboxfield.addActionListener(new FilterDialog_comboxfield_actionAdapter(this));
    jButton1.setText("多重规则");
    jButton1.addActionListener(new FilterDialog_jButton1_actionAdapter(this));
    this.getContentPane().add(jLabel2, new XYConstraints(23, 48, 54, 23));
    this.getContentPane().add(jLabel1, new XYConstraints(23, 12, 43, 24));
    this.getContentPane().add(jLabel3, new XYConstraints(23, 81, 48, 22));
    this.getContentPane().add(comboxfield, new XYConstraints(80, 12, 170, 24));
    this.getContentPane().add(textvalue, new XYConstraints(80, 81, 169, 23));
    this.getContentPane().add(comboxoper, new XYConstraints(81, 47, 168, 24));
    this.getContentPane().add(jBtOK, new XYConstraints(28, 281, 91, 27));
    this.getContentPane().add(jBtcancle, new XYConstraints(159, 281, 96, 26));
    this.getContentPane().add(textarea, new XYConstraints(27, 164, 228, 111));
    this.getContentPane().add(jBtoper, new XYConstraints(27, 122, 87, -1));
    this.getContentPane().add(jButton1, new XYConstraints(157, 122, -1, -1));
    textarea.setLineWrap(true);
    fllevel= ApplicationDB.frameDB.flnode.getLevel();
    if (fllevel < 3) {
      m_tablename = ApplicationDB.frameDB.flnode.toString();
      setTitle(m_tablename);
    }
    if (fllevel == 3){
      m_tablename = ApplicationDB.frameDB.flnode.getParent().toString();
      setTitle(m_tablename + "/" + ApplicationDB.frameDB.flnode.toString());
    }
    if (fllevel == 4)
    {
      m_tablename = ApplicationDB.frameDB.flnode.getParent().getParent().
          toString();
       setTitle(m_tablename + "/" + ApplicationDB.frameDB.flnode.getParent().toString()+ "/"+ ApplicationDB.frameDB.flnode.toString());
    }
    //setTitle(m_tablename);
    tablesetinfo = new TableSetInfo();
    tablesetinfo.tableFieldInfo = new FieldProp(m_db.trim(), m_tablename.trim());
    tablesetinfo.tableFieldInfo.getTableInfo();
    int fieldnums = tablesetinfo.tableFieldInfo.fieldnums;
    for (int cols = 0; cols < fieldnums; cols++) {
      comboxfield.insertItemAt(tablesetinfo.tableFieldInfo.getTableFieldName(
          cols), cols);
    }

    comboxfield.setSelectedIndex(0);
  }

  public FilterDialog(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      setResizable(false);
      setBounds(300, 300, 300, 350);

      jbInit();
    }
    catch (Exception ex) {
      ex.printStackTrace();
    }

  }

  public FilterDialog() {
    this(null, "", false);

  }

  public void jBtOK_actionPerformed(ActionEvent e) {
    String tempstr;
    int level = ApplicationDB.frameDB.flnode.getLevel();
      if (ApplicationDB.frameDB.flnode.isLeaf() || ApplicationDB.frameDB.flnode.getLevel()==2) { //叶子节点的刷新是:判断保存，保存错误，将不刷新
        boolean bExpandNodes = true;
        bExpandNodes = ApplicationDB.frameDB.removeTreeLeafNode(ApplicationDB.
            frameDB.flnode, true);
        if (bExpandNodes) {

          ApplicationDB.frameDB.jTree1.flwhere = textarea.getText();
          ApplicationDB.frameDB.jTree1.vSelectFlag = true;
          ApplicationDB.frameDB.jTree1.addBranchNodes(ApplicationDB.frameDB.
              flnode,
              ApplicationDB.frameDB.
              flnode.toString(), level);
          ApplicationDB.frameDB.jTree1.vSelectFlag = false;
     //     tempstr=ApplicationDB.frameDB.jnodeinfo.getText();
      //    if(textarea.getText().equals(null)==false && textarea.getText().equals("")==false){
         //   ApplicationDB.frameDB.jnodeinfo.setText(tempstr + " [筛选条件 " +
         //       textarea.getText() + "]");
       //   }
          ApplicationDB.frameDB.setRecordNums();
        }
      }
      dispose();
  }

  public void jBtcancle_actionPerformed(ActionEvent e) {
    dispose();
  }

  public void jBtoper_actionPerformed(ActionEvent e) {
    String colname = comboxfield.getSelectedItem().toString();
    String oper = comboxoper.getSelectedItem().toString();
    int operid = comboxfield.getSelectedIndex();
    String type = pdll.substr(tablesetinfo.tableFieldInfo.tableinfo[operid *
                              3 + 1]);
    int itype = Integer.parseInt(type);
    switch (itype) {
      case ConstantDef.CS_CHAR_TYPE:
      case ConstantDef.CS_VARCHAR_TYPE: {
        String where = colname + " " + oper + " '%" + textvalue.getText() +
            "%'";
        textarea.setText(where);
        break;
      }

      default: {
        String where = colname + " " + oper + " " + textvalue.getText();
        textarea.setText(where);
        break;
      }

    }
  }

  public void comboxfield_actionPerformed(ActionEvent e) {
    int operid = comboxfield.getSelectedIndex();
    String type = pdll.substr(tablesetinfo.tableFieldInfo.tableinfo[operid *
                              3 + 1]);
    int itype = Integer.parseInt(type);
    switch (itype) {
      case ConstantDef.CS_CHAR_TYPE:
      case ConstantDef.CS_VARCHAR_TYPE: {
        comboxoper.removeAllItems();
        comboxoper.insertItemAt("like", 0);
        comboxoper.setSelectedIndex(0);
        break;
      }

      default: {
        comboxoper.removeAllItems();
        comboxoper.insertItemAt("=", 0);
        comboxoper.insertItemAt(">", 1);
        comboxoper.insertItemAt("<", 2);
        comboxoper.insertItemAt("<>", 3);
        comboxoper.setSelectedIndex(0);
        break;
      }
    }
  }

  public void jButton1_actionPerformed(ActionEvent e) {
    String where;
    if (textarea.getText().equals(null)==false && textarea.getText().equals("")==false) {
      where = textarea.getText();
      String colname = comboxfield.getSelectedItem().toString();
      String oper = comboxoper.getSelectedItem().toString();
      int operid = comboxfield.getSelectedIndex();
      String type = pdll.substr(tablesetinfo.tableFieldInfo.tableinfo[operid *
                                3 + 1]);
      int itype = Integer.parseInt(type);
      switch (itype) {
        case ConstantDef.CS_CHAR_TYPE:
        case ConstantDef.CS_VARCHAR_TYPE: {
          where = where +" and " + colname + " " + oper + " '%" + textvalue.getText() +
              "%'";
          textarea.setText(where);
          break;
        }

        default: {
         where = where +" and " + colname + " " + oper + " " + textvalue.getText();
          textarea.setText(where);
          break;
        }

      }

    }
  }
}

class FilterDialog_jButton1_actionAdapter
    implements ActionListener {
  private FilterDialog adaptee;
  FilterDialog_jButton1_actionAdapter(FilterDialog adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton1_actionPerformed(e);
  }
}

class FilterDialog_comboxfield_actionAdapter
    implements ActionListener {
  private FilterDialog adaptee;
  FilterDialog_comboxfield_actionAdapter(FilterDialog adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.comboxfield_actionPerformed(e);
  }
}

class FilterDialog_jBtoper_actionAdapter
    implements ActionListener {
  private FilterDialog adaptee;
  FilterDialog_jBtoper_actionAdapter(FilterDialog adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jBtoper_actionPerformed(e);
  }
}

class FilterDialog_jBtcancle_actionAdapter
    implements ActionListener {
  private FilterDialog adaptee;
  FilterDialog_jBtcancle_actionAdapter(FilterDialog adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jBtcancle_actionPerformed(e);
  }
}

class FilterDialog_jBtOK_actionAdapter
    implements ActionListener {
  private FilterDialog adaptee;
  FilterDialog_jBtOK_actionAdapter(FilterDialog adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jBtOK_actionPerformed(e);
  }
}
