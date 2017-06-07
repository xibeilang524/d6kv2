package com.daqo.dbinfo;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class FrameDB_AboutBox
    extends JDialog
    implements ActionListener {

  JPanel panel1 = new JPanel();
  JPanel panel2 = new JPanel();
  JPanel insetsPanel1 = new JPanel();
  JPanel insetsPanel2 = new JPanel();
  JPanel insetsPanel3 = new JPanel();
  JButton button1 = new JButton();
  JLabel imageLabel = new JLabel();
  JLabel label1 = new JLabel();
  JLabel label2 = new JLabel();
  JLabel label3 = new JLabel();
  JLabel label4 = new JLabel();
  BorderLayout borderLayout1 = new BorderLayout();
  BorderLayout borderLayout2 = new BorderLayout();
  FlowLayout flowLayout1 = new FlowLayout();
  GridLayout gridLayout1 = new GridLayout();
  String product = "\u6570\u636e\u5e93\u7ef4\u62a4\u5de5\u5177";
  String version = "1.0";
  String copyright = "Copyright (c) 2010";
  String comments = "\u6570\u636e\u5e93\u7ef4\u62a4\u5de5\u5177";
  JLabel jLabel1 = new JLabel();
  private JTextArea jTextArea1 = new JTextArea();
  public FrameDB_AboutBox(Frame parent) {
    super(parent);
    enableEvents(AWTEvent.WINDOW_EVENT_MASK);
    try {
      jbInit();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
    pack();
  }

  //Component initialization
  private void jbInit() throws Exception {
    //imageLabel.setIcon(new ImageIcon(FrameDB_AboutBox.class.getResource("[Your Image]")));
    this.setTitle("About");
    setResizable(false);
    panel1.setLayout(borderLayout1);
    panel2.setLayout(borderLayout2);
    insetsPanel1.setLayout(flowLayout1);
    insetsPanel2.setLayout(flowLayout1);
    insetsPanel2.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
    gridLayout1.setRows(4);
    gridLayout1.setColumns(1);
    label1.setText(product);
    label2.setToolTipText("");
    label2.setText("1.0.0.0版");
    label3.setText("Copyright (c) 2010");
    label4.setToolTipText("");
    label4.setText("数据库维护工具");
    insetsPanel3.setLayout(gridLayout1);
    insetsPanel3.setBorder(BorderFactory.createEmptyBorder(10, 60, 10, 10));
    button1.setText("确定");
    button1.addActionListener(this);
    jLabel1.setToolTipText("");
    jLabel1.setText("系统属性");
    jTextArea1.setText("版本特征：代码自动生成；SCD设备参数表的devid自动生成。应用类型显示能排序。");
    insetsPanel2.add(imageLabel, null);
    panel2.add(insetsPanel3, BorderLayout.CENTER);
    panel2.add(insetsPanel2, BorderLayout.WEST);
    this.getContentPane().add(panel1, null);
    insetsPanel3.add(label1, null);
    insetsPanel3.add(label2, null);
    insetsPanel3.add(label3, null);
    insetsPanel3.add(label4, null);
    insetsPanel3.add(jLabel1, null);
    panel1.add(jTextArea1, BorderLayout.CENTER);
    insetsPanel1.add(button1, null);
    panel1.add(insetsPanel1, BorderLayout.SOUTH);
    panel1.add(panel2, BorderLayout.NORTH);
  }

  //Overridden so we can exit when window is closed
  protected void processWindowEvent(WindowEvent e) {
    if (e.getID() == WindowEvent.WINDOW_CLOSING) {
      cancel();
    }
    super.processWindowEvent(e);
  }

  //Close the dialog
  void cancel() {
    dispose();
  }

  //Close the dialog on a button event
  public void actionPerformed(ActionEvent e) {
    if (e.getSource() == button1) {
      cancel();
    }
  }
}
